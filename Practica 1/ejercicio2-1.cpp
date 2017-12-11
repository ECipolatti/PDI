#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void CallBackFunc(int event, int x, int y, int flags, void* userdata);


int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output"); //by default is in autosize
	
	Mat img = imread("lion.png",CV_LOAD_IMAGE_GRAYSCALE);
	
//	for (int j=0;j<img.rows;j++){
//		for (int i=0; i<img.cols;i++){
//			cout<<img.at<Vec3b>(j,i)<<" ";
//		}
//		cout<<endl;
//	}
	
	
	//set the callback function for any mouse event
	setMouseCallback("Output", CallBackFunc, NULL);
	
//	
	//display the image:
	imshow("Output", img);
	
	
	waitKey(0);
	return 0;
} 


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left click - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_RBUTTONDOWN )
	{
		cout << "Right click - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_MBUTTONDOWN )
	{
		cout << "Middle click - position (" << x << ", " << y << ")" << endl;
	}
	else if ( event == EVENT_MOUSEMOVE )
	{
		cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
		
	}
}
