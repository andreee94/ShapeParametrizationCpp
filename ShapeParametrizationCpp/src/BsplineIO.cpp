#include "Bspline.h"
#include "Point.h"
#include "Line.h"
#include "Utils.h"
#include "Settings.h"
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
typedef std::vector<string> strings;



void Bspline::IOsave(string filename) const
{
    Points Normals = this->getnormalsInCP();
    ofstream file;
    file.open(filename, ios::out);
    //int margin = 5;

    file << "degree=" << this->n << endl;
    file << "numberCP=" << this->CParray.size() << endl;
    file << "uarray=" << Utils::print(this->uarray, " ", false) << endl; // TODO implement this kind of print

//    string ss = Utils::print(this->CParray, " ", false);
//    string sss = pystring::join(" ", this->getCParray_str());
    file << "rangemin=" << Utils::print(this->Editable::minRange, " ", false) << endl;
    file << "rangemax=" << Utils::print(this->Editable::maxRange, " ", false) << endl;
    file << "adjustableindices=" << Utils::print(this->Editable::adjustableIndices, " ", false) << endl;

    // longest double string is -2.2250738585072020E−308 which has 24 characters;
//    file << "Index" << setw(5 + margin);
//    file << "CPx" << setw(24 + margin) << "CPy" << setw(24 + margin);
//    file << "NormalsX" << setw(24 + margin) << "NormalsY" << setw(24 + margin);
//    file << "IsFixed" << setw(7 + margin);
//    file << "RangeMin" << setw(24 + margin) << "RangeMax" << setw(24 + margin);
//    file << endl;
//    for (unsigned int i = 0; i < this->CParray.size(); i++)
//    {
//        file << i + 1 << setw(5 + margin); // index
//        file << this->CParray[i].x << setw(24 + margin) << this->CParray[i].y << setw(24 + margin); // CPx, CPy
//        file << Normals[i].x << setw(24 + margin) << Normals[i].y << setw(24 + margin); // NormalsX, NormalsY
//        //file << this->isFixedCP(i);
//        //file << this->minRange[i] << this->maxRange[i];
//        file << endl;
//    }
    file.close();
}


Bspline Bspline::IOload(string filename)
{
    Settings s = Settings(filename);
    Bspline bspline = Bspline();
    bspline.n = s.getint("degree");
    bspline.uarray = s.getdoubles("uarray");
    bspline.CParray = s.getpoints("CParray");
    //bspline.normals = s.getpoints("normals") //TODO think if is convenient to save and load instead of to evaluate
    bspline.Editable::minRange = s.getdoubles("rangemin");
    bspline.Editable::maxRange = s.getdoubles("rangemax");
    bspline.Editable::adjustableIndices = s.getints("adjustableindices");
    return bspline;
}


strings Bspline::getCParray_str() const
{
    strings res;
    res.reserve(this->CParray.size());
    for (int i = 0; i < CParray.size(); i++)
        res[i] = CParray[i].to_str();
    return res;
}


Settings Bspline::getSettings() const
{
    return this->settings;
}

void Bspline::setSettings(Settings s)
{
    this->settings = s;
}

void Bspline::setSettings(string filename)
{
    this->settings = Settings(filename);
}












