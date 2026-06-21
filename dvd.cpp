#include <iostream>
#include <ostream>

#include "andray.h"

class DvdObject : public andray::IObject {
public:
    DvdObject() :
        IObject("dvd", 200, 200) {
    }
    void draw() override {
        DrawTexture(*texture, x, y , WHITE);
    }
};

class DvdBehavior : public andray::IBehavior {
public:
    void onStart() {
        app->loadTexture("dvd", "assets/textures/dvd.png");
        app->addObject(new DvdObject());
    }
    void onUpdate() {
    }
    void onRender() {
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