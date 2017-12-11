#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

float prom_intensidad(Mat roi);
void detectar(Mat img);

int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output", CV_WINDOW_AUTOSIZE); //by default is in autosize
	
	Mat img1 = imread("a7v600-SE.jpeg",CV_8U);
	Mat img1m = imread("a7v600-SE(RImpulsivo).jpeg",CV_8U);
	Mat img2 = imread("a7v600-X.jpeg",CV_8U);
	Mat img2m = imread("a7v600-X(RImpulsivo).jpeg",CV_8U);
	
	//set the ROI
	detectar(img2m);
	
	waitKey(0);
	return 0;
} 

float prom_intensidad(Mat roi){
	float prom=0;
	int tam=roi.cols*roi.rows;
	for(int i = 0; i<roi.rows; i++){
		for(int j = 0; j< roi.cols; j++){
			prom = prom + roi.at<uchar>(i,j);
		}
	}
	
	prom =prom/tam;
	return prom;
}
void detectar(Mat img){
	Mat roi = img(Rect(200,100,50,50));
	float promedio= prom_intensidad(roi);
	if (promedio>100)
		cout<<"La placa es la a7v600-SE";
	else
		cout<<"La placa es la a7v600-X";
}
