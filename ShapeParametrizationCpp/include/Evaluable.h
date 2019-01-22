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
        Evaluable() :
            parallel(true),
            coresNumber(4)
        {
        }

        Points evaluate(const doubles& values) const;
        Points evaluate(double start, double end, int steps) const;
        virtual Point evaluate(double) const = 0;
        virtual Points evaluate(int steps) const = 0;

        bool isParallel() const;
        void setParallel(bool value);

        unsigned int getCoresNumber() const;
        void setCoresNumber(unsigned int value);

protected:
        bool parallel = true;
        unsigned int coresNumber;
private:
};

#endif // EVALUABLE_H
