#include "Table.h"
#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <map>
#include <string>


using namespace std;

typedef std::vector<int> ints;
typedef std::vector<double> doubles;
typedef std::vector<float> floats;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;

Table::~Table()
{
    //dtor
}

Table::Table(string filename)
{
    ifstream file;
    file.open(filename, ios::in);

    int num_rows = 0;
    string line;
    double buf;
    while (file)
    {
        getline(file, line);
        line = Utils::fixline(line);

        if (line.empty())
            continue;

        doubles row;
        stringstream ss(line, ios_base::out|ios_base::in|ios_base::binary);

        while (ss >> buf)
            row.push_back(buf);

        this->data.insert(this->data.end(), row.begin(), row.end());
        // this->data.push_back(row);
        num_rows++;
    }
    file.close();
    this->numRows = num_rows;
    this->numCols = this->data.size() / num_rows;

    cout << "numrows = " << numRows << endl;
    cout << "numcols = " << numCols << endl;
    for (int i = 0; i < this->data.size(); i++)
    {
        cout << data[i] << "   ";
        if (i % numCols == numCols - 1)
            cout << endl;
    }
}

doubles Table::StridedSlice( int start, int length, int stride ) const
{
    doubles result;
    result.reserve( length );
    const double *pos = &this->data[start];
    for( int i = 0; i < length; i++ ) {
        result.push_back(*pos);
        pos += stride;
    }
    return result;
}

Table::Table( int rows, int cols, const double& initVal )
    : data( rows * cols, initVal )
    , numRows( rows )
    , numCols( cols )
{
}

doubles Table::getrow( int row, int colBegin, int colEnd ) const
{
    if( colEnd < 0 ) colEnd = this->numCols-1;
    if( colBegin < 0 ) colBegin = this->numCols-1;
    if( colBegin <= colEnd )
        return StridedSlice( Index(row,colBegin), colEnd-colBegin+1, 1 );
    else
        return StridedSlice( Index(row,colBegin), colBegin-colEnd+1, -1 );
}

doubles Table::getcolumn( int col, int rowBegin, int rowEnd ) const
{
    if( rowEnd < 0 ) rowEnd = this->numRows-1;
    if( rowBegin < 0 ) rowBegin = this->numRows-1;
    if( rowBegin <= rowEnd )
        return StridedSlice( Index(rowBegin,col), rowEnd-rowBegin+1, numCols );
    else
        cout << "rowBegin = " << rowBegin << endl;
        cout << "rowEnd = " << rowEnd << endl;
        return StridedSlice( Index(rowBegin,col), rowBegin-rowEnd+1, -numCols );
}

Table Table::load(string filename)
{
    return Table(filename);
}


