#ifndef TRAILINGEDGE_H
#define TRAILINGEDGE_H

#include <Evaluable.h>
#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <map>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<float> floats;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;

enum TrailingEdgeType { TEnone=0, TEcircle=1, TEellipse=2};

class TrailingEdge: public Evaluable
{
    public:
        /** Default constructor */
        TrailingEdge();
        /** Default destructor */
        virtual ~TrailingEdge();

        virtual double getangleofpoint(const Point &p) = 0;

        Points computeTE(Points &points, int steps);



    protected:

    private:
};

#endif // TRAILINGEDGE_H
