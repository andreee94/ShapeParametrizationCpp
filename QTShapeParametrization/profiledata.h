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
    ProfileData setFrameOfReference(FrameOfReference frameOfReference);

    double Value( int row, int col ) const { return this->data.Value(row, col); }

    doubles getX() const;
    doubles getY() const;
    int getColumnX() const;
    int getColumnY() const;
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
    FrameOfReference frameOfReference;

};

#endif // PROFILEDATA_H
