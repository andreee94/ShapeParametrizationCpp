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

BaseKnotSequence *BaseKnotSequence::fromClass(string classname)
{
//    if (classname == typeid(UniformFixedKS).name())
    //        return new UniformFixedKS(0, 1, 10);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BaseFixedKnotSequence::BaseFixedKnotSequence(double start, double end)
:BaseKnotSequence(0, start, end)
{
}

strings BaseFixedKnotSequence::getValuesStrings()
{
    vector<std::variant<int, double, bool>> values = getValues();
    strings ss(values.size());
    for (auto const [index, value] : Utils::enumerate(values))
    {
        if (property_types[index] == ParamType::INT)
            ss.push_back(std::to_string(std::get<int>(value)));
        else if (property_types[index] == ParamType::DOUBLE)
            ss.push_back(std::to_string(std::get<double>(value)));
        else if (property_types[index] == ParamType::BOOL)
            ss.push_back(std::to_string(std::get<bool>(value)));
    }
    return ss;
}


void BaseFixedKnotSequence::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 2 && values.size() != 4) // TODO throw error
        return;
    BaseKnotSequence::setStart(std::get<double>(values[0]));
    BaseKnotSequence::setEnd(std::get<double>(values[1]));
    if (values.size() == 4)
    {
        BaseKnotSequence::setStartIncluded(std::get<bool>(values[2]));
        BaseKnotSequence::setEndIncluded(std::get<bool>(values[3]));
    }
}

vector<std::variant<int, double, bool>> BaseFixedKnotSequence::getValues()
{
    vector<std::variant<int, double, bool>> values;
    values.push_back(start);
    values.push_back(end);
    //values.push_back(startIncluded);
    //values.push_back(endIncluded);
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
    return Utils::linspace(this->start, this->end, (int)params[0], startIncluded, endIncluded);
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
    this->UniformKS::start = start;
    this->UniformKS::end = end;
    this->steps = steps;
    this->property_names = {"Start", "End", "Include Start", "Include End", "Steps"};
    this->property_types = {ParamType::DOUBLE, ParamType::DOUBLE, ParamType::BOOL, ParamType::BOOL, ParamType::INT};
}

doubles UniformFixedKS::getSequence()
{
    doubles params = {(double)this->steps};
    return UniformKS::getSequence(params);
}

void UniformFixedKS::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 5) // TODO throw error
        return;
    BaseFixedKnotSequence::setValues(Utils::slice(values, 0, 4));
    UniformKS::start = BaseFixedKnotSequence::start;
    UniformKS::end = BaseFixedKnotSequence::end;
    UniformKS::startIncluded = BaseFixedKnotSequence::startIncluded;
    UniformKS::endIncluded = BaseFixedKnotSequence::endIncluded;
    this->steps = std::get<int>(values[4]);
}

vector<std::variant<int, double, bool>> UniformFixedKS::getValues()
{
    vector<std::variant<int, double, bool>> values = BaseFixedKnotSequence::getValues();
    values.push_back(BaseFixedKnotSequence::startIncluded);
    values.push_back(BaseFixedKnotSequence::endIncluded);
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

void ValueFixedKS::setValues(vector<std::variant<int, double, bool> > values)
{
    if (values.size() != 1) // TODO throw error
        return;
    this->start = std::get<double>(values[0]);
    this->end = std::get<double>(values[0]);
}

vector<std::variant<int, double, bool>> ValueFixedKS::getValues()
{
    vector<std::variant<int, double, bool>> values;
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

void MultiplicityFixedValueKS::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 2) // TODO throw error
        return;
    BaseFixedKnotSequence::setValues({values[0], values[0]});
    UniformKS::start = BaseFixedKnotSequence::start;
    UniformKS::end = BaseFixedKnotSequence::end;
    this->steps = std::get<int>(values[1]);
}

vector<std::variant<int, double, bool>> MultiplicityFixedValueKS::getValues()
{
    vector<std::variant<int, double, bool>> values;
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

void BeginKS::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 1) // TODO throw error
        return;
    BaseFixedKnotSequence::setValues({0.0, 0.0});
    this->steps = std::get<int>(values[0]) + 1;
}

vector<std::variant<int, double, bool>> BeginKS::getValues()
{
    vector<std::variant<int, double, bool>> values;
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

void EndKS::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 1) // TODO throw error
        return;
    //static_cast<BaseFixedKnotSequence *>(this)->setValues({1.0, 1.0});
    BaseFixedKnotSequence::setValues({1.0, 1.0});
    this->steps = std::get<int>(values[0]) + 1;
}

vector<std::variant<int, double, bool>> EndKS::getValues()
{
    vector<std::variant<int, double, bool>> values;
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
    uarray = Utils::extract(uarray, startIncluded ? 0 : 1, endIncluded ? 0 : 1);
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
    this->RationalKS::start = start;
    this->RationalKS::end = end;
    this->property_names = {"Start", "End", "Include Start", "Include End", "Points number", "Ratio"};
    this->property_types = {ParamType::DOUBLE, ParamType::DOUBLE, ParamType::BOOL, ParamType::BOOL, ParamType::INT, ParamType::DOUBLE};
    this->q = q;
}

void RationalFixedKS::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 6) // TODO throw error
        return;

    BaseFixedKnotSequence::setValues(Utils::slice(values, 0, 4));
    RationalKS::start = BaseFixedKnotSequence::start;
    RationalKS::end = BaseFixedKnotSequence::end;
    RationalKS::startIncluded = BaseFixedKnotSequence::startIncluded;
    RationalKS::endIncluded = BaseFixedKnotSequence::endIncluded;
    this->numpoints = std::get<int>(values[4]);
    this->q = std::get<double>(values[5]);
}

vector<std::variant<int, double, bool>> RationalFixedKS::getValues()
{
    vector<std::variant<int, double, bool>> values = BaseFixedKnotSequence::getValues();
    values.push_back(BaseFixedKnotSequence::startIncluded);
    values.push_back(BaseFixedKnotSequence::endIncluded);
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
    bool bkp_startincluded = startIncluded;
    bool bkp_endincluded = endIncluded;

    // call rational getsequence for the left range:
    this->start = bkp_start;
    this->end = center;
    RationalKS::startIncluded = true;
    RationalKS::endIncluded = centerIncluded;
    doubles uarray_left = RationalKS::getSequence({q1});

    // call rational getsequence for the right range:
    this->start = center;
    this->end = bkp_end;
    RationalKS::startIncluded = centerIncluded;
    RationalKS::endIncluded = true;
    doubles uarray_right = RationalKS::getSequence({q2});
    if (centerIncluded)
        uarray_left.pop_back(); // the center element is inserted 2 times so we remove one occurrence

    //restore start and end values
    this->start = bkp_start;
    this->end = bkp_end;
    this->startIncluded = bkp_startincluded;
    this->endIncluded = bkp_endincluded;

    // concatenate left and right
    doubles uarray(uarray_left);
    uarray.insert(uarray.end(), uarray_right.begin(), uarray_right.end());
    uarray = Utils::extract(uarray, startIncluded ? 0 : 1, endIncluded ? 0 : 1);
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
    this->BiRationalKS::start = start;
    this->BiRationalKS::end = end;
    this->property_names = {"Start", "End", "Include Start", "Include End", "Include Center", "Points number", "Ratio 1", "Ratio 2", "Center"};
    this->property_types = {ParamType::DOUBLE, ParamType::DOUBLE, ParamType::BOOL, ParamType::BOOL,ParamType::BOOL, ParamType::INT, ParamType::DOUBLE, ParamType::DOUBLE, ParamType::DOUBLE};
    this->q1 = q1;
    this->q2 = q2;
    this->center = center;
}

void BiRationalFixedKS::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 9) // TODO throw error
        return;

    BaseFixedKnotSequence::setValues(Utils::slice(values, 0, 4));
    BiRationalKS::start = BaseFixedKnotSequence::start;
    BiRationalKS::end = BaseFixedKnotSequence::end;
    BiRationalKS::startIncluded = BaseFixedKnotSequence::startIncluded;
    BiRationalKS::endIncluded = BaseFixedKnotSequence::endIncluded;
    this->centerIncluded = std::get<bool>(values[4]);
    BiRationalKS::centerIncluded = centerIncluded;
    BiRationalKS::numpoints = std::get<int>(values[5]);
    RationalKS::numpoints = std::get<int>(values[5]);
    this->numpoints = std::get<int>(values[5]);
    this->q1 = std::get<double>(values[6]);
    this->q2 = std::get<double>(values[7]);
    this->center = std::get<double>(values[8]);
}

vector<std::variant<int, double, bool>> BiRationalFixedKS::getValues()
{
    vector<std::variant<int, double, bool>> values = BaseFixedKnotSequence::getValues();
    values.push_back(BaseFixedKnotSequence::startIncluded);
    values.push_back(BaseFixedKnotSequence::endIncluded);
    values.push_back(centerIncluded);
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

void CustomFixedKS::setValues(vector<std::variant<int, double, bool>> values)
{
    if (values.size() != 2) // TODO throw error
        return;
    //static_cast<BaseFixedKnotSequence *>(this)->setValues(Utils::slice(values, 0, 2));
    BaseFixedKnotSequence::setValues(Utils::slice(values, 0, 2));
}

vector<std::variant<int, double, bool>> CustomFixedKS::getValues()
{
    vector<std::variant<int, double, bool>> values;
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
    this->CumulKS::start = start;
    this->CumulKS::end = end;
    this->params = params;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
