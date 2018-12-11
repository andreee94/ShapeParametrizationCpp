#include "Point.h"
#include "Line.h"


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


Point operator+(const Point& a, const Point& b)
{
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point& a, const Point& b)
{
    return Point(a.x - b.x, a.y - b.y);
}

// NOT WORKING
//double static slope(const Point& p1, const Point& p2) const
//{
//    return (p2.y - p1.y) / (p2.x - p1.x);
//}

double Point::slope(const Point& p2) const
{
    return (p2.y - this->y) / (p2.x - this->x);
}

Line Point::axis(const Point& p2) const
{
    double m, q;
    m = this->slope(p2);
    q = 0.5 * (this->y + p2.y) - 0.5 * (this->x + p2.x) * m;
    return Line(m, q);
}
