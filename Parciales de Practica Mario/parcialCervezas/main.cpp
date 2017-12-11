#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <../include/pdi_functions.h>
#include <../include/my_functions.h>

using namespace cv;
using namespace std;
using namespace pdi;



int main(){
    //***************************************************************//
    // Falta unificar el codigo. Primero defino el tipo de cerveza. Despues hago una logica
    // para las cervezas rubias y por ultimo hago para las cervezas negras. Lo que falta es
    // juntar todo para que haga cada cosa dependiendo del tipo de cerveza
    //***************************************************************//


    // *************   Defino el tipo de cerveza: RUBIA o NEGRA *********************
//    Point3i color = Point3i(17,16,25);
//    int radio = 116;
//    Mat seg = colorDistance(src, color, radio);
//    cvtColor(seg, seg, CV_BGR2GRAY);
//    threshold(seg, seg, 1, 255, THRESH_BINARY);

//    imshow("seg", seg);
//    float f = porcentaje(seg, 255, 0);
//    int tipo = 0;
//    if (f*100 < 10){
//        tipo = 1;
//        cout << "La cerveza es Rubia" << endl;
//    }else{
//        tipo = 2;
//        cout << "La cerveza es Negra" << endl;
//    }

    // *******************  CERVEZAS RUBIAS  *************************
//    vector<Mat> channels = canales("../imgs/13.jpg", 0, 0);
//    Mat src = imread("../imgs/13.jpg", CV_LOAD_IMAGE_COLOR);
//    Point3i color = Point3i(255,255,255);
//    int radio = 14;
//    Mat seg = colorDistance(src, color, radio);
//    imshow("SEG", seg);
//    cvtColor(seg, seg, CV_BGR2GRAY);
//    threshold(seg, seg, 1, 255, THRESH_BINARY_INV);
//    Mat EE = getEE(2);
//    Mat Vaso = seg.clone();
//    Vaso = Morfologia(Vaso, EE, 'e');
//    Vaso = Morfologia(Vaso, EE, 'e');
//    Vaso = Morfologia(Vaso, EE, 'e');
//    Vaso = Morfologia(Vaso, EE, 'e');
//    Vaso = Morfologia(Vaso, EE, 'e');
//    imshow("Vaso Binarizado", Vaso);

//    Mat vasoEntero = mult_mask(src, Vaso);
//    imshow("Vaso Entero", vasoEntero);


//    Mat Liquido = channels[4];
//    threshold(Liquido, Liquido, 50, 255, THRESH_BINARY);
//    imshow("Liquido", Liquido);

//    Mat sinLiquido = mult_mask(src, Liquido);
//    imshow("Sin Liquido", sinLiquido);

//    Mat espumaYbase = vasoEntero - sinLiquido;
//    cvtColor(espumaYbase, espumaYbase, CV_BGR2GRAY);
//    threshold(espumaYbase, espumaYbase, 1, 255, THRESH_BINARY);
//    imshow("espumaYbase", espumaYbase);
//    Mat EE2 = getEE(1);
//    Mat morph = espumaYbase.clone();
//    morph = Morfologia(morph, EE2, 'c');
//    morph = Morfologia(morph, EE2, 'c');
//    morph = Morfologia(morph, EE2, 'c');
//    morph = Morfologia(morph, EE2, 'e');
//    morph = Morfologia(morph, EE2, 'e');
//    morph = Morfologia(morph, EE2, 'd');
//    imshow("Morph", morph);

//    Mat espuma = morph(Rect(0, 0, morph.cols, morph.rows/2));
//    imshow("Solo Espuma", espuma);

//    // Calculo el Area del vaso entero con porron y espuma. Luego calculo el Area
//    // de la espuma y saco el porcentaje de espuma. AreaEspuma/AreaVaso
//    vector<vector<Point>> contours = count_element(Vaso);
//    vector<float> areas = get_areas(contours);
//    cout << "TAMANO DEL CONTOURS DEL VASO " << contours.size() << endl;
//    cout << areas.size() << endl;

//    vector<vector<Point>> contours2 = count_element(espuma);
//    vector<float> areas2 = get_areas(contours2);
//    cout << "TAMANO DEL CONTOURS DE LA ESPUMA " << contours2.size() << endl;
//    cout << areas2.size() << endl;

//    cout << "PORCENTAJE DE ESPUMA: " << (areas2[0]/areas[0])*100 << endl;


    // *****************      CERVEZAS NEGRAS       **************************
//    segmentacionBGR_trackbar("../imgs/31.jpg");
    Mat src = imread("../imgs/31.jpg", CV_LOAD_IMAGE_COLOR);
    Point3i color = Point3i(255,255,255);
    int radio = 15;
    Mat seg = colorDistance(src, color, radio);
    cvtColor(seg, seg, CV_BGR2GRAY);
    threshold(seg, seg, 1, 255, THRESH_BINARY_INV);
    Mat EE3 = getEE(2);
    Mat img = seg.clone();
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'c');
    img = Morfologia(img, EE3, 'd');
    img = Morfologia(img, EE3, 'd');
    img = Morfologia(img, EE3, 'e');
    img = Morfologia(img, EE3, 'e');
    img = Morfologia(img, EE3, 'e');
    imshow("seg", img);
    Mat vasoEntero = mult_mask(src,img);
    imshow("Vaso Entero", vasoEntero);


    Point3i color2 = Point3i(20,20,40);
    int radio2 = 180;
    Mat seg2 = colorDistance(src, color2, radio2);
    imshow("seg2", seg2);
    Mat Liquido = seg2.clone();
    cvtColor(Liquido, Liquido, CV_BGR2GRAY);
    threshold(Liquido, Liquido, 1, 255, THRESH_BINARY);
    imshow("Liquido", Liquido);
    Rect R = Rect(0, Liquido.rows-40, Liquido.cols, Liquido.rows);
    rectangle(Liquido, R, Scalar(0,0,0), 25, 8);
    imshow("Liquido sin Base", Liquido);
    Mat soloLiquido = mult_mask(src, Liquido);

    Mat espumaYbase = vasoEntero - soloLiquido;
    imshow("Espuma y Base", espumaYbase);
    cvtColor(espumaYbase, espumaYbase, CV_BGR2GRAY);
    threshold(espumaYbase, espumaYbase, 1, 255, THRESH_BINARY);
    imshow("Espuma y Base Binarizado", espumaYbase);
    Mat EE4 = getEE(2);
    Mat aux = espumaYbase.clone();
    aux = Morfologia(aux, EE4, 'e');
    imshow("espuma y Base Morph", aux);
    Mat espuma = aux(Rect(0, 0, aux.cols, aux.rows/2));
    imshow("Solo Espuma", espuma);

    // Calculo el Area del vaso entero con porron y espuma. Luego calculo el Area
    // de la espuma y saco el porcentaje de espuma. AreaEspuma/AreaVaso
    vector<vector<Point>> contours3 = count_element(img);
    vector<float> areas3 = get_areas(contours3);
    cout << "TAMANO DEL CONTOURS DEL VASO " << contours3.size() << endl;
    cout << areas3.size() << endl;

    vector<vector<Point>> contours4 = count_element(espuma);
    vector<float> areas4 = get_areas(contours4);
    cout << "TAMANO DEL CONTOURS DE LA ESPUMA " << contours4.size() << endl;
    cout << areas4.size() << endl;

    cout << "PORCENTAJE DE ESPUMA: " << (areas4[0]/areas3[0])*100 << endl;

    waitKey(0);

    return 0;
}
