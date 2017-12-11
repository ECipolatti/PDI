#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	//create a gui window:
	
	Mat img = imread("mario.png",CV_16S);
	Mat img2 = imread("mario.png",CV_16S);
	
	bool isEqual = (sum(img != img2) == Scalar(0,0,0,0));
	if(isEqual){
		cout<<"son iguales 1111111111"<<endl;
//		cout<<"El periodo es: "<<per;
	}
	else{
		cout<<"SON DISTINTAS 1"<<endl;
	}
	img2 = imread("mm.png");
	
	 isEqual = (sum(img != img2) == Scalar(0,0,0,0));
	if(isEqual){
		cout<<"son iguales 2222222222"<<endl;
//		cout<<"El periodo es: "<<per;
		
	}
	else{
		cout<<"SON DISTINTAS 2"<<endl;
	}
	
	//set the ROI
	
	
	
	
	//display the image:
	imshow("Output", img);
	
	
	waitKey(0);
	return 0;
} 


