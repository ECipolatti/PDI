#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat suma(Mat a, Mat b){

	Mat c(a.cols, a.rows, CV_8U);
	for (int i=0;i<a.cols;i++){
		for (int j=0;j<a.rows;j++){
			int aaux = a.at<uchar>(i,j);
			int baux = b.at<uchar>(i,j);
			c.at<uchar>(i,j) = aaux/2 + baux/2;
		}
	}
	return c;
}



int main(int argc, char** argv) {
	
	Mat img = imread("lion.png",CV_8U);
	Mat img2 = imread("FAMILIA.jpg",CV_8U);
		
	Mat img3 = img2(Rect(0,0,img.cols,img.rows));
	Mat resultado = suma(img,img3);
		
	
	//display the image:
	imshow("leon", img);
	imshow("familia", img2);
	imshow("recorte familia", img3);
	imshow("suma", resultado);
	
	
	waitKey(0);
	return 0;
} 


