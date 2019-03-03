# ShapeParametrizationCpp
A shape parametrization tool which uses bspline curves in c++

The tool provides the GUI to interpolate a set of points with B-Spline curves.
An extensive set of knots is available, and in the future will be possible to optimize the knot sequence.

The tool has been developed with in mind the interpolation of blades, 
 so than it is possible to remove the trailing edge points and interpolate it independently with a circle or an ellipse.

The moving the control points orthogonally respect to the curve can be done directly within the GUI.
