#ifndef LEAP_HPP
# define LEAP_HPP

#include <Leap.h>

class LeapControl
{
public:
    Leap::Controller controller;
    double x;
    double y;
    bool connected;

public:
    LeapControl();
    ~LeapControl();
    void getCoord();
    bool checkLaunch();

private:
    void connect();
    void getCoordX(Leap::Vector);
    void getCoordY(Leap::Vector);
};

#endif
