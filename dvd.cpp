#include <iostream>
#include <ostream>

#include "andray.h"

class DvdObject : public andray::IObject {
public:
    int x{200}, y{200};
    std::string textureName{"dvd"};
};

class DvdBehavior : public andray::IBehavior {
public:
    void onStart() {
        std::cout << "onStart" << std::endl;
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