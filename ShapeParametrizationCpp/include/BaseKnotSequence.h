#ifndef BASEKNOTSEQUENCE_H
#define BASEKNOTSEQUENCE_H
#include <vector>
#include <cmath>
#include <Point.h>

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

class BaseKnotSequence
{
    public:
        /** Default constructor */
        BaseKnotSequence();
        BaseKnotSequence(int  numParams, double start, double end);
        virtual doubles getSequence(doubles params) = 0;

        int getNumParams() const { return numParams;}
        double getStart() const { return start;}
        double getEnd() const { return end;}

    protected:
        int numParams;
        //doubles params;
        double start;
        double end;

    private:
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BaseFixedKnotSequence: public BaseKnotSequence
{
    public:
        BaseFixedKnotSequence(double start, double end);
        virtual doubles getSequence() = 0;
        doubles getSequence(doubles params)
        {
            return this->getSequence();
        }

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class UniformKS: public BaseKnotSequence
{
    public:
        UniformKS(double start, double end);
        doubles getSequence(doubles params);

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class UniformFixedKS: public BaseFixedKnotSequence, public UniformKS
{
    public:
        UniformFixedKS(double start, double end, int steps);
        doubles getSequence();

    protected:
        int steps;

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class ValueFixedKS: public BaseFixedKnotSequence
{
    public:
        ValueFixedKS(double value);
        doubles getSequence();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class MultiplicityFixedValueKS: public UniformFixedKS
{
    public:
        MultiplicityFixedValueKS(double value, int multiplicity);
        doubles getSequence();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


class MultiplicityValueKS: public UniformKS
{
    public:
        MultiplicityValueKS(double value);
        doubles getSequence(doubles params);

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BeginKS: public MultiplicityFixedValueKS // initial zeros
{
    public:
        BeginKS(int bspline_n);
        doubles getSequence();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class EndKS: public MultiplicityFixedValueKS // final zeros
{
    public:
        EndKS(int bspline_n);
        doubles getSequence();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class RationalKS: public BaseKnotSequence
{
    public:
        RationalKS(double start, double end, int numpoints);
        doubles getSequence(doubles params);

    protected:
        int numpoints;
    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class RationalFixedKS: public BaseFixedKnotSequence, public RationalKS
{
    public:
        RationalFixedKS(double start, double end, int numpoints, double q);
        doubles getSequence();

    protected:
        double q;
    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BiRationalKS: public RationalKS
{
    public:
        BiRationalKS(double start, double end, int numpoints);
        doubles getSequence(doubles params);

    protected:
        int numpoints;
    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BiRationalFixedKS: public BaseFixedKnotSequence, public BiRationalKS
{
    public:
        BiRationalFixedKS(double start, double end, int numpoints, double q1, double q2, double center);
        // by defaults if not specified, the sequence is symmetric respect to the center, so q, 1/q
        BiRationalFixedKS(double start, double end, int numpoints, double q, double center)
        :BiRationalFixedKS(start, end, numpoints, q, 1 / q, center){};
        doubles getSequence();

    protected:
        double q1, q2;
        double center;
    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif // BASEKNOTSEQUENCE_H
