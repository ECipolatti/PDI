#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FuncionesMias.h"

using namespace cv;
//using namespace pdi;

int main(int argc, char** argv) {
	Mat imagen = imread("iguazu_ruidogris.jpg",IMREAD_GRAYSCALE);
//	Mat imagen = imread("corrientes_ruidogris.jpg",IMREAD_GRAYSCALE);
//	Mat imagen = imread("corrientes_ruidogris_0.jpg",IMREAD_GRAYSCALE);
//	Mat imagen = imread("corrientes_ruidogris_70.jpg",IMREAD_GRAYSCALE);
	imshow("Original",imagen);
	
	///Aplico un filtro de alfa recortado
	filtro_mediaAlfa_recortado(imagen,2,5);
	imshow("Filtrada",imagen);
	
	///Binarizo
	threshold(imagen,imagen,100,255,THRESH_BINARY);
	imshow("Binarizada",imagen);
	
	///3 Erosiones y 1 Apertura
	Mat ee = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(imagen,imagen,MORPH_ERODE,ee);
	morphologyEx(imagen,imagen,MORPH_ERODE,ee);
	morphologyEx(imagen,imagen,MORPH_ERODE,ee);
	morphologyEx(imagen,imagen,MORPH_OPEN,ee);
	imshow("Apertura",imagen);
	
	///Detecto las Lineas
	imagen = DetectarLineas(imagen,3);
	imshow("Lineas",imagen);
	
	///Aplico Hough
	double nLineas = 50;
	Mat imagenFinal = DibujarHough(imagen,nLineas);
	imshow("Hough",imagenFinal);
	
	
	waitKey(0);
	return 0;
} 
