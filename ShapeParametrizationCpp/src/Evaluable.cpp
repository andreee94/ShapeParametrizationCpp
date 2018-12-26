#include "Evaluable.h"

#include <Point.h>
#include "Utils.h"
#include <vector>

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;


Points Evaluable::evaluate(const doubles x) const
{
    Points points;
    points.reserve(x.size());
    for (unsigned int i = 0; i < x.size(); i++)
    {
        points[i] = this->evaluate(x[i]);
    }
    return points;
}

Points Evaluable::evaluate(double start, double end, int steps) const
{
    doubles x = Utils::linspace(start, end, steps);
    return evaluate(x);
}
