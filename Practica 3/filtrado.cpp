#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"

using namespace cv;
using namespace std;
using namespace pdi;

Mat kernel_pasa_bajo(int tipo, int vecindad);
Mat kernel_pasa_alto(int tipo, int suma);
Mat resta(Mat img,Mat img2);
Mat mascara_difusa(Mat img);
Mat multiplicacion(Mat img, Mat mascara);
Mat acentuado(float A,Mat img);

int main(int argc, char** argv) {
	
	Mat img = imread("cameraman.tif",CV_8U);
	imshow("imagen",img);
	
//	Mat kernel_10 = kernel_pasa_alto(1,0);
//	Mat kernel_20 = kernel_pasa_alto(2,0);
//	Mat kernel_11 = kernel_pasa_alto(1,1);
//	Mat kernel_21 = kernel_pasa_alto(2,1);
//	Mat kernel_31 = kernel_pasa_alto(3,1);
//	
//	imshow("imagen Filtrada_10", convolve(img,kernel_10));
//	imshow("imagen Filtrada_20", convolve(img,kernel_20));
//	imshow("imagen Filtrada_11", convolve(img,kernel_11));
//	imshow("imagen Filtrada_21", convolve(img,kernel_21));
//	imshow("imagen Filtrada_31", convolve(img,kernel_31));
	//-----------------------------
//	Mat kernel_13 = kernel_pasa_bajo(1,3);
//	Mat kernel_23 = kernel_pasa_bajo(2,3);
//	Mat kernel_33 = kernel_pasa_bajo(3,3);
//	Mat kernel_15 = kernel_pasa_bajo(1,5);
//	Mat kernel_17 = kernel_pasa_bajo(1,7);
//	
//	imshow("imagen Filtrada_13", convolve(img,kernel_13));
//	imshow("imagen Filtrada_23", convolve(img,kernel_23));
//	imshow("imagen Filtrada_33", convolve(img,kernel_33));
//	imshow("imagen Filtrada_15", convolve(img,kernel_15));
//	imshow("imagen Filtrada_17", convolve(img,kernel_17));
	//_---------------------------------
	
	imshow("mascara difusa",mascara_difusa(img));
	imshow("mascara difusa multi",2*mascara_difusa(img));
	
	imshow("ACENTUADO",acentuado(2.5,img));
	waitKey(0);
	return 0;
} 


Mat kernel_pasa_bajo(int tipo, int vecindad){
	
	switch (vecindad){
	case 3:{
		if (tipo ==1){ //suavizado
			Mat kernel=Mat::zeros(3,3,CV_32F);
			kernel.at<float>(1,1)=(float) 1/5;
			kernel.at<float>(0,1)=(float)1/5;
			kernel.at<float>(2,1)=(float)1/5;
			kernel.at<float>(1,0)=(float)1/5;
			kernel.at<float>(1,2)=(float)1/5;
			return kernel;
			break;
		}
		if(tipo == 2){ //promediado
			Mat kernel=Mat::zeros(3,3,CV_32F);
			kernel.at<float>(1,1)= (float)1/2;
			kernel.at<float>(0,1)=(float)1/8;
			kernel.at<float>(2,1)=(float)1/8;
			kernel.at<float>(1,0)=(float)1/8;
			kernel.at<float>(1,2)=(float)1/8;
			return kernel;
			break;
		}
		if(tipo ==3) {	//Promediado
			Mat kernel=Mat::zeros(3,3,CV_32F);
			for (int i=0;i<3;i++){
				for (int j=0; j<3; j++){
					kernel.at<float>(i,j)=(float)1/9;
				}
			}
			return kernel;
			break;
		}
		
	}
	case 5:{//pasa alto
		Mat kernel=Mat::zeros(5,5,CV_32F);
		for (int i=0;i<5;i++){
			for (int j=0; j<5; j++){
				kernel.at<float>(i,j)=(float) 1/25;
			}
		}
		return kernel;
		break;}
	case 7:{
		Mat kernel=Mat::zeros(7,7,CV_32F);
		for (int i=0;i<7;i++){
			for (int j=0; j<7; j++){
				kernel.at<float>(i,j)=(float)1/49;
			}
		}
		return kernel;
		break;}
	}
}


Mat kernel_pasa_alto(int tipo, int suma){
	
	switch (suma){
	case 0:{//Suma=0: extracción de altas frecuencias, eliminando las bajas frecuencias.
		if (tipo ==1){ 
			Mat kernel=Mat::zeros(3,3,CV_32F);
			for (int i=0;i<3;i++){
				for (int j=0; j<3; j++){
					kernel.at<float>(i,j)=-1;
				}
			}
			kernel.at<float>(1,1)=8;
			return kernel;
		}
		if (tipo==2){ 
			Mat kernel=Mat::zeros(3,3,CV_32F);
			kernel.at<float>(1,1)=4;
			kernel.at<float>(0,1)=-1;
			kernel.at<float>(2,1)=-1;
			kernel.at<float>(1,0)=-1;
			kernel.at<float>(1,2)=-1;
			return kernel;
		}
		break;}
	case 1:{////Suma=1: realce de altas frecuencias sin alterar las bajas frecuencias.
			if (tipo ==1){
				Mat kernel=Mat::zeros(3,3,CV_32F);
				kernel.at<float>(1,1)=5;
				kernel.at<float>(0,1)=-1;
				kernel.at<float>(2,1)=-1;
				kernel.at<float>(1,0)=-1;
				kernel.at<float>(1,2)=-1;
				return kernel;
			}
			if (tipo ==2){
				Mat kernel=Mat::zeros(3,3,CV_32F);
				for (int i=0;i<3;i++){
					for (int j=0; j<3; j++){
						kernel.at<float>(i,j)=-1;
					}
				}
				kernel.at<float>(1,1)=9;
				return kernel;
			}
			if (tipo ==3){
				Mat kernel=Mat::zeros(3,3,CV_32F);
				kernel.at<float>(1,1)=5;
				kernel.at<float>(0,1)=-2;
				kernel.at<float>(2,1)=-2;
				kernel.at<float>(1,0)=-2;
				kernel.at<float>(1,2)=-2;
				kernel.at<float>(0,0)=1;
				kernel.at<float>(0,2)=1;
				kernel.at<float>(2,0)=1;
				kernel.at<float>(2,2)=1;
				return kernel;
			}
			break;}
	}
}

Mat resta(Mat img,Mat img2){
	Mat img3 = img2(Rect(0,0,img.cols,img.rows));
	Mat resultado(img.cols,img.cols,CV_8U);
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			int a = img.at<uchar>(i,j);
			int b = img3.at<uchar>(i,j);
			resultado.at<uchar>(i,j) = (a-b)/2 + 128;
		}
	}
	return resultado;
}

Mat multiplicacion(Mat img, Mat mascara){
	Mat resultado(img.cols,img.cols,img.type());
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			int a = img.at<uchar>(i,j);
			int b = mascara.at<uchar>(i,j);
			resultado.at<uchar>(i,j) = a*b;
		}
	}
	return resultado;
}
Mat mascara_difusa(Mat img){
	Mat k= kernel_pasa_bajo(1,3);
	return resta(img,convolve(img,k));
	
};
Mat acentuado(float A,Mat img){
	Mat k= kernel_pasa_bajo(1,3);
	return resta(A*img,convolve(img,k));
	
};
