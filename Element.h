/*
 * Vertex.h
 *
 *  Created on: 12 mai 2017
 *      Author: Achyle
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_
#include <cmath>

using namespace std;

template <class T>
class Element {
	public:
	    Element();
		Element(T x, T y, T z);
		Element(T a, T b);
		T x, y, z;

		Element<T> operator+(const Element &e) const;
        Element<T> operator-(const Element &e) const;
        Element<T> operator*(const Element &e) const;
        Element<T> operator^(const Element &e) const;
        void normaliser();

};

typedef Element<float> FloatElement;
typedef Element<int> IntElement;

#endif /* ELEMENT_H_ */
