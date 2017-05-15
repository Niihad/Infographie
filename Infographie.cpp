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
#include "Matrix.h"

using namespace std;

Model *model = NULL;
const int width  = 800;
const int height = 800;
FloatElement focus = FloatElement(1,-1,1).normaliser();
FloatElement eye(1,1,3);
FloatElement center(0,0,0);

/*********************************************************************************************************/

Matrix viewport(int x, int y, int w, int h) {
	Matrix view(4,4);
	view(0,0) = view(1, 1) = view(2, 2) = view(3, 3) = 1;
	view(0,3) = x+w/2.f;
	view(1,3) = y+h/2.f;
	view(2,3) = 0;
	view(0,0) = w/2.f;
	view(1,1) = h/2.f;
	view(2,2) = (w+h)/2.f;
	return view;
}

Matrix lookat(FloatElement eye, FloatElement center, FloatElement up) {
    FloatElement z = (eye-center).normaliser();
    FloatElement x = (up^z).normaliser();
    FloatElement y = (z^x).normaliser();
    Matrix res(4,4);
    res(0,0) = res(1,1) = res(2,2) = res(3,3) = 1;
    for (int i=0; i<3; i++) {
        res(0,i) = x[i];
        res(1,i) = y[i];
        res(2,i) = z[i];
        res(i,3) = -center[i];
    }
    return res;
}

Matrix projection(){
    Matrix perse(4,4);
    perse(0,0) = perse(1,1) = perse(2,2) = perse(3,3) = 1;
    perse(3,2) = -1.f/3;
    return perse;
}

/*********************************************************************************************************/

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

void contraste(TGAColor &co) {
    for(int i=2;i>=0;i--){
        co.bgra[i] = (3*pow((co.bgra[i]/256),2) - 2*pow((co.bgra[i]/256),3))*256;
    }
}

void negatif(TGAColor &co) {
    for(int i=2;i>=0;i--){
        co.bgra[i] = 255 - co.bgra[i];
    }
}

void triangle(FloatElement *e, FloatElement *e_texture, float *f_intensity, float **z_buffer, TGAImage &img, TGAImage &texture) {
    FloatElement bbmin(std::max(0.f,std::min((float)img.get_width()-1,std::min(e[0].x,std::min(e[1].x,e[2].x)))),
                     std::max(0.f,std::min((float)img.get_height()-1,std::min(e[0].y,std::min(e[1].y,e[2].y)))));
    FloatElement bbmax(std::min((float)img.get_width()-1,std::max(e[0].x,std::max(e[1].x,e[2].x))),
                     std::max((float)img.get_height()-1,std::max(e[0].y,std::max(e[1].y,e[2].y))));

    float aire_e = aire(e[0], e[1], e[2]);
    float alpha,beta,gamma;
    float txtX, txtY, ity;

    for(int i=bbmin.x; i<=bbmax.x;i++){
        for(int j=bbmin.y; j<=bbmax.y;j++){
            FloatElement tmp(i,j);
            if(barycentric(aire_e, e, tmp,alpha,beta,gamma)){
                if(z_buffer[i][j] < tmp.z){
                    z_buffer[i][j] = tmp.z;
                    /*txtX = (e_texture[0].x*alpha + e_texture[1].x*beta + e_texture[2].x*gamma)*texture.get_width();
                    txtY = (e_texture[0].y*alpha + e_texture[1].y*beta + e_texture[2].y*gamma)*texture.get_height();
                    TGAColor color = texture.get(txtX,txtY);
                    img_t.set(i, j, color);*/

                    ity = (f_intensity[0]*alpha + f_intensity[1]*beta + f_intensity[2]*gamma);
                    TGAColor color = TGAColor(255, 255, 255)*ity;
                    negatif(color);
                    img.set(i,j,color);
                }
            }
        }
    }
}

/* Technique de ombrage plat pour calculer la luminosite d une face*/
float ombragePlat(FloatElement *current){
    /* calcule des vecteurs ab et ac */
    FloatElement ab = (current[1]-current[0]);
    FloatElement ac = (current[2]-current[0]);
    /* calcule du produit vectoriel de ab^ac */
    FloatElement pv = (ac)^(ab);
    pv.normaliser();
    return (pv.x*focus.x + pv.y*focus.y + pv.z*focus.z);
}

int main(int argc, char** argv) {
    if (2==argc) {
        string str = argv[1];
        model = new Model(str);
    } else {
        model = new Model("obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    Matrix viewPort = viewport(width/8, height/8, width*3/4, height*3/4);
    Matrix modelView = lookat(eye, center, FloatElement(0,1,0));
    Matrix project = projection();

    float **z_buffer = new float*[width];
    for (int i=0; i<width; i++){
        z_buffer[i] = new float[height];
        for (int j=0; j<height; j++){
            z_buffer[i][j] = -std::numeric_limits<float>::max();
        }
    }
    Matrix tmp(4,1);

    TGAImage africanDiffuse;
    africanDiffuse.read_tga_file("obj/african_head_diffuse.tga");
    africanDiffuse.flip_vertically();
    vector<vector<IntElement> > list = model->getFaces();
    for (unsigned int i=0; i<list.size(); i++) {
        FloatElement pts[3], textures[3];
        float intensity[3];
    	vector<IntElement> face = model->getFace(i);
    	for (int j=0; j<3; j++) {
			FloatElement v0 = model->getElement(0,face[j].x);
            FloatElement v1 = model->getElement(1,face[j].y);
            FloatElement v2 = model->getElement(2,face[j].z);
            pts[j] = tmp.convMtoE(viewPort*project*modelView*tmp.convEtoM(v0));
            textures[j] = FloatElement(v1.x, v1.y, v1.z);
            intensity[j] = v2.x*focus.x + v2.y*focus.y + v2.z*focus.z;
    	}
    	triangle(pts, textures, intensity, z_buffer, image, africanDiffuse);
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");

    delete model;
    return 0;
}
