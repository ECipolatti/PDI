#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>

using namespace cv;
using namespace std;

// Example main copied from https://gist.github.com/EyalAr/3940636
int main(int argc, char** argv) {
	int cont=0;
	for (int i=0; i<10;i++){
		stringstream sstm;
		sstm << "000" << i<< "_rgb.tiff";
		string aux;
		aux = sstm.str();
		Mat im = imread(aux);
		if(im.data != NULL){
			im = imread(aux);
			stringstream ssstm;
			ssstm << "a" << cont<< ".tiff";
			string nombre;
			nombre = ssstm.str();
			imwrite(nombre,im);
			cont++;
		}
	}
	for (int i=10; i<90;i++){
		stringstream sstm;
		sstm << "00" << i<< "_rgb.tiff";
		string aux;
		aux = sstm.str();
		Mat im = imread(aux);
		if(im.data != NULL){
			im = imread(aux);
			stringstream ssstm;
			ssstm << "a" << cont<< ".tiff";
			string nombre;
			nombre = ssstm.str();
			imwrite(nombre,im);
			cont++;
		}
	}
	
		return 0;
} 

/// warning: This template will *dynamicaly* link against opencv, so yo'll need to copy some 
///          dll files from zinjai\mingw-gcc5\opencv3\bin in order to run this example

/// aviso: esta plantilla está configurada para enlazar *dinámicamente* con opencv, por lo
///        que necesitará copiar algunos archivos dll desde zinjai\mingw-gcc5\opencv3\bin
///        para ejecutar correctamente este ejemplo

