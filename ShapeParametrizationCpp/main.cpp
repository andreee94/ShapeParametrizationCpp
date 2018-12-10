#include <iostream>
#include "Line.h"
#include "Point.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    Point p = Point(10, 10);
    Line l = Line(p, 3);
    cout << l.getm() << endl;
    cout << l.getq() << endl;


    return 0;
}
