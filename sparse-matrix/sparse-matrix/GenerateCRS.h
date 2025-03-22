#pragma once

#include "MatrixCRS.h"
#include <random>
#include <algorithm>

MatrixCRS generateCRS(int matrixSize, int countInRow, int seed);

class GenerateCRS {
public:
  MatrixCRS even(int matrixSize, int countInRow, int seed);
  MatrixCRS even(int matrixSize, double percent, int seed);
  MatrixCRS diagonally(int matrixSize, double percent, int seed);
};