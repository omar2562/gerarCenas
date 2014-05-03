#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cv.h>

#include "Quadro.h"
#include "Tomada.h"
#include "Cena.h"
#include "readFil.h"
#include "cuadKey.h"
#include <fstream>
#include <math.h>
#include <dirent.h>

using namespace cv;
using namespace std;

#define NUM_TOMADA_CHECK 3
#define DIF_MAX 1.0
#define MAX_VALUE 10000
#define PERCENTAGE_CHANGE 0.85
//valores segun paper de thiago , falta experimentar
#define hue 180
#define sat 256
#define bins_hue 8
#define bins_sat 4

double scComarations[MAX_VALUE][MAX_VALUE];

vector<Tomada> getFilmInTomadas(string filmPath);
void findQuadroClaveForVector(vector<Tomada>& tomadaVector);
void findQuadroClave(vector<Tomada>::iterator& tomada);
vector<Cena> generateCenas(vector<Tomada>& tomadaVector);
vector<Mat> getVectorForQuadro(string quadroPath);
double compareQuadroClave(Tomada tomada1,Tomada tomada2);
void calculateCoherenceTomadas(vector<Tomada>& tomadaVector);

int main( int argc, char** argv )
{

    string filmPath = "/media/New Volume/hachiko";
    vector<Tomada> tomadaVector;
    tomadaVector = getFilmInTomadas(filmPath);
    cout<<"pasee 1"<<endl;
    findQuadroClaveForVector(tomadaVector);
    cout<<"pasee 2"<<endl;
    calculateCoherenceTomadas(tomadaVector);
    cout<<"pasee 3"<<endl;
    vector<Tomada>::iterator it2;
    for (it2=tomadaVector.begin(); it2<tomadaVector.end(); it2++)
        cout << ' ' << *it2<< '\n';
    vector<Cena> cenaVector;
    cenaVector=generateCenas(tomadaVector);
    cout<<"pasee 4"<<endl;
    // -- Imprimir vecto
    //cout << "myvector contains:\n";
    vector<Cena>::iterator it;
    for (it=cenaVector.begin(); it<cenaVector.end(); it++)
        cout << ' ' << *it<< '\n';

    return 0;
}

vector<Tomada> getFilmInTomadas(string filmPath)
{
    vector<Tomada> tomadaVector;

    readFil varfile;

    vector<string> mainDir;

    //lectura de directorio inicial
    varfile.readAllFil(filmPath);

    mainDir=varfile.getNameTomada();

    //leer imagenes del directorio
    for(int y=0; y<mainDir.size(); y++)
    {
        Tomada tomada;
        tomada.setTomadaName(filmPath+"/"+mainDir[y]);
        varfile.readAllFil(filmPath+"/"+mainDir[y]);
        vector<Quadro> quadroVector;
        vector<String> quadroNameVector = varfile.getNameTomada();
        for (int i=0; i<quadroNameVector.size(); i++)
        {
            Quadro quadro;
            quadro.setQuadroPath(quadroNameVector[i]);
            quadroVector.push_back(quadro);
        }
        tomada.setQuadroVector(quadroVector);
        tomadaVector.push_back(tomada);
    }

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
    Quadro lastQuadro = tomada->getQuadroVector().back();
    histQuadro = getVectorForQuadro(tomada->getTomadaName()+"/"+lastQuadro.getQuadroPath());
    lastQuadro.setPlanoVector(histQuadro);
    tomada->addQuadroChaveVector(lastQuadro);
}

void calculateCoherenceTomadas(vector<Tomada>& tomadaVector)
{
    for(int i = 0 ; i< MAX_VALUE; i++)
    {
        for(int j = 0 ; j< MAX_VALUE; j++)
        {
            scComarations[i][j] = -1;
        }
    }
    int max_k = NUM_TOMADA_CHECK,i=0;
    double maxValue;
    vector<Tomada>::iterator it;
    for (it=tomadaVector.begin(); it<tomadaVector.end(); it++)
    {
        maxValue = INFINITY;
        for(int k = 1 ; k<=max_k && i+k < tomadaVector.size(); k++)
        {
            if(scComarations[i][k] == -1)
            {
                scComarations[i][k]=compareQuadroClave(tomadaVector[i],tomadaVector[i+k]);
                scComarations[k][i]=scComarations[i][k];
            }
            maxValue = min(maxValue,scComarations[i][k]);
        }
        it->setCoherence(maxValue);
        i++;
    }
}

vector<Cena> generateCenas(vector<Tomada>& tomadaVector)
{
    vector<Cena> cenaVector;
    vector<Tomada>::iterator it;
    it=tomadaVector.begin();
    Tomada tomada;
    tomada = *it;
    double compareQuadrosCena;
    compareQuadrosCena=0;
    Cena *cena2;
    Cena cenaIni;
    cena2 = &cenaIni;
    (*cena2).addTomada(*it);
    for (it++; it<tomadaVector.end(); it++)
    {
        double percentage = (*it).getCoherence()/tomada.getCoherence();
        if( percentage<= PERCENTAGE_CHANGE || percentage >= 1+(1-PERCENTAGE_CHANGE))
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
double compareQuadroClave(Tomada tomada1,Tomada tomada2) //BSC tecnic
{
    double compareQuadros,maxCompareQuadros,compareQuadros_0;
    int method = CV_COMP_CORREL;
//    int method = CV_COMP_CHISQR ;
//    int method = CV_COMP_INTERSECT ;
//    int method = CV_COMP_BHATTACHARYYA ;
//    int method = CV_COMP_HELLINGER  ;
    /*r_compareQuadros = compareHist( tomada.getQuadroClaveVector()[0], (*it).getQuadroClaveVector()[0], method );
    g_compareQuadros = compareHist( tomada.getQuadroClaveVector()[1], (*it).getQuadroClaveVector()[1], method );
    b_compareQuadros = compareHist( tomada.getQuadroClaveVector()[2], (*it).getQuadroClaveVector()[2], method );
    compareQuadros = sqrt(pow(r_compareQuadros,2.0)+pow(g_compareQuadros,2.0)+pow(b_compareQuadros,2.0));*/
    maxCompareQuadros = INFINITY;
    for(int i = 0; i<tomada1.getQuadroClaveVector().size(); i++)
    {
        for(int j = 0; j<tomada2.getQuadroClaveVector().size(); j++)
        {
            for(int k = 0; k<tomada1.getQuadroClaveVector()[i].getPlanoVector().size(); k++)
            {
                compareQuadros_0 = compareHist( tomada1.getQuadroClaveVector()[i].getPlanoVector()[k], tomada2.getQuadroClaveVector()[j].getPlanoVector()[k], method );
                compareQuadros += pow(compareQuadros_0,2.0);
            }
            compareQuadros = sqrt(compareQuadros);
        }
        maxCompareQuadros = min(maxCompareQuadros,compareQuadros);
    }
    return maxCompareQuadros;
}
