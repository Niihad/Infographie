//============================================================================
// Name        : Infographie.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//============================================================================
// Name        : Infographie.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "model.h"
#include "tgaimage.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Model *model = NULL;
const int width  = 800;
const int height = 800;


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    /* Pour calculer le deplacement de y c'est a dire t on le compare a la distance de x or
     * pour ne pas avoir un deplacement superieur a 1 il faut egalement effectuer un swap. */
    bool steep = false;
    if (abs(x0-x1)<abs(y0-y1)) {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    /* Le parcourt se fait de x0 a x1 si x0 > x1 alors la valeur est negatif
     * pour remedier a cela on effectuer un swap des 2 points */
    if(x0>x1){
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}


int main(int argc, char** argv) {
    if (2==argc) {
        string str = argv[1];
        model = new Model(str);
    } else {
        model = new Model("obj/african_head.obj");
    }
    TGAImage image(width, height, TGAImage::RGB);

    vector<vector<IntElement> > list = model->getFaces();
    cout << "Nombre de face est de " << list.size() << endl;
    for (unsigned int i=0; i<list.size(); i++) {
    	vector<IntElement> face = model->getFace(i);
    	for (int j=0; j<3; j++) {
			FloatElement v0 = model->getElement(0,face[j].x);
			FloatElement v1 = model->getElement(0,face[(j+1)%3].x);
			int x0 = (v0.x+1.)*width/2.;
			int y0 = (v0.y+1.)*height/2.;
			int x1 = (v1.x+1.)*width/2.;
			int y1 = (v1.y+1.)*height/2.;
			line(x0, y0, x1, y1, image, white);
    	}
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
