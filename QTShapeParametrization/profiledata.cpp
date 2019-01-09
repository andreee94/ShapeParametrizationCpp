#include "profiledata.h"

#include "Table.h"
#include "Point.h"
#include "string"
#include <vector>

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

ProfileData ProfileData::setFrameOfReference(FrameOfReference frameOfReference)
{
    this->frameOfReference = frameOfReference;
    return *this;
}

doubles ProfileData::getX() const
{
    // TODO perform change in coordinates if not cartesian
    return data.getcolumn(this->columnX);
}

doubles ProfileData::getY() const
{
    // TODO perform change in coordinates if not cartesian
    return data.getcolumn(this->columnY);
}

int ProfileData::getColumnX() const
{
    // TODO perform change in coordinates if not cartesian
    return this->columnX;
}

int ProfileData::getColumnY() const
{
    // TODO perform change in coordinates if not cartesian
    return this->columnY;
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
    return Point::fromDoubles(x, y);
}


