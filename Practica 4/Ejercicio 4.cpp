#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"

using namespace cv;
using namespace std;
using namespace pdi;

////VARIABLES GLOBALES PARA EL EJERCICIO 4.1
int radio = 0;
Point3i color;
Mat img;
Mat mascara; //Mascara binaria

// Funcion que toma el color del punto clickeado
void on_mouse(int event, int x, int y, int flags, void*){	
	if ( event == CV_EVENT_LBUTTONDOWN) {
		color.x = (int) img.at<Vec3b>(x,y)[0];
		color.y = (int) img.at<Vec3b>(x,y)[1];;
		color.z = (int) img.at<Vec3b>(x,y)[2];
	}
}

// Controlo la barra para mover los planos de color
void on_trackbar(int,void*){
	Mat aux = mascara.clone();
	
	//Compruebo la distancia del circulo
	for(int i=0;i<img.rows;i++){
		for(int j=0;j<img.cols;j++){
			int blue  = (int)img.at<Vec3b>(i,j)[0];
			int green = (int)img.at<Vec3b>(i,j)[1];
			int red   = (int)img.at<Vec3b>(i,j)[2];
			
			//Calculo la distancia
			Point3i bgr(blue,green,red);
			float distance = norm(color-bgr);
			if(distance < radio){
				aux.at<Vec3b>(i,j)[0]=1;
				aux.at<Vec3b>(i,j)[1]=1;
				aux.at<Vec3b>(i,j)[2]=1;
			}
		}
	}
	Mat imgfinal;
	multiply(aux,img,imgfinal);
	imshow("Segmentacion",imgfinal);
	imshow("MASCARA",aux*255); //Muestro la mascara binaria
}

int main(int argc, char** argv) {
	
	
//	Mat img = imread("futbol.jpg");
//	imshow("Output", img);
//	vector<Mat> rgb_channels;
//	split(img, rgb_channels);
//	
//	Mat hist = histogram(rgb_channels[2],256);
//	Mat canvas(img.size(),img.type());
//	normalize(hist,hist,0,1,CV_MINMAX);
//	draw_graph(canvas,hist);
//	imshow("Histograma",canvas);
	
	
	img = imread("futbol.jpg"); //Variable global

	mascara = Mat::zeros(img.size(),img.type()); //Variable gobal
	
	imshow("Segmentacion",img);
	
	//Las variables de los trackbar son globales (arriba del main)
	setMouseCallback("Segmentacion",on_mouse);
	createTrackbar("Radio","Segmentacion",&radio,255,on_trackbar);
	
	
	waitKey(0);
	return 0;
} 


