#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Mat resta(Mat img,Mat img2){
	Mat img3 = img2(Rect(0,0,img.cols,img.rows));
	Mat resultado(img.cols,img.cols,CV_8U);
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			int a = img.at<uchar>(i,j);
			int b = img3.at<uchar>(i,j);
			resultado.at<uchar>(i,j) = (a-b)/2 + 128;
		}
	}
	return resultado;
	
}

int main(int argc, char** argv) {
	
	Mat img = imread("lion.png",CV_8U);
	Mat img2 = imread("FAMILIA.jpg",CV_8U);
	
//	Mat img3 = img2(Rect(0,0,img.cols,img.rows));
//	Mat resultado(img.cols,img.cols,CV_8U);
//	for (int i=0;i<img.cols;i++){
//		for (int j=0;j<img.rows;j++){
//			int a = img.at<uchar>(i,j);
//			int b = img3.at<uchar>(i,j);
//			resultado.at<uchar>(i,j) = (a-b)/2 + 128;
//		}
//	}
	
	Mat resultado =resta(img,img2);
	//display the image:
	imshow("leon", img);
	imshow("familia", img2);
//	imshow("recorte familia", img3);
	imshow("Resta", resultado);
	
	
	waitKey(0);
	return 0;
} 



