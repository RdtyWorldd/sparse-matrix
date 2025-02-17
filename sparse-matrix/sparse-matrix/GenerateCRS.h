#pragma once

#include "MatrixCRS.h"
#include <random>
#include <algorithm>

MatrixCRS generateCRS(int matrixSize, int countInRow);

class GenerateCRS {
public:
  MatrixCRS even(int matrixSize, int countInRow);
};