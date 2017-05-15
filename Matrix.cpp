#include "Matrix.h"

Matrix::Matrix(){}

Matrix::Matrix(int nb, int mb){
    n = nb;
	m = mb;
	matrices = new float*[n];
	for (int i=0; i<n; i++){
		matrices[i] = new float[m];;
		for (int j=0; j<m; j++){
			matrices[i][j] = 0.f;
		}
    }
}

float &Matrix::operator()(unsigned int i, unsigned int j){
	if (i>n || j>m){
		cerr << "ERREUR: Depacement indice" << endl;
	}
	return matrices[i][j];
}


FloatElement Matrix::convMtoE(Matrix mat){
    float x = mat.matrices[0][0]/mat.matrices[3][0];
    float y = mat.matrices[1][0]/mat.matrices[3][0];
    float z = mat.matrices[2][0]/mat.matrices[3][0];
    return FloatElement(x,y,z);
}

Matrix Matrix::convEtoM(FloatElement elm){
    Matrix mat(4,1);
    mat(0,0) = elm.x;
    mat(1,0) = elm.y;
    mat(2,0) = elm.z;
    mat(3,0) = 1.f;
    return mat;
}

Matrix Matrix::operator*(const Matrix &mat) const{
	Matrix tmp(n, mat.m);
	/* teste si le calcule matricielle est realisable */
	if (m == mat.n){
		for (unsigned int i=0; i<n; i++){
			for (unsigned int j=0; j<mat.m; j++){
				for (unsigned int k=0; k<m; k++){ // produit scalaire
					tmp(i,j) += matrices[i][k] * mat.matrices[k][j];
				}
			}
		}
	}else{
		cerr << "ERREUR: multiplication matricielle impossible" << endl;
	}
	return tmp;
}

Matrix Matrix::operator+(const Matrix &mat) const{
	Matrix tmp(n, m);
	for (unsigned int i=0; i<n; i++){
		for (unsigned int j=0; j<m; j++){
			tmp(i,j) = matrices[i][j] + mat.matrices[i][j];
        }
	}
	return tmp;
}

Matrix Matrix::operator-(const Matrix &mat) const{
	Matrix tmp(n, m);
	for (unsigned int i=0; i<n; i++){
		for (unsigned int j=0; j<m; j++){
			tmp(i,j) = matrices[i][j] - mat.matrices[i][j];
		}
	}
	return tmp;
}


Matrix Matrix::operator*(const double nb) const{
	Matrix tmp(n, m);
	for (unsigned int i=0; i<n; i++){
		for (unsigned int j=0; j<m; j++){
			tmp(i,j) = matrices[i][j] * nb;
        }
	}
	return tmp;
}


