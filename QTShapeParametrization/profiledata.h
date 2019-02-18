#ifndef PROFILEDATA_H
#define PROFILEDATA_H

#include "Table.h"
#include "Point.h"
#include "string"
#include <QPointF>
#include <vector>


typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

using namespace std;

enum FrameOfReference { CARTESIAN, CYLINDRICAL};

class ProfileData
{
public:
    ProfileData();
    ProfileData(string filename);

    ProfileData setFile(string filename);
    ProfileData setData(Table& data);
    ProfileData setColumnX(int columnX);
    ProfileData setColumnY(int columnY);
    ProfileData setColumnR(int columnR);
    ProfileData setReverseX(bool reverseX);
    ProfileData setReverseY(bool reverseY);
    ProfileData setReverseZ(bool reverseY);
    ProfileData setFrameOfReference(FrameOfReference frameOfReference);
    //void setFirstPoint(const Point &value);
    //void setLastPoint(const Point &value);
    void setFirstPointIndex(int value);
    void setLastPointIndex(int value);

    double Value( int row, int col ) const { return this->data.Value(row, col); }

    doubles getX(bool forcefull=false) const;
    doubles getY(bool forcefull=false) const;
    doubles getZ(bool forcefull=false) const;
    double getR(bool forcefull=false) const;
    int getColumnX() const;
    int getColumnY() const;
    int getColumnR() const;
    bool getReverseX() const;
    bool getReverseY() const;
    bool getReverseZ() const;
    int getDataColumnsNum() const;
    int getDataRowsNum() const;
    string getFileName() const;
    FrameOfReference getFrameOfReference() const;
    Points getPoints(bool forcefull=false) const;
    //std::optional<Point> getFirstPoint() const;
    //std::optional<Point> getLastPoint() const;
    int getFirstPointIndex() const;
    int getLastPointIndex() const;
    Points getCurvePoints() const;
    Points getTEPoints() const;

    std::optional<QPointF> firstPoint, lastPoint;

    // other functions
    void computePointsAndTE(Points &curvePoints, Points &TEPoints);

private:
    string filename;
    Table data;
    int columnX = 1;
    int columnY = 2;
    int columnR = 3;
    double R;
    bool reverseX = false;
    bool reverseY = false;
    bool reverseZ = false;
    FrameOfReference frameOfReference;
    int firstPointIndex, lastPointIndex;
    ints curvePointsIndex, TEPointsIndex;

};

#endif // PROFILEDATA_H
