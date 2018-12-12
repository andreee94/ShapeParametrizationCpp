#ifndef UTILS_H
#define UTILS_H

#include <vector>
typedef std::vector<double> doubles;


class Utils
{
    public:
        static doubles linspace(double start_in, double end_in, int num_in);
        static doubles extractmid(const doubles items, int offset=1);

    protected:

    private:
        Utils() {}
};

#endif // UTILS_H
