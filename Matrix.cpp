#include "Matrix.h"

Matrix::Matrix(){}

Matrix::Matrix(int nb, int mb){
    n = nb;
	m = mb;
	matrices = new float*[n];
	for (unsigned int i=0; i<n; i++){
		matrices[i] = new float[m];;
		for (unsigned int j=0; j<m; j++){
			matrices[i][j] = 0.f;
		}
    }
}

 /* convertie une matrice (4,1) en un element */
FloatElement Matrix::convMtoE(Matrix mat){
    float x = mat.matrices[0][0]/mat.matrices[3][0];
    float y = mat.matrices[1][0]/mat.matrices[3][0];
    float z = mat.matrices[2][0]/mat.matrices[3][0];
    return FloatElement(x,y,z);
}

 /* convertie un element elm en une matrice (4,1) */
Matrix Matrix::convEtoM(FloatElement elm){
    Matrix mat(4,1);
    mat(0,0) = elm.x;
    mat(1,0) = elm.y;
    mat(2,0) = elm.z;
    mat(3,0) = 1.f;
    return mat;
}


/******************************************************************************/
/********************************** OPERATOR **********************************/
/******************************************************************************/

float &Matrix::operator()(unsigned int i, unsigned int j){
	if (i>n || j>m){
		cerr << "ERREUR: Depacement indice" << endl;
	}
	return matrices[i][j];
}


Matrix Matrix::operator*(const Matrix &mat) const{
	Matrix tmp(n, mat.m);
	/* teste si le calcule matricielle est realisable */
	if (m == mat.n){
		for (unsigned int i=0; i<n; i++){
			for (unsigned int j=0; j<mat.m; j++){
				for (unsigned int k=0; k<m; k++){ // produit vectoriel
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

Matrix &Matrix::operator=(const Matrix &mat){
	if (this != &mat){
		if (mat.n != n || mat.m != m){
			for (unsigned int i = 0; i<n; i++) // on détruit...
				delete[] matrices[i];
			delete[] matrices;
			n = mat.n;
			m = mat.m,
			matrices = new float*[n];
            for (unsigned int i=0; i<n; i++){
                matrices[i] = new float[m];
            }
		}

		for (unsigned int i = 0; i<n; i++)
			for (unsigned int j = 0; j<m; j++)
				matrices[i][j] = mat.matrices[i][j]; // et on copie...
	}
	return *this;
}

Matrix Matrix::operator / (const double nb) const{
	Matrix tmp = *this;
	if (nb){
		for (unsigned int i = 0; i<n; i++)
			for (unsigned int j = 0; j<n; j++)
				tmp.matrices[i][j] /= nb;
	}else{
		cerr << "ERREUR: division par 0 matricielle impossible" << endl;
	}
	return tmp;
}


/******************************************************************************/
/******************************* TRANSFORMATION *******************************/
/******************************************************************************/

/* applique une translation entre la base et le deplacement souhaité
 * retourne un vecteur homogene
 *  1 0 0 10    10      20
 *  0 1 0 0     10      10
 *  0 0 1 0  *  10   =  10
 *  0 0 0 1      1      1
 *
 */
Matrix Matrix::translation(FloatElement dep){
    Matrix mat(4,4);
    mat(3,0) = dep.x;
    mat(3,1) = dep.y;
    mat(3,2) = dep.z;
    mat(0,0) = mat(1,1) = mat(2,2) = mat(3,3) = 1;
    return mat * *this;
}

/* matrice d'itentite */
Matrix Matrix::identity(){
    Matrix mat(4,4);
    mat(0,0) = mat(1,1) = mat(2,2) = mat(3,3) = 1;
    return mat;
}

/* reedimensionne un vecteur par la valeur red dans toutes les direction
 *  2 0 0 0     x       2*X
 *  0 2 0 0     y       2*Y
 *  0 0 2 0  *  z  =    2*Z
 *  0 0 0 1     w        W
 *
 */
Matrix Matrix::resiz(float red){
    Matrix mat(4,4);
    mat(0,0) = mat(1,1) = mat(2,2) = red;
    mat(3,3) = 1;
    return mat * *this;
}

/* applique une rotation en fonction de l'angle
 *  1  0  0  0     c  0  s  0      c -s  0  0
 *  0  c -s  0     0  1  0  0      s  c  0  0
 *  0  s  c  0  *  -s 0  c  0  *   0  0  1  0
 *  0  0  0  1     0  0  0  1      0  0  0  1
 *
 */
Matrix Matrix::rotation(FloatElement angle){
    Matrix matX(4,4);
    Matrix matY(4,4);
    Matrix matZ(4,4);
    matX(0,0) = matX(3,3) = matY(1,1) = matY(3,3) = matZ(2,2) = matZ(3,3) = 1;

    matX(1,1) = matX(2,2) = cos(angle.x);
    matX(1,2) = -sin(angle.x);
    matX(2,1) = sin(angle.x);

    matY(0,0) = matY(2,2) = cos(angle.y);
    matY(0,2) = sin(angle.y);
    matY(2,0) = -sin(angle.y);

    matZ(0,0) = matZ(1,1) = cos(angle.z);
    matZ(0,1) = -sin(angle.z);
    matZ(1,0) = sin(angle.z);

    return matX * matY * matZ * *this;
}

/* applique un deplacement de l'objet */
Matrix Matrix::modelView(FloatElement angle, float zoom){
    return *this * rotation(angle) * resiz(zoom);
}

