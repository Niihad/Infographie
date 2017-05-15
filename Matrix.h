#ifndef MATRIX_H
#define MATRIX_H

#include "Element.h"

class Matrix
{
    public:
        Matrix();
        Matrix(int n);
        Matrix(int n, int m);

        void homogeneous4D(Element<float> elemnt);

        unsigned int n;
        unsigned int m;
        float *matrice;
        float **matrices;
};

#endif // MATRIX_H
