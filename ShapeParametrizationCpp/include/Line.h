#ifndef LINE_H
#define LINE_H

#include "Point.h"
#include "Evaluable.h"
#include <vector>

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

class Line: public Evaluable
{
    public:
        Line();
        Line(double m, double q);
        Line(const Point &p, double m);
        Line(const Point &p1, const Point &p2);
        Line(const Line& other);
        virtual ~Line();
        Line& operator=(const Line& other);

        Point evaluate(double) const;
        Points evaluate(int steps ) const;

        Line perpendicular(const Point& other) const;
        Point intersection(const Line& other) const;
        double rotateslope(double theta) const;

        Point getdirection() const;
        double getm() const;
        double getq() const;

        void print() const;


    protected:
    // y = mx + q
        double m;
        double q;

    private:
};

#endif // LINE_H
