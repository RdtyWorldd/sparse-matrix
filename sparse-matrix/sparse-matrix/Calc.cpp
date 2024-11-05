#include "Calc.h"

MatrixCOO Calc::transposition(MatrixCOO& matrix)
{
  MatrixCOO res(matrix);
  for(int i =0;i < matrix.getElCount(); i++)
  {
	res.Row()[i] = matrix.Col()[i];
	res.Col()[i] = matrix.Row()[i];
  }
  return res;
}

double* Calc::vectorMultSort(MatrixCOO& matrix, double* vector)
{
  int size = matrix.getMatrixSize();
  double* res = new double[size] {0.0};
  int j = 0;
  for (int i = 0; i < size; i++)
  {
	double sum = 0;
	while ((j <= size * size)&& (matrix.Row()[j] == i))
	{
	  sum += matrix.Values()[j] * vector[matrix.Col()[j]];
	  j++;
	}
	res[i] = sum;
  }
  return res;
}

double* Calc::vectorMult(MatrixCOO& matrix, double* vector)
{
  double* res = new double[matrix.getMatrixSize()] {0.0};
  for (int i = 0; i < matrix.getElCount(); i++)
  {
	res[matrix.Row()[i]] += matrix.Values()[i] * vector[matrix.Col()[i]];
  }
  return res;
}