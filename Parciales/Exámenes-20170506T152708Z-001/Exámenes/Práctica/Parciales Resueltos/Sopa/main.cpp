#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FuncionesMias.h"
#include <vector>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/imgproc/imgproc_c.h>

using namespace cv;

void Intensidad( int event, int x, int y, int, void* var){
	Mat *img = (Mat*)var;
	vector<Mat> canales;
	split(*img,canales);
	
	if (event == EVENT_LBUTTONDOWN){
		cout<< "B: "<<(int)canales[0].at<uchar>(y,x)<< 
			   " G: "<<(int)canales[1].at<uchar>(y,x)<<
			   " R: "<<(int)canales[2].at<uchar>(y,x)<<endl;
	}
}
bool TipoDeSopa(Mat img){
	for(int i = 0 ; i< img.rows; i++){
		for(int j = 0; j < img.cols; j++){
			int r = (int) img.at<uchar>(i,j);
			if( r != 0)
				return true;
		}
	}
	return false;
}

int ContarMoscas(Mat img){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	return contours.size();
}

int main(int argc, char** argv) {
	Mat imagen = imread("Platos01.jpg",IMREAD_COLOR);
	namedWindow("Original",0);
	imshow("Original",imagen);
	
	

//	vector<Mat> canales;
//	cvtColor(imagen,imagen,COLOR_BGR2HSV);
//	split(imagen,canales);
//	imshow("H",canales[0]);
//	imshow("S",canales[1]);
	///Segmento la sopa
	Mat sopa = Segmentar(imagen,5,20,150,255); //Obtengo el naranja
	///Aplico una Apertura para limpiar
	Mat ee = getStructuringElement(MORPH_RECT,Size(10,10));
	morphologyEx(sopa,sopa,MORPH_OPEN,ee);
	morphologyEx(sopa,sopa,MORPH_ERODE,ee);
	morphologyEx(sopa,sopa,MORPH_ERODE,ee);
//	namedWindow("Sopa",0);
//	imshow("Sopa",sopa);
	
	/// 1 - Identifico el tipo de sopa
	// true --> Zapallo
	// false --> De la Casa
	bool tipoSopa = TipoDeSopa(sopa);
	if(tipoSopa)
		cout<< "Es sopa de zapallo";
	else
		cout<<"Es sopa de la casa";
	
	/// 2 - Cantidad de moscas en la escena (No interesa el tipo de sopa)
	Mat moscas(imagen.size(),imagen.type());
	cvtColor(imagen,moscas,COLOR_BGR2GRAY);  //Transforma a gris
	threshold(moscas,moscas,15,255,THRESH_BINARY_INV); //A valores menores a 200 los pone en 0
	
	ee = getStructuringElement(MORPH_RECT,Size(5,5));
	morphologyEx(moscas,moscas,MORPH_CLOSE,ee); //Para limpiar dentro las moscas
	morphologyEx(moscas,moscas,MORPH_OPEN,ee); //Para limpiar fuera de las moscas

//	namedWindow("Moscas",0);
//	imshow("Moscas",moscas);
	
	cout<<endl<<"\nTotal de Moscas: "<< ContarMoscas(moscas);

	/// 3 - Cantidad en el plato (No interesa el tipo de sopa)

	Mat moscas2(imagen.size(),imagen.type());
	cvtColor(imagen,moscas2,COLOR_BGR2GRAY);  //Transforma a gris
	threshold(moscas2,moscas2,100,255,THRESH_BINARY); //A valores menores a 200 los pone en 0

	ee = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(moscas2,moscas2,MORPH_ERODE,ee,Point(-1,-1),6); 
	morphologyEx(moscas2,moscas2,MORPH_CLOSE,ee,Point(-1,-1),9); 
	namedWindow("Moscas",0);
	imshow("Moscas",moscas2);
	
	cout<<endl<<"\nMoscas en el plato : "<< ContarMoscas(moscas2);
	
	
	/// 4 - Cantidad de moscas en la sopa
	
	if(tipoSopa){
		Mat sopaZapallo = Segmentar(imagen,0,90,100,2555); //Obtengo el naranja
		namedWindow("Zapallo",0);
		imshow("Zapallo",sopaZapallo);
		
		cvtColor(sopaZapallo,sopaZapallo,COLOR_BGR2GRAY);  //Transforma a gris
		threshold(sopaZapallo,sopaZapallo,0,255,THRESH_BINARY); //A valores menores a 200 los pone en 0
		
		ee = getStructuringElement(MORPH_RECT,Size(3,3));
		morphologyEx(sopaZapallo,sopaZapallo,MORPH_ERODE,ee,Point(-1,-1),4); //Para limpiar fuera de las moscas
		morphologyEx(sopaZapallo,sopaZapallo,MORPH_CLOSE,ee,Point(-1,-1),9); //Para limpiar fuera de las moscas

		cout<<endl<<"\nMoscas en la sopa de Zapallo : "<< ContarMoscas(sopaZapallo)-1; //Resto 1 porque sino cuenta la circunferencia
	}
	else{
		Mat sopaCasa= Segmentar(imagen,0,100,100,255); //Obtengo Sopa de la casa
		namedWindow("Casa",0);
		imshow("Casa",sopaCasa);
		
		cvtColor(sopaCasa,sopaCasa,COLOR_BGR2GRAY);  //Transforma a gris
		threshold(sopaCasa,sopaCasa,0,255,THRESH_BINARY); //A valores menores a 200 los pone en 0
		
		ee = getStructuringElement(MORPH_RECT,Size(3,3));
		morphologyEx(sopaCasa,sopaCasa,MORPH_ERODE,ee,Point(-1,-1),4); //Para limpiar fuera de las moscas
		morphologyEx(sopaCasa,sopaCasa,MORPH_CLOSE,ee,Point(-1,-1),9); //Para limpiar fuera de las moscas

		cout<<endl<<"\nMoscas en la sopa de la Casa : "<< ContarMoscas(sopaCasa)-1; //Resto 1 porque sino cuenta la circunferencia
	}



	

	
	
	waitKey();
	
	return 0;
} 
