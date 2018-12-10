#include "Line.h"
#include "Point.h"

Line::Line(double y_intercept, double slope)
{
    m = y_intercept;
    q = slope;
}

Line::Line(const Point& p, double slope)
{
    m = slope;
    q = p.y - slope * p.x; // yp - m * xp
}

Line::Line(const Point& p1, const Point& p2)
{
    double slope = (p2.y - p1.y) / (p2.x - p1.x);
    Line:Line(p1, slope);
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

double Line::getm()
{
    return m;
}

double Line::getq()
{
    return q;
}
