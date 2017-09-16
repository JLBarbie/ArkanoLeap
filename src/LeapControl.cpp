#include "LeapControl.hpp"
#include <unistd.h>

#define PRECISION 35

LeapControl::LeapControl() {
    controller = Leap::Controller();
    controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
    connect();
}

LeapControl::~LeapControl() {
}

void
LeapControl::getCoord() {
    Leap::Frame frame = controller.frame(); // The latest frame
    Leap::Pointable pointable = frame.pointables().frontmost();
    Leap::Vector currentPosition = pointable.tipPosition();

    getCoordX(currentPosition);
    getCoordY(currentPosition);
    // sleep(300);
}

void
LeapControl::getCoordX(Leap::Vector currentPosition) {
    if (currentPosition.x / 180 < -1)
        x = -1;
    else if (currentPosition.x / 180 > 1)
        x = 1;
    else
        x = (currentPosition.x / 180);
}

void
LeapControl::getCoordY(Leap::Vector currentPosition) {
    if ((currentPosition.y - 250) < -PRECISION)
        y = 1;
    else if ((currentPosition.y - 250) > PRECISION)
        y = -1;
    else
        y = 0;
}

bool
LeapControl::checkLaunch() {
    Leap::Frame frame = controller.frame(); // The latest frame
    Leap::GestureList gestures = frame.gestures();
    
    for(Leap::GestureList::const_iterator gl = gestures.begin(); gl != frame.gestures().end(); gl++)
    {
        if ((*gl).type() == Leap::Gesture::TYPE_KEY_TAP) {
            std::cout << "KeyTap" << std::endl;
            return true;
        } else {
            return false;
        }
    }
}

void
LeapControl::connect() {
    sleep(2);
    if(controller.isConnected()) {
        std::cout << "Connected" << std::endl;
        connected = true;
    } else {
        std::cerr << "Leap Motion not connected" << std::endl;
        connected = false;
    }
}
