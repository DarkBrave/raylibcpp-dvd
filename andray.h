#include <iostream>
#include <raylib.h>
#include <string>
#include <unordered_map>


namespace andray {
    class App;

    class ILifecycle {
    public:
        App* app = nullptr;
        virtual void onStart() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
    };

    class IBehavior : public ILifecycle {
    public:
        std::string name;
        int initW{}, initH{};
    };

    class IObject : public ILifecycle {
    public:
        int x{}, y{};
        std::string textureName;
        std::optional<std::reference_wrapper<Texture2D>> texture;
        IObject(const std::string& textureName, const int& x, const int& y) :
            x(x), y(y),
            textureName(textureName)
        {}
        ~IObject() = default;
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
                InitWindow(window_width, window_height, "hi");
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
        class ObjectManager {
        private:
        public:
            std::vector<IObject*> objects;
            void updateObjects() {
                for (auto object : objects) {
                    object->onUpdate();
                }
            }
            void renderObjects(Assets& assets) {
                for (auto& object : objects) {
                    auto workingTexture = assets.textures.find(object->textureName);
                    if (workingTexture == assets.textures.end()) {
                        std::cout << "Missing texture: " << object->textureName << "\n";
                        continue;
                    }
                    object->texture = workingTexture->second;
                    object->onRender();
                }
            }
            void addObject(IObject* object) {
                objects.push_back(object);
                object->onStart();
            }
        };
        IBehavior* behavior;
        Assets assets;
        AudioManager audioManager;
        Window window;
        ObjectManager objectManager;
    public:
        App(IBehavior* initBehavior) :
            assets(),
            objectManager(),
            audioManager(assets.musics, assets.sounds),
            behavior(initBehavior),
            window(behavior->initW, behavior->initH, behavior->name)
        {
            behavior->app = this;
        }
        void loadTexture(const std::string& name, const std::string& path) {
            assets.loadTexture(name, path);
        };
        void addObject(IObject* object) {
            object->app = this;
            objectManager.addObject(object);
        }
        int getWindowWidth() {
            return window.window_width;
        }
        int getWindowHeight() {
            return window.window_height;
        }
        float getFrameTime() {
            return GetFrameTime();
        }
        void run() {
            behavior->onStart();
            while (!WindowShouldClose()) {
                window.update();
                audioManager.update();
                behavior->onUpdate();
                objectManager.updateObjects();

                BeginDrawing();
                ClearBackground(BLACK);
                behavior->onRender();
                objectManager.renderObjects(assets);
                EndDrawing();
            }
        }
    };
}