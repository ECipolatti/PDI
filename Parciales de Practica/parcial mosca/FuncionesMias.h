#include <iostream>
#include <vector>
#include <iomanip>
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

//Practica 4
Mat PseudoColor(Mat &imagen, vector<int> &color, vector<int> &rango);

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

Mat DibujarHough(Mat img,double nLineas);
Mat DetectarLineas(Mat img, int tipo);

///MARIO
void bbw();

/// Para cargar las imagenes suponiendo que se encuentran en el directorio "../imgs/"
Mat loadimg(string file, int type, bool display, int autosize);


/// LUT lineal de la forma s = a*r + c. Funciona para tipo de dato CV_8UC1.
Mat linear_lut(float a, float c);

/// LUT para imagen en negativo. Funciona para tipo de dato CV_8UC1.
Mat negative_lut();


/// LUT por tramos definido por 3 pares de datos (a,c). Funciona para tipo de dato CV_8UC1.
Mat piecewise_lut(int p1, int p2, float a1, float c1, float a2, float c2, float a3, float c3, bool inv);

/// captura un punto con el click izquierdo y lo guarda en un vector de puntos
void getPoint( int event, int x, int y, int, void* var);

/// realiza un lut por tramos a partir de dos clicks
Mat clickLUT();

/// LUT logarítmico
Mat logLUT(float c);


/// crea una máscara binaria rectangular en la posición indicada
Mat binMask(int height, int width, int x1, int y1, int x2, int y2);


/// Suma de imágenes tipo CV_32FC1 contenidas en el vector v
Mat mat_sum(vector<Mat> v);

/// Suma de imágenes tipo CV_8UC1 contenidas en el vector v
Mat mat_sumf(vector<Mat> v);



/// Funcion para crear mscaras de filtros de promediado lleno y no lleno
Mat kernelPromediado(int n, int tipo);

/// Aplica el Kernel Pasa-Alto de Suma 1 o 0, tanto Llenos como NO Llenos
Mat filtroPromediado(Mat src, int n, int type);



/// Aplica el Kernel Pasa-Alto de Suma 1 o 0, tanto Llenos como NO Llenos
Mat filtroPasaAlto(Mat src, int n, int type);


/// Para sumar y restar imagenes
Mat arithm_transf(const Mat &img1, const Mat &img2, int op);

/// Aplica a la Imagen src un Filtro Gaussiano de nxn, con desvio Sigma y devuelve la imagen resultante en dst
Mat gaussianFilter(Mat src, int n, float sigma);

/// Devuelve La imagen y su Histograma
void imageHist(Mat src);

// Calcula el histograma de una imagen a color, muestra las 3 componentes y las devuelve en un vector<Mat>
vector<Mat> colorHistogram(Mat src, bool graph);

// Devuelve una imagen en pseudo-color a partir de una imagen en escala de grises. Modifica todos los pixeles en el rango
// [a,b] con las componentes de color (R,G,B) (mejor usar inRange())
Mat pseudoColor(Mat src, int a, int b, int R, int G, int B);

/// Nuestro Espectrum (para visualizar el espectro de Transformada Discreta de Fourier)
Mat my_spectrum(Mat I);

/// Devuelve la imagen (en escala de grises) y su espectro de Fourier
void imageSpec(Mat src);

/// Filtrado frecuencial gaussiano
Mat FiltroFrecuencialGaussiano(Mat img, int tipo, float fc);


/// Modelos de ruido
Mat noise_impulsive(Mat src, uchar a, uchar b, float pa, float pb);

Mat noise_uniform(Mat src, float min, float max);

Mat noise_gaussian(Mat src, float mean, float var);

Mat noise_gamma(Mat src, float a, float b);

Mat noise_exponential(Mat src, float lambda);

///Filtros de Medias
Mat filtro_geometrico(Mat src,int msize);

Mat filtro_contra_armonico(Mat src,int msize, float Q);




Mat filtro_alfa_rec(Mat src,int msize, int d);

Mat filtro_notch_butterworth(size_t rows, size_t cols, double v[][4],int filas);

Mat edge_detection(Mat src, int method, bool display);

/// Convierte y devuelve una imagen binarizada utilizando el umbral definido por 'thres'
/// Con 'display' se elige si se muestra o no la imagen en pantalla
Mat bin_img(Mat src, float thres, bool display);

Mat rotateMat(Mat src, float deg);

/// Funcion para Segmentar por Regiones Tp 07 - Ej 03
Mat Seg_CR(Mat img, Point p, Scalar L, Scalar H);

/// Algoritmos de morfología binaria
Mat Morfologia(Mat src, Mat EE, char oper);

/// Devuelve elementos estructurantes de 3x3 según los vistos en los distintos problemas
Mat getEE(int op);

float porcentaje(Mat src, int color, bool msj);

/// Funciones de Segmentación
// Color Distance: dada una imagen src busca los pixceles de color "color" con un radio dado - Devuelve la
// imagen segmentada - SEGMENTACION: B G R - Point31 color(B,G,R)
Mat colorDistance(Mat src, Point3i color, int radio);

/// Segmentación BGR utilizando un trackbar. Al finalizar devuelve por consola las líneas que habría
/// que poner en el main() para reproducir lo mismo utilizando la función colorDistance definida arriba
/// y poder hacer la segmentación de forma automática, sin necesidad de clicks.
void colorDistance();

// Funcion que toma el color del punto clickeado
void seg_bgr_mouse_callback(int event, int x, int y, int flags, void*);

// Controlo la barra para mover los planos de color a través de la función 'colorDistance'
void seg_bgr_trackbar(int, void*);

void segmentacionBGR_trackbar(Mat src);
void segmentacionBGR_trackbar(string file);
/// *-*-*-*-*-*-*-*-*-* Fin de la Segmentación BGR. *-*-*-*-*-*-*-*-*-*-*

// Segmentacion en HSV: dada una imagen se le pasa los rango de cada canal
Mat SegmentacionHSV(Mat img, int Hmin, int Hmax, int Smin, int Smax, int Vmin, int Vmax);


// Informacion en canales RGB y HSV
vector<Mat> canales(string file, bool disp_bgr, bool disp_hsv);

//Contar elementos en una imagen
//img es una imagen binarizada
int count_element(Mat img, bool msj);

/// Devuelve los contornos de una imagen binarizada
vector<vector<Point>> count_element(Mat img);

/// Devuelve los centros de masa de un vector de contornos
vector<Point2f> get_mass_centers(vector<vector<Point>> contours);

/// Detecta líneas en un rango de ángulos deseado utilizando el algoritmo de Hough estándar
vector<Vec2f> detect_lines(const Mat &src, Mat & dst, int threshold, int range_a, int range_b);

/// Detecta líneas en un rango de ángulos deseado utilizando el algoritmo de Hough probabilístico
vector<Vec4i> detect_linesP(const Mat &src, Mat &dst, int threshold, int range_a, int range_b,
							int minLineLength, int maxLineGap, int mode);
//int minLineLength = 50, int maxLineGap = 10, int mode = 0)

void houghp_trackbar(int, void*);

void detect_linesP_trackbar(Mat src);

///Devuelve las areas.
//Devuelve el valor de las areas ordenadas Mayor -> Menor
vector<float> get_areas(vector<vector<Point>> contours);
////Devuelve los indices de las areas ordenadas Mayor -> Menor
vector<int> get_idx_areas(vector<vector<Point>> contours);

/// Pinta las areas definidas en contours con un determinado color
Mat paint_contour_area(Mat src, vector<vector<Point>> contours, int idx, Scalar color);

///Scan Lines
/// Horizontal
vector<Point> scanLine_Horizontal(Mat img, int altura, uchar color, bool lado, bool linea);

/// Vertical
vector<Point> scanLine_Vertical(Mat img, int altura, uchar color, bool lado, bool linea);

/// Multiplica una máscara binaria (con valores 0-negro y 255-blanco) a una imagen a color
Mat mult_mask(Mat src, Mat mask);


