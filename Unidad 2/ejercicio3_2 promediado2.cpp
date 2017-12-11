#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>


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


Mat ruido(int cols, int rows, float varianza){
	Mat r(cols,rows,CV_32FC1);
	Mat med(1,1,CV_32FC1);
	Mat vari(1,1,CV_32FC1);
	med.setTo(0);
	vari.setTo(sqrt(varianza));
	randn(r,med,vari);
	return r;
}


int main(int argc, char** argv) {
	
	Mat img = imread("lion.png",CV_8U);
	imshow("Imagen Original", img);
	vector<Mat> vecmat;
	int cant = 5;
	for (int i=0;i<cant;i++){
		Mat r = ruido(img.cols,img.rows,.00505);
		vecmat.push_back(suma(img,r));
	}
	
	
	Mat resultado(img.cols,img.rows,CV_8U);
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			float acum =0;
			for (int x=0;x<cant; x++){
				acum = acum +  vecmat[x].at<uchar>(i,j);
			}
			resultado.at<uchar>(i,j)=(acum/cant);
			acum=0;
		}
	}
	
	
	
	
	//display the image:
	imshow("Output", resultado);
	
	
	waitKey(0);
	return 0;
} 


