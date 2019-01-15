#ifndef TABLE_H
#define TABLE_H
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


class Table
{
    public:
        /** Default constructor */
        Table() {};
        Table(string filename);
        Table( int rows, int cols, const double& initVal = 0.0 );
        /** Default destructor */
        virtual ~Table();

        // shorter version
        doubles r(int row) const {  return getrow(row); }
        doubles c(int column) const { return getcolumn(column); }

        // Simple row or column slices
        doubles getrow( int row, int colBegin = 0, int colEnd = -1 ) const;
        doubles getcolumn( int col, int rowBegin = 0, int rowEnd = -1) const ;


        static Table load(string filename);

        // Size and structure
        int NumRows() const                       { return this->numRows; }
        int NumColumns() const                    { return this->numCols; }
        int NumElements() const                   { return this->data.size(); }

        // Direct vector access and indexing
        operator const doubles& () const        { return this->data; }
        int Index( int row, int col ) const
        {
//        cout << "row=" << row << endl;
//        cout << "col=" << col << endl;
//        cout << "index=" << this->numCols * row + col << endl;
        return this->numCols * row + col;
        }

        // Get a single value
              double & Value( int row, int col )       { return this->data[Index(row,col)]; }
        const double & Value( int row, int col ) const { return this->data[Index(row,col)]; }
              double & operator[]( size_t idx )        { return this->data[idx]; }
        const double & operator[]( size_t idx ) const  { return this->data[idx]; }


    protected:
        int numRows, numCols;
        // matrix of double
        doubles data;


       doubles StridedSlice( int start, int length, int stride ) const;
    private:
};

#endif // TABLE_H
