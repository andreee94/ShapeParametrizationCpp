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
#include <cstdio>
#include <iterator>
#include <utility>


using namespace std;

class Point;

typedef std::vector<double> doubles;
typedef std::vector<int> ints;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;


class Utils
{
    public:
        static doubles linspace(double start_in, double end_in, int num_in, bool includeFirst=true, bool includeLast=true);
        static doubles extractmid(const doubles &items, int offset=1);
        //static doubles extractmid_self(doubles &items, int offset=1);
        static doubles extract(const doubles &items, int offsetstart, int offsetend);
        //static doubles extract_self(doubles &items, int offsetstart, int offsetend);
        static doubles centripetal(const Points &points, double alpha=0.5);
        static doubles chordlength(const Points &points);
        static doubles interp1(const doubles &x, const doubles &y, const doubles &x_new );
        static int findNearestNeighbourIndex(const double value, const doubles &x );
        static doubles computeCumul(double start, double end, doubles params);
        static void printcout(doubles vector, string separator=" ", bool endline=true);
        static bool eq(double d1, double d2, double threshold=1e-15);
        static bool eqzero(double d1, double threshold=1e-15);
        static bool lessoreq(double d1, double d2, double threshold=1e-15);
        static bool less(double d1, double d2, double threshold=1e-15);
        static bool greateroreq(double d1, double d2, double threshold=1e-15);
        static bool greater(double d1, double d2, double threshold=1e-15);
        static void getupperlowercurves(const Points &points, Points &lower, Points &upper);
        static void getminmaxindexes(const Points &points, int &minindex, int &maxindex, char XorY);
        static void splitcurve(const Points &points, int indexfirst, int indexlast, Points &curve1, Points &curve2);
        static Points getpointswithoutTE(const Points &curve1, const Points &curve2, const Point &first, const Point &last, Points &TEPoints);
        static Points getpointswithoutTE(const Points &points, int indexfirst, int indexlast, Points &TEPoints); // call together split curve and getpointswithoutTE
        static string fixline(string line);
        static string tostring( std::ostream& str );
        static strings tostring(const doubles &values);
        static strings tostring(const ints &values);
        static strings tostring(const bools &values);
        static bool getbool(string str);
        static bool iszeroint(string str);
        static doubles rational(double start, double end, double q, int numpoints, bool startIncluded = true, bool endIncluded = true);
        static doubles birational(double start, double end, double q1, double q2, double center, int numpoints, bool startIncluded = true, bool endIncluded = true, bool centerIncluded = true);



        static constexpr unsigned int str2int(const char* str, int h = 0)
        {
            return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
        }

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

        template<class T>
        static void move(vector<T> &items, size_t source, size_t dest)
        {
            // move item from index source to index dest
            rotate(items.begin()+dest, items.begin() + source, items.begin() + source+1);
        }

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

        template<class T>
        static int indexof(vector<T> items, const T item)
        {
            auto begin = items.begin();
            auto end = items.end();

            for(int index = 0; begin != end; ++begin, ++index)
            {
                if ((*begin) == item)
                {
                    return index;
                }
            }
            return -1;
        }

        template<typename T>
        static std::vector<T> slice(std::vector<T> const &v, int start, int end)
        {
            auto first = v.cbegin() + start;
            auto last = v.cbegin() + end;

            std::vector<T> vec(first, last);
            return vec;
        }
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////
        /// ENUMERATE

        template <class Iterator>
        struct enumerate_iterator {
          using iterator = Iterator;
          using index_type = typename std::iterator_traits<iterator>::difference_type;
          using reference = typename std::iterator_traits<iterator>::reference;

          enumerate_iterator(index_type index, iterator iterator)
              : index(index), iter(iterator) {}

          enumerate_iterator &operator++() {
            ++index;
            ++iter;
            return *this;
          }

          bool operator!=(const enumerate_iterator &other) const {
            return iter != other.iter;
          }

          std::pair<index_type &, reference> operator*() { return {index, *iter}; }

         private:
          index_type index;
          iterator iter;
        };

        template <class Iterator>
        struct enumerate_range {
          using index_type = typename std::iterator_traits<Iterator>::difference_type;
          using iterator = enumerate_iterator<Iterator>;

          enumerate_range(Iterator first, Iterator last, index_type initial)
              : first(first), last(last), initial(initial) {}

          iterator begin() const { return iterator(initial, first); }

          iterator end() const { return iterator(0, last); }

         private:
          Iterator first;
          Iterator last;
          index_type initial;
        };

        template <class Iterator>
        static decltype(auto) enumerate(
            Iterator first, Iterator last,
            typename std::iterator_traits<Iterator>::difference_type initial) {
          return enumerate_range(first, last, initial);
        }

        template <class Container>
        static decltype(auto) enumerate(Container &content) {
          return enumerate_range(std::begin(content), std::end(content), 0);
        }

        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////

    protected:

    private:
        Utils() {}
};

#endif // UTILS_H
