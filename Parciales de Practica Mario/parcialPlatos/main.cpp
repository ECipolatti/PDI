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

    if (disp_bgr){
        namedWindow("B", 0);
        namedWindow("G", 0);
        namedWindow("R", 0);
        imshow("B", bgr_channels[0]);
        imshow("G", bgr_channels[1]);
        imshow("R", bgr_channels[2]);
    }

    if (disp_hsv){
        namedWindow("H", 0);
        namedWindow("S", 0);
        namedWindow("V", 0);
        imshow("H", hsv_channels[0]);
        imshow("S", hsv_channels[1]);
        imshow("V", hsv_channels[2]);
    }

    return channels;
}

int main(){

    Mat src = loadimg("Platos05.jpg", CV_LOAD_IMAGE_COLOR, 0);

    Point3i delaCasa = Point3i(50, 140, 160);
    Point3i colorZapallo = Point3i(0, 80, 220);
    int Radio = 20;
    Mat SopaSegmentadaZ = colorDistance(src, colorZapallo, Radio);
    Mat SopaSegmentadaCasa = colorDistance(src, delaCasa, Radio);
    Mat grisesZ, grisesCasa;
    cvtColor(SopaSegmentadaZ, grisesZ, CV_BGR2GRAY);
    cvtColor(SopaSegmentadaCasa, grisesCasa, CV_BGR2GRAY);
    Mat binarizadaZ, binarizadaCasa;
    threshold(grisesZ, binarizadaZ, 1, 255, THRESH_BINARY);
    threshold(grisesCasa, binarizadaCasa, 1, 255, THRESH_BINARY);

    // Defino el tipo de Sopa. Si el porcentaje de blancos es mayor al 10% es de Zapallo
    float Z = porcentaje(binarizadaZ, 255, 0);
    float C = porcentaje(binarizadaCasa, 255, 0);
    cout << "porcentaje Zapallo: " << Z*100 << endl;
    cout << "porcentaje Casa: " << C*100 << endl;
    if (Z*100 > C*100){
        cout << "SOPA DE ZAPALLO" << endl;
    }else {
        cout << "SOPA DE LA CASA" << endl;
    }



// ----------- SIGO CON EL RESTO DEL CODIGO PARA RESOLVER LOS OTROS ITEMS DEL ENUNCACIADO
    vector<Mat> channels = canales("../imgs/Platos05.jpg", 0, 1);
    Mat canalBinarizado;

    namedWindow("Canal V", 0);
    imshow("Canal V", channels[5]);

    threshold(channels[5], canalBinarizado, 110, 255, THRESH_BINARY_INV);
    namedWindow("Canal V binarizado", 0);
    imshow("Canal V binarizado", canalBinarizado);

    // Cuento la cantidad de moscas en la escena
    Mat EE = getEE(1);
    Mat aux = canalBinarizado.clone();
    aux = Morfologia(aux, EE, 'e');
    aux = Morfologia(aux, EE, 'e');
    aux = Morfologia(aux, EE, 'e');
    aux = Morfologia(aux, EE, 'e');
    aux = Morfologia(aux, EE, 'd');
    aux = Morfologia(aux, EE, 'a');
    aux = Morfologia(aux, EE, 'a');

    namedWindow("Canal V binarizazo y Dilatado", 0);
    imshow("Canal V binarizazo y Dilatado", aux);

    Mat aux2 = aux.clone();
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( aux2, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cout << "Cantidad de Moscas en la Escena: " << contours.size() << endl;

    // Cuento la cantidad de moscas en el PLATO ENTERO

    Mat srcGray;
    cvtColor(src, srcGray, CV_BGR2GRAY);
    namedWindow("Original en Gris", 0);
    imshow("Original en Gris", srcGray);

    Mat srcGrayBinarizada;
    threshold(srcGray, srcGrayBinarizada, 200, 255, THRESH_BINARY);
    Mat EE2 = getEE(1);
    Mat MoscasEnPlato = srcGrayBinarizada.clone();
    MoscasEnPlato = Morfologia(MoscasEnPlato, EE, 'e');

    namedWindow("Original en Gris y Binarizada", 0);
    imshow("Original en Gris y Binarizada", MoscasEnPlato);

    Mat Nueva = MoscasEnPlato.clone();
    vector<vector<Point>> contours2;
    vector<Vec4i> hierarchy2;
    findContours(MoscasEnPlato, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// Get the moments
    vector<Moments> mu(contours2.size() );
    for( size_t i = 0; i < contours2.size(); i++ )
    { mu[i] = moments( contours2[i], false ); }

    ///  Get the mass centers:
    vector<Point2f> mc( contours2.size() );
    for( size_t i = 0; i < contours2.size(); i++ ){
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }

    float Area = 0.f;
    int IndiceArea;
    for (int i = 0; i < contours2.size(); i++){
        float pepe = contourArea(contours2[i]);
        if (pepe > Area){
            Area = pepe;
            IndiceArea = i;
        }
    }

    Scalar color = Scalar(255, 255, 255);
    Point2f p = Point2f(0, 0);
    floodFill(Nueva, mc[IndiceArea], color);
    floodFill(Nueva, p, color);
    threshold(Nueva, Nueva, 1, 255, THRESH_BINARY_INV);

    Mat EE3 = getEE(1);
    Mat Nueva2 = Nueva.clone();
    Nueva2 = Morfologia(Nueva2, EE, 'd');
    Nueva2 = Morfologia(Nueva2, EE, 'd');
    Nueva2 = Morfologia(Nueva2, EE, 'd');
    Nueva2 = Morfologia(Nueva2, EE, 'd');
    Nueva2 = Morfologia(Nueva2, EE, 'd');
    Nueva2 = Morfologia(Nueva2, EE, 'd');
    Nueva2 = Morfologia(Nueva2, EE, 'd');

    Mat Nueva3 = Nueva2.clone();
    vector<vector<Point>> contours3;
    vector<Vec4i> hierarchy3;
    findContours(Nueva2, contours3, hierarchy3, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    cout << "Cantidad de moscas en el borde del plato: " << contours3.size() << endl;

    // Cuento la cantidad de moscas en la sopa

    Mat ImagenNegra = Mat::zeros(src.size(), CV_8UC1);
    circle(ImagenNegra, mc[IndiceArea], 313, color, -1); // con -1 rellena el circulo del Color

    ImagenNegra = ImagenNegra/255;
    Mat MoscaEnSopa;
    multiply(aux, ImagenNegra, MoscaEnSopa);


    vector<vector<Point>> contours4;
    vector<Vec4i> hierarchy4;
    findContours(MoscaEnSopa, contours4, hierarchy4, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    imshow("LALALA", MoscaEnSopa);

    cout << "En la sopa hay: " << contours4.size() << " moscas" << endl;

    if ((Z*100 > C*100) && (contours4.size() > 3)){
        cout << "SOPA DE ZAPALLO MAL SERVIDA" << endl;
    }else if ((Z*100 > C*100) && (contours4.size() <= 3)){
        cout << "SOPA DE ZAPALLO BIEN SERVIDA" << endl;
    }else if ((Z*100 < C*100) && (contours4.size() > 4)){
        cout << "SOPA DE LA CASA MAL SERVIDA" << endl;
    }else if ((Z*100 < C*100) && (contours4.size() <= 4)){
        cout << "SOPA DE LA CASA BIEN SERVIDA" << endl;
    }

    waitKey();
    return 0;

}
