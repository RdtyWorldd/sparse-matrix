#include "GenerateCRS.h"

using namespace std;

MatrixCRS generateCRS(int matrixSize, int countInRow)
{
  //random_device rd;
  mt19937 gen;
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

MatrixCRS GenerateCRS::even(int matrixSize, int countInRow)
{
  mt19937 gen;
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