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

  ///можно выделить сразу весь массив двумерный и потом его конвертировать в матрицу
  ///так как для параллельной версии в люббом случае выделиться та же матрица 
  /// и операции пушбек в вектора станут бессмысленны, случайность потоков
   
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

void Calc::mult_parallel_from_v(MatrixCRS& m1, MatrixCRS& m2, MatrixCRS** res)
{
	int num_threads = omp_get_max_threads();
	
	vector<vector<int> > xb;
	vector<vector<int> > x;
	vector<vector<double> > values;
	vector<vector<int> > cols;
	int* RowIndex = new int[m1.getMatrixSize() + 1] {0};

	for (int i = 0; i < num_threads; i++)
	{
		x.push_back(vector<int>());
		xb.push_back(vector<int>());
		values.push_back(vector <double>());
		cols.push_back(vector<int>());
		for (int j = 0; j < m2.getMatrixSize(); j++)
		{
			x[i].push_back(0);
			xb[i].push_back(- 1);
		}
	}


	RowIndex[0] = 0;
#pragma omp parallel shared(xb, x, values, cols, RowIndex) 
{
		int ip = 0; //added elements by one thread 
		int current_thread = omp_get_thread_num();
		#pragma omp for schedule(static)
			for (int i = 0; i < m1.getMatrixSize(); i++)
			{
				int el_row_count = 0;
				for (int jp = m1.RowIndex()[i]; jp < m1.RowIndex()[i + 1]; jp++)
				{
					int j = m1.Col()[jp];
					for (int kp = m2.RowIndex()[j]; kp < m2.RowIndex()[j + 1]; kp++)
					{
						int k = m2.Col()[kp];
						if (xb[current_thread][k] != i) // фишка которая позволяет не обнулять массив
						{
							cols[current_thread].push_back(k);
							el_row_count++;
							xb[current_thread][k] = i;
							x[current_thread][k] = m1.Values()[jp] * m2.Values()[kp];
						}
						else
						{
							x[current_thread][k] += m1.Values()[jp] * m2.Values()[kp];
						}
					}
				}
					RowIndex[i + 1] = el_row_count;
					for (int vp = ip; vp < ip + el_row_count; vp++)
					{
						int v = cols[current_thread][vp];
						values[current_thread].push_back(x[current_thread][v]);
					}
					ip = ip + el_row_count;
				
			}
	}

	for (int i = 2; i <= m1.getMatrixSize(); i++)
	{
		RowIndex[i] += RowIndex[i - 1];
	}
	int s = 0;
	//*res = new MatrixCRS(m1.getMatrixSize(), ip);
	//
	//int count = 0;
	//for(int i = 0; i < num_threads; i++)
	//{
	//	for (int j = 0; j < values[i].size(); j++)
	//	{
	//		(*res)->Values()[count] = values[i][j];
	//		(*res)->Values()[count] = cols[i][j];
	//		count++;
	//	}
	//}

	//for (int i = 0; i < m1.getMatrixSize(); i++)
	//{
	//	(*res)->RowIndex()[i] = RowIndex[i];
	//}
}

void Calc::mult_parallel_from_d(MatrixCRS& m1, MatrixCRS& m2, MatrixCRS** res)
{
	int* RowIndex = new int[m1.getMatrixSize() + 1] {0};
	double* tmp = new double[m1.getMatrixSize() * m2.getMatrixSize()] {0.0};
	double** denseM = new double*[m1.getMatrixSize()];

	int el_count = 0;

	for (int i = 0; i < m1.getMatrixSize(); i++)
	{
		denseM[i] = tmp + (i * m1.getMatrixSize());
	}
	
	mult(m1, m2, denseM);

	//std::cout << el_count << endl;
#pragma omp parallel for reduction(+: el_count)
	for (int i = 0; i < m1.getMatrixSize(); i++)
	{
		for (int j = 0; j < m1.getMatrixSize(); j++)
		{
			if (denseM[i][j] != 0.0)
				el_count++;
		}
	}

	*(res) = new MatrixCRS(m1.getMatrixSize(), el_count);

	el_count = 0;
	for (int i = 0; i < m1.getMatrixSize(); i++)
	{
		for (int j = 0; j < m1.getMatrixSize(); j++)
		{
			if (denseM[i][j] != 0.0)
			{
				(*res)->Values()[el_count] = denseM[i][j];
				(*res)->Col()[el_count] = j;
			}
		}
		(*res)->RowIndex()[i+1] = el_count;
	}
}

////дописать
//void Calc::mult(Ellpack& m1, Ellpack& m2, double** denseM)
//{
//	int el_in_row;
//	int matrixSize;
//	for (int i = 0; i < matrixSize; i++)
//	{
//		for (int j = 0; j < el_in_row; j++)
//		{
//			for (int k = 0; k < el_in_row; k++)
//			{
//				denseM[i][m2.getCol()[m1.getCol()[i][j]][k]] += m1.getValue()[i][j] * m2.getValue()[m1.getCol()[i][j]][k];
//			}
//		}
//	}
//}

