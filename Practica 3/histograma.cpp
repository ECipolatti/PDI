#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"

using namespace cv;
using namespace std;
using namespace pdi;

int main(int argc, char** argv) {
	
	//Ejercicio 1-1
	
	
//	Mat img = imread("cameraman.tif",CV_8UC1);
//	imshow("Original", img);
//	
//	Mat h = pdi::histogram(img,256,cv::Mat());
//	Mat histograma= pdi::draw_graph(h,cv::Scalar::all(255));
//	imshow("HISTOGRAMA original",histograma);
//	
//	//Ecualizo la imagen (porque tiene un solo canla, sino la tengo que dividir
//	Mat img2(img.size(),img.type());
//	equalizeHist(img,img2);
//	Mat h2 = pdi::histogram(img2,256,cv::Mat());
//	Mat histograma2= pdi::draw_graph(h2,cv::Scalar::all(255));
//	imshow("HISTOGRAMA ecualizado",histograma2);
//	imshow("IMAGEN ecualidada", img2);
	
	//Ejercicio 1-2
	
	Mat img = imread("imagenE.tif",CV_8UC1);
	imshow("imagenE", img);
	
	Mat h = pdi::histogram(img,256,cv::Mat());
	Mat histograma= pdi::draw_graph(h,cv::Scalar::all(255));
	imshow("HISTOGRAMA imagenE",histograma);
	
	
	
	
	
	waitKey(0);
	return 0;
} 


