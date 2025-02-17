#pragma once
#include "MatrixCOO.h"
#include "MatrixCRS.h"
#include <vector>

class Calc
{
public:
  /*
	-транспонирование
	-умножение на вектор
	-умножение разреженных матриц
	  *нужны отсортированные представления матрицы иначе беда
  */

  //MatrixCOO
  void transposition(MatrixCOO& matrix, MatrixCOO& matrixT);
  void vectorMultSort(MatrixCOO& matrix, double* multV, double* resV);
  void vectorMult(MatrixCOO& matrix, double* multV, double* resV);
  void mult(MatrixCOO& m1, MatrixCOO& m2, double** denseM);
  void multSort(MatrixCOO& m1, MatrixCOO& m2, double**denseM);

  //MatrixCRS
  void transposition(MatrixCRS& matrix, MatrixCRS& matrixT);
  void vectorMult(MatrixCRS& matrix, double* multV, double* resV);
  void mult(MatrixCRS& m1, MatrixCRS& m2, double** denseM);
  void mult(MatrixCRS& m1, MatrixCRS& m2,MatrixCRS** res);
};