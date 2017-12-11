#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "FuncionesMias.h"
#include <opencv2/gpu/gpu.hpp>
#include <string>


using namespace cv;
using namespace pdi;
using namespace std;

Mat Recortar(Mat img){
	
	for(int i = 0; i<img.rows;i++){
		for(int j = 0; j<img.cols;j++){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				Mat Subimagen = img(Rect(j,i,60,80));
				return	Subimagen;
			}	
		}
	}
}

//////IDEA: segmentar mediante el modelo HSV, utilizando una mascara binaria.


int main(int argc, char** argv) {

	vector<Mat> hsv_channels(3);
	Mat imgHSI;
	
	//cargo la imagen
	Mat imagen = imread("2.jpg");
	imshow("Imagen",imagen);
	
	//Mascara binaria
	Mat mascara = cv::Mat::zeros(imagen.size(),imagen.type());
	
	//	//Paso de RGB a HSI
	Mat img_hsi;
	cvtColor(imagen,img_hsi,COLOR_BGR2HSV);
	
	//Divido los canales
	split(img_hsi,hsv_channels);
	
	////Variando estos valores obtengo diferentes rangos para el color del rebanado
	////Coloco el Hue en el intervalo del amarillo. 
	////Coloco la saturacion en un valor alto
	//// Obtengo estos valores observando el circulo de colores
	////Defino el color amarillo (para el cartel)
	int Hmin = 20,
		Hmax = 40,
		Smin = 200,
		Smax = 255;
	
	////Recorro las componentes H y S, entonces si estos valores están dentro de 
	////los minimos y maximos definidos anteriormente a la mascara le coloco 1,
	////sino queda en cero. Entonces en otras palabras, si encuentra el amarillo
	//// a la mascara la dejo en 1, de lo contrario 0.
	for (int i = 0; i<imagen.rows;i++)
		for (int j = 0; j<imagen.cols;j++){
		int H = (int)hsv_channels[0].at<uchar>(i,j);
		int S = (int)hsv_channels[1].at<uchar>(i,j);
		if ((Hmin<H)&&(H<Hmax) && (Smin<S)&&(S<Smax)){
			mascara.at<cv::Vec3b>(i,j)[0]=1; //Blue
			mascara.at<cv::Vec3b>(i,j)[1]=1; //Green
			mascara.at<cv::Vec3b>(i,j)[2]=1; //Red
		}
	}
	
	////Multiplico la imagen con la mascara y asi obtengo solo el cartel
	Mat cartelAmarillo;
	multiply(imagen,mascara,cartelAmarillo); //Multiplico la imagen por la mascara
	imshow("Cartel Amarillo",cartelAmarillo);
	
	
	////Convierto la imagen a escala de grises para luego Binarizar
	Mat cartelGris;
	cvtColor(cartelAmarillo,cartelGris,COLOR_BGR2GRAY);
	imshow("Cartel escala Gris",cartelGris);

	////Binarizo la imagen
	Mat cartelBinarizado;
	threshold(cartelGris,cartelBinarizado,10,255,CV_THRESH_BINARY); //Hago 0 valores menores a 10
	imshow("Cartel Binarizado",cartelBinarizado);
	
	////Aplico Apertura para poder limpiar afuera del cartel
	Mat ee = getStructuringElement(MORPH_RECT,Size(3,3)); //Defino el EE rectangular
	Mat apertura;
	morphologyEx(cartelBinarizado,apertura,MORPH_OPEN,ee);
	imshow("Apertura",apertura);
	
	
//	////Extraigo el cartel
	Mat recortada =  Recortar(apertura);
	imshow("Recortado",recortada);
	



	
	
	
	
	
	
	
	waitKey();
	
	
	
	return 0;
} 
