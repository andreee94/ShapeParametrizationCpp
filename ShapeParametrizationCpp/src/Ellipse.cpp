#include "Ellipse.h"
#include "Circle.h"
#include "Line.h"
#include "Point.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <array>

#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

typedef Matrix<double, 3, 1> Vector3;
typedef Matrix<double, 3, 3> Matrix3;

Ellipse::~Ellipse()
{
    //dtor
}

Ellipse::Ellipse(const Point &center, double a, double b, double theta)
{
    this->center = center;
    this->a = a;
    this->b = b;
    this->theta = theta;
    this->ellipsetype = EllipseType::centerframe;
}

Ellipse::Ellipse(const Ellipse &e)
{
    this->center = e.center;
    this->a = e.a;
    this->b = e.b;
    this->theta = e.theta;
    this->A = e.A;
    this->B = e.B;
    this->C = e.C;
    this->D = e.D;
    this->E = e.E;
    this->F = e.F;
    this->ellipsetype = e.ellipsetype;
}


Ellipse::Ellipse(double a, const Line &m1,  const Line &m2)
{
    *this = Ellipse(a, m1.getm(), m2.getm());
}

Ellipse::Ellipse(Matrix3 M)
{
    *this = Ellipse(M(0,0), M(0,1)*2, M(1,1), M(0, 2)*2, M(1, 2) * 2,  M(2,2));
}

Ellipse::Ellipse(double a, double m1, double m2)
{
    // change of variables
    m1 = -1.0 / m1;
    m2 = -1.0 / m2;
    this->A = 1;
    this->B = m1 + m2;
    this->C = 1.0 + 0.5 * (m1 + m2) * (m1 + m2);
    this->D = 0;
    this->E = a * (m1 - m2);
    this->F = - a * a;
    this->ellipsetype = EllipseType::coef;
    //cout << this->A << " " << this->B << " " << this->C << " " << this->D << " " << this->E << " " << this->F << endl;
    //Ellipse(A, B, C, D, E, F);
}

Ellipse::Ellipse(double A, double B, double C, double D, double E, double F)
{
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
    this->E = E;
    this->F = F;
    this->ellipsetype = EllipseType::coef;
}

Ellipse::Ellipse(const Point &p1, const Point &p2, double m1, double m2)
{
    *this = Ellipse(p1, p2, Line(m1, 0), Line(m2, 0));
}

Ellipse::Ellipse(const Point &p1, const Point &p2, const Line &m1,  const Line &m2)
{
    Point center = 0.5 * (p1 + p2);
    double a = 0.5 * p1.distance(p2);
    double theta = (p1 - p2).atan2();
    double tangent1 = m1.rotateslope(-theta);
    double tangent2 = m2.rotateslope(-theta);
    // create ellipse in the easy reference frame
    *this = Ellipse(a, tangent1, tangent2); // SET A,B,C,D,E,F
    cout << this->A << " " << this->B << " " << this->C << " " << this->D << " " << this->E << " " << this->F << endl;
    // restore original frame
    // this->rotate_self(-theta).translate(center);
    this->rotate_self(-theta);
    cout << this->A << " " << this->B << " " << this->C << " " << this->D << " " << this->E << " " << this->F << endl;
    this->translate_self(center);
    cout << this->A << " " << this->B << " " << this->C << " " << this->D << " " << this->E << " " << this->F << endl;
}

Ellipse& Ellipse::operator=(const Ellipse& other)
{
    if (this == &other) return *this; // handle self assignment
    this->A = other.A;
    this->B = other.B;
    this->C = other.C;
    this->D = other.D;
    this->E = other.E;
    this->F = other.F;
    this->ellipsetype = other.ellipsetype;
    this->center = other.center;
    this->a = other.a;
    this->b = other.b;
    this->theta = other.theta;
    return *this;
}

Point Ellipse::evaluate(double alpha) const
{
    Point p;
    p.setx(this->a * cos(this->theta) * cos(alpha) - this->b * sin(this->theta) * sin(alpha));
    p.sety(this->a * sin(this->theta) * cos(alpha) + this->b * cos(this->theta) * sin(alpha));
    return p + this->center;
}


// when no start and end specified, use between 0 and 2 pi
Points Ellipse::evaluate(int steps) const
{
    return this->Evaluable::evaluate(0.0,  2 * M_PI, steps);
}


Ellipse Ellipse::compute_frame() const
{
    Ellipse e = Ellipse(*this);
    e.compute_frame_self();
    return e;
}

Ellipse Ellipse::compute_frame_self()
{
    Ellipse e = Ellipse(*this);
    e.ellipsetype = EllipseType::coef;
    e.center = Point(2 * e.C * e.D - e.B * e.E, 2 * e.A * e.E - e.B * e.D);
    e.center = e.center / (e.B * e.b - 4 * e.A * e.C);
    if (std::abs(e.B) < 1e-15)
        e.theta = e.A <= e.C ? 0 : M_PI / 2;
    else
        e.theta = atan((e.C - e.A - sqrt((e.A - e.C) * (e.A - e.C) + e.B * e.B)) / e.B);
    Ellipse reference_ellipse = Ellipse(e);
    reference_ellipse.translate_self(e.center.reverse());
    reference_ellipse.rotate_self(e.theta);
    e.a = sqrt(- reference_ellipse.F / reference_ellipse.A);
    e.b = sqrt(- reference_ellipse.F / reference_ellipse.C);
    e.ellipsetype = (EllipseType)(e.ellipsetype & EllipseType::centerframe);
    *this = e;
    //*this = const_cast<Ellipse*>(this)->compute_frame();
    return *this;
}

Ellipse Ellipse::rotate_self(double theta)
{
    if (ellipsetype & EllipseType::coef)
        return this->rotate_coef_self(theta);
    if (ellipsetype & EllipseType::centerframe)
        return this->rotate_centerframe_self(theta);
    return *this;
}
Ellipse Ellipse::rotate(double theta) const
{
    if (ellipsetype & EllipseType::coef)
        return this->rotate_coef(theta);
    if (ellipsetype & EllipseType::centerframe)
        return this->rotate_centerframe(theta);
    return *this;
}
Ellipse Ellipse::translate_self(const Point &p)
{
    if (ellipsetype & EllipseType::coef)
        return this->translate_coef_self(p);
    if (ellipsetype & EllipseType::centerframe)
        return this->translate_centerframe_self(p);
    return *this;
}
Ellipse Ellipse::translate(const Point &p) const
{
    if (ellipsetype & EllipseType::coef)
        return this->translate_coef(p);
    if (ellipsetype & EllipseType::centerframe)
        return this->translate_centerframe(p);
    return *this;
}


double Ellipse::getangleofpoint(const Point &p)
{
    if ((this->ellipsetype & EllipseType::centerframe) == false)
        this->compute_frame();
    Point p_referenceframe = (p - this->center).rotate(-this->theta);
    double alpha = atan2(p_referenceframe.gety() / this->b, p_referenceframe.getx() / this->a);
    return alpha;
}

Ellipse Ellipse::rotate_coef_self(double theta)
{
    Matrix3 R, newM;
    R << cos(theta), sin(theta), 0,
        -sin(theta), cos(theta), 0,
         0               , 0               , 1;
    R = R.inverse().eval();
    // A' = (R^-1)T * A * (R^-1)
    newM = R.transpose() * this->getM() * R;
    *this = Ellipse(newM);
    return *this;
}
Ellipse Ellipse::rotate_coef(double theta) const
{
    Ellipse e = Ellipse(*this);
    return e.rotate_coef_self(theta);
}

Ellipse Ellipse::rotate_centerframe_self(double theta)
{
    this->theta = this->theta + theta;
    return *this;
}
Ellipse Ellipse::rotate_centerframe(double theta) const
{
    Ellipse e = Ellipse(*this);
    e.rotate_centerframe_self(theta);
    return e;
}

Ellipse Ellipse::translate_coef_self(const Point &p)
{
    //*this = const_cast<Ellipse*>(this)->translate_coef(p);
    this->F = this->A * p.getx() * p.getx() + this->B * p.getx() * p.gety() + this->C * p.gety() * p.gety() - p * Point(this->D, this->E) + this->F;
    this->D = this->D - p * Point(2 * this->A, this->B);
    this->E = this->E - p * Point(this->B, 2 * this->C);
    return *this;
}
Ellipse Ellipse::translate_coef(const Point &p) const
{
    Ellipse e = Ellipse(*this);
    return e.translate_coef_self(p);
}
Ellipse Ellipse::translate_centerframe_self(const Point &p)
{
    this->center = this->center + p;
    return *this;
}
Ellipse Ellipse::translate_centerframe(const Point &p) const
{
    Ellipse e = Ellipse(*this);
    e.translate_centerframe_self(p);
    return e;
}

//get method
Point Ellipse::getcenter() const
{
    return this->center;
}

//get method
double Ellipse::geta() const
{
    return this->a;
}

//get method
double Ellipse::getb() const
{
    return this->b;
}

//get method
double Ellipse::gettheta() const
{
    return this->theta;
}


double Ellipse::geteccentricity()
{
    if ((this->ellipsetype & EllipseType::centerframe) == false)
        this->compute_frame();
    return sqrt(1 - this->b * this->b / this->a / this->a);
}

//get method
Matrix3 Ellipse::getM() const
{
    Matrix3 M;
    M << A, B/2, D/2,
         B/2, C, E/2,
         D/2, E/2, F;
    return M;
}


void Ellipse::print() const
{
    if (this->ellipsetype & EllipseType::centerframe)
    {
        cout << "cx = " << this->center.getx()
             << " cy = " << this->center.gety()
             << " a = " << this->a
             << " b = " << this->b << endl;
    }

    if (this->ellipsetype & EllipseType::coef)
    {
         cout << "A = " << this->A
             << " B = " << this->B
             << " C = " << this->C
             << " D = " << this->D
             << " E = " << this->E
             << " F = " << this->F << endl;
    }
}





