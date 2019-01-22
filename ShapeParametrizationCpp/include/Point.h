#ifndef POINT_H
#define POINT_H

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>
#include <sstream>
#include  <string>
#include  <fstream>
#include <iostream>

using namespace std;

class Line;
class Point;
class Utils;

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

class Point
{
    public:
        /** Default constructor */
        Point();
        Point(int value);
        Point(double value);
        Point(int x, int y);
        Point(double x, double y);
        Point(string str);
        /** Default destructor */
        virtual ~Point();
        /** Copy constructor
         *  \param other Object to copy from
         */
        Point(const Point& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Point& operator=(const Point& other);

        friend Point operator+(const Point& a, const Point& b); //in class
        friend Point operator-(const Point& a, const Point& b); //in class
        friend double operator*(const Point& a, const Point& b); //in class
        friend Point operator*(const Point& a, const double b); //in class
        friend Point operator*(const double b, const Point& a); //in class
        friend Point operator/(const Point& a, const double b); //in class
        friend Point operator/(const double b, const Point& a); //in class

        Point reverse_self();
        Point reverse() const;
        Point rotate_self(double theta);
        Point rotate(double theta) const;
        Point normalize_self();
        Point normalize() const;
        Point normal_self(); // compute the normal direction
        Point normal() const; // compute the normal direction
        double length() const;
        double lengthsquared() const;
        double atan() const;
        double atan2() const;
        double distance(const Point& other) const;
        double distancesquared(const Point& other) const;
        double slope(const Point& other) const;
        double slope() const;
        Line axis(const Point& other) const;
        Point move_self(const Line& line, const double dist);
        Point move_self(const Point& direction, const double dist);
        Point move(const Line& line, const double dist) const;
        Point move(const Point& direction, const double dist) const;

        bool iszero() const;
        bool isversor() const;
        double getx() const;
        double gety() const;
        void setx(double x);
        void sety(double y);


        string to_str() const;
        void print() const;
        //ostream & operator<<(ostream & Str);
        friend ostream & operator<<(ostream & Stream, Point const & v)
        {
            // print something from v to str, e.g: Str << v.getX();
            Stream << v.x << "," << v.y;
            return Stream;
        }
        friend stringstream & operator<<(stringstream & Stream, Point const & v)
        {
            // print something from v to str, e.g: Str << v.getX();
            Stream << v.x << "," << v.y;
            return Stream;
        }
        operator string() const
        {
            return this->to_str();
        }
        bool operator==(const Point& b);

        //friend static double slope(const Point& a, const Point& b);

        static void normalize(Points &points);
        static void normals(Points &points);
        static Points fromDoubles(doubles &x, doubles &y);
        static void toDoubles(const Points& points, doubles &x, doubles &y);

    protected:
        double x, y;

    private:
};

#endif // POINT_H
