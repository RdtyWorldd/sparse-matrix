#include "MatrixCRS.h"

MatrixCRS::MatrixCRS(int _matrixSize, int el_count)
{
  matrixSize = _matrixSize;
  elCount = el_count;
  values = new double[elCount] {0.0};
  col = new int[elCount] {0};
  rowIndex = new int[matrixSize + 1] {0};
}

MatrixCRS::MatrixCRS(const MatrixCRS& matrix)
{
  matrixSize = matrix.matrixSize;
  elCount = matrix.elCount;

  values = new double[elCount];
  col = new int[elCount] {0};
  rowIndex = new int[matrixSize + 1] {0};

  for (int i = 0; i < elCount; i++)
  {
    values[i] = matrix.values[i];
    col[i] = matrix.col[i];
  }

  for (int i = 0; i < matrixSize + 1; i++)
    rowIndex[i] = matrix.rowIndex[i];
}


MatrixCRS::~MatrixCRS()
{
  delete[] values;
  delete[] rowIndex;
  delete[] col;
}

MatrixCRS& MatrixCRS::operator=(const MatrixCRS& matrix)
{
  matrixSize = matrix.matrixSize;
  if (elCount != matrix.elCount)
  {
    elCount = matrix.elCount;
    delete[] values;
    delete[] rowIndex;
    delete[] col;

    values = new double[elCount];
    col = new int[elCount] {0};
    rowIndex = new int[matrixSize + 1] {0};
  }
  for (int i = 0; i < elCount; i++)
  {
    values[i] = matrix.values[i];
    col[i] = matrix.col[i];
  }
  for (int i = 0; i < matrixSize + 1; i++)
    rowIndex[i] = matrix.rowIndex[i];
  return *this;
}

//int MatrixCRS::getMatrixSize() const
//{
//  return matrixSize;
//}
//
//int MatrixCRS::getElCount() const
//{
//  return elCount;
//}
//
//double* MatrixCRS::Values() const
//{
//  return values;
//}
//
//int* MatrixCRS::RowIndex() const
//{
//  return rowIndex;
//}
//
//int* MatrixCRS::Col() const
//{
//  return col;
//}

ostream& operator<<(ostream& out, const MatrixCRS& matrix)
{
  out << matrix.matrixSize << ' ' << matrix.elCount << '\n';
  for (int i = 0; i < matrix.elCount; i++)
    out << matrix.values[i] << ' ';

  out << '\n';
  for (int i = 0; i < matrix.elCount; i++)
    out << matrix.col[i] << ' ';

  out << '\n';
  for (int i = 0; i < matrix.matrixSize + 1; i++)
    out << matrix.rowIndex[i] << ' ';
  
  out << endl;
  return out;
}

istream& operator>>(istream& in, MatrixCRS& matrix)
{
  int elCount = 0;
  int matrixSize = 0;
  in >> matrixSize >> elCount;
  if (matrix.matrixSize != matrixSize)
  {
    matrix.matrixSize = matrixSize;
    delete[] matrix.rowIndex;
    matrix.rowIndex = new int[matrixSize + 1];
  }
  if (matrix.elCount != elCount)
  {
    matrix.elCount = elCount;
    delete[] matrix.values;
    delete[] matrix.col;

    matrix.values = new double[elCount];
    matrix.col = new int[elCount] {0};
  }
  for (int i = 0; i < matrix.elCount; i++)
  {
    in >> matrix.values[i];
  }
  for (int i = 0; i < matrix.elCount; i++)
  {
    in >> matrix.col[i];
  }
  for (int i = 0; i < matrix.matrixSize + 1; i++)
  {
    in >> matrix.rowIndex[i];
  }

  return in;
}
