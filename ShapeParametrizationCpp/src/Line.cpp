#include "Line.h"
#include "Point.h"
#include <cmath>

using namespace std;
typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

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

Point Line::evaluate(double x) const
{
    // x, mx + q
    return Point(x, this->m * x + this->q);
}

// when no start and end specified, use between 0 and 1
Points Line::evaluate(int steps) const
{
    return this->Evaluable::evaluate(0.0, 1.0, steps);
}

double Line::rotateslope(double theta) const
{
    return (this->m * std::cos(theta) + std::sin(theta)) / (std::cos(theta) - this->m * std::sin(theta));
}

Point Line::intersection(const Line& l) const
{
    Point p;
    p.x = - (l.q - this->q) / (l.m - this->m);
    p.y = this->q + this->m * p.x;
    return p;
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
