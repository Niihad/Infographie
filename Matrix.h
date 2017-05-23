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
        Matrix &operator = (const Matrix &mat);
        Matrix operator /(const double nb) const;
        FloatElement convMtoE(Matrix mat);
        Matrix convEtoM(FloatElement elm);
        Matrix translation(FloatElement deplacement);
        Matrix identity();
        Matrix resiz(float red);
        Matrix rotation(FloatElement angle);
        Matrix modelView(FloatElement angle, float zoom);
};

#endif // MATRIX_H
