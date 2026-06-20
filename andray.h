#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <raylib.h>

namespace andray {
    class IAndrayBehavior {
    public:
        std::string name;

        virtual void onStart() {}
        virtual void onUpdate() {}
        virtual void onRender() {}

        virtual ~IAndrayBehavior() = default;
    };

    class AndrayApp {
    private:
        class AudioManager {
        private:
            std::string currentMusic;
            std::unordered_map<std::string, Music>& musics;
            std::unordered_map<std::string, Sound>& sounds;

        public:
            AudioManager(std::unordered_map<std::string, Music>& musicAssets,
                         std::unordered_map<std::string, Sound>& soundsAssets);

            ~AudioManager();

            void update() const;
            void playMusic(const std::string& music);
            void playSound(const std::string& sound) const;
        };

        class Window {
        public:
            int window_width;
            int window_height;
            float frame_time{};

            Window(int w, int h, const std::string& initName);

            void update();
        };

        class Assets {
        private:
            template <typename T, typename F>
            static void unloadAssets(std::unordered_map<std::string, T>& assets, F function);

        public:
            std::unordered_map<std::string, Texture2D> textures;
            std::unordered_map<std::string, Music> musics;
            std::unordered_map<std::string, Sound> sounds;

            Assets();
            ~Assets();

            void loadTexture(const std::string& name, const std::string& path);
            void loadMusic(const std::string& name, const std::string& path);
            void loadSound(const std::string& name, const std::string& path);
        };

        IAndrayBehavior* behavior;
        Assets assets;
        AudioManager audioManager;
        Window window;

    public:
        AndrayApp(IAndrayBehavior* initBehavior, int initW, int initH);

        void run();
    };
}