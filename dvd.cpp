#include <iostream>
#include <raylib.h>

class IBehavior {
public:
    std::string name;
    virtual void onStart() {}
    virtual void onUpdate() {}
    virtual void onRender() {}
    virtual ~IBehavior() = default;
};

class App {
private:
    class AudioManager {
    private:
        std::string currentMusic;
        std::unordered_map<std::string, Music>& musics;
        std::unordered_map<std::string, Sound>& sounds;
    public:
        AudioManager(std::unordered_map<std::string, Music>& musicAssets, std::unordered_map<std::string, Sound>& soundsAssets) :
        musics(musicAssets),
        sounds(soundsAssets)
        {
            InitAudioDevice();

        }
        ~AudioManager() {
            CloseAudioDevice();
        }
        void update() const {
            if (!currentMusic.empty())
                UpdateMusicStream(musics.find(currentMusic)->second);
        }
        void playMusic(const std::string& music) {
            if (!currentMusic.empty())
                StopMusicStream(musics.find(currentMusic)->second);
            currentMusic = music;
            PlayMusicStream(musics.find(music)->second);
        }
        void playSound(const std::string& sound) const {
            const Sound& soundToPlay = sounds.find(sound)->second;
            PlaySound(soundToPlay);
        }
    };

    class Window {
    public:
        int window_width;
        int window_height;
        float frame_time{};

        Window(const int w, const int h, const std::string& initName) : window_width(w), window_height(h) {
            InitWindow(window_width, window_height, initName.c_str());
            SetTargetFPS(60);
        }

        void update() {
            window_width = GetScreenWidth();
            window_height = GetScreenHeight();
            frame_time = GetFrameTime();
        }
    };

    class Assets {
    private:
        static void unloadAssets(auto& assets, auto function) {
            for (auto& asset : assets)
                function(asset.second);
        };
    public:
        std::unordered_map<std::string, Texture2D> textures;
        std::unordered_map<std::string, Music> musics;
        std::unordered_map<std::string, Sound> sounds;
        Assets() = default;
        ~Assets() {
            unloadAssets(textures, UnloadTexture);
            unloadAssets(musics, UnloadMusicStream);
            unloadAssets(sounds, UnloadSound);
        }
        void loadTexture(const std::string& name, const std::string& path) {
            textures.emplace(name, LoadTexture(path.c_str()));
        };
        void loadMusic(const std::string& name, const std::string& path) {
            musics.emplace(name, LoadMusicStream(path.c_str()));
        };
        void loadSound(const std::string& name, const std::string& path) {
            sounds.emplace(name, LoadSound(path.c_str()));
        };
    };
    IBehavior* behavior;
    Assets assets;
    AudioManager audioManager;
    Window window;
public:
    App(IBehavior* initBehavior, const int initW, const int initH) :
        assets(),
        audioManager(assets.musics, assets.sounds),
        window(initW, initH, behavior->name),
        behavior(initBehavior)
    {
        behavior->onStart();
    }
    void run() {
        while (!WindowShouldClose()) {
            window.update();
            audioManager.update();
            behavior->onUpdate();

            BeginDrawing();
            ClearBackground(BLACK);
            behavior->onRender();
            EndDrawing();
        }
    }
};

class MyBehavior : public IBehavior {
public:
    std::string name = "Test";
    void onStart() override {
        std::cout << "Started\n";
    }
    void onUpdate() override {
    }
    void onRender() override {
    }
};

int main() {
    MyBehavior behavior;
    App app(&behavior, 100,100);
    app.run();
    return 0;
}