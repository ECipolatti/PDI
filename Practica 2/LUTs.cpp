#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"
using namespace cv;


Mat linear_lut(float a, float c);
Mat LogLUT(int c);
Mat expLUT(float c, float gamma);

int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Original"); //by default is in autosize
	namedWindow("Salida");
	Mat img = imread("lion.png",CV_8UC1);
	
	float a = 1;
	float c = 50;
	Mat lut = linear_lut(a, c);
	Mat lut2 = LogLUT(80);
	Mat lut3 = expLUT(.5,1.2);
	Mat img_mod,img_mod2,img_mod3;
	
	LUT(img, lut, img_mod);
	LUT(img, lut2, img_mod2);
	LUT(img, lut3, img_mod3);
	
	
	//display the image:
	imshow("Original", img);
	imshow("Salida lut lineal", img_mod);
	imshow("Salida lut logaritmico", img_mod2);
	imshow("Salida lut exponencial", img_mod3);
//	Mat img3(255,255,CV_8U);
//	img3.setTo(Scalar(0,0,0));
//	pdi::draw_graph(img3,lut);
//	namedWindow("Grafico del LUT");
//	imshow("Grafico del LUT", img3);
	
	waitKey(0);
	return 0;
} 


Mat linear_lut(float a, float c){
	int s = 0;
	Mat lut(1, 256, CV_8UC1);
	
	for (int r = 0; r < 256; ++r){
		s = floor (a*r + c);
		if (s > 255)
			s = 255;
		if (s < 0)
			s = 0;
		lut.at<uchar>(r) = s;
	}
	return lut;
}


Mat LogLUT(int c){
	Mat lut(1,256,CV_8U);		
	for(int r = 0; r<lut.cols; r++){
		float s = c * log10(1+r);
		if(s>255) s = 255;
		if(s<0) s = 0;
		lut.at<uchar>(0,r) =  (int)s;
	}
	return lut;
}

Mat expLUT(float c, float gamma){
	Mat lut(1,256,CV_8U);		
	for(int r = 0; r<lut.cols; r++){
		float s = c * pow(r,gamma);
		if(s>255) s = 255;
		if(s<0) s = 0;
		lut.at<uchar>(0,r) =  (int)s;
	}
	return lut;
}

