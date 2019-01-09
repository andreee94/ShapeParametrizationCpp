#include "Line.h"
#include "Point.h"
#include <cmath>
#include <iostream>

using namespace std;
typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

Line::Line(double m, double q): m(m), q(q) { }

//constructor
Line::Line(const Point& p, double slope)
{
    m = slope;
    q = p.gety() - slope * p.getx(); // yp - m * xp
}

//constructor
Line::Line(const Point& p1, const Point& p2)
:Line(p1, p1.slope(p2)) { }

// copy constructor
Line::Line(const Line& other)
{
    this->m = other.m;
    this->q = other.q;
}

Line::~Line()
{
    //dtor
}

Line& Line::operator=(const Line& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    this->m = rhs.m;
    this->q = rhs.q;
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
    p.setx(- (l.q - this->q) / (l.m - this->m));
    p.sety(this->q + this->m * p.getx());
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

void Line::print() const
{
    cout << "m = " << this->m
         << " q = " << this->q << endl;
}






