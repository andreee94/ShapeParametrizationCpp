#ifndef EDITABLE_H
#define EDITABLE_H

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <algorithm>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

enum TEMotion
{
    RIGID = 0,
    FIRSTPOINT,
    SECONDPOINT,
    SECONDLASTPOINT,
    LASTPOINT,
    AUTO,
    NONE
};

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

        doubles filterFixedParams(const doubles& params, bool exceptTE=false) const
        {
            unsigned int padding = exceptTE ? 2 : 0;
            doubles res;
            res.reserve(params.size());
            for (unsigned int i = 0; i< padding; i++)
                res.push_back(params[i]);
            for (unsigned int i = 0 + padding; i< params.size() - padding; i++)
                res.push_back(Utils::contains<int>(this->adjustableIndices, i) ? params[i] : 0);
            for (unsigned int i = params.size() - padding; i< params.size(); i++)
                res.push_back(params[i]);
            return res;
        }

        doubles fillFixedParams(const doubles& params, int numCP) const
        {
            doubles res(numCP, 0); // set everything to zero
            for (int i = 0; i < params.size(); i++)
            {
                res[adjustableIndices[i]] = params[i];
            }
            return res;
        }


        void manageTEType(doubles& params, Points &normals, TEMotion temotion) const
        {
            int end = params.size();
            int index = -1;
            if (temotion == TEMotion::RIGID) // we will set everything to 0
                index = -1;
            else if (temotion == TEMotion::FIRSTPOINT) // we will set everything to 0
                index = 0;
            else if (temotion == TEMotion::SECONDPOINT) // we will set everything to 0
                index = 1;
            else if (temotion == TEMotion::LASTPOINT) // we will set everything to 0
                index = end - 1;
            else if (temotion == TEMotion::SECONDLASTPOINT) // we will set everything to 0
                index = end - 2;
            else if (temotion == TEMotion::AUTO) // we will set everything to 0
            {
                ints free_indexes = {0, 1, end - 1, end - 2}; // fill with TE control points
                // delete items which are not contained in adjustableIndices
                free_indexes.erase(
                            std::remove_if(free_indexes.begin(), free_indexes.end(),
                               [this] (int i)
                                {
                                    return !Utils::contains(adjustableIndices, i);
                                }),
                            free_indexes.end());
                if (free_indexes.size() > 1) // for auto recognition just one free point is required
                    temotion = TEMotion::NONE;
                else if (free_indexes.size() < 1) // all points are fixed
                    index = -1;
                else
                    index = free_indexes.at(0); // get the only free point index
            }
            if (temotion == TEMotion::NONE)
            {
                params = filterFixedParams(params, false); // apply filtering also at TE
                return;
            }
            ints te_indexes = {0, 1, end - 1, end - 2}; // fill with TE control points
            for (int ind : te_indexes)
            {
                params[ind] = index >= 0 ? params[index] : 0;
                normals[ind] = index >= 0 ? normals[index] : normals[ind]; //Point(0, 0);
            }
        }   

        doubles getMinRange() const
        {
            return minRange;
        }

        void setMinRange(const doubles &value)
        {
            minRange = value;
        }

        doubles getMaxRange() const
        {
            return maxRange;
        }

        void setMaxRange(const doubles &value)
        {
            maxRange = value;
        }

        ints getAdjustableIndices() const
        {
            return adjustableIndices;
        }

        void setAdjustableIndices(const ints &value)
        {
            adjustableIndices = value;
        }

        TEMotion getTEMotion() const
        {
            return temotion;
        }

        void setTEMotion(const TEMotion &value)
        {
            temotion = value;
        }

        //        T getExtremeMin();
        //        T getExtremeMax();
        //        T getExtremeMin(const doubles &minRange);
        //        T getExtremeMax(const doubles &maxRange);

        //        // set params to 0 for fixed CP
        //        doubles filterFixedParams(const doubles& params);
        //        // take only adjustable params and set 0 the others
        //        doubles fillFixedParams(const doubles& params, int numCP);

        //        doubles getMinRange() const;
        //        void setMinRange(const doubles &value);

        //        doubles getMaxRange() const;
        //        void setMaxRange(const doubles &value);

        //        ints getAdjustableIndices() const;
        //        void setAdjustableIndices(const ints &value);

        //        TEMotion getTEMotion() const;
        //        void setTEMotion(const TEMotion &value);

protected:
        doubles minRange;
        doubles maxRange;
        ints adjustableIndices;
        TEMotion temotion;

    private:
};

#endif // EDITABLE_H
