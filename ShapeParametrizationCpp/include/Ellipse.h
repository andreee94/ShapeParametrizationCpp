#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <TrailingEdge.h>
#include <Evaluable.h>
#include "Line.h"
#include "Point.h"
#include <vector>
#include <array>
#include <cmath>
#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

typedef Matrix<double, 3, 1> Vector3;
typedef Matrix<double, 3, 3> Matrix3;

enum EllipseType { none=0, coef=1, centerframe=2, full=3};

class Ellipse : public TrailingEdge
{
    public:

        /** Default constructor */
        Ellipse()
        {
            this->ellipsetype = EllipseType::none;
        }
        Ellipse(const Ellipse &e);
        Ellipse(const Point &center, double a, double b, double theta);
        Ellipse(const Point &p1, const Point &p2, double m1, double m2);
        Ellipse(const Point &p1, const Point &p2, const Line &m1,  const Line &m2);

        /** Default destructor */
        virtual ~Ellipse();
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Ellipse& operator=(const Ellipse& other);

        Ellipse compute_frame_self();
        Ellipse compute_frame() const;
        Ellipse rotate_self(double theta);
        Ellipse rotate(double theta) const;
        Ellipse translate_self(const Point &p);
        Ellipse translate(const Point &p) const;

        double getangleofpoint(const Point &p);

        Point evaluate(double) const;
        Points evaluate(int steps) const;

        Point getcenter() const;
        double geta() const;
        double getb() const;
        double gettheta() const;
        double geteccentricity();
        Matrix3 getM() const;

        void print() const;


    protected:
        Point center;
        double a, b, theta;
        double A, B, C, D, E, F;
        EllipseType ellipsetype;
        //double M[][];

        // a is not the semiaxis of the ellipse
        Ellipse(double a, double m1, double m2);
        Ellipse(double a, const Line &m1, const Line &m2);
        Ellipse(double A, double B, double C, double D, double E, double F);
        Ellipse(Matrix3 M);

        Ellipse rotate_coef_self(double theta);
        Ellipse rotate_coef(double theta) const;
        Ellipse rotate_centerframe_self(double theta);
        Ellipse rotate_centerframe(double theta) const;

        Ellipse translate_coef_self(const Point &p);
        Ellipse translate_coef(const Point &p) const;
        Ellipse translate_centerframe_self(const Point &p);
        Ellipse translate_centerframe(const Point &p) const;

    private:
};

#endif // ELLIPSE_H
