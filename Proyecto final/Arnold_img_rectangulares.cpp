#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <conio.h>
#include <ctime>
#include <thread>
#include <pthread.h>
#include <cmath>
using namespace std;
using namespace cv;

vector<Mat> carga_db();

Mat encripto(Mat img, int periodo);
Mat desencripto(Mat img, int periodo);
vector<Mat> particiona(Mat img, int &periodo);
Mat forma_imagen(vector<Mat> V, Mat O);

////periodos
//  tamaño periodo
//  3		4;
//  4		3;
//  5		10;
//  6		12;
//  7       8;
//  25		50;
//  32		24;
//  64		49;
//  100		150;
//  120		60;
//  128 	96;
//  256		192;
//  512		284;

int main(){
	
	Mat img = imread("img1.png");
//	img = img(Rect(0,0,1280,720));
	int periodo;
	cout<<"columnas"<<img.cols<<endl;cout<<"filas"<<img.rows<<endl;
	imshow("Original",img);
	vector<Mat> partes = particiona(img, periodo);
//	imshow("partecita",partes[1]);
	cout<<"tamaño: "<<partes[0].cols<<" periodo es: "<<periodo<<endl;
	
	for (int i=0;i<partes.size();i++){
		partes[i]=encripto(partes[i],periodo);
	}
	
	Mat encriptada = forma_imagen(partes,img);
	imshow("Encriptada",encriptada);
	partes = particiona(encriptada,periodo);
	
	for (int i=0;i<partes.size();i++){
		partes[i]=desencripto(partes[i],periodo);
	}
	
	Mat desencriptada = forma_imagen(partes,img);
	imshow("desencriptada",desencriptada);

	
	
	waitKey(0);
	return 0;
}


vector<Mat> carga_db(){
	vector<Mat> V;
	for(int i=0;i<460;i++){
		stringstream sstm;
		sstm << "DB\\a" << i<< ".tiff";
		string aux;
		aux = sstm.str();
		V.push_back(imread(aux));
	}
	return V;
}


Mat encripto(Mat img, int periodo){
	Mat im=img.clone();
	double m=im.rows,x,x1,y,y1; 
	int enciter=floor(periodo/3);
	int deciter=periodo-enciter;
	Mat temp=Mat::zeros(im.size(),im.type());
	for(int iter=0;iter<enciter;iter++)
	{
		
		for(double i=0;i<m;i++)
		{
			for(double j=0;j<m;j++)
			{
				x=fmod((i+j),m);   
				y=fmod((i+2*j),m);            
				temp.at<Vec3b>(x,y) = im.at<Vec3b>(i,j);
			}
		}
		temp.copyTo(im);
		Mat temp=Mat::zeros(im.size(),im.type());
	}
	return im;
}
Mat desencripto(Mat img, int periodo){
	Mat im = img.clone();
	Mat temp=Mat::zeros(im.size(),im.type());
	double m=im.rows,x,x1,y,y1; 
	
	int enciter=floor(periodo/3);
	int deciter=periodo-enciter;
		
	for(int iter=0;iter<deciter;iter++)
	{
		
		for(double i=0;i<m;i++)
		{
			for(double j=0;j<m;j++)
			{
				x=fmod((i+j),m);   
				y=fmod((i+2*j),m);            
				temp.at<Vec3b>(x,y)=im.at<Vec3b>(i,j);
			}
		}
		temp.copyTo(im);
		temp=Mat::zeros(im.size(),im.type());
	}
	return im;
}

vector<Mat> particiona(Mat img, int &periodo){
	vector<Mat> V;
	int a=32;
	if((img.cols%a==0) && (img.rows%a==0)){
		periodo = 24;
		for(int i=0; i<img.cols/a;i++){
			for (int j=0;j<img.rows/a;j++){
				V.push_back(img(Rect(a*i,a*j,a,a)));
			}
		}
	}else{
		a=120;
		if((img.cols%a==0) && (img.rows%a==0)){
			periodo = 60;
			for(int i=0; i<img.cols/a;i++){
				for (int j=0;j<img.rows/a;j++){
					V.push_back(img(Rect(a*i,a*j,a,a)));
				}
			}
		}
		else{
			a=80;
			if((img.cols%a==0) && (img.rows%a==0)){
				periodo = 60;
				for(int i=0; i<img.cols/a;i++){
					for (int j=0;j<img.rows/a;j++){
						V.push_back(img(Rect(a*i,a*j,a,a)));
					}
				}
			}
			else{
				a=128;
				if((img.cols%a==0) && (img.rows%a==0)){
					periodo = 96;
					for(int i=0; i<img.cols/a;i++){
						for (int j=0;j<img.rows/a;j++){
							V.push_back(img(Rect(a*i,a*j,a,a)));
						}
					}
				}
				else{
					a=60;
					if((img.cols%a==0) && (img.rows%a==0)){
						periodo = 60;
						for(int i=0; i<img.cols/a;i++){
							for (int j=0;j<img.rows/a;j++){
								V.push_back(img(Rect(a*i,a*j,a,a)));
							}
						}
					}
					else{
						a=100;
						if((img.cols%a==0) && (img.rows%a==0)){
							periodo = 150;
							for(int i=0; i<img.cols/a;i++){
								for (int j=0;j<img.rows/a;j++){
									V.push_back(img(Rect(a*i,a*j,a,a)));
								}
							}
						}else{
							a=25;
							if((img.cols%a==0) && (img.rows%a==0)){
								periodo = 50;
								for(int i=0; i<img.cols/a;i++){
									for (int j=0;j<img.rows/a;j++){
										V.push_back(img(Rect(a*i,a*j,a,a)));
									}
								}
							}else{
								a=16;
								if((img.cols%a==0) && (img.rows%a==0)){
									periodo = 12;
									for(int i=0; i<img.cols/a;i++){
										for (int j=0;j<img.rows/a;j++){
											V.push_back(img(Rect(a*i,a*j,a,a)));
										}
									}
								}else{
									a=4;
									if((img.cols%a==0) && (img.rows%a==0)){
										periodo = 3;
										for(int i=0; i<img.cols/a;i++){
											for (int j=0;j<img.rows/a;j++){
												V.push_back(img(Rect(a*i,a*j,a,a)));
											}
										}
									}else{ V.push_back(Mat::zeros(img.size(),img.type()));}
								}
							}
						}
					}
				}
			}
		}
	}
//	cout<<"Termina bien"<<endl;
	return V;
}

Mat forma_imagen(vector<Mat> V, Mat O){
	Mat img = Mat::zeros(O.size(), O.type());
	int ancho = V[0].rows;
//	cout<<"Tamaño del vector: "<<V.size()<<endl;
	for(int i=0; i<img.cols/ancho;i++){
		for(int j=0; j<img.rows/ancho;j++){
//			cout<<"Indice: "<<  i + (j*img.cols/ancho) <<endl;
			V[ (i*img.rows/ancho) + j].copyTo(img(Rect(i*ancho,j*ancho,ancho,ancho)));
		}
	}
	return img;
}
//106.32
//776.061
