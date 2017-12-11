#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"

using namespace cv;
using namespace std;
using namespace pdi;

//Practica 1
void PerfilIntensidades( int event, int x, int y, int, void* var);
Mat Cuantizacion(Mat img, int bits, bool informacion = false );	
void BotellaLlena(int event, int x, int y, int, void* var);

//Practica 2
Mat LinealLUT(float a, float c);
Mat AplicarLUT(Mat imagen, Mat lut);
Mat UmbralLUT( int tipo, int p1, int p2, double Smax = 255);
Mat Transformacion3Tramos(Mat &img, Point p1,Point p2,Point p3,Point p4);
Mat LogLUT(int c);
Mat expLUT(float c, float gamma);
Mat Suma(vector<Mat> imgs);
Mat Resta(vector<Mat> imgs, int k, int escalado);
void obtenerROI(int x,int y, int nX, int nY, Mat &ROI);
Mat BitPlane(Mat img, int plane);
float ECM(Mat img, Mat img2);
vector<pair<int,int> > Blister(Mat imagen);
//Mat PersonalLUT(Point p1, Point p2,Point p3,Point p4);
//void MouseLUT( int event, int x, int y, int, void* var);


//Practica 3
Mat kernelPasaBajo(int n, int tipo);
Mat kernelPasaAlto(int n, int tipo);

//Practica 5
Mat mult_frecuencias(Mat img,Mat filtro);

Mat AltaPotenciaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW ,float A);
Mat EnfasisAltaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW, float a, float b);
Mat FiltroFrecuencialIdeal(Mat img, int tipo, float fc);
Mat FiltroFrecuencialButterworth(Mat img, int tipo, float fc, int orden);
Mat FiltroFrecuencialGaussian(Mat img, int tipo, float fc);
Mat FiltroHomomorfico(Mat img, float gh, float gl, float fc, float c);

//Practica 6
void Add_gaussian_Noise(Mat &src ,double mean,double sigma);
void Add_salt_pepper_Noise(Mat &srcArr, float pa, float pb );
void salPimienta_noise(Mat &src, double a, double b);
void draw_histograma(Mat src, string nombre);
void filtro_media_geometrica(Mat &src, int k_size);
void filtro_media_contraArmonica(Mat &src, float Q, int k_size);
double ECM2(Mat src1, Mat src2);
void filtro_mediana(Mat &src,int k_size);
void filtro_punto_medio(Mat &src, int k_size);
void filtro_mediaAlfa_recortado(Mat &src, int D,int k_size);

//Practica 7
Mat Segmentar(Mat img, int Hmin, int Hmax,int Smin, int Smax);
Mat Roberts(Mat img);
Mat Prewitt(Mat img, int tipo);


