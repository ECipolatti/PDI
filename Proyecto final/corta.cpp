#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// Example main copied from https://gist.github.com/EyalAr/3940636
int main(int argc, char** argv) {
	//create a gui window:
	int tam =500;
	for (int i=0; i<10;i++){
		stringstream sstm;
		sstm << "000" << i<< "_rgb.tiff";
		string aux;
		aux = sstm.str();
		Mat im = imread(aux);

		Mat dst;
		im(Rect(0,0,tam,tam)).copyTo(dst);
		imwrite(aux,dst);
	}
	for (int i=10; i<58;i++){
		stringstream sstm;
		sstm << "00" << i<< "_rgb.tiff";
		string aux;
		aux = sstm.str();
		Mat im=imread(aux);
		Mat dst;
		im(Rect(0,0,tam,tam)).copyTo(dst);
		imwrite(aux,dst);
	}
		return 0;
} 

/// warning: This template will *dynamicaly* link against opencv, so yo'll need to copy some 
///          dll files from zinjai\mingw-gcc5\opencv3\bin in order to run this example

/// aviso: esta plantilla está configurada para enlazar *dinámicamente* con opencv, por lo
///        que necesitará copiar algunos archivos dll desde zinjai\mingw-gcc5\opencv3\bin
///        para ejecutar correctamente este ejemplo

