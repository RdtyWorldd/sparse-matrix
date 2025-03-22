#include "GenerateCRS.h"

using namespace std;

MatrixCRS generateCRS(int matrixSize, int countInRow, int seed)
{
  //random_device rd;
  mt19937 gen(seed);
  uniform_int_distribution<> idis(0, matrixSize - 1);
  uniform_real_distribution<> rdis(0, 10);
  
  int elCount = matrixSize * countInRow;
  MatrixCRS m(matrixSize, elCount);
 
  //заполнение значения столбцов
  for (int i = 0; i < matrixSize; i++)
  {
	for (int j = 0; j < countInRow; j++)
	{
	  int f = 0;
	  do
	  {
		m.Col()[i * countInRow + j] = idis(gen);
		f = 0;
		// проверка на повтор
		for (int k = 0; k < j; k++)
		{
		  if (m.Col()[i * countInRow + j] == m.Col()[i * countInRow + k])
			f = 1;
		}
	  } while (f == 1);
	}

	//сортировка массива столбцов в строке
	sort(m.Col() + (countInRow * i), m.Col() + (countInRow * (i + 1)) );
  }
  //заполнение значений 
  for (int i = 0; i < elCount; i++)
	m.Values()[i] = rdis(gen);
  //заполнение индексов
  for (int i = 0; i <= matrixSize; i++)
	m.RowIndex()[i] = i * countInRow;

  return m;
}

MatrixCRS GenerateCRS::even(int matrixSize, int countInRow, int seed)
{
  mt19937 gen(seed);
  uniform_int_distribution<> idis(0, matrixSize - 1);
  uniform_real_distribution<> rdis(0, 10);

  int elCount = matrixSize * countInRow;
  MatrixCRS m(matrixSize, elCount);

  //заполнение значения столбцов
  for (int i = 0; i < matrixSize; i++)
  {
	for (int j = 0; j < countInRow; j++)
	{
	  int f = 0;
	  do
	  {
		m.Col()[i * countInRow + j] = idis(gen);
		f = 0;
		// проверка на повтор
		for (int k = 0; k < j; k++)
		{
		  if (m.Col()[i * countInRow + j] == m.Col()[i * countInRow + k])
			f = 1;
		}
	  } while (f == 1);
	}

	//сортировка массива столбцов в строке
	sort(m.Col() + (countInRow * i), m.Col() + (countInRow * (i + 1)));
  }
  //заполнение значений 
  for (int i = 0; i < elCount; i++)
	m.Values()[i] = rdis(gen);
  //заполнение индексов
  for (int i = 0; i <= matrixSize; i++)
	m.RowIndex()[i] = i * countInRow;

  return m;
}

MatrixCRS GenerateCRS::even(int matrixSize, double percent, int seed)
{
  mt19937 gen(seed);
  uniform_int_distribution<> idis(0, matrixSize - 1);
  uniform_real_distribution<> rdis(0, 10);

  int countInRow = (int)((double)(matrixSize) * percent);
  int elCount = matrixSize * countInRow;
  MatrixCRS m(matrixSize, elCount);

  //заполнение значения столбцов
  for (int i = 0; i < matrixSize; i++)
  {
	for (int j = 0; j < countInRow; j++)
	{
	  int f = 0;
	  do
	  {
		m.Col()[i * countInRow + j] = idis(gen);
		f = 0;
		// проверка на повтор
		for (int k = 0; k < j; k++)
		{
		  if (m.Col()[i * countInRow + j] == m.Col()[i * countInRow + k])
			f = 1;
		}
	  } while (f == 1);
	}

	//сортировка массива столбцов в строке
	sort(m.Col() + (countInRow * i), m.Col() + (countInRow * (i + 1)));
  }
  //заполнение значений 
  for (int i = 0; i < elCount; i++)
	m.Values()[i] = rdis(gen);
  //заполнение индексов
  for (int i = 0; i <= matrixSize; i++)
	m.RowIndex()[i] = i * countInRow;

  return m;
}

MatrixCRS GenerateCRS::diagonally(int matrixSize, double percent, int seed)
{
  int x = (int) (1.0 / (2*percent)); 
  int el_count = (matrixSize / 2) * (1 + (matrixSize / x));
  MatrixCRS m(matrixSize, el_count);

  //random_device rd;
  mt19937 gen(seed);
  uniform_int_distribution<> idis(0, matrixSize - 1);
  uniform_real_distribution<> rdis(0, 10);

  int el_in_row = 1;
  int row = 0;
  int count = 0;
  int ins = 0;
  m.RowIndex()[0] = 0;
  for (row = 0; row < matrixSize;) //что-то делать с этим циклом
  {
	for (int i = 0;(row < matrixSize) && i < x; i++)
	{
	  ins = 0;
	  for (int j = el_in_row - 1; (count < el_count) && (j >= 0); j--)
	  {
		m.Values()[count] = rdis(gen);
		m.Col()[count] = row - j;
		ins++;
		count++;
	  }
	  row++;
	  m.RowIndex()[row] = m.RowIndex()[row - 1] + ins;
	}
	el_in_row++;
  }
  m.RowIndex()[row] = m.RowIndex()[row - 1] + ins;
  
  return m;
}
