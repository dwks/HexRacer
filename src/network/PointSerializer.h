#ifndef PROJECT_NETWORK__POINT_SERIALIZER_H
#define PROJECT_NETWORK__POINT_SERIALIZER_H

#include "boost/serialization/split_free.hpp"

#include "math/Point.h"

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

}  // namespace serialization
}  // namespace boost

BOOST_SERIALIZATION_SPLIT_FREE(Project::Math::Point)

#endif
