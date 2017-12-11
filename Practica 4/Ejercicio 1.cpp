#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void mostrar_comp(Mat img){
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			cout<<img.at<Vec3b>(i,j)<<" ";
		}
		cout<<endl<<endl;
	}
}
void info(Mat img){
	cout<<"cantidad de columnas: "<<img.cols<<endl;
	cout<<"cantidad de filas: "<<img.rows<<endl;
}

Mat variacion1(Mat img){
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			img.at<Vec3b>(i,j)[0] = 180-img.at<Vec3b>(i,j)[0];
		}
	}
	return img;
}

Mat variacion2(Mat img){
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			img.at<Vec3b>(i,j)[1] = 255;
			img.at<Vec3b>(i,j)[2] = 255;
		}
	}
	return img;
}

int main(int argc, char** argv) {
//
//	Mat img = imread("patron.tif",CV_16S);
//	Mat img_hsv;
//	imshow("Output", img);
//	
//	mostrar_comp(img(Rect(0,0,1,64)));
////	info(img);
//	//CONVIERO A HSV
//	cvtColor(img, img_hsv, CV_BGR2HSV);
//	
//	cout<<endl<<endl;
//	
//	mostrar_comp(img_hsv(Rect(0,0,1,64)));
//	Mat var1 = variacion1(img_hsv);
//	Mat var2 = variacion2(img_hsv);
//	
//	mostrar_comp(var2(Rect(0,0,1,64)));
//	
//	
//	//display the image:
//	cvtColor(var1, var1, CV_HSV2BGR);
//	cvtColor(var2, var2, CV_HSV2BGR);
//	imshow("variacion 1", var1);
//	imshow("variacion 2", var2);
	
//----------------------------------------------------
	Mat img = imread("rosas.jpg",CV_16S);
	imshow("Rosas originales",img);
	
	Mat img_hsv(img.size(),img.type()); 
	
	
	//Paso de RGB a HSV
	cvtColor(img,img_hsv,CV_RGB2HSV);
	vector<Mat> hsv_channels;
	//Muestro la imagen en HSV original
	imshow("Imagen HSV",img_hsv);
	
	//Separo los canales
	split(img_hsv, hsv_channels);
	
	hsv_channels[0] +=90;
//	
	//Uno las componentes HSV Modificadasy muestro 
	merge(hsv_channels,img_hsv);
	imshow("Imagen HSV modificada",img_hsv);
	
	//Paso la a RGB
	cvtColor(img_hsv,img,CV_HSV2BGR);
	//Muestro la imagen en HSV original
	imshow("Imagen RGB Modificada",img);
	
	waitKey(0);
	return 0;
} 


