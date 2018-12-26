#ifndef EDITABLE_H
#define EDITABLE_H

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<Point> Points;


template <class T>
class Editable
{
    public:
        /** Default constructor */
        Editable();

        virtual T modifyCP(doubles params);

        T getExtremeMin();
        T getExtremeMax();
        T getExtremeMin(doubles minRange);
        T getExtremeMax(doubles maxRange);

        // set params to 0 for fixed CP
        doubles filterFixedParams(const doubles& params);
        // take only adjustable params and set 0 the others
        doubles fillFixedParams(const doubles& params, int numCP);


    protected:
        doubles minRange;
        doubles maxRange;
        ints adjustableIndices;

    private:
};

#endif // EDITABLE_H
