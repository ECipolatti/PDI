#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FuncionesMias.h"
#include <vector>
#include <opencv2/core/types_c.h>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

int LineaLateralSuperior(Mat img){
	for(int j=0; j<img.cols;j++){
		for(int i = img.rows-1; i>0 ; i--){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				return i;
			}
		}
	}
}


int ObtenerMedio(Mat img){
	for(int i = img.rows-1;i>0; i--){
		for(int j=0; j<img.cols;j++){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				return j;
			}
		}
	}
	
}



void InsertarPublicidad(Mat &publicidad,Mat cartel,Point2d pos){
	
	for(int i = 0 ; i<cartel.rows; i++){
		for(int j=0; j<cartel.cols*3;j++){
			int intensidad = (int) cartel.at<uchar>(i,j);
			publicidad.at<uchar>(i+pos.y,j+(pos.x)*3) = (int) intensidad;
			}
		}

	}
	
	

int main(int argc, char** argv) {
	Mat cancha = imread("Fut02_3.png",IMREAD_COLOR); //A esta le inserto los carteles
	Mat imagen = imread("Fut02_3.png",IMREAD_GRAYSCALE);
	imshow("Imagen", cancha);
	
	///Binarizo para remarcar las lineas blancas
	threshold(imagen,imagen,200,255,CV_THRESH_BINARY);
//	imshow("Cancha",imagen);
	

	///Aplico Hough para obtener la linea lateral y medio
	double nLineas = 130; //160
	Mat imagenHough = DibujarHough(imagen,nLineas);
//	imshow("Hough",imagenHough);
	
	///Punto medio superior
	int i = LineaLateralSuperior(imagenHough);
	int j = ObtenerMedio(imagenHough);
	imshow("Hough",imagenHough);


	///Obtengos las posiciones correspondientes a cada cartel
	Mat fich = imread("fich.jpg",IMREAD_COLOR);
	Mat unl = imread("unl.jpg",IMREAD_COLOR);
	Mat sinc = imread("Logo03.png",IMREAD_COLOR);
	
	
	Point2d posFich;
	posFich.x = j-(int) fich.cols-5;//el 5 es por la linea vertical de la cancha
	posFich.y = i+2;
//	circle(cancha,posFich,5,255,2);

	Point2d posUNL;
	posUNL.x = j+(int) fich.cols-(int) unl.cols;
	posUNL.y = i+2;
//	circle(cancha,posUNL,5,255,2);
	
	Point2d posSinc;
	posSinc.x = j-(int) sinc.cols/2;
	posSinc.y = cancha.rows-sinc.rows-1;
//	circle(cancha,posSinc,5,255,2);
//	imshow("Circulos",cancha);
	
	
	///Inserto los carteles
	InsertarPublicidad(cancha,fich,posFich);
	InsertarPublicidad(cancha,unl,posUNL);
	InsertarPublicidad(cancha,sinc,posSinc);
	imshow("Publicidad",cancha);
	
	
	
	


	
	waitKey(0);
	return 0;
} 
