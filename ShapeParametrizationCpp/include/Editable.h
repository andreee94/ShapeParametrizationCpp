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
        Editable<T>(){}

        virtual T modifyCP(const doubles &params) const = 0;
        virtual T modifyCP_self(const doubles &params) = 0;

        T getExtremeMin() const{
            return modifyCP(minRange);
        }

        T getExtremeMax() const{
            return modifyCP(maxRange);
        }

        T getExtremeMin(const doubles &minRange) const{
            T t = modifyCP(minRange);
            t.minRange = minRange;
            return t;
        }

        T getExtremeMax(const doubles &maxRange) const{
            T t = modifyCP(maxRange);
            t.maxRange = maxRange;
            return t;
        }

        T getExtremeMin_self(){
            return modifyCP_self(minRange);
        }

        T getExtremeMax_self(){
            return modifyCP_self(maxRange);
        }

        T getExtremeMin_self(const doubles &minRange){
            this->minRange = minRange;
            return getExtremeMin_self();
        }

        T getExtremeMax_self(const doubles &maxRange){
            this->maxRange = maxRange;
            return getExtremeMax_self();
        }

        doubles filterFixedParams(const doubles& params)
        {
            doubles res;
            res.reserve(params.size());
            for (int i = 0; i< params.size(); i++)
            {
                res[i] = Utils::contains<int>(this->adjustableIndices, i) ? params[i] : 0;
            }
            return res;
        }

        doubles fillFixedParams(const doubles& params, int numCP)
        {
            doubles res(numCP, 0); // set everything to zero
            for (int i = 0; i < params.size(); i++)
            {
                res[adjustableIndices[i]] = params[i];
            }
            return res;
        }


//        T getExtremeMin();
//        T getExtremeMax();
//        T getExtremeMin(const doubles &minRange);
//        T getExtremeMax(const doubles &maxRange);

//        // set params to 0 for fixed CP
//        doubles filterFixedParams(const doubles& params);
//        // take only adjustable params and set 0 the others
//        doubles fillFixedParams(const doubles& params, int numCP);


    protected:
        doubles minRange;
        doubles maxRange;
        ints adjustableIndices;

    private:
};

#endif // EDITABLE_H
