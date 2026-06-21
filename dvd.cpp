#include "andray.h"
#include <raylib.h>

class DvdObject : public andray::IObject {
public:
    float velocityX{}, velocityY{};
    float sizeX{}, sizeY{};
    DvdObject(const float vX, const float vY, const float sX, const float sY) :
        IObject("dvd", 200, 200),
        velocityX(vX), velocityY(vY), sizeX(sX), sizeY(sY)
    {}
    void onStart() override {
        app->loadTexture("dvd", "assets/textures/dvd.png");
        auto generateRandomPosition = [](int max) -> float {
            return static_cast<float>(GetRandomValue(0, max));
        };
        x = generateRandomPosition(app->getWindowWidth() - sizeX);
        y = generateRandomPosition(app->getWindowHeight() - sizeY);
    }
    void onRender() override {
        DrawTexture(texture->get(), x, y , WHITE);
    }
};

class DvdBehavior : public andray::IBehavior {
public:
    void onStart() override {
        app->loadTexture("dvd", "assets/textures/dvd.png");
        app->addObject(new DvdObject(10,10,10,10));
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