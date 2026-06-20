#include <iostream>
#include <raylib.h>


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

        Window(int w, int h) : window_width(w), window_height(h) {}

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
    Assets assets;
    AudioManager audioManager;
    Window window;
public:
};