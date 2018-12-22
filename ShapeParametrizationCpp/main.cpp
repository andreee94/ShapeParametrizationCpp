#include <iostream>
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "Ellipse.h"
#include "BaseKnotSequence.h"
#include "Utils.h"


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


    cout << "-------------------------------------" << endl;
    UniformKS u1 = UniformKS(10, 11);
    Utils::print(u1.getSequence({5}));

    cout << "-------------------------------------" << endl;
    UniformFixedKS u2 = UniformFixedKS(10, 11, 5);
    Utils::print(u2.getSequence());

    cout << "-------------------------------------" << endl;
    ValueFixedKS u4 = ValueFixedKS(10);
    Utils::print(u4.getSequence());
    Utils::print(u4.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    MultiplicityFixedValueKS u5 = MultiplicityFixedValueKS(10, 5);
    Utils::print(u5.getSequence());
    Utils::print(u5.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    MultiplicityValueKS u6 = MultiplicityValueKS(10);
    Utils::print(u6.getSequence({5}));

    cout << "-------------------------------------" << endl;
    BeginKS u7 = BeginKS(3);
    Utils::print(u7.getSequence());
    Utils::print(u7.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    EndKS u8 = EndKS(3);
    Utils::print(u8.getSequence());
    Utils::print(u8.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    RationalFixedKS u9 = RationalFixedKS(1, 2, 6, 1.5);
    Utils::print(u9.getSequence());
    Utils::print(u9.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    RationalKS u10 = RationalKS(1, 2, 6);
    Utils::print(u10.getSequence({1.5}));

    cout << "-------------------------------------" << endl;
    BiRationalFixedKS u11 = BiRationalFixedKS(0, 1, 5, 5, 0.5);
    Utils::print(u11.getSequence());
    Utils::print(u11.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    BiRationalKS u12 = BiRationalKS(0, 1, 5);
    Utils::print(u12.getSequence({1, 2, 0.5}));


    //plt::plot({1,3,2,4});
    //plt::show();












    return 0;
}
