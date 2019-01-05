#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

class matrix {
 public:
    matrix(int i, int j);
    matrix(const matrix& m);
    ~matrix();

    int n_rows();
    int n_cols();

    float *access(const int i, const int j) const;
    friend std::ostream& operator<<(std::ostream &os, matrix &m);
    friend matrix operator+(const matrix&, const matrix&);
    friend matrix operator*(const matrix&, const matrix&);
    matrix& operator=(const matrix&);

    static matrix matrix_read(std::string filename);
 private:
    matrix() {}
    int rows;
    int cols;
    float *data;
};

#endif  // PROJECT_INCLUDE_MATRIX_H_
