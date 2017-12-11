#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FuncionesMias.h"
#include <vector>
#include <opencv2/core/types_c.h>
#include <opencv2/gpu/gpu.hpp>

using namespace cv;


Mat etiq,VerdesRojas;
int etiqueta;
int tam;

//int 
//	cent1 = 0,
//	cent2= 0,
//	cent5 = 0,
//	cent10 = 0,
//	cent20 = 0,
//	cent50 = 0,
//	eur1 = 0,
//	eur2 = 0;
//

void crecer(int i,int j){
	tam++;
	etiq.at<uchar>(i,j) = etiqueta;
	for(int k = -1;k<2;k++){
		for(int m = -1;m<2;m++){
			int ni=i+k;
			int nj=j+m;
			if( (int)VerdesRojas.at<uchar>(ni,nj)!=0 and (int)etiq.at<uchar>(ni,nj)==0 and ni!=nj){
				crecer(ni,nj);
			}
		}
	}
	
}

vector<Point2d> etiquetar_regiones_no_solapadas(){
	etiqueta=1;
	vector<Point2d> v;
	for(int i = 0 ; i<VerdesRojas.rows;i++){
		for(int j = 0 ; j<VerdesRojas.cols*3;j++){
			if( (int)VerdesRojas.at<uchar>(i,j)!= 0 && (int)etiq.at<uchar>(i,j)==0 ){
				tam =0;
				crecer(i,j);
				if (/*tam>100&&*/tam<200){
					cout<<"\nTamaño: "<<tam<<endl;
					v.push_back(Point2d((int)j/3,i));
				}
				etiqueta++;
			}
			
		}
	}
	return v;
}



int ContarManzanas(Mat img){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	return contours.size();
}

int main(int argc, char** argv) {
	Mat img = imread("EXAMEN10.jpg",IMREAD_COLOR);
	imshow("Imagen",img);
	
	


	///Segmento las manzanas rojas
	Mat manzanasRojas = Segmentar(img,100,200,200,255);
	cvtColor(manzanasRojas,manzanasRojas,CV_BGR2GRAY);
	threshold(manzanasRojas,manzanasRojas,50,255,CV_THRESH_BINARY);
	Mat ee = getStructuringElement(MORPH_RECT,Size(4,4));
	morphologyEx(manzanasRojas,manzanasRojas,MORPH_ERODE,ee);
	morphologyEx(manzanasRojas,manzanasRojas,MORPH_ERODE,ee);
	imshow("Manzanas Rojas",manzanasRojas);
	Mat rojas = manzanasRojas.clone();//lo uso despues
	cout<<"\nCantidad de manzanas rojas: "<<ContarManzanas(manzanasRojas);

	
	///Segmento las manzanas Verdes
	Mat manzanasVerdes = Segmentar(img,20,50,200,255);
	cvtColor(manzanasVerdes,manzanasVerdes,CV_BGR2GRAY);
	threshold(manzanasVerdes,manzanasVerdes,80,255,CV_THRESH_BINARY);
	ee = getStructuringElement(MORPH_RECT,Size(4,4));
	morphologyEx(manzanasVerdes,manzanasVerdes,MORPH_ERODE,ee);
	morphologyEx(manzanasVerdes,manzanasVerdes,MORPH_ERODE,ee);
	imshow("Manzanas Verdes",manzanasVerdes);
	Mat verdes = manzanasVerdes.clone();//lo uso despues
	cout<<"\nCantidad de manzanas verdes: "<<ContarManzanas(manzanasVerdes);

	
	VerdesRojas = verdes+rojas;
	cvtColor(VerdesRojas,VerdesRojas,CV_GRAY2BGR);
	etiq = Mat::zeros(VerdesRojas.size(),VerdesRojas.type());
	etiqueta = 0;
	vector<Point2d> v = etiquetar_regiones_no_solapadas();
	
	for(int i = 0 ; i<v.size();i++){
		circle(img,v[i],10,Scalar(255,0,0),2,-1);
	}
	imshow("Manzanas Azules",img);
	
	
	waitKey();
	return 0;
} 
