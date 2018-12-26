#ifndef POINT_H
#define POINT_H

#include <vector>
#include <cmath>
#include <Utils.h>
#include <iostream>

using namespace std;

class Line;
class Point;

typedef std::vector<double> doubles;
typedef std::vector<Point> Points;

class Point
{
    public:
        double x, y;
        /** Default constructor */
        Point();
        Point(int value);
        Point(double value);
        Point(int x, int y);
        Point(double x, double y);
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
        //Point& operator=(const Point& other);

        friend Point operator+(const Point& a, const Point& b); //in class
        friend Point operator-(const Point& a, const Point& b); //in class
        friend double operator*(const Point& a, const Point& b); //in class
        friend Point operator*(const Point& a, const double b); //in class
        friend Point operator*(const double b, const Point& a); //in class
        friend Point operator/(const Point& a, const double b); //in class
        friend Point operator/(const double b, const Point& a); //in class

        Point reverse();
        Point reverse() const;
        Point rotate(double theta);
        Point rotate(double theta) const;
        Point normalize();
        Point normalize() const;
        Point normal(); // compute the normal direction
        Point normal() const; // compute the normal direction
        double length() const;
        double lengthsquared() const;
        double atan() const;
        double atan2() const;
        double distance(const Point& other) const;
        double distancesquared(const Point& other) const;
        double slope(const Point& other) const;
        Line axis(const Point& other) const;
        Point move(const Line& line, const double dist) const;
        Point move(const Point& direction, const double dist) const;

        void print() const;
        //friend static double slope(const Point& a, const Point& b);

        static void normalize(Points &points);
        static void normals(Points &points);

    protected:

    private:
};

#endif // POINT_H
