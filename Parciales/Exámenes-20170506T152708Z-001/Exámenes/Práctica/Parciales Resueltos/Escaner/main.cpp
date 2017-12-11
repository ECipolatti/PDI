#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FuncionesMias.h"
#include <vector>
#include <opencv2/core/types_c.h>

using namespace cv;





//Recorro horizontalmente
Point2d PrimerPunto(Mat imagen){
	Point2d p;
	for(int j=0; j< (int)imagen.cols/2; j++){
		for(int i=0; i< imagen.rows; i++){
			int r = (int)imagen.at<uchar>(i,j);
			if(r != 0){
				p.x = j;
				p.y = i;
				return p;
			}
		}
	}
}

Point2d SegundoPunto(Mat imagen){
	Point2d p;
	for(int j= (int) imagen.cols-1; j>(int) imagen.cols/2; j--){
		for(int i=0; i< imagen.rows; i++){
			int r = (int)imagen.at<uchar>(i,j);
			if(r != 0){
				p.x = j;
				p.y = i;
				return p;
			}
		}
	}
}
	
float ObtenerAngulo(Mat imagen){
	float angulo = 0;
	Point2d p1,p2,vertice;
	
	p1 = PrimerPunto(imagen);
	p2 = SegundoPunto(imagen);
	
	//Si el angulo de rotacion es negativo
	if(p1.y < p2.y){
		Point2d aux;
		aux = p1;
		p1 = p2;
		p2 = aux;
	}
	
	vertice.x = p1.x;
	vertice.y = p2.y;
	
	float CO = abs(p1.y-vertice.y);
	float CA = abs(p2.x-vertice.x);
	
	//Para ver el angulo de rotacion
	Mat img;
	img = imagen.clone();
	line(img,p1,p2,255,4);
	line(img,p2,vertice,255,4);
	line(img,p1,vertice,255,4);
	namedWindow("triangulo",0);
	imshow("triangulo",img);
	
	 angulo = atan(CO/CA) * 180 / CV_PI;
	return angulo;
	
}

///NO ESTA ANDANDO DEL TODO BIEN, PERO EL ANGNGULO SI ESTA BIEN,
///TENGO QUE ACOMODAR LAS LINEAS DE HOUGH, TRATAR DE QUE QUEDE SOLO UNA

int main(int argc, char** argv) {
	Mat imagen = imread("escaneo3.jpg",IMREAD_GRAYSCALE);
	Mat Img = imagen.clone();
	
	namedWindow("Imagen",0);
	imshow("Imagen",imagen);
	
	///Binarizo la imagen
	threshold(imagen,imagen,200,255,THRESH_BINARY);
//	namedWindow("Binarizada",0);
//	imshow("Binarizada",imagen);

	
	///Detecto las Lineas en todas las direcciones
	Mat imagen0 = DetectarLineas(imagen,0);
	Mat imagen1 = DetectarLineas(imagen,1);
	Mat imagen2 = DetectarLineas(imagen,2);
	Mat imagen3 = DetectarLineas(imagen,3);
	imagen = imagen0+imagen1+imagen2+imagen3;
//	namedWindow("Lineas",0);
//	imshow("Lineas",imagen);
		
		
	///Aplico Hough
	double nLineas = 340;
	Mat imagenHough = DibujarHough(imagen,nLineas);
	namedWindow("Hough",0);
	imshow("Hough",imagenHough);


	///Obtengo el angulo de rotacion
	float angulo = ObtenerAngulo(imagenHough);
	cout<< angulo;
	
	
	Img = rotate(Img,angulo);
	namedWindow("Rotada",0);
	imshow("Rotada",Img);

	waitKey(0);
	return 0;
} 
