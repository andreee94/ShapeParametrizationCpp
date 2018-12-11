#include <iostream>
#include "Line.h"
#include "Point.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    Point p1 = Point(0, 0);
    Point p2 = Point(1, 1);
    Point p3 = Point(1, 0);
    Point p = 2*p1 + p2/2 - p3*2.0;
    Line l = p1.axis(p2);
    cout << "m = " << l.getm() << endl;
    cout << "q = " << l.getq() << endl;
    cout << p.x << ", " << p.y << endl;
    cout << p2.slope(p3) << endl;


    return 0;
}
