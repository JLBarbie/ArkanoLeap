#ifndef LEAP_HPP
# define LEAP_HPP

#include <Leap.h>

class LeapControl
{
public:
    Leap::Controller controller;
    double x;
    double y;

public:
    LeapControl();
    ~LeapControl();
    void getCoord();

private:
    void connect();
    void getCoordX(Leap::Vector);
    void getCoordY(Leap::Vector);
};

#endif
