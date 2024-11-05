#pragma once
#include <iostream>

using namespace std;

#define MAX_MATRIX_SIZE 1000

class MatrixCOO {
  double* values;
  int* row;
  int* col;

  int matrixSize;
  int elCount;


public:
  MatrixCOO(int _matrixSize, int el_count);
  MatrixCOO(const MatrixCOO& matrix);
  ~MatrixCOO();
  MatrixCOO& operator= (const MatrixCOO& matrix);

  int getMatrixSize();
  int getElCount();

  double* Values() const;
  int* Row() const;
  int* Col() const;

  friend ostream& operator << (ostream& out, const MatrixCOO& matrix);
  friend istream& operator >> (istream& in, MatrixCOO& matrix);
};