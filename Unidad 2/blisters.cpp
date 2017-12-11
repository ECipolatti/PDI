#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat binary_lut(int umbral);
void comprobar(Mat img);
float prom_intensidad(Mat roi);
int main(int argc, char** argv) {
	
	Mat img = imread("blister_completo.jpg",CV_8U);
	Mat img_in = imread("blister_incompleto.jpg",CV_8U);
		

	
	//display the image:
	imshow("blister", img_in);
//	imshow("blister binarizado", img);
	cout<<img.cols<<"  "<<img.rows<<endl;
	comprobar(img_in);
	
	waitKey(0);
	return 0;
} 

Mat binary_lut(int umbral){
	Mat lut(1, 256, CV_8UC1);
	int s;
	for (int r = 0; r < 256; ++r){
		if (r > umbral)
			s = 255;
		else
			s = 0;
		lut.at<uchar>(r) = s;
	}
	return lut;
}
float prom_intensidad(Mat roi){
	float prom=0;
	int tam=roi.cols*roi.rows;
	for(int i = 0; i<roi.rows; i++){
		for(int j = 0; j< roi.cols; j++){
			prom = prom + roi.at<uchar>(i,j);
		}
	}
	
	prom =prom/tam;
	return prom;
}
void comprobar(Mat img){
	Mat lut = binary_lut(100);
	LUT(img, lut, img);
	bool completo=true;
	for (int i=0; i<5;i++){
		for (int j=0;j<2;j++){
			Mat roi = img(Rect(46+(50*i),45+(50*j),10,10));
			float promedio= prom_intensidad(roi);
			if (promedio<100){
				cout<<"Al blister le falta la pastilla en la posicion"<<j+1<<" "<<i+1<<endl;
				completo=false;
			}
		}
	}
	if (completo)
		cout<<"El blister esta completo";
};

