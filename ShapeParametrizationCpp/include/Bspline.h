#ifndef BSPLINE_H
#define BSPLINE_H
#include "Point.h"
#include "Line.h"
#include <cmath>

using namespace std;

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;


class Bspline: public Evaluable
{
    public:
        /** Default constructor */
        Bspline();
        Bspline(Points CParray, doubles uarray, int n);
        /** Default destructor */
        virtual ~Bspline();
        /** Copy constructor
         *  \param other Object to copy from
         */
        Bspline(const Bspline& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Bspline& operator=(const Bspline& other);

        Bspline computeDerivarive() const;
        Points getnormals(doubles us) const;
        Points getnormalsInCP() const;

        Point evaluate(double) const;
        Points evaluate(int steps) const;

        Points evaluate(const doubles d) const {
            return this->Evaluable::evaluate(d);
        }
        Points evaluate(double start, double end, int steps) const {
            return this->Evaluable::evaluate(start, end, steps);
        }

        void IOsave(string filename) const;
        Bspline IOload(string filename);


    protected:
        Points CParray;
        int n;
        doubles uarray;

        Point deBoor(int k, double u) const;
        static double interpolate_N(int l, int n, doubles uarray, double u);
        static Points interpolate_getCP(Points points, doubles uarray, int numCP, int n);

    private:
};

#endif // BSPLINE_H
