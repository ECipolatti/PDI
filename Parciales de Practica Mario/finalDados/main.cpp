#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <../include/pdi_functions.h>
#include <../include/my_functions.h>

using namespace cv;
using namespace std;
using namespace pdi;

Mat SegmentacionHSV(Mat img, int Hmin, int Hmax, int Smin, int Smax, int Vmin, int Vmax){
    vector<Mat> CanalaesHSV(3);
    Mat img_hsi;
    Mat mascara = cv::Mat::zeros(img.size(),img.type());
    cvtColor(img,img_hsi,COLOR_BGR2HSV);
    split(img_hsi,CanalaesHSV);
    imshow("Canal H", CanalaesHSV[0]);
    imshow("Canal S", CanalaesHSV[1]);
    imshow("Canal V", CanalaesHSV[2]);

    for (int i = 0; i<img.rows;i++)
        for (int j = 0; j<img.cols;j++){
            int H = (int)CanalaesHSV[0].at<uchar>(i,j);
            int S = (int)CanalaesHSV[1].at<uchar>(i,j);
            int V = (int)CanalaesHSV[2].at<uchar>(i,j);
            if ((Hmin<H)&&(H<Hmax) && (Smin<S)&&(S<Smax) && (Vmin<V)&&(V<Vmax)){
                mascara.at<cv::Vec3b>(i,j)[0]=1; //Blue
                mascara.at<cv::Vec3b>(i,j)[1]=1; //Green
                mascara.at<cv::Vec3b>(i,j)[2]=1; //Red
            }
        }
    Mat imgfinal;
    multiply(img,mascara,imgfinal);
    Mat imgFinalRGB;
    cvtColor(imgfinal, imgFinalRGB, COLOR_HLS2BGR);
    return imgFinalRGB;
}

// SEGMENTADOR EN RGB
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


int contar_puntos_dado(Mat dado_color){
    Mat dado_gris;
    cvtColor(dado_color, dado_gris, CV_BGR2GRAY);

    Mat bin;

    threshold(dado_gris, bin, 200, 255, CV_THRESH_BINARY);

    imshow("dado a color", dado_color);
    imshow("dado escala de grises", dado_gris);
    imshow("puntos", bin);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( bin, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    return contours.size();
}



int main(){

//    Mat src = loadimg("Dados01-3dados.jpg", CV_LOAD_IMAGE_COLOR, 1);
    Mat src = loadimg("Dados02-3dados_azul.jpg", CV_LOAD_IMAGE_COLOR, 1);
//    Mat src = loadimg("Dados03-3dados.jpg", CV_LOAD_IMAGE_COLOR, 1);
//    Mat src = loadimg("Dados04-3dados.jpg", CV_LOAD_IMAGE_COLOR, 1);

    Mat hsv;
    vector<Mat> hsv_channels;
    cvtColor(src, hsv, COLOR_BGR2HSV);
    split(hsv, hsv_channels);

    Mat H = hsv_channels[0].clone();

    imshow("segmentada", H);

    Mat bin;
    threshold(H, bin, 100, 255, CV_THRESH_BINARY);


    setMouseCallback("segmentada", pinfo, &H);


    imshow("bin", bin);


    Mat EE = getEE(1);
    Mat morph = bin.clone();
    morph = Morfologia(morph, EE, 'd');
    morph = Morfologia(morph, EE, 'd');
    morph = Morfologia(morph, EE, 'd');
    morph = Morfologia(morph, EE, 'd');
    morph = Morfologia(morph, EE, 'e');
    morph = Morfologia(morph, EE, 'e');
    morph = Morfologia(morph, EE, 'e');
    morph = Morfologia(morph, EE, 'e');
    morph = Morfologia(morph, EE, 'c');
    morph = Morfologia(morph, EE, 'c');
    morph = Morfologia(morph, EE, 'c');

    imshow("morph", morph);

    Mat mascara = morph.clone();
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( mascara, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    morph = morph/255;
    cvtColor(morph, morph, CV_GRAY2BGR);

    Mat imgfinal;
    multiply(src,morph,imgfinal);
    imshow("SOLO DADOS", imgfinal);

    Rect rect;
    Mat dado0, dado1, dado2;

    rect = boundingRect(contours[0]);
    dado0 = imgfinal(rect);

    rect = boundingRect(contours[1]);
    dado1 = imgfinal(rect);

    rect = boundingRect(contours[2]);
    dado2 = imgfinal(rect);

    /// La cantidad de la imagen la sabemos con el size de contour
    cout << "Hay " << contours.size() << " en la imagen" << endl;
    cout << "En el primer dado salio: " << contar_puntos_dado(dado0) << endl;
    cout << "En el segundo dado salio: " << contar_puntos_dado(dado1) << endl;
    cout << "En el tercer dado salio: " << contar_puntos_dado(dado2) << endl;
    cout << "La suma total es de: " << (contar_puntos_dado(dado0)+ contar_puntos_dado(dado1)+
                                        contar_puntos_dado(dado2)) << endl;



    waitKey();
    return 0;
}
