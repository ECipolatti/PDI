#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "FuncMias.h"

using namespace cv;
using namespace pdi;
using namespace m;

int main(int argc, char** argv) {
	Mat img1 = imread("f01.jpg");
	Mat img2 = imread("f02.jpg");
	Mat img3 = imread("f03.jpg");
	Mat img4 = imread("f04.jpg");
	Mat img5 = imread("f05.jpg");
	Mat img6 = imread("f06.jpg");
	Mat img7 = imread("f07.jpg");
	Mat img8 = imread("f08.jpg");
	Mat img9 = imread("f09.jpg");
	Mat img10 = imread("f10.jpg");
	Mat img11= imread("f11.jpg");
	Mat img12 = imread("f12.jpg");
	Mat img13 = imread("f13.jpg");
	Mat img14 = imread("f14.jpg");
	Mat img15 = imread("f15.jpg");
	Mat img16 = imread("f16.jpg");
	Mat img17 = imread("f17.jpg");
	Mat img18 = imread("f18.jpg");
	Mat img19 = imread("f19.jpg");
	Mat img20 = imread("f20.jpg");
	Mat img21 = imread("f21.jpg");
	
	vector<Mat> v;
	v.push_back(img21);
	v.push_back(img20);
	v.push_back(img19);
	v.push_back(img18);
	v.push_back(img17);
	v.push_back(img16);
	v.push_back(img15);
	v.push_back(img14);
	v.push_back(img13);
	v.push_back(img12);
	v.push_back(img11);
	v.push_back(img10);
	v.push_back(img9);
	v.push_back(img8);
	v.push_back(img7);
	v.push_back(img6);
	v.push_back(img5);
	v.push_back(img4);
	v.push_back(img3);
	v.push_back(img2);
	v.push_back(img1);
	
	/** ******************************************************** **/
	vector<Mat> VSN;
	Mat salida = Mat::zeros(img1.size(),img1.type());
	Mat img,aux;
	Mat planos[3];
	Mat hue;
	Mat val;
	//recorro todas las imagenes y construyo una nueva con los pixeles que no contienen nubes
	for(int k=0; k<v.size(); k++){
		img = v[k];
		cvtColor(img,aux,CV_BGR2HSV);
		split(aux,planos);
		hue = planos[0];
		val = planos[2];
		for (int i=0; i<hue.rows; i++){
			for(int j=0; j<hue.cols; j++){
				if (hue.at<uchar>(i,j) < 85 or hue.at<uchar>(i,j)>110){
					salida.at<Vec3b>(i,j) = img.at<Vec3b>(i,j);
				}else {
					if (val.at<uchar>(i,j)<127) salida.at<Vec3b>(i,j) = img.at<Vec3b>(i,j);
				}
			}
		}
		medianBlur(salida,salida,3);
		medianBlur(salida,salida,3);
		VSN.push_back(salida);
	}
	
	vector<Mat> VC;
	for (int k=0; k<VSN.size(); k++){
		img = VSN[1];
		cvtColor(img,aux,CV_BGR2HSV);
		split(aux,planos);
		hue = planos[0];
		for(int i=0; i<hue.rows;i++){
			for(int j=0; j<hue.cols; j++){
				if (hue.at<uchar>(i,j)<39 or hue.at<uchar>(i,j)>41){
					hue.at<uchar>(i,j) = 0;
				}
			}
		}
		medianBlur(hue,hue,3);
		Mat ee = getStructuringElement(MORPH_RECT,Size(3,3));
		morphologyEx(hue,hue,MORPH_CLOSE,ee,Point(-1,-1),3);
		morphologyEx(hue,hue,MORPH_ERODE,ee,Point(-1,-1),5);
		
		VC.push_back(hue);
	}
	
	Mat imgfinal2 = DibujarHough(VC[15],52);
	
	/** Para calcular el angula, recorro la primer linea y encuentro el primer punto
		Recorro la segunda linea y encuentro el segundo punto **/
	
	

	namedWindow("estadio");
	imshow("esatdio",VSN[0]);
	namedWindow("h");
	imshow("h",imgfinal2);
	waitKey(0);
	
	return 0;
} 
