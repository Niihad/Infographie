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

const int width  = 1000;
const int height = 1000;
TGAImage image(width, height, TGAImage::RGB);
FloatElement focus = FloatElement(1,1,1).normaliser();
FloatElement eye(0.5f,0.5f,3);
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

/* applique un effet de contracte aux rendu  */
void contraste(TGAColor &co) {
    for(int i=2;i>=0;i--){
        co.bgra[i] = (3*pow((co.bgra[i]/256),2) - 2*pow((co.bgra[i]/256),3))*256;
    }
}

/* applique un effet negatif aux rendu */
void negatif(TGAColor &co) {
    for(int i=2;i>=0;i--){
        co.bgra[i] = 255 - co.bgra[i];
    }
}

void triangle(FloatElement *e, FloatElement *e_texture, float **z_buffer, TGAImage &img, TGAImage *texture) {
    FloatElement bbmin(std::max(0.f,std::min((float)img.get_width()-1,std::min(e[0].x,std::min(e[1].x,e[2].x)))),
                     std::max(0.f,std::min((float)img.get_height()-1,std::min(e[0].y,std::min(e[1].y,e[2].y)))));
    FloatElement bbmax(std::min((float)img.get_width()-1,std::max(e[0].x,std::max(e[1].x,e[2].x))),
                     std::max((float)img.get_height()-1,std::max(e[0].y,std::max(e[1].y,e[2].y))));

    float aire_e = aire(e[0], e[1], e[2]);
    float alpha,beta,gamma;
    float txtX, txtY;
    for(int i=bbmin.x; i<=bbmax.x;i++){
        for(int j=bbmin.y; j<=bbmax.y;j++){
            FloatElement tmp(i,j);
            if(barycentric(aire_e, e, tmp,alpha,beta,gamma)){
                if(z_buffer[i][j] < tmp.z){
                    z_buffer[i][j] = tmp.z;

                    txtX = (e_texture[0].x*alpha + e_texture[1].x*beta + e_texture[2].x*gamma)*texture[0].get_width();
                    txtY = (e_texture[0].y*alpha + e_texture[1].y*beta + e_texture[2].y*gamma)*texture[0].get_height();
                    TGAColor color = texture[0].get(txtX,txtY);
                    TGAColor colorNm = texture[1].get(txtX,txtY);
                    FloatElement nM = FloatElement(colorNm.bgra[2]/255.f*2.f - 1.f, colorNm.bgra[1]/255.f*2.f - 1.f, colorNm.bgra[0]/255.f*2.f - 1.f).normaliser();

                    float psNm = 2 * nM.produitScalaire(focus);
                    FloatElement r = FloatElement((nM.x*psNm)-focus.x, (nM.y*psNm)-focus.y, (nM.z*psNm)-focus.z).normaliser();
                    TGAColor colorSpec = texture[2].get(txtX,txtY);
                    float spec = pow(max(r.z, 0.f), colorSpec.bgra[0]/1.f);
                    float intensity = max(0.f, nM.x*focus.x + nM.y*focus.y + nM.z*focus.z);

                    for(int k=0; k<4; k++)
                        color[k] = min(5 + color[k]*(intensity + .6f*spec),255.f);
                    img.set(i, j, color);
                }
            }
        }
    }
}

/* Technique de ombrage plat pour calculer la luminosite d une face */
float ombragePlat(FloatElement *current){
    /* calcule des vecteurs ab et ac */
    FloatElement ab = (current[1]-current[0]);
    FloatElement ac = (current[2]-current[0]);
    /* calcule du produit vectoriel de ab^ac */
    FloatElement pv = (ac)^(ab);
    pv.normaliser();
    return (pv.x*focus.x + pv.y*focus.y + pv.z*focus.z);
}

void print_texture(Model *model, Matrix vPm, float **z_buffer){
    Matrix tmp(4,1);
    TGAImage *obj = model->getObjetPrint();
    vector<vector<IntElement> > list = model->getFaces();
    for (unsigned int i=0; i<list.size(); i++) {
        FloatElement pts[3], textures[3], normls[3];
    	vector<IntElement> face = model->getFace(i);
    	for (int j=0; j<3; j++) {
			FloatElement v0 = model->getElement(0,face[j].x);
            FloatElement v1 = model->getElement(1,face[j].y);
            pts[j] = tmp.convMtoE(vPm*tmp.convEtoM(v0));
            textures[j] = FloatElement(v1.x, v1.y, v1.z);
            normls[j] = v0;
    	}
        triangle(pts, textures, z_buffer, image, obj);
    }
}

int main(int argc, char** argv) {

    Model model[2];
    model[0] = Model("obj/african/african_head.obj");
    model[0].choose_print("obj/african/african_head_diffuse.tga", "obj/african/african_head_nm.tga", "obj/african/african_head_spec.tga");
    model[1] = Model("obj/african/african_head_eye_inner.obj");
    model[1].choose_print("obj/african/african_head_eye_inner_diffuse.tga", "obj/african/african_head_eye_inner_nm.tga", "obj/african/african_head_eye_inner_spec.tga");
    //model[2] = Model("obj/african/african_head_eye_outer.obj");
    //model[2].choose_print("obj/african/african_head_eye_outer_diffuse.tga", "obj/african/african_head_eye_outer_nm.tga", "obj/african/african_head_eye_outer_spec.tga");

    Matrix viewPort = viewport(width/8, height/8, width*3/4, height*3/4);
    Matrix modelView = lookat(eye, center, FloatElement(0,1,0));
    Matrix project = projection();
    Matrix vPm = viewPort*project*modelView;
    float **z_buffer = new float*[width];
    for (int i=0; i<width; i++){
        z_buffer[i] = new float[height];
        for (int j=0; j<height; j++){
            z_buffer[i][j] = -std::numeric_limits<float>::max();
        }
    }

    for (int k=0; k<2; k++)
        print_texture(&model[k], vPm, z_buffer);

    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}
