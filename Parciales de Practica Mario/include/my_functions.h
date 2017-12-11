#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;
using namespace pdi;

/// VARIABLES GLOBALES

int radio = 30;
Point3i color;
Mat img, imgfinal;
Mat mascara; //Mascara binaria

int range_a = 0, range_b = 360;
int thresh = 10;
int minLinLength = 100;
int maxLineGap = 2500;
int mode = 0;

/// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- ///

/// Para eliminar el Gtk-warning de incompatibilidad de locales...no lo elimna... no más no lo muestra XD.
void bbw() {
    namedWindow("Output",1);
    Mat output = Mat::zeros( 1, 1, CV_8UC1 );
    imshow("Output", output);
    destroyAllWindows();
    system("clear");
}

/// Para cargar las imagenes suponiendo que se encuentran en el directorio "../imgs/"
Mat loadimg(string file, int type, bool display, int autosize = 1){
    string route = "../imgs/" + file;

    Mat img = imread(route, type);

    if(display){
        namedWindow(file, autosize);
        imshow(file, img);
    }

    return img;
}

/// Callback. Muestra la información del pixel pulsado por consola.
void pinfo( int event, int x, int y, int, void* var){
    Mat *img = (Mat*)var;
    string pos, color;
    bool format = true;

    if(img->type() == CV_8UC1){
        int aux = img->at<uchar>(y,x);
        color = "Intensidad de Gris: " + to_string(aux);
        pos =  "(" + to_string(y) + "," + to_string(x) + ") - ";
    }
    else if(img->type() == CV_8UC3){
        Point3_<uchar>* p = img->ptr<Point3_<uchar> >(y,x);
        color = " BGR: [ " + to_string(p->x) +" "+ to_string(p->y) +" "+ to_string(p->z) +" ]";
        pos =  "(" + to_string(y) + "," + to_string(x) + ") - ";
        Mat HSV;
        cvtColor(*img, HSV, CV_BGR2HSV);
        p = HSV.ptr<Point3_<uchar> >(y,x);
        color = color + " HSV: [ " + to_string(p->x) +" "+ to_string(p->y) +" "+ to_string(p->z) +" ]";
    }else {
        format = false;
    }

    if(event==CV_EVENT_LBUTTONDOWN){   	// Para una fila
        if (format)
            cout << pos << color << endl;
        else
            cout<< "Formato de imagen no tolerado" << endl;
    }

}

/// LUT lineal de la forma s = a*r + c. Funciona para tipo de dato CV_8UC1.
Mat linear_lut(float a, float c){
    float s = 0;
    Mat lut(1, 256, CV_8UC1);

    for (int r = 0; r < 256; ++r){
        s = a*r + c;

        if (s > 255)
            s = 255;

        if (s < 0)
            s = 0;

        lut.at<uchar>(r) = s;
    }

    return lut;
}

/// LUT para imagen en negativo. Funciona para tipo de dato CV_8UC1.
Mat negative_lut(){
    Mat lut(1, 256, CV_8UC1);

    for (int r = 0; r < 256; ++r){
        lut.at<uchar>(r) = 255 - r;
    }

    return lut;
}


/// LUT por tramos definido por 3 pares de datos (a,c). Funciona para tipo de dato CV_8UC1.
Mat piecewise_lut(int p1, int p2, float a1, float c1, float a2, float c2, float a3, float c3, bool inv){
    Mat lut(1, 256, CV_8U);

    int s = 0;

    for(int r = 0; r < 256; ++r){
        if (r < p1){
            s = a1*r + c1;
        }
        else{
            if (r >= p1 && r < p2)
                s = a2*r + c2;
            else
                s = a3*r + c3;
        }

        if (s > 255)
            s = 255;

        if (s < 0)
            s = 0;

        if(inv) // si se activa la bandera inv, se realiza 255-s
            lut.at<uchar>(r) = 255 - s;
        else	// sino se calcula normalmente
            lut.at<uchar>(r) = s;
    }

    return lut;
}

/// captura un punto con el click izquierdo y lo guarda en un vector de puntos
void getPoint( int event, int x, int y, int, void* var){
    if(event==CV_EVENT_LBUTTONDOWN){   	// Para una fila
        vector<Point>* v = (vector<Point>*)var;
        v->push_back(Point(x,y));
    }
}

/// realiza un lut por tramos a partir de dos clicks
Mat clickLUT() {
    Point p1(0,255); 								// punto (0,0) de la línea identidad
    Point p4(255,0);								// punto (255,255) de la línea identidad
    Point p2, p3;									// puntos interiores capturados por mouse

    vector<Point> v;								// vector para almacenar los puntos clickeados
    Mat r_s(256,256,CV_8UC1,Scalar(0,0,0)); 		// imagen en negro de 256x256

    line(r_s, p1, p4, 255, 1);    					// grafico la recta identidad

    string title = "Plano r-s";
    namedWindow(title, 1);
    setMouseCallback(title, getPoint, (void*)&v);	// captura los puntos y los manda al vector v
    imshow(title, r_s);								// muestra la recta identidad
    while(1) { 										// bucle infinito hasta que el vector tenga 2 puntos
        waitKey(1);									// para que se muestre la ventana dentro del bucle

        if( v.size() == 1){							// al capturar el primer punto
            p1.x = 0; p1.y = 255;
            p2.x = v[0].x; p2.y = v[0].y;
            line(r_s, p1, p2, 255, 1);				// grafica una recta entre (0,255) y el nuevo punto
            imshow(title, r_s);						// refresca la imagen
        }

        if(v.size() == 2){							// al capturar el segundo punto
            p3.x = v[1].x; p3.y = v[1].y;
            line(r_s, p2, p3, 255, 1);				// grafica una recta entre el primer y el segundo punto
            line(r_s, p3, p4, 255, 1);				// grafica una recta entre el segundo punto y (255,0)

            imshow(title, r_s);						// refresca la imagen

            break;									// termina el bucle infinito
        }
    }

    setMouseCallback(title, NULL, NULL);			// desactiva el callback para dejar de capturar puntos

    float a1 = (float)(p2.y-p1.y)/(float)(p2.x-p1.x); 	// pendiente del tramo 1-2
    float c1 = 255; 									// ordenada al origen del tramo 1-2

    float a2 = (float)(p3.y-p2.y)/(float)(p3.x-p2.x); 	// pendiente del tramo 2-3
    float c2 = p2.y-a2*p2.x; 							// ordenada al origen del tramo 2-3

    float a3 = (float)(p4.y-p3.y)/(float)(p4.x-p3.x); 	// pendiente del tramo 3-4
    float c3 = p3.y-a3*p3.x; 							// ordenada al origen del tramo 3-4

    // calculo el lut por tramos con transformación final de (255-s) (último parámetro de la función)
    Mat lut = piecewise_lut(p2.x, p3.x, a1, c1, a2, c2, a3, c3, 1);

    return lut;
}

/// LUT logarítmico
Mat logLUT(float c){
    Mat lut(1, 256, CV_8U);

    int s = 0;

    for(int r = 0; r < 256; ++r){
            s = c*log(1 + r)*255/log(256);

        if (s > 255)
            s = 255;

        if (s < 0)
            s = 0;

        lut.at<uchar>(r) = s;
    }

    return lut;
}

/// LUT exponencial
Mat expLUT(float c, float gamma){
    Mat lut(1, 256, CV_8U);

    int s = 0;

    for(int r = 0; r < 256; ++r){
        s = c*pow(r,gamma)*255/pow(255,gamma);

        if (s > 255)
            s = 255;

        if (s < 0)
            s = 0;

        lut.at<uchar>(r) = s;
    }

    return lut;
}


/// crea una máscara binaria rectangular en la posición indicada
Mat binMask(int height, int width, int x1, int y1, int x2, int y2){
    Mat mask = Mat::zeros(height, width, CV_8UC1);

    for (int j = x1; j < x2; j++)
        for (int i = y1; i < y2; i++)
            mask.at<uchar>(i,j) = 1;

    return mask;
}


/// Suma de imágenes tipo CV_32FC1 contenidas en el vector v
Mat mat_sum(vector<Mat> v){
    Mat out = Mat::zeros(v[0].rows, v[0].cols, CV_32FC1);

    int N = v.size();

    for(int i = 0; i < N; i++){
        Mat aux;
        v[i].convertTo(aux, CV_32FC1);
        out = out + aux/N;
    }

    out.convertTo(out, CV_8UC1);

    return out;
}

/// Suma de imágenes tipo CV_8UC1 contenidas en el vector v
Mat mat_sumf(vector<Mat> v){
    Mat out = Mat::zeros(v[0].rows, v[0].cols, CV_32FC1);

    int N = v.size();

    for(int i = 0; i < N; i++)
        out = out + v[i]/N;

    return out;
}

/// Permite devolver la información del plano de bits n-ésimo (0-7 si es de 8 bits). El 1 (blanco) se ajusta a 255.
Mat BitPlane(Mat img, int plane){
    Mat out(img.size(),img.type());

    int p = pow(2,plane);

    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            out.at<uchar>(i,j) = (img.at<uchar>(i,j) & uchar(p)) ? uchar(255) : uchar(0);
        }
    }

    return out;
}

/// Devuelve el error cuadrático medio entre dos imágenes
float ECM(Mat img1, Mat img2) {
    int ncol = img1.cols;
    int nrow = img1.rows;
    int N = ncol * nrow;
    float e = 0.0;

    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol; j++){
            e += pow(img1.at<uchar>(i,j) - img2.at<uchar>(i,j), 2);
        }
    }

    return e/N;
}

/// Funcion para crear mscaras de filtros de promediado lleno y no lleno
Mat kernelPromediado(int n, int tipo){
    Mat K(n, n, CV_32FC1);
    switch (tipo){
    case 1:		//promediado lleno
        for (int i=0;i<n;i++){
            for (int j=0;j<n;j++){
                K.at<float>(i,j) = (float) 1/(n*n);
            }
        }
        break;
    case 2:{	//promediado  no lleno
        K.setTo(0);
        int c = (n / 2); // Division entera entre el tamaño del kernel y 2
        for (int i=0;i<=((n-1)/2);i++){
            for(int j=((n-1)/2)-i; j<((n-1)/2)+1+i; j++){
                K.at<float>(i,j) = (float) 1 / ((n* (n-c)) - c);
            }
        }
        for (int i=(n-1);i>((n-1)/2);i--){
            for(int j=((n-1)/2)-(n-i-1); j<((n-1)/2)+(n-i); j++){
                K.at<float>(i,j) = (float) 1 / ((n * (n-c)) - c);
            }
        }
        break;
    }
    case 3: 	/// hay que revisar a ver que mascara devuelve el caso 3
            float valor;
            if(n == 3) valor = 0.125;
            if(n == 5) valor = 0.0625;
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

/// Aplica el Kernel Pasa-Alto de Suma 1 o 0, tanto Llenos como NO Llenos
Mat filtroPromediado(Mat src, int n, int type) {
    Mat kernel = kernelPromediado(n, type);

    Mat dst = convolve(src, kernel);

    return dst;
}

/// Funcion para hacer los distintos LUT de las diapositivas
Mat UmbralLUT(int tipo, int p1, int p2, double Smax){
    Mat lut(1,256,CV_8UC1);
    switch (tipo){
    case 1 :	//Binarizacion
        for(int r = 0; r<lut.cols; r++){
            float s = r;
            if(s<=p1) s = 0;
            else
                if(s>p1) s = Smax;
            lut.at<uchar>(0,r) =  (int)s;
        }
        break;
    case 2 : 	// Binarizacion con umbral invertido
        for(int r = 0; r<lut.cols; r++){
            float s = r;
            if(s<=p1) s = Smax;
            else
                if(s>p1) s = 0;
            lut.at<uchar>(0,r) =  (int)s;
        }
        break;
    case 3 :	// Intervalo Umbral
        for(int r = 0; r<lut.cols; r++){
            float s = r;
            if(s<=p1 || s>=p2) s = Smax;
            else
                if(s>p1 && s<p2) s = 0;
            lut.at<uchar>(0,r) =  (int)s;
        }
        break;
    case 4 :	// Intervalo umbral invertido
        for(int r = 0; r<lut.cols; r++){
            float s = r;
            if(s<=p1 || s>=p2) s = 0;
            else
                if(s>p1 && s<p2) s = Smax;
            lut.at<uchar>(0,r) =  (int)s;
        }
        break;
    case 5 :	// Escala de grises
        for(int r = 0; r<lut.cols; r++){
            float s = r;
            if(s<=p1 || s>=p2) s = Smax;
            else
                if(s>p1 && s<p2) s = r;
            lut.at<uchar>(0,r) =  (int)s;
        }
        break;
    case 6 :	//Escala de grises invertido
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

/// Devuelve un Kernel Pasa-Alto de Suma 1 o 0, tanto Llenos como NO Llenos
Mat kernelPasaAlto(int n, int type){
    Mat kernel = Mat::ones(n, n, CV_32FC1);
    switch (type){
    case 1:{ /// Suma 1 No Lleno
        kernel.setTo(0);
        int c = (n / 2); // Division entera entre el tamaño del kernel y 2
        for (int i=0;i<=((n-1)/2);i++){
            for(int j=((n-1)/2)-i; j<((n-1)/2)+1+i; j++){
                kernel.at<float>(i,j) = (float) -1;
            }
        }
        for (int i=(n-1);i>((n-1)/2);i--){
            for(int j=((n-1)/2)-(n-i-1); j<((n-1)/2)+(n-i); j++){
                kernel.at<float>(i,j) = (float) -1;
            }
        }
        kernel.at<float>(n/2, n/2) = (float) ((n * (n-c)) - c);

        break;
    }
    case 2: /// Suma 1 Lleno
            kernel.setTo(-1);
            kernel.at<float>(n/2, n/2) = n*n;
            break;
    case 3: /// Suma 0 Lleno
        kernel.setTo(-1);
        kernel.at<float>(n/2, n/2) = (n*n)-1;
        break;
    case 4: /// suma 1 Lleno RARO
        kernel.setTo(1);
        int c = (n / 2); // Division entera entre el tamaño del kernel y 2
        for (int i=0;i<=((n-1)/2);i++){
            for(int j=((n-1)/2)-i; j<((n-1)/2)+1+i; j++){
                kernel.at<float>(i,j) = (float) -2;
            }
        }
        for (int i=(n-1);i>((n-1)/2);i--){
            for(int j=((n-1)/2)-(n-i-1); j<((n-1)/2)+(n-i); j++){
                kernel.at<float>(i,j) = (float) -2;
            }
        }
        kernel.at<float>(n/2, n/2) = (float) ((n * (n-c)) - c);
        break;
    }
    return kernel;
}

/// Aplica el Kernel Pasa-Alto de Suma 1 o 0, tanto Llenos como NO Llenos
Mat filtroPasaAlto(Mat src, int n, int type) {
    Mat kernel = kernelPasaAlto(n, type);

    Mat dst = convolve(src, kernel);

    return dst;
}


/// Para sumar y restar imagenes
Mat arithm_transf(const Mat &img1, const Mat &img2, int op){
    Mat out = Mat::zeros(img1.rows, img1.cols, CV_8UC1);
    Mat aux1, aux2, s;

    img1.convertTo(aux1, CV_32FC1);
    img2.convertTo(aux2, CV_32FC1);

    switch (op){
    case 1: /// suma
        //			for(int i = 0; i < img1.rows; i++) {
        //				for(int j = 0; j < img1.cols; j++) {
        //					aux = (img1.at<uchar>(i,j) + img2.at<uchar>(i,j)) / 2;
        //					out.at<uchar>(i,j) = (uchar)aux;
        //				}
        //			}
        s = (aux1 + aux2)/2;
        s.convertTo(out, CV_8UC1);
        break;
        case 2: /// resta y las dos posibles normalizaciones
            //			for(int i = 0; i < img1.rows; i++) {
            //				for(int j = 0; j < img1.cols; j++) {
            //					aux = img1.at<uchar>(i,j) - img2.at<uchar>(i,j);
            //					out.at<uchar>(i,j) = (uchar)aux;
            //				}
            //			}
//			s = aux1 - aux2;
//			s.convertTo(out, CV_8UC1);
//						out = (out + 255) / 2;
            double min, max;
            minMaxLoc(out, &min, &max);
            out = out - min;

            break;
        case 3: /// multiplicación
            out = img1.mul(img2);
            break;
    }
    return out;
}

/// Aplica a la Imagen src un Filtro Gaussiano de nxn, con desvio Sigma y devuelve la imagen resultante en dst
Mat gaussianFilter(Mat src, int n, float sigma){
    Mat dst(src.size(), src.type());
    GaussianBlur(src, dst, Size(n, n), sigma, sigma, BORDER_DEFAULT);
    return dst;
}

/// Devuelve La imagen y su Histograma
void imageHist(Mat src){
    Mat hist = histogram(src, 255);
    normalize(hist, hist, 0, 1, CV_MINMAX);

    Mat canvas = Mat(src.size(), src.type(), Scalar(0,0,0));
    canvas = draw_graph(canvas, hist);

    vector<Mat> v;

    v.push_back(src); v.push_back(canvas);
    Mat imgv = mosaic(v,1);

    string title = "Imagen e Histograma";
    namedWindow(title, 1);
    imshow(title,imgv);

}

// Calcula el histograma de una imagen a color, muestra las 3 componentes y las devuelve en un vector<Mat>
vector<Mat> colorHistogram(Mat src, bool graph){
    vector<Mat> bgr_planes;
    split(src, bgr_planes);

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    if (graph){
        /// Draw for each channel
        for( int i = 1; i < histSize; i++ )
        {
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                 Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                 Scalar( 255, 0, 0), 2, 8, 0  );
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                 Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                 Scalar( 0, 255, 0), 2, 8, 0  );
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                 Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                 Scalar( 0, 0, 255), 2, 8, 0  );
        }

        /// Display
        namedWindow("Histograma", CV_WINDOW_AUTOSIZE );
        imshow("Histograma", histImage );
    }

    vector<Mat> v;
    v.push_back(b_hist);
    v.push_back(g_hist);
    v.push_back(r_hist);

    return v;
}

// Devuelve una imagen en pseudo-color a partir de una imagen en escala de grises. Modifica todos los pixeles en el rango
// [a,b] con las componentes de color (R,G,B) (mejor usar inRange())
Mat pseudoColor(Mat src, int a, int b, int R, int G, int B){
    Mat img = Mat(src.size(), CV_8UC3);

    cvtColor(src, img, CV_GRAY2BGR);
    vector<Mat> bgr_channels;
    split(img, bgr_channels);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if ( (src.at<uchar>(i,j) >= a) && (src.at<uchar>(i,j) <= b) ){
                // si se supera el umbral se cambia el color a amarillo puro
                bgr_channels[0].at<uchar>(i,j) = B;
                bgr_channels[1].at<uchar>(i,j) = G;
                bgr_channels[2].at<uchar>(i,j) = R;
            }
        }
    }

    merge(bgr_channels, img);

    return img;
}

/// Nuestro Espectrum (para visualizar el espectro de Transformada Discreta de Fourier)
Mat my_spectrum(Mat I){
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    return magI;
}

/// Devuelve la imagen (en escala de grises) y su espectro de Fourier
void imageSpec(Mat src){
    Mat spec = my_spectrum(src);
    if(src.type() == CV_LOAD_IMAGE_GRAYSCALE)
        src.convertTo(src, CV_32F, 1/255.f);

    vector<Mat> v;

    v.push_back(src);
    v.push_back(spec);

    Mat imgv = mosaic(v,1);

    namedWindow("Imagen y espectro de Fourier", 0);
    imshow("Imagen y espectro de Fourier", imgv);
}

/// Filtro Frecuencial Ideal
Mat FiltroFrecuencialIdeal(Mat img, int tipo, float fc){
    Mat results;
    img.convertTo(img,CV_32F, 1/255.f);
    img = optimum_size(img);
    switch(tipo){
        case 1: { //filtro ideal frecuencial pasa bajo
            Mat FIPB = filter_ideal(img.rows,img.cols, fc);
            FIPB = optimum_size(FIPB);
            results = filter(img, FIPB);
            break;
        }
        case 2: { //filtro ideal frecuencial pasa alto
            Mat FIPB = filter_ideal(img.rows, img.cols, fc);
            Mat FIPA = 1 - FIPB;
            FIPA = optimum_size(FIPA);
            results = filter(img, FIPA);
        }
    }
    return results;
}

/// Filtro frecuencial Butterworth
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

/// Filtrado frecuencial gaussiano
Mat FiltroFrecuencialGaussiano(Mat img, int tipo, float fc){
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

/// Filtrado homomórfico
Mat FiltroHomomorfico(Mat img, float gh, float gl, float fc, float c){
    img.convertTo(img,CV_32F);
    img = optimum_size(img);
    Mat H(img.size(),img.type());
    Mat	results(img.size(),img.type());
    Mat	imgLog = Mat::zeros(img.size(),img.type());

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

//    imgLog.convertTo(imgLog,CV_32F,1/255.f);
//    imgLog = optimum_size(imgLog);

//    log(img,imgLog);
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if(img.at<float>(i,j) == 0){
                imgLog.at<float>(i,j) = 0;
            }
            else
                imgLog.at<float>(i,j) = log(img.at<float>(i,j))/log(255);
        }
    }


//    img.convertTo(img,CV_32F,1/log(255.f));
    Mat filtrado = filter(imgLog,H);
    cout << filtrado << endl;
    exp(filtrado,results);

    cout << H << endl;

    return results;
}

/// Filtro de Alta Potencia Frecuencia
Mat AltaPotenciaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW, float A){
    //HAP = (A-1) + HPA
    img.convertTo(img,CV_32F,1/255.f);
    img = optimum_size(img);
    Mat HPA;
    switch(tipoPAfrec){
    case 1:{ // filtro frecuencial ideal
            Mat FIPB = filter_ideal(img.rows, img.cols, fc); // devuelve el filtro ideal pasa bajo frecuencial
            Mat FIPA = 1 - FIPB; //obtengo el filtro ideal pasa alto frecuencial
            FIPA = optimum_size(FIPA);
            HPA = filter(img,FIPA); // realiza el filtrado en frecuencia de la imagen con el filtro pasa alto
            break;
        }
    case 2: { // filtro frecuencial butterworth
            Mat FButterPB = filter_butterworth(img.rows, img.cols, fc, ordenBW); // devuelve el filtro butterworth pasa bajo
            Mat FButterPA = 1 - FButterPB; //obtengo el filtro butterworth pasa alto
            FButterPA = optimum_size(FButterPA);
            HPA = filter(img,FButterPA); // realiza el filtrado en frecuencia de la imagen con el filtro pasa alto
            break;
        }
    case 3: { // filtro frecuencial gaussiano
            Mat FGPB = filter_gaussian(img.rows, img.cols, fc); // devuelve el filtro gaussiano pasa bajo
            Mat FGPA = 1 - FGPB; //obtengo el filtro gaussiano pasa alto frecuencial
            FGPA = optimum_size(FGPA);
            HPA = filter(img,FGPA); // realiza el filtrado en frecuencia de la imagen con el filtro pasa alto
            break;
        }
    }
    return (A-1) + HPA;
}

/// Filtro de Enfasis de Alta Frecuencia en dominio frecuencial
Mat EnfasisAltaFrec(Mat img, int tipoPAfrec, float fc, int ordenBW, float a, float b){
    //HEAF = a + bHPA
    img.convertTo(img,CV_32F,1/255.f);
    img = optimum_size(img);

    Mat HPA;
    switch(tipoPAfrec){
    case 1:{ //filtro frecuencial ideal
            Mat FIPB = filter_ideal(img.rows, img.cols, fc); //devuelve el filtro ideal pasa bajo en dom espacial
            Mat FIPA = 1 - FIPB; //obtengo el filtro ideal pasa alto en dom espacial
            FIPA = optimum_size(FIPA);
            HPA = filter(img,FIPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
            break;
        }
    case 2: { //filtro frecuencial butterworth
            Mat FButterPB = filter_butterworth(img.rows, img.cols, fc, ordenBW); //devuelve el filtro butter pasa bajo en dom espacial
            Mat FButterPA = 1 - FButterPB; //obtengo el filtro butter pasa alto en dom espacial
            FButterPA = optimum_size(FButterPA);
            HPA = filter(img,FButterPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
            break;
        }
    case 3: { //filtro frecuencial gaussiano
            Mat FGPB = filter_gaussian(img.rows, img.cols, fc); //devuelve el filtro gaussian pasa bajo en dom espacial
            Mat FGPA = 1 - FGPB; //obtengo el filtro gaussian pasa alto en dom espacial
            FGPA = optimum_size(FGPA);
            HPA = filter(img,FGPA); // transforma el filtro al dom frec y realiza la multiplicacion en el dom frec
            break;
        }
    }
    return a + b*HPA;
}


/// Modelos de ruido
Mat noise_impulsive(Mat src, uchar a, uchar b, float pa, float pb){ //Ruido Impulsivo
    Mat dst = src.clone();

    srand(time(NULL));

    float number = 0.f;

    for (int i = 0; i < dst.rows; i++){
        for (int j = 0; j < dst.cols; j++){
            number = ((double) rand() / (RAND_MAX));
            if (number <= pa)
                dst.at<uchar>(i,j) = a;
            else if (number >= 1 - pb)
                dst.at<uchar>(i,j) = b;
        }
    }
    return dst;
}

Mat noise_uniform(Mat src, float min, float max){ // Ruido uniforme
    default_random_engine generator;                            // generador de nros. aleatorios
    uniform_real_distribution<double> distribution(min,max);    // genera distribución uniforme
    Mat noise = Mat::zeros(src.size(), CV_32F);                 // primero trabajamos en flotantes
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols;j++){
            // para cada pixel se suman valores aleatorios según distribución uniforme
            noise.at<float>(i,j) = (src.at<uchar>(i,j) + distribution(generator))/255.f;
        }
    }
    noise.convertTo(noise, CV_8UC1,255.f);                      // vuelve a convertir a rango [0,255] en uchar
    return noise;
}

Mat noise_gaussian(Mat src, float mean, float var){ // Ruido gaussiano
    default_random_engine generator;                            // generador de nros. aleatorios
    normal_distribution<double> distribution(mean,var);         // genera distribución gaussiana
    Mat noise = Mat::zeros(src.size(), CV_32F);                 // primero trabajamos en flotantes
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols;j++){
            // para cada pixel se suman valores aleatorios según distribución gaussiana
            noise.at<float>(i,j) = (src.at<uchar>(i,j) + distribution(generator))/255.f;
        }
    }
    noise.convertTo(noise, CV_8UC1,255.f);                      // vuelve a convertir a rango [0,255] en uchar
    return noise;
}

Mat noise_gamma(Mat src, float a, float b){ // Ruido gaussiano
    default_random_engine generator;                            // generador de nros. aleatorios
    gamma_distribution<double> distribution(a,b);               // genera distribución gamma
    Mat noise = Mat::zeros(src.size(), CV_32F);                 // primero trabajamos en flotantes
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols;j++){
            // para cada pixel se suman valores aleatorios según distribución gamma
            noise.at<float>(i,j) = (src.at<uchar>(i,j) + distribution(generator))/255.f;
        }
    }
    noise.convertTo(noise, CV_8UC1,255.f);                      // vuelve a convertir a rango [0,255] en uchar
    return noise;
}

Mat noise_exponential(Mat src, float lambda){ // Ruido exponencial
    default_random_engine generator;                            // generador de nros. aleatorios
    exponential_distribution<double> distribution(lambda);      // genera distribución exponencial
    Mat noise = Mat::zeros(src.size(), CV_32F);                 // primero trabajamos en flotantes
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols;j++){
            // para cada pixel se suman valores aleatorios según distribución exponencial
            noise.at<float>(i,j) = (src.at<uchar>(i,j) + distribution(generator))/255.f;
        }
    }
    noise.convertTo(noise, CV_8UC1,255.f);                      // vuelve a convertir a rango [0,255] en uchar
    return noise;
}

///Filtros de Medias
Mat filtro_geometrico(Mat src,int msize){  // Filtro de media geométrica
    // msize tiene que ser impar o no va andar
    double acum;
    Mat dst=src.clone();
    int mside=(msize-1)/2;
    for(int i = mside; i < src.rows-mside; i++){
        for(int j = mside; j < src.cols-mside; j++){
            acum = 1;
            for(int ii = i-mside; ii <= i+mside; ii++){
                for(int jj = j-mside; jj <= j+mside; jj++){
                    acum *= src.at<uchar>(ii,jj);
                }
            }
            acum = pow(acum,1./(msize*msize));
            dst.at<uchar>(i,j) = acum;
        }
    }
    return dst;
}

Mat filtro_contra_armonico(Mat src,int msize, float Q){
    float acum1, acum2;
    Mat dst = src.clone();
    int mside=(msize-1)/2;
    for(int i = mside; i < src.rows-mside; i++){
        for(int j = mside; j < src.cols-mside; j++){
            acum1 = 0.f;
            acum2 = 0.f;
            for(int ii = i-mside; ii <= i+mside; ii++){
                for(int jj = j-mside; jj <= j+mside; jj++){
                    acum1 += pow(src.at<uchar>(ii,jj), Q + 1);
                    acum2 += pow(src.at<uchar>(ii,jj), Q);
                }
            }
            dst.at<uchar>(i,j) = acum1/acum2;
        }
    }
    return dst;
}

Mat filtro_mediana(Mat src,int msize){
    uchar v[msize*msize];
    Mat dst = src.clone();
    int k;
    int m = ((msize*msize)-1)/2;
    int mside = (msize-1)/2;
    for(int i = mside; i < src.rows-mside; i++){
        for(int j = mside; j < src.cols-mside; j++){
            k = 0; // k va entre [0, msize*msize-1]
            for(int ii = i-mside; ii <= i+mside; ii++){
                for(int jj = j-mside; jj <= j+mside; jj++){
                    v[k] = src.at<uchar>(ii, jj);
                    k++;
                }
            }
            sort(v, v + (msize*msize));
            dst.at<uchar>(i,j)=v[m];
        }
    }
    return dst;
}


Mat filtro_punto_medio(Mat src,int msize){
    uchar v[ msize*msize ];
    Mat dst = src.clone();
    int k;
    int N = msize * msize;      // cantidad de pixels que abarca la vecindad
    int mside = (msize-1)/2;    // cuántos pixels hay para cada lado desde el pixel (i,j)
    for(int i = mside; i < src.rows-mside; i++){
        for(int j = mside; j< src.cols-mside; j++){
            k = 0;
            for(int ii = i-mside; ii <= i+mside; ii++){
                for(int jj = j-mside; jj <= j+mside; jj++){
                    v[k] = src.at<uchar>(ii, jj);
                    k++;
                }
            }
            sort(v, v + N);                             // ordena los N elementos de la vecindad
            dst.at<uchar>(i,j) = 0.5 * (v[0] + v[N-1]); // el promedio entre el mayor y el menor de la vecindad
        }
    }
    return dst;
}


Mat filtro_alfa_rec(Mat src,int msize, int d){
    const int N = msize*msize;
    uchar v[ N ];
    Mat dst = src.clone();
    float t = 1./(N - d);
    int a, k;
    int t2 = N - 1;
    int mside = (msize - 1)/2;
    for(int i = mside; i < src.rows-mside; i++){
        for(int j = mside; j < src.cols-mside; j++){
            k = 0;
            for(int ii = i-mside; ii <= i+mside; ii++){
                for(int jj = j-mside; jj <= j+mside; jj++){
                    v[k] = src.at<uchar>(ii,jj);
                    k++;
                }
            }
            sort(v, v + N);
            a = 0;
            for(int i = d/2; i <= t2 - d/2; i++)
                a = a + v[i];
            dst.at<uchar>(i,j) = floor(t*a);
        }
    }
    return dst;
}

Mat filtro_notch_butterworth(size_t rows, size_t cols, double v[][4],int filas){ //v is mx4 [u,v,order,corte]
    //corte = w en imagen de lado 1
    //1 \over 1 + {D \over w}^{2n}
    Mat magnitud = Mat::ones(rows, cols, CV_32F);
    double corte;
    int m=filas;
    for(int f=0;f<m;f++){
    corte=(double)v[f][3];
    corte *= rows;
        for(size_t K=0; K<rows; ++K)
            for(size_t L=0; L<cols; ++L){
            double d2 = distance2(K+.5, L+.5, (rows/2)-v[f][1], (cols/2)-v[f][0]);
            double dc2 = distance2(K+.5, L+.5, (rows/2)+v[f][1], (cols/2)+v[f][0]);
            magnitud.at<float>(K,L) *=( 1.0/(1 + pow((corte*corte)/d2, v[f][2]) ) ) *
                    ( 1.0/(1 + pow((corte*corte)/dc2, v[f][2]) ) );
        }
    }

    centre(magnitud);
    return magnitud;
}

Mat filtro_movimiento(size_t rows, size_t cols, double a,double b){
    Mat filter[2];
    filter[0] = Mat::ones(rows, cols, CV_32F);
    filter[1] = Mat::ones(rows, cols, CV_32F);
    Mat dst = Mat::ones(rows, cols, CV_32F);
    for(size_t u = 0; u < rows; u++){
        for(size_t v = 0; v < cols; v++){
            filter[0].at<float>(u, v) = (1/(M_PI*(u*a + v*b))) * sin(M_PI * (u*a + v*b));
            filter[1].at<float>(u, v) = exp(M_PI * (u*a + v*b));
        }
    }
    merge(filter, 2, dst);
    return dst;
}

Mat edge_detection(Mat src, int method, bool display = false){
    // 1- Roberts, 2- Prewitt, 3- Sobel, 4- Laplaciano, 5- Laplaciano del Gaussiano
    string nombre;

    Mat mgx = Mat::zeros(3, 3, CV_32F);
    Mat mgy = Mat::zeros(3, 3, CV_32F);

    Mat dst = Mat::zeros(src.rows, src.cols, CV_32FC1);
//    cvtColor(src, src, CV_BGR2GRAY);
//    cvtColor(dst, dst, CV_BGR2GRAY);

    switch (method) {
    case 1: // Roberts
        if (src.type()==0) {
            src.convertTo(src, CV_32F, 1/255.f);
        }
        mgx.at<float>(1, 1) = -1;
        mgx.at<float>(2, 2) = 1;
        mgy.at<float>(1, 2) = -1;
        mgy.at<float>(2, 1) = 1;
        nombre = "Roberts";
        break;
    case 2: //Prewitt
        if (src.type()==0) {
            src.convertTo(src, CV_32F, 1/255.f);
        }
        mgx.at<float>(0,0)=1;   mgx.at<float>(0,1)=0;      mgx.at<float>(0,2)=-1;
        mgx.at<float>(1,0)=1;   mgx.at<float>(1,1)=0;      mgx.at<float>(1,2)=-1;
        mgx.at<float>(2,0)=1;   mgx.at<float>(2,1)=0;      mgx.at<float>(2,2)=-1;

        mgy.at<float>(0,0)=1;   mgy.at<float>(0,1)=1;      mgy.at<float>(0,2)=1;
        mgy.at<float>(1,0)=0;   mgy.at<float>(1,1)=0;      mgy.at<float>(1,2)=0;
        mgy.at<float>(2,0)=-1;  mgy.at<float>(2,1)=-1;     mgy.at<float>(2,2)=-1;
        nombre = "Prewitt";
        break;
    case 3: //Sobel
        if (src.type()==0) {
            src.convertTo(src, CV_32F, 1/255.f);
        }
        mgx.at<float>(0,0)=-1;  mgx.at<float>(0,1)=-2;     mgx.at<float>(0,2)=-1;
        mgx.at<float>(1,0)=0;   mgx.at<float>(1,1)=0;      mgx.at<float>(1,2)=0;
        mgx.at<float>(2,0)=1;   mgx.at<float>(2,1)=2;      mgx.at<float>(2,2)=1;

        mgy.at<float>(0,0)=-1;  mgy.at<float>(0,1)=0;      mgy.at<float>(0,2)=1;
        mgy.at<float>(1,0)=-2;  mgy.at<float>(1,1)=0;      mgy.at<float>(1,2)=2;
        mgy.at<float>(2,0)=-1;  mgy.at<float>(2,1)=0;      mgy.at<float>(2,2)=1;
        nombre = "Sobel";
        break;
    case 4: //Laplaciano
        Laplacian(src,dst,CV_16S,3);
        convertScaleAbs(dst,dst);
        nombre = "Laplaciano";
        break;
    case 5: //Laplaciano del Gaussiano
        GaussianBlur(src,src,Size(3,3),0,0);
        Laplacian(src,dst,CV_16S,3);
        convertScaleAbs(dst,dst);
        nombre = "Laplaciano del Gaussiano";
        break;
    default:
        cout<<"\nNumero ingresado invalido.\n";
        break;
    }

    vector<Mat> v;

    if (method == 4 || method == 5 ){
        v.push_back(src); v.push_back(dst);
        Mat imgv = mosaic(v,1);
        if(display)
            imshow("Imagen | " +nombre, imgv);
    }
    else {
        Mat gx = convolve(src, mgx);
        Mat gy = convolve(src, mgy);

        dst = abs(gx) + abs(gy);

        v.push_back(src); v.push_back(dst); v.push_back(gx); v.push_back(gy);
        Mat imgv = mosaic(v,2);
        if(display)
            imshow("Imagen | " + nombre + " | Gx | Gy ", imgv);
    }

    return dst;
}

/// Convierte y devuelve una imagen binarizada utilizando el umbral definido por 'thres'
/// Con 'display' se elige si se muestra o no la imagen en pantalla
Mat bin_img(Mat src, float thres, bool display){
    if(src.type() == CV_8UC1)
        src.convertTo(src, CV_32F, 1/255.f);

    Mat dst = src.clone();

    threshold(src, dst, thres, 255, THRESH_BINARY);
    if(display)
        imshow("Imagen binarizada", dst);

    return dst;
}

Mat rotateMat(Mat src, float deg){
    Point2f pc(src.cols/2.f, src.rows/2.f);

    Mat r = getRotationMatrix2D(pc, deg, 1.0);
    Mat dst;
    warpAffine(src, dst, r, src.size());

    return dst;
}

/// Funcion para Segmentar por Regiones Tp 07 - Ej 03
Mat Seg_CR(Mat img, Point p, Scalar L, Scalar H){
    Mat Mascara = Mat::zeros(img.rows, img.cols, img.type()), im;
    img.convertTo(im, CV_32F);
    float k;

    floodFill(im, p, 256, NULL, L, H);
    for(int i=0; i<im.rows; i++)
        for(int j=0; j<im.cols; j++){
            k = im.at<float>(i, j);
            if(k == 256)
                Mascara.at<uchar>(i, j) = 255;
        }
    return Mascara;
}

/// Algoritmos de morfología binaria
Mat Morfologia(Mat src, Mat EE, char oper){
    Mat dst = Mat::zeros(src.size(),src.type());
    switch (oper) {
    case 'd'://Dilatacion
        dilate(src,dst,EE);
        break;
    case 'e'://Erosion
        erode(src,dst,EE);
        break;
    case 'a'://Apertura
        morphologyEx(src, dst, MORPH_OPEN, EE);
        break;
    case 'c'://Cierre
        morphologyEx(src, dst, MORPH_CLOSE, EE);
        break;
    default:
        cout<<"\nERROR MORFOLOGICO\n";
        break;
    }

    return dst;

}

/// Devuelve elementos estructurantes de 3x3 según los vistos en los distintos problemas
Mat getEE(int op){
    Mat EE = Mat::ones(3, 3, CV_8UC1);

    switch (op) {
        case 1:{
            uchar mask[3][3] = {{1,1,1},
                                {1,1,1},
                                {1,1,1},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 2:{
            uchar mask[3][3] = {{0,1,0},
                                {1,1,1},
                                {0,1,0},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 3:{
            uchar mask[3][3] = {{0,1,0},
                                {0,1,1},
                                {0,1,0},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 4:{
            uchar mask[3][3] = {{0,1,0},
                                {0,1,1},
                                {0,0,0},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 5:{
            uchar mask[3][3] = {{0,1,1},
                                {0,1,1},
                                {0,1,1},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 6:{
            uchar mask[3][3] = {{0,0,1},
                                {0,0,1},
                                {0,0,1},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 7:{
            uchar mask[4][4] = {{0,0,0,1},
                                {0,0,1,0},
                                {0,1,0,0},
                                {1,0,0,0},};
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 8:{
            uchar mask[3][3] = {{0,0,1},
                                {0,1,0},
                                {1,0,0},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }

        case 9:{
            uchar mask[7][7] = {{1,1,1,1,1,1,1,},
                                {1,1,1,1,1,1,1,},
                                {1,1,1,1,1,1,1,},
                                {1,1,1,1,1,1,1,},
                                {1,1,1,1,1,1,1,},
                                {1,1,1,1,1,1,1,},
                                {1,1,1,1,1,1,1,},};
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }

        case 10:{
            uchar mask[3][3] = {{1,0,0},
                                {0,1,0},
                                {0,0,1},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
        case 11:{
            uchar mask[5][5] = {{0,0,0,0,0,},
                                {0,0,0,0,0,},
                                {1,1,1,1,1,},
                                {0,0,0,0,0,},
                                {0,0,0,0,0,},};
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }

        default:{
            uchar mask[3][3] = {{1,1,1},
                                {1,1,1},
                                {1,1,1},};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    EE.at<uchar>(i,j) = mask[i][j];
                }
            }
            break;
        }
    }

    return EE;
}

float porcentaje(Mat src, int color, bool msj){ //Promedio de los pixeles de color_sum en la imagen
    float sum = 0.0;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            if(src.at<uchar>(i,j) == color)
            sum += 1.0;
        }
    }
    float porcentaje = sum/(src.rows*src.cols);
//    if (msj)
//       cout << fixed << setprecision(2) << "Porcentaje de pixeles " << to_string(color) << " en la imagen: " << (porcentaje*100) << " %" <<endl;
    return porcentaje;
}

/// Funciones de Segmentación
// Color Distance: dada una imagen src busca los pixceles de color "color" con un radio dado - Devuelve la
// imagen segmentada - SEGMENTACION: B G R - Point31 color(B,G,R)
Mat colorDistance(Mat src, Point3i color, int radio){
    Mat mask = Mat::zeros(src.size(), src.type());
    Mat dst = src.clone();

    //Compruebo la distancia del circulo
    for(int i=0; i<src.rows; i++){
        for(int j=0; j<src.cols; j++){
            int blue =  (int)src.at<Vec3b>(i, j)[0];
            int green = (int)src.at<Vec3b>(i, j)[1];
            int red = (int)src.at<Vec3b>(i, j)[2];

            //Calculo la distancia
            Point3i bgr(blue, green, red);
            float distance = norm(color-bgr);
            if(distance < radio){
                mask.at<Vec3b>(i,j)[0]=1;
                mask.at<Vec3b>(i,j)[1]=1;
                mask.at<Vec3b>(i,j)[2]=1;
            }
        }
    }

    multiply(src, mask, dst);

    return dst;
}

/// Segmentación BGR utilizando un trackbar. Al finalizar devuelve por consola las líneas que habría
/// que poner en el main() para reproducir lo mismo utilizando la función colorDistance definida arriba
/// y poder hacer la segmentación de forma automática, sin necesidad de clicks.
void colorDistance(){
    Mat aux = mascara.clone();

    //Compruebo la distancia del circulo
    for(int i=0; i<img.rows; i++){
        for(int j=0; j<img.cols; j++){
            int blue =  (int)img.at<Vec3b>(i, j)[0];
            int green = (int)img.at<Vec3b>(i, j)[1];
            int red = (int)img.at<Vec3b>(i, j)[2];

            //Calculo la distancia
            Point3i bgr(blue, green, red);
            float distance = norm(color-bgr);
            if(distance < radio){
                aux.at<Vec3b>(i,j)[0]=1;
                aux.at<Vec3b>(i,j)[1]=1;
                aux.at<Vec3b>(i,j)[2]=1;
            }
        }
    }

    multiply(aux, img, imgfinal);
    imshow("Segmentacion", imgfinal);
}

// Funcion que toma el color del punto clickeado
void seg_bgr_mouse_callback(int event, int x, int y, int flags, void*){
    (void) flags;
    if (event == CV_EVENT_LBUTTONDOWN) {
        // obtiene las componentes BGR del pixel clickeado
        color.x=(int)img.at<Vec3b>(y,x)[0];
        color.y =(int)img.at<Vec3b>(y,x)[1];
        color.z=(int)img.at<Vec3b>(y,x)[2];

        // muestra la info del pixel clickeado
        cout << color << endl;

        // actualiza la ventana "Segmentacion"
        colorDistance();
    }

    if (event == CV_EVENT_RBUTTONDOWN){
        setMouseCallback("Imagen Original (hacer click y actualiza la otra ventana)", NULL, NULL);
        destroyWindow("Imagen Original (hacer click y actualiza la otra ventana)");
    }
}

// Controlo la barra para mover los planos de color a través de la función 'colorDistance'
void seg_bgr_trackbar(int, void*){
    colorDistance();
}

void segmentacionBGR_trackbar(Mat src){
    img = src.clone();
    mascara = Mat::zeros(img.size(), img.type());   // inicializa la variable global 'mascara'

    cout << img.size() << endl;
    namedWindow("Segmentacion");
    imshow("Segmentacion", img);
    imshow("Imagen Original (hacer click y actualiza la otra ventana)", img);

    //Las variables de los trackbar son globales (arriba del main)
    setMouseCallback("Imagen Original (hacer click y actualiza la otra ventana)", seg_bgr_mouse_callback);
    createTrackbar("Radio","Segmentacion", &radio, 300, seg_bgr_trackbar);

    waitKey(0);

    cout << "--------------------------------------------------------------------------------" << endl;
//    cout << "Mat src = imread(\"" << file << "\", CV_LOAD_IMAGE_COLOR);" << endl;
    cout << "Point3i color = Point3i(" << color.x << "," << color.y << "," << color.z << ");" << endl;
    cout << "int radio = " << radio << ";" << endl;
    cout << "Mat seg = colorDistance(src, color, radio);" << endl << endl;
    cout << "imshow(\"seg\", seg);" << endl << endl;
}

void segmentacionBGR_trackbar(string file){
    img = imread(file, CV_LOAD_IMAGE_COLOR);                   // inicializa la variable global 'img'

    segmentacionBGR_trackbar(img);
}
/// *-*-*-*-*-*-*-*-*-* Fin de la Segmentación BGR. *-*-*-*-*-*-*-*-*-*-*

// Segmentacion en HSV: dada una imagen se le pasa los rango de cada canal
Mat SegmentacionHSV(Mat img, int Hmin, int Hmax, int Smin, int Smax, int Vmin, int Vmax){
    vector<Mat> CanalaesHSV(3);
    Mat img_hsi;
    Mat mascara = cv::Mat::zeros(img.size(),img.type());
    cvtColor(img,img_hsi,COLOR_BGR2HSV);
    split(img_hsi,CanalaesHSV);
    for (int i = 0; i<img.rows;i++)
        for (int j = 0; j<img.cols;j++){
            int H = (int)CanalaesHSV[0].at<uchar>(i,j);
            int S = (int)CanalaesHSV[1].at<uchar>(i,j);
            int V = (int)CanalaesHSV[2].at<uchar>(i,j);
            if ((Hmin<H)&&(H<Hmax) && (Smin<S)&&(S<Smax)&& (Vmin<V)&&(V<Vmax)){
                mascara.at<cv::Vec3b>(i,j)[0]=1; //Blue
                mascara.at<cv::Vec3b>(i,j)[1]=1; //Green
                mascara.at<cv::Vec3b>(i,j)[2]=1; //Red
            }
        }
    Mat imgfinal;
    multiply(img,mascara,imgfinal);
    return imgfinal;
}


// Informacion en canales RGB y HSV
vector<Mat> canales(string file, bool disp_bgr = false, bool disp_hsv = false){
    vector<Mat> channels;
    vector<Mat> bgr_channels;
    vector<Mat> hsv_channels;

    Mat bgr = imread(file, CV_LOAD_IMAGE_COLOR);
    Mat hsv;
    cvtColor(bgr, hsv, CV_BGR2HSV);

    split(bgr, bgr_channels);
    split(hsv, hsv_channels);

    channels.push_back(bgr_channels[0]);
    channels.push_back(bgr_channels[1]);
    channels.push_back(bgr_channels[2]);
    channels.push_back(hsv_channels[0]);
    channels.push_back(hsv_channels[1]);
    channels.push_back(hsv_channels[2]);
    channels.push_back(bgr);
    channels.push_back(hsv);

    if (disp_bgr){
        namedWindow("BGR", 0);
        namedWindow("B", 0);
        namedWindow("G", 0);
        namedWindow("R", 0);
        imshow("BGR", bgr);
        imshow("B", bgr_channels[0]);
        imshow("G", bgr_channels[1]);
        imshow("R", bgr_channels[2]);
    }

    if (disp_hsv){
        namedWindow("HSV", 0);
        namedWindow("H", 0);
        namedWindow("S", 0);
        namedWindow("V", 0);
        imshow("HSV", hsv);
        imshow("H", hsv_channels[0]);
        imshow("S", hsv_channels[1]);
        imshow("V", hsv_channels[2]);
    }

    return channels;
}

//Contar elementos en una imagen
//img es una imagen binarizada
int count_element(Mat img, bool msj){

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    if (msj)
       cout << "Total de objetos en la imagen: " << contours.size() <<endl;

    return contours.size();
}

/// Devuelve los contornos de una imagen binarizada
vector<vector<Point>> count_element(Mat img){

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    return contours;
}

/// Devuelve los centros de masa de un vector de contornos
vector<Point2f> get_mass_centers(vector<vector<Point>> contours){
    // Primero se obtienen los momentos
    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    { mu[i] = moments( contours[i], false ); }

    //  A partir de los momentos se pueden hallar los centros de masa
    vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }

    return mc;
}

/// Detecta líneas en un rango de ángulos deseado utilizando el algoritmo de Hough estándar
vector<Vec2f> detect_lines(const Mat &src, Mat & dst, int threshold, int range_a, int range_b){
    Mat aux;
    int low = 50;
    int ratio = 4;
    Canny(src, aux, low, low*ratio, 3);
    cvtColor(aux, dst, CV_GRAY2BGR);

    // Algoritmo de Hough standard
    vector<Vec2f> lines;
    vector<Vec2f> final;
    // HoughLines(dst, lines, rho, theta, threshold, 0, 0 );
    // dst: imagen de entrada para el detector de líneas...debe estar binarizada (Ejemplo: algoritmo de Canny)
    // lines: vector donde se almacenan todas las líneas (para luego pintarlas en el for)
    // rho: parámetro de resulución en pixeles... se usa rho = 1
    // theta: parámetro de resolucin en radianes... se usa theta = 1 grado (se convierte con CV_PI/180)
    // threshold: el mínimo número de intersecciones para detectar una línea (prueba y error ñ_ñ)
    HoughLines(aux, lines, 1, CV_PI/180, threshold, 0, 0);

    // recorre el vector de lneas que encontró el algoritmo de Hough
    for( size_t i = 0; i < lines.size(); i++ )
    {
        // recupera el valor de rho y theta para cada línea
        float rho = lines[i][0], theta = lines[i][1];
        float ang = theta * 180 / CV_PI;

        if(ang >= range_a and ang <= range_b) {
            // con los dos ángulos arma las líneas y las dibuja sobre la imagen final: cdst
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( dst, pt1, pt2, Scalar(255,0,0), 3, CV_AA);
            final.push_back(lines[i]);
        }
    }


    return final;
}

/// Detecta líneas en un rango de ángulos deseado utilizando el algoritmo de Hough probabilístico
vector<Vec4i> detect_linesP(const Mat &src, Mat &dst, int threshold, int range_a, int range_b,
                            int minLineLength = 50, int maxLineGap = 10, int mode = 0){
    Mat aux;
    int low = 50;
    int ratio = 4;

    Canny(src, aux, low, low*ratio, 3);
    cvtColor(aux, dst, CV_GRAY2BGR);

    vector<Vec4i> lines;
    vector<Vec4i> final;
    HoughLinesP(aux, lines, 1, CV_PI/180, threshold, minLineLength, maxLineGap);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI;


        if (mode == 0){
            line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
            final.push_back(l);
        }

        if (mode == 1) {
            if(angle  >= -range_b and angle <= -range_a){
                line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
                final.push_back(l);
            }
        }

        if (mode == 2) {
            if(angle  >= range_a and angle <= range_b){
                line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
                final.push_back(l);
            }
        }
    }

    return final;
}

void houghp_trackbar(int, void*){


    detect_linesP(img, imgfinal, thresh, range_a, range_b, minLinLength, maxLineGap, mode);

    imshow("Hough Probabilistico", imgfinal);

    if (range_a >= range_b)
        setTrackbarPos("MIN angulo", "Hough Probabilistico", range_b - 1);
}

void detect_linesP_trackbar(Mat src){
    img = src.clone();
    string title = "Hough Probabilistico";
    namedWindow(title, 0);
    createTrackbar("MIN intersecciones", title, &thresh, 300, houghp_trackbar);
    createTrackbar("Modo de deteccion", title, &mode, 2, houghp_trackbar);
    createTrackbar("MIN angulo", title, &range_a, 89, houghp_trackbar);
    createTrackbar("MAX angulo", title, &range_b, 90, houghp_trackbar);
    createTrackbar("MIN largo de linea", title, &minLinLength, 500, houghp_trackbar);
    createTrackbar("MAX separacion entre lineas", title, &maxLineGap, 500, houghp_trackbar);

    houghp_trackbar(0, 0);

    waitKey(0);

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "En src esta la imgen donde se quiere dibujar" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "vector<Vec4i> lines;" << endl;
    cout << "Mat dst = src.clone();" << endl;
    cout << "Mat con_lineas = src.clone();" << endl;
    cout << "lines = detect_linesP(src, dst, " << thresh << ", " << range_a << ", " << range_b << ", " <<
            minLinLength << ", " << maxLineGap << ", " << mode << ");" << endl;

    cout << "for(size_t i = 0; i < lines.size(); ++i) {" << endl;
    cout << "    Vec4i l = lines[i];" << endl;
    cout << "    line(con_lineas, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);" << endl;
    cout << "}" << endl;
    cout << "imshow(\"Original con lineas\", con_lineas);" << endl;
    cout << endl;
}

///Devuelve las areas.
//Devuelve el valor de las areas ordenadas Mayor -> Menor
vector<float> get_areas(vector<vector<Point>> contours){
    vector<float> areas;

    for( size_t i = 0; i< contours.size(); i++ ){
        areas.push_back(contourArea(contours[i]));
    }

    sort(areas.begin(), areas.end());
    reverse(areas.begin(), areas.end());

    return areas;
}
////Devuelve los indices de las areas ordenadas Mayor -> Menor
vector<int> get_idx_areas(vector<vector<Point>> contours){
    vector<float> areas;

    for( size_t i = 0; i< contours.size(); i++ ){
        areas.push_back(contourArea(contours[i]));
    }

    vector<int> idx(areas.size());
    size_t n(0);
    generate(begin(idx), end(idx), [&]{ return n++; });

    sort(  begin(idx),
           end(idx),
           [&](int i1, int i2) { return areas[i1] < areas[i2]; } );

    reverse(idx.begin(), idx.end());

    return idx;
}

/// Pinta las areas definidas en contours con un determinado color
Mat paint_contour_area(Mat src, vector<vector<Point>> contours, int idx, Scalar color){
    Mat dst = src.clone();

    drawContours( dst, contours, idx, color, CV_FILLED);

    return dst;
}

///Scan Lines
/// Horizontal
vector<Point> scanLine_Horizontal(Mat img, int altura, uchar color, bool lado = true, bool linea = false){
    int count = 0;
    Point p;
    vector<Point> vp;
    string aux;

    if(lado == true){
        for (int j = 0; j < img.cols; ++j) {                  // recorre todas las columnas de morph (ROI luego de morfología binaria)
            if (img.at<uchar>(altura,j) != color){                 // tomo el pixel a la altura H
                if (img.at<uchar>(altura,j) == 255){               // si es blanco
                    count++;                                    // cuenta uno
                    color = 255;                                // el color actual ahora es blanco
                    p.x = j;                                    // me guardo la columna
                    p.y = altura;                                    // siempre el punto a la altura H
                    vp.push_back(p);                            // guardo el punto donde se encontró una línea blanca
                } else
                    color = 0;                                  // sino se cambia el color a negro de nuevo
            }
        }
        aux = "Izquierda -> Derecha";
    }
    else {
        for (int j = img.cols - 1; j >= 0 ; --j) {                  // recorre todas las columnas de morph (ROI luego de morfología binaria)
            if (img.at<uchar>(altura,j) != color){                 // tomo el pixel a la altura H
                if (img.at<uchar>(altura,j) == 255){               // si es blanco
                    count++;                                    // cuenta uno
                    color = 255;                                // el color actual ahora es blanco
                    p.x = j;                                    // me guardo la columna
                    p.y = altura;                                    // siempre el punto a la altura H
                    vp.push_back(p);                            // guardo el punto donde se encontró una línea blanca
                } else
                    color = 0;                                  // sino se cambia el color a negro de nuevo
            }
        }
        aux = "Derecha -> Izquierda";
    }

    Mat l = img.clone();
    if(linea){
        Point a(0,altura);
        Point b(l.cols,altura);
        line(l,a,b,Scalar(255,255,255),1);
        string msj = "ScanLine Horizontal / "+aux;
        imshow(msj,l);
        cout<<"Elementos intersectados: "<<vp.size()<<endl;
    }
    return vp;
}

/// Vertical
vector<Point> scanLine_Vertical(Mat img, int altura, uchar color, bool lado, bool linea){
    int count = 0;
    Point p;
    vector<Point> vp;
    string aux;

    if(lado == true){
        for (int j = 0; j < img.rows; ++j) {                  // recorre todas las columnas de morph (ROI luego de morfología binaria)
            if (img.at<uchar>(j,altura) != color){                 // tomo el pixel a la altura H
                if (img.at<uchar>(j,altura) == 255){               // si es blanco
                    count++;                                    // cuenta uno
                    color = 255;                                // el color actual ahora es blanco
                    p.x = altura;                                    // me guardo la columna
                    p.y = j;                                    // siempre el punto a la altura H
                    vp.push_back(p);                            // guardo el punto donde se encontró una línea blanca
                } else
                    color = 0;                                  // sino se cambia el color a negro de nuevo
            }
        }
        aux = "Arriba -> Abajo";
    }
    else {
        for (int j = img.rows - 1; j >= 0 ; --j) {                  // recorre todas las columnas de morph (ROI luego de morfología binaria)
            if (img.at<uchar>(j,altura) != color){                 // tomo el pixel a la altura H
                if (img.at<uchar>(j,altura) == 255){               // si es blanco
                    count++;                                    // cuenta uno
                    color = 255;                                // el color actual ahora es blanco
                    p.x = altura;                                    // me guardo la columna
                    p.y = j;                                    // siempre el punto a la altura H
                    vp.push_back(p);                            // guardo el punto donde se encontró una línea blanca
                } else
                    color = 0;                                  // sino se cambia el color a negro de nuevo
            }
        }
        aux = "Abajo -> Arriba";
    }

    Mat l = img.clone();
    if(linea){
        Point a(altura,0);
        Point b(altura,l.rows);
        line(l,a,b,Scalar(255,255,255),1);
        string msj = "ScanLine Vertical / "+aux;
        imshow(msj,l);
        cout<<"Elementos intersectados: "<<vp.size()<<endl;
    }
    return vp;
}

/// Multiplica una máscara binaria (con valores 0-negro y 255-blanco) a una imagen a color
Mat mult_mask(Mat src, Mat mask){
    Mat aux = mask.clone();
    aux = aux / 255;
    cvtColor(aux, aux, CV_GRAY2BGR);

    Mat dst;
    multiply(src, aux, dst);

    return dst;
}

#endif // MY_FUNCTIONS_H
