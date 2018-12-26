#include "Settings.h"

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <map>
#include <pystring.h>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<float> floats;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;

Settings::Settings()
{

}

Settings::Settings(string filename)
{
    ifstream file;
    file.open(filename, ios::in);

    string line;
    while (file)
    {
       getline(file, line);
       line = Utils::fixline(line);

       if (line.size() < 1)
            continue;

       strings ss;
       pystring::split(line, ss, "=");

       if (ss.size() < 2)
            continue;
       ss[0] = pystring::strip(ss[0]);
       this->dict[ss[0]] = pystring::strip(ss[1]);
    }

    //for( auto const& [key, val] : this->dict)
    //   cout << key << "=" << this->dict[key] << endl;
}

Settings::~Settings()
{

}
