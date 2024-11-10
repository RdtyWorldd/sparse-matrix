#pragma once
#include "MatrixCOO.h"

class Calc
{
public:
  /*
	-����������������
	-��������� �� ������
	-��������� ����������� ������
	  *����� ��������������� ������������� ������� ����� ����
  */

  MatrixCOO transposition(MatrixCOO& matrix);
  double* vectorMultSort(MatrixCOO& matrix, double* vector);
  double* vectorMult(MatrixCOO& matrix, double* vector);
  double** mult(MatrixCOO& m1, MatrixCOO& m2);
  double** multSort(MatrixCOO& m1, MatrixCOO& m2);
};