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

//усножение для отсортированного представления 
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

//работает
double** Calc::mult(MatrixCOO& m1, MatrixCOO& m2)
{
  double* tmp = new double[m1.getElCount() * m1.getElCount()] {0.0};
  double** res = new double*[m1.getMatrixSize()];
  for (int i = 0; i < m1.getMatrixSize(); i++)
	res[i] = tmp + i * m1.getMatrixSize();

  for (int i = 0; i < m1.getElCount(); i++)
	for (int j = 0; j < m2.getElCount(); j++)
	  if (m1.Col()[i] == m2.Row()[j])
		res[m1.Row()[i]][m2.Col()[j]] += m1.Values()[i] * m2.Values()[j];
  
  return res;
}


double** Calc::multSort(MatrixCOO& m1, MatrixCOO& m2)
{
  double* tmp = new double[m1.getElCount() * m1.getElCount()] {0.0};
  double** res = new double* [m1.getMatrixSize()];
  for (int i = 0; i < m1.getMatrixSize(); i++)
	res[i] = tmp + i * m1.getMatrixSize();
  
  int* rowIndexM2 = new int[m2.getMatrixSize()];
  int* elAdd = new int[m2.getMatrixSize()];

  for (int i = 0; i < m2.getMatrixSize(); i++)
  {
	rowIndexM2[i] = 0;
	elAdd[i] = 0;
  }

  for (int i = 0; i < m2.getElCount(); i++)
	rowIndexM2[m2.Row()[i]]++;

  for (int i = 1; i < m2.getMatrixSize(); i++)
	elAdd[i] = elAdd[i - 1] + rowIndexM2[i-1];
 // cout << endl;
 // for (int i = 0; i < m2.getMatrixSize(); i++)
	//cout << rowIndexM2[i] << ' ';
 // cout << endl;

  for (int i = 0; i < m1.getElCount(); i++)
  {
	int col = m1.Col()[i];
	for(int j = 0; j < rowIndexM2[col]; j++)
	  res[m1.Row()[i]][m2.Col()[j + elAdd[col]]] += m1.Values()[i] * m2.Values()[j + elAdd[col]];
  }
  return res;
}
