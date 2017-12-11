#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"
using namespace cv;


int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output", CV_WINDOW_AUTOSIZE); //by default is in autosize
	
	Mat img = imread("billete.jpg",CV_8U);
	
	//set the ROI
	Mat img2;
	img.convertTo(img,CV_32F,1./255);	
	img2 =  pdi::filter_butterworth(img.rows,img.cols,.08,50);
	
	
	//display the image:
	imshow("Output", img);
	imshow("Output2", img2);
	
	
	waitKey(0);
	return 0;
} 


