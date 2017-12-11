#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pdi_functions.h"
using namespace cv;
using namespace std;
using namespace pdi;


void Add_gaussian_Noise(Mat &src ,double mean,double sigma);
void Add_salt_pepper_Noise(Mat &srcArr, float pa, float pb);
void salPimienta_noise(Mat &src, double a, double b);
void draw_histograma(Mat src, string nombre);
void filtro_media_geometrica(Mat &src, int k_size);
void filtro_media_contraArmonica(Mat &src, float Q, int k_size);
double ECM2(Mat src1, Mat src2);
void filtro_mediana(Mat &src,int k_size);
void filtro_punto_medio(Mat &src, int k_size);
void filtro_mediaAlfa_recortado(Mat &src, int D,int k_size);
	
	
int main(int argc, char** argv) {
	///EJERCICIO 1
//	//formo una imagen con un cuadrado y un circulo
//	Mat src=Mat(Size(500,500),CV_8UC(1),Scalar(50,50,50));
//	rectangle(src,Point(0,0),Point(src.rows/2,src.cols/2),Scalar(100,100,100),-1);
//	circle(src,Point(src.rows/2,src.cols/2),src.cols/6,Scalar(180,180,180),-1);
//	imshow("Img Original",src);
//	
//	//Original+Ruido Gaussiano
//	Mat Gnoise=src.clone();
//	Add_gaussian_Noise(Gnoise,0,50);
//	imshow("Gaussian Noise", Gnoise);
//	draw_histograma(Gnoise,"Histograma Ruido Gaussiano");// Histograma con ruido Gaussiano
//	
//	//Original + Ruido Sal y Pimienta
//	Mat syp=src.clone();
//	Add_salt_pepper_Noise(syp,0.01,0.01);
//	imshow("Original + Sal y Pimienta",syp);
//	draw_histograma(syp,"Histograma Sal y Pimienta");//Histograma con Ruido sal y pimienta.
//	
//	waitKey(0);

	
	///EJERCICIO 2
	
//	Mat src = imread("sangre.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	namedWindow("Sangre",CV_WINDOW_AUTOSIZE);
//	imshow("Sangre",src);
//	Mat img_noise = src.clone();
//	
//	//Original + Ruido Gaussiano
//	Add_gaussian_Noise(img_noise,0,70);
//	namedWindow("Original + Ruido Gaussiano",CV_WINDOW_AUTOSIZE);
//	imshow("Original + Ruido Gaussiano", img_noise);
//	
//	//Original + Ruido Gaussiano + Ruido Sal y Pimienta
//	Add_salt_pepper_Noise(img_noise,0.005,0.05);
//	namedWindow("Original + Ruido Gaussiano + Sal y Pimienta",CV_WINDOW_AUTOSIZE);
//	imshow("Original + Ruido Gaussiano + Sal y Pimienta",img_noise);
//	
//	//Filtrado con Media Geometrica
//	Mat img_f = img_noise.clone();
//	filtro_media_geometrica(img_f,3);
//	namedWindow("Filtrada con Media Geometrica",CV_WINDOW_AUTOSIZE);
//	imshow("Filtrada con Media Geometrica",img_f);
//	
//	//Filtrado con Media Contra Armonica
//	Mat img_f2 = img_noise.clone();
//	filtro_media_contraArmonica(img_f2,0, 3);
//	namedWindow("Filtro Media Contra Armonica", CV_WINDOW_AUTOSIZE);
//	imshow("Filtro Media Contra Armonica",img_f2);
//	
//	//Comparo con ECM los errores entre la imagenes:
//	cout<<"ECM entre imagen Ruidosa vs Filtrada: "<<ECM2(img_noise, src)<<endl;
//	
//	cout<<"Filtro Media Contra Armonica: "<<endl;
//	cout<<"ECM entre imagen Original vs Filtrada: "<<ECM2(img_f2, src)<<endl;
//	
//	cout<<"Filtro Media Geometrica: "<<endl;
//	cout<<"ECM entre imagen Original vs Filtrada: "<<ECM2(img_f, src)<<endl;
//	
//	waitKey(0);
	///Ejercicio 3
	
	Mat src=imread("sangre.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Sangre",src);
	Mat img_noise=src.clone();
	
	//Original+Ruido Gaussiano
	Add_gaussian_Noise(img_noise,0,40);
	//Original + Ruido Sal y Pimienta
	Add_salt_pepper_Noise(img_noise,0.2,0.2);
	
	
	imshow("Original + Ruido Gaussiano + Sal y Pimienta",img_noise);
	
	//Filtro con la Mediana (filtro de orden)
	Mat img_fm=img_noise.clone();
	filtro_mediana(img_fm,3);
	imshow("Filtro Mediana",img_fm);
	
	
	//Filtro Punto medio
	Mat img_pm=img_noise.clone();
	filtro_punto_medio(img_pm, 3);
	imshow("Filtro Punto Medio",img_pm);
	
	
	//Filtro Media-Alfa Recortado
	Mat img_ma=img_noise.clone();
	int D=4,k_size=5;
	filtro_mediaAlfa_recortado(img_ma, D,k_size);
	namedWindow("Filtro Media-Alfa Recortado",CV_WINDOW_AUTOSIZE);
	imshow("Filtro Media-Alfa Recortado",img_ma);
	
	//Comparo Metodos
	cout<<"Ecm(img_Limpia, img_Ruidosa)= "<<ECM2(src,img_noise)<<endl;
	cout<<"Ecm(img_Limpia, img_Filtro_Mediana)= "<<ECM2(src,img_fm)<<endl;
	cout<<"Ecm(img_Limpia, img_Filtro_Pto_Medio)= "<<ECM2(src,img_pm)<<endl;
	cout<<"Ecm(img_Limpia, img_MediaAlfa_Recortado)= "<<ECM2(src,img_ma)<<endl;
	
	
	waitKey();
	return 0;
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
	//k_size: TamaÒo del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	
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
	//k_size: TamaÒo del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
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
	//k_size: TamaÒo del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	
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
	//k_size: TamaÒo del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
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
	//k_size: TamaÒo del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
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
