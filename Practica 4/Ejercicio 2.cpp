#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"
#include <vector>

using namespace cv;
using namespace pdi;
using namespace std;

int main(int argc, char** argv) {
	//create a gui window:
	//by default is in autosize
	
	Mat img = imread("rio.jpg",CV_16UC3);
	imshow("imagen",img);
//	Mat hist = histogram(img,256);
//	Mat canvas(img.size(),img.type());
//	normalize(hist,hist,0,1,CV_MINMAX);
//	draw_graph(canvas,hist);
//	namedWindow("Histograma",0);
//	imshow("Histograma",canvas);
	
	
	vector<Mat> hsv_channels(3);
	hsv_channels[0]=img.clone();
	hsv_channels[1]=img.clone();
	hsv_channels[2]=img.clone();
	
	
	for (int i=0;i<img.rows;i++){
		for (int j=0; j<img.cols;j++){
			int intensidad = (int) img.at<uchar>(i,j);
			if (intensidad<9){
				hsv_channels[0].at<uchar>(i,j)=0;
				hsv_channels[1].at<uchar>(i,j)=255;
				hsv_channels[2].at<uchar>(i,j)=255;
			}
		}
	}
	Mat img_coloreada;
	merge(hsv_channels,img_coloreada);
	imshow("Imagen HSV modificada",img_coloreada);

	
	
	waitKey(0);
	return 0;
} 


