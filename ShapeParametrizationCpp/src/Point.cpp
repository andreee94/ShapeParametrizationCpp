#include "Point.h"
#include "Line.h"
#include <cmath>
#include <iostream>

//Point::Point(double px, double py): x(px), y(py)
//{
//}
using namespace std;

Point::Point()
{
    Point(0.0, 0.0);
}

Point::Point(int value)
{
    this->x = value;
    this->y = value;
}

Point::Point(double value)
{
    this->x = value;
    this->y = value;
}

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

Point::~Point()
{
    //dtor
}

Point::Point(const Point& other)
{
    //copy ctor
}

//Point& Point::operator=(const Point& rhs)
//{
//    if (this == &rhs) return *this; // handle self assignment
//    //assignment operator
//    return *this;
//}

// OPERATOR plus
Point operator+(const Point& a, const Point& b)
{
    return Point(a.x + b.x, a.y + b.y);
}
// OPERATOR minus
Point operator-(const Point& a, const Point& b)
{
    return a + b.reverse();
}
// OPERATOR times point is scalar product
double operator*(const Point& a, const Point& b)
{
    return a.x * b.x + a.y * b.y;
}
// OPERATOR times
Point operator*(const Point& a, const double b)
{
    return Point(b * a.x, b * a.y);
}
Point operator*(const double b, const Point& a)
{
    return a * b;
}
Point operator*(const Point& a, const int b)
{
    return a * (double)b;
}
Point operator*(const int b, const Point& a)
{
    return a * (double)b;
}
// OPERATOR divide
Point operator/(const Point& a, const double b)
{
    return a * (1.0 / b);
}
Point operator/(const Point& a, const int b)
{
    return a * (1.0 / b);
}


// NOT WORKING
//double static slope(const Point& p1, const Point& p2) const
//{
//    return (p2.y - p1.y) / (p2.x - p1.x);
//}

double Point::length() const
{
    return std::sqrt(this->lengthsquared());
}
double Point::lengthsquared() const
{
    return (*this) * (*this); // scalar product with itself
}


double Point::atan() const
{
    return std::atan(this->y / this->x);
}

double Point::atan2() const
{
    return std::atan2(this->y, this->x);
}

Point Point::rotate(double theta) const
{
    Point p;
    // x = x cos(theta) - y sin(theta)
    p.x = *this * Point(std::cos(theta), - std::sin(theta));
    // y = x sin(theta) + y cos(theta)
    p.y = *this * Point(std::sin(theta), std::cos(theta));
    return p;
}
Point Point::rotate(double theta)
{
    // x = x cos(theta) - y sin(theta)
    this->x = *this * Point(std::cos(theta), - std::sin(theta));
    // y = x sin(theta) + y cos(theta)
    this->y = *this * Point(std::sin(theta), std::cos(theta));
    return *this;
}
Point Point::reverse() const
{
    return Point(-this->x, -this->y);
}
Point Point::reverse()
{
    this->x = -this->x;
    this->y = -this->y;
    return *this;
}
Point Point::normalize() const
{
    return *this / this->length();
}
Point Point::normalize()
{
    double len = this->length();
    this->x = this->x / len;
    this->y = this->y / len;
    return *this;
}

double Point::distance(const Point& p2) const
{
    return (*this - p2).length();
}
double Point::distancesquared(const Point& p2) const
{
    return (*this - p2).lengthsquared();
}

double Point::slope(const Point& p2) const
{
    return (p2.y - this->y) / (p2.x - this->x);
}

Line Point::axis(const Point& p2) const
{
    double m, q;
    m = -1.0 / this->slope(p2);
    q = 0.5 * (this->y + p2.y) - 0.5 * (this->x + p2.x) * m;
    return Line(m, q);
}

Point Point::move(const Line& line, double dist) const
{
    return this->move(line.getdirection(), dist);
}

Point Point::move(const Point& direction, double dist) const
{
    return *this + direction.normalize() * dist;
}

void Point::print() const
{
    std::cout << "x = " << this->x
              << " y = " << this->y << std::endl;
}

