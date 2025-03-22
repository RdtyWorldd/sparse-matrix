#include <mkl.h>
#include "Calc.h"
#include "MatrixCOO.h"
#include "GenerateCRS.h"
#include <fstream>
#include <chrono>

void print(double** t, int n, int m)
{
  for (int i = 0; i < n; i++)
  {
	for (int j = 0; j < m; j++)
	  cout << t[i][j] << ' ';
	cout << endl;
  }
  cout << endl;
}

double** genDenseM(int matrixSize) {
  double* tmp = new double[matrixSize * matrixSize] {0.0};
  double** res = new double* [matrixSize];
  for (int i = 0; i < matrixSize; i++)
	res[i] = tmp + i * matrixSize;
  return res;
}

void printToFile(const MatrixCRS& m)
{
  ofstream file("C:\\Users/Rdty\\Desktop\\sparse-matrix\\matrixFile2.txt");
  file << m.getMatrixSize() << endl;

  for (int i = 0; i < m.getMatrixSize(); i++)
  {
	int j1 = m.RowIndex()[i], j2 = m.RowIndex()[i + 1];
	for (int j = j1; j < j2; j++)
	{
	  file << i << " " << m.Col()[j] <<endl;
	}
  }
}

bool correctMult(double** myDence, double* denceMkl, int size, double delta)
{
  for (int i = 0; i < size; i++)
  {
	for (int j = 0; j < size; j++)
	{
	  if ((denceMkl[(i * size) + j] - delta >= myDence[i][j]) ||
		(denceMkl[(i * size) + j] + delta <= myDence[i][j]))
		return false;
	}
  }
  return true;
}

bool correctMult(double* myDence, double* denceMkl, int size, double delta)
{
  for (int i = 0; i < size; i++)
  {
	for (int j = 0; j < size; j++)
	{
	  if ((denceMkl[(i * size) + j] - delta >= myDence[(i * size) + j]) ||
		(denceMkl[(i * size) + j] + delta <= myDence[(i * size) + j]))
		return false;
	}
  }
  return true;
}

int main(int argc, char* argv[])
{
  if (argc < 8)
  {
	std::cout << "exec(x)" << std::endl;
	return -1;
  }

  int seed_first = atoi(argv[1]);
  int seed_second = atoi(argv[2]);
  int matrix_size = atoi(argv[3]);
  double percent_first = atof(argv[4]);
  double percent_second = atof(argv[5]);
  int first_matrix_type = atoi(argv[6]); //0 - even, 1 - diagonaly
  int second_matrix_type = atoi(argv[7]); //0 - even, 2 - diagonaly

  double delta = 1e-8;

  GenerateCRS gen;
  MatrixCRS m1(1, 1);
  MatrixCRS m2(1, 1);
  if (first_matrix_type == 1)
	  m1 = gen.diagonally(matrix_size, percent_first, seed_first);
  else
  {
	  m1 = gen.even(matrix_size, percent_first, seed_first);
  }
	

  if (second_matrix_type == 1)
	  m2 = gen.diagonally(matrix_size, percent_second, seed_second);
  else
  {
  	m2 = gen.even(matrix_size, percent_second, seed_second);
  }

  double** dense = genDenseM(matrix_size);
  double* denseM = new double[matrix_size * matrix_size] {0.0};
  {
	Calc calc;
	const auto start = std::chrono::steady_clock::now();
	calc.mult(m1, m2, dense);
	const auto end = std::chrono::steady_clock::now();
	const std::chrono::duration<double> elapsed_seconds{ end - start };
	std::cout << elapsed_seconds.count() << std::endl;
  }

  {
	int* m1_pntre = new int[matrix_size];
	int* m2_pntre = new int[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
	  m1_pntre[i] = m1.RowIndex()[i + 1];
	  m2_pntre[i] = m2.RowIndex()[i + 1];
	}
	sparse_matrix_t mkl1 = NULL, mkl2 = NULL;

	mkl_sparse_d_create_csr(&mkl1, SPARSE_INDEX_BASE_ZERO,
	  m1.getMatrixSize() /*���������� �����*/,
	  m1.getMatrixSize() /*���������� �����*/,
	  m1.RowIndex() /*row index ��� ������*/,
	  m1_pntre /*���-�� �� � ������ �����������*/,
	  m1.Col() /*col*/,
	  m1.Values() /*values*/
	);

	mkl_sparse_d_create_csr(&mkl2, SPARSE_INDEX_BASE_ZERO,
	  m2.getMatrixSize() /*���������� �����*/,
	  m2.getMatrixSize() /*���������� �����*/,
	  m2.RowIndex() /*row index ��� ������*/,
	  m2_pntre /*���-�� �� � ������ �����������*/,
	  m2.Col() /*col*/,
	  m2.Values() /*values*/
	);

	double* dense_mkl = new double[matrix_size * matrix_size];
	int ldc = matrix_size;

	const auto start = std::chrono::steady_clock::now();
	mkl_sparse_d_spmmd(SPARSE_OPERATION_NON_TRANSPOSE, mkl1, mkl2, SPARSE_LAYOUT_ROW_MAJOR, dense_mkl, ldc);
	const auto end = std::chrono::steady_clock::now();
	const std::chrono::duration<double> elapsed_seconds{ end - start };
	std::cout << elapsed_seconds.count() << std::endl;

	std::cout << correctMult(dense, dense_mkl, matrix_size, delta);
	delete[] dense;
	delete[] dense_mkl;
  }
}

