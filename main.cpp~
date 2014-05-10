#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cv.h>
#include "Tomada.h"
#include "Cena.h"
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;

#define NUM_TOMADA_CHECK 3
#define DIF_MAX 1.0

vector<Tomada> getFilmInTomadas(string filmPath,string filmName);
void findQuadroClaveForVector(vector<Tomada>& tomadaVector);
void findQuadroClave(vector<Tomada>::iterator& tomada);
vector<Cena> generateCenas(vector<Tomada>& tomadaVector);
vector<Mat> getVectorForQuadro(string quadroPath);

int main( int argc, char** argv )
{
    string filmPath = "c:/Users/Omar/Documents/FMI Resources/";
    string filmName = "hachiko";
    vector<Tomada> tomadaVector;
    tomadaVector = getFilmInTomadas(filmPath,filmName);
    findQuadroClaveForVector(tomadaVector);
    vector<Cena> cenaVector;
    cenaVector=generateCenas(tomadaVector);
    ///* -- Imprimir vecto
    //cout << "myvector contains:\n";
    vector<Cena>::iterator it;
    for (it=cenaVector.begin(); it<cenaVector.end(); it++)
        cout << ' ' << *it<< '\n';

    //cout << "Numero de cenas: "<<cenaVector.size()<<"\n";
    //*/
    Mat image;
    image = imread("e:/Mestrado/FMI/code/lena_std.tif", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.
    //waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}

vector<Tomada> getFilmInTomadas(string filmPath,string filmName)
{
    vector<Tomada> tomadaVector;
    string inFile = filmPath+filmName+"/"+filmName+".in";
    ifstream myReadFile;
    myReadFile.open(inFile.c_str());
    string word;
    if (myReadFile.is_open())
    {
        while (!myReadFile.eof())
        {
            Tomada tomada;
            myReadFile >> word;
            tomada.setPathTomada(filmPath+filmName+"/"+word);
            tomada.setNumTomada(atoi(word.substr(7).c_str()));
            myReadFile >> word;
            tomada.setInitQuadro(atoi(word.c_str()));
            myReadFile >> word;
            tomada.setEndQuadro(atoi(word.c_str()));
            //tomada.addQuadroChave(6);
            tomadaVector.push_back(tomada);
        }
    }
    myReadFile.close();

    return tomadaVector;
}

void findQuadroClaveForVector(vector<Tomada>& tomadaVector)
{
    vector<Tomada>::iterator it;
    for (it=tomadaVector.begin(); it<tomadaVector.end(); it++)
    {
        findQuadroClave(it);
    }
}
void findQuadroClave(vector<Tomada>::iterator& tomada)
{
    vector<Mat> histQuadro;
    stringstream ss;
    ss.str("");
    ss << setw(6) << setfill('0') << tomada->getEndQuadro();
    histQuadro = getVectorForQuadro(tomada->getPathTomada() + "/" + ss.str() + ".jpg");
    tomada->setQuadroClaveVector(histQuadro);
}

vector<Cena> generateCenas(vector<Tomada>& tomadaVector)
{
    vector<Cena> cenaVector;
    vector<Tomada>::iterator it;
    it=tomadaVector.begin();
    Tomada tomada;
    tomada = *it;
    double r_compareQuadros,g_compareQuadros,b_compareQuadros,compareQuadros,compareQuadrosCena;
    compareQuadrosCena=0;
    Cena *cena2;
    Cena cenaIni;
    cena2 = &cenaIni;
    (*cena2).addTomada(*it);
    int method = CV_COMP_CORREL;
//    int method = CV_COMP_CHISQR ;
//    int method = CV_COMP_INTERSECT ;
//    int method = CV_COMP_BHATTACHARYYA ;
//    int method = CV_COMP_HELLINGER  ;
    for (it++; it<tomadaVector.end(); it++)
    {

        r_compareQuadros = compareHist( tomada.getQuadroClaveVector()[0], (*it).getQuadroClaveVector()[0], method );
        g_compareQuadros = compareHist( tomada.getQuadroClaveVector()[1], (*it).getQuadroClaveVector()[1], method );
        b_compareQuadros = compareHist( tomada.getQuadroClaveVector()[2], (*it).getQuadroClaveVector()[2], method );
        compareQuadros = sqrt(pow(r_compareQuadros,2.0)+pow(g_compareQuadros,2.0)+pow(b_compareQuadros,2.0));
        cout << tomada.getNumTomada() << " " << (*it).getNumTomada() << " " << compareQuadros << "\n";
        //if(abs(compareQuadros-compareQuadrosCena) == compareQuadros) compareQuadrosCena = compareQuadros;
        if(abs(compareQuadros-compareQuadrosCena) < DIF_MAX)
        {
            tomada = *it;
            cenaVector.push_back(*cena2);
            Cena cena;
            cena2 = &cena;
            compareQuadrosCena = 0;
        }
        (*cena2).addTomada(*it);

    }
    return cenaVector;
}

vector<Mat> getVectorForQuadro(string quadroPath)
{
    //cout << quadroPath << "\n";
    Mat src, dst;
    /// Load image
    src = imread(quadroPath, CV_LOAD_IMAGE_COLOR);

    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( src, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true;
    bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    normalize( b_hist, b_hist, 0, 1, NORM_MINMAX, -1, Mat() );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    normalize( g_hist, b_hist, 0, 1, NORM_MINMAX, -1, Mat() );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    normalize( r_hist, b_hist, 0, 1, NORM_MINMAX, -1, Mat() );
    vector<Mat> vect;
    vect.push_back(r_hist);
    vect.push_back(g_hist);
    vect.push_back(b_hist);
    return vect;
}
