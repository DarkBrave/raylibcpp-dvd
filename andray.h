#pragma once

#include <iostream>
#include <raylib.h>
#include <string>
#include <unordered_map>

namespace andray {

class IBehavior {
public:
    std::string name;
    int initW{}, initH{};

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
        AudioManager(std::unordered_map<std::string, Music>& musicAssets,
                     std::unordered_map<std::string, Sound>& soundsAssets)
            : musics(musicAssets), sounds(soundsAssets)
        {
            InitAudioDevice();
        }

        ~AudioManager() {
            CloseAudioDevice();
        }

        void update() const {
            if (!currentMusic.empty()) {
                UpdateMusicStream(musics.at(currentMusic));
            }
        }

        void playMusic(const std::string& music) {
            if (!currentMusic.empty()) {
                StopMusicStream(musics.at(currentMusic));
            }

            currentMusic = music;
            PlayMusicStream(musics.at(music));
        }

        void playSound(const std::string& sound) const {
            PlaySound(sounds.at(sound));
        }
    };

    class Window {
    public:
        int window_width;
        int window_height;
        float frame_time{};

        Window(int w, int h, const std::string& initName)
            : window_width(w), window_height(h)
        {
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
        template <typename Map, typename Func>
        static void unloadAssets(Map& assets, Func function) {
            for (auto& asset : assets) {
                function(asset.second);
            }
        }

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
        }

        void loadMusic(const std::string& name, const std::string& path) {
            musics.emplace(name, LoadMusicStream(path.c_str()));
        }

        void loadSound(const std::string& name, const std::string& path) {
            sounds.emplace(name, LoadSound(path.c_str()));
        }
    };

    IBehavior* behavior;
    Assets assets;
    AudioManager audioManager;
    Window window;

public:
    App(IBehavior* initBehavior)
        : assets(),
          audioManager(assets.musics, assets.sounds),
          behavior(initBehavior),
          window(behavior->initW, behavior->initH, initBehavior->name)
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

} // namespace andray