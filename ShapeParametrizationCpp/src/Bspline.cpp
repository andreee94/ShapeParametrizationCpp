#include "Bspline.h"
#include "Point.h"
#include "Line.h"
#include "Utils.h"
#include <cmath>

using namespace std;

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;


Bspline::Bspline(Points CParray, doubles uarray, int n)
{
    this->CParray = CParray;
    this->uarray = uarray;
    this->n = n;
}

Bspline::Bspline()
{
    //ctor
}

Bspline::~Bspline()
{
    //dtor
}

Bspline::Bspline(const Bspline& other)
{
    //copy ctor
}

Bspline& Bspline::operator=(const Bspline& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

Point Bspline::deBoor(int k, double u) const
{
    /*"""
    Evaluates S(x).

    Args
    ----
    k: index of knot interval that contains x
    u: position
    uarray: array of knot positions, needs to be padded as described above
    CParray: array of control points
    n: degree of B-spline
    */
    // we need to have u != from  uarray[0]
    Points d;
    d.reserve(this->n + 1);
    for (int j = 0; j < this->n + 1; j++)
        d[j] = this->CParray[j + k - this->n];

    for (int r = 1; r < this->n + 1; r++)
        for (int j = this->n; j > r - 1; j--)
        {
            double alpha = (u - uarray[j + k - n]) / (uarray[j + 1 + k - r] - uarray[j + k - n]);
            d[j] = (1.0 - alpha) * d[j - 1] + alpha * d[j];
        }
    return d[n];
}

Point Bspline::evaluate(double u) const
{
    unsigned int k = 0;
    if (u == this->uarray[0])
        u = u + 1e-6;

    while (k + 1 < this->uarray.size() && u > this->uarray[k + 1])
        k = k + 1;
    return this->deBoor(k, u);
}


// when no start and end specified, use between 0 and 2 pi
Points Bspline::evaluate(int steps) const
{
    return this->Evaluable::evaluate(0.0,  1.0, steps);
}

Bspline Bspline::computeDerivarive() const
{
    // https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-derv.html
    Points CQarray;
    CQarray.reserve(this->CParray.size() - 1); // 1 less control point
    for (unsigned int i = 0; i < this->CParray.size() - 1; i++) // 1 less control point
    {
        Point P = this->n / (this->uarray[i + this->n + 1] - this->uarray[i + 1]) * (this->CParray[i + 1] - this->CParray[i]);
        CQarray[i] = P;
    }
    // return a new Bspline: knots array, control points, degrees
    // we remove first and last knots
    // degrees is reduced by one
    Bspline deriv = Bspline(CQarray, Utils::extractmid(this->uarray, 1), this->n - 1);
    return deriv;
}
