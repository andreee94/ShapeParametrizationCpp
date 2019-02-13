#ifndef BASEKNOTSEQUENCE_H
#define BASEKNOTSEQUENCE_H
#include <vector>
#include <cmath>
#include <Point.h>
#include <variant>
#include <typeinfo>
#include <map>

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

#define IMPLEMENT_CLONE(TYPE) \
    BaseKnotSequence* clone() const { return new TYPE(*this); }

#define MAKE_PROTOTYPE(TYPE) \
   BaseKnotSequence* TYPE ## _myProtoype = \
      BaseKnotSequence::addPrototype(#TYPE, new TYPE());

class BaseKnotSequence
{
    public:
        enum ParamType { INT = 1, DOUBLE = 2, BOOL = 3 };

        typedef void (virtual_init)();
        BaseKnotSequence(virtual_init vinit) { (*vinit)(); }

        // not fixed type constructor
        BaseKnotSequence()
            : start(0), end(1), numParams(0), fixed(false) { }
        BaseKnotSequence(int numParams, double start, double end)
            : start(start), end(end), numParams(numParams), fixed(false) { }
        BaseKnotSequence(double start, double end) // fixed type constructor
            : start(start), end(end), fixed(true) { }
        virtual ~BaseKnotSequence (){}

        BaseKnotSequence& operator=(const BaseKnotSequence& other)
        {
            if (this == &other) return *this; // handle self assignment
            //assignment operator
            this->fixed = other.fixed;
            this->numParams = other.numParams;
            this->start = other.start;
            this->end = other.end;
            this->startIncluded = other.startIncluded;
            this->endIncluded = other.endIncluded;
            this->property_names = other.property_names;
            this->property_types = other.property_types;
            return *this;
        }

        BaseKnotSequence(const BaseKnotSequence& other)
        {
            this->fixed = other.fixed;
            this->numParams = other.numParams;
            this->start = other.start;
            this->end = other.end;
            this->startIncluded = other.startIncluded;
            this->endIncluded = other.endIncluded;
            this->property_names = other.property_names;
            this->property_types = other.property_types;
        }

        virtual doubles getSequence(doubles params) = 0;
        virtual doubles getSequence() = 0;
        virtual void setValues(vector<std::variant<int, double, bool>> values) = 0;
        virtual vector<std::variant<int, double, bool>> getValues() = 0;
        strings getValuesStrings();

        bool isFixed() const { return fixed; }
        int getNumParams() const { return numParams;}
        double getStart() const { return start;}
        double getEnd() const { return end;}
        bool isStartIncluded() const { return startIncluded; }
        bool isEndIncluded() const { return endIncluded; }

        void setFixed(bool value) { this->fixed = value;}
        void setStart(double start) { this->start = start;}
        void setEnd(double end) { this->end = end;}
        void setStartIncluded(bool value) { startIncluded = value; }
        void setEndIncluded(bool value) { endIncluded = value; }

        virtual void initProps() = 0 ;
        vector<string> propNames(){ return property_names; }
        vector<ParamType> propTypes(){ return property_types; }
        string propName(size_t i){ return property_names[i]; }
        ParamType propType(size_t i){ return property_types[i]; }

        virtual BaseKnotSequence *clone() const = 0;
        static BaseKnotSequence* makeKnot(string type);
        static BaseKnotSequence* addPrototype(string type, BaseKnotSequence* p);
        static std::map<string, BaseKnotSequence*> protoTable;
        static bool find(string k, BaseKnotSequence*& v, const map<string, BaseKnotSequence*>& m);

    protected:
        vector<string> property_names;
        vector<ParamType> property_types;
        double start=0;
        double end=1;
        int numParams=0;
        bool fixed;
        bool startIncluded = true;
        bool endIncluded = true;
    private:
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//class BaseFixedKnotSequence: public BaseKnotSequence
//{
//    public:
//        enum ParamType { INT = 1, DOUBLE = 2, BOOL = 3 };

//        BaseFixedKnotSequence(double start, double end);
//        virtual doubles getSequence() = 0;
//        doubles getSequence(doubles params)
//        {
//            return this->getSequence();
//        }
//        vector<string> propNames(){ return property_names; }
//        vector<ParamType> propTypes(){ return property_types; }
//        string propName(size_t i){ return property_names[i]; }
//        ParamType propType(size_t i){ return property_types[i]; }
//        virtual void setValues(vector<std::variant<int, double, bool>> values) = 0;
//        virtual vector<std::variant<int, double, bool>> getValues() = 0;
//        strings getValuesStrings();

//protected:
//        //virtual vector<string> getPropertyNames() { return {}; }
//        //virtual vector<string> getPropertyTypes() { return {}; }
//        vector<string> property_names;
//        vector<ParamType> property_types;

//    private:
//};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class UniformKS: public BaseKnotSequence
{
    public:
        IMPLEMENT_CLONE(UniformKS)
//        MAKE_PROTOTYPE(UniformKS)

        UniformKS() : BaseKnotSequence(1, 0.0, 1.0) { initProps(); } // NOT FIXED
        UniformKS(double start, double end) : BaseKnotSequence(1, start, end) { initProps(); } // NOT FIXED
        UniformKS(double start, double end, int steps) : BaseKnotSequence(start, end), steps(steps) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:
        int steps;

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class ValueKS: public BaseKnotSequence
{
    public:
        IMPLEMENT_CLONE(ValueKS)
//        MAKE_PROTOTYPE(ValueKS)

        ValueKS() : BaseKnotSequence(0, 0.0, 1.0) { initProps(); } // NOT FIXED
        ValueKS(double value) : BaseKnotSequence(value, value) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class MultiplicityValueKS: public UniformKS
{
    public:
        IMPLEMENT_CLONE(MultiplicityValueKS)
//        MAKE_PROTOTYPE(MultiplicityValueKS)

        MultiplicityValueKS() : UniformKS() { initProps(); } // NOT FIXED
        MultiplicityValueKS(double value) : UniformKS(value, value) { initProps(); } // NOT FIXED
        MultiplicityValueKS(double value, int mult) : UniformKS(value, value, mult) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BeginKS: public MultiplicityValueKS
{
    public:
        IMPLEMENT_CLONE(BeginKS)
//        MAKE_PROTOTYPE(BeginKS)

        BeginKS() : MultiplicityValueKS() { initProps(); } // NOT FIXED
        BeginKS(int bspline_n) : MultiplicityValueKS(0, bspline_n + 1) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class EndKS: public MultiplicityValueKS
{
    public:
        IMPLEMENT_CLONE(EndKS)
//        MAKE_PROTOTYPE(EndKS)

        EndKS() : MultiplicityValueKS() { initProps(); } // NOT FIXED
        EndKS(int bspline_n) : MultiplicityValueKS(1, bspline_n + 1) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class RationalKS: public BaseKnotSequence
{
    public:
        IMPLEMENT_CLONE(RationalKS)
//        MAKE_PROTOTYPE(RationalKS)

        RationalKS()
            : BaseKnotSequence(1, 0.0, 1.0) { initProps(); } // NOT FIXED
        RationalKS(double start, double end)
            : BaseKnotSequence(2, start, end) { initProps(); } // NOT FIXED
        RationalKS(double start, double end, int numpoints, double q)
            : BaseKnotSequence(start, end), q(q), numpoints(numpoints) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:
        double q;
        int numpoints;

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BiRationalKS: public BaseKnotSequence
{
    public:
        IMPLEMENT_CLONE(BiRationalKS)
//        MAKE_PROTOTYPE(BiRationalKS)

        BiRationalKS()
            : BaseKnotSequence(0, 0.0, 1.0) { initProps(); } // NOT FIXED
        BiRationalKS(double start, double end, int numpoints)
            : BaseKnotSequence(1, start, end), numpoints(numpoints) { initProps(); } // NOT FIXED
        BiRationalKS(double start, double end, int numpoints, double q1, double q2, double center)
            : BaseKnotSequence(start, end), q1(q1), q2(q2), center(center), numpoints(numpoints) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

        bool isCenterIncluded() const { return centerIncluded; }
        void setCenterIncluded(bool value) { centerIncluded = value; }

    protected:
        double q1, q2;
        double center;
        int numpoints;
        bool centerIncluded = true;

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// it takes params, compute the cumulate sequence and normalize between start and end;
class CumulKS: public BaseKnotSequence
{
    public:
        IMPLEMENT_CLONE(CumulKS)
//        MAKE_PROTOTYPE(CumulKS)

        CumulKS()
            : BaseKnotSequence(1, 0.0, 1.0) { initProps(); } // NOT FIXED
        CumulKS(double start, double end)
            : BaseKnotSequence(1, start, end) { initProps(); } // NOT FIXED
        CumulKS(double start, double end, doubles params)
            : BaseKnotSequence(start, end), params(params) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:
        doubles params;

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CustomKS: public BaseKnotSequence
{
    public:
        IMPLEMENT_CLONE(CustomKS)
//        MAKE_PROTOTYPE(CustomKS)

        CustomKS()
            : BaseKnotSequence(1, 0.0, 1.0) { initProps(); } // NOT FIXED
        CustomKS(double start, double end)
            : BaseKnotSequence(1, start, end) { initProps(); } // NOT FIXED
        CustomKS(double start, double end, doubles params)
            : BaseKnotSequence(start, end), params(params) { initProps(); } // FIXED

        void initProps() override;

        doubles getSequence(doubles params);
        doubles getSequence();

        void setValues(vector<std::variant<int, double, bool>> values);
        vector<std::variant<int, double, bool>> getValues();

    protected:
        doubles params;

    private:
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//class UniformFixedKS: public BaseFixedKnotSequence, public UniformKS
//{
//    public:
//        UniformFixedKS(double start, double end, int steps);
//        doubles getSequence();
//        doubles getSequence(doubles params)
//        {
//            return BaseFixedKnotSequence::getSequence(params);
//        }
//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();

//    protected:
//        int steps;

//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class ValueFixedKS: public BaseFixedKnotSequence
//{
//    public:
//        ValueFixedKS(double value);
//        doubles getSequence();
//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();

//    protected:

//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class MultiplicityFixedValueKS: public UniformFixedKS
//{
//    public:
//        MultiplicityFixedValueKS(double value, int multiplicity);
//        doubles getSequence();
//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();

//    protected:

//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


//class MultiplicityValueKS: public UniformKS
//{
//    public:
//        MultiplicityValueKS(double value);
//        doubles getSequence(doubles params);

//    protected:

//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class BeginKS: public MultiplicityFixedValueKS // initial zeros
//{
//    public:
//        BeginKS(int bspline_n);
//        doubles getSequence();
//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();

//    protected:

//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class EndKS: public MultiplicityFixedValueKS // final zeros
//{
//    public:
//        EndKS(int bspline_n);
//        doubles getSequence();
//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();

//    protected:

//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class RationalKS: public BaseKnotSequence
//{
//    public:
//        RationalKS(double start, double end, int numpoints);
//        doubles getSequence(doubles params);

//    protected:
//        int numpoints;
//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class RationalFixedKS: public BaseFixedKnotSequence, public RationalKS
//{
//    public:
//        RationalFixedKS(double start, double end, int numpoints, double q);

//        doubles getSequence() {
//            doubles params = {(double)this->q};
//            return RationalKS::getSequence(params);
//        }
//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();

//    protected:
//        double q;
//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class BiRationalKS: public RationalKS
//{
//    public:
//        BiRationalKS(double start, double end, int numpoints);
//        doubles getSequence(doubles params);
//        bool isCenterIncluded() const { return centerIncluded; }
//        void setCenterIncluded(bool value) { centerIncluded = value; }

//    protected:
//        int numpoints;
//        bool centerIncluded = true;
//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class BiRationalFixedKS: public BaseFixedKnotSequence, public BiRationalKS
//{
//    public:
//        BiRationalFixedKS(double start, double end, int numpoints, double q1, double q2, double center);
//        // by defaults if not specified, the sequence is symmetric respect to the center, so q, 1/q
//        BiRationalFixedKS(double start, double end, int numpoints, double q, double center)
//        :BiRationalFixedKS(start, end, numpoints, q, 1 / q, center){};

//        doubles getSequence(){
//            doubles params = {this->q1, this->q2, this->center};
//            return BiRationalKS::getSequence(params);
//        }

//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();
//    protected:
//        double q1, q2;
//        double center;
//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//class CustomFixedKS: public BaseFixedKnotSequence
//{
//    public:
//        CustomFixedKS(double start, double end, doubles sequence);
//        doubles getSequence();
//        void setValues(vector<std::variant<int, double, bool>> values);
//        vector<std::variant<int, double, bool>> getValues();

//    protected:
//        doubles sequence;
//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//// it takes params, compute the cumulate sequence and normalize between start and end;
//class CumulKS: public BaseKnotSequence
//{
//    public:
//        CumulKS(double start, double end, int numParams);
//        doubles getSequence(doubles params);

//    protected:

//    private:
//};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//// it takes params, compute the cumulate sequence and normalize between start and end;
//class CumulFixedKS: public BaseFixedKnotSequence, public CumulKS
//{
//    public:
//        CumulFixedKS(double start, double end, doubles params);

//        doubles getSequence(){
//            return this->CumulKS::getSequence(this->params);
//        }

//    protected:
//        doubles params;

//    private:
//};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif // BASEKNOTSEQUENCE_H
