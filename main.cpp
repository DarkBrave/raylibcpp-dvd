#include <iostream>
#include <raylib.h>
#include <vector>

class DvdWindow {
public:
    static inline int window_width = 0;
    static inline int window_height = 0;
    static inline bool showDebug = false;

    static void init(const int w, const int h)  {
        window_width = w;
        window_height = h;
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(window_width, window_height, "DVD");
    }
    static float getFrameTime() {
        return GetFrameTime();
    }
    static void update() {
        window_width = GetScreenWidth();
        window_height = GetScreenHeight();
    }
};

class DvdAudio {
public:
    static inline Music music{};
    static void init() {
        InitAudioDevice();
        music = LoadMusicStream("assets/music.ogg");
        PlayMusicStream(music);
    }
    static void update() {
        UpdateMusicStream(music);
    }
    static void end() {
        UnloadMusicStream(music);
        CloseAudioDevice();
    }
};

class DvdLogo {
private:
    void updateColor() {
        if (currentHue + hueJump >= 360) {
            currentHue = (currentHue + hueJump) - 360;
        } else {
            currentHue += hueJump;
        }
        color = ColorFromHSV(static_cast<float>(currentHue), 1.0f, 1.0f);
    }
public:
    static Texture2D dvdSprite;
    static Rectangle dvdSource;

    static void init() {
        dvdSprite = LoadTexture("assets/dvd.png");
        dvdSource = {
            0.0f,
            0.0f,
            (float)dvdSprite.width,
            (float)dvdSprite.height
        };
    }

    float velocityX;
    float velocityY;
    float posX;
    float posY;
    float sizeX;
    float sizeY;

    int hueJump = 30;
    Color color;
    int currentHue;

    DvdLogo(float vX, float vY, float sX, float sY) {
        velocityX = vX;
        velocityY = vY;
        sizeX = sX;
        sizeY = sY;
        posX = GetRandomValue(0, DvdWindow::window_width-sizeX);
        posY = GetRandomValue(0, DvdWindow::window_height-sizeY);
        currentHue = 0;
        color = ColorFromHSV(static_cast<float>(currentHue), 1.0f, 1.0f);
    }

    void processPhysics() {
        const float dt = DvdWindow::getFrameTime();
        posX += velocityX * dt;
        posY += velocityY * dt;

        if (posX <= 0) {
            posX = 0;
            velocityX = -velocityX;
            updateColor();
        }
        if (posX >= static_cast<float>(DvdWindow::window_width) - sizeX) {
            posX = static_cast<float>(DvdWindow::window_width) - sizeX;
            velocityX = -velocityX;
            updateColor();
        }
        if (posY <= 0) {
            posY = 0;
            velocityY = -velocityY;
            updateColor();
        }
        if (posY >= static_cast<float>(DvdWindow::window_height) - sizeY) {
            posY = static_cast<float>(DvdWindow::window_height) - sizeY;
            velocityY = -velocityY;
            updateColor();
        }
    }
    void draw() {
        Rectangle dvdLogo = {
            posX,
            posY,
            sizeX,
            sizeY
        };
        DrawTexturePro(dvdSprite, dvdSource, dvdLogo, (Vector2){0.0f, 0.0f}, 0.0f, color);
    }
};
Texture2D DvdLogo::dvdSprite;
Rectangle DvdLogo::dvdSource;

int main() {
    bool spamDvd{false};
    DvdWindow::init(900, 600);
    DvdAudio::init();
    DvdLogo::init();
    std::vector<DvdLogo> dvds;
    dvds.emplace_back(600,300,150,80);

    while (!WindowShouldClose()) {
        DvdWindow::update();
        DvdAudio::update();
        BeginDrawing();
        ClearBackground(BLACK);
        if (IsKeyPressed(KEY_A)) {
            dvds.emplace_back(
                GetRandomValue(0, 1000),
                GetRandomValue(0, 800),
                150,
                80
            );
        }
        if (IsKeyPressed(KEY_B))
            DvdWindow::showDebug = !DvdWindow::showDebug;
        if (IsKeyPressed(KEY_S))
            spamDvd = !spamDvd;
        if (IsKeyPressed(KEY_C)) {
            dvds.clear();
            dvds.shrink_to_fit();
        }
        for (DvdLogo& dvd : dvds) {
            dvd.processPhysics();
            dvd.draw();
        }
        if (DvdWindow::showDebug == true)
            DrawText(TextFormat("DVDs: %d, %dfps", dvds.size(), GetFPS()),20,10,20,WHITE);
        if (spamDvd)
            dvds.emplace_back(
                GetRandomValue(0, 1000),
                GetRandomValue(0, 800),
                150,
                80
            );
        EndDrawing();
    }
    DvdAudio::end();
    CloseWindow();
}