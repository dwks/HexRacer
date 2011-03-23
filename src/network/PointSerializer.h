#ifndef PROJECT_NETWORK__POINT_SERIALIZER_H
#define PROJECT_NETWORK__POINT_SERIALIZER_H

#include "boost/serialization/split_free.hpp"

#include "math/Point.h"
#include "math/Matrix.h"
#include "opengl/Color.h"

namespace boost {
namespace serialization {

template <typename Archive>
void save(Archive &ar, const Project::Math::Point &point,
    const unsigned version) {
    
    double x = point.getX();
    ar << x;
    double y = point.getY();
    ar << y;
    double z = point.getZ();
    ar << z;
    double w = point.getW();
    ar << w;
}

template <typename Archive>
void load(Archive &ar, Project::Math::Point &point, const unsigned version) {
    double x, y, z, w;
    ar >> x >> y >> z >> w;
    
    point.setX(x);
    point.setY(y);
    point.setZ(z);
    point.setW(w);
}

template <typename Archive>
void save(Archive &ar, const Project::Math::Matrix &matrix,
    const unsigned version) {
    
    const double *data = matrix.getData();
    
    for(int i = 0; i < 16; i ++) {
        ar << data[i];
    }
}

template <typename Archive>
void load(Archive &ar, Project::Math::Matrix &matrix, const unsigned version) {
    double data[16];
    
    for(int i = 0; i < 16; i ++) {
        ar >> data[i];
    }
    
    matrix = Project::Math::Matrix(data);
}

template <typename Archive>
void save(Archive &ar, const Project::OpenGL::Color &color,
    const unsigned version) {
    
    float r = color.redf();
    float g = color.greenf();
    float b = color.bluef();
    float a = color.alphaf();
    
    ar << r << g << b << a;
}

template <typename Archive>
void load(Archive &ar, Project::OpenGL::Color &color, const unsigned version) {
    float r, g, b, a;
    ar >> r >> g >> b >> a;
    
    color = Project::OpenGL::Color(r, g, b, a);
}

}  // namespace serialization
}  // namespace boost

BOOST_SERIALIZATION_SPLIT_FREE(Project::Math::Point)
BOOST_SERIALIZATION_SPLIT_FREE(Project::Math::Matrix)
BOOST_SERIALIZATION_SPLIT_FREE(Project::OpenGL::Color)

#endif
