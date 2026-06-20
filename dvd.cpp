#include <iostream>
#include <ostream>

#include "andray.h"

class DvdBehavior : public andray::IBehavior {
    void onStart() {
        std::cout << "onStart" << std::endl;
    }
    void onUpdate() {
    }
    void onRender() {
    }
public:
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