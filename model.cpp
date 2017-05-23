/*
 * Model.cpp
 *
 *  Created on: 12 mai 2017
 *      Author: Achyle
 */

#include "model.h"

Model::Model(string filename) {
	buildStructFile(filename);
}

void Model::buildStructFile(string filename){
    ifstream file(filename.c_str(), ifstream::in);
    if(file){
        string ligne;
        istringstream iss;
        vector<FloatElement > verteces;
        vector<FloatElement > textures;
        vector<FloatElement > normals;
        float x,y,z;
        while (getline(file, ligne, '\n')){
            char space;
            istringstream iss(ligne.c_str());
			/* la ligne correspond a un sommet */
            if(ligne[0]=='v' && ligne[1]==' '){
                iss >> space;
                iss >> x >> y >> z;
                FloatElement vertex(x,y,z);
                verteces.push_back(vertex);
            }else if(ligne[0] == 'v' && ligne[1] == 't'){
            	iss >> space >> space;
                iss >> x >> y;
                FloatElement texture(x,y);
                textures.push_back(texture);
            /* la ligne correspond a une normale */
            }else if(ligne[0] == 'v' && ligne[1] == 'n'){
            	iss >> space >> space;
                iss >> x >> y >> z;
                FloatElement normal(x,y,z);
                normals.push_back(normal);
			/* la ligne correspond a une face */
            }else if(ligne[0] == 'f' && ligne[1]==' '){
                iss >> space;
                vector<IntElement> face;
                while (iss >> x >> space >> y >> space >> z) {
                    IntElement elem(x-1,y-1,z-1);
                    face.push_back(elem);
                }
                faces.push_back(face);
            }
        }
        elements.push_back(verteces);
        elements.push_back(textures);
        elements.push_back(normals);
    }else{
        cerr << "ERREUR : Lecture impossible du fichier !" << endl;
    }
}

vector<vector<FloatElement> > Model::getElements(){
    return elements;
}

vector<vector<IntElement> > Model::getFaces(){
	return faces;
}

FloatElement Model::getElement(int i, int j){
    return elements[i][j];
}

vector<IntElement> Model::getFace(int i){
    return faces[i];
}


