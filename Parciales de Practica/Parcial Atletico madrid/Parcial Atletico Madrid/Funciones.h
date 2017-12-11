#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "opencv2/opencv.hpp"
#include "pdi_functions.h"
#include <vector>
#include <iostream>
#include <bitset>
using namespace cv;
using namespace std;

namespace eri{

    Mat Bastida (String pregunta,bool color= false,string nombre = ""){

        if (nombre == ""){
            cout<<pregunta;
            //String nombre;
            cin>>nombre;
        }
        
        Mat imagen = imread("C:\\Users\\ERic\\Desktop\\PDI\\imagenes\\"+nombre,color);
        if (imagen.empty()){
            cout<<"\nLa imagen no pudo ser cargada...\n";
            return Mat::zeros(1,1,CV_8U);
        }
        imshow("Imagen original",imagen);
        return imagen;
    }
    Mat PB(Mat &imagen, int N){
        Mat kernel = Mat::ones(N,N,CV_32F)/(N*N);
        return pdi::convolve(imagen,kernel);
    }
    void transformacion(int a, int c,Mat &image, bool a_tramos){
    //Genero la tabla de la correspondiente transformacion lineal sobre la imagen
    Mat LUT = Mat::zeros(256,1,CV_8UC1);
    int temp;
    namedWindow("r-s");//Cuando lo vea en Qt depurarlo
    Mat canvas(256, 256, CV_8UC(3), cv::Scalar(255,255,255));
    cv::line( canvas, cv::Point( 0,255), cv::Point( 255,0 ), Scalar(0,0,255), 1, CV_AA );
    imshow("r-s",canvas);


    if (!a_tramos){
        for (int i = 0; i<256;i++){
            temp= a*i+c;
            if (temp > 255)
                LUT.at<uchar>(i) =255;
            else{
                if (temp < 0)
                    LUT.at<uchar>(i)  = 0;
                else
                    LUT.at<uchar>(i)= temp;
            }
        }
    }else{
        Point p1,p2;
        //cout<<"Desea ingresas los datos con teclado o mouse  [t/m]? :";
        cout<<"Ingrese los puntos :";
        cout<<"\n\nPunto 1 : \n x = ";	cin>>p1.x;cout<<" y = "; cin>>p1.y;
        cout<<"\n\nPunto 2 : \n x = ";	cin>>p2.x;cout<<" y = "; cin>>p2.y;

        //p1.y = canvas.rows - p1.y;
        //p2.y = canvas.rows - p2.y;

        for (int i = 0; i<256;i++){
            if (i <= p1.x){
                temp= round((p1.y/p1.x)*i+0);
            }
            if ((i > p1.x) && (i < p2.x)){
                temp= round(((p2.y-p1.y)/(p2.x-p1.x))*(i-p1.x)+p1.y);
            }
            if (i >= p2.x){
                temp= round(((255-p2.y)/(255-p2.x))*(i-p2.x) + p2.y);
            }

            if (temp > 255)
                LUT.at<uchar>(i) =255;
            else{
                if (temp < 0)
                    LUT.at<uchar>(i)  = 0;
                else
                    LUT.at<uchar>(i)= temp;
            }

        }
    }
    //Diseno del grafico

    for (int i = 0; i<image.rows;i++)
        for (int j = 0; j < image.cols; j++)
            image.at<uchar>(i,j) = LUT.at<uchar>(image.at<uchar>(i,j));


    pdi::draw_graph(canvas,LUT);
    imshow("r-s",canvas);

    }
    void transformacion_log(cv::Mat &image,int c){
        int temp;
        cv::Mat LUT = cv::Mat::zeros(256,1,CV_8UC1);
        for (int i = 0; i<256;i++){
            temp= c*log(i+1);
            if (temp > 255)
                LUT.at<uchar>(i) =255;
            else{
                if (temp < 0)
                    LUT.at<uchar>(i)  = 0;
                else
                    LUT.at<uchar>(i)= temp;
            }
        }
        for (int i = 0; i<image.rows;i++)
            for (int j = 0; j < image.cols; j++)
                image.at<uchar>(i,j) = LUT.at<uchar>(image.at<uchar>(i,j));

    }
    void transformacion_gamma(cv::Mat &image,int g,int c){
        float temp;
        cv::Mat LUT = cv::Mat::zeros(256,1,CV_8UC1);
        for (int i = 0; i<256;i++){
            temp= c*pow(i,g);
            if (temp > 255)
                LUT.at<uchar>(i) =255;
            else{
                if (temp < 0)
                    LUT.at<uchar>(i)  = 0;
                else
                    LUT.at<uchar>(i)= temp;
            }
        }



        for (int i = 0; i<image.rows;i++)
            for (int j = 0; j < image.cols; j++)
                image.at<uchar>(i,j) = LUT.at<uchar>(image.at<uchar>(i,j));

    }
    int comprobar (int n){
        int x;
        if (n > 255)
            x = 255;
        if (n < 0 )
            x = 0;
        else
            x = n;
        return x;
    }


    void umbralBinario(Mat image,int u,uchar a,uchar d){

        //Genero la tabla de la correspondiente transformacion lineal sobre la imagen
        Mat LUT = Mat::zeros(256,1,CV_8UC1);
        for (int i = 0; i<256;i++){
            if (i< u)
                LUT.at<uchar>(i) = a;
            else
                LUT.at<uchar>(i) = d;

        }
        for (int i = 0; i<image.rows;i++)
            for (int j = 0; j < image.cols; j++)
                image.at<uchar>(i,j) = LUT.at<uchar>(image.at<uchar>(i,j));

    }


    cv::Mat aritmetica (Mat &img1, Mat &img2,char op){
        Mat temp = Mat::ones(img1.rows,img2.cols,img1.type())*255;
        if ((img1.cols != img2.cols) || (img1.rows != img2.rows) ){
            cout<<"Error de dimensión.\n";
        }
        else{

            float t;
            switch (op){
                case 's': //Suma
                    temp = (img1+img2)/2;
                    break;
                case 'r'://Resta
                    temp = ((img1-img2)+temp)/2;
                    break;
                case 'm'://Multiplicacion
                        for (int i = 0; i<img1.rows;i++)
                            for (int j = 0; j<img1.cols;j++){
                                t = img1.at<uchar>(i,j)*img2.at<uchar>(i,j);
                                if (t > 255)
                                    temp.at<uchar>(i,j)=255;
                                if (t<0)
                                    temp.at<uchar>(i,j)=0;
                                else
                                    temp.at<uchar>(i,j)=t;
                            }
                    break;
                case 'd'://Division

                    for (int i = 0; i<img1.rows;i++)
                        for (int j = 0; j<img1.cols;j++){
                            t = img1.at<uchar>(i,j)*(-1*img2.at<uchar>(i,j)+255);
                            if (t > 255)
                                temp.at<uchar>(i,j)=255;
                            if (t<0)
                                temp.at<uchar>(i,j)=0;
                            else
                                temp.at<uchar>(i,j)=t;
                        }
                    break;
                case 'c'://Complemento
                for (int i = 0; i<img1.rows;i++)
                    for (int j = 0; j<img1.cols;j++){
                        t = img1.at<uchar>(i,j);
                        if (t == 1)
                            temp.at<uchar>(i,j)=0;
                        else
                            temp.at<uchar>(i,j)=1;
                    }

                break;
            default:
                cout<<"\nOperacion inválida.\n";
            }


        }
    return temp;
    }

    Mat promedio(vector<Mat> img){
        int N = img.size();

        if (N < 2){
            cout<<"Error de promedio.";
            return Mat::zeros(1,1,CV_8S);
        }
        Mat suma = Mat::zeros(img[0].rows,img[0].cols,img[0].type());
        for (int i = 0;i<N-1;i++)
            suma = aritmetica(img[i],img[i+1],'s');
        return suma;
    }
    cv::Mat MatrizBinaria(Mat &imagen,int n,int bp = 0){
        //n representa cuantos bits serán nulos de izquierda a derecha
        //bp representa el plano de bits (bits plane) que queremos obtener [1 - 8] [Mas sig - Menos sig]
        Mat img = imagen.clone();
        if (bp == 0){
            for (int i = 0;i<imagen.rows;i++)
                for (int j= 0; j<imagen.cols;j++){
                    bitset<8> bin(imagen.at<uchar>(i,j));
                    for (int b = 7; b > 8 - n-1;b--)
                        bin[b]=0;
                    img.at<uchar>(i,j)=(128)*bin[7]+(64)*bin[6]+(32)*bin[5]+(16)*bin[4]+(8)*bin[3]+(4)*bin[2]+(2)*bin[1]+(1)*bin[0];
                }

        }
        else
        {
            for (int i = 0;i<imagen.rows;i++)
                for (int j= 0; j<imagen.cols;j++){
                    bitset<8> bin(imagen.at<uchar>(i,j));
                    img.at<uchar>(i,j) = 255*bin[bp-1];
                }

        }
        return img;

    }
    void verHistograma(Mat &imagen){
        int altura = imagen.rows;
        Mat hist = pdi::histogram(imagen,256);
        Mat canvas = Mat::ones(altura,256,CV_8U)*255;
        hist.convertTo(hist,CV_32F);
        float norm = hist.at<float>(0);

        for (int i = 1; i< 256;i++){ //Busco el máximo para normalizar.
                if (norm< hist.at<float>(i))
                    norm =hist.at<float>(i);
        }

        for (int i = 0; i < 256; i++) //Ploteo en canvas las lineas
        {
            float mag = hist.at<float>(i);
            line(canvas,Point(i,altura),Point(i,altura-(mag/norm)*altura),Scalar(0,0,0));
        }
        imshow("Histograma",canvas);
    }
    Mat PA(Mat &imagen, int A){
        Mat img = imagen.clone();
        Mat f =PB(imagen,20);
        img = A*img;
        return aritmetica(img,f,'r');
    }
    Mat Morfologia(Mat A,Mat B, char oper){
        /*
            Recibo la imagen en A, en B es el elemento estructurante
            oper son las iniciales de las operaciones morfologicas.
        */
        Mat resultado = Mat::zeros(A.rows,A.cols,A.type());
        switch (oper) {
        case 'd'://Dilatacion
            dilate(A,resultado,B);
            break;
        case 'e'://Erosion
            erode(A,resultado,B);
            break;
        case 'a'://Apertura
            morphologyEx( A, resultado, MORPH_OPEN, B);
            break;
        case 'c'://Cierre
            morphologyEx( A, resultado, MORPH_CLOSE, B);
            break;
        default:
            cout<<"\nERROR MORFOLOGICO\n";
            break;
        }

    return resultado;

    }
    Mat bordes(Mat img,char metodo,float umbral){
        Mat mgx,mgy,gy,gx,R = Mat::zeros(img.rows,img.cols,CV_32FC1);

        mgx=Mat::zeros(3,3,CV_32F);
        mgy=Mat::zeros(3,3,CV_32F);

        switch (metodo) {
        case 'r'://Roberts
            img.convertTo(img,CV_32F,1./255);
            mgx.at<float>(1,1)=-1;
            mgx.at<float>(2,2)=1;
            mgy.at<float>(1,2)=-1;
            mgy.at<float>(2,1)=1;
            break;
        case 'p'://Prewitt
            img.convertTo(img,CV_32F,1./255);
            mgx.at<float>(1,1)=-1;   mgx.at<float>(1,2)=-1;      mgx.at<float>(1,3)=-1;
            mgx.at<float>(3,1)=1;    mgx.at<float>(3,1)=1;       mgx.at<float>(3,1)=1;

            mgy.at<float>(1,1)=-1;   mgy.at<float>(1,3)=1;
            mgy.at<float>(2,1)=-1;   mgy.at<float>(2,3)=1;
            mgy.at<float>(3,1)=-1;   mgy.at<float>(3,3)=1;
            break;
        case 's'://Sobel
/*
            img.convertTo(img,CV_32F,1./255);
            mgx.at<float>(1,1)=-1;   mgx.at<float>(1,2)=-2;      mgx.at<float>(1,3)=-1;
            mgx.at<float>(3,1)=1;    mgx.at<float>(3,1)=2;       mgx.at<float>(3,1)=1;

            mgy.at<float>(1,1)=-1;   mgy.at<float>(1,3)=1;
            mgy.at<float>(2,1)=-2;   mgy.at<float>(2,3)=2;
            mgy.at<float>(3,1)=-1;   mgy.at<float>(3,3)=1;
*/

            Sobel(img,R,CV_32FC1,1,1);
            convertScaleAbs(R,R);
            break;
        case 'l'://Laplaciano
            Laplacian(img,R,CV_16S,3);
            convertScaleAbs(R,R);
            break;
        case 'L'://Laplaciano del Gaussiano
            GaussianBlur(img,img,Size(3,3),0,0);
            Laplacian(img,R,CV_16S,3);
            convertScaleAbs(R,R);
            break;


        default:
            cout<<"\nMetodo ingresado invalido.\n";
            break;
        }
        if (metodo == 'l' || metodo == 'L' || metodo == 's')
            return R;
        gx=pdi::convolve(img ,mgx);
        gy=pdi::convolve(img, mgy);
        imshow("Gx",gx);
        imshow("Gy",gy);
        R=abs(gx)+abs(gy);
        threshold(R,R,umbral,255,THRESH_BINARY);

        return R;
    }
    Mat Trans_Hough(Mat imagen,double thita1, double thita2,int umbral,int &contador){
        Mat R,R1;
        vector<Vec2f> lines;
        //Pide que le pasemos algún gradiente para que sea más fácil detectar bordes
        //Laplacian(im,result,CV_16S,3,1,0,BORDER_DEFAULT);
        //convertScaleAbs(result,result);
        //Queré sabe que pasa?....
        //Lo que pasa es el Laplaciano es muy suceptible al ruido

        //Canny tiene la posta, además R1 es una imagen de 8-bit
        //                T-H T-L
        Canny(imagen, R1, 50, 200);//Leelo del Gonzalez p719 // Detect the edges of the image by using a Canny detector
        cvtColor(imagen, R, CV_GRAY2BGR);
        //HoughLines(InputArray image, OutputArray lines, double rho, double theta, int threshold, double srn=0, double stn=0 )

        HoughLines(R1, lines, 1, CV_PI/180, umbral, 0, 0 );
        //Lines (pi,0i)
        for( size_t i = 0; i < lines.size(); i++ )
        {

            float rho = lines[i][0], theta = lines[i][1];
            float thetagrad=theta*180/CV_PI;//El ángulo me lo da en radianes
            //Pregunto por las rectas que estan en el rango de los bordes de los conos. El origen esta abajo a la izquierda
            if(thetagrad >= thita1 && thetagrad <= thita2){
                contador++;
                Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a*rho, y0 = b*rho;
                pt1.x = cvRound(x0 + 1000*(-b));       pt1.y = cvRound(y0 + 1000*(a));
                pt2.x = cvRound(x0 - 1000*(-b));       pt2.y = cvRound(y0 - 1000*(a));
                line( R, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
        }
        }
        /*Transformada de Hough Probabilista: Tiene la ventaja de que me tira los puntos iniciales y finales de cada recta encontrada en la image
         * La desventaja que tiene es que hay que calcular el angulo en caso de que querramos rectas con un angulo en particular.
            vector<Vec4i> lines;
            HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
            for( size_t i = 0; i < lines.size(); i++ )
            {
              Vec4i l = lines[i];
              line( R, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
            }

        */


        return R;
    }
    Mat Seg_CR(Mat img,Point p,Scalar L, Scalar H){
        Mat Mascara = Mat::zeros(img.rows,img.cols,img.type()),im;
        img.convertTo(im,CV_32F);
        float k;

        floodFill(im,p,256,NULL,L,H);
        for(int i=0;i<im.rows;i++)
            for(int j=0;j<im.cols;j++){
                k=im.at<float>(i,j);
                if(k==256)
                    Mascara.at<uchar>(i,j)=255;
            }
        return Mascara;
    }




    Mat Seg_color(Mat img,int pH,int pS,int vH,int vS){
        //Pasar de RGB a HSI

        Mat mask = Mat::zeros(img.size(),img.type()),img_hsi;
        cvtColor(img,img_hsi,COLOR_BGR2HSV);



        int    H1 = comprobar(pH-vH)
                ,H2 = comprobar(pH+vH)
                ,S1 = comprobar(pS-vS)
                ,S2 = comprobar(pS+vS);

        for (int i = 0; i<img.rows;i++)
            for (int j = 0; j<img.cols;j++){
                Vec3b temp =img_hsi.at<Vec3b>(i,j);
                int H = temp.val[0];
                int S = temp.val[1];
                if ( (H1 < H) && (H < H2) && (S1 < S) && (S < S2)) {
                    mask.at<cv::Vec3b>(i,j)[0]=1; //Blue
                    mask.at<cv::Vec3b>(i,j)[1]=1; //Green
                    mask.at<cv::Vec3b>(i,j)[2]=1; //Red
                }
            }

        Mat final;

        multiply(img,mask,final);

        return mask;

    }
    cv::Mat ruido_gaussiano(cv::Mat im, float mean, float var){
        cv::Mat imr=im.clone(),noise;
        float a;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(mean,var);
        noise=Mat::zeros(im.size(),CV_32F);
        imr.convertTo(imr,CV_32F,1./255);
        for(int i=0;i<im.rows;i++)
            for(int j=0;j<im.cols;j++){
            a=distribution(generator);
            noise.at<float>(i,j)=a;
            imr.at<float>(i,j)+=a;
        }
        imr.convertTo(imr,CV_8UC1,255);
        return imr;
    }
    cv::Mat ruido_gamma(cv::Mat im, float a, float b){
        cv::Mat imr=im.clone(),noise;
        float k;
        std::default_random_engine generator;
        std::gamma_distribution<double> distribution(a,b);
        noise=Mat::zeros(im.size(),CV_32F);
        imr.convertTo(imr,CV_32F,1./255);
        for(int i=0;i<im.rows;i++)
            for(int j=0;j<im.cols;j++){
            k=distribution(generator);
            noise.at<float>(i,j)=k;
            imr.at<float>(i,j)+=k;
        }
        imr.convertTo(imr,CV_8UC1,255);
        return imr;
    }
    cv::Mat ruido_impulsivo(cv::Mat &srcArr, float pa, float pb){
            //pa y pb es un porcentaje con respecto a la cantidad total de pixeles
        RNG rng;
        int amount1=srcArr.rows*srcArr.cols*pa;
        int amount2=srcArr.rows*srcArr.cols*pb;
        for(int counter=0; counter<amount1; ++counter){
            // rng.uniform( 0,srcArr.rows), rng.uniform(0, srcArr.cols)??????
            srcArr.at<uchar>(rng.uniform( 0,srcArr.rows), rng.uniform(0, srcArr.cols)) =0;
        }
        for (int counter=0; counter<amount2; ++counter){
            srcArr.at<uchar>(rng.uniform(0,srcArr.rows), rng.uniform(0,srcArr.cols)) = 255;
        }
        return srcArr;
    }
    cv::Mat ruido_uniforme(cv::Mat im, float min, float max){
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(min,max);
        cv::Mat noise=Mat::zeros(im.size(),CV_32F);
        for(int i=0;i<im.rows;i++)
            for(int j=0;j<im.cols;j++){
            noise.at<float>(i,j)=distribution(generator);
        }
        return noise;
    }
    cv::Mat filtro_contraharmonico(cv::Mat &im,int msize, float Q){
        float acum1,acum2;
        cv::Mat result=im.clone();
        int mside=(msize-1)/2;
        for(int i=mside;i<im.rows-mside;i++)
            for(int j=mside;j<im.cols-mside;j++){
            acum1=0;acum2=0;
            for(int ii=i-mside;ii<=i+mside;ii++){
                for(int jj=j-mside;jj<=j+mside;jj++){
                    acum1+=pow(im.at<uchar>(ii,jj),Q+1);
                    acum2+=pow(im.at<uchar>(ii,jj),Q);
                }
            }
            result.at<uchar>(i,j)=acum1/acum2;
        }
        return result;

    }
    cv::Mat filtro_geometrico(cv::Mat &im,int msize){
        double acum;
        cv::Mat result=im.clone();
        int mside=(msize-1)/2;
        for(int i=mside;i<im.rows-mside;i++)
            for(int j=mside;j<im.cols-mside;j++){
                acum=1;
                for(int ii=i-mside;ii<=i+mside;ii++){
                    for(int jj=j-mside;jj<=j+mside;jj++){
                        acum*=im.at<uchar>(ii,jj);
                    }
                }
                acum=pow(acum,1./(msize*msize));
                result.at<uchar>(i,j)=acum;
        }
        return result;

    }
    cv::Mat filtro_mediana(cv::Mat &im,int msize){
            uchar v[msize*msize];
        cv::Mat result=im.clone();
        int k,m=((msize*msize)-1)/2,mside=(msize-1)/2;
        for(int i=mside;i<im.rows-mside;i++)
            for(int j=mside;j<im.cols-mside;j++){
            k=0;
            for(int ii=i-mside;ii<=i+mside;ii++){
                for(int jj=j-mside;jj<=j+mside;jj++){
                    v[k++]=im.at<uchar>(ii,jj);
                }
            }
            std::sort(v, v+(msize*msize));
            result.at<uchar>(i,j)=v[m];
        }
        return result;
    }
    cv::Mat filtro_puntoMedio(cv::Mat &im,int msize){
        uchar v[msize*msize];
        cv::Mat result=im.clone();
        int k,t=(msize*msize)-1,mside=(msize-1)/2;
        for(int i=mside;i<im.rows-mside;i++)
            for(int j=mside;j<im.cols-mside;j++){
            k=0;
            for(int ii=i-mside;ii<=i+mside;ii++){
                for(int jj=j-mside;jj<=j+mside;jj++){
                    v[k++]=im.at<uchar>(ii,jj);
                }
            }
            std::sort(v, v+(t+1));
            result.at<uchar>(i,j)=0.5*(v[0]+v[t]);
        }
        return result;
    }
    cv::Mat filtro_AlphaRec(cv::Mat &im,int msize, int d){
        uchar v[msize*msize];
        cv::Mat result=im.clone();
        float t=1./((msize*msize)-d);
        int a,k,t2=(msize*msize)-1,mside=(msize-1)/2;
        for(int i=mside;i<im.rows-mside;i++)
            for(int j=mside;j<im.cols-mside;j++){
            k=0;
            for(int ii=i-mside;ii<=i+mside;ii++){
                for(int jj=j-mside;jj<=j+mside;jj++){
                    v[k++]=im.at<uchar>(ii,jj);
                }
            }
            std::sort(v, v+(msize*msize));
            a=0;
            for(int i=d/2;i<=t2-(d/2);i++)
                a=a+v[i];
            result.at<uchar>(i,j)=floor(t*a);
        }
        return result;
    }
    cv::Mat filtro_Weiner(cv::Mat H, cv::Mat G, double K){
        cv::Mat lside,x[2],magnitud,result,y[2],rside_real,filteredspec;
        split(H,x);
        cv::magnitude(x[0], x[1], magnitud);
        rside_real=(magnitud/(magnitud+K));
        y[0]=((1/x[0])*rside_real); //real part
        y[1]=(1/x[1]); 				//imag part
        merge(y,2,lside);
        cv::mulSpectrums(lside, G, filteredspec, cv::DFT_ROWS, true ); //multiply conjugate
        cv::idft(filteredspec, result, cv::DFT_REAL_OUTPUT | cv::DFT_SCALE); //transformada inversa
        return result;
    }

    cv::Mat notch_butterworth(size_t rows, size_t cols, double v[][4],int filas){ //v is mx4 [u,v,order,corte]
        //corte = w en imagen de lado 1
        //1 \over 1 + {D \over w}^{2n}
        cv::Mat magnitud = cv::Mat::ones(rows, cols, CV_32F);
        double corte;
        int m=filas;
        for(int f=0;f<m;f++){
        corte=(double)v[f][3];
        corte *= rows;
            for(size_t K=0; K<rows; ++K)
                for(size_t L=0; L<cols; ++L){
                double d2 = pdi::distance2(K+.5, L+.5, (rows/2)-v[f][1], (cols/2)-v[f][0]);
                double dc2 = pdi::distance2(K+.5, L+.5, (rows/2)+v[f][1], (cols/2)+v[f][0]);
                magnitud.at<float>(K,L) *=( 1.0/(1 + std::pow((corte*corte)/d2, v[f][2]) ) ) * ( 1.0/(1 + std::pow((corte*corte)/dc2, v[f][2]) ) );
            }
        }

        pdi::centre(magnitud);
        return magnitud;
    }
    /*cv::Mat pseudo_filter(cv::Mat H, cv::Mat G, double type){

        IplImage* image_Re1,*dft_C,*image_Im1,*image_Re2,*image_Im2,*dft_H,*img,*restored;
        CvMat tmp3;
        IplImage copy = G;
        IplImage* dft_A = &copy;
        IplImage copy2 = H;
        IplImage* dft_B = &copy2;

        // compute the DFTs of the kernel (DFT_B) and the blurred image (DBF_A)
        cvDFT( dft_A, dft_A, CV_DXT_FORWARD);
        cvDFT( dft_B, dft_B, CV_DXT_FORWARD);
        double threshold=0.1; // threshold for H

        // the first type is the inverse fitlering
        if (type == 1) {
            printf("...performing inverse filtering\n");
            // dividing the transforms
            cvDiv(dft_A, dft_B, dft_C, 1);
        }
        // the second type is the pseudo-inverse filtering
        else {
            printf("...prepare kernel for pseudo-inverse filtering\n");
            // will try to access the real values in order to see if value is above a

            cvSplit( dft_B, image_Re1, image_Im1,NULL, NULL);
            // pointers to access the data into the real and imaginary matrices
            uchar * dRe1 = (uchar *)image_Re1->imageData;
            uchar * dIm1 = (uchar *)image_Im1->imageData;
            int width  = image_Re1->width;
            int height = image_Re1->height;
            int step = image_Re1->widthStep;
            image_Re2 = cvCreateImage(cvGetSize(image_Re1), IPL_DEPTH_32F, 1);
            image_Im2 = cvCreateImage(cvGetSize(image_Im2), IPL_DEPTH_32F, 1);
            // pointers to access the data into the real and imaginary matrices
            // it will be the resulting pseudo-inverse filter
            uchar * dRe2 = (uchar *)image_Re2->imageData;
            uchar * dIm2 = (uchar *)image_Im2->imageData;

            printf("...building kernel for pseudo-inverse filtering\n");
            for ( int i = 0; i < height; i++ )  {
                for ( int j = 0; j < width; j++ ) {
                    // generate the 1/H(i,j) value
                    if (dRe1[i * step + j] > threshold) {
                        float realsq = dRe1[i * step + j]*dRe1[i * step + j];
                        float imagsq = dIm1[i * step + j]*dIm1[i * step + j];

                        dRe2[i * step + j] = dRe1[i * step + j] / (realsq + imagsq);
                        dIm2[i * step + j] = -1 * (dIm1[i * step + j] / (realsq + imagsq));
                    }
                    else {
                        dRe2[i * step + j] = 0;
                        dIm2[i * step + j] = 0;
                    }
                }
            }
            printf("...merging final kernel\n");
            cvMerge(image_Re2, image_Im2, 0, 0, dft_B);
            printf("...performing pseudo-inverse filtering\n");
            cvMulSpectrums(dft_A, dft_B, dft_C, 1);
        }
        printf("...performing IDFT\n");
        cvDFT(dft_C, dft_H, CV_DXT_INV_SCALE, 1);

        printf("...getting size\n");
        cvGetSubRect(dft_H, &tmp3, cvRect(0, 0, img->width, img->height));

        printf("......(%d, %d) - (%d, %d)\n", tmp3.cols, tmp3.rows, restored->width, restored->height);

        cvSplit( &tmp3, image_Re1, image_Im1, 0, 0 );

        cvNamedWindow("re", 0);
        cvShowImage("re", image_Re2);
        cvWaitKey(0);

        printf("...copying final image\n");
        // error is in the line below
        IplImage* imf=   cvCreateImage(cvGetSize(image_Re1), IPL_DEPTH_32F, 1);
        cvCopy(image_Re1, imf, NULL);

    }*/

    void HSI_p (Mat imagen,Point p,int &H,int &S,int &I){
        //Obtengo los valores de Hue, Saturation e Intensity según un punto
        Mat img_hsi;
        cvtColor(imagen,img_hsi,COLOR_BGR2HSV);
        Vec3b color = img_hsi.at<Vec3b>(p.y,p.x);

        H = color.val[0];
        S = color.val[1];
        I = color.val[2];
    }
  /*  static void mirarHSI( int event, int x, int y, int, void* )
    {//Es un callback para obtener en consola los valores de HSI
        //NOTA: Pegarlo arriba del main y poner la imagen como global
        //setMouseCallback("Imagen original",mirarHSI);

        if( event == EVENT_LBUTTONDOWN ){
            //int h,s,i;
            //HSI_p(imagen,Point(x,y),h,s,i);
            //cout<< "H = "<<h<<", S = "<<s<<", I = "<<i<<endl;
        }

    }*/
    vector<vector<Point>> componentes_conectadas_etiquetado (const Mat &imagen,int indice = -1){

        //Find contours
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(imagen, contours, hierarchy , CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );

        //Draw contours
        RNG rng(12345);
        Mat drawing = Mat::zeros( imagen.size(), CV_8UC3 );

        vector<vector<Point> > contours_poly( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );
        int N = contours.size();
        for( int i = 0; i < N; i++ )
        {
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
        }


        Mat img2 = imagen.clone();

        for( int i = 0; i< N; i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            circle( img2, center[i], (int)radius[i], color, 2, 8, 0 );
        }






        vector<double> areas(contours.size() );
        double areamax = 0;
        for( int i = 0; i<N; i++ ) // iterate through each contour.
        {
            areas[i] = contourArea( contours[i], false);
            if (areas[i]>areamax){//Un chamuyito para obtener la forma más grande de la imagen
                areamax =areas[i];
                indice = i;
            }

        }



        //imshow("Imagen", img2);
        imshow("Contornos detectados", drawing);
        std::cout<<"Cantidad de componentes: "<<contours.size()<<std::endl;


        return contours;
    }
    int min_max (vector<Point> c, bool col,bool m){
        //Busca el minimo y máximo de un vector de puntos
        //col = 0 -> X
        //col = 1 -> Y
        //m = 0 el máximo
        //m = 1 el minimo
        int t;
        if (m == 0){ //Quiere decir el maximo
            if (!col){ //Si es x

                int N = c.size();
                if (N == 0)
                    return 0;
                t = c[0].x;
                if (N == 1)
                    return t;

                for (int i = 2; i<N;i++){
                     if (c[i].x> t)
                        t = c[i].x;

                }
                }
            else {
                int N = c.size();
                if (N == 0)
                    return 0;
                t = c[0].y;
                if (N == 1)
                    return t;
                for (int i = 2; i<N;i++)
                    if (c[i].y> t)
                        t = c[i].y;

            }
        }
        else{
            if (col == 0){ //Si es x
                int N = c.size();
                if (N == 0)
                    return 0;
                t = c[0].x;
                if (N == 1)
                    return t;
                for (int i = 2; i<N;i++)
                    if (c[i].x< t)
                        t = c[i].x;
                }
            else {
                int N = c.size();
                if (N == 0)
                    return 0;
                t = c[0].y;
                if (N == 1)
                    return t;
                for (int i = 2; i<N;i++)
                    if (c[i].y< t)
                        t = c[i].y;

            }

        }
        return t;
    }
    void rectangulo (vector<Point> c,int &xmin,int &xmax, int &ymin, int &ymax){
        //Segun un contorno busco los limites horizontales y verticales (Rectangulo)

        xmax = min_max(c,0,0);
        xmin = min_max(c,0,1);
        ymax = min_max(c,1,0);
        ymin = min_max(c,1,1);
        if (!(xmin<xmax) && !(ymin<ymax))
            cout<<"\nFuncion Recorte : Limites invalidos.\n";

    }
    int brillo(Mat &img){
        int f = img.rows;
        int c = img.cols;
        int N = f*c,S = 0;
        for (int i= 0;i<f;i++)
            for (int j= 0;j<c;j++)
                S += img.at<uchar>(i,j);
        return S/N;
    }

    void ComparadorHistograma(vector<Mat> imagenes, Mat comparador){
        //Se recibe en un vector que contiene la base de datos
        //La imagen a comparar según el histograma
        int N = imagenes.size();
        double k;

        std::stringstream convert;
        cv:: Mat histocomp,histo_i;


        histocomp=pdi::histogram(comparador,256);
        for(int i=0;i<N;i++){
            histo_i=pdi::histogram(imagenes[i],256);
            /*CV_COMP_CORREL Correlation
            CV_COMP_CHISQR Chi-Square
            CV_COMP_INTERSECT Intersection
            CV_COMP_BHATTACHARYYA Bhattacharyya distance
            CV_COMP_HELLINGER Synonym for CV_COMP_BHATTACHARYYA*/

            k=compareHist(histocomp,histo_i,CV_COMP_CORREL);
            //int c1= 0,c2= 0,c3= 0,c4= 0;
            if(k>0.3){ // ver umbral
                convert<<i;
                namedWindow(convert.str());
                imshow(convert.str(),imagenes[i]);
                convert.str("");


            }

        }



    }

}

#endif
