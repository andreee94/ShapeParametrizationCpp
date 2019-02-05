#include "Utils.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <math.h>
#include <pystring.h>
#include <boost/lexical_cast.hpp>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<float> floats;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;

doubles Utils::linspace(double start_in, double end_in, int num_in, bool includeFirst, bool includeLast)
{
  doubles linspaced;

  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);

  if (num == 0) { return linspaced; }
  if (num == 1)
    {
      linspaced.push_back(start);
      return linspaced;
    }

  double delta = (end - start) / (num - 1);

  int offset = includeFirst ? 0 : 1;
  for(int i=offset; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  if (includeLast)
    linspaced.push_back(end); // I want to ensure that start and end
                            // are exactly the same as the input
  return linspaced;
}

doubles Utils::extractmid(const doubles &items, int offset)
{
    return Utils::extract(items, offset, offset);
}

doubles Utils::extract(const doubles &items, int offsetstart, int offsetend)
{
    doubles::const_iterator first = items.begin() + offsetstart;
    doubles::const_iterator last = items.end() - offsetend;
    doubles newitems(first, last);
    return newitems;
}


 doubles Utils::centripetal(const Points &points, double alpha)
 {
    doubles w;
    double totallenght = 0;
    w.push_back(0);
    for (unsigned int i = 1; i< points.size(); i++) // skip i = 0
    {
        double previous_dist = points[i].distance(points[i-1]);
        previous_dist = pow(previous_dist, alpha);
        w.push_back(w[i-1] + previous_dist);
        totallenght += previous_dist;
    }
    // now we need to normalize respect to total lenght
    doubles w_norm(w.size());
    for(unsigned int i = 0; i < w.size(); i++)
        w_norm[i] = w[i] / totallenght;
    return w_norm;
 }

 doubles Utils::chordlength(const Points &points)
 {
    // centripetal is with alpha 0.5
    // chordlength is with alpha 1
    return Utils::centripetal(points, 1);
 }



doubles Utils::interp1(const doubles &x,const doubles &y,const doubles &x_new )
{
    doubles y_new;
    y_new.reserve( x_new.size() );

    doubles dx, dy, slope, intercept;
    dx.reserve( x.size() );
    dy.reserve( x.size() );
    slope.reserve( x.size() );
    intercept.reserve( x.size() );
    for(unsigned int i = 0; i < x.size(); ++i ){
        if( i < x.size()-1 )
        {
            dx.push_back( x[i+1] - x[i] );
            dy.push_back( y[i+1] - y[i] );
            slope.push_back( dy[i] / dx[i] );
            intercept.push_back( y[i] - x[i] * slope[i] );
        }
        else
        {
            dx.push_back( dx[i-1] );
            dy.push_back( dy[i-1] );
            slope.push_back( slope[i-1] );
            intercept.push_back( intercept[i-1] );
        }
    }

    for (unsigned int i = 0; i < x_new.size(); ++i )
    {
        int idx = findNearestNeighbourIndex( x_new[i], x );
        y_new.push_back( slope[idx] * x_new[i] + intercept[idx] );
    }
    return y_new;
}

int Utils::findNearestNeighbourIndex(const double value, const doubles &x )
{
    double dist = 1e100;
    int idx = -1;
    for (unsigned int i = 0; i < x.size(); ++i ) {
        double newDist = value - x[i];
        if ( newDist > 0 && newDist < dist ) {
            dist = newDist;
            idx = i;
        }
    }

    return idx;
}


doubles Utils::computeCumul(double start, double end, doubles params)
{
    doubles cumul(params);
    cumul[0] = params[0];
    // compute cumulate
    for (unsigned int i = 1; i < params.size(); i++)
        cumul[i] = cumul[i-1] + params[i];
    // normalize into (start-end)
    double cumul0 = cumul[0];
    double k = (end - start) / (cumul[cumul.size()-1] - cumul[0]); // conversion of the scale
    for (unsigned int i = 0; i < params.size(); i++)
        cumul[i] = start + (cumul[i] - cumul0) * k;
    return cumul;
}


void Utils::getminmaxindexes(const Points &points, int &minindex, int &maxindex, char XorY)
{
    double minvalue = INT_MAX;
    double maxvalue = INT_MIN;
    for (unsigned int i = 0; i < points.size(); i++)
    {
        if (XorY == 'X')
        {
            if (points[i].getx() > maxvalue)
            {
                maxvalue = points[i].getx();
                maxindex = i;
            }
            if (points[i].getx() < minvalue)
            {
                minvalue = points[i].getx();
                minindex = i;
            }
        }
        else if (XorY == 'Y')
        {
            if (points[i].gety() > maxvalue)
            {
                maxvalue = points[i].gety();
                maxindex = i;
            }
            if (points[i].gety() < minvalue)
            {
                minvalue = points[i].gety();
                minindex = i;
            }
        }
    }
}

void Utils::getupperlowercurves(const Points &points, Points &lower, Points &upper)
{
    int index_minX;
    int index_maxX;
    getminmaxindexes(points, index_minX, index_maxX, 'X');
    lower.clear();
    upper.clear();
    if (index_minX > index_maxX)
    {
        lower = vector<Point>(points.begin() + index_maxX, points.begin() + index_minX);
        std::reverse(lower.begin(), lower.end()); // points from left to right
        upper = vector<Point>(points.begin() + index_minX, points.end());
        upper.insert(upper.end(), points.begin(), points.begin() + index_maxX);

    }
    else if (index_minX > index_maxX)
    {
        lower = vector<Point>(points.begin() + index_maxX, points.end());
        lower.insert(lower.end(), points.begin(), points.begin() + index_minX);
        std::reverse(lower.begin(), lower.end()); // points from left to right
        upper = vector<Point>(points.begin() + index_minX, points.begin() + index_maxX);
    }
}

 void Utils::printcout(doubles vec, string separator, bool endline)
 {
    for (const auto& i: vec)
        cout << i << ' ';
    if (endline)
        cout << endl;
 }

 bool Utils::eq(double d1, double d2, double threshold)
 {
     return std::abs(d1 - d2) < threshold;
 }

 bool Utils::eqzero(double d1, double threshold)
 {
     return std::abs(d1) < threshold;
 }

 bool Utils::lessoreq(double d1, double d2, double threshold)
 {
     return less(d1, d2, threshold) || eq(d1, d2, threshold);
 }

 bool Utils::less(double d1, double d2, double threshold)
 {
     return d1 - d2 < 0;
 }

 bool Utils::greateroreq(double d1, double d2, double threshold)
 {
     return greater(d1, d2, threshold) || eq(d1, d2, threshold);
 }

 bool Utils::greater(double d1, double d2, double threshold)
 {
     return d1 - d2 > 0;
 }

//string Utils::print(doubles vec, string separator, bool endline)
// {
//    std::stringstream out;
//    for (const auto& i: vec)
//        out << i << separator;
//    if (endline)
//        out << endl;
//    return out.str();
// }


string Utils::fixline(string line)
{
    strings newline;
    pystring::split(line, newline, "#", 1); // remove comment

    string newline2 = newline[0];
    pystring::split(newline2, newline); // remove whitespaces and split

    string res = pystring::join(" ", newline); // concat splitted tokens
    return res;
}


string Utils::tostring( std::ostream& str )
{
    std::ostringstream ss;
    ss << str.rdbuf();
    return ss.str();
}

//template<class T>
//strings Utils::tostrings(vector<T> items) const
//{
//    strings res;
//    res.reserve(items.size());
//    for (int i = 0; i < items.size(); i++)
//        res[i] = (string)items[i];
//    return res;
//}


bool Utils::getbool(string str)
{
    vector<string> valid_bools = {"true", "1", "yes", "on"};
    if (Utils::contains(valid_bools, pystring::lower(str)))
        return true;
    if (Utils::iszeroint(str))
        return false;
    return true;

}

bool Utils::iszeroint(string str)
{
    try
    {
        long x = boost::lexical_cast<long>(str);
        return x == 0;
    }
    catch(...)
    {
        // it is not an int so non zero
        return false;
    }
}


//template<typename T>
//bool Utils::contains(vector<T> items, const T item)
//{
//    return std::find(items.begin(), items.end(), item) != items.end();
//}
//
//template<typename T>
//ints Utils::find_all(vector<T> items, const T item)
//{
//    auto begin = items.begin();
//    auto end = items.end();
//    ints result;
//
//    for(int index = 0; begin != end; ++begin, ++index)
//    {
//        if ((*begin) == item)
//        {
//            result.emplace_back(index);
//        }
//    }
//    return result;
//}
