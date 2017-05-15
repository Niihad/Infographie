/*
 * Vertex.cpp
 *
 *  Created on: 12 mai 2017
 *      Author: Achyle
 */

#include "Element.h"

using namespace std;

template <class T>
Element<T>::Element(T a, T b, T c): x(a), y(b), z(c){}
template <class T>
Element<T>::Element(T a, T b): x(a), y(b), z(0){}
template class  Element<float>;
template class  Element<int>;

