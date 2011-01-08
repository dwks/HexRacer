#ifndef PROJECT_SDL__POINT2_D_H
#define PROJECT_SDL__POINT2_D_H

namespace Project {
namespace SDL {

/** Represents a point in SDL screen space.
*/
class Point2D {
private:
    int x, y;
public:
    Point2D() : x(0), y(0) {}
    Point2D(int x, int y) : x(x), y(y) {}
    
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

}  // namespace SDL
}  // namespace Project

#endif
