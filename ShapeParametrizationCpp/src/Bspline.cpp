#include "Bspline.h"
#include "Point.h"
#include "Line.h"
#include "Utils.h"
#include <cmath>

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



Points Bspline::getnormals(doubles us) const
{
    Bspline derivative = this->computeDerivarive(); // compute bspline derivative
    Points derivatives = derivative.Evaluable::evaluate(us); // evaluate bspline derivative is control points
    Point::normalize(derivatives); // normalize directions
    return derivatives;
}


Points Bspline::getnormalsInCP() const
{
    doubles wCP = Utils::centripetal(this->CParray); // estimate u value of the control points
    return this->getnormals(wCP);
}



// compute coef of matrix for interpolation
double Bspline::interpolate_N(int l, int n, doubles uarray, double u)
{
    if(n == 0)
        return uarray[l - 1] <= u  && u < uarray[l] ? 1 : 0;
    else
    {
        double alpha = 0;
        double beta = 0;
        if ((uarray[l + n - 1] - uarray[l - 1]) != 0)
            alpha = (u - uarray[l - 1]) / (uarray[l + n - 1] - uarray[l - 1]);
        //else alpha = 0;

        if ((uarray[l + n] - uarray[l]) != 0)
            beta = (uarray[l + n] - u) / (uarray[l + n] - uarray[l]);
        //else beta = 0;

        return alpha * interpolate_N(l, n - 1, uarray, u) + beta * interpolate_N(l + 1, n - 1, uarray, u);
    }

}

Points Bspline::interpolate_getCP(Points points, doubles uarray, int numCP, int n)
{
    doubles w = Utils::centripetal(points, 1);
    MatrixXd M = MatrixXd::Zero(numCP, numCP); // init empty matrix
    VectorXd bx = VectorXd::Zero(numCP); // init empty known term vector x coordinate
    VectorXd by = VectorXd::Zero(numCP); // init empty known term vector y coordinate

    // M * CP = b
    // calculation of M
    for (int k = 0; k < numCP; k++)
        for (int j = 0; j < numCP; j++)
            for (unsigned int i = 0; i < points.size(); i++)
            {
                double Nj = interpolate_N(j + 1, n, uarray, w[i]);
                double Nk = interpolate_N(k + 1, n, uarray, w[i]);
                M(j, k) = M(j, k) + Nj * Nk;
            }

    // calculation of b
    for (int k = 0; k < numCP; k++)
        for (unsigned int i = 0; i < points.size(); i++)
        {
            double Nk = interpolate_N(k + 1, n, uarray, w[i]);
            bx(k) = bx(k) + Nk * points[i].x;
            by(k) = by(k) + Nk * points[i].y;
        }

    // first point fixed to points[0]
    for (int i = 0; i< numCP; i++)
    {
        bx(i) = bx(i) - M(i, 0) * points.front().x - M(i, numCP - 1) * points.back().x;
        by(i) = by(i) - M(i, 0) * points.front().y - M(i, numCP - 1) * points.back().y;
    }
    // first column and row
    M.col(0) = VectorXd::Zero(numCP);
    M.row(0) = VectorXd::Zero(numCP);
    // last column and row
    M.col(numCP - 1) = VectorXd::Zero(numCP);
    M.row(numCP - 1) = VectorXd::Zero(numCP);
    //
    M(0, 0) = 1;
    M(numCP - 1, numCP - 1) = 1;
    // fix first and last points in known vector
    bx(0) = points.front().x;
    by(0) = points.front().y;
    bx(numCP - 1) = points.back().x;
    by(numCP - 1) = points.back().y;

    LDLT<MatrixXd> L = M.ldlt(); // cholesky decomposition
    VectorXd CPx = L.solve(bx);
    VectorXd CPy = L.solve(by);

    // build control points from x and y coordinates
    Points CP;
    CP.reserve(numCP);
    for( int i = 0; i < numCP; i++)
        CP[i] = Point(CPx[i], CPy[i]);
    return CP;
}














