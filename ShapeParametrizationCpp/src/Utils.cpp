#include "Utils.h"
#include <vector>

typedef std::vector<double> doubles;

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
