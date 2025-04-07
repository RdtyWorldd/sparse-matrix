#pragma once
#include <iostream>

using namespace std;

class MatrixCRS 
{
  int matrixSize;
  int elCount;

  double* values;
  int* col;
  int* rowIndex;
public:
  MatrixCRS(int _matrixSize, int el_count);
  MatrixCRS(const MatrixCRS& matrix);

  ~MatrixCRS();
  MatrixCRS& operator= (const MatrixCRS& matrix);

  int getMatrixSize() const;
  int getElCount() const;

  double* Values() const;
  int* RowIndex() const;
  int* Col() const;

  friend ostream& operator << (ostream& out, const MatrixCRS& matrix);
  friend istream& operator >> (istream& in, MatrixCRS& matrix);
};
