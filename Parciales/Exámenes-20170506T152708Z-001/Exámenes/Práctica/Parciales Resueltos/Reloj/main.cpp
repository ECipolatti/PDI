#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "FuncionesMias.h"

using namespace cv;
using namespace std;
using namespace pdi;

int CantArena(Mat img){
	int c = 0;
	for(int i=0;i<img.rows; i++){
		for(int j=0;j<img.cols; j++){
			int v = (int)img.at<uchar>(i,j);
			if ( v != 0){
				c += 1;
			}
		}
	}
	return c;
}

int main(int argc, char** argv) {
	Mat img = imread("reloj1.jpg");
	info(img);
	
	/** ***************************************** **/
	/** recorto la parte de abajo del reloj       **/
	/** ***************************************** **/
	Mat imgRec = img(Rect(80,310,145,195));

	/** ********************************************** **/
	/**  Segmento solo la arena. Luego umbralizo       **/
	/** ********************************************** **/
	
	Mat imgArena = Segmentar(imgRec, 0, 100, 40, 80);
	Mat imgBinA;
	cvtColor(imgArena,imgBinA,COLOR_BGR2GRAY);
	threshold(imgBinA,imgBinA,150,255,THRESH_BINARY);
		
	
	/** ********************************************** **/
	/**  Elimino todo el ruido de la segmentacion      **/
	/** ********************************************** **/
	Mat ee = getStructuringElement(MORPH_ERODE,Size(3,3));
	morphologyEx(imgBinA,imgBinA,MORPH_ERODE,ee);
	morphologyEx(imgBinA,imgBinA,MORPH_ERODE,ee);
	threshold(imgBinA,imgBinA,0,255,THRESH_BINARY);
	
	/** ********************************************** **/
	/**  Cuento cuantos pixeles de arena hay           **/
	/** ********************************************** **/
	int cantidad = CantArena(imgBinA);
	cout << "Cantidad de pixeles con arena "<< cantidad;
	
	/** ********************************************** **/
	/**  SEGMENTO EL VIDRIO. Luego umbralizo       **/
	/** ********************************************** **/
	/// Tengo que calcular cuantos granos de arena entran en el vidrio
	Mat imgVidrio = Segmentar(imgRec, 0, 90, 0, 100);
	
	Mat imgBinV;
	cvtColor(imgVidrio,imgBinV,COLOR_BGR2GRAY);
	threshold(imgBinV,imgBinV,0,255,THRESH_BINARY);
	
	
	/** ********************************************************************* **/
	/**  Elimino el ruido de la segmentacion. Y relleno pixeles blancos       **/
	/** ********************************************************************* **/
	Mat eeV = getStructuringElement(MORPH_RECT,Size(9,9));
	morphologyEx(imgBinV,imgBinV,MORPH_ERODE,eeV);
	
	eeV = getStructuringElement(MORPH_RECT,Size(10,10));
	morphologyEx(imgBinV,imgBinV,MORPH_DILATE,eeV);
	morphologyEx(imgBinV,imgBinV,MORPH_DILATE,eeV);
	threshold(imgBinV,imgBinV,0,255,THRESH_BINARY);
	
	/** ********************************************************* **/
	/** Cuento cuantos pixeles de arena entran en el vidrio       **/
	/** ********************************************************* **/
	int capacidad = CantArena(imgBinV);
	cout << "\n" << "Cantidad de pixeles total " << capacidad;
	

	/** *************************************************************************** **/
	/**  Calculo mediante una regla de 3 simple cuantos minutos transcurrieron      **/
	/** *************************************************************************** **/
	float hora = (cantidad * 36000 ) / capacidad;
	cout << " \n  Transcurrieron: " << hora/3600 << " minutos" ;
	
	namedWindow("Imagen",0);
	imshow("Imagen",img);
	
	waitKey(0);
	return 0;
} 
