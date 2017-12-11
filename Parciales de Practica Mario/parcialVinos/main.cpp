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
    Mat src = loadimg("blanco_5.jpg", CV_LOAD_IMAGE_COLOR, 1);

    // Defino el tipo de Vino: Blanco o Tinto
    Point3i color = Point3i(60, 75, 250);
    int radio = 80;
    Mat vino = colorDistance(src, color, radio);
//    imshow("TIPO DE VINO", vino);
    Mat vinoGris;
    cvtColor(vino, vinoGris, CV_BGR2GRAY);
//    imshow("Vino GRIS", vinoGris);
    threshold(vinoGris, vinoGris, 100, 255, THRESH_BINARY);
//    imshow("VINO BINARIZAO", vinoGris);

//    float x = porcentaje(vinoGris, 255, 1);
//    cout << x*100 << endl;
//    if (x*100 > 1.3){
//        cout << "VINO TINTO" << endl;
//    }else{
//        cout << "VINO BLANCO" << endl;
//    }

    // Busco la altura de la copa
    vector<Mat> channels = canales("../imgs/blanco_5.jpg", 0, 0);
    Mat canalB = channels[0].clone();
    threshold(canalB, canalB, 220, 255, THRESH_BINARY_INV);
//    imshow("CANAL B BIARIZADO", canalB);
    Mat EE = getEE(2);
    Mat aux = canalB.clone();
    aux = Morfologia(aux, EE, 'd');
    aux = Morfologia(aux, EE, 'c');
//    imshow("B MORFOLOGIZADO", aux);

    Mat aux2 = aux.clone();
    vector<vector<Point>> contours = count_element(aux);
    int index = biggest_contour(contours);

    Rect rect;
    Mat copa;
    rect = boundingRect(contours[index]);
    copa = aux2(rect);
//    imshow("SOLO LA COPA", copa);
    cout << "La copa tiene una altura de: " << copa.rows << endl;

    // Busca la altura del liquido
    Mat canalS = channels[4].clone();
//    imshow("CANAL S", canalS);
    threshold(canalS, canalS, 20, 255, THRESH_BINARY);
//    imshow("CANAL S BINARIZADO", canalS);
    Mat EE2 = getEE(2);
    Mat morph = canalS.clone();
    morph = Morfologia(morph, EE2, 'd');
    morph = Morfologia(morph, EE2, 'c');
//    morph = Morfologia(morph, EE2, 'c');
//    morph = Morfologia(morph, EE2, 'e');
//    imshow("MORPH S", morph);

    Mat aux3 = morph.clone();
    vector<vector<Point>> contours2 = count_element(morph);
    int index2 = biggest_contour(contours2);
    Rect rect2;
    Mat liquido;
    rect2 = boundingRect(contours2[index2]);
    liquido = aux3(rect2);
//    imshow("LIQUIDO SOLO", liquido);
    cout << "La altura del liquido es de: " << liquido.rows << endl;

    // Ahora debo verificar si la copa esta bien servida o no.
    // 50 % para vinos tintos y 30 % para vinos blancos
    int altoCopa = copa.rows;
    int altoLiquido = liquido.rows;
//    float volumen = (altoLiquido * 100)/altoCopa;
//    cout << "VOLUMEN" << volumen << endl;

    float x = porcentaje(vinoGris, 255, 1);
    float umbralB = (30 * altoCopa)/100;
    float umbralT = (50 * altoCopa)/100;
//    cout << x*100 << endl;
    if (x*100 > 1.3){
        if (altoLiquido <= umbralT){
            cout << "VINO TINTO -- Bien Servido" << endl;
        }else{
            cout << "VINO TINTO -- Servido en Exceso" << endl;
        }
    }else if(x*100 < 1.3){
        if(altoLiquido <= umbralB){
            cout << "VINO BLANCO -- Bien Servido" << endl;
        }else{
            cout << "VINO BLANCO -- Servido en Exceso" << endl;
        }
    }

    waitKey();
    return 0;
}
