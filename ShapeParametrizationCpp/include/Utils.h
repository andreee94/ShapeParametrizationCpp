#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <Point.h>
#include <pystring.h>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include  <string>
#include  <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

class Point;

typedef std::vector<double> doubles;
typedef std::vector<int> ints;
typedef std::vector<string> strings;
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

//        template<class T>
//        static string print(vector<T> vec, string separator, bool endline=true)
//         {
//            string out;
//            out = pystring::join(separator, Utils::tostrings(vec));
//            if (endline)
//                out = out + "\n";
//            return out;
//         }

        template<class T>
        static string print(vector<T> vec, string separator, bool endline=true)
        {
            stringstream out;
            for (const auto& i: vec)
                out << i << separator;
            if (endline)
                out << endl;
            return out.str();
//            return Utils::tostring(out);
        }

        static string fixline(string line);
        static string tostring( std::ostream& str );
//        template<class T>
//        static strings tostrings(vector<T> items)
//        {
//            strings res;
//            res.reserve(items.size());
//            for (int i = 0; i < items.size(); i++)
//                res[i] = (string)items[i];
//            return res;
//        }


        static bool getbool(string str);
        static bool iszeroint(string str);

//        template<typename T>
//        static ints find_all(vector<T> items, const T item);
//
//        template<typename T>
//        static bool contains(vector<T> items, const T item);


        template<class T>
        static bool contains(vector<T> items, const T item)
        {
            return find(items.begin(), items.end(), item) != items.end();
        }

        template<class T>
        static ints find_all(vector<T> items, const T item)
        {
            auto begin = items.begin();
            auto end = items.end();
            ints result;

            for(int index = 0; begin != end; ++begin, ++index)
            {
                if ((*begin) == item)
                {
                    result.emplace_back(index);
                }
            }
            return result;
        }

    protected:

    private:
        Utils() {}
};

#endif // UTILS_H
