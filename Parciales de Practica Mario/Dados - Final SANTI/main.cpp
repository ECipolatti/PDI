#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "FuncMias.h"

using namespace cv;
using namespace pdi;
using namespace m;

int d1x, d1y;
Mat Dados;

int Contar(Mat img){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	return contours.size();
}

Point2d Encontrar(){
	int v;
	Point2d d1;
	for (int i=0; i<Dados.rows; i++){
		for (int j=0; j<Dados.cols; j++){
			v = (int)Dados.at<uchar>(i,j);
			if (v != 0){
				d1.x = i;
				d1.y = j;
				return d1;
			}
		}		
	}
}
 Mat Restar(Mat img, Mat img2){
	for(int i=0; i<img2.rows;i++){
		for(int j=0; j<img2.cols;j++){
			img.at<uchar>(i,j) = 0;
		}
	}
	return img;
 }

int main(int argc, char** argv) {
	Mat img = imread("Dados02-3dados_azul.jpg");
	
	
	Mat planos[3];
	Mat hsi;
	Mat Seg(img.size(),img.type());
	cvtColor(img,hsi,COLOR_BGR2HSV);
	split(hsi,planos);
	imshow("h",planos[0]);
	imshow("s",planos[1]);
	imshow("i",planos[2]);
	
	/** segmento en hsi, con sautracion en negro al maximo, y obtengo la cantidad de puntos, por tanto es el
		numero total de dados **/
	Mat s = planos[1];
	for (int i =0; i< s.rows;i++){
		for (int j =0; j< s.cols;j++){
			if (s.at<uchar>(i,j)>0 and s.at<uchar>(i,j)<50 ) {
				Seg.at<Vec3b>(i,j) = img.at<Vec3b>(i,j) ;
			}
		}
		
	}
	
	/** Umbralizo **/
	cvtColor(Seg,Seg,COLOR_BGR2GRAY);
	threshold(Seg,Seg,150,255,CV_THRESH_BINARY);
	
	/** Erosiono para borrar imperfecciones y aplico cerradura para llenar las circunferencais **/
	Mat ee = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(Seg,Seg,MORPH_ERODE,ee,Point(-1,-1),1);
	morphologyEx(Seg,Seg,MORPH_CLOSE,ee,Point(-1,-1),5);
	Dados = Seg.clone();
//	imshow("i",Dados);
	int Numero = Contar(Seg);
	cout << "La suma total de los dados es: " << Numero << "\n" ;
	
	

//	int area = 100;
//	int dist = 30;
//	Mat Dado1, Dado2, Dado3;
//	int d1x, d1y;
//	int v;
//	
////	print(Dados(Rect(0,0,80,80)));
//	
////	//Buscamos dado 1
//	Point2d d1 = Encontrar();
//	cout << d1.x<< " " << d1.y;
//	Dado1 = Dados(Rect(d1.x-50,d1.y-50,100,100));
////	imshow("d",Dado1);
//	int NumeroDado1 = Contar(Dado1);
//	cout << " \nDado1:  " << NumeroDado1;
//	
//	
	//Buscamos dado 2
//	Dados = Restar(Dados,Dado1);
//    Point2d d2 =Encontrar();
//	cout << d2.x<< " " << d2.y;
//	imshow("p",Dados);
//	Dado2 = Dados(Rect(d2.x,d2.y,100,10));
//	imshow("d",Dado2);
//	int NumeroDado2 = Contar(Dado2);
//	cout << " Dado2:  " << NumeroDado2;
	
	
    //Buscamos dado 3
//	Dados = Restar(Dados,Dado2);
//	Encontrar();
//	cout << d3x << " " << d3y;
//	Dado3 = Dados(Rect(d3x-dist,d3y,area, area));
//	imshow("i",Dados);
	
	
	waitKey(0);
	return 0;
} 
