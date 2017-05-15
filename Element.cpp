/*
 * Vertex.cpp
 *
 *  Created on: 12 mai 2017
 *      Author: Achyle
 */

#include "Element.h"

using namespace std;

template <class T>
Element<T>::Element(){}
template <class T>
Element<T>::Element(T a, T b, T c): x(a), y(b), z(c){}
template <class T>
Element<T>::Element(T a, T b): x(a), y(b), z(0){}

template class  Element<float>;
template class  Element<int>;

template <class T>
Element<T> Element<T>::operator + (const Element<T> &e) const{
    return Element<T>(x+e.x, y+e.y, z+e.z);
}
template <class T>
Element<T> Element<T>::operator - (const Element<T> &e) const{
    return Element<T>(x-e.x, y-e.y, z-e.z);
}
template <class T>
Element<T> Element<T>::operator * (const Element<T> &e) const{
    return Element<T>(x*e.x, y*e.y, z*e.z);
}
template <class T>
Element<T> Element<T>::operator ^ (const Element<T> &e) const{
    return Element<T>(y*e.z-z*e.y, z*e.x-x*e.z, x*e.y-y*e.x);
}

template <class T>
T Element<T>::operator[](int i) const{
    if (i<0 || i>3){
		cerr << "ERREUR: Depacement indice" << endl;
	}
    switch(i){
        case 0: return x; break;
        case 1: return y; break;
        case 2: return z; break;
    }
    return 0;
}

template <class T>
Element<T> Element<T>::normaliser(){
    Element res;
	double tmp = sqrt(x*x + y*y + z*z);
	x /= tmp;
	y /= tmp;
	z /= tmp;
	return *this;
}


