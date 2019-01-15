#ifndef PROFILEDATA_H
#define PROFILEDATA_H

#include "Table.h"
#include "Point.h"
#include "string"
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

    double Value( int row, int col ) const { return this->data.Value(row, col); }

    doubles getX() const;
    doubles getY() const;
    doubles getZ() const;
    double getR() const;
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
    Points getPoints() const;

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

};

#endif // PROFILEDATA_H
