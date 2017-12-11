#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "pdi_functions.h"

using namespace cv;
using namespace std;
using namespace pdi;



int main(int argc, char** argv) {
	//create a gui window:
	
	
	Mat img = imread("clown.jpg",CV_8U);
	Mat img2 =imread("lion.png");
//	vector<Mat> planes(3);
//	split(img, planes);
//	Mat R = planes[2];
//	Mat G = planes[1];
//	Mat B = planes[0];
	
	
	Mat grafico = Mat::zeros(img.rows,img.cols,CV_8U);
	draw_graph(grafico,img);
	namedWindow("Intensidades");
	imshow("Intensidades",grafico);
	
	
	
	//display the image:
	imshow("Output", img);
	
	
	waitKey(0);
	return 0;
} 


