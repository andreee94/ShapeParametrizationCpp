#ifndef POINT_H
#define POINT_H

class Line;

class Point
{
    public:
        double x, y;
        /** Default constructor */
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
        Point& operator=(const Point& other);

        friend Point operator+(const Point& a, const Point& b); //in class
        friend Point operator-(const Point& a, const Point& b); //in class

        double slope(const Point& other) const;
        Line axis(const Point& other) const;
        //friend static double slope(const Point& a, const Point& b);

        // TODO override operators sum and others

    protected:

    private:
};

#endif // POINT_H
