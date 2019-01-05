#ifndef CIRCLE_H
#define CIRCLE_H

#include <Evaluable.h>
#include <TrailingEdge.h>
#include "Line.h"
#include "Point.h"
#include <cmath>
#include <iostream>

using namespace std;
typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

class Circle : public TrailingEdge
{
    public:

        /** Default constructor */
        Circle();
        Circle(const Point &center, double radius);
        Circle(const Point &p1, const Point &p2, double m, bool tangentfirst=true);

        /** Default destructor */
        virtual ~Circle();
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        //Circle& operator=(const Circle& other);

        Point evaluate(double) const;
        Points evaluate(int steps) const;

        double getangleofpoint(const Point &p);

        Point getcenter() const;
        double getradius() const;

        void print() const;

    protected:
        Point center;
        double radius;

    private:
};
#endif // CIRCLE_H
