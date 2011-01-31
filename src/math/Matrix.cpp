#include <iostream>
#include <iomanip>
#include "../opengl/OpenGL.h"
#include <math.h>

#include "Matrix.h"

namespace Project {
namespace Math {

Matrix::Matrix() {
    setToIdentity();
}

Matrix::Matrix(const double data[WIDTH * HEIGHT]) {
    for(int col = 0; col < WIDTH; col ++) {
        for(int row = 0; row < HEIGHT; row ++) {
            matrix[WIDTH * col + row] = data[WIDTH * col + row];
        }
    }
}

Matrix Matrix::getTranslationMatrix(const Math::Point &translation) {
    Matrix m;  // identity by default
    m.set(0, 3, translation.getX());
    m.set(1, 3, translation.getY());
    m.set(2, 3, translation.getZ());
    
    return m;
}

Matrix Matrix::getScalingMatrix(const Math::Point &scale) {
    Matrix m;  // identity by default
    m.set(0, 0, scale.getX());
    m.set(1, 1, scale.getY());
    m.set(2, 2, scale.getZ());
    
    return m;
}

Matrix Matrix::getScalingMatrix(double scale) {
    Matrix m;  // identity by default
    m.set(0, 0, scale);
    m.set(1, 1, scale);
    m.set(2, 2, scale);
    
    return m;
}

Matrix Matrix::getRotationMatrix(Axis axis, double angle) {
	Matrix m;

	double cos_a = cos(angle);
	double sin_a = sin(angle);

	switch (axis) {

		case X_AXIS:
			m.set(1, 1, cos_a);
			m.set(1, 2, -sin_a);
			m.set(2, 1, sin_a);
			m.set(2, 2, cos_a);
			return m;

		case Y_AXIS:
			m.set(0, 0, cos_a);
			m.set(0, 2, sin_a);
			m.set(2, 0, -sin_a);
			m.set(2, 2, cos_a);
			return m;

		case Z_AXIS:
			m.set(0, 0, cos_a);
			m.set(0, 1, -sin_a);
			m.set(1, 0, sin_a);
			m.set(1, 1, cos_a);
			return m;

		default:
			return m;
	}
}

void Matrix::setToIdentity() {
    for(int col = 0; col < WIDTH; col ++) {
        for(int row = 0; row < HEIGHT; row ++) {
            matrix[WIDTH * col + row] = (row == col ? 1 : 0);
        }
    }
}

void Matrix::set(int row, int col, double value) {
    matrix[WIDTH * col + row] = value;
}

double Matrix::get(int row, int col) const {
    return matrix[WIDTH * col + row];
}

void Matrix::operator = (const Matrix &other) {
    for(int col = 0; col < WIDTH; col ++) {
        for(int row = 0; row < HEIGHT; row ++) {
            matrix[WIDTH * col + row] = other[WIDTH * col + row];
        }
    }
}

Point Matrix::operator * (const Point &point) const {
    Point result;
    
    for(int col = 0; col < WIDTH; col ++) {
        for(int row = 0; row < HEIGHT; row ++) {
            result[row] += matrix[WIDTH * col + row] * point[col];
        }
    }
    
    return result;
}

Matrix Matrix::operator * (const Matrix &other) const {
    Matrix result;
    
    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            double sum = 0.0;
            for(int n = 0; n < WIDTH; n ++) {
                sum += get(row, n) * other.get(n, col);
            }
            
            result.set(row, col, sum);
        }
    }
    
    return result;
}

bool Matrix::operator == (const Matrix &other) const {
    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            if(get(row, col) != other.get(row, col)) return false;
        }
    }
    
    return true;
}

bool Matrix::operator != (const Matrix &other) const {
    return !this->operator == (other);
}

double Matrix::operator [] (int index) const {
    return matrix[index];
}

double &Matrix::operator [] (int index) {
    return matrix[index];
}

Matrix Matrix::transpose() const {
    Matrix trans;
    
    for(int col = 0; col < WIDTH; col ++) {
        for(int row = 0; row < HEIGHT; row ++) {
            trans[WIDTH * row + col] = matrix[WIDTH * col + row];
        }
    }
    
    return trans;
}

std::istream &operator >> (std::istream &stream, Matrix &matrix) {
    for(int row = 0; row < Matrix::HEIGHT; row ++) {
        for(int col = 0; col < Matrix::WIDTH; col ++) {
            double value;
            stream >> value;
            matrix[Matrix::WIDTH * col + row] = value;
        }
    }
    
    return stream;
}

std::ostream &operator << (std::ostream &stream, const Matrix &matrix) {
    for(int row = 0; row < Matrix::HEIGHT; row ++) {
        for(int col = 0; col < Matrix::WIDTH; col ++) {
            stream << std::setw(3) << std::setprecision(2)
                << matrix.get(row, col);
            if(col + 1 < Matrix::WIDTH) stream << " ";
        }
        stream << "\n";
    }
    
    return stream;
}

}  // namespace Math
}  // namespace Project
