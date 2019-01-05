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
    file.close();

    //for( auto const& [key, val] : this->dict)
    //   cout << key << "=" << this->dict[key] << endl;
}

bool Settings::getbool(string key)
{
    return Utils::getbool(this->dict[key]);
}

int Settings::getint(string key)
{
    return std::stoi(this->dict[key]);
}

float Settings::getfloat(string key)
{
    return std::stof(this->dict[key]);
}

double Settings::getdouble(string key)
{
    return std::stod(this->dict[key]);
}

string Settings::getstring(string key)
{
    return this->dict[key];
}

Point Settings::getpoint(string key)
{
    return Point(this->dict[key]);
}

strings Settings::getlist(string key)
{
    strings ss;
    pystring::split(this->getstring(key), ss, " ");
    return ss;
}

Settings::~Settings()
{

}

ints Settings::getints(string key)
{
    strings ss = this->getlist(key);
    ints res(ss.size());
    for(unsigned int i = 0; i < ss.size(); ++i )
    {
        res[i] = std::stoi(ss[i]);
    }
    return res;
}

floats Settings::getfloats(string key)
{
    strings ss = this->getlist(key);
    floats res(ss.size());
    for(unsigned int i = 0; i < ss.size(); ++i )
    {
        res[i] = std::stof(ss[i]);
    }
    return res;
}

doubles Settings::getdoubles(string key)
{
    strings ss = this->getlist(key);
    doubles res(ss.size());
    for(unsigned int i = 0; i < ss.size(); ++i )
    {
        res[i] = std::stod(ss[i]);
    }
    return res;
}

bools Settings::getbools(string key)
{
    strings ss = this->getlist(key);
    bools res(ss.size());
    for(unsigned int i = 0; i < ss.size(); ++i )
    {
        res[i] = Utils::getbool(ss[i]);
    }
    return res;
}

Points Settings::getpoints(string key)
{
    strings ss = this->getlist(key);
    Points res(ss.size());
    for(unsigned int i = 0; i < ss.size(); ++i )
    {
        res[i] = Point(ss[i]);
    }
    return res;
}

strings Settings::getstrings(string key)
{
    return this->getlist(key);
}

Settings Settings::load(string filename)
{
    return Settings(filename);
}












