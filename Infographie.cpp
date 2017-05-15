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

/* on calcule a partir des sommet du triangle l'air de point pt */
float aire(IntElement a, IntElement b, IntElement c) {
    return 0.5 * abs((b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y));
}

bool barycentric(float aire_e, IntElement *e, IntElement p){
    //On calcule les coordonnées barycentriques α, β et γ du point p
    float alpha = aire(p, e[1], e[2]) / aire_e;
    float beta = aire(e[0], p, e[2]) / aire_e;
    float gamma = aire(e[0], e[1], p) / aire_e;
    float res = alpha + beta + gamma;
    return ((fabs(res - 1) < 0.01)) ? true : false;
}

void triangle(IntElement *e, TGAImage &image, TGAColor color) {
    IntElement bbmin(std::max(0,std::min(image.get_width()-1,std::min(e[0].x,std::min(e[1].x,e[2].x)))),
                     std::max(0,std::min(image.get_height()-1,std::min(e[0].y,std::min(e[1].y,e[2].y)))));
    IntElement bbmax(std::min(image.get_width()-1,std::max(e[0].x,std::max(e[1].x,e[2].x))),
                     std::max(image.get_height()-1,std::max(e[0].y,std::max(e[1].y,e[2].y))));


    float aire_e = aire(e[0], e[1], e[2]);
    for(int i=bbmin.x; i<=bbmax.x;i++){
        for(int j=bbmin.y; j<=bbmax.y;j++){
            if(barycentric(aire_e, e, IntElement(i,j))){
                image.set(i, j, color);
            }
        }
    }
}

void lesson1(TGAImage &image);
void lesson2(TGAImage &image);

int main(int argc, char** argv) {
    if (2==argc) {
        string str = argv[1];
        model = new Model(str);
    } else {
        model = new Model("obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    FloatElement focus(0,0,-1);

    vector<vector<IntElement> > list = model->getFaces();
    cout << "Nombre de face est de " << list.size() << endl;
    for (unsigned int i=0; i<list.size(); i++) {
        IntElement pts[3] = {IntElement(0,0), IntElement(0, 0), IntElement(0, 0)};
        FloatElement normls[3] = {FloatElement(0,0), FloatElement(0, 0), FloatElement(0, 0)};
    	vector<IntElement> face = model->getFace(i);
    	for (int j=0; j<3; j++) {
			FloatElement v0 = model->getElement(0,face[j].x);
            pts[j] = IntElement((v0.x+1.)*width/2., (v0.y+1.)*height/2);
            normls[j] = v0;
    	}
    	/* calcule des vecteurs ab et ac */
    	FloatElement ab = (normls[1]-normls[0]);
    	FloatElement ac = (normls[2]-normls[0]);
    	/* calcule du produit vectoriel de ab^ac */
    	FloatElement normal = (ac)^(ab);
    	normal.normaliser();

    	float intensity = normal.x*focus.x + normal.y*focus.y + normal.z*focus.z;
        if(intensity>0){
            triangle(pts, image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("buffer.tga");
    delete model;
    return 0;
}
