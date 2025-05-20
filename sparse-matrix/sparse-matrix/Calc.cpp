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
  int m_size = m1.getMatrixSize();
	int* rowIndexM1 = new int[m_size + 1];
  int* rowIndexM2 = new int[m_size + 1];

  for (int i = 0; i < m_size + 1; i++)
  {
		rowIndexM1[i] = 0;
		rowIndexM2[i] = 0;
  }

	for (int i = 0; i < m1.getElCount(); i++)
	{
		rowIndexM1[m1.Row()[i] + 1]++;
	}

	for (int i = 0; i < m2.getElCount(); i++)
	{
		rowIndexM2[m2.Row()[i] + 1]++;
	}

	for (int i = 2; i < m_size + 1; i++)
	{
		rowIndexM1[i] = rowIndexM1[i] + rowIndexM1[i - 1];
		rowIndexM2[i] = rowIndexM2[i] + rowIndexM2[i - 1];
	}
	
#pragma omp parallel for
	for (int i = 0; i < m_size; i++) {
		int j1 = rowIndexM1[i]; int j2 = rowIndexM1[i + 1];
		int row = i;
		for (int j = j1; j < j2; j++) {
			int col = m1.Col()[j];
			int k1 = rowIndexM2[col]; int k2 = rowIndexM2[col + 1];
			for (int k = k1; k < k2; k++)
				denseM[row][m2.Col()[k]] += m1.Values()[j] * m2.Values()[k];
		}
	}
	delete[] rowIndexM1;
	delete[] rowIndexM2;
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
	//omp_set_num_threads(16);
	int num_threads = omp_get_max_threads();
	
	//vector<vector<int> > xb(num_threads);
	//vector<vector<int> > x(num_threads);
	vector<vector<double> > values(num_threads);
	vector<vector<int> > cols(num_threads);
	int* xbarray = new int[num_threads*m2.getMatrixSize()];
	int** xb = new int*[num_threads];
	int* xarray = new int[num_threads * m2.getMatrixSize()];
	int** x = new int* [num_threads];

	int* RowIndex = new int[m1.getMatrixSize() + 1] {0};
	RowIndex[0] = 0;
#pragma omp parallel shared(xb, x, values, cols, RowIndex) 
{

	int ip = 0; //added elements by one thread 
	int current_thread = omp_get_thread_num();
	//x[current_thread] = vector<int>(m2.getMatrixSize()); //время увеличилось относительно коммита
	//xb[current_thread] = vector<int>(m2.getMatrixSize());

	const int m1_size = m1.getMatrixSize();
	const int m2_size = m2.getMatrixSize();

	values[current_thread].reserve(8);
	cols[current_thread].reserve(8);

	values[current_thread].resize(0);
	cols[current_thread].resize(0);
	//x[i] = vector<int>(m2.getMatrixSize());
	x[current_thread] = xarray + current_thread * m2_size;
	//xb[i] = vector<int>(m2.getMatrixSize());
	xb[current_thread] = xbarray + current_thread * m2_size;

	for (int i = 0; i < m2_size; i++)
	{
		x[current_thread][i] = 0;
		xb[current_thread][i] = -1;
	}
	
	#pragma omp for schedule(static)
	for (int i = 0; i < m1_size; i++)
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

	*res = new MatrixCRS(m1.getMatrixSize(), RowIndex);
	
	vector<int> sizes;
	sizes.reserve(num_threads+1);
	sizes.push_back(0);
	for (int i = 0; i < num_threads; i++)
	{
		sizes.push_back(values[i].size());
	}

#pragma omp parallel shared(sizes, values, res, cols)
	{
		int num_thread = omp_get_thread_num();
		int count = 0;
		for (int i = sizes[num_thread]; i < sizes[num_thread+1]; i++)
		{
			(*res)->Values()[i] = values[num_thread][count];
			(*res)->Col()[i] = cols[num_thread][count];
			count++;
		}
	}

	delete[] xb;
	delete[] xbarray;
	delete[] x;
	delete[] xarray;
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

