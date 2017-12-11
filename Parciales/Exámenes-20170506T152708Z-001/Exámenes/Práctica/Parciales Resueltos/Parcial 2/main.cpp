#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "FuncionesMias.h"
#include <opencv2/gpu/gpu.hpp>
#include <string>


using namespace cv;
using namespace pdi;
using namespace std;

Point2d EsquinaSuperiorIzquierda(Mat img){
	Point2d p;
	
	for(int i = 0; i<img.rows;i++){
		for(int j = 0; j<img.cols;j++){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				p.x = i;
				p.y = j;
				return p;
	
			}	
		}
	}
}


Point2d EsquinaInferiorIzquierda(Mat img){
	Point2d p;
	
	for(int i = 0; i<img.rows;i++){
		for(int j = img.cols-1; j>0;j--){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				p.x = i;
				p.y = j;
				return p;
				
			}	
		}
	}
}

Point2d EsquinaSuperiorDerecha(Mat img){
	Point2d p;
	
	for(int i = img.rows-1; i>0;i--){
		for(int j = 0; j<img.cols;j++){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				p.x = i;
				p.y = j;
				return p;
				
			}	
		}
	}
}

Point2d EsquinaInferiorDerecha(Mat img){
	Point2d p;
	
	
	for(int i = img.rows-1; i>0;i--){
		for(int j = img.cols-1; j> 0;j--){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				p.x = i;
				p.y = j;
				return p;
				
			}	
		}
	}
}


Mat Recortar(Mat img){
	
	for(int i = 0; i<img.rows;i++){
		for(int j = 0; j<img.cols;j++){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0){
				Mat Subimagen = img(Rect(j,i,70,130));
				return	Subimagen;
			}	
		}
	}
}


vector<int> AcumuladorSuperior(Mat img){
	vector<int> v(img.cols,0);

	for(int i = 0; i<img.cols; i++){
		for(int j = 0; j< (int) img.rows/3;j++){
			int intensidad = (int) img.at<uchar>(j,i) ;
			if(intensidad == 0)
				v[i] +=1;
		}
	}
	return v;
}


vector<int> AcumuladorInferior(Mat img){
	vector<int> v(img.cols,0);
	for(int i = 0; i<img.cols; i++){
		for(int j = img.rows-1; j>(int)(img.rows-img.rows/5); j--){
			int intensidad = (int) img.at<uchar>(j,i) ;
			if(intensidad == 0)
				v[i] +=1;
		}
	}
	return v;
}

int CuentaFlechas(vector<int> v){
	vector<int> n;
	for(int i = 0; i<v.size(); i++){
		int j = i;
		while(i != v.size() && v[i] !=0)
			i = ++i;
		if( i-j != 0)
			n.push_back(1);
		
	}
	return n.size();			
	}


	


	
int main(int argc, char** argv) {
	
	
	
	///cargo la imagen
	Mat imagen = imread("3.jpg");
	imshow("Imagen",imagen);
	
	
	int Hmin = 20,
		Hmax = 40,
		Smin = 200,
		Smax = 255;
	
	Mat cartelAmarillo = Segmentar(imagen,Hmin,Hmax,Smin,Smax);
//	imshow("Cartel Amarillo",cartelAmarillo);
	
	
	///Convierto la imagen a escala de grises para luego Binarizar
	Mat cartelGris;
	cvtColor(cartelAmarillo,cartelGris,COLOR_BGR2GRAY);
//	imshow("Cartel escala Gris",cartelGris);
	
	///Binarizo la imagen
	Mat cartelBinarizado;
	threshold(cartelGris,cartelBinarizado,10,255,CV_THRESH_BINARY); //Hago 0 valores menores a 10
//	imshow("Cartel Binarizado",cartelBinarizado);
	
	
	Mat cartel = cartelBinarizado.clone(); //A partir de ahora trabajo con esta variable nomas
	
	///Defino un elemento estreucturante para trabajar con las operaciones morfologicas
	Mat ee = getStructuringElement(MORPH_RECT,Size(4,4)); //Defino el EE rectangular
	
	///Aplico Apertura para poder limpiar afuera del cartel
	morphologyEx(cartel,cartel,MORPH_OPEN,ee);
	///Aplico una Dilatacion. (encojo las flechas) 
	morphologyEx(cartel,cartel,MORPH_DILATE,ee); 
//	imshow("Apertura+Dilatacion",cartel);


	
	///Extraigo el cartel
	cartel =  Recortar(cartel);
	imshow("Recortado",cartel);
	
	
	///Acumulador
	vector<int> vs = AcumuladorSuperior(cartel);
	vector<int> vi = AcumuladorInferior(cartel);

	
	cout<< "\nSUPERIOR"<<endl;
	for(int i = 0 ; i< vs.size(); i++)
		cout<< vs[i]<<" ";
	cout<<"\nINFERIOR"<<endl;
	for(int i = 0 ; i< vi.size(); i++)
		cout<< vi[i]<<" ";
	
	
	///Para detectar las lineas a 45
	cartel = DetectarLineas(cartel,3);
	imshow("Lineas",cartel);
	
	cartel = DibujarHough(cartel,8);
	cartel = DetectarLineas(cartel,3);
	imshow("Hough",cartel);
	
	///Aplico una Dilatacion. (encojo las flechas) 
	morphologyEx(cartel,cartel,MORPH_DILATE,ee); 
	
	///Respuesta: 
	cout<<"\nReduccion de calzada de " <<CuentaFlechas(vi)<<" a "<<CuentaFlechas(vs);
	

	
	
	
	
	///Invierto para poner las lineas blancas
	//	Mat lut = LinealLUT(-1,255);
	//	cartel = AplicarLUT(cartel,lut);
	//	imshow("Invertido",cartel);
	


//	cout<< v.size();
//	cout<< "S I"<<EsquinaSuperiorIzquierda(cartel)<<endl ;
//	cout<< "S D"<<EsquinaSuperiorDerecha(cartel)<<endl ;
//	cout<< "I I"<<EsquinaInferiorIzquierda(cartel)<<endl ;	
//	cout<< "I D"<<EsquinaInferiorDerecha(cartel)<<endl ;
//	

	
	waitKey();
	
	
	
	return 0;
} 
