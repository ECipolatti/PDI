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
	
	Mat src = imread("mano06.png");
	imshow("Imagen ORIGINAL",src);
//	vector<Mat> channels = canales("mano01.png", 1, 1);
//	segmentacionBGR_trackbar(src);
	src=src(Rect(0,0,src.cols,350));
//	info(src);
	
	//PARA LOS ANILLOS DORADOS
	Point3i color = Point3i(77,158,186);
	int radio =20;
	Mat seg_dor = colorDistance(src,color,radio);
//	imshow("seg_dor",seg_dor);

//	segmentacionBGR_trackbar(src);
	
	//PARA LOS ANILLOS PLATEADOS
	color = Point3i(174,171,166);
	radio =20;
	Mat seg_pla = colorDistance(src,color,radio);
//	imshow("seg_pla",seg_pla);
	
//	segmentacionBGR_trackbar(src);
	Mat anillo_d=seg_dor.clone();
	threshold(seg_dor,anillo_d,1,255,CV_THRESH_BINARY);
//	imshow("thres",anillo_d);
	
	cvtColor(anillo_d,anillo_d,CV_BGR2GRAY);
	Mat e = getEE(1);
	erode(anillo_d,anillo_d,e);
//	erode(anillo_d,anillo_d,e);
	dilate(anillo_d,anillo_d,e);
//	imshow("morph",anillo_d);
	int cant_anillos_d = count_element(anillo_d,0);
	cout<<"la cantidad de anillos dorados son: "<<cant_anillos_d<<endl;
	
	
	
	//PLATEADOS
	Mat anillo_p=seg_pla.clone();
	threshold(seg_pla,anillo_p,1,255,CV_THRESH_BINARY);
//	imshow("thres",anillo_p);
	
	cvtColor(anillo_p,anillo_p,CV_BGR2GRAY);
	e = getEE(1);
	erode(anillo_p,anillo_p,e);
	//	erode(anillo_d,anillo_d,e);
	dilate(anillo_p,anillo_p,e);
//	imshow("morph",anillo_p);
	int cant_anillos_p = count_element(anillo_p,0);
	cout<<"la cantidad de anillos plateados son: "<<cant_anillos_p<<endl;
	cout<<"Cantidad total de anillos: "<<cant_anillos_d+cant_anillos_p<<endl;
	
	///PARTE COMPLEMENTARIA
	
	Mat img=src.clone();
//	imshow("img",img);
	cout<<endl<<endl<<endl;
//	info(img);
	img=img(Rect(0,200,img.cols,img.rows-200));
//	imshow("img",img);
	
	cvtColor(img,img,CV_BGR2GRAY);
	threshold(img,img,250,255,CV_THRESH_BINARY_INV);
	
	
//	imshow("bianrizada",img);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
	erode(img,img,e);
//	imshow("bianrizada",img);
	
	vector<Mat> v;
	Mat d,ero;
	dilate(img,d,e);
	erode(img,ero,e);
	v.push_back(d);v.push_back(ero);
	Mat r = Resta(v,1,0);
//	imshow("contorno",r);
	
//	detect_linesP_trackbar(r);
	vector<Vec4i> lines;
	cvtColor(r,r,CV_GRAY2BGR);
	Mat dst = r.clone();
	Mat con_lineas = r.clone();
	lines = detect_linesP(r, dst, 29, 16, 360, 100, 2500, 1);
//	for(size_t i = 0; i < lines.size(); ++i) {
//		Vec4i l = lines[i];
//		line(con_lineas, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
//	}
//	imshow("Original con lineas", con_lineas);
	
	Vec4i l = lines[0];
	double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI - 90;
//	cout<<"angulo: "<<angle<<endl;
	
	bool derecha;
	if((angle>0 && angle<90) || (angle<-90&&angle>-170)){
		cout<<"pulgar a la derecha"<<endl;
		derecha=true;
	}else{
		cout<<"pulgar a la izquierda"<<endl;
		derecha=false;
	}
	
	
	Mat img_grey = src.clone();
	cvtColor(img_grey,img_grey,CV_BGR2GRAY);
	threshold(img_grey,img_grey,250,555,CV_THRESH_BINARY_INV);
//	imshow("anillo_p",anillo_p);
	//imshow("bin",img_grey);
	//info(img_grey);
	
	int primero=0;
	int ultimo=0;
	for(int i=0;i<img_grey.cols;i++){
		int c = (int) img_grey.at<uchar>(200,i);
		if (c!=0 && primero==0){
			primero = i;
		}
		if (c!=0 && primero!=0){
			ultimo = i;
		}
	}
	
//	imshow("asfasd",anillo_d);
	vector<vector<Point>> vec_d = count_element(anillo_d);
	vector<vector<Point>> vec_p = count_element(anillo_p);
	
	int dedos_d[4]; dedos_d[0]=0; dedos_d[1]=0; dedos_d[2]=0; dedos_d[3]=0;
	int dedos_p[4]; dedos_p[0]=0; dedos_p[1]=0; dedos_p[2]=0; dedos_p[3]=0;
	
	
	for (int elem=0;elem<cant_anillos_d;elem++){
		int pos= vec_d[elem][0].x;
		int aux=(ultimo-primero)/4;
		if (pos>primero && pos<primero+aux){
			if (derecha){
				dedos_d[3]++;
			}else{
				dedos_d[0]++;
			}
		}
		if (pos>primero+aux && pos<primero+(2*aux)){
			if (derecha){
				dedos_d[2]++;
			}else{
				dedos_d[1]++;
			}
		}
		if (pos>primero+(2*aux) && pos<primero+(3*aux)){
			if (derecha){
				dedos_d[1]++;
			}else{
				dedos_d[2]++;
			}
		}
		if (pos>primero+(3*aux) && pos<ultimo){
			if (derecha){
				dedos_d[0]++;
			}else{
				dedos_d[3]++;
			}
		}
	}

	for (int elem=0;elem<cant_anillos_p;elem++){
		int pos= vec_p[elem][0].x;
		int aux=(ultimo-primero)/4;
		if (pos>primero && pos<primero+aux){
			if (derecha){
				dedos_p[3]++;
			}else{
				dedos_p[0]++;
			}
		}
		if (pos>primero+aux && pos<primero+(2*aux)){
			if (derecha){
				dedos_p[2]++;
			}else{
				dedos_p[1]++;
			}
		}
		if (pos>primero+(2*aux) && pos<primero+(3*aux)){
			if (derecha){
				dedos_p[1]++;
			}else{
				dedos_p[2]++;
			}
		}
		if (pos>primero+(3*aux) && pos<ultimo){
			if (derecha){
				dedos_p[0]++;
			}else{
				dedos_p[3]++;
			}
		}
	}
	
	

	
	for (int i=0;i<4;i++){
//		if (derecha){
//			cout<<"En el dedo "<<i+1<<" hay: "<<dedos_d[3-i]<<" dorados y "<<dedos_p[3-i]<<" plateados"<<endl;
//		}
//		else{
//			cout<<"En el dedo "<<i+1<<" hay: "<<dedos_d[i]<<" dorados y "<<dedos_p[i]<<" plateados"<<endl;
//		}
		cout<<"En el dedo "<<i+1<<" hay: "<<dedos_d[i]<<" dorados y "<<dedos_p[i]<<" plateados"<<endl;
	}
	
	
	
	
	
	
	
	waitKey(0);
	return 0;
} 
