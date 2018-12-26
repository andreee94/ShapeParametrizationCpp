#include "KnotSequences.h"
#include "Utils.h"
#include "BaseKnotSequence.h"
#include <vector>
#include <cmath>
#include <Point.h>
#include <iostream>

using namespace std;

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;
typedef std::vector<BaseKnotSequence*> Knots;

KnotSequences::KnotSequences(Knots knots)
{
    this->knots = knots;
}

doubles KnotSequences::getSequence(doubles params)
{
    doubles uarray;
    int StartIndex = 0;
    // for each knot extract params ang generate the sequence
    for (BaseKnotSequence* item : this->knots)
    {
        // extract params
        if (typeid(*item) != typeid(BaseFixedKnotSequence) && item->getNumParams() > 0)
        {
            doubles item_params(params.begin() + StartIndex, params.begin() + StartIndex + item->getNumParams());
            //Utils::print(item->getSequence(item_params));
            doubles sequence = item->getSequence(item_params);
            uarray.insert(uarray.end(), sequence.begin(), sequence.end());
        }
        else
        {
            BaseFixedKnotSequence* fixed_item = (BaseFixedKnotSequence*)item;
            //Utils::print(fixed_item->getSequence());
            doubles sequence = fixed_item->getSequence();
            uarray.insert(uarray.end(), sequence.begin(), sequence.end());
        }
        StartIndex += item->getNumParams();
    }
    return uarray;
}

int KnotSequences::computeNumParams()
{
    int numParams = 0;
    for (auto item : this->knots)
        numParams += item->getNumParams();
    return numParams;
}


KnotSequences KnotSequences::getCompleteBirationalFixedKS(int bspline_n, int bspline_numCP, double q1, double q2, double center)
{
    // numCP must be an odd number.
    if (bspline_numCP % 2 == 0)
    {
        cout << "bspline_numCP must be ODD, so we add 1 to make it odd. New bspline_numCP = " << bspline_numCP << endl;
        bspline_numCP++;
    }
    //example 11 CP, n = 3
    // 0 0 0 (0 x x x x x x x 1) 1 1 1
    // begin (   birational    ) end
    // but birational generates *2+1 points other than 0 and 1
    int numpoints = bspline_numCP + bspline_n + 1 - 2 * (bspline_n + 1); // number of knots not 0 and 1
    numpoints = (int)((numpoints + 1) / 2); // birational generates 2n + 1 values

    BeginKS beginKS = BeginKS(bspline_n-1); // n-1 since we include the point also into the birational
    EndKS endKS = EndKS(bspline_n-1); // n-1 since we include the point also into the birational
    BiRationalFixedKS midKS = BiRationalFixedKS(0, 1, numpoints, q1, q2, center);

    Knots knots;
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&beginKS));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&midKS));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&endKS));
    return KnotSequences(knots);
}


KnotSequences KnotSequences::getCompleteBirationalKS(int bspline_n, int bspline_numCP)
{
    // numCP must be an odd number.
    if (bspline_numCP % 2 == 0)
    {
        cout << "bspline_numCP must be ODD, so we add 1 to make it odd. New bspline_numCP = " << bspline_numCP << endl;
        bspline_numCP++;
    }
    //example 11 CP, n = 3
    // 0 0 0 (0 x x x x x x x 1) 1 1 1
    // begin (   birational    ) end
    // but birational generates *2+1 points other than 0 and 1
    int numpoints = bspline_numCP + bspline_n + 1 - 2 * (bspline_n + 1); // number of knots not 0 and 1
    numpoints = (int)((numpoints + 1) / 2); // birational generates 2n + 1 values

    BeginKS beginKS = BeginKS(bspline_n-1); // n-1 since we include the point also into the birational
    EndKS endKS = EndKS(bspline_n-1); // n-1 since we include the point also into the birational
    BiRationalKS midKS = BiRationalKS(0, 1, numpoints);

    Knots knots;
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&beginKS));
    knots.push_back(dynamic_cast<BaseKnotSequence*>(&midKS));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&endKS));
    return KnotSequences(knots);
}

KnotSequences KnotSequences::getKSFromCumulParameters(int bspline_n, int bspline_numCP, doubles t)
{
    return KnotSequences::getKSFromParameters(bspline_n, bspline_numCP, Utils::computeCumul(0, 1, t));
}

KnotSequences KnotSequences::getKSFromParameters(int bspline_n, int bspline_numCP, doubles t)
{
    doubles uarray;
    uarray.reserve(bspline_numCP + bspline_n + 1 - 2 * bspline_n); // number of knots including a 0 and a 1

    // extract uniform data from centripetal parametrization
    uarray = Utils::interp1(t, Utils::linspace(0, 1, t.size()), Utils::linspace(0, 1, uarray.size()));

    BeginKS beginKS = BeginKS(bspline_n-1); // n-1 since we include the point also into the birational
    EndKS endKS = EndKS(bspline_n-1); // n-1 since we include the point also into the birational
    CustomFixedKS midKS = CustomFixedKS(0, 1, uarray);

    Knots knots;
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&beginKS));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&midKS));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&endKS));
    return KnotSequences(knots);
}

