#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "FuncionesMias.h"
#include "Funciones.h"
#include <vector>

using namespace cv;
using namespace std;
using namespace pdi;

int main(int argc, char** argv) {
	Mat src = imread("Fut02_3.png",CV_LOAD_IMAGE_COLOR);
	Mat logo = imread("Logo03.png",CV_LOAD_IMAGE_COLOR);
	Mat unl = imread("unl.jpg",CV_LOAD_IMAGE_COLOR);
	Mat fich = imread("fich.jpg",CV_LOAD_IMAGE_COLOR);
	
//	vector<Mat> channels = canales("Fut01_2.png", 0, 0);
	
//	detect_linesP_trackbar(img);
	vector<Vec4i> lines; //LO QUE DA EL DETECTLINES OSEA ESCRIBE LAS LINEAS ARRIBA DE LA IMAGEN
	Mat dst = src.clone();
	Mat con_lineas = src.clone();
	lines = detect_linesP(src, dst, 156, 26, 27, 123, 147, 0);
	for(size_t i = 0; i < lines.size(); ++i) {
		Vec4i l = lines[i];
		line(con_lineas, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
	}
//	imshow("Original con lineas", con_lineas);
	
	int columna=0,fila=0;
//	info(src);
	for (int i=0; i<src.cols;i++){
		Vec3b intensidad = con_lineas.at<Vec3b>(300,i);
		if(intensidad[0] == 255 && intensidad[1]==0 &&intensidad[2]==0){
			columna=i;
			break;
		}
	}
	for (int i=src.rows-1; i>0;i--){
		Vec3b intensidad = con_lineas.at<Vec3b>(i,400);
		if(intensidad[0] == 255 && intensidad[1]==0 &&intensidad[2]==0){
			fila=i;
			break;
		}
	}
//	cout<<"la columna es: "<<columna<<endl;
//	cout<<"la fila es: "<<fila<<endl;
	

	
	
	Point3i color = Point3i(src.at<Vec3b>(450,20)[0],src.at<Vec3b>(450,20)[1],src.at<Vec3b>(450,20)[2]);
	int radio =50;
	Mat seg = colorDistance(src,color,radio);
//	imshow("seg",seg);
	
	
	Mat img= src.clone();
	info(fich);
	
	
	for(int i=fila+2; i<fila+2+fich.rows; i++){
		for (int j=columna-fich.cols-2; j<columna-2; j++){
			Vec3b intensidad = seg.at<Vec3b>(i,j);
			if(intensidad[0]!=0 || intensidad[1]!=0 || intensidad[2]!=0){
				img.at<Vec3b>(i,j) = fich.at<Vec3b>(i-fila-2,j-columna+fich.cols+2);
			}
		}
	}
	
	for(int i=fila+2; i<fila+2+unl.rows; i++){
		for (int j=columna+5; j<columna+5+unl.cols; j++){
			Vec3b intensidad = seg.at<Vec3b>(i,j);
			if(intensidad[0]!=0 || intensidad[1]!=0 || intensidad[2]!=0){
				img.at<Vec3b>(i,j) = unl.at<Vec3b>(i-fila-2,j-columna-5);
			}
		}
	}
	
	info(logo); //58 * 396
	
	for(int i=img.rows-logo.rows-2; i<img.rows-2; i++){
		for (int j=columna-logo.cols/2; j<columna+logo.cols/2; j++){
			Vec3b intensidad = seg.at<Vec3b>(i,j);
			if(intensidad[0]!=0 || intensidad[1]!=0 || intensidad[2]!=0){
				img.at<Vec3b>(i,j) = logo.at<Vec3b>(i-img.rows+logo.rows+2,j-columna+logo.cols/2);
			}
		}
	}
	
	imshow("IMAGEN FINAL",img);
//	segmentacionBGR_trackbar(img);
	
	

	
	
	
//	
//	imshow("futbol",img);
//	Mat imgbw,img2;
//	cvtColor(img,img2,CV_BGR2GRAY);
//	threshold(img2,imgbw,50,255,CV_THRESH_BINARY);
	
//	segmentacionBGR_trackbar("Fut01_2.png");
	
	
	
	
///-----------------   ESTO ANDA JOSHA ----------------------------------------------
	waitKey(0);
	return 0;
} 
