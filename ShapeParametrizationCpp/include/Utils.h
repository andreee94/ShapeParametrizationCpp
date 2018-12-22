#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <Point.h>
typedef std::vector<double> doubles;
typedef std::vector<Point> Points;


class Utils
{
    public:
        static doubles linspace(double start_in, double end_in, int num_in);
        static doubles extractmid(const doubles items, int offset=1);
        static doubles centripetal(const Points points, int alpha=1);
        static void print(doubles vector);

    protected:

    private:
        Utils() {}
};

#endif // UTILS_H
