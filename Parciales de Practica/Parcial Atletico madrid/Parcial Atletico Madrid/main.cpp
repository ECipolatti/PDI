#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "FuncionesMias.h"
#include "Funciones.h"
#include <vector>

using namespace cv;
using namespace std;
using namespace pdi;

vector<Mat> carga_db(){
	vector<Mat> V;
	for(int i=1;i<22;i++){
		stringstream sstm;
		sstm << "f" << i<< ".jpg";
		string aux;
		aux = sstm.str();
		V.push_back(imread(aux));
	}

	return V;
}

Mat promedio_imagenes(vector<Mat> V){
	if(V.empty()){
		cout<<"VECTOR VACIO!!!!"<<endl;
		return Mat::zeros(1,1,1);
	}
	Mat prom = Mat::zeros(V[0].size(),V[0].type());
	
	for (int i=0;i<V[0].rows;i++){
		for (int j=0; j<V[0].cols;j++){	
			float b=0,g=0,r=0;
			for (int v=0;v<V.size();v++){
				
				b += V[v].at<Vec3b>(i,j)[0];
				g += V[v].at<Vec3b>(i,j)[1];
				r += V[v].at<Vec3b>(i,j)[2];
				//				Vec3f color = V[v].at<Vec3f>(i,j);
			}
			prom.at<Vec3b>(i,j)[0] =  b/V.size();
			prom.at<Vec3b>(i,j)[1] =  g/V.size();
			prom.at<Vec3b>(i,j)[2] =  r/V.size();
		}
	}
	return prom;
}

int main(int argc, char** argv) {
	vector<Mat> vec = carga_db();
//	cout<<vec.size()<<endl;
	Mat prome = promedio_imagenes(vec);
	vec.push_back(prome);
	
//	segmentacionBGR_trackbar(prome);
	
	Point3i color = Point3i(132,120,132);
	int radio =9;
	Mat seg = colorDistance(prome,color,radio);
	imshow("seg",seg);
	
//	detect_linesP_trackbar(seg);
	vector<Vec4i> lines; //LO QUE DA EL DETECTLINES OSEA ESCRIBE LAS LINEAS ARRIBA DE LA IMAGEN
	Mat dst = seg.clone();
	Mat con_lineas = seg.clone();
	lines = detect_linesP(seg, dst, 54, 0, 360, 11, 29, 0);
	for(size_t i = 0; i < lines.size(); ++i) {
		Vec4i l = lines[i];
		line(con_lineas, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
	}
	
	imshow("Promedio",con_lineas);
	
	//PARA ROTARLA
	Vec4i l = lines[0];
	double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI - 90;
	
	Mat Salida = prome.clone();
	Salida = rotateMat(Salida, angle);
	imshow("sin nubes", Salida);
	
	
	waitKey(0);
	return 0;
} 
