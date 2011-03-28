#include "SimpleTrackball.h"
#include <math.h>
#include "math/Values.h"
#include "math/Geometry.h"
using namespace std;
using namespace Project;
using namespace Math;
using namespace OpenGL;

SimpleTrackball::SimpleTrackball(double x_scale, double y_scale)
{
	xScale = x_scale;
	yScale = y_scale;
	longitude = PI*0.5;
	latitude = 0.0f;

	updateSpherePoint();

}

SimpleTrackball::~SimpleTrackball(void)
{
}

void SimpleTrackball::setMouseStartAt(Point point) {
	lastPoint = point;
}

void SimpleTrackball::setMouseCurrentAt(Point point) {

	longitude += (point.getU()-lastPoint.getU())*xScale;
	longitude = smartMod(longitude, PI*2.0);
	latitude += (point.getV()-lastPoint.getV())*yScale;
	latitude = bound(latitude, -PI*0.45, PI*0.45);

	updateSpherePoint();

	lastPoint = point;
}

Point SimpleTrackball::getSpherePoint() {
	return spherePoint;
}

void SimpleTrackball::setSpherePoint(Point point) {

	point.normalize();

	Math::Point longitude_vector = Point::point2D(point, Y_AXIS);
	setLongitude(Geometry::vectorTo2DAngle(longitude_vector, Y_AXIS));
	float new_latitude = asin(point.getY());
	setLatitude(new_latitude);
}

void SimpleTrackball::updateSpherePoint() {
	spherePoint.setX(cos(longitude)*cos(latitude));
	spherePoint.setY(sin(latitude));
	spherePoint.setZ(sin(longitude)*cos(latitude));
}

void SimpleTrackball::setLongitude(double _longitude) {
	longitude = smartMod(_longitude, PI*2.0);
	updateSpherePoint();
}

void SimpleTrackball::setLatitude(double _latitude) {
	latitude = smartMod(_latitude, PI*2.0);
	latitude = bound(_latitude, -PI*0.45, PI*0.45);
	updateSpherePoint();
}