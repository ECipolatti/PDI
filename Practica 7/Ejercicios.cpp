#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat img,imgSplit[3],imgEtiq,rosas;
RNG rng(12345);

bool verif_vecino (int x, int y){ //Notar que y corresponde a la fila y x a la col->  at(y,x)== at(row,col)
	bool condicion_bordes = x >= 0 && x <= rosas.cols && y >= 0 && y <= rosas.rows; //No se pasa de los bordes
	bool condicion_region = false;
	bool condicion_etiq = false;
	if( condicion_bordes){
		condicion_region = rosas.at<unsigned char>(y,x) !=0; //No es del fondo blanco
		condicion_etiq = imgEtiq.at<unsigned char>(y,x) == 0; //No fue ya etiquetado
	}
	
	return (condicion_bordes && condicion_region && condicion_etiq);
}

void verificarYagregar_8vecinos (Point p, vector<Point> &v, int etiqueta){
	
	if(verif_vecino(p.x + 1, p.y +1)){
		v.push_back(Point(p.x +1, p.y+1) );
		imgEtiq.at<unsigned char>(p.y +1, p.x +1) = etiqueta; //Etiqueto pixel de la region
	}
	if(verif_vecino(p.x , p.y +1)){
		v.push_back(Point(p.x , p.y+1) );
		imgEtiq.at<unsigned char>(p.y +1, p.x ) = etiqueta;
	}
	if(verif_vecino(p.x - 1, p.y +1)){
		v.push_back(Point(p.x - 1, p.y+1) );
		imgEtiq.at<unsigned char>(p.y +1, p.x -1) = etiqueta;
	}
	if(verif_vecino(p.x - 1, p.y )){
		v.push_back(Point(p.x - 1, p.y ) );
		imgEtiq.at<unsigned char>(p.y , p.x -1) = etiqueta;
	}
	if(verif_vecino(p.x - 1, p.y - 1)){
		v.push_back(Point(p.x - 1, p.y - 1) );
		imgEtiq.at<unsigned char>(p.y -1, p.x -1) = etiqueta;
	}
	if(verif_vecino(p.x , p.y -1)){
		v.push_back(Point(p.x , p.y-1) );
		imgEtiq.at<unsigned char>(p.y -1, p.x ) = etiqueta;
	}
	if(verif_vecino(p.x + 1, p.y -1)){
		v.push_back(Point(p.x +1, p.y-1) );
		imgEtiq.at<unsigned char>(p.y -1, p.x +1) = etiqueta;
	}
	if(verif_vecino(p.x + 1, p.y )){
		v.push_back(Point(p.x +1, p.y ) );
		imgEtiq.at<unsigned char>(p.y, p.x +1) = etiqueta;
	}
	
	
}


void segmentar_region(int row, int col, int etiqueta){ //Recibo img en gris y imgEtiq donde tengo que poner 1s en la region a segmentar
	vector<Point> vecPoints; //Vector de puntos de la region a segmentar
	vecPoints.push_back(Point(col,row)); //Agrego punto recibido
	while (vecPoints.size()>0){ //Itero mientras haya puntos por segmentar
		//Tengo que agregar 8 vecinos (NO HAY SOLAPAMIENTO DE REGIONES)
		Point punto = vecPoints[0];
		verificarYagregar_8vecinos(punto, vecPoints, etiqueta);
		imgEtiq.at<unsigned char>(punto.y, punto.x) = etiqueta; //Etiqueto pixel de la region
		vecPoints.erase(vecPoints.begin()); //Elimino punto ya etiquetado
	} 
}

int main(int argc, char** argv) {
	img = imread("rosas.jpg",CV_32F);
	Mat imgo=img.clone();
	Mat gris(img.size(),img.type());
	cvtColor(img,gris,COLOR_BGR2GRAY);  //Transforma a gris
	threshold(gris,gris,200,255,THRESH_BINARY); //A valores menores a 200 los pone en 0
	cvtColor(gris,gris,COLOR_GRAY2BGR); //Transforma a BGR
	img = img - gris; //Elimina el fondo
	
	
	split(img,imgSplit);
	Mat imgR;
	threshold(imgSplit[2],imgR,100,255,THRESH_BINARY); // Se queda con el ROJO
	Mat imgG;
	threshold(imgSplit[1],imgG,80,255,THRESH_BINARY); // 
	
	
	rosas = imgR - imgG;
	
	Mat ee = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(rosas,rosas,MORPH_OPEN,ee,Point(-1,-1),2);
	
	imgEtiq = Mat::zeros(rosas.size(), rosas.type());
	
	//SEGMENTAR ROSAS
	int label = 1;
	for (int i = 0; i < rosas.rows; i++){	
		for (int j = 0; j < rosas.cols; j++){
			if (rosas.at<unsigned char>(i,j)!= 0 && imgEtiq.at<unsigned char>(i,j)==0){ //Region de rosa, la etiqueto
				//1) Si los vecinos pertenecen al fondo, agrego etiqueta nueva
				//2) Si solo un vecino tiene etiqueta!= 0 , le pongo esa
				//3) Si mas de un vecino tiene etiqueta, elijo cualquier etiqueta y luego agrego equivalencia en tabla
				segmentar_region(i,j,label);
				label++;
			}	
		}		
	}
	label--;
	cout<<"Cantidad de rosas: "<<label<<endl;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	Mat eee= getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(rosas,rosas,MORPH_CLOSE,eee,Point(-1,-1),3);
	
	Mat rosas2 = rosas.clone();
	/// Find contours
	findContours( rosas2, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	/// Get the moments
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{ mu[i] = moments( contours[i], false ); }
	
	///  Get the mass centers:
	vector<Point2f> mc( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
	
	/// Draw contours
	Mat drawing = Mat::zeros( rosas.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		circle( drawing, mc[i], 4, color, -1, 8, 0 );
	}
	
	/// Show in a window
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );
	
	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	printf("\t Info: Area and Contour Length \n");
	for( int i = 0; i< contours.size(); i++ )
	{
		printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		circle( imgo, mc[i], 20, color, 3, 8, 0 );
	}
	
	
	
	namedWindow("img",WINDOW_AUTOSIZE);
	imshow("img",imgo);
	namedWindow("imgR",WINDOW_AUTOSIZE);
	imshow("imgR",imgR);
	namedWindow("imgG",WINDOW_AUTOSIZE);
	imshow("imgG",imgG);
	namedWindow("rosas",WINDOW_AUTOSIZE);
	imshow("rosas",rosas);
	
	waitKey(0);
	return 0;
} 


