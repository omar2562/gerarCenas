#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cv.h>
#include "Tomada.h"
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;

vector<Tomada> getFilmInTomadas(string filmPath,string filmName);
void findQuadroClaveForVector(vector<Tomada>& tomadaVector);
void findQuadroClave(vector<Tomada>::iterator& tomada);

int main( int argc, char** argv )
{
    string filmPath = "c:/Users/Omar/Documents/FMI Resources/";
    string filmName = "hachiko";
    vector<Tomada> tomadaVector;
    tomadaVector = getFilmInTomadas(filmPath,filmName);
    findQuadroClaveForVector(tomadaVector);
    /* -- Imprimir vecto
        cout << "myvector contains:";
        vector<Tomada>::iterator it;
        for (it=tomadaVector.begin(); it<tomadaVector.end(); it++)
            cout << ' ' << *it<< '\n';

        cout << "Numero de tomadas: "<<tomadaVector.size()<<"\n";
    */
    Mat image;
    image = imread("e:/Mestrado/FMI/code/lena_std.tif", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.
    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}

vector<Tomada> getFilmInTomadas(string filmPath,string filmName)
{
    vector<Tomada> tomadaVector;
    string inFile = filmPath+filmName+"/"+filmName+".in";
    ifstream myReadFile;
    myReadFile.open(inFile.c_str());
    string word;
    Tomada* tomadaTmp;
    if (myReadFile.is_open())
    {
        while (!myReadFile.eof())
        {
            Tomada tomada;
            myReadFile >> word;
            tomada.setPathTomada(filmPath+filmName+"/"+filmName+"/"+word);
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
    tomada->addQuadroChave(tomada->getInitQuadro());
    tomada->addQuadroChave((tomada->getInitQuadro()+tomada->getEndQuadro())/2);
    tomada->addQuadroChave(tomada->getEndQuadro());
}
