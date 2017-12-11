#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(int argc, char** argv) {
	Mat imagen = imread("1.png",IMREAD_COLOR);
	imshow("Original",imagen);
	waitKey(0);
	return 0;
} 
