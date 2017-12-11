#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"
using namespace cv;


Mat linear_lut(float a, float c);
int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Original"); //by default is in autosize
	namedWindow("Salida");
	Mat img = imread("lion.png",CV_8UC1);
	
	float a = 1;
	float c = 50;
	Mat lut = linear_lut(a, c);
	Mat img_mod;
	
	LUT(img, lut, img_mod);
	
	
	
	
	
	
	//display the image:
	imshow("Original", img);
	imshow("Salida", img_mod);
	
	Mat img3(255,255,CV_8U);
	img3.setTo(Scalar(0,0,0));
	pdi::draw_graph(img3,lut);
	namedWindow("Grafico del LUT");
	imshow("Grafico del LUT", img3);
	
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

