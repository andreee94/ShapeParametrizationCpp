#ifndef SETTINGS_H
#define SETTINGS_H

#include "BaseKnotSequence.h"

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
typedef std::vector<BaseKnotSequence*> Knots;


class Settings
{
    public:
        /** Default constructor */
        Settings();
        Settings(string filename);
        /** Copy constructor
         *  \param other Object to copy from
         */
        Settings(const Settings& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Settings& operator=(const Settings& other);
        /** Default destructor */
        virtual ~Settings();

        string getstring(string key);
        int getint(string key);
        float getfloat(string key);
        double getdouble(string key);
        bool getbool(string key);
        Point getpoint(string key);

        strings getlist(string key);

        strings getstrings(string key);
        ints getints(string key);
        floats getfloats(string key);
        doubles getdoubles(string key);
        bools getbools(string key);
        Points getpoints(string key);
        Knots getknots(string key);

        void setvalue(string key, string value);
        void setvalue(string key, bool value);
        void setvalue(string key, int value);
        void setvalue(string key, size_t value);
        void setvalue(string key, float value);
        void setvalue(string key, double value);
        void setvalue(string key, BaseKnotSequence::ParamType value);
        void setvalues(string key, const strings &values);
        void setvalues(string key, const doubles &values);
        void setvalues(string key, const ints &values);
        void setvalues(string key, const bools &values);
        void setvalues(string key, const Knots &values);
        void setvalues(string key, const vector<BaseKnotSequence::ParamType> &values);

        // IO methods
        Settings save(string filename);
        static Settings load(string filename);

    protected:
        map<string, string> dict;

    private:
};

#endif // SETTINGS_H
