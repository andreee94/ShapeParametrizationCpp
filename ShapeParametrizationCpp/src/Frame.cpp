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


#include "Frame.h"

Frame::Frame()
{

}

Frame::~Frame()
{

}

void Frame::addColumn(Frame::ColumnType type, const variant<vector<double>, vector<string> > &column)
{
    column_types.push_back(type);
    data.push_back(column);
}

void Frame::addColumn(const vector<double> &column)
{
    column_types.push_back(ColumnType::DOUBLE);
    data.push_back(column);
}

void Frame::addColumn(const vector<int> &column)
{
    column_types.push_back(ColumnType::INT);
    data.push_back(vector<double>(column.begin(), column.end()));
}

void Frame::addColumn(const vector<bool> &column)
{
    column_types.push_back(ColumnType::BOOL);
    data.push_back(vector<double>(column.begin(), column.end()));
}

void Frame::addColumn(const vector<string> &column)
{
    column_types.push_back(ColumnType::DOUBLE);
    data.push_back(column);
}

void Frame::addRow(const vector<variant<double, string> > &row)
{
    for (auto const &[index, item] : Utils::enumerate(row))
    {
//        if (column_types.at(index) == ColumnType::INT)
//            std::get<vector<double>>(data.at(index)).push_back(std::get<int>(item));
//        else if (column_types.at(index) == ColumnType::DOUBLE)
//            std::get<vector<double>>(data.at(index)).push_back(std::get<double>(item));
//        else if (column_types.at(index) == ColumnType::BOOL)
//            std::get<vector<double>>(data.at(index)).push_back(std::get<bool>(item));
//        else if (column_types.at(index) == ColumnType::STRING)
//            std::get<vector<string>>(data.at(index)).push_back(std::get<string>(item));
    }
}

vector<string> Frame::getRow(size_t row)
{
    vector<string> res;
    res.reserve(data.size()); //number of columns
//    for (size_t index = 0; index < column_types.size(); index++)
//    {
//        if (column_types.at(index) == ColumnType::INT)
//            res.push_back(std::to_string(std::get<vector<double>>(data.at(index)).at(row)));
//        else if (column_types.at(index) == ColumnType::DOUBLE)
//            res.push_back(std::to_string((int)std::get<vector<double>>(data.at(index)).at(row)));
//        else if (column_types.at(index) == ColumnType::BOOL)
//            res.push_back(std::to_string((bool)std::get<vector<double>>(data.at(index)).at(row)));
//        else if (column_types.at(index) == ColumnType::STRING)
//            res.push_back(std::get<vector<string>>(data.at(index)).at(row));
//    }
    return res;
}

void Frame::save(const string filename)
{
    ofstream file;
    file.open(filename, ios::out);
//    int col_count = this->data.size();
//    int row_count = this->data.at(0).size();
//    for( auto const& column : this->data)
//    {
//        cout << key << "=" << val << endl;
//        file << key << "=" << val << endl;
//    }
    file.close();
}











