#include "LeapControl.hpp"
#include <unistd.h>

#define PRECISION 35

LeapControl::LeapControl() {
    this->controller = Leap::Controller();
    this->connect();
}

LeapControl::~LeapControl() {
}

void
LeapControl::getCoord() {
    Leap::Frame frame = this->controller.frame(); // The latest frame
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


void
LeapControl::connect() {
    sleep(2);
    if(this->controller.isConnected()) {
        std::cout << "Connected" << std::endl;
    } else {
        std::cerr << "Leap Motion not connected" << std::endl;
        exit(-1);
    }
}
