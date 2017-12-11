#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FuncMias.h"
#include "pdi_functions.h"

using namespace cv;
using namespace pdi;
using namespace m;
using namespace std;



int EsDeCasa(Mat img){
	int q = 0;
	for(int i=0; i<img.rows; i++){
		for(int j=0; j<img.cols; j++){
			int p = (int)img.at<uchar>(i,j);
			if (p != 0){
				q = 1;
			}				
		}
	}
	return q;
}
int ContarMoscas(Mat img){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	return contours.size();
}

int main(int argc, char** argv) {
	Mat img = imread("Platos03.jpg");
	info(img);
	
	/** ***************************************** **/
	/** Segmento HSV solamente la sopa de la casa **/
	/** ***************************************** **/
	Mat imgSeg = SegmentacionHSV(img,20,50,100,200);
	Mat imgRect = imgSeg(Rect(500,500,50,50));
	
	
	/** ***************************************** **/
	/** Veo si es sopa de zapallo o de la casa    **/
	/** ***************************************** **/
	int p = EsDeCasa(imgRect);
	
	if(p == 1){ // es de la casa
		cout << "Es Sopa de la Casa\n";
		/** **************************************************** **/
		/** Cuento cantidad de moscas en la imagen de la casa    **/
		/** **************************************************** **/
			Mat imgEsc;
			cvtColor(img,imgEsc,COLOR_BGR2GRAY);
			Mat imgBin;
			threshold(imgEsc,imgBin,0,255,CV_THRESH_BINARY_INV);
			Mat ee = getStructuringElement(MORPH_RECT,Size(3,3));
			morphologyEx(imgBin,imgBin,MORPH_CLOSE,ee,Point(-1,-1),8);
			ee = getStructuringElement(MORPH_RECT,Size(2,2));
			morphologyEx(imgBin,imgBin,MORPH_ERODE,ee,Point(-1,-1),3);
			//Falla con una mosca en la imagen 01
			
			/// Find contours
			int c = ContarMoscas(imgBin);	
			cout << "Cantidad Moscas: " << c;
	
		/** **************************************************** **/
		/** Cuento cantidad de moscas en la sopa de la casa      **/
		/** **************************************************** **/
//			cvtColor(img,img,COLOR_GRAY2BGR);
			Mat imgSegSopa = SegmentacionHSV(img,0,100,100,255);
			Mat imgBinSopa;
			cvtColor(imgSegSopa,imgSegSopa,COLOR_BGR2GRAY);
			threshold(imgSegSopa,imgBinSopa,0,255,CV_THRESH_BINARY);
			
			Mat eeS = getStructuringElement(MORPH_RECT,Size(3,3));
			morphologyEx(imgBinSopa,imgBinSopa,MORPH_ERODE,eeS,Point(-1,-1),4);
			eeS = getStructuringElement(MORPH_RECT,Size(3,3));
			morphologyEx(imgBinSopa,imgBinSopa,MORPH_CLOSE,eeS,Point(-1,-1),9);
			
			int s = ContarMoscas(imgBinSopa);	
			cout << "\nCantidad Moscas en la sopa: " << s-1 ; // Resto uno xq el findcountourn me detecta la circunferencia
		/** **************************************************** **/
		/** Cuento cantidad de moscas fuera del plato de la casa     **/
		/** **************************************************** **/	
			Mat imgSegEsc = SegmentacionHSV(img,100,255,200,255);
			Mat imgBinEsc;
			cvtColor(imgSegEsc,imgSegEsc,COLOR_BGR2GRAY);
			threshold(imgSegEsc,imgBinEsc,0,255,CV_THRESH_BINARY);
			
			Mat eeE = getStructuringElement(MORPH_RECT,Size(3,3));
			morphologyEx(imgBinEsc,imgBinEsc,MORPH_ERODE,eeE,Point(-1,-1),4);
			eeE = getStructuringElement(MORPH_RECT,Size(3,3));
			morphologyEx(imgBinEsc,imgBinEsc,MORPH_CLOSE,eeE,Point(-1,-1),9);
			
			int e = ContarMoscas(imgBinEsc);	
			cout << "\nCantidad Moscas en el plato: " << c-e-(s-1) ;
			
			if ((s-1)>4){
				cout << "\n El plato de sopa de la casa esta MAL servido";
			}
			else{
				cout << "\n El plato de sopa de la casa esta BIEN servido";
			}		
		
	}else { // es de zapallo
		cout << "Es sopa de zapallo\n";
	
		/** ***************************************** **/
		/** Cuento cantidad de moscas en la imagen     **/
		/** ***************************************** **/
		Mat imgEsZ;
		cvtColor(img,imgEsZ,COLOR_BGR2GRAY);
		Mat imgBinEsZ;
		threshold(imgEsZ,imgBinEsZ,0,255,CV_THRESH_BINARY_INV);
		
		Mat eeEZ = getStructuringElement(MORPH_RECT,Size(3,3));
		morphologyEx(imgBinEsZ,imgBinEsZ,MORPH_CLOSE,eeEZ,Point(-1,-1),8);
		eeEZ = getStructuringElement(MORPH_RECT,Size(2,2));
		morphologyEx(imgBinEsZ,imgBinEsZ,MORPH_ERODE,eeEZ,Point(-1,-1),3);
//		Falla con una mosca en la imagen 01
		
		/// Find contours
		int c = ContarMoscas(imgBinEsZ);	
		cout << "Cantidad Moscas en la imagen: " << c;
	
	
		/** ************************************************* **/
		/** Cuento cantidad de moscas en la sopa zapallo      **/
		/** ************************************************* **/
		Mat imgSegSopaZ;
		imgSegSopaZ = SegmentacionHSV(img,1,90,50,2550);
		Mat imgBinSopaZ;
		cvtColor(imgSegSopaZ,imgSegSopaZ,COLOR_BGR2GRAY);
		threshold(imgSegSopaZ,imgBinSopaZ,0,255,CV_THRESH_BINARY);
	
		Mat eeSZ = getStructuringElement(MORPH_RECT,Size(3,3));
		morphologyEx(imgBinSopaZ,imgBinSopaZ,MORPH_ERODE,eeSZ,Point(-1,-1),4);
		eeSZ = getStructuringElement(MORPH_RECT,Size(6,6));
		morphologyEx(imgBinSopaZ,imgBinSopaZ,MORPH_CLOSE,eeSZ,Point(-1,-1),5);
//		
		int s = ContarMoscas(imgBinSopaZ);	
		cout << "\nCantidad Moscas en la sopa: " << s-1 ;
		
		/** **************************************************** **/
		/** Cuento cantidad de moscas fuera del plato de la casa     **/
		/** **************************************************** **/	
		Mat imgSegEsc = SegmentacionHSV(img,100,255,200,255);
		Mat imgBinEsc;
		cvtColor(imgSegEsc,imgSegEsc,COLOR_BGR2GRAY);
		threshold(imgSegEsc,imgBinEsc,0,255,CV_THRESH_BINARY_INV);
//		
		Mat eeE = getStructuringElement(MORPH_RECT,Size(3,3));
		morphologyEx(imgBinEsc,imgBinEsc,MORPH_ERODE,eeE,Point(-1,-1),3);
		eeE = getStructuringElement(MORPH_RECT,Size(3,3));
		morphologyEx(imgBinEsc,imgBinEsc,MORPH_CLOSE,eeE,Point(-1,-1),8);
//		
		int e = ContarMoscas(imgBinEsc);	
		cout << "\nCantidad Moscas en el plato: " << c-(e-1)-(s-1); // resto uno xq findcountorns me cuenta la circunferencia
		
		
		if ((s-1)>3){
			cout << "\n El plato de sopa de la casa esta MAL servido";
		}
		else{
			cout << "\n El plato de sopa de la casa esta BIEN servido";
		}

	}
	
	namedWindow("Imagen",0);
	imshow("Imagen",img);

	
	waitKey(0);
	return 0;
} 
