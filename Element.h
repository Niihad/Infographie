/*
 * Vertex.h
 *
 *  Created on: 12 mai 2017
 *      Author: Achyle
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

using namespace std;

template <class T>
class Element {
	public:
		Element(T x, T y, T z);
		Element(T a, T b);
		T x, y, z;

};

typedef Element<float> FloatElement;
typedef Element<int> IntElement;

#endif /* ELEMENT_H_ */
