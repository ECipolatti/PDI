#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"

using namespace cv;
using namespace pdi;

Mat mult_frecuencias(Mat img,Mat filtro);
Mat AltaPotenciaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW ,float A);
Mat EnfasisAltaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW, float a, float b);
Mat FiltroFrecuencialIdeal(Mat img, int tipo, float fc);
Mat FiltroFrecuencialButterworth(Mat img, int tipo, float fc, int orden);
Mat FiltroFrecuencialGaussian(Mat img, int tipo, float fc);
Mat FiltroHomomorfico(Mat img, float gh, float gl, float fc, float c);

int main(int argc, char** argv) {
		Mat imagen = imread("cameraman.tif",IMREAD_GRAYSCALE);
		imshow("Imagen",imagen);
		
	/// ************-PASA BAJOS-************
	//LAS FUNCIONES DE FILTRO SON LAS MISMAS! 
	//	TIPO 1 = PASA BAJOS
	//	TIPO 2 = PASA ALTOS
	// Ejercicio 2.1 - Filtro Ideal PB
		Mat ImagenFFTIdeal = FiltroFrecuencialIdeal(imagen,1,.8);//Tipo 1 = PBajo
		imshow("FFT Ideal PB",ImagenFFTIdeal);	
	// Ejercicio 2.2 - Filtro Butterworth PB
		Mat ImagenFFTButter = FiltroFrecuencialButterworth(imagen,1,0.3,5);
		imshow("FFT Butter PB",ImagenFFTButter);	
	// Ejercicio 2.3 - Filtro Gaussiano PB 
		Mat ImagenFFTGaussiano = FiltroFrecuencialGaussian(imagen,1,0.3);
		imshow("FFT Gaussiano PB",ImagenFFTGaussiano);
	
	/// ************-PASA ALTOS-************
	//Filtro Ideal PA
		Mat ImagenFFTIdealPA = FiltroFrecuencialIdeal(imagen,2,0.3);//Tipo 1 = PBajo
		imshow("FFT Ideal PA",ImagenFFTIdealPA);
	//Filtro Butterworth PA
		Mat ImagenFFTButterPA = FiltroFrecuencialButterworth(imagen,2,0.3,5);
		imshow("FFT Butter PA",ImagenFFTButterPA);
	//Filtro Gaussiano PA
		Mat ImagenFFTGaussianoPA = FiltroFrecuencialGaussian(imagen,2,0.3);
		imshow("FFT Gaussiano PA",ImagenFFTGaussianoPA);	
	waitKey(0);
	return 0;
} 

Mat mult_frecuencias(Mat img,Mat filtro){
	Mat salida,imgFFT;
	dft(img,imgFFT,DFT_COMPLEX_OUTPUT);
	mulSpectrums(imgFFT,filtro,salida,DFT_ROWS);
	idft(salida,salida,DFT_REAL_OUTPUT| DFT_SCALE);
	normalize(salida,salida,0,1,CV_MINMAX);
	return salida;
}


Mat AltaPotenciaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW ,float A){
	//HAP = (A-1) + HPA
	img.convertTo(img,CV_32F,1/255.f);
	img = optimum_size(img);
	Mat imgAPDF;
	switch(tipoPAfrec){
	case 1:{ //filtro frecuencial ideal
		Mat FIPB = filter_ideal(img.rows, img.cols, fc); //devuelve el filtro ideal pasa bajo en dom espacial
		Mat FIPA = 1 - FIPB; //obtengo el filtro ideal pasa alto en dom espacial
		FIPA = optimum_size(FIPA);
		imgAPDF = filter(img,FIPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	case 2: { //filtro frecuencial butterworth
			Mat FButterPB = filter_butterworth(img.rows, img.cols, fc, ordenBW); //devuelve el filtro butter pasa bajo en dom espacial
			Mat FButterPA = 1 - FButterPB; //obtengo el filtro butter pasa alto en dom espacial
			FButterPA = optimum_size(FButterPA);
			imgAPDF = filter(img,FButterPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
			break;
		}
	case 3: { //filtro frecuencial gaussiano
		Mat FGPB = filter_gaussian(img.rows, img.cols, fc); //devuelve el filtro gaussian pasa bajo en dom espacial
		Mat FGPA = 1 - FGPB; //obtengo el filtro gaussian pasa alto en dom espacial
		FGPA = optimum_size(FGPA);
		imgAPDF = filter(img,FGPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	}	
	return (A-1) + imgAPDF;
}
Mat EnfasisAltaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW, float a, float b){
	//HAP = (A-1) + HPA
	Mat imgAPDF;
	switch(tipoPAfrec){
	case 1:{ //filtro frecuencial ideal
		Mat FIPB = filter_ideal(img.rows, img.cols, fc); //devuelve el filtro ideal pasa bajo en dom espacial
		Mat FIPA = 1 - FIPB; //obtengo el filtro ideal pasa alto en dom espacial
		FIPA = optimum_size(FIPA);
		imgAPDF = filter(img,FIPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	case 2: { //filtro frecuencial butterworth
			Mat FButterPB = filter_butterworth(img.rows, img.cols, fc, ordenBW); //devuelve el filtro butter pasa bajo en dom espacial
			Mat FButterPA = 1 - FButterPB; //obtengo el filtro butter pasa alto en dom espacial
			FButterPA = optimum_size(FButterPA);
			imgAPDF = filter(img,FButterPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
			break;
		}
	case 3: { //filtro frecuencial gaussiano
		Mat FGPB = filter_gaussian(img.rows, img.cols, fc); //devuelve el filtro gaussian pasa bajo en dom espacial
		Mat FGPA = 1 - FGPB; //obtengo el filtro gaussian pasa alto en dom espacial
		FGPA = optimum_size(FGPA);
		imgAPDF = filter(img,FGPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	}	
	return a + b*imgAPDF;
}
Mat FiltroFrecuencialIdeal(Mat img, int tipo, float fc){
	Mat results;
	img.convertTo(img,CV_32F, 1/255.f);
	img = optimum_size(img);
	switch(tipo){
	case 1: { //filtro ideal frecuencial pasa bajo
		Mat FIPB = filter_ideal(img.rows,img.cols,fc);
		FIPB = optimum_size(FIPB);
		results = filter(img,FIPB);
		break;
	}
	case 2: { //filtro ideal frecuencial pasa alto
		Mat FIPB = filter_ideal(img.rows,img.cols,fc);
		Mat FIPA = 1 - FIPB;
		FIPA = optimum_size(FIPA);
		results = filter(img,FIPA);
	}
	}
	return results;
}
Mat FiltroFrecuencialButterworth(Mat img, int tipo, float fc, int orden){
	Mat results;
	img.convertTo(img,CV_32F, 1/255.f);
	img = optimum_size(img);
	switch(tipo){
	case 1: { //filtro butterworth frecuencial pasa bajo
		Mat FPB_BW = filter_butterworth(img.rows,img.cols,fc,orden);
		FPB_BW = optimum_size(FPB_BW);
		results = filter(img,FPB_BW);
		break;
	}
	case 2: { //filtro butterworth frecuencial pasa alto
		Mat FPB_BW = filter_butterworth(img.rows,img.cols,fc,orden);
		Mat FPA_BW = 1 - FPB_BW;
		FPA_BW = optimum_size(FPA_BW);
		results = filter(img,FPA_BW);
		break;
	}
	}
	return results;
}
Mat FiltroFrecuencialGaussian(Mat img, int tipo, float fc){
	Mat results;
	img.convertTo(img,CV_32F, 1/255.f);
	img = optimum_size(img);
	switch(tipo){
	case 1:{
		Mat FGPB = filter_gaussian(img.rows,img.cols,fc);
		FGPB = optimum_size(FGPB);
		results = filter(img,FGPB);
		break;
	}
	case 2:{
			Mat FGPB = filter_gaussian(img.rows,img.cols,fc);
			Mat FGPA = 1 - FGPB;
			FGPA = optimum_size(FGPA);
			results = filter(img,FGPA);	
			break;
		}
	}
	return results;
}
Mat FiltroHomomorfico(Mat img, float gh, float gl, float fc, float c){
	img.convertTo(img,CV_32F,1/255.f);	
	img = optimum_size(img);
	Mat H(img.size(),CV_32F,1/255.);
	Mat	results;
	Mat	imgLog;
	//Defino funcion H
	float dist;
	int centro_x=img.rows/2;
	int centro_y=img.cols/2;
	for(int x=0;x<img.rows;x++){
		for(int y=0;y<img.cols;y++){
			dist=sqrt(pow(x-centro_x,2)+pow(y-centro_y,2));
			H.at<float>(x,y)=(float)(gh-gl)*(1.0-exp(-c*(pow(dist,2)/pow(fc,2))))+gl;
		}
	}
	H = optimum_size(H);
	
	imgLog.convertTo(imgLog,CV_32F,1/255.f);
	imgLog = optimum_size(imgLog);
	
	log(img,imgLog);
	Mat filtrado = filter(imgLog,H);
	exp(filtrado,results);
	return results;
}






