#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <Point.h>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include  <string>
#include  <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

class Point;

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;


class Utils
{
    public:
        static doubles linspace(double start_in, double end_in, int num_in);
        static doubles extractmid(const doubles &items, int offset=1);
        static doubles centripetal(const Points &points, int alpha=0.5);
        static doubles chordlength(const Points &points);
        static doubles interp1(const doubles &x, const doubles &y, const doubles &x_new );
        static int findNearestNeighbourIndex(const double value, const doubles &x );
        static doubles computeCumul(double start, double end, doubles params);
        static void printcout(doubles vector, string separator=" ", bool endline=true);
        static string print(doubles vector, string separator=" ", bool endline=true);
        static string fixline(string line);

        template<class T>
        static bool contains(vector<T> items, T item);

    protected:

    private:
        Utils() {}
};

#endif // UTILS_H
