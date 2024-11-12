#include "Calc.h"

void Calc::transposition(MatrixCOO& matrix, MatrixCOO& matrixT)
{
  for(int i =0;i < matrix.getElCount(); i++)
  {
	matrixT.Values()[i] = matrix.Values()[i];
	matrixT.Row()[i] = matrix.Col()[i];
	matrixT.Col()[i] = matrix.Row()[i];
  }
}

//усножение для отсортированного представления 
void Calc::vectorMultSort(MatrixCOO& matrix, double* multV, double* resV)
{
  int size = matrix.getMatrixSize();
  int j = 0;
  for (int i = 0; i < size; i++)
  {
	double sum = 0;
	while ((j <= size * size)&& (matrix.Row()[j] == i))
	{
	  sum += matrix.Values()[j] * multV[matrix.Col()[j]];
	  j++;
	}
	resV[i] = sum;
  }
}

void Calc::vectorMult(MatrixCOO& matrix, double* multV, double* resV)
{
  for (int i = 0; i < matrix.getElCount(); i++)
  {
	resV[matrix.Row()[i]] += matrix.Values()[i] * multV[matrix.Col()[i]];
  }
}

//работает
//обнуление матриц
void Calc::mult(MatrixCOO& m1, MatrixCOO& m2, double** denseM)
{
  for (int i = 0; i < m1.getElCount(); i++)
	for (int j = 0; j < m2.getElCount(); j++)
	  if (m1.Col()[i] == m2.Row()[j])
		denseM[m1.Row()[i]][m2.Col()[j]] += m1.Values()[i] * m2.Values()[j];
}


void Calc::multSort(MatrixCOO& m1, MatrixCOO& m2, double** denseM)
{  
  int m2_size = m2.getMatrixSize();
  int* rowIndexM2 = new int[m2_size + 1];
  int* elInRow = new int[m2_size];

  for (int i = 0; i < m2_size; i++)
  {
	rowIndexM2[i] = 0;
	elInRow[i] = 0;
  }
  rowIndexM2[m2_size] = 0;

  for (int i = 0; i < m2.getElCount(); i++)
	elInRow[m2.Row()[i]]++;

  for (int i = 1; i < m2_size + 1; i++)
	rowIndexM2[i] = rowIndexM2[i - 1] + elInRow[i - 1];

  for (int i = 0; i < m1.getElCount(); i++)
  {
	int col = m1.Col()[i];
	int row = m1.Row()[i];
	int value = m1.Values()[i];

	int j1 = rowIndexM2[col]; int j2 = rowIndexM2[col + 1];
	int j = 0;
	for (int j = j1; j < j2; j++)
	{
	  denseM[row][m2.Col()[j]] += m1.Values()[i] * m2.Values()[j];
	}
  }
  delete[] rowIndexM2;
  delete[] elInRow;
}

