#include "BaseKnotSequence.h"
#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <typeinfo>
#include <variant>

using namespace std;

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

BaseKnotSequence::BaseKnotSequence()
:BaseKnotSequence(0, 0.0, 1.0)
{
}

BaseKnotSequence::BaseKnotSequence(int  numParams, double start, double end)
{
    this->numParams = numParams;
    this->start = start;
    this->end = end;
}

BaseFixedKnotSequence::BaseFixedKnotSequence(double start, double end)
:BaseKnotSequence(0, start, end)
{
}

void BaseFixedKnotSequence::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 2) // TODO throw error
        return;
    static_cast<BaseKnotSequence *>(this)->setStart(std::get<double>(values[0]));
    static_cast<BaseKnotSequence *>(this)->setEnd(std::get<double>(values[1]));
}

vector<std::variant<int, double>> BaseFixedKnotSequence::getValues()
{
    vector<std::variant<int, double>> values;
    values.push_back(start);
    values.push_back(end);
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

UniformKS::UniformKS(double start, double end)
:BaseKnotSequence(1, start, end)
{
}

doubles UniformKS::getSequence(doubles params)
{
    return Utils::linspace(this->start, this->end, (int)params[0]);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

UniformFixedKS::UniformFixedKS(double start, double end, int steps)
:BaseFixedKnotSequence(start, end)
,UniformKS(start, end)
{
    this->steps = steps;
    this->property_names = {"Start", "End", "Steps"};
    this->property_types = {ParamType::DOUBLE, ParamType::DOUBLE, ParamType::INT};
}

doubles UniformFixedKS::getSequence()
{
    doubles params = {(double)this->steps};
    return UniformKS::getSequence(params);
}

void UniformFixedKS::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 3) // TODO throw error
        return;
    static_cast<BaseFixedKnotSequence *>(this)->setValues(Utils::slice(values, 0, 2));
    this->steps = std::get<int>(values[2]);
}

vector<std::variant<int, double>> UniformFixedKS::getValues()
{
    vector<std::variant<int, double>> values = BaseFixedKnotSequence::getValues();
    values.push_back(steps);
    return values;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ValueFixedKS::ValueFixedKS(double value)
:BaseFixedKnotSequence(value, value)
{
    this->property_names = {"Value"};
    this->property_types = {ParamType::DOUBLE};
}

doubles ValueFixedKS::getSequence()
{
    doubles d = {this->getStart()};
    return d;
}

void ValueFixedKS::setValues(vector<std::variant<int, double> > values)
{
    if (values.size() != 1) // TODO throw error
        return;
    this->start = std::get<double>(values[0]);
    this->end = std::get<double>(values[0]);
}

vector<std::variant<int, double>> ValueFixedKS::getValues()
{
    vector<std::variant<int, double>> values;
    values.push_back(start);
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

MultiplicityFixedValueKS::MultiplicityFixedValueKS(double value, int multiplicity)
:UniformFixedKS(value, value, multiplicity)
{
    this->property_names = {"Value", "Multiplicity"};
    this->property_types = {ParamType::DOUBLE, ParamType::INT};
}

doubles MultiplicityFixedValueKS::getSequence()
{
    return UniformFixedKS::getSequence();
}

void MultiplicityFixedValueKS::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 2) // TODO throw error
        return;
    static_cast<BaseFixedKnotSequence *>(this)->setValues({values[0], values[0]});
    this->steps = std::get<int>(values[1]);
}

vector<std::variant<int, double>> MultiplicityFixedValueKS::getValues()
{
    vector<std::variant<int, double>> values;
    values.push_back(static_cast<BaseFixedKnotSequence *>(this)->getStart());
    values.push_back(steps);
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

MultiplicityValueKS::MultiplicityValueKS(double value)
:UniformKS(value, value)
{
}

doubles MultiplicityValueKS::getSequence(doubles params)
{
    return UniformKS::getSequence(params);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BeginKS::BeginKS(int bspline_n)
:MultiplicityFixedValueKS(0, bspline_n + 1)
{
    this->property_names = {"Bspline Degree"};
    this->property_types = {ParamType::INT};
}

doubles BeginKS::getSequence()
{
    return MultiplicityFixedValueKS::getSequence();
}

void BeginKS::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 1) // TODO throw error
        return;
    static_cast<BaseFixedKnotSequence *>(this)->setValues({0, 0});
    this->steps = std::get<int>(values[0]) + 1;
}

vector<std::variant<int, double>> BeginKS::getValues()
{
    vector<std::variant<int, double>> values;
    values.push_back(steps - 1);
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

EndKS::EndKS(int bspline_n)
:MultiplicityFixedValueKS(1, bspline_n + 1)
{
    this->property_names = {"Bspline Degree"};
    this->property_types = {ParamType::INT};
}

doubles EndKS::getSequence()
{
    return MultiplicityFixedValueKS::getSequence();
}

void EndKS::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 1) // TODO throw error
        return;
    static_cast<BaseFixedKnotSequence *>(this)->setValues({1, 1});
    this->steps = std::get<int>(values[0]) + 1;
}

vector<std::variant<int, double>> EndKS::getValues()
{
    vector<std::variant<int, double>> values;
    values.push_back(steps - 1);
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

RationalKS::RationalKS(double start, double end, int numpoints)
:BaseKnotSequence(1, start, end)
{
    this->numpoints = numpoints;
}

doubles RationalKS::getSequence(doubles params)
{
    // return contains both start and end points, so numpoints+1 items
    double q = params[0];
    // a*(1 + 1/q + 1/q^2+...) = end - start = b
    double b = this->end - this->start;
    double a = b / this->numpoints;
    if (q != 1.0)
        a = b / (1.0 - pow( 1.0 / q, this->numpoints)) * (1.0 - 1.0 / q);

    doubles uarray(this->numpoints + 1);

    // add one more index which is the start point
    uarray[0] = this->start;
    for (int i = 1; i < this->numpoints + 1; i++)
    {
        uarray[i] = uarray[i - 1] + a / pow(q, i - 1);
    }
    //uarray.insert(uarray.begin(), this->start);
    return uarray;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

RationalFixedKS::RationalFixedKS(double start, double end, int numpoints, double q)
:BaseFixedKnotSequence(start, end)
,RationalKS(start, end, numpoints)
{
    this->property_names = {"Start", "End", "Points number", "Ratio"};
    this->property_types = {ParamType::DOUBLE, ParamType::DOUBLE, ParamType::INT, ParamType::DOUBLE};
    this->q = q;
}

void RationalFixedKS::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 4) // TODO throw error
        return;
    static_cast<BaseFixedKnotSequence *>(this)->setValues(Utils::slice(values, 0, 2));
    this->numpoints = std::get<int>(values[2]);
    this->q = std::get<double>(values[3]);
}

vector<std::variant<int, double>> RationalFixedKS::getValues()
{
    vector<std::variant<int, double>> values = BaseFixedKnotSequence::getValues();
    values.push_back(numpoints);
    values.push_back(q);
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


BiRationalKS::BiRationalKS(double start, double end, int numpoints)
:RationalKS(start, end, numpoints)
{
    this->numParams = 3; // override value of rationalKS
    this->numpoints = numpoints;
}

doubles BiRationalKS::getSequence(doubles params)
{
    double q1 = params[0];
    double q2 = params[1];
    double center = params[2];
    // backup start and end values
    double bkp_start = this->BaseKnotSequence::getStart();
    double bkp_end = this->getEnd();

    // call rational getsequence for the left range:
    this->start = bkp_start;
    this->end = center;
    doubles uarray_left = RationalKS::getSequence({q1});

    // call rational getsequence for the right range:
    this->start = center;
    this->end = bkp_end;
    doubles uarray_right = RationalKS::getSequence({q2});
    uarray_left.pop_back(); // the center element is inserted 2 times so we remove one occurrence

    //restore start and end values
    this->start = bkp_start;
    this->end = bkp_end;

    // concatenate left and right
    doubles uarray(uarray_left);
    uarray.insert(uarray.end(), uarray_right.begin(), uarray_right.end());
    return uarray;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BiRationalFixedKS::BiRationalFixedKS(double start, double end, int numpoints, double q1, double q2, double center)
:BaseFixedKnotSequence(start, end)
,BiRationalKS(start, end, numpoints)
{
    this->property_names = {"Start", "End", "Points number", "Ratio 1", "Ratio 2", "Center"};
    this->property_types = {ParamType::DOUBLE, ParamType::DOUBLE, ParamType::INT, ParamType::DOUBLE, ParamType::DOUBLE, ParamType::DOUBLE};
    this->q1 = q1;
    this->q2 = q2;
    this->center = center;
}

void BiRationalFixedKS::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 6) // TODO throw error
        return;
    static_cast<BaseFixedKnotSequence *>(this)->setValues(Utils::slice(values, 0, 2));
    this->numpoints = std::get<int>(values[2]);
    this->q1 = std::get<double>(values[3]);
    this->q2 = std::get<double>(values[4]);
    this->center = std::get<double>(values[5]);
}

vector<std::variant<int, double>> BiRationalFixedKS::getValues()
{
    vector<std::variant<int, double>> values = BaseFixedKnotSequence::getValues();
    values.push_back(numpoints);
    values.push_back(q1);
    values.push_back(q2);
    values.push_back(center);
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CustomFixedKS::CustomFixedKS(double start, double end, doubles sequence) // TODO ADD SEQUENCE IN PROPERTIES
:BaseFixedKnotSequence(start, end)
{
    this->property_names = {"Start", "End"};
    this->property_types = {ParamType::DOUBLE, ParamType::DOUBLE};
    this->sequence = sequence;
}

doubles CustomFixedKS::getSequence()
{
    return this->sequence;
}

void CustomFixedKS::setValues(vector<std::variant<int, double>> values)
{
    if (values.size() != 6) // TODO throw error
        return;
    static_cast<BaseFixedKnotSequence *>(this)->setValues(Utils::slice(values, 0, 2));
}

vector<std::variant<int, double>> CustomFixedKS::getValues()
{
    vector<std::variant<int, double>> values;
    values = static_cast<BaseFixedKnotSequence *>(this)->getValues();
    return values;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CumulKS::CumulKS(double start, double end, int numParams)
:BaseKnotSequence(numParams, start, end)
{
}

doubles CumulKS::getSequence(doubles params)
{
    return Utils::computeCumul(this->start, this->end, params);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CumulFixedKS::CumulFixedKS(double start, double end, doubles params)
:BaseFixedKnotSequence(start, end)
,CumulKS(start, end, params.size())
{
    this->params = params;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
