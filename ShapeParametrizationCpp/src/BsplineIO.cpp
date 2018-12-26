#include "Bspline.h"
#include "Point.h"
#include "Line.h"
#include "Utils.h"
#include <cmath>

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <array>

#include <iostream>
#include  <string>
#include  <fstream>
#include <iostream>
#include <iomanip>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;



void Bspline::IOsave(string filename) const
{
    Points Normals = this->getnormalsInCP();
    ofstream file;
    file.open(filename, ios::out);
    int margin = 5;

    file << "degree=" << this->n << endl;
    file << "numberCP=" << this->CParray.size() << endl;
    file << "uarray=" << Utils::print(this->uarray, " ", true); // TODO implement this kind of print

    // longest double string is -2.2250738585072020E−308 which has 24 characters;
    file << "Index" << setw(5 + margin);
    file << "CPx" << setw(24 + margin) << "CPy" << setw(24 + margin);
    file << "NormalsX" << setw(24 + margin) << "NormalsY" << setw(24 + margin);
    file << "IsFixed" << setw(7 + margin);
    file << "RangeMin" << setw(24 + margin) << "RangeMax" << setw(24 + margin);
    file << endl;
    for (unsigned int i = 0; i < this->CParray.size(); i++)
    {
        file << i + 1 << setw(5 + margin); // index
        file << this->CParray[i].x << setw(24 + margin) << this->CParray[i].y << setw(24 + margin); // CPx, CPy
        file << Normals[i].x << setw(24 + margin) << Normals[i].y << setw(24 + margin); // NormalsX, NormalsY
        //file << this->isFixedCP(i);
        //file << this->minRange[i] << this->maxRange[i];
        file << endl;
    }
    file.close();
}


Bspline Bspline::IOload(string filename)
{
}
