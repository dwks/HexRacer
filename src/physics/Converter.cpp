#include "Converter.h"

#include "log/Logger.h"

namespace Project {
namespace Physics {

Math::Point Converter::toPoint(const btVector3 &vector) {
    return Math::Point(
        vector.getX(),
        vector.getY(),
        vector.getZ());
}

btVector3 Converter::toVector(const Math::Point &point) {
    return btVector3(
        point.getX(),
        point.getY(),
        point.getZ());
}

Math::Matrix Converter::toMatrix(const btTransform &transform) {
    btScalar data[16];
    transform.getOpenGLMatrix(data);
    
    double doubleData[16];
    for(int i = 0; i < 16; i ++) doubleData[i] = data[i];
    
    return Math::Matrix(doubleData);
}

btTransform Converter::toTransform(const Math::Matrix &matrix) {
    btTransform transform;
    
    btScalar data[16];
    for(int i = 0; i < 16; i ++) data[i] = matrix.getData()[i];
    
    transform.setFromOpenGLMatrix(data);
    
    return transform;
}

}  // namespace Physics
}  // namespace Project
