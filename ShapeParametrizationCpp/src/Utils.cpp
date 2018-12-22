#include "Utils.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

typedef std::vector<double> doubles;
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

doubles Utils::extractmid(const doubles items, int offset)
{
    doubles::const_iterator first = items.begin() + offset;
    doubles::const_iterator last = items.end() - offset;
    doubles newitems(first, last);
    return newitems;
}



 doubles Utils::centripetal(const Points points, int alpha)
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


 void Utils::print(doubles vec)
 {
    for (const auto& i: vec)
        cout << i << ' ';
    cout << endl;
 }

