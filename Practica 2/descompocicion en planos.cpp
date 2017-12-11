#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <bitset>

using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	//create a gui window:
	
	
	Mat img = imread("lion.png",CV_8U);
	
	int columnas=img.cols;
	int filas =img.rows;
	Mat plano0(filas, columnas, CV_8U);
	Mat plano1(filas, columnas, CV_8U);
	Mat plano2(filas, columnas, CV_8U);
	Mat plano3(filas, columnas, CV_8U);
	Mat plano4(filas, columnas, CV_8U);
	Mat plano5(filas, columnas, CV_8U);
	Mat plano6(filas, columnas, CV_8U);
	Mat plano7(filas, columnas, CV_8U);
	

	for (int i=0; i<img.rows; i++){
		for (int j=0; j<img.cols; j++){
			std::bitset<8> x((int) img.at<uchar>(i,j));
			plano0.at<uchar>(i,j) = x[0]*255; 
			plano1.at<uchar>(i,j) = x[1]*255;
			plano2.at<uchar>(i,j) = x[2]*255;
			plano3.at<uchar>(i,j) = x[3]*255;
			plano4.at<uchar>(i,j) = x[4]*255;
			plano5.at<uchar>(i,j) = x[5]*255;
			plano6.at<uchar>(i,j) = x[6]*255;
			plano7.at<uchar>(i,j) = x[7]*255;

		}
	}

	//display the image:
	imshow("Output", img);
	imshow("Plano 0", plano0);
	imshow("Plano 1", plano1);
	imshow("Plano 2", plano2);
	imshow("Plano 3", plano3);
	imshow("Plano 4", plano4);
	imshow("Plano 5", plano5);
	imshow("Plano 6", plano6);
	imshow("Plano 7", plano7);
	
	
	waitKey(0);
	return 0;
} 


