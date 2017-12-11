#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FuncionesMias.h"

using namespace cv;


bool EsBlanco(Mat img){
	bool q = 0;
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

int BordeCopa(Mat img){
	for(int i = 0 ; i<img.rows; i++){
		for(int j = 0 ; j<img.cols; j++){
			int intensidad = (int) img.at<uchar>(i,j);
			if(intensidad != 0) 
				return i;
		}
	}
}



int FondoCopa(Mat img){
	for(int i = img.rows-50; i>0; i--){
		for(int j = 0; j<img.cols; j++){
			int k = 0;
			while((int) img.at<uchar>(i,j) != 0 && j != img.cols){
				k += 1;
				j++;
			}
			if(k > 50)
				return i;
		}
	}
}

bool Verifica(Mat img, int Porcentaje){
	
	int intensidad,
		n = 0;
	
	for(int i = img.rows-1; i >0; i--){
		intensidad = (int) img.at<uchar>(i,40); //con la coordenada 'Y' me posiciono en el centro
		if(intensidad != 0)
			n += 1;
	}
	
	float total = n*100/img.rows;
	cout<<total<<"% llena"<<endl;
	if(total> Porcentaje)
		return false;
	else
		return true;
}

	


int main(int argc, char** argv) {

	Mat imagen = imread("4.jpg",IMREAD_COLOR);
	imshow("Imagen",imagen);
	
//	cout<<imagen.size();
	
	////Segmento blanco  nada mas, si no es blanco entonces segmento tinto
	Mat vino;
	///Vino Blanco
	int Hmin = 20,
		Hmax = 50,
		Smin = 100,
		Smax = 200;
		vino =  Segmentar(imagen, Hmin,Hmax,Smin,Smax);
		
	if(EsBlanco(vino))//Si no es blanco entonces segmento el tinto
	{
		imshow("Blaco Blanco", vino);
		///Binarizo (invertido para que el fondo sea negro)
		Mat imagenGris;
		cvtColor(imagen,imagenGris,COLOR_BGR2GRAY);
		threshold(imagenGris,imagenGris,200,255,CV_THRESH_BINARY_INV);
		///Aplico dilatacion, para que el cuello de la copa no tenga huecos
		Mat ee = getStructuringElement(MORPH_RECT,Size(5,5)); //Defino el EE rectangular
		morphologyEx(imagenGris,imagenGris,MORPH_DILATE,ee);
		morphologyEx(imagenGris,imagenGris,MORPH_DILATE,ee);
		morphologyEx(imagenGris,imagenGris,MORPH_DILATE,ee);

		int borde = BordeCopa(imagenGris);
		int cuello = FondoCopa(imagenGris);
		
		Mat imgRecortada = imagenGris(Rect(0,borde,(imagen.cols),cuello-borde));
		imshow("Cortada", imgRecortada);
		
		if(Verifica(imgRecortada,30))
			cout<< "La copa de Blanco esta bien servida";
		else
			cout<< "La copa de Blanco esta mal servida";
	}
	else
	{
		int Hmin = 0,
			Hmax = 40,
			Smin = 100,
			Smax = 200;
		vino =  Segmentar(imagen, Hmin,Hmax,Smin,Smax);
		imshow("Blaco Tinto", vino);
		
		///Binarizo (invertido para que el fondo sea negro)
		Mat imagenGris;
		cvtColor(imagen,imagenGris,COLOR_BGR2GRAY);
		threshold(imagenGris,imagenGris,200,255,CV_THRESH_BINARY_INV);
		///Aplico dilatacion, para que el cuello de la copa no tenga huecos
		Mat ee = getStructuringElement(MORPH_RECT,Size(5,5)); //Defino el EE rectangular
		morphologyEx(imagenGris,imagenGris,MORPH_DILATE,ee);
		morphologyEx(imagenGris,imagenGris,MORPH_DILATE,ee);
		morphologyEx(imagenGris,imagenGris,MORPH_DILATE,ee);

//		imshow("Vino", imagenGris);

		int borde = BordeCopa(imagenGris);
		int cuello = FondoCopa(imagenGris);

		Mat imgRecortada = imagenGris(Rect(0,borde,imagen.cols,cuello-borde));
		imshow("Cortada", imgRecortada);

		if(Verifica(imgRecortada,50))
			cout<< "La copa de Tinto esta bien servida";
		else
			cout<< "La copa de Tinto esta mal servida";
	}
	waitKey();
	return 0;
} 
