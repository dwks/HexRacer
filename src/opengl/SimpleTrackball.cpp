#include "SimpleTrackball.h"
#include <math.h>
#include "math/Values.h"
using namespace std;
using namespace Project;
using namespace Math;
using namespace OpenGL;

SimpleTrackball::SimpleTrackball(float x_scale, float y_scale)
{
	xScale = x_scale;
	yScale = y_scale;
	longitude = PI*0.5f;
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
	longitude = smartMod(longitude, (float) PI*2.0f);
	latitude += (point.getV()-lastPoint.getV())*yScale;
	latitude = bound(latitude, (float) -PI*0.45f, (float) PI*0.45f);

	updateSpherePoint();

	lastPoint = point;
}

Point SimpleTrackball::getSpherePoint() {
	return spherePoint;
}

void SimpleTrackball::updateSpherePoint() {
	spherePoint.setX(cos(longitude)*cos(latitude));
	spherePoint.setY(sin(latitude));
	spherePoint.setZ(sin(longitude)*cos(latitude));
}