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
#include <limits>
#include "model.h"
#include "tgaimage.h"


using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

/* on calcule a partir des sommet du triangle l'air */
float aire(FloatElement a, FloatElement b, FloatElement c) {
    return 0.5 * abs((b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y));
}

bool barycentric(float aire_e, FloatElement *e, FloatElement &p, float &alpha, float &beta, float &gamma){
    //On calcule les coordonnées barycentriques α, β et γ du point p
    alpha = aire(p, e[1], e[2]) / aire_e;
    beta = aire(e[0], p, e[2]) / aire_e;
    gamma = aire(e[0], e[1], p) / aire_e;
    float res = alpha + beta + gamma;

    p.z = (alpha*e[0].z + beta*e[1].z + gamma*e[2].z) / res;
    return ((fabs(res - 1) < 0.01)) ? true : false;
}

void triangle(FloatElement *e, FloatElement *e_text, float **z_buffer, TGAImage &image, TGAImage &texture) {
    FloatElement bbmin(std::max(0.f,std::min((float)image.get_width()-1,std::min(e[0].x,std::min(e[1].x,e[2].x)))),
                     std::max(0.f,std::min((float)image.get_height()-1,std::min(e[0].y,std::min(e[1].y,e[2].y)))));
    FloatElement bbmax(std::min((float)image.get_width()-1,std::max(e[0].x,std::max(e[1].x,e[2].x))),
                     std::max((float)image.get_height()-1,std::max(e[0].y,std::max(e[1].y,e[2].y))));

    float aire_e = aire(e[0], e[1], e[2]);
    float alpha,beta,gamma;
    float txtX, txtY;
    for(int i=bbmin.x; i<=bbmax.x;i++){
        for(int j=bbmin.y; j<=bbmax.y;j++){
            FloatElement tmp(i,j);
            if(barycentric(aire_e, e, tmp,alpha,beta,gamma)){
                if(z_buffer[i][j] < tmp.z){
                    z_buffer[i][j] = tmp.z;
                    txtX = (e_text[0].x*alpha + e_text[1].x*beta + e_text[2].x*gamma)*1024;
                    txtY = (e_text[0].y*alpha + e_text[1].y*beta + e_text[2].y*gamma)*1024;
                    TGAColor color = texture.get(txtX,txtY);
                    image.set(i, j, color);
                }
            }
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
    FloatElement focus(0,0,-1);
    float **z_buffer = new float*[width];
    for (int i=0; i<width; i++)
        z_buffer[i] = new float[height];
    for (int i=0; i<width; i++){
        for (int j=0; j<height; j++){
            z_buffer[i][j] = -std::numeric_limits<float>::max();
        }
    }

    TGAImage africanDiffuse;
    africanDiffuse.read_tga_file("obj/african_head_diffuse.tga");
    africanDiffuse.flip_vertically();
    vector<vector<IntElement> > list = model->getFaces();
    cout << "Nombre de face est de " << list.size() << endl;
    for (unsigned int i=0; i<list.size(); i++) {
        FloatElement pts[3], normls[3], textures[3];
    	vector<IntElement> face = model->getFace(i);
    	for (int j=0; j<3; j++) {
			FloatElement v0 = model->getElement(0,face[j].x);
            pts[j] = FloatElement((v0.x+1.)*width/2., (v0.y+1.)*height/2, v0.z);
            FloatElement v1 = model->getElement(1,face[j].y);
            textures[j] = FloatElement(v1.x, v1.y, v1.z);
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
            triangle(pts, textures, z_buffer, image, africanDiffuse);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("buffer.tga");
    delete model;
    return 0;
}
