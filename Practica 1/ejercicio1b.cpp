#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// Example main copied from https://gist.github.com/EyalAr/3940636
int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output", CV_WINDOW_AUTOSIZE); //by default is in autosize
	namedWindow("Output2",CV_WINDOW_AUTOSIZE);
	
	
	Mat img = imread("lion.png",CV_16S);

	//set the ROI
	Rect roi = Rect(10,10,50,50);
	Mat image_recorte = img(roi);
	

	
	//display the image:
	imshow("Output", img);
	
	imshow("Output2", image_recorte);
	
	waitKey(0);
	return 0;
} 

/// warning: This template will *dynamicaly* link against opencv, so yo'll need to copy some 
///          dll files from zinjai\mingw-gcc5\opencv3\bin in order to run this example

/// aviso: esta plantilla está configurada para enlazar *dinámicamente* con opencv, por lo
///        que necesitará copiar algunos archivos dll desde zinjai\mingw-gcc5\opencv3\bin
///        para ejecutar correctamente este ejemplo

