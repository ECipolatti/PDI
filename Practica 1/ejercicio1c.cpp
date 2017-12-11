#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output", CV_WINDOW_AUTOSIZE); //by default is in autosize
	
	Mat img = imread("lion.png",CV_8U);
	
	for (int i=10; i<50; i++){
		for (int j=10; j<50;j++){

			Rect roi = Rect(10,10,50,50);
			img(roi)=200; //Remplazo la ROI por un color
		}
		cout<<endl;
	}
	
	
	//display the image:
	imshow("Output", img);
	
	
	waitKey(0);
	return 0;
} 


