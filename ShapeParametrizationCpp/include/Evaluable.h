#ifndef EVALUABLE_H
#define EVALUABLE_H
#include <vector>
#include <Point.h>

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

class Evaluable
{
    public:
        /** Default constructor */
        Evaluable()
        {
        }

        Points evaluate(const doubles& values) const;
        Points evaluate(double start, double end, int steps) const;
        virtual Point evaluate(double) const = 0;
        virtual Points evaluate(int steps) const = 0;

    protected:

    private:
};

#endif // EVALUABLE_H
