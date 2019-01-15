#include "Editable.h"

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

//template<class T>
//T Editable<T>::getExtremeMin(){
//    return modifyCP(minRange);
//}


//template<class T>
//T Editable<T>::getExtremeMax(){
//    return modifyCP(maxRange);
//}


//template<class T>
//T Editable<T>::getExtremeMin(const doubles &minRange){
//    this->minRange = minRange;
//    return getExtremeMin();
//}


//template<class T>
//T Editable<T>::getExtremeMax(const doubles &maxRange){
//    this->maxRange = maxRange;
//    return getExtremeMax();
//}


//template<class T>
//doubles Editable<T>::filterFixedParams(const doubles& params)
//{
//    doubles res;
//    res.reserve(params.size());
//    for (int i = 0; i< params.size(); i++)
//    {
//        res[i] = Utils::contains<int>(this->adjustableIndices, i) ? params[i] : 0;
//    }
//    return res;
//}


//template<class T>
//doubles Editable<T>::fillFixedParams(const doubles& params, int numCP)
//{
//    doubles res(numCP, 0); // set everything to zero
//    for (int i = 0; i < params.size(); i++)
//    {
//        res[adjustableIndices[i]] = params[i];
//    }
//    return res;
//}
