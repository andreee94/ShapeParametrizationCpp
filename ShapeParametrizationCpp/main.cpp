#include <iostream>
#include "Line.h"
#include "Point.h"
#include "Circle.h"
#include "Ellipse.h"
#include "BaseKnotSequence.h"
#include "KnotSequences.h"
#include "Utils.h"
#include "Settings.h"
#include "Table.h"
#include "Bspline.h"


using namespace std;

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

typedef std::vector<double> doubles;
typedef std::vector<int> ints;
typedef std::vector<Point> Points;
typedef std::vector<BaseKnotSequence*> Knots;

int main()
{


    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    Table table = Table("section1");
    doubles x = table.getcolumn(0);
    doubles y = table.getcolumn(2);

    Points blade = Point::fromDoubles(x, y);

    KnotSequences sequence =  KnotSequences::getCompleteBirationalFixedKS(3, 21, 1.5, 0.55);
    Utils::printcout(sequence.getSequence((doubles){}));

    cout << Utils::print(blade, "  ", true);

    cout << "-------------------------------------" << endl;

    Bspline bspline = Bspline::interpolate(blade, 21, 3, sequence);

    //Points interpolated = bspline.evaluateWithTE(1000, 100, "ellipse");

    //plt::plot(x, y, ".k");

    //Point::toDoubles(interpolated, x, y);

    //plt::plot(x, y, "-r");


    Ellipse ellipse = Ellipse(Point(1,11), Point(2,12), -5, -1);
    ellipse = ellipse.compute_frame();
    cout <<  "a = " << ellipse.geta() << endl;
    cout <<  "b = " << ellipse.getb() << endl;
    cout <<  "theta = " << ellipse.gettheta() << endl;
    cout <<  "center = " << ellipse.getcenter() << endl;
    doubles ex, ey;
    Point::toDoubles(ellipse.evaluate(100), ex ,ey);
    plt::plot(ex, ey, "-r");

    doubles cpx, cpy;
    Point::toDoubles(bspline.getCParray(), cpx ,cpy);
    //plt::plot(cpx, cpy, "+b");
    plt::axis("equal");
    plt::grid(true);
    plt::show();



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
    Utils::printcout(u1.getSequence({5}));

    cout << "-------------------------------------" << endl;
    UniformFixedKS u2 = UniformFixedKS(11, 12, 5);
    Utils::printcout(u2.getSequence());

    cout << "-------------------------------------" << endl;
    ValueFixedKS u4 = ValueFixedKS(11);
    Utils::printcout(u4.getSequence());
    Utils::printcout(u4.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    MultiplicityFixedValueKS u5 = MultiplicityFixedValueKS(10, 5);
    Utils::printcout(u5.getSequence());
    Utils::printcout(u5.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    MultiplicityValueKS u6 = MultiplicityValueKS(10);
    Utils::printcout(u6.getSequence({5}));

    cout << "-------------------------------------" << endl;
    BeginKS u7 = BeginKS(3);
    Utils::printcout(u7.getSequence());
    Utils::printcout(u7.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    EndKS u8 = EndKS(3);
    Utils::printcout(u8.getSequence());
    Utils::printcout(u8.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    RationalFixedKS u9 = RationalFixedKS(1, 2, 6, 1.5);
    Utils::printcout(u9.getSequence());
    Utils::printcout(u9.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-------------------------------------" << endl;
    RationalKS u10 = RationalKS(1, 2, 6);
    Utils::printcout(u10.getSequence({1.5}));

    cout << "-------------------------------------" << endl;
    BiRationalFixedKS u11 = BiRationalFixedKS(12, 20, 3, 1.5, 1.5, 13);
    Utils::printcout(u11.getSequence());
    Utils::printcout(u11.BaseFixedKnotSequence::getSequence({10})); // useless but should work

    cout << "-----------------birational--------------------" << endl;
    BiRationalKS u12 = BiRationalKS(0, 1, 2);
    Utils::printcout(u12.getSequence({1, 2, 0.5}));

    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    Knots knots;
    knots.push_back(dynamic_cast<BaseKnotSequence*>(&u1));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&u4));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&u2));
    knots.push_back(dynamic_cast<BaseFixedKnotSequence*>(&u11));
    KnotSequences u = KnotSequences(knots);
    Utils::printcout(u.getSequence({3}));

    //plt::plot({1,3,2,4});
    //plt::show();

//    cout << "-------------------------------------" << endl;
//    cout << "-------------------------------------" << endl;
//    cout << "-------------------------------------" << endl;
//    doubles x = {0, 1, 2, 3};
//    doubles y = {0, 1, 10, 1000};
//
//    doubles xx = Utils::linspace(0, 3, 3);
//
//    Utils::printcout(Utils::interp1(x, y, xx));


    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;

    string teststr = "    key     =    sdfjk sdfjk hbsdf        dsf         emfk            nkj      # comment    ";

    cout << Utils::fixline(teststr) << endl;

    Settings s = Settings("Script_Parameters.txt");

    for(double i: s.getdoubles("multipoint-weights-objective1"))
        cout<< i << "|||";

    cout << endl;
    cout << "parallel doe " << s.getbool("parallel-doe");

    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;
    cout << "-------------------------------------" << endl;

    vector<int> myints = { 10, 20, 30, 10, 40 };

    ints indexes = Utils::find_all(myints, 10);

    auto aaaaa = Utils::contains<bool>((vector<bool>){true, true, true, false}, true);

    for(auto pp: indexes)
        cout << pp << endl;

    Point pointttt = Point(0.0, 1000.0);
    vector<Point> pps;
    pps.reserve(4);
    pps.push_back(p1);
    pps.push_back(pointttt);
    pps.push_back(Point(0.001, 1.11));
    pps.push_back(Point(-1, -11));
    cout << Utils::print(pps, " ", true);

    cout << "-------------------------------------" << endl;
    cout << pps[1].to_str() << endl;
//    cout << pps[0]->x << endl;
//    cout << pps[0]->y << endl;
//    pps[3]->print();

    cout << "-------------------------------------" << endl;
    cout << pointttt << endl;
    cout << pointttt.to_str() << endl;
    cout << pointttt << endl;
    cout << pointttt << endl;
    pointttt.print();


    return 0;
}








