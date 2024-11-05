#include "MatrixCOO.h"

MatrixCOO::MatrixCOO(int _matrixSize, int _elCount)
{
  matrixSize = _matrixSize;
  elCount = _elCount;

  values = new double[elCount] {0};
  row = new int[elCount] {0};
  col = new int[elCount] {0};
}

MatrixCOO::MatrixCOO(const MatrixCOO& matrix)
{
  matrixSize = matrix.matrixSize;
  elCount = matrix.elCount;

  values = new double[elCount];
  row = new int[elCount] {0};
  col = new int[elCount] {0};

  for (int i = 0; i < elCount; i++)
  {
    values[i] = matrix.values[i];
    row[i] = matrix.row[i];
    col[i] = matrix.col[i];
  }
}

MatrixCOO& MatrixCOO::operator=(const MatrixCOO& matrix)
{
  if (this != &matrix)
  {
    matrixSize = matrix.matrixSize;
    if (elCount != matrix.elCount)
    {
      elCount = matrix.elCount;
      delete[] values;
      delete[] row;
      delete[] col;

      values = new double[elCount];
      row = new int[elCount] {0};
      col = new int[elCount] {0};
    }
    for (int i = 0; i < elCount; i++)
    {
      values[i] = matrix.values[i];
      row[i] = matrix.row[i];
      col[i] = matrix.col[i];
    }
  }
  return *this;
}

MatrixCOO::~MatrixCOO()
{
  delete[] values;
  delete[] row;
  delete[] col;
}

int MatrixCOO::getMatrixSize() { return matrixSize; }
int MatrixCOO::getElCount() { return elCount; }

double* MatrixCOO::Values() const { return values; }
int* MatrixCOO::Row() const { return row; }
int* MatrixCOO::Col() const { return col; }

ostream& operator<<(ostream& out, const MatrixCOO& matrix)
{
  out << matrix.matrixSize << ' ' << matrix.elCount << '\n';
  for (int i = 0; i < matrix.elCount; i++)
  {
    out << matrix.values[i] << ' ';
  }
  out << '\n';
  for (int i = 0; i < matrix.elCount; i++)
  {
    out << matrix.row[i] << ' ';
  }
  out << '\n';
  for (int i = 0; i < matrix.elCount; i++)
  {
    out << matrix.col[i] << ' ';
  }
  out << endl;
  return out;
}

istream& operator>>(istream& in, MatrixCOO& matrix)
{
  int elCount = 0;
  in >> matrix.matrixSize >> elCount;
  if (matrix.elCount != elCount)
  {
    matrix.elCount = elCount;
    delete[] matrix.values;
    delete[] matrix.row;
    delete[] matrix.col;

    matrix.values = new double[elCount];
    matrix.row = new int[elCount] {0};
    matrix.col = new int[elCount] {0};
  }
  for (int i = 0; i < matrix.elCount; i++)
  {
    in >> matrix.values[i];
  }

  for (int i = 0; i < matrix.elCount; i++)
  {
    in >> matrix.row[i];
  }

  for (int i = 0; i < matrix.elCount; i++)
  {
    in >> matrix.col[i];
  }

  return in;
}