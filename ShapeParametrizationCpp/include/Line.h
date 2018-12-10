#ifndef LINE_H
#define LINE_H

#include "Point.h"


class Line
{
    public:
        Line();
        Line(double m, double q);
        Line(const Point &p, double m);
        Line(const Point &p1, const Point &p2);
        virtual ~Line();
        Line& operator=(const Line& other);
        double getm();
        double getq();


    protected:
    // y = mx + q
        double m;
        double q;

    private:
};

#endif // LINE_H
