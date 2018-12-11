#include "Line.h"
#include "Point.h"

Line::Line(double y_intercept, double slope)
{
    m = y_intercept;
    q = slope;
}

//constructor
Line::Line(const Point& p, double slope)
{
    m = slope;
    q = p.y - slope * p.x; // yp - m * xp
}

//constructor
Line::Line(const Point& p1, const Point& p2)
{
    //double slope = (p2.y - p1.y) / (p2.x - p1.x);
    Line:Line(p1, p1.slope(p2));
}

Line::~Line()
{
    //dtor
}

Line& Line::operator=(const Line& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}


Line Line::perpendicular(const Point& p) const
{
    return Line(p, -1.0 / this->m);
}


Point Line::getdirection() const
{
    return Point(this->m, 1.0).normalize();
}

//get method
double Line::getm() const
{
    return m;
}

//get method
double Line::getq() const
{
    return q;
}
