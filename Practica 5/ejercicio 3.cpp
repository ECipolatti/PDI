#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"

using namespace cv;
using namespace pdi;

Mat FiltroHomomorfico(Mat img, float gh, float gl, float fc, float c);

int main(int argc, char** argv) {
	Mat img = imread("reunion.tif",IMREAD_GRAYSCALE);
	Mat img2 = img.clone();
	Mat canvas(img.size(),img.type());
	Mat canvas2(img.size(),img.type());
	
	Mat hist = histogram(img,256);
	normalize(hist,hist,0,1,CV_MINMAX);
	draw_graph(canvas,hist);
	
	equalizeHist(img,img);
	Mat HistEq = histogram(img,256);
	normalize(HistEq,HistEq,0,1,CV_MINMAX);
	draw_graph(canvas2,HistEq);
	
	float gl = 2.1;
	float gh = 0.7;
	float fc = 10;
	float c = 2;
	
	Mat results = FiltroHomomorfico(img, gh, gl, fc, c);
	
	imshow("Imagen",img2);
	imshow("Histograma",canvas);
	imshow("Hist Eq",canvas2);
	imshow("Imagen FHomo",results);
	waitKey(0);
	return 0;
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


