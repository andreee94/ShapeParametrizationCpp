/********************************************************************************
 *                                                                              *
 *   Copyright (C) 2019 by Andreee94                                            *
 *                                                                              *
 *   SHAPE PARAMETRIZATION                                                      *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU Affero General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU Affero General Public License for more details.                          *
 *                                                                              *
 * You should have received a copy of the GNU Affero General Public License     *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                              *
 ********************************************************************************/

#include <vector>
#include <cmath>
#include <Point.h>
#include <variant>
#include <typeinfo>
#include <map>

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

#ifndef FRAME_H
#define FRAME_H


class Frame
{
public:
    enum ColumnType { INT = 1, DOUBLE = 2, BOOL = 3, STRING = 4 };
    Frame();
    virtual ~Frame();

    void addColumn(Frame::ColumnType type, const variant<vector<double>, vector<string>> &column);
    void addColumn(const vector<double> &column);
    void addColumn(const vector<int> &column);
    void addColumn(const vector<bool> &column);
    void addColumn(const vector<string> &column);

    void addRow(const vector<variant<double, string>> &row);

//    vector<double> getdoubles(int col);
//    vector<ints> getints(int col);
//    vector<bools> getbools(int col);
//    vector<strings> getstrings(int col);

    vector<string> getRow(size_t row);

    void save(const string filename);

private:
    // data saved as columns of string or double, then converted to bool and int by type
    vector<variant<vector<double>, vector<string>>> data;
    vector<ColumnType> column_types;

};

#endif // FRAME_H
