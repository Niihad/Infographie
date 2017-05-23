/*
 * Model.h
 *
 *  Created on: 12 mai 2017
 *      Author: Achyle
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Element.h"
#include "tgaimage.h"

using namespace std;

class Model {
	public:
	    Model();
		Model(string filename);
		void buildStructFile(string filename);
		vector<vector<FloatElement> > getElements();
		vector<vector<IntElement> > getFaces();
		FloatElement getElement(int i, int j);
		vector<IntElement> getFace(int i);
		TGAImage* getObjetPrint();


		void choose_print(string diffuse, string nm, string spec);

	private:
		vector<vector<IntElement> > faces;
		vector<vector<FloatElement> > elements;
		TGAImage objetPrint[3];
};

#endif /* MODEL_H_ */
