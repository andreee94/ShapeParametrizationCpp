#include "Point.h"

Point::Point(double px, double py): x(px), y(py)
{
}

Point::~Point()
{
    //dtor
}

Point::Point(const Point& other)
{
    //copy ctor
}

Point& Point::operator=(const Point& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
