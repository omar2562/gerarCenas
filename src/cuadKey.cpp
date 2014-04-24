#include "cuadKey.h"




using namespace cv;

cuadKey::cuadKey()
{
}

/* para comparar dos histogramass ( H_{1} and H_{2} ), primero se tiene que elegir la metrica ( en el paper de thiago no la encuentro)
//(d(H_{1}, H_{2}))

//OpenCV implementa la funcion: "compareHist" para realizar las comparacione a la vez ofrece 4 metricas para comparar :
//el codigo está en la pagina principal de opencv

    Correlation ( CV_COMP_CORREL )

    Chi-Square ( CV_COMP_CHISQR )

    Intersection ( method=CV_COMP_INTERSECT )

    Bhattacharyya distance ( CV_COMP_BHATTACHARYYA )
*/
cuadKey::~cuadKey()
{
    //dtor
}

//se compara dos imagenes
void cuadKey::compHist(cv::Mat base,cv::Mat test,int hue,int bins_hue,int sat,int bins_sat)
{
    // convierte a HSV
    cv::Mat hsv_base;
    cv::Mat hsv_test;
    cvtColor( base, hsv_base, CV_BGR2HSV );
    cvtColor( test, hsv_test, CV_BGR2HSV );


    //tamanio de histograma
    int histSize[] = { bins_hue, bins_sat };

    //hue varia de 0 to hue, satuarcion varia de  0 to sat
    float h_rang[] = { 0, hue };
    float s_rang[] = { 0, sat };

    const float* ranges[] = { h_rang, s_rang };
    //canales
     int channels[] = { 0, 1 };

     // Histogramas
     MatND hist_base;
     MatND hist_test;
     // calcula histogramas
       calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
       //normalizacion del histograma de [0 - 1]
       normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

       calcHist( &hsv_test, 1, channels, Mat(), hist_test, 2, histSize, ranges, true, false );
       //normalizacion del histograma de [0 - 1]
       normalize( hist_test, hist_test, 0, 1, NORM_MINMAX, -1, Mat() );

       /// aplicando metodos de comparacion
       for( int i = 0; i < 4; i++ )
          {
              int compare_method = i;

            double base_base = compareHist( hist_base, hist_base, compare_method );

            double base_test = compareHist( hist_base, hist_test, compare_method );


            printf( " metodo [%d] Perfecto, Base-Test(1): %f, %f \n", i, base_base, base_test);
          }

}
