#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"

using namespace cv;
using namespace pdi;

int main(int argc, char** argv) {
//	Point 	p1(127,255), 
//			p2(127,0),      
//			p3(140,10),      
//			p4(255,0);  	   
//	
//	Mat linea(256,256,CV_32FC1,Scalar(0,0,0)); 
//	Mat circulo(256,256,CV_32FC1,Scalar(0,0,0)); 
//	Mat cuadrado(256,256,CV_32FC1,Scalar(0,0,0)); 
//	Mat rectangulo(256,256,CV_32FC1,Scalar(0,0,0)); 
//	Mat transformada(256,256,CV_32FC1,Scalar(0,0,0)); 
//	
//	//Linea Vertical
//	line(linea, p1,p2,255,2);  
//	imshow("Linea Vertical",linea);
//	transformada = spectrum(linea);
//	imshow("FFT Linea Vertical",transformada);
//	
//	//Linea Horizontal
//	linea = rotate(linea,90);  
//	imshow("Linea Horizontal",linea);
//	transformada = spectrum(linea);
//	imshow("FFT Linea Horizontal",transformada);
//	
//	//Circulo
//	p1.x = 127;
//	p1.y = 127;
//	circle(circulo,p1,50,255,CV_FILLED,2);
//	imshow("Circulo",circulo);
//	transformada = spectrum(circulo);
//	imshow("FFT circulo",transformada);
//	
//	//Cuadrado
//	p1.x = 100;
//	p1.y = 100;
//	p2.x = 140;
//	p2.y = 140;
//	rectangle(cuadrado,p1,p2,255,2);
//	imshow("Cuadrado",cuadrado);
//	transformada = spectrum(cuadrado);
//	imshow("FFT cuadrado",transformada);
//	
//	//Rectangulo
//	p1.x = 50;
//	p1.y = 100;
//	p2.x = 200;
//	p2.y = 120;
//	rectangle(rectangulo,p1,p2,255,2);
//	imshow("Rectangulo",rectangulo);
//	transformada = spectrum(rectangulo);
//	imshow("FFT rectangulo",transformada);
	
	Mat lineaV(512,512,CV_32FC1,Scalar(0,0,0));
	Point p1(256,0),
		  p2(256,511);
	line(lineaV, p1,p2,1);  
	
//	imshow("Linea Vertical",lineaV);
	Mat lineaV1 = lineaV(Rect(128,128,256,256));
	imshow("Linea Recortada Vertical",lineaV1);
	
	Mat lineaR = rotate(lineaV,20);
//	imshow("Linea Rotada",lineaR);
	Mat lineaR1 = lineaR(Rect(128,128,256,256));
	imshow("Linea Rotada Recortada",lineaR1);
	
	
	Mat	transformadaV1 = spectrum(lineaV1);
	imshow("FFT Rotada Original",transformadaV1);
	Mat	transformadaR1 = spectrum(lineaR1);
	imshow("FFT Rotada Recortada",transformadaR1);
	
	waitKey();
	return 0;
} 


