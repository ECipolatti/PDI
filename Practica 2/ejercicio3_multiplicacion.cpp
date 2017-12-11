#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Mat multiplicacion(Mat img, Mat mascara);

int main(int argc, char** argv) {
	
	Mat img = imread("lion.png",CV_8U);
	Mat img2 = Mat::zeros(img.cols,img.rows,CV_8U);
	for (int i=50;i<img.cols-50;i++){
		for (int j=50;j<img.rows-50;j++){
			img2.at<uchar>(i,j)=1;
		}
	}
	
//	Mat resultado(img.cols,img.cols,CV_8U);
//	for (int i=0;i<img.cols;i++){
//		for (int j=0;j<img.rows;j++){
//			int a = img.at<uchar>(i,j);
//			int b = img2.at<uchar>(i,j);
//			resultado.at<uchar>(i,j) = a*b;
//			img2.at<uchar>(i,j)=img2.at<uchar>(i,j)*255;
//		}
//	}
	Mat resultado = multiplicacion(img,img2);
	
	//display the image:
	imshow("leon", img);
	imshow("Mascara", img2);
	imshow("Multiplicacion", resultado);
	
	
	waitKey(0);
	return 0;
} 
Mat multiplicacion(Mat img, Mat mascara){
	Mat resultado(img.cols,img.cols,img.type());
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			int a = img.at<uchar>(i,j);
			int b = mascara.at<uchar>(i,j);
			resultado.at<uchar>(i,j) = a*b;
		}
	}
	return resultado;
}
