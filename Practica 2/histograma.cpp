#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"
using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output", CV_WINDOW_AUTOSIZE); //by default is in autosize
	
	Mat img = imread("lion.png",CV_8U);
	vector<Mat > bgr; 
	split(img, bgr);
	bgr[2]=bgr[2]-50;
	merge(bgr, img);
	
	pdi::draw_graph(pdi::histogram(bgr[1],256,cv::Mat()),255);
	
	imshow("imagen 1",img);
	
	return 0;
} 


