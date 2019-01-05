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
        BaseKnotSequence(int numParams, double start, double end);
        virtual doubles getSequence(doubles params) = 0;

        virtual ~BaseKnotSequence (){}

        BaseKnotSequence& operator=(const BaseKnotSequence& other)
        {
            if (this == &other) return *this; // handle self assignment
            //assignment operator
            this->numParams = other.numParams;
            this->start = other.start;
            this->end = other.end;
            return *this;
        }

        // copy constructor
        BaseKnotSequence(const BaseKnotSequence& other)
        {
            this->numParams = other.numParams;
            this->start = other.start;
            this->end = other.end;
        }


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
        doubles getSequence(doubles params)
        {
            return BaseFixedKnotSequence::getSequence(params);
        }

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

        doubles getSequence() {
            doubles params = {(double)this->q};
            return RationalKS::getSequence(params);
        }

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

        doubles getSequence(){
            doubles params = {this->q1, this->q2, this->center};
            return BiRationalKS::getSequence(params);
        }

    protected:
        double q1, q2;
        double center;
    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CustomFixedKS: public BaseFixedKnotSequence
{
    public:
        CustomFixedKS(double start, double end, doubles sequence);
        doubles getSequence();

    protected:
        doubles sequence;
    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// it takes params, compute the cumulate sequence and normalize between start and end;
class CumulKS: public BaseKnotSequence
{
    public:
        CumulKS(double start, double end, int numParams);
        doubles getSequence(doubles params);

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// it takes params, compute the cumulate sequence and normalize between start and end;
class CumulFixedKS: public BaseFixedKnotSequence, public CumulKS
{
    public:
        CumulFixedKS(double start, double end, doubles params);

        doubles getSequence(){
            return this->CumulKS::getSequence(this->params);
        }

    protected:
        doubles params;

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif // BASEKNOTSEQUENCE_H
