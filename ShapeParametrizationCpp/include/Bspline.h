#ifndef BSPLINE_H
#define BSPLINE_H
#include "Point.h"
#include "Line.h"
#include "Editable.h"
#include "Evaluable.h"
#include <cmath>
#include <string>
#include <Settings.h>
#include <BaseKnotSequence.h>
#include <KnotSequences.h>

using namespace std;

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;
typedef std::vector<string> strings;
typedef std::vector<BaseKnotSequence*> Knots;


class Bspline: public Evaluable, public Editable<Bspline>
{
    public:

        /** Default constructor */
        Bspline();
        Bspline(const Points &CParray, const doubles &uarray, int n);
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
        Points getNormals(doubles us) const;
        Points getNormalsInCP() const;
        Points getTangents(doubles us) const;
        Points getTangentsInCP() const;

        Point evaluate(double) const;
        Points evaluate(int steps) const;

        Points evaluate(const doubles d) const {
            return this->Evaluable::evaluate(d);
        }
        Points evaluate(double start, double end, int steps) const {
            return this->Evaluable::evaluate(start, end, steps);
        }

        Points evaluateWithTE(int numpoints, int numpointsTE, string shape, bool tangent_first=true);
        Points evaluateTE(int numpointsTE, string shape, bool tangent_first=true);

        Bspline modifyCP(const doubles &params) const;
        Bspline modifyCP_self(const doubles &params);

        void IOsave(string filename) const;
        Bspline IOload(string filename);

        strings getCParray_str() const;
        Points getCParray() const;
        Settings getSettings() const;
        void setSettings(Settings s);
        void setSettings(string filename);

        static Bspline interpolate(const Points &points, int numCP, int n, const KnotSequences &knotsequence);

    protected:
        Points CParray;
        int n;
        doubles uarray;
        Settings settings;

        Point deBoor(int k, double u) const;
        static double interpolate_N(int l, int n, doubles uarray, double u);
        static Points interpolate_getCP(const Points &points, doubles uarray, int numCP, int n);

    private:
};

#endif // BSPLINE_H
