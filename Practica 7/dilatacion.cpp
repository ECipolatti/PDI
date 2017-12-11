#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output", CV_WINDOW_AUTOSIZE); //by default is in autosize
	
	Mat img = imread("rosas.jpg",CV_8U);

	imshow("Output", img);
	Mat kernel(3,3,CV_8U);
	for (int i=0;i<kernel.cols;i++){
		for (int j=0; j<kernel.rows;j++){
			kernel.at<uchar>(i,j)=255;
		}
	}
	threshold(img,img,200,255,THRESH_BINARY);
//	dilate(img,kernel,9);
	imshow("Output2", img);
	
	waitKey(0);
	return 0;
} 


