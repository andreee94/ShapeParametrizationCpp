#include "Utils.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <math.h>
#include <pystring.h>

using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<float> floats;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;

doubles Utils::linspace(double start_in, double end_in, int num_in)
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

  for(int i=0; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  linspaced.push_back(end); // I want to ensure that start and end
                            // are exactly the same as the input
  return linspaced;
}

doubles Utils::extractmid(const doubles &items, int offset)
{
    doubles::const_iterator first = items.begin() + offset;
    doubles::const_iterator last = items.end() - offset;
    doubles newitems(first, last);
    return newitems;
}


 doubles Utils::centripetal(const Points &points, int alpha)
 {
    doubles w;
    double totallenght = 0;
    w.push_back(0);
    for (int i = 1; i< points.size(); i++) // skip i = 0
    {
        double previous_dist = points[i].distance(points[i-1]);
        previous_dist = pow(previous_dist, alpha);
        w.push_back(w[i-1] + previous_dist);
        totallenght += previous_dist;
    }
    // now we need to normalize respect to total lenght
    doubles w_norm(w.size());
    for(int i = 0; i < w.size(); i++)
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

    for ( int i = 0; i < x_new.size(); ++i )
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
    for ( int i = 0; i < x.size(); ++i ) {
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


 void Utils::printcout(doubles vec, string separator, bool endline)
 {
    for (const auto& i: vec)
        cout << i << ' ';
    if (endline)
        cout << endl;
 }

string Utils::print(doubles vec, string separator, bool endline)
 {
    std::stringstream out;
    for (const auto& i: vec)
        out << i << separator;
    if (endline)
        out << endl;
    return out.str();
 }


string Utils::fixline(string line)
{
    strings newline;
    pystring::split(line, newline, "#", 1); // remove comment

    string newline2 = newline[0];
    pystring::split(newline2, newline); // remove whitespaces and split

    string res = pystring::join(" ", newline); // concat splitted tokens
    return res;
}


template<class T>
bool Utils::contains(vector<T> items, T item)
{
    return std::find(items.begin(), items.end(), item) != items.end();
}

