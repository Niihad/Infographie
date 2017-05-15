#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include "Element.h"

class Matrix{
    public:
        Matrix();
        Matrix(int n, int m);

        Matrix identity(int n);

        unsigned int n;
        unsigned int m;
        float **matrices;

        float &operator()(unsigned int i, unsigned int j);
        Matrix operator * (const Matrix &mat) const;
        Matrix operator + (const Matrix &mat) const;
        Matrix operator - (const Matrix &mat) const;
        Matrix operator * (const double nb) const;
        FloatElement convMtoE(Matrix mat);
        Matrix convEtoM(FloatElement elm);
};

#endif // MATRIX_H
