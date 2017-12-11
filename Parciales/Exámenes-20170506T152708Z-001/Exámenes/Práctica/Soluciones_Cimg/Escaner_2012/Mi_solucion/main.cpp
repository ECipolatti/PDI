//Para compilar:
//g++ -o ejemplo ejemplo.cpp -O2 -lm -lpthread -lX11 -lfftw3 -Wall

#include <CImg.h>
#include<iostream>
#include "../PDI_functions.h"

#include <cmath>
#include <vector>
#include <algorithm>

#include <string>
using namespace std;
using namespace cimg_library;

CImg<double> apply_sobel(CImg<double>& img){
	CImg<double> Gx(3,3), Gy(3,3);
	Gx.fill(0);
	Gy.fill(0);
	
	Gx(0,2) = 1;
	Gx(1,2) = 2;
	Gx(2,2) = 1;
	Gx(0,0) = -1;
	Gx(1,0) = -2;
	Gx(2,0) = -1;
	
	Gy(2,0) = 1;
	Gy(2,1) = 2;
	Gy(2,2) = 1;
	Gy(0,0) = -1;
	Gy(0,1) = -2;
	Gy(0,2) = -1;
	
	//aproximo el modulo como suma de los valores absolutos
	CImg<double> ret = (img.get_convolve(Gx).get_abs() + img.get_convolve(Gy).get_abs());
//	ret.display("sobel");
	
	return ret;
}

CImg<double> binarizar(CImg<double>& img, double umbral){ //funcion que devuelve una imagen binarizada (0/1), segun un umbral
	int w = img.width(), h = img.height();
	CImg<double> res(w,h);
	
	for(int i = 0; i < w; ++i){
		for(int j = 0; j < h; ++j){
			if(img(i,j) < umbral)
				res(i,j) = 0;
			else
				res(i,j) = 1;
		}
	}
	
	return res;
}

struct angulo { //struct que representa un angulo theta, cuyo "peso" en la imagen es de accu
	int accu;
	double theta;
	
	const bool operator < (const angulo &a) const{
		return (accu < a.accu);
	}
	
};

double angulo_principal(CImg<double>& img){
	int w = img.width(), h = img.height();
	
	CImg<double> bordes;
	bordes = apply_sobel(img); //calculo la imagen de bordes
	
	//lo siguiente calcula la media de intensidad de los puntos (descartando aquellos de intensidad inferior a 5)
	int contador = 0;
	double media_borde = 0;
	for(int i = 0; i < w; ++i){
		for(int j = 0; j < h; ++j){
			if(bordes(i,j) > 5){ //fijo un pequeño umbral para no tener en cuenta todos los puntos
				media_borde += bordes(i,j);
				contador++;
			}
		}
	}
	media_borde /= double(contador);
	
	bordes = binarizar(bordes, 3*media_borde); //binarizo
//	bordes.display();
	
	CImg<double> trans_hough = hough(bordes); //calculo la transformada de Hough
	
	//lo siguiente calcula el maximo acumulador de la transformada
	int max_acc = 0;
	for(int i = 0; i < w; ++i){
		for(int j = 0; j < h; ++j){
			if(max_acc < trans_hough(i,j))
				max_acc = trans_hough(i,j);
		}
	}
	
	//lo siguiente calcula, para cada angulo, su peso, sumando todos los acumuladores que se pueda considerar que corresponden
	//a rectas en la imagen (se toma como criterio que su valor sea superior a la mitad del maximo acumulador)
	vector<angulo> angulos;
	for(int i = 0; i < w; ++i){
		angulo nuevo;
		nuevo.accu = 0;
		nuevo.theta = i*180/double(w-1) - 90;
		for(int j = 0; j < h; ++j){
			if(trans_hough(i,j) > max_acc*.5)
				nuevo.accu += trans_hough(i,j);
		}
		angulos.push_back(nuevo);
	}
	
	sort(angulos.begin(),angulos.end()); //ordeno el vector de angulos (segun su acumulador total)
	reverse(angulos.begin(),angulos.end()); //revierto el vector, para que los angulos de mayor acumulador queden adelante
	
	angulo ang = angulos.at(0);
	return ang.theta;
}


int main (int argc, char *argv[]) {
	
	const char* filename = cimg_option("-i",(char*)0,"Ruta y nombre de la imagen que sera cargada para corregir.\n");
	
	if (!filename){
		cout<<"Debe ingresar el nombre de la imagen a corregir."<<endl;
		exit(1);
	}
	
	CImg<double> img_(filename);
	
	CImg<double> img = (img_.get_channel(0)+img_.get_channel(1)+img_.get_channel(2))/3.; //obtengo la imagen en escala de grises

	int n = cimg_option("-n",9,"n\n");
	CImg<double> filtro_prom(n,n);
	filtro_prom.fill(1.);
	filtro_prom /= n*n;
	img.convolve(filtro_prom); //aplico un filtro de promediado para atenuar los detalles
//	img.display();
	
	double angulo_rot = angulo_principal(img); //calculo el angulo de interes
	
//	cout<<"angulo de rotacion: "<<angulo_rot<<endl;
	
	CImg<double> img_desrotada;
	//Segun el valor del angulo, debe procederse de diferentes formas
	if(abs(angulo_rot) < 45){
		img_desrotada = img_.get_rotate(angulo_rot);
	}
	else{
		if(angulo_rot<0)
			img_desrotada = img_.get_rotate(90+angulo_rot);
		else{
			img_desrotada = img_.get_rotate(-90+angulo_rot);
		}
	}
	
	(img_, img_desrotada).display(); //muestro las imagenes (original y corregida)
	
	//ahora armo el nombre de archivo de la imagen corregida
	string s(filename);
	int ind = s.find_last_of('.');
	s = s.substr(0, ind);
	s += "_corregida.bmp"; 
	
	img_desrotada.save(s.c_str()); //guardo la imagen corregida
	cout<<"Imagen corregida guardada con exito.\n";
	
	return 0;
}

