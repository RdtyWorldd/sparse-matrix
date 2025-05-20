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
  MatrixCRS(int _matrixSize, int*& _rowIndex);

  ~MatrixCRS();
  MatrixCRS& operator= (const MatrixCRS& matrix);

  int getMatrixSize() const
  {
    return matrixSize;
  }
  int getElCount() const
  {
    return elCount;
  }

  double* const Values() const
  {
    return values;
  }
  int* const RowIndex() const
  {
    return rowIndex;
  }
  int* const Col() const
  {
    return col;
  }

  friend ostream& operator << (ostream& out, const MatrixCRS& matrix);
  friend istream& operator >> (istream& in, MatrixCRS& matrix);
};
