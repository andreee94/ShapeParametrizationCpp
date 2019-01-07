#include "Point.h"
#include "Line.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <Utils.h>
#include <pystring.h>
#include <sstream>
using namespace std;

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;
typedef std::vector<string> strings;

//Point::Point(double px, double py): x(px), y(py)
//{
//}
using namespace std;

Point::Point(): x(0), y(0){}

Point::Point(int value): x(value), y(value){}

Point::Point(double value): x(value), y(value){}

Point::Point(int xx, int yy): x(xx), y(yy){}

Point::Point(double xx, double yy): x(xx), y(yy){}


Point::Point(string str)
{
    strings ss;
    pystring::split(str, ss, ",");
    this->x = std::stod(ss[0]);
    this->y = std::stod(ss[1]);
}

Point::~Point()
{
    //dtor
}

// copy constructor
Point::Point(const Point& other)
{
    this->x = other.x;
    this->y = other.y;
}

Point& Point::operator=(const Point& other)
{
    if (this == &other) return *this; // handle self assignment
    this->x = other.x;
    this->y = other.y;
    return *this;
}

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
Point Point::rotate_self(double theta)
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
Point Point::reverse_self()
{
    this->x = -this->x;
    this->y = -this->y;
    return *this;
}
Point Point::normalize() const
{
    return *this / this->length();
}
Point Point::normalize_self()
{
    double len = this->length();
    this->x = this->x / len;
    this->y = this->y / len;
    return *this;
}

Point Point::normal() const
{
    return Point(-this->y, this->x);
}
Point Point::normal_self()
{
    double tempx = this->x;
    this->x = -this->y;
    this->y = tempx;
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

double Point::slope() const
{
    return this->y / this->x;
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

Point Point::move_self(const Line& line, double dist)
{
    this->move_self(line.getdirection(), dist);
    return *this;
}

Point Point::move_self(const Point& direction, double dist)
{
    *this = *this + direction.normalize() * dist;
    return *this;
}

void Point::print() const
{
    std::cout << "x = " << this->x
              << " y = " << this->y << std::endl;
}

string Point::to_str() const
{
    std::ostringstream strs;
    strs << this->x;
    strs << ",";
    strs << this->y;
    return strs.str();
}

//operator Point::string() const
//{
//    return this->to_str();
//}

void Point::normalize(Points &points)
{
    for (Point p : points)
        p.normalize();
}
void Point::normals(Points &points)
{
    for (Point p : points)
        p.normal();
}


//get method
double Point::getx() const
{
    return this->x;
}

//get method
double Point::gety() const
{
    return this->y;
}

void Point::setx(double x)
{
    this->x = x;
}

//get method
void Point::sety(double y)
{
    this->y = y;
}


Points Point::fromDoubles(doubles &x, doubles &y)
{
    Points points;
    points.reserve(x.size());
    for (int i = 0; i < x.size(); i++)
    {
        points.push_back(Point(x[i], y[i]));
    }
    return points;
}

void Point::toDoubles(const Points& points, doubles &x, doubles &y)
{
    x.clear();
    y.clear();
    x.reserve(points.size());
    y.reserve(points.size());
    for (int i = 0; i < points.size(); i++)
    {
        x.push_back(points[i].x);
        y.push_back(points[i].y);
    }
}

//std::ostream & Point::operator<<(std::ostream & Str) {
//    // print something from v to str, e.g: Str << v.getX();
//    std::ostringstream strs;
//    strs << this->x;
//    strs << ",";
//    strs << this->y;
//    return strs;
//}
//
//std::ostream & operator<<(std::ostream & Str, Point const & v) {
//    // print something from v to str, e.g: Str << v.getX();
//    std::ostringstream strs;
//    strs << v.x;
//    strs << ",";
//    strs << v.y;
//    return strs;
//}

