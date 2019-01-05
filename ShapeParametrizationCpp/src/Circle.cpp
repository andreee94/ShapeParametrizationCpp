#include "Circle.h"
#include "Line.h"
#include "Point.h"
#define _USE_MATH_DEFINES
#include <cmath>


using namespace std;
typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

//Circle::Circle()
//{
//    this->center = Point(0, 0);
//    this->radius = 0;
//}

Circle::~Circle()
{
    //dtor
}

Circle::Circle(const Point &center, const double radius)
{
    this->center = center;
    this->radius = radius;
}

Circle::Circle(const Point &p1, const Point &p2, const double m, const bool tangentfirst)
{
    Line axis = p1.axis(p2);
    Line perpendicular = Line(m, 0).perpendicular(tangentfirst ? p1 : p2);
    this->center = axis.intersection(perpendicular);
    this->radius = p1.distance(this->center);
}

//Circle& Circle::operator=(const Circle& rhs)
//{
//    if (this == &rhs) return *this; // handle self assignment
//    //assignment operator
//    return *this;
//}

Point Circle::evaluate(double theta) const
{
    Point p;
    p = this->center + this->radius * Point(cos(theta), sin(theta));
    return p;
}


// when no start and end specified, use between 0 and 2 pi
Points Circle::evaluate(int steps) const
{
    return this->Evaluable::evaluate(0.0, 2 * M_PI, steps);
}


double Circle::getangleofpoint(const Point &p)
{
    Point p_referenceframe = (p - this->center) / this->radius;
    return p_referenceframe.atan2();
}

//get method
Point Circle::getcenter() const
{
    return this->center;
}

//get method
double Circle::getradius() const
{
    return this->radius;
}


void Circle::print() const
{
    cout << "cx = " << this->center.getx()
         << " cy = " << this->center.gety()
         << " r = " << this->radius << endl;
}


