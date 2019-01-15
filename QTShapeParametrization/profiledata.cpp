#include "profiledata.h"

#include "Table.h"
#include "Point.h"
#include "string"
#include <vector>
#include <algorithm>

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

doubles ProfileData::getX() const
{
    // TODO perform change in coordinates if not cartesian
    doubles x = data.getcolumn(this->columnX);
    if (reverseX)
        std::for_each(x.begin(), x.end(), [](double &n){ n = -n; });
    return x;
}

doubles ProfileData::getY() const
{
    // TODO perform change in coordinates if not cartesian
    doubles y = data.getcolumn(this->columnY);
    if (reverseY)
        std::for_each(y.begin(), y.end(), [](double &n){ n = -n; });
    return y;
}

doubles ProfileData::getZ() const
{
    doubles z = data.getcolumn(this->columnR);
    if (reverseZ)
        std::for_each(z.begin(), z.end(), [](double &n){ n = -n; });
    return z;
}

double ProfileData::getR() const
{
    doubles z = data.getcolumn(this->columnR);
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

Points ProfileData::getPoints() const
{
    doubles x = getX();
    doubles y = getY();
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
