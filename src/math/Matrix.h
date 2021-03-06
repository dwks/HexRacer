#ifndef PROJECT_MATH__MATRIX_H
#define PROJECT_MATH__MATRIX_H

#include <iosfwd>

#include "Point.h"
#include "Triangle3D.h"

namespace Project {
namespace Math {

/** A 4x4 matrix.
*/
class Matrix {
public:
    static const int WIDTH = 4, HEIGHT = 4;
private:
    double matrix[WIDTH * HEIGHT];
public:
    Matrix();
    Matrix(const double data[WIDTH * HEIGHT]);
    
    static Matrix getTranslationMatrix(const Math::Point &translation);
    static Matrix getScalingMatrix(const Math::Point &scale);
	static Matrix getScalingMatrix(double scale);
	static Matrix getRotationMatrix(Axis axis, double angle);
    
    void setToIdentity();
    
    const double *getData() const { return matrix; }
    double *getData() { return matrix; }
    
    void set(int row, int col, double value);
    double get(int row, int col) const;
    
    void operator = (const Matrix &other);
    
    Point operator * (const Point &point) const;
	Triangle3D operator * (const Triangle3D &triangle) const;
    Matrix operator * (const Matrix &other) const;
	void operator *= (const Matrix &other);
    
    bool operator == (const Matrix &other) const;
    bool operator != (const Matrix &other) const;
    
    /** Returns an element of the matrix, where @a index is of the form
        4 * col + row.
    */
    double operator [] (int index) const;
    
    double &operator [] (int index);
    
    /** Note: this is only defined for square matrices.
    */
    Matrix transpose() const;
};

std::istream &operator >> (std::istream &stream, Matrix &matrix);
std::ostream &operator << (std::ostream &stream, const Matrix &matrix);

}  // namespace Math
}  // namespace Project

#endif
