#include "TrailingEdge.h"
#include "Ellipse.h"
#include "Circle.h"
#include "Line.h"
#include "Point.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <array>


TrailingEdge::TrailingEdge()
{
    //ctor
}

TrailingEdge::~TrailingEdge()
{
    //dtor
}


Points TrailingEdge::computeTE(Points &points, int steps)
{
    Point p1 = points.front(); // first bspline point
    Point p2 = points.back(); // latest bspline point

    // calculate alpha of the 2 points
    double alpha1 = this->getangleofpoint(p1);
    double alpha2 = this->getangleofpoint(p2);

    // bring alpha1, alpha2 between 0 and 2pi
    alpha1 = std::fmod((alpha1 + 2 * M_PI),(2 * M_PI));
    alpha2 = std::fmod((alpha2 + 2 * M_PI),(2 * M_PI));

    // now we have to choose the range:
    // --> (alpha2 - alpha1)
    // --> (alpha1 - alpha2)
    // we compute the first point and we take the range with longer distance to second point of the curve

    double alpha1_guess = std::min(alpha1, alpha2) + std::abs(alpha1 - alpha2) / steps; // alpha of the first value of the step
    double alpha2_guess = std::min(alpha1, alpha2) - std::abs(alpha1 - alpha2) / steps; // alpha of the last value of the step

    Point p1_guess = this->evaluate(alpha1_guess);
    Point p2_guess = this->evaluate(alpha2_guess);

    // depending on alpha1 and alpha2 take second or second-last point
    Point distancePoint = alpha2 > alpha1 ? points[1] : points[points.size() - 2];

    if (p2_guess.distance(distancePoint) > p1_guess.distance(distancePoint))
        return this->evaluate(max(alpha1, alpha2) - 2 * M_PI, min(alpha1, alpha2), steps);
    else
        return this->evaluate(min(alpha1, alpha2), max(alpha1, alpha2), steps);

}
