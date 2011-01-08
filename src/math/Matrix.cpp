#include <iostream>
#include <iomanip>
#include "opengl/OpenGL.h"

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
