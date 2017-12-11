#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Mat expLUT(float c, float gamma){
	Mat lut(1,256,CV_8U);		
	for(int r = 0; r<lut.cols; r++){
		float s = c * pow(r,gamma);
		if(s>255) s = 255;
		if(s<0) s = 0;
		lut.at<uchar>(0,r) =  (int)s;
	}
	return lut;
}

Mat LogLUT(int c){
	Mat lut(1,256,CV_8U);		
	for(int r = 0; r<lut.cols; r++){
		float s = c * log10(1+r);
		if(s>255) s = 255;
		if(s<0) s = 0;
		lut.at<uchar>(0,r) =  (int)s;
	}
	return lut;
}

int main(int argc, char** argv) {
	//create a gui window:
//	namedWindow("Output", CV_WINDOW_AUTOSIZE); //by default is in autosize
	
	Mat img = imread("earth.bmp",CV_8U);
	
	//set the ROI
	Mat lut = expLUT(.5,1.9);
	Mat lutlog =LogLUT(90);
	Mat img_mod;
	
	LUT(img, lutlog, img_mod);
	
	
	
	
	//display the image:
	imshow("original", img);
	imshow("modificada",img_mod);
	
	
	waitKey(0);
	return 0;
} 


