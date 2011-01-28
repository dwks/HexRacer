#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

// enables some Boost-specific (but more portable) code
#define HAVE_BOOST

// enables sound
//#define HAVE_OPENAL

#define PAINT_CELL_RADIUS 0.75
#define PAINT_CELL_VERTEX_HEIGHT_THRESHHOLD 1.0
#define PAINT_CELL_LIFT_AMOUNT 0.085
#define PAINT_CELL_CONTRACTION 0.1

#define INITIAL_CAR_LOCATION Math::Point(0.0, 2.0, 0.0)
#define VIEW_DISTANCE 200.0

#endif
