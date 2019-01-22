#include "Evaluable.h"

#include <Point.h>
#include "Utils.h"
#include <vector>
#include <algorithm>
#include <omp.h>

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;



Points Evaluable::evaluate(const doubles &x) const
{
    Points points(x.size(), 0);

    if (isParallel())
    {
        #pragma omp parallel for ordered schedule(dynamic)
        for (unsigned int i = 0; i < x.size(); i++)
        {
            points[i] = this->evaluate(x[i]);
            //int tid = omp_get_thread_num();
            //std::cout << "tid = " << tid << endl;
        }
    }
    else
    {
        for (unsigned int i = 0; i < x.size(); i++)
        {
            points[i] = this->evaluate(x[i]);
        }
    }
//    }
    return points;
}

Points Evaluable::evaluate(double start, double end, int steps) const
{
    doubles x = Utils::linspace(start, end, steps);
    return evaluate(x);
}

bool Evaluable::isParallel() const
{
    return parallel;
}

void Evaluable::setParallel(bool value)
{
    parallel = value;
}

unsigned int Evaluable::getCoresNumber() const
{
    return coresNumber;
}

void Evaluable::setCoresNumber(unsigned int value)
{
    coresNumber = value;
}
