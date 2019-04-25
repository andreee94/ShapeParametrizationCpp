# Shape Parametrization
A shape parametrization tool which uses bspline curves in c++

The tool provides the GUI to interpolate a set of points with B-Spline curves.
An extensive set of knots is available, and in the future will be possible to optimize the knot sequence.

The tool has been developed with in mind the interpolation of blades, 
 so than it is possible to remove the trailing edge points and interpolate it independently with a circle or an ellipse.

The control points are moved orthogonally respect to the curve directly within the GUI.

# Release file:
The file to run the program is: `QTShapeParametrization-run`

# INSTALL
- Eigen Library
`sudo apt install libeigen3-dev`
- Boost Library
`sudo apt-get install libboost-all-dev`

# TODO
- [ ] Knots optimization.
	- Choose between the knots which are fixed and which one to optimize

