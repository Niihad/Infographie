#include "Matrix.h"

Matrix::Matrix(){}

Matrix::Matrix(int n){
    n = n;
	matrice = new float[n];  // allocations
	for (int i=0; i<n; i++){
        matrice[i] = 0;
    }
}

Matrix::Matrix(int n, int m){
    n = n;
	m = m;
	matrice = new float[n];  // allocations
	for (int i=0; i<n; i++){
		matrices[i] = new float[m];
		for (int j = 0; j<m; j++){
			matrices[i][j] = 0;
		}
    }
}

void Matrix::homogeneous4D(Element<float> elemnt){
    matrice[0] = elemnt.x;
    matrice[1] = elemnt.y;
    matrice[2] = elemnt.z;
}


/*Matrix::Matrix(Element &elem){
    n = nbl;
	m = nbc;
	matrice = new matrice[n];  // allocations
	for (unsigned int i=0; i<n; i++){
		lignes[i] = new float[m];
		for (unsigned int j = 0; j<m; j++){
			matrice[i][j] = elem;
		}
    }
}*/
