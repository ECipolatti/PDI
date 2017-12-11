#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>
#include <iostream>
#include <math.h>
//#include <conio.h>
#include <ctime>
#include <thread>
#include <pthread.h>
using namespace std;
using namespace cv;

vector<Mat> carga_db();
void *getim(void *vec);


int main(){
	
	
	vector<Mat> vec = carga_db();
	
//	for (int rep =1;rep<6;rep++){
//		cout<<"REPETICIONES: "<<rep<<endl;
//		
//		clock_t start;
//		double duration;
//		start = clock();
//		for(int r=0;r<rep;r++){
//			for (int i=0;i<460;i+=4){
//				pthread_t h1,h2,h3,h4;
//				pthread_create(&h1,NULL,getim,&vec[i]);
//				pthread_create(&h2,NULL,getim,&vec[i+1]);
//				pthread_create(&h3,NULL,getim,&vec[i+2]);
//				pthread_create(&h4,NULL,getim,&vec[i+3]);
//				pthread_join(h1,NULL);
//				pthread_join(h2,NULL);
//				pthread_join(h3,NULL);
//				pthread_join(h4,NULL);
//			}
//		}
//		duration = (clock()-start)/(double) CLOCKS_PER_SEC;
//		cout<<"La duracion en paralelo es: "<<duration<<endl;
//		
//		start = clock();
//		for (int r=0;r<rep;r++){
//			for (int i=0;i<460;i++){
//				getim(&vec[i]);
//			}
//		}
//		duration = (clock()-start)/(double) CLOCKS_PER_SEC;
//		cout<<"La duracion en single core es: "<<duration<<endl;
//		
//	}
	getim(&vec[4]);
	waitKey(0);
	return 0;
}

void *getim(void *vec)
{
	Mat im =*((Mat *) vec);
//	Mat im=imread("parrot.tif");
	if (im.empty()) 
	{
		cout << "Error : Image cannot be loaded..!!" << endl;
	}
	
	resize(im,im,Size(240,240),0.0,0.0,1);
		imshow("Input Image",im);
	Mat temp=Mat::zeros(im.size(),im.type());
	double m=im.rows,x,x1,y,y1; 
	int enciter=5;
	int deciter=60-enciter;
	
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
		temp=Mat::zeros(im.size(),im.type());
	}
		imshow("Scrambled Image",im);
	
	
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
		imshow("Inverse Scrambled Image",im);
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

//opennp
