#include <iostream>
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "Ellipse.h"

using namespace std;

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

int main()
{
    cout << "Hello world!" << endl;

//    Point p1 = Point(0, 0);
//    Point p2 = Point(1, 1);
//    Point p3 = Point(1, 0);
//    Point p = 2*p1 + p2/2 - p3*2.0;
//    Line l = p1.axis(p2);
//    cout << "m = " << l.getm() << endl;
//    cout << "q = " << l.getq() << endl;
//    cout << p.x << ", " << p.y << endl;
//    cout << p2.slope(p3) << endl;


    cout << "-------------------------------------" << endl;
    cout << "Axis: p1, p2, line" << endl;
    Point p1 = Point(1, 0);
    Point p2 = Point(2, 1);
    Line l = p1.axis(p2);
    p1.print();
    p2.print();
    l.print();

    cout << "-------------------------------------" << endl;
    cout << "Circle: l1, p1, p2, circle" << endl;
    Line l1 = Line(-2, 0);
    p1 = Point(10, 10);
    p2 = Point(11, 11);
    Circle circle = Circle(p1, p2, l1.getm());
    l1.print();
    p1.print();
    p2.print();
    circle.print();



    //plt::plot({1,3,2,4});
    //plt::show();












    return 0;
}
