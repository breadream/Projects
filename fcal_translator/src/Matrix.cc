#include "include/Matrix.h"
#include <fstream>
#include <sstream>
#include <string>


int matrix::n_rows() {
  return rows;
}

int matrix::n_cols() {
  return cols;
}

float* matrix::access(const int i, const int j) const {
  return data + i*cols + j;
}

matrix::~matrix() {
  if (data) delete [] data;
}

matrix::matrix(int i, int j) {
  if (i < 1 || j < 1) {
    throw "Attempt to initialize array with a dimension < 0, invalid.";
  }
  rows = i;
  cols = j;
  data = new float[i*j];
}

matrix::matrix(const matrix& m) {
  rows = m.rows;
  cols = m.cols;
  data = new float[rows*cols];
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      data[i*cols + j] = m.data[i*cols + j];
    }
  }
}

matrix& matrix::operator=(const matrix& m) {
  rows = m.rows;
  cols = m.cols;
  if (data) delete [] data;
  data = new float[rows*cols];
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      data[i*cols + j] = m.data[i*cols + j];
    }
  }
  return *this;
}

std::ostream& operator<<(std::ostream &os, matrix &m) {
  os << m.n_rows() << " " << m.n_cols() << "\n";

  for (int i = 0; i < m.n_rows(); i++) {
    for (int j = 0; j < m.n_cols(); j++) {
      os << *(m.access(i, j)) << "  ";
    }
    os << "\n";
  }
  return os;
}

matrix operator+(const matrix& m1, const matrix& m2) {
  if (m1.rows != m2.rows || m1.cols != m2.cols)
    throw std::string("Attempt to add matrices with invalid dimensions\n");

  matrix output(m1.rows, m1.cols);
  int num_rows = m1.rows;
  int num_cols = m1.cols;
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      output.data[i*num_rows + j] =
      m1.data[i*num_rows + j] + m2.data[i*num_rows + j];
    }
  }
  return output;
}

matrix operator*(const matrix& m1, const matrix& m2) {
  int m1_rows = m1.rows;
  int m2_rows = m2.rows;
  int m1_cols = m1.cols;
  int m2_cols = m2.cols;
  int total_sum = 0;
  matrix output = matrix(m1_rows, m2_cols);
  for (int i = 0; i < m1_rows; i++) {
    for (int j = 0; j < m2_cols; j++) {
      for (int cross = 0; cross < m1_cols; cross++) {
        total_sum = m1.data[i*m1_rows + cross] * m2.data[cross*m2_rows + j];
      }
      output.data[i*m1_rows + j] = total_sum;
      total_sum = 0;
    }
  }
  return output;
}

// static member function that returns a matrix object
matrix matrix::matrix_read(std::string filename) {
  int num_rows;
  int num_cols;
  std::ifstream infile(filename.c_str());
  std::string line;

  // Get the numbers of rows and columns from the first line of a file
  std::getline(infile, line);
  std::istringstream iss(line);
  if (!(iss >> num_rows >> num_cols)) {
    throw std::string("parsing error!");
  }
  matrix output = matrix(num_rows, num_cols);
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      infile >> *(output.access(i, j));
    }
  }
  infile.close();
  return output;
}

