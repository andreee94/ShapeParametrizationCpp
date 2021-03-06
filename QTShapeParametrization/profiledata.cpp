#include "profiledata.h"

#include "Table.h"
#include "Point.h"
#include "string"
#include <vector>
#include <algorithm>
#include <Settings.h>

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

ProfileData::ProfileData()
{
}

ProfileData::ProfileData(string filename)
{
    this->setFile(filename);
}

ProfileData ProfileData::setFile(string filename)
{
    this->filename = filename;
    this->data = Table(filename);
    return *this;
}

ProfileData ProfileData::setData(Table& data)
{
    this->filename = "";
    this->data = data;
    return *this;
}

ProfileData ProfileData::setColumnX(int columnX)
{
    this->columnX = columnX;
    return *this;
}

ProfileData ProfileData::setColumnY(int columnY)
{
    this->columnY = columnY;
    return *this;
}

ProfileData ProfileData::setColumnR(int columnR)
{
    this->columnR = columnR;
    return *this;
}

ProfileData ProfileData::setReverseX(bool reverseX)
{
    this->reverseX = reverseX;
    return *this;
}

ProfileData ProfileData::setReverseY(bool reverseY)
{
    this->reverseY = reverseY;
    return *this;
}

ProfileData ProfileData::setReverseZ(bool reverseZ)
{
    this->reverseZ = reverseZ;
    return *this;
}

ProfileData ProfileData::setFrameOfReference(FrameOfReference frameOfReference)
{
    this->frameOfReference = frameOfReference;
    return *this;
}

doubles ProfileData::getX(bool forcefull) const
{
    // TODO perform change in coordinates if not cartesian
    doubles x = data.getcolumn(this->columnX);
    if (!forcefull && curvePointsIndex.size() > 0)
        x = Utils::extract(x, curvePointsIndex);
    if (reverseX)
        std::for_each(x.begin(), x.end(), [](double &n){ n = -n; });
    return x;
}

doubles ProfileData::getY(bool forcefull) const
{
    // TODO perform change in coordinates if not cartesian
    doubles y = data.getcolumn(this->columnY);
    if (!forcefull && curvePointsIndex.size() > 0)
        y = Utils::extract(y, curvePointsIndex);
    if (reverseY)
        std::for_each(y.begin(), y.end(), [](double &n){ n = -n; });
    return y;
}

doubles ProfileData::getZ(bool forcefull) const
{
    doubles z = data.getcolumn(this->columnR);
    if (!forcefull && curvePointsIndex.size() > 0)
        z = Utils::extract(z, curvePointsIndex);
    if (reverseZ)
        std::for_each(z.begin(), z.end(), [](double &n){ n = -n; });
    return z;
}

double ProfileData::getR(bool forcefull) const
{
    doubles z = data.getcolumn(this->columnR);
    if (!forcefull && curvePointsIndex.size() > 0)
        z = Utils::extract(z, curvePointsIndex);
    return *max_element(std::begin(z), std::end(z));
}

int ProfileData::getColumnX() const
{
    return this->columnX;
}

int ProfileData::getColumnY() const
{
    return this->columnY;
}

int ProfileData::getColumnR() const
{
    return this->columnR;
}

bool ProfileData::getReverseX() const
{
    return this->reverseX;
}

bool ProfileData::getReverseY() const
{
    return this->reverseY;
}

bool ProfileData::getReverseZ() const
{
    return this->reverseZ;
}

int ProfileData::getDataColumnsNum() const
{
    return this->data.NumColumns();
}

int ProfileData::getDataRowsNum() const
{
    return this->data.NumRows();
}

FrameOfReference ProfileData::getFrameOfReference() const
{
    return this->frameOfReference;
}

string ProfileData::getFileName() const
{
    return this->filename;
}

Points ProfileData::getPoints(bool forcefull) const
{
    doubles x = getX(forcefull);
    doubles y = getY(forcefull);
    if (this->getFrameOfReference() == FrameOfReference::CYLINDRICAL)
    {
        doubles z = getZ();
        double R = getR();
        doubles rtheta;
        rtheta.reserve(x.size());
        for( unsigned int i = 0; i < x.size(); i++)
        {
            rtheta.push_back(R * atan(x[i] / z[i]));
        }
        return Point::fromDoubles(rtheta, y);
    }
    else return Point::fromDoubles(x, y);
}

//Point ProfileData::getFirstPoint() const
//{
//    return firstPoint.value();
//}

//void ProfileData::setFirstPoint(const Point &value)
//{
//    firstPoint = value;
//}

//Point ProfileData::getLastPoint() const
//{
//    return lastPoint.value();
//}

//void ProfileData::setLastPoint(const Point &value)
//{
//    lastPoint = value;
//}

int ProfileData::getFirstPointIndex() const
{
    return firstPointIndex;
}

void ProfileData::setFirstPointIndex(int value)
{
    firstPointIndex = value;
}

int ProfileData::getLastPointIndex() const
{
    return lastPointIndex;
}

//Points ProfileData::getCurvePoints() const
//{
//    return curvePoints;
//}

//Points ProfileData::getTEPoints() const
//{
//    return TEPoints;
//}

void ProfileData:: computePointsAndTE(Points &curvePoints, Points &TEPoints)
{
    Points points = getPoints(true);
    Utils::getpointswithoutTE(points, firstPointIndex, lastPointIndex, curvePointsIndex, TEPointsIndex);
    curvePoints = Utils::extract(points, curvePointsIndex);
    TEPoints = Utils::extract(points, TEPointsIndex);
}

void ProfileData::setLastPointIndex(int value)
{
    lastPointIndex = value;
}

void ProfileData::saveToSettings(Settings* settings, const string key) const
{
    settings->setvalue(key + "-filename", filename);
    settings->setvalue(key + "-columnX", columnX);
    settings->setvalue(key + "-columnY", columnY);
    settings->setvalue(key + "-columnR", columnR);
    settings->setvalue(key + "-R", R);
    settings->setvalue(key + "-reverseX", reverseX);
    settings->setvalue(key + "-reverseY", reverseY);
    settings->setvalue(key + "-reverseZ", reverseZ);
    settings->setvalue(key + "-frameOfReference", frameOfReference);
    settings->setvalue(key + "-firstPointIndex", firstPointIndex);
    settings->setvalue(key + "-lastPointIndex", lastPointIndex);
    settings->setvalues(key + "-curvePointsIndex", curvePointsIndex);
    settings->setvalues(key + "-TEPointsIndex", TEPointsIndex);
}

void ProfileData::loadFromSettings(Settings *settings, const string key)
{
    filename = settings->getstring(key + "-filename");
    columnX = settings->getint(key + "-columnX");
    columnY = settings->getint(key + "-columnY");
    columnR = settings->getint(key + "-columnR");
    R = settings->getdouble(key + "-R");
    reverseX = settings->getbool(key + "-reverseX");
    reverseY = settings->getbool(key + "-reverseY");
    reverseZ = settings->getbool(key + "-reverseZ");
    frameOfReference = (FrameOfReference)settings->getint(key + "-frameOfReference");
    firstPointIndex = settings->getint(key + "-firstPointIndex");
    lastPointIndex = settings->getint(key + "-lastPointIndex");
    curvePointsIndex = settings->getints(key + "-curvePointsIndex");
    TEPointsIndex = settings->getints(key + "-TEPointsIndex");
}
