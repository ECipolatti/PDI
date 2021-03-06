#include <iostream>
#include <vector>
#include <iomanip>
#include "FuncionesMias.h"
#include "pdi_functions.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

using namespace pdi;

void PerfilIntensidades( int event, int x, int y, int, void* var)
{
	
	Mat *imagen = (Mat*)var;
	
	
	if( event==CV_EVENT_LBUTTONDOWN)   // Para una fila
	{
		
		Mat fila = imagen->row(x); // Tomo la fila de la coordenada X
		
		namedWindow("Perfil de Intensidades");
		Mat grafico(100,498,CV_8U); //Defino un canvas de 100x498 (la imagen es de 498)
		grafico.setTo(Scalar(0,0,0));
		draw_graph(grafico,fila);    //Paso el canvas y el vector de intensidades
		imshow("Perfil de Intensidades",grafico); //Muestro 
		
	}
	
	else if( event==CV_EVENT_RBUTTONDOWN) // Para una columna
	{
		
		Mat fila = imagen->col(y); // Tomo la fila de la coordenada X
		
		namedWindow("Perfil de Intensidades");
		Mat grafico(100,498,CV_8U); //Defino un canvas de 100x498 (la imagen es de 498)
		grafico.setTo(Scalar(0,0,0));
		draw_graph(grafico,fila);    //Paso el canvas y el vector de intensidades
		imshow("Perfil de Intensidades",grafico); //Muestro 
		
	}
	
}


Mat Cuantizacion(Mat imagen, int bits, bool informacion){
	
	
	int paso = 256/pow(2,bits);
	
	
	Mat nuevaImagen;
	imagen.copyTo(nuevaImagen);
	
	for(int i = 0; i<imagen.rows; i++){
		for(int j = 0; j<imagen.cols; j++){
			int intensidad = (int) imagen.at<uchar>(i,j);
			int nuevaIntensidad = (int) paso*(intensidad/paso);
			nuevaImagen.at<uchar>(i,j) = nuevaIntensidad;
			
		}
	}	
	
	if(informacion){
		info(imagen);
		
		cout<<"Paso:" <<paso<<endl;
		cout<<"Bits:"<<bits<<endl<<endl;
		Mat subImagen = imagen(Rect(100,150,10,10)); // Con Rect corto la porcion de la imagen.
		//		namedWindow("subimagen",1);
		//		imshow("subimagen",subImagen);
		
		Mat subImagen2 = nuevaImagen(Rect(100,150,10,10)); // Con Rect corto la porcion de la imagen.
		//		namedWindow("subimagen2",1);
		//		imshow("subimagen2",subImagen2);
		
		cout<<subImagen<<endl<<endl;
		cout<<subImagen2<<endl;
		
	}
	
	return nuevaImagen;
}	




void BotellaLlena(int event, int x, int y, int, void* var)
{
	//	cv::Mat *ptr = (cv::Mat*)var; 
	int x0,x1,y0,y1;
	bool f1 = false,
		f2 = false;
	
	//	cout << "Defnir tama�o de botella llena"<<endl;
	
	if(CV_EVENT_LBUTTONDOWN){
		cout <<"Coordenada 1"<<endl;
		x0 = x;
		y0 = y;
		f1 = true;
	}
	if(CV_EVENT_LBUTTONDOWN){
		cout <<"Coordenada 2"<<endl;
		x1 = x;
		y1 = y;
		f2 = true;
	}
	
}

// ***************************PRACTICA 2*******************************

Mat LinealLUT(float a, float c){
	
	Mat lut(1,256,CV_8U);
	
	for(int r = 0; r<lut.cols; r++){
		float s = a*r+c;
		if(s>255) s = 255;
		if(s<0) s = 0;
		lut.at<uchar>(0,r) =  (int)s;
	}
	return lut;
}


Mat UmbralLUT(  int tipo, int p1, int p2,double Smax){
	
	Mat lut(1,256,CV_8UC1);
	
	switch (tipo){
		//Binarizacion	
	case 1 : 
		for(int r = 0; r<lut.cols; r++){
			float s = r;
			if(s<=p1) s = 0;
			else
				if(s>p1) s = Smax;
			lut.at<uchar>(0,r) =  (int)s;
		}
		break;
		// Binarizacion con umbral invertido
	case 2 :  
		for(int r = 0; r<lut.cols; r++){
			float s = r;
			if(s<=p1) s = Smax;
			else
				if(s>p1) s = 0;
			lut.at<uchar>(0,r) =  (int)s;
		}
		break;
		// Intervalo Umbral
	case 3 :
		for(int r = 0; r<lut.cols; r++){
			float s = r;
			if(s<=p1 || s>=p2) s = Smax;
			else
				if(s>p1 && s<p2) s = 0;
			lut.at<uchar>(0,r) =  (int)s;
		}
		break;
		// Intervalo umbral invertido
	case 4 : 
		for(int r = 0; r<lut.cols; r++){
			float s = r;
			if(s<=p1 || s>=p2) s = 0;
			else
				if(s>p1 && s<p2) s = Smax;
			lut.at<uchar>(0,r) =  (int)s;
		}
		break;
		// Escala de grises	
	case 5 :
		for(int r = 0; r<lut.cols; r++){
			float s = r;
			if(s<=p1 || s>=p2) s = Smax;
			else
				if(s>p1 && s<p2) s = r;
			lut.at<uchar>(0,r) =  (int)s;
		}
		break;
		//Escala de grises invertido
	case 6 :
		for(int r = 0; r<lut.cols; r++){
			float s = r;
			if(s<=p1 || s>=p2) s = Smax;
			else
				if(s>p1 && s<p2) s = Smax-r;
			lut.at<uchar>(0,r) =  (int)s;
		}
		break;
	}
	
	return lut;
	
}


//Mat PersonalLUT(Point p1, Point p2,Point p3,Point p4){
//	Mat lut(1,256,CV_8UC1);
//	float a,c,s;
//	
//	for(int r = 0; r<p2.x; r++){
//		a = p2.y/p2.x;
//		c = 0;
//		s = a*r+c;
//		if(s>255) s = 255;
//		if(s<0) s = 0;
//		lut.at<uchar>(0,r) =  (int)s;
//		
//	}
//	
//	for(int r = p2.x; r<lut.cols; r++){
//		a = (p4.y-p2.y)/(p4.x-p2.x);
//		c = p2.y;
//		s = a*r+c;
//		if(s>255) s = 255;
//		if(s<0) s = 0;
//		lut.at<uchar>(0,r) =  (int)s;
//		
//	}
//	
//}
//
//
//void MouseLUT( int event, int x, int y, int, void* var)
//{
//	
//	
//	/*
//	Mat *r_s = (Mat*)var; 
//	
//	
//	Point p1(0,255),
//	p2(255,0),
//	p3(255,0),
//	p4(255,0);
//	
//	
//	r_s->setTo(Scalar(0,0,0));
//	
//	if (event == CV_EVENT_LBUTTONDOWN){
//	
//	cout<< "ClicL: (" << x<<","<<y<<")"<<endl;
//	
//	p2.x = x;
//	p2.y = y;	
//	line(*r_s, p1,p2,255,1);
//	line(*r_s, p2,p4,255,1);
//	line(*r_s, p3,p4,255,1);
//	imshow("Plano r-s",*r_s);
//	
//}
//	*/
//	vector<Mat> *v = (vector<Mat> *)var; 
//	Mat r_s = (*v)[0],
//		imagen = (*v)[1];
//	
//	
//	Point p1(0,255),
//		p2(255,0),
//		p3(255,0),
//		p4(255,0);
//	
//	
//	r_s.setTo(Scalar(0,0,0));
//	
//	if (event == CV_EVENT_LBUTTONDOWN){
//		
//		cout<< "ClicL: (" << x<<","<<y<<")"<<endl;
//		
//		p2.x = x;
//		p2.y = y;	
//		line(r_s, p1,p2,255,1);
//		line(r_s, p2,p4,255,1);
//		line(r_s, p3,p4,255,1);
//		imshow("Plano r-s",r_s);
//		
//		Mat lut(1,256,CV_8UC1,Scalar(0,0,0));
//		float a,c,s;
//		
//		for(int r = 0; r<p2.y; r++){
//			a = p2.y/(255-p2.x);
//			c = 0;
//			s = a*r+c;
//			if(s>255) s = 255;
//			if(s<0) s = 0;
//			lut.at<uchar>(0,r) =  (int)s;
//			
//		}
//		
//		for(int r = p2.x; r<lut.cols; r++){
//			a = (p4.y-p2.y)/(p4.x-p2.x);
//			c = p2.y;
//			s = a*r+c;
//			if(s>255) s = 255;
//			if(s<0) s = 0;
//			lut.at<uchar>(0,r) =  (int)s;
//			
//		}
//				
//		Mat imgModificada = AplicarLUT(imagen,lut);
//		namedWindow("Imagen Modificada");
//		imshow("Imagen Modificada",imgModificada);
//		imgModificada.setTo(Scalar(0,0,0));
//				
//	}
//	
//}



Mat Transformacion3Tramos(Mat &imagen, Point p1,Point p2,Point p3,Point p4){
	//Reacomodo la coordenada Y
	p1.y = 255-p1.y;
	p2.y = 255-p2.y;
	p3.y = 255-p3.y;
	p4.y = 255-p4.y;
	
	Mat img;
	imagen.copyTo(img);
	// sumo 1 al denominador para que no sea haga 0
	float a1 = (p2.y-p1.y)/(p2.x-p1.x+1); //Pendiente
	float c1 = p1.y-a1*p1.x; // ordenada al origen
	
	float a2 = (p3.y-p2.y)/(p3.x-p2.x+1); //Pendiente
	float c2 = p2.y-a2*p2.x; // ordenada al origen
	
	float a3 = (p4.y-p3.y)/(p4.x-p3.x+1); //Pendiente
	float c3 = p3.y-a3*p3.x; // ordenada al origen
	
	for(int i = 0; i<img.rows;i++){
		for(int j = 0; j<img.cols;j++){
			int r = (int) img.at<uchar>(i,j);
			int s;
			if(r < p2.x) {//Estoy en la primer recta
				s = a1*r+c1;
				if(s>255) s = 255;
				if(s<0) s = 0;
				img.at<uchar>(i,j) = (int)s;
			}
			else 
				if(r >= p2.x && r < p3.x ){ //Segunda recta
				s = a2*r+c2;
				if(s>255) s = 255;
				if(s<0) s = 0;
				img.at<uchar>(i,j) = (int)s;
			}
				else 
					if(r >= p3.x && r <= p4.x ){ //Tercer recta
				s = a3*r+c3;
				if(s>255) s = 255;
				if(s<0) s = 0;
				img.at<uchar>(i,j) = (int)s;
			}
		}
	}
	return img;
	
}


Mat LogLUT(int c){
	Mat lut(1,256,CV_8U);		
	for(int r = 0; r<lut.cols; r++){
		float s = c * log10(1+r);
		if(s>255) s = 255;
		if(s<0) s = 0;
		lut.at<uchar>(0,r) =  (int)s;
	}
	return lut;
}

Mat expLUT(float c, float gamma){
	Mat lut(1,256,CV_8U);		
	for(int r = 0; r<lut.cols; r++){
		float s = c * pow(r,gamma);
		if(s>255) s = 255;
		if(s<0) s = 0;
		lut.at<uchar>(0,r) =  (int)s;
	}
	return lut;
}


Mat AplicarLUT(Mat imagen, Mat lut){
	Mat img;
	imagen.copyTo(img);
	
	for(int i = 0; i<img.rows; i++){
		for(int j = 0; j< img.cols; j++){
			int pos = (int) img.at<uchar>(i,j);
			img.at<uchar>(i,j) = (int) lut.at<uchar>(0,pos);	
		}
	}
	
	
	return img;
}


Mat Suma(vector<Mat> imgs){
	int nImg = imgs.size();
	Mat imgFinal; 
	
	imgs[0].copyTo(imgFinal);
	
	for (int i = 1; i<nImg; i++){
		imgFinal += imgs[i];  
	}
	imgFinal = imgFinal/nImg;
	return imgFinal;
}

Mat Resta(vector<Mat> imgs, int k,int escalado){
	
	double min, max;
	int minInd, maxInd;
	
	int nImg = imgs.size();
	Mat imgFinal; 
	
	imgs[0].copyTo(imgFinal);
	
	for (int i = 1; i<nImg; i++){
		imgFinal -= imgs[i];  
	}
	
	
	if (escalado > 0){
		
//	// Esto es para que el valor quede entre 0-k
//	//Primer Opcion: Resto el minimo y reescalo a k = 255
	minMaxIdx(imgFinal, &min, &max, &minInd, &maxInd, Mat());  
	Mat fm;
	fm = imgFinal - min;
	minMaxIdx(fm, &min, &max, &minInd, &maxInd, Mat());  
	imgFinal = k*(fm/max);
	}
	else 
//	//Segunda Opcion: sumo k = 255 y divido por la cantidad
	imgFinal = (imgFinal + k)/nImg;
	
	return imgFinal;
}



void obtenerROI(int x,int y, int nX, int nY, Mat &ROI){ 
	// Toma el X creciente para abajo y el Y para la derecha
	//Recibe (x,y) y cantidad de puntos en cada direccion
	
	for(int i = x; i<x+nX; i++){
		for(int j = y; j<y+nY; j++){
			ROI.at<uchar>(i,j) = (int) 1;
		}
	}
}


Mat BitPlane(Mat img, int plane){
//	Mat img_out(256,256,CV_8U);
	Mat img_out(img.size(),img.type());
	int paso = pow(2,(plane-1));
	if (plane == 0){
		for(int i=0; i< img.rows; i++){
			for(int j=0; j<img.cols; j++){
				int intensidad = (int)img.at<uchar>(i,j);
				int mod = (intensidad % 2);
				img_out.at<uchar>(i,j) = mod*255;
				
				
			}
		}
	} else {
		for(int i=0; i< img.rows; i++){
			for(int j=0; j<img.cols; j++){
				int intensidad = (int)img.at<uchar>(i,j);
				int mod = ((int)(intensidad/paso)) % 2;
				img_out.at<uchar>(i,j)= mod*255;
			} 
		}
	}
	
	return img_out;
}

float ECM(Mat img, Mat img2){
	double ecm = 0.0;
	for(int i=0; i<img.rows; i++){
		for(int j=0; j<img.cols; j++){
			int e1 = (int)img.at<uchar>(i,j);
			int e2 = (int)img2.at<uchar>(i,j);
			ecm += pow((e1-e2),2);
		}
	}
	return ecm/(img.rows*img.cols);
}

vector<pair<int,int> > Blister(Mat imagen){
	////Binarizamos la imagen
	int tipo = 1,
		p1 = 90;
	
	Mat lut = UmbralLUT(tipo, p1, 0,255); 
	Mat img = AplicarLUT(imagen,lut); //Aplico el LUT a la imagen
	
	int x = 53,
		y = 53,
		d = 50;
//	cout<< "Ingrese la posicion de la pastilla superior izquierda: "<<endl;
//	cin>> x;
//	cin>> y;
	vector<pair<int,int> > v;
	pair<int,int> p;
	
	for(int i = 1; i<3 ; i++){
		int posX = x*i;
		for(int j = 1; j<5 ; j++){
			int posY = y*j;
			int r = (int) img.at<uchar>(posX,posY);
			if(r < 100){
				p.first = posX;
				p.second = posY;
				v.push_back(p);
			}
		}
	}
	
	
	return v;
	
}


Mat kernelPasaBajo(int n, int tipo){
	Mat K(n,n,CV_32FC1);
	switch (tipo){
	case 1:
		//promediado lleno
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				K.at<float>(i,j) = (float) 1/(n*n);
			}
		}
		break;
	case 2:
		//promediado  no lleno
		K.setTo(0);	
		for (int i=0;i<=((n-1)/2);i++){
			for(int j=((n-1)/2)-i; j<((n-1)/2)+1+i; j++){
				K.at<float>(i,j) = (float) 1; 
			}
		}
		for (int i=(n-1);i>((n-1)/2);i--){
			for(int j=((n-1)/2)-(n-i-1); j<((n-1)/2)+(n-i); j++){
				K.at<float>(i,j) = (float) 1; 
			}
		}			
		break;
	case 3:
		float valor;
		if(n == 3) valor = 0.125;
		if(n == 5) valor = 0.0625 ;
		if(n == 7) valor = 0.0208;
//		if(n == 9) valor = 0.00625;
		K.setTo(0);	
		
		for (int i=0;i<=((n-1)/2);i++){
			for(int j=((n-1)/2)-i; j<((n-1)/2)+1+i; j++){
				K.at<float>(i,j) = (float) valor; 
			}
		}
		for (int i=(n-1);i>((n-1)/2);i--){
			for(int j=((n-1)/2)-(n-i-1); j<((n-1)/2)+(n-i); j++){
				K.at<float>(i,j) =  (float) valor; 
			}
		}		
		K.at<float>((n-1)/2,(n-1)/2) = (float) 1/2; 
		break;
	}
	
	return K;
}

Mat kernelPasaAlto(int n, int tipo){
	Mat K(n,n,CV_32FC1);
	switch (tipo){
	case 1:
		//Suma cero
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				K.at<float>(i,j) = (float) -1;
			}
		}
		K.at<float>((n-1)/2,(n-1)/2) = (float) n*n-1;
		break;
	case 2: 
		//Suma uno : Matriz llena 
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				K.at<float>(i,j) = (float) -1;
			}
		}
		K.at<float>((n-1)/2,(n-1)/2) = (float) n*n;
		break;
	case 3:
		//Suma uno : Matriz con ceros
		float valor;
		if(n == 3) valor = 5;
		if(n == 5) valor = 13;
		if(n == 7) valor = 25;
		
		K.setTo(0);	
		
		for (int i=0;i<=((n-1)/2);i++){
			for(int j=((n-1)/2)-i; j<((n-1)/2)+1+i; j++){
				K.at<float>(i,j) = (float) -1; 
			}
		}
		for (int i=(n-1);i>((n-1)/2);i--){
			for(int j=((n-1)/2)-(n-i-1); j<((n-1)/2)+(n-i); j++){
				K.at<float>(i,j) = (float) -1; 
			}
		}			
		K.at<float>((n-1)/2,(n-1)/2) = (float) valor; 
		break;
	}
	
	return K;
}

//------------------PRACTICA 4-----------------------
Mat PseudoColor(Mat &imagen, vector<int> &color, vector<int> &rango){
	//vector con las 3 imagenes
	vector<cv::Mat>v(3);
	v[0] = imagen.clone();
	v[1] = imagen.clone();
	v[2] = imagen.clone();
	
	// c - Recorro y asigno colores
	//Los grises que elijo van a ir de 0 a 10 en intensidad
	//tengo los planos bgr y voy agregando 255 a G y a R por
	//cada vez que encuentre un gris entre ese rango en una posicion
	//determinada
	int min = rango[0],
		max = rango[1],
		B = color[0],
		G = color[1],
		R = color[2];
	
	for(int i=0;i<imagen.rows;i++){
		for(int j=0;j<imagen.cols;j++){
			int intensidad = (int) imagen.at<uchar>(i,j);
			if(intensidad >= min && intensidad<=max){
				v[0].at<uchar>(i,j) = B;
				v[1].at<uchar>(i,j) = G;
				v[2].at<uchar>(i,j) = R;
			}
		}
	}
	//Uno los canales
	Mat img_coloreada;
	merge(v,img_coloreada);
	return img_coloreada;
}

//-------------------PRACTICA 5-----------------------
//Recibe el filtro en frecuencia y la imagen
Mat mult_frecuencias(Mat img,Mat filtro){
	Mat salida,imgFFT;
	dft(img,imgFFT,DFT_COMPLEX_OUTPUT);
	mulSpectrums(imgFFT,filtro,salida,DFT_ROWS);
	idft(salida,salida,DFT_REAL_OUTPUT| DFT_SCALE);
	normalize(salida,salida,0,1,CV_MINMAX);
	return salida;
}


Mat AltaPotenciaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW ,float A){
	//HAP = (A-1) + HPA
	img.convertTo(img,CV_32F,1/255.f);
	img = optimum_size(img);
	Mat imgAPDF;
	switch(tipoPAfrec){
	case 1:{ //filtro frecuencial ideal
		Mat FIPB = filter_ideal(img.rows, img.cols, fc); //devuelve el filtro ideal pasa bajo en dom espacial
		Mat FIPA = 1 - FIPB; //obtengo el filtro ideal pasa alto en dom espacial
		FIPA = optimum_size(FIPA);
		imgAPDF = filter(img,FIPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	case 2: { //filtro frecuencial butterworth
			Mat FButterPB = filter_butterworth(img.rows, img.cols, fc, ordenBW); //devuelve el filtro butter pasa bajo en dom espacial
			Mat FButterPA = 1 - FButterPB; //obtengo el filtro butter pasa alto en dom espacial
			FButterPA = optimum_size(FButterPA);
			imgAPDF = filter(img,FButterPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
			break;
		}
	case 3: { //filtro frecuencial gaussiano
		Mat FGPB = filter_gaussian(img.rows, img.cols, fc); //devuelve el filtro gaussian pasa bajo en dom espacial
		Mat FGPA = 1 - FGPB; //obtengo el filtro gaussian pasa alto en dom espacial
		FGPA = optimum_size(FGPA);
		imgAPDF = filter(img,FGPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	}	
	return (A-1) + imgAPDF;
}
Mat EnfasisAltaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW, float a, float b){
	//HAP = (A-1) + HPA
	Mat imgAPDF;
	switch(tipoPAfrec){
	case 1:{ //filtro frecuencial ideal
		Mat FIPB = filter_ideal(img.rows, img.cols, fc); //devuelve el filtro ideal pasa bajo en dom espacial
		Mat FIPA = 1 - FIPB; //obtengo el filtro ideal pasa alto en dom espacial
		FIPA = optimum_size(FIPA);
		imgAPDF = filter(img,FIPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	case 2: { //filtro frecuencial butterworth
			Mat FButterPB = filter_butterworth(img.rows, img.cols, fc, ordenBW); //devuelve el filtro butter pasa bajo en dom espacial
			Mat FButterPA = 1 - FButterPB; //obtengo el filtro butter pasa alto en dom espacial
			FButterPA = optimum_size(FButterPA);
			imgAPDF = filter(img,FButterPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
			break;
		}
	case 3: { //filtro frecuencial gaussiano
		Mat FGPB = filter_gaussian(img.rows, img.cols, fc); //devuelve el filtro gaussian pasa bajo en dom espacial
		Mat FGPA = 1 - FGPB; //obtengo el filtro gaussian pasa alto en dom espacial
		FGPA = optimum_size(FGPA);
		imgAPDF = filter(img,FGPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
		break;
	}
	}	
	return a + b*imgAPDF;
}
Mat FiltroFrecuencialIdeal(Mat img, int tipo, float fc){
	Mat results;
	img.convertTo(img,CV_32F, 1/255.f);
	img = optimum_size(img);
	switch(tipo){
	case 1: { //filtro ideal frecuencial pasa bajo
		Mat FIPB = filter_ideal(img.rows,img.cols,fc);
		FIPB = optimum_size(FIPB);
		results = filter(img,FIPB);
		break;
	}
	case 2: { //filtro ideal frecuencial pasa alto
		Mat FIPB = filter_ideal(img.rows,img.cols,fc);
		Mat FIPA = 1 - FIPB;
		FIPA = optimum_size(FIPA);
		results = filter(img,FIPA);
	}
	}
	return results;
}
Mat FiltroFrecuencialButterworth(Mat img, int tipo, float fc, int orden){
	Mat results;
	img.convertTo(img,CV_32F, 1/255.f);
	img = optimum_size(img);
	switch(tipo){
	case 1: { //filtro butterworth frecuencial pasa bajo
		Mat FPB_BW = filter_butterworth(img.rows,img.cols,fc,orden);
		FPB_BW = optimum_size(FPB_BW);
		results = filter(img,FPB_BW);
		break;
	}
	case 2: { //filtro butterworth frecuencial pasa alto
		Mat FPB_BW = filter_butterworth(img.rows,img.cols,fc,orden);
		Mat FPA_BW = 1 - FPB_BW;
		FPA_BW = optimum_size(FPA_BW);
		results = filter(img,FPA_BW);
		break;
	}
	}
	return results;
}
Mat FiltroFrecuencialGaussian(Mat img, int tipo, float fc){
	Mat results;
	img.convertTo(img,CV_32F, 1/255.f);
	img = optimum_size(img);
	switch(tipo){
	case 1:{
		Mat FGPB = filter_gaussian(img.rows,img.cols,fc);
		FGPB = optimum_size(FGPB);
		results = filter(img,FGPB);
		break;
	}
	case 2:{
			Mat FGPB = filter_gaussian(img.rows,img.cols,fc);
			Mat FGPA = 1 - FGPB;
			FGPA = optimum_size(FGPA);
			results = filter(img,FGPA);	
			break;
		}
	}
	return results;
}
Mat FiltroHomomorfico(Mat img, float gh, float gl, float fc, float c){
	img.convertTo(img,CV_32F,1/255.f);	
	img = optimum_size(img);
	Mat H(img.size(),CV_32F,1/255.);
	Mat	results;
	Mat	imgLog;
	//Defino funcion H
	float dist;
	int centro_x=img.rows/2;
	int centro_y=img.cols/2;
	for(int x=0;x<img.rows;x++){
		for(int y=0;y<img.cols;y++){
			dist=sqrt(pow(x-centro_x,2)+pow(y-centro_y,2));
			H.at<float>(x,y)=(float)(gh-gl)*(1.0-exp(-c*(pow(dist,2)/pow(fc,2))))+gl;
		}
	}
	H = optimum_size(H);
	
	imgLog.convertTo(imgLog,CV_32F,1/255.f);
	imgLog = optimum_size(imgLog);
	
	log(img,imgLog);
	Mat filtrado = filter(imgLog,H);
	exp(filtrado,results);
	return results;
}


//-------------------PRACTICA 6-----------------------

//Agrega ruido gaussiano a la imagen de entrada, con media=u y varianza=v
void Add_gaussian_Noise(Mat &src ,double mean,double sigma)
{
	Mat Noise = src.clone();
	RNG rng;
	rng.fill(Noise, RNG::NORMAL, mean,sigma);
	add(src, Noise, src);
}

//Ruido Sal y Pimienta
void Add_salt_pepper_Noise(Mat &srcArr, float pa, float pb )
{    RNG rng; // rand number generate,
int amount1=srcArr.rows*srcArr.cols*pa;
int amount2=srcArr.rows*srcArr.cols*pb;
for(int counter=0; counter<amount1; ++counter)
{
	srcArr.at<uchar>(rng.uniform( 0,srcArr.rows), rng.uniform(0, srcArr.cols)) =0;
	
}
for (int counter=0; counter<amount2; ++counter)
{
	srcArr.at<uchar>(rng.uniform(0,srcArr.rows), rng.uniform(0,srcArr.cols)) = 255;
}
}

//Agrega ruido sal y pimienta a la imagen. a=umbral inferior para pimienta. b=umbral para sal
void salPimienta_noise(Mat &src, double a, double b){
	double s;
	
	for(int i=0;i<src.rows;i++){
		for(int j=0; j<src.cols;j++){
			s=rand()/RAND_MAX;
			
			if(s<a){
				src.at<uchar>(i,j)=0;
			}else{
				if( s>b ){
					src.at<uchar>(i,j)=255;
				}
			}
		}
	}
	
}

//Grafica histograma de la imagen de entrada
void draw_histograma(Mat src, string nombre){
	//nombre: Nombre de la ventana
	
	Mat hist=histogram(src,255);
	normalize(hist,hist,0,1,CV_MINMAX);
	
	Mat Canvas=Mat(src.size(),CV_8UC(1),Scalar(0,0,0));
	Canvas= draw_graph(Canvas,hist);
	
	namedWindow(nombre,CV_WINDOW_AUTOSIZE);
	imshow(nombre, Canvas);  // No se porque parece mas un ruido exponencial que  gaussiano
	
}

//Filtro de media Geometrica
void filtro_media_geometrica(Mat &src, int k_size){
	//k_size: Tama�o del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	
	int Nrows=src.rows;
	int Ncols=src.cols;
	int R=(k_size-1)/2;
	double mg;
	Mat tmp=src.clone();
	
	
	for(int i=1; i<Nrows-1; i++){
		for(int j=1; j<Ncols-1 ; j++){
			mg=1;
			for(int k=i-R; k<=i+R ; k++){  //Recorre el kernel para cada posicion x,y de la imagen
				for(int l=j-R; l<=j+R ; l++){
					mg=(double) mg*tmp.at<uchar>(k,l);
				}
			}
			src.at<uchar>(i,j)= (double) pow(mg, (float) 1/(k_size*k_size));;
		}
	}
}

void filtro_media_contraArmonica(Mat &src, float Q, int k_size){
	//k_size: Tama�o del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	//Q>0 -> Elimina Pimienta
	//Q<0 -> Elimina Sal
	//Q=0 -> Media Aritmetica
	//Q=-1 -> Media Armonica (Bueno para Sal, Malo para pimienta, Bien para ruido Gaussiano)
	
	int Nrows=src.rows;
	int Ncols=src.cols;
	int R=(k_size-1)/2;
	double mg;
	Mat tmp=src.clone();
	
	double Num, Den,Coc;
	
	for(int i=1; i<Nrows-1; i++){
		for(int j=1; j<Ncols-1 ; j++){
			Num=0;
			Den=0;
			for(int k=i-R; k<=i+R ; k++){  //Recorre el kernel para cada posicion x,y de la imagen
				for(int l=j-R; l<=j+R ; l++){
					Num+=pow((int) tmp.at<uchar>(k,l), Q+1);
					Den+=pow((int) tmp.at<uchar>(k,l), Q);
				}
			}
			
			Coc=Num/Den;
			src.at<uchar>(i,j)= (uchar) Coc ;
			
		}
		
		
	}
	
	
}

//Error Cuadratico Medio entre dos imagenes
double ECM2(Mat src1, Mat src2){
	
	int Nrows=src1.rows;
	int Ncols=src1.cols;
	double e=0;
	for(int i=0; i<Nrows; i++){
		for(int j=0; j<Ncols; j++){
			
			e+=pow((int) (src1.at<uchar>(i,j)-src2.at<uchar>(i,j)),2 );
		}
	}
	return e/(Nrows*Ncols);
}


void filtro_mediana(Mat &src,int k_size){
	//k_size: Tama�o del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	
	int Nrows=src.rows;
	int Ncols=src.cols;
	int R=(k_size-1)/2;
	int Mid=(k_size*k_size-1)/2;
	Mat tmp=src.clone();
	
	
	vector <int> V;
	for(int i=1; i<Nrows-1; i++){
		for(int j=1; j<Ncols-1 ; j++){
			
			for(int k=i-R; k<=i+R ; k++){  //Recorre el kernel para cada posicion x,y de la imagen
				for(int l=j-R; l<=j+R ; l++){
					V.push_back((int) tmp.at<uchar>(k,l));
					
				}
			}
			
			sort(V.begin(),V.end());
			src.at<uchar>(i,j)= (uchar) V[Mid];
			V.clear();
		}
	}
}

void filtro_punto_medio(Mat &src, int k_size){
	//k_size: Tama�o del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	//Punto Medio: f(x,y)=1/2*(min+max);
	int Nrows=src.rows;
	int Ncols=src.cols;
	int R=(k_size-1)/2;
	int Mid=(k_size*k_size-1)/2;
	Mat tmp=src.clone();
	
	double min_,max_;
	Mat S;
	for(int i=R; i<Nrows-R; i++){
		for(int j=R; j<Ncols-R ; j++){
			S=tmp(Range(i-R,i+R),Range(j-R,j+R));
			minMaxLoc(S,&min_,&max_);
			src.at<uchar>(i,j)= (uchar (int (0.5*(min_+max_))));
		}
	}
	
}

//Filtro Media-Alfa Recortado
void filtro_mediaAlfa_recortado(Mat &src, int D,int k_size){
	//k_size: Tama�o del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	//Filtro medAlfa Recortado: f(x,y)=[1/(m*n-D)]*sum;
	//D : Cantidad de Elementos que descarto de cada kernel (que sea par !!)
	
	
	int
		Nrows=src.rows,
		Ncols=src.cols,
		R=(k_size-1)/2,
		Mid=(k_size*k_size-1)/2;
	if(D>=Mid){cout<<"Elija D mas pequeno"; return;}
	Mat tmp=src.clone();
	vector <int> V;
	double aux;
	
	for(int i=2; i<Nrows-2; i++){
		for(int j=2; j<Ncols-2 ; j++){
			
			for(int k=i-R; k<=i+R ; k++){  //Recorre el kernel para cada posicion x,y de la imagen
				for(int l=j-R; l<=j+R ; l++){
					V.push_back((int) tmp.at<uchar>(k,l));
				}
			}
			
			sort(V.begin(),V.end());
			aux=0;
			for (int m=Mid-D/2; m<=Mid+D/2; m++){
				aux+=V[m];	}
			aux=aux/(k_size*k_size-D);
			
			
			src.at<uchar>(i,j)= (int) V[Mid];
			V.clear();
		}
		
	}
	
	
}

///******************Practica 7********************

Mat Segmentar(Mat imagen, int Hmin, int Hmax,int Smin, int Smax){
	//Variables
	Mat mascara = cv::Mat::zeros(imagen.size(),imagen.type());
	vector<Mat> hsv_channels(3);
	Mat imgHSV;
	//Paso de RGB a HSI
	cvtColor(imagen,imgHSV,COLOR_BGR2HSV);
	//Divido los canales
	split(imgHSV,hsv_channels);
		
	
	///Recorro las componentes H y S, entonces si estos valores est�n dentro de 
	///los minimos y maximos definidos anteriormente a la mascara le coloco 1,
	///sino queda en cero. Entonces en otras palabras, si encuentra el amarillo
	/// a la mascara la dejo en 1, de lo contrario 0.
	for (int i = 0; i<imagen.rows;i++)
		for (int j = 0; j<imagen.cols;j++){
		int H = (int)hsv_channels[0].at<uchar>(i,j);
		int S = (int)hsv_channels[1].at<uchar>(i,j);
		if ((Hmin<H)&&(H<Hmax) && (Smin<S)&&(S<Smax)){
			mascara.at<cv::Vec3b>(i,j)[0]=1; //Blue
			mascara.at<cv::Vec3b>(i,j)[1]=1; //Green
			mascara.at<cv::Vec3b>(i,j)[2]=1; //Red
		}
	}
	
	///Multiplico la imagen con la mascara y asi obtengo solo el cartel
	Mat imgSegmentada;
	multiply(imagen,mascara,imgSegmentada); //Multiplico la imagen por la mascara

	return imgSegmentada;
}



Mat Roberts(Mat img){
	
	Mat kernel = Mat::eye( 2, 2, CV_32FC1 );
	Point anchor = Point( 0,0 );
	*(((float*) (kernel.data))) = 0.0;  // [0][0] 
	*(((float*) (kernel.data))+1) = 1.0;  // [0][1] 
	*(((float*) (kernel.data))+2) = -1.0;  // [1][0] 
	*(((float*) (kernel.data))+3) = 0.0;  // [1][1] 
	Mat roberts;
	filter2D(img, roberts, -1, kernel);
	return roberts;
}

Mat Prewitt(Mat img, int tipo){
	
	//Tipo: 0 = Vertical
	//		1 = Horizontal
	//		2 = Diagnoal /
	//		3 = Diagonal \
	
		Mat Prewitt1;
		Mat kernel = Mat::eye( 3, 3, CV_32FC1 );
	switch(tipo){
	case 0 : //Vertical
		
		*(((float*) (kernel.data))) = -1;  // [0][0] 
		*(((float*) (kernel.data))+1) = 0;  // [0][1] 
		*(((float*) (kernel.data))+2) = 1.0;  // [0][2] 
		*(((float*) (kernel.data))+3) = -1.0; //[1][0]
		*(((float*) (kernel.data))+4) = 0.0;// [1][1]
		*(((float*) (kernel.data))+5) = 1.0;// [1][2]
		*(((float*) (kernel.data))+6) = -1.0;// [2][0]
		*(((float*) (kernel.data))+7) = 0.0;// [2][1]
		*(((float*) (kernel.data))+8) = 1.0;// [2][2]
	
		filter2D(img, Prewitt1, -1, kernel);
	break;
	case 1 : //Horizontal
		
		
		*(((float*) (kernel.data))) = -1.0;  // [0][0] 
		*(((float*) (kernel.data))+1) = -1.0;  // [0][1] 
		*(((float*) (kernel.data))+2) = -1.0;  // [0][2] 
		*(((float*) (kernel.data))+3) = 0.0; //[1][0]
		*(((float*) (kernel.data))+4) = 0.0;// [1][1]
		*(((float*) (kernel.data))+5) = 0.0;// [1][2]
		*(((float*) (kernel.data))+6) = 1.0;// [2][0]
		*(((float*) (kernel.data))+7) = 1.0;// [2][1]
		*(((float*) (kernel.data))+8) = 1.0;// [2][2]
		
		filter2D(img, Prewitt1, -1, kernel);
		return Prewitt1;
	case 2 : //Diagonal /
		
		
		*(((float*) (kernel.data))) = 0.0;  // [0][0] 
		*(((float*) (kernel.data))+1) = 1.0;  // [0][1] 
		*(((float*) (kernel.data))+2) = 1.0;  // [0][2] 
		*(((float*) (kernel.data))+3) = -1.0; //[1][0]
		*(((float*) (kernel.data))+4) = 0.0;// [1][1]
		*(((float*) (kernel.data))+5) = 1.0;// [1][2]
		*(((float*) (kernel.data))+6) = -1.0;// [2][0]
		*(((float*) (kernel.data))+7) = -1.0;// [2][1]
		*(((float*) (kernel.data))+8) = 0.0;// [2][2]
	
		filter2D(img, Prewitt1, -1, kernel);
		return Prewitt1;
	case 3 : //Diagonal \
		
		
		*(((float*) (kernel.data))) = -1.0;  // [0][0] 
		*(((float*) (kernel.data))+1) = -1.0;  // [0][1] 
		*(((float*) (kernel.data))+2) = 0;  // [0][2] 
		*(((float*) (kernel.data))+3) = -1.0; //[1][0]
		*(((float*) (kernel.data))+4) = 0.0;// [1][1]
		*(((float*) (kernel.data))+5) = 1.0;// [1][2]
		*(((float*) (kernel.data))+6) = 0.0;// [2][0]
		*(((float*) (kernel.data))+7) = 1.0;// [2][1]
		*(((float*) (kernel.data))+8) = 1.0;// [2][2]

		filter2D(img, Prewitt1, -1, kernel);
		return Prewitt1;
	}
	
}

Mat DibujarHough(Mat img, double nLineas){
	//Canny para detectar los bordes de la imagen
	//cvtColor para cambiar la imagen de escala de grises a BGR y poder mostrar los resultados.
	// nLineas = cantidad minima de lineas a detectar
	Mat dst, 
		cdst(img.size(),CV_8UC1);
	cdst.setTo(0);
	Canny(img, dst, 50, 200, 3);
//	cvtColor(dst, cdst, CV_GRAY2BGR);
	

	double	tita = 1,
			ro = CV_PI/180; // = 1
//			nlineas = 50; //cantidad minima de lineas a detectar
	
	vector<Vec2f> lines;
	HoughLines(dst, lines,tita,ro, nLineas, 0, 0 );
	
	for ( size_t i = 0; i < lines.size(); i++ ) {
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
//		line( cdst, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
		line( cdst, pt1, pt2, 255, 1);

	}
	
	return cdst;
}




Mat DetectarLineas(Mat img, int tipo){
	
	//Tipo: 0 = Vertical
	//		1 = Horizontal
	//		2 = Diagnoal /
	//		3 = Diagonal \
	
	Mat Prewitt1;
	Mat kernel = Mat::eye( 3, 3, CV_32FC1 );
	switch(tipo){
	case 0 : //Vertical
		
		*(((float*) (kernel.data))) = -1;  // [0][0] 
		*(((float*) (kernel.data))+1) = 2;  // [0][1] 
		*(((float*) (kernel.data))+2) = -1.0;  // [0][2] 
		*(((float*) (kernel.data))+3) = -1.0; //[1][0]
		*(((float*) (kernel.data))+4) = 2.0;// [1][1]
		*(((float*) (kernel.data))+5) = -1.0;// [1][2]
		*(((float*) (kernel.data))+6) = -1.0;// [2][0]
		*(((float*) (kernel.data))+7) = 2.0;// [2][1]
		*(((float*) (kernel.data))+8) = -1.0;// [2][2]
		
		filter2D(img, Prewitt1, -1, kernel);
		break;
	case 1 : //Horizontal
		
		
		*(((float*) (kernel.data))) = -1.0;  // [0][0] 
		*(((float*) (kernel.data))+1) = -1.0;  // [0][1] 
		*(((float*) (kernel.data))+2) = -1.0;  // [0][2] 
		*(((float*) (kernel.data))+3) = 2.0; //[1][0]
		*(((float*) (kernel.data))+4) = 2.0;// [1][1]
		*(((float*) (kernel.data))+5) = 2.0;// [1][2]
		*(((float*) (kernel.data))+6) = -1.0;// [2][0]
		*(((float*) (kernel.data))+7) = -1.0;// [2][1]
		*(((float*) (kernel.data))+8) = -1.0;// [2][2]
		
		filter2D(img, Prewitt1, -1, kernel);
		return Prewitt1;
	case 2 : //Diagonal /
		
		
		*(((float*) (kernel.data))) = -1.0;  // [0][0] 
		*(((float*) (kernel.data))+1) = -1.0;  // [0][1] 
		*(((float*) (kernel.data))+2) = 2.0;  // [0][2] 
		*(((float*) (kernel.data))+3) = -1.0; //[1][0]
		*(((float*) (kernel.data))+4) = 2.0;// [1][1]
		*(((float*) (kernel.data))+5) = -1.0;// [1][2]
		*(((float*) (kernel.data))+6) = 2.0;// [2][0]
		*(((float*) (kernel.data))+7) = -1.0;// [2][1]
		*(((float*) (kernel.data))+8) = -1.0;// [2][2]
		
		filter2D(img, Prewitt1, -1, kernel);
		return Prewitt1;
	case 3 : //Diagonal \
		
		
		*(((float*) (kernel.data))) = 2.0;  // [0][0] 
		*(((float*) (kernel.data))+1) = -1.0;  // [0][1] 
		*(((float*) (kernel.data))+2) = -1.0;  // [0][2] 
		*(((float*) (kernel.data))+3) = -1.0; //[1][0]
		*(((float*) (kernel.data))+4) = 2.0;// [1][1]
		*(((float*) (kernel.data))+5) = -1.0;// [1][2]
		*(((float*) (kernel.data))+6) = -1.0;// [2][0]
		*(((float*) (kernel.data))+7) = -1.0;// [2][1]
		*(((float*) (kernel.data))+8) = 2.0;// [2][2]
		
		filter2D(img, Prewitt1, -1, kernel);
		return Prewitt1;
	}
	
}

