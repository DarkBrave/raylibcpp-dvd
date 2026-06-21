#include "andray.h"
#include <raylib.h>

class DvdObject : public andray::IObject {
public:
    float velocityX{}, velocityY{};
    float sizeX{}, sizeY{};
    int hueJump = {30};
    Color color{ColorFromHSV(static_cast<float>(currentHue), 1.0f, 1.0f)};
    int currentHue{0};
    DvdObject(const float vX, const float vY, const float sX, const float sY) :
        IObject("dvd", 200, 200),
        velocityX(vX), velocityY(vY), sizeX(sX), sizeY(sY)
    {}
    void updateColor() {
        if (currentHue + hueJump >= 360) {
            currentHue = (currentHue + hueJump) - 360;
        } else {
            currentHue += hueJump;
        }
        color = ColorFromHSV(static_cast<float>(currentHue), 1.0f, 1.0f);
    }
    void onStart() override {
        app->loadTexture("dvd", "assets/textures/dvd.png");
        auto generateRandomPosition = [](int max) -> float {
            return static_cast<float>(GetRandomValue(0, max));
        };
        x = generateRandomPosition(app->getWindowWidth() - sizeX);
        y = generateRandomPosition(app->getWindowHeight() - sizeY);
    }
    void onUpdate() override {
        const float dt = app->getFrameTime();
        const int windowWidth = app->getWindowWidth();
        const int windowHeight = app->getWindowHeight();
        x += velocityX * dt;
        y += velocityY * dt;

        if (x <= 0) {
            x = 0;
            velocityX = -velocityX;
            updateColor();
        }
        if (x >= static_cast<float>(windowWidth) - sizeX) {
            x = static_cast<float>(windowWidth) - sizeX;
            velocityX = -velocityX;
            updateColor();
        }
        if (y <= 0) {
            y = 0;
            velocityY = -velocityY;
            updateColor();
        }
        if (y >= static_cast<float>(windowHeight) - sizeY) {
            y = static_cast<float>(windowHeight) - sizeY;
            velocityY = -velocityY;
            updateColor();
        }
    }
    void onRender() override {
        Rectangle dvdSource = {0.0f,0.0f,
            static_cast<float>(texture->get().width), static_cast<float>(texture->get().height)};
        Rectangle dvdLogo = {static_cast<float>(x), static_cast<float>(y), sizeX, sizeY};
        DrawTexturePro(texture->get(), dvdSource, dvdLogo, (Vector2){0.0f, 0.0f}, 0.0f, color);
    }
};

class DvdBehavior : public andray::IBehavior {
public:
    void onStart() override {
        app->loadTexture("dvd", "assets/textures/dvd.png");
        app->addObject(new DvdObject(600,300,150,80));
    }
    void onUpdate() override {
    }
    void onRender() override {
    }
    DvdBehavior() {
        name = "DvdBehavior";
        initW = 800;
        initH = 600;
    }
};

int main() {
    DvdBehavior behavior;
    andray::App app(&behavior);
    app.run();
    return 0;
}