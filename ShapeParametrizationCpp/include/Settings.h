#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <map>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<float> floats;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;


class Settings
{
    public:
        /** Default constructor */
        Settings();
        Settings(string filename);
        /** Default destructor */
        virtual ~Settings();

        string getstring(string key);
        int getint(string key);
        float getfloat(string key);
        double getdouble(string key);
        bool getbool(string key);
        Point getpoint(string key);

        strings getlist(string key);

        ints getints(string key);
        floats getfloats(string key);
        doubles getdoubles(string key);
        bools getbools(string key);
        Points getpoints(string key);

        // IO methods
        Settings save(string filename);
        static Settings load(string filename);

    protected:
        map<string, string> dict;

    private:
};

#endif // SETTINGS_H
