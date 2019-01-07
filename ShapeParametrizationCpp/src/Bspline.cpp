#include "Bspline.h"
#include "Point.h"
#include "Line.h"
#include "Ellipse.h"
#include "Circle.h"
#include "Utils.h"
#include "TrailingEdge.h"
#include "Editable.h"
#include "Evaluable.h"
#include "pystring.h"
#include "Table.h"
#include "Bspline.h"
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



Bspline::Bspline(const Points &CParray, const doubles &uarray, int n)
:Evaluable::Evaluable()
,Editable::Editable()
{
    this->CParray = CParray;
    this->uarray = uarray;
    this->n = n;
}

Bspline::Bspline()
:Evaluable::Evaluable()
,Editable<Bspline>::Editable()
{
    //ctor
}

Bspline::~Bspline()
{
    //dtor
}

Bspline::Bspline(const Bspline& other)
:Evaluable::Evaluable()
,Editable<Bspline>::Editable()
{
    this->CParray = other.CParray;
    this->uarray = other.uarray;
    this->n = other.n;
    this->Editable<Bspline>::minRange = other.Editable<Bspline>::minRange;
    this->Editable<Bspline>::maxRange = other.Editable<Bspline>::maxRange;
    this->Editable<Bspline>::adjustableIndices = other.Editable<Bspline>::adjustableIndices;
    this->settings = other.settings;
}

Bspline& Bspline::operator=(const Bspline& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    this->n = rhs.n;
    this->uarray = rhs.uarray;
    this->CParray = rhs.CParray;
    this->minRange = rhs.minRange;
    this->maxRange = rhs.maxRange;
    this->adjustableIndices = rhs.adjustableIndices;
    this->settings = rhs.settings;
    return *this;
}


Points Bspline::getCParray() const
{
    return this->CParray;
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
    {
//        cout << j << endl;
//        cout << j + k - this->n << endl;
//        cout << this->n + 1 << endl;
//        cout << CParray.size() << endl;

        d[j] = this->CParray[j + k - this->n];
    }

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
        CQarray.push_back(P);
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
            bx(k) = bx(k) + Nk * points[i].getx();
            by(k) = by(k) + Nk * points[i].gety();
        }

    // first point fixed to points[0]
    for (int i = 0; i< numCP; i++)
    {
        bx(i) = bx(i) - M(i, 0) * points.front().getx() - M(i, numCP - 1) * points.back().getx();
        by(i) = by(i) - M(i, 0) * points.front().gety() - M(i, numCP - 1) * points.back().gety();
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
    bx(0) = points.front().getx();
    by(0) = points.front().gety();
    bx(numCP - 1) = points.back().getx();
    by(numCP - 1) = points.back().gety();

    LDLT<MatrixXd> L = M.ldlt(); // cholesky decomposition
    VectorXd CPx = L.solve(bx);
    VectorXd CPy = L.solve(by);

    // build control points from x and y coordinates
    Points CP;
    CP.reserve(numCP);
    for( int i = 0; i < numCP; i++)
        CP.push_back(Point(CPx[i], CPy[i]));
    return CP;
}



Bspline Bspline::interpolate(Points &points, int numCP, int n, KnotSequences &knotsequence)
{
    doubles uarray = knotsequence.getSequence((doubles){});
    Points CParray = Bspline::interpolate_getCP(points, uarray, numCP, n);
    cout << "CParray = " << CParray.size() << endl;
    return Bspline(CParray, uarray, n);
}



Points Bspline::evaluateWithTE(int numpoints, int numpointsTE, string shape, bool tangent_first)
{
    // compute bspline points
    Points bspline_points = this->evaluate(numpoints);
    // compute tangents at start and end
    Points tangents = this->getnormals((doubles){0, 1});

    TrailingEdge *TE;
    Points tepoints;
    if (pystring::lower(shape) == "ellipse")
    {
        Ellipse ellipse = Ellipse(bspline_points.front(), bspline_points.back(), tangents.front().slope(), tangents.back().slope());
        TE = dynamic_cast<TrailingEdge*>(&ellipse);
        tepoints = ellipse.computeTE(bspline_points, numpointsTE);
    }
    else if (pystring::lower(shape) == "circle")
    {
        double m = tangent_first ? tangents.front().slope() : tangents.back().slope();
        Circle circle = Circle(bspline_points.front(), bspline_points.back(), m, tangent_first);
        TE = dynamic_cast<TrailingEdge*>(&circle);
        tepoints = circle.computeTE(bspline_points, numpointsTE);
    }
    // compute trailing edge points
    if (tepoints.size() > 0)
    {
        // reverse te points array
        // remove latest item if size > 1 since it is dupplicated
        std::reverse(tepoints.begin(),tepoints.end() - (int)(tepoints.size() == 1));
    }
    // concat bspline and te points
    bspline_points.insert(bspline_points.end(), tepoints.begin(), tepoints.end());
    return bspline_points;
}



Bspline Bspline::modifyCP(doubles params) const
{
    Bspline modified_bspline = Bspline(*this);
    Points normals = this->getnormalsInCP();

    for (unsigned int i = 0; i < this->CParray.size(); i++)
    {
        modified_bspline.CParray[i] = this->CParray[i].move(normals[i], params[i]);
    }
    return modified_bspline;
}

Bspline Bspline::modifyCP(doubles params)
{
    Points normals = this->getnormalsInCP();

    for (unsigned int i = 0; i < this->CParray.size(); i++)
    {
        this->CParray[i] = this->CParray[i].move(normals[i], params[i]);
    }
    return *this;
}












