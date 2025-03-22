#include "Calc.h"

void Calc::transposition(MatrixCOO& matrix, MatrixCOO& matrixT)
{
  for (int i = 0; i < matrix.getElCount(); i++)
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
	while ((j <= size * size) && (matrix.Row()[j] == i))
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

	int j1 = rowIndexM2[col]; int j2 = rowIndexM2[col + 1];
	for (int j = j1; j < j2; j++)
	{
	  denseM[row][m2.Col()[j]] += m1.Values()[i] * m2.Values()[j];
	}
  }
  delete[] rowIndexM2;
  delete[] elInRow;
}

void Calc::transposition(MatrixCRS& matrix, MatrixCRS& matrixT)
{
  for (int i = 0; i < matrix.getElCount(); i++)
	matrixT.RowIndex()[matrix.Col()[i] + 1]++;
  for (int i = 1; i < matrix.getMatrixSize(); i++)
	matrixT.RowIndex()[i] += matrixT.RowIndex()[i - 1];
  for(int i = matrix.getMatrixSize(); i > 0; i--)
	matrixT.RowIndex()[i] = matrixT.RowIndex()[i - 1];

  for (int i = 0; i < matrix.getMatrixSize(); i++)
  {
	int j1 = matrix.RowIndex()[i]; int j2 = matrix.RowIndex()[i + 1];
	int col = i;
	for (int j = j1; j < j2; j++)
	{
	  double value = matrix.Values()[j];
	  int RIndex = matrix.Col()[j];
	  /*
		IIndex - это индекс на который нужно поставить следующий элемент
		тк rowIndex хранит индекс начала строки в массиве col,
	  */
	  int IIndex = matrixT.RowIndex()[RIndex + 1];
	  matrixT.Values()[IIndex] = value;
	  matrixT.Col()[IIndex] = col;
	  matrixT.RowIndex()[RIndex + 1]++;
	}
  }
}

void Calc::vectorMult(MatrixCRS& matrix, double* multV, double* resV)
{
  for (int i = 0; i < matrix.getMatrixSize(); i++)
  {
	int j1 = matrix.RowIndex()[i]; int j2 = matrix.RowIndex()[i + 1];
	double sum = 0.0;
	for (int j = j1; j < j2; j++)
	{
	  sum += matrix.Values()[j] * multV[matrix.Col()[j]];
	}
	resV[i] = sum;
  }
}

void Calc::mult(MatrixCRS& m1, MatrixCRS& m2, double** denseM)
{
#pragma omp parallel for 
  for (int i = 0; i < m1.getMatrixSize(); i++)
  {
	int j1 = m1.RowIndex()[i]; int j2 = m1.RowIndex()[i + 1];
	int row = i;
	for (int j = j1; j < j2; j++)
	{
	  int col = m1.Col()[j];
	  int k1 = m2.RowIndex()[col]; int k2 = m2.RowIndex()[col + 1];
	  for (int k = k1; k < k2; k++)
	  {
		denseM[row][m2.Col()[k]] += m1.Values()[j] * m2.Values()[k];
	  }
	}
  }
}

void Calc::mult(MatrixCRS& m1, MatrixCRS& m2, double* denseM)
{
#pragma omp parallel for 
  for (int i = 0; i < m1.getMatrixSize(); i++)
  {
	int j1 = m1.RowIndex()[i]; int j2 = m1.RowIndex()[i + 1];
	int row = i;
	for (int j = j1; j < j2; j++)
	{
	  int col = m1.Col()[j];
	  int k1 = m2.RowIndex()[col]; int k2 = m2.RowIndex()[col + 1];
	  for (int k = k1; k < k2; k++)
	  {
		denseM[row * m1.getMatrixSize() + m2.Col()[k]] += m1.Values()[j] * m2.Values()[k];
	  }
	}
  }
}


//
void Calc::mult(MatrixCRS& m1, MatrixCRS& m2, MatrixCRS** res)
{
  vector<double> val;
  vector<int> col;
  int* rowIndex = new int[m2.getMatrixSize() + 1];
  rowIndex[0] = 0;

  double* iteration_res = new double[m2.getMatrixSize()] {0.0};
  
  for (int i = 0; i < m1.getMatrixSize(); i++)
  {
	int j1 = m1.RowIndex()[i]; int j2 = m1.RowIndex()[i + 1];
	int row = i;
	for (int j = j1; j < j2; j++)
	{
	  int col = m1.Col()[j];
	  int k1 = m2.RowIndex()[col]; int k2 = m2.RowIndex()[col + 1];
	  for (int k = k1; k < k2; k++)
	  {
		iteration_res[m2.Col()[k]] += m1.Values()[j] * m2.Values()[k];
	  }
	}

	for (int j = 0; j < m2.getMatrixSize(); j++)
	{
	  if (iteration_res[j] != 0.0)
	  {
		val.push_back(iteration_res[j]);
		col.push_back(j);
	  }
	}
	rowIndex[i + 1] = col.size();
	//memset(iteration_res, 0.0, m2.getMatrixSize());
	for (int j = 0; j < m2.getMatrixSize(); j++)
	  iteration_res[j] = 0.0;
  }

  (*res) = new MatrixCRS(m2.getMatrixSize(), val.size());
  for (int i = 0; i < rowIndex[m2.getMatrixSize()]; i++)
  {
	(*res)->Values()[i] = val[i];
	(*res)->Col()[i] = col[i];
  }

  for (int i = 0; i < m2.getMatrixSize() + 1; i++)
	(*res)->RowIndex()[i] = rowIndex[i];
 
  delete[] rowIndex;
  delete[] iteration_res;
}
