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

using namespace std;

class Model {
	public:
		Model(string filename);
		void buildStructFile(string filename);
		void buildElement(vector<FloatElement> &element, istringstream *iss);
		void buildFace(vector<int> &element, string buildline);
		vector<vector<FloatElement> > getElements();
		vector<vector<IntElement> > getFaces();
		FloatElement getElement(int i, int j);
		vector<IntElement> getFace(int i);
	private:
		vector<vector<IntElement> > faces;
		vector<vector<FloatElement> > elements;
};

#endif /* MODEL_H_ */
