#ifndef KNOTSEQUENCES_H
#define KNOTSEQUENCES_H
#include "Utils.h"
#include "BaseKnotSequence.h"
#include <vector>
#include <cmath>
#include <Point.h>

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;
typedef std::vector<BaseKnotSequence*> Knots;


class KnotSequences
{
    public:
        /** Default constructor */
        KnotSequences() {}
        KnotSequences(const KnotSequences &knotSequences);
        KnotSequences(Knots knots);

        KnotSequences& operator=(const KnotSequences& other);

        doubles getSequence() const ;
        doubles getSequence(const doubles &values) const;
        doubles computeError(int numCP, int n, const Points &original, const doubles &values);
        int computeNumParams();
        size_t count() const;
        void setPropsToOptimize(const bools &values);
        void setValuesToOptimize(const vector<std::variant<int, double, bool>> &values);
        vector<std::variant<int, double, bool>> valuesToOptimizeFromDoubles(const doubles &values);


        static KnotSequences getCompleteBirationalFixedKS(int bspline_n, int bspline_numCP, double q1, double q2, double center);
        static KnotSequences getCompleteBirationalFixedKS(int bspline_n, int bspline_numCP, double q, double center){
            return getCompleteBirationalFixedKS(bspline_n, bspline_numCP, q, 1/q, center);
        }
        static KnotSequences getCompleteBirationalKS(int bspline_n, int bspline_numCP);

        static KnotSequences getKSFromParameters(int bspline_n, int bspline_numCP, doubles t);
        static KnotSequences getKSFromCumulParameters(int bspline_n, int bspline_numCP, doubles t);

        static KnotSequences getCentripetalKS(int bspline_n, int bspline_numCP, Points points){
            return getKSFromParameters(bspline_n, bspline_numCP, Utils::centripetal(points, 0.5));
        }
        static KnotSequences getChordLengthKS(int bspline_n, int bspline_numCP, Points points){
            return getKSFromParameters(bspline_n, bspline_numCP, Utils::chordlength(points));
        }

        Knots getKnots() const;
        void setKnots(const Knots &value);

protected:
        Knots knots;

private:
};

#endif // KNOTSEQUENCES_H
