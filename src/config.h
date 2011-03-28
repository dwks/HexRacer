#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#define CONFIG_FILE "config.txt"

// enables some Boost-specific (but more portable) code
#define HAVE_BOOST

#ifdef HAVE_BOOST
    #define HAVE_BOOST_FILESYSTEM
#endif

#ifdef HAVE_BOOST
    #define HAVE_BOOST_SYSTEM
#endif

// enables sound
#define HAVE_OPENAL

#define VEHICLE_CHASSIS_MODEL_NAME "vehicleChassis"
#define VEHICLE_NONCHASSIS_MODEL_NAME "vehicleNonChassis"
#define VEHICLE_WHEEL_MODEL_NAME "vehicleWheel"

#define PAINT_CELL_RADIUS 0.425
#define PAINT_CELL_VERTEX_HEIGHT_THRESHHOLD 1.0
#define PAINT_CELL_LIFT_AMOUNT 0.075
#define PAINT_CELL_CONTRACTION 0.1

#define PAINTING_RADIUS 1.25

#define INITIAL_CAR_LOCATION Math::Point(0.0, 2.0, 0.0)
#define VIEW_DISTANCE 200.0

#define VEHICLE_WARP_Y_OFFSET 1.5
#define VEHICLE_PATH_RAY_MAX_HEIGHT 10.0

#define MAP_KILL_PLANE_Y_GAP -20.0

#define MAX_PATHNODE_DISTANCE 40.0

#define LIGHT_MANAGER_MIN_ATTENUATION_THRESHHOLD 0.01

#endif
