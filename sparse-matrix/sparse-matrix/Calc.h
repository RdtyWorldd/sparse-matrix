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
};