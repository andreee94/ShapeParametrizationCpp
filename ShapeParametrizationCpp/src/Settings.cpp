#include "Settings.h"

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <map>
#include <string>
#include <pystring.h>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<float> floats;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;
typedef std::vector<BaseKnotSequence*> Knots;

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


// copy constructor
Settings::Settings(const Settings& other)
{
    this->dict = other.dict;
}

Settings& Settings::operator=(const Settings& other)
{
    if (this == &other) return *this; // handle self assignment
    this->dict = other.dict;
    return *this;
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

Knots Settings::getknots(string key)
{
    int knotsnum = getint(key + "-num");
    Knots knots(knotsnum);
    for (int i = 0; i < knotsnum; i++)
    {
        BaseKnotSequence * knot = BaseKnotSequence::makeKnot(getstring(key + std::to_string(i) + "-class"));
        knots.push_back(knot);
    }
    return knots;
}

strings Settings::getstrings(string key)
{
    return this->getlist(key);
}

void Settings::setvalue(string key, string value)
{
    this->dict[key] = value;
}

void Settings::setvalue(string key, bool value)
{
    this->dict[key] = std::to_string(value);
}

void Settings::setvalue(string key, int value)
{
    this->dict[key] = std::to_string(value);
}

void Settings::setvalue(string key, size_t value)
{
    this->dict[key] = std::to_string(value);
}

void Settings::setvalue(string key, float value)
{
    this->dict[key] = std::to_string(value);
}

void Settings::setvalue(string key, double value)
{
    this->dict[key] = std::to_string(value);
}

void Settings::setvalue(string key, BaseKnotSequence::ParamType value)
{
    this->dict[key] = std::to_string((int)value);
}

void Settings::setvalues(string key, const strings &values)
{
    this->dict[key] = pystring::join(" ", values);
}

void Settings::setvalues(string key, const doubles &values)
{
    this->dict[key] = pystring::join(" ", Utils::tostring(values));
}

void Settings::setvalues(string key, const ints &values)
{
    this->dict[key] = pystring::join(" ", Utils::tostring(values));
}

void Settings::setvalues(string key, const bools &values)
{
    this->dict[key] = pystring::join(" ", Utils::tostring(values));
}

void Settings::setvalues(string key, const Knots &knots)
{
    setvalue(key + "-num", knots.size());
    for (auto const [index, knot] : Utils::enumerate(knots))
    {
        if (knot->isFixed())
        {
            setvalue(key + std::to_string(index) + "-class", typeid(knot).name()); // save the knot type
            strings knot_values = knot->getValuesStrings(); // extract all the properties values as strings
            for (auto const [kindex, kvalue] : Utils::enumerate(knot_values))
                setvalue(key + std::to_string(index) + "-prop" + std::to_string(kindex), kvalue);

            setvalues(key + std::to_string(index) + "-prop-types", knot->propTypes());
        }
        else
        {
            setvalue(key + std::to_string(index) + "-class", typeid(knot).name()); // save the knot type
        }
    }
}

void Settings::setvalues(string key, const vector<BaseKnotSequence::ParamType> &values)
{
    ints enumints(values.size());
    for (auto v : values)
        enumints.push_back((int)v);
    this->dict[key] = pystring::join(" ", Utils::tostring(enumints));
}

Settings Settings::load(string filename)
{
    return Settings(filename);
}

Settings Settings::save(string filename)
{
    ofstream file;
    file.open(filename, ios::out);
    for( auto const& [key, val] : this->dict)
        file << key << "=" << this->dict[key] << endl;
    file.close();
}












