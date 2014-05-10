#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cv.h>
#include <stdlib.h>

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

#define NUM_TOMADA_CHECK 5
#define DIF_MAX 1.0
#define MAX_VALUE 3000
#define PERCENTAGE_CHANGE 0.85
//valores segun paper de thiago , falta experimentar
#define hue 180
#define sat 256
#define bins_hue 8
#define bins_sat 4
#define alpha 0.90

template<typename T>
ostream& operator<< (ostream& out, const vector<T> v) {
    int last = v.size() - 1;
    out << "[";
    for(int i = 0; i < last; i++)
        out << v[i] << ", ";
    out << v[last] << "]";
    return out;
}




double scComarations[MAX_VALUE][MAX_VALUE];
double scComarationsFull[MAX_VALUE][MAX_VALUE][3];

vector<Tomada> getFilmInTomadas(string filmPath);
void findQuadroClaveForVector(vector<Tomada>& tomadaVector,vector<vector<int> > keyframes);
void findQuadroClave(vector<Tomada>::iterator& tomada, vector<int> keys);
vector<Cena> generateCenas(vector<Tomada>& tomadaVector);
vector<Mat> getVectorForQuadro(string quadroPath);
double compareQuadroClave(Tomada tomada1,Tomada tomada2,int i,int j);
void calculateCoherenceTomadas(vector<Tomada>& tomadaVector);
void removeCenasMinusculas(vector<Cena>& cenaVector);
void joinCenas(vector<Cena>& cenaVector,int i,bool fusionDer);

int main( int argc, char** argv )
{

    readFil varfile;
    vector<string> mainDir;
    string direcname="c:/Users/Omar/Documents/FMI Resources/hachiko";
    string direcnamef="c:/Users/Omar/Documents/FMI Resources/hachiko_carac";
    string direcnames="c:/Users/Omar/Documents/FMI Resources/hachiko_singularvalue";
    //vector de vector  de directorios
    vector<vector<string> > dirmatrix(3000000,vector<string>());
    //lectura de directorio inicial
    varfile.readAllFil(direcname);
    mainDir=varfile.getNameTomada();
    //leer imagenes del directorio
    for(int y=0; y<mainDir.size(); y++)
    {
        varfile.readAllFil(direcname+"/"+mainDir[y]);
        dirmatrix[y]=varfile.getNameTomada();
    }
    vector<vector<int> > keyFramesvector;
    //cout<<dirmatrix[0]<<endl;

cuadKey datapr;
vector <int> numKeyframes;
//cout<<"tamañno de matriz"<<dirmatrix.size()<<endl;
for(int oo=0;oo<1167;oo++){
    vector<int> vect;
    Mat a(dirmatrix[oo].size(),datapr.sizefeatures,CV_32FC1,Scalar::all(0));
    string pathh=direcnamef+"/"+mainDir[oo]+".txt";
    string pathh2=direcnames+"/"+mainDir[oo]+".txt";

    datapr.getRankfromFile(dirmatrix[oo],pathh,a);

    Mat A, w, u, vt;
    SVD::compute(a, w, u, vt);
    std::ofstream file_out(pathh2.c_str(),std::ofstream::out);
    //cout << w << "\n";
    //printf("matrix W columna =%d , fila=%d\n",w.cols,w.rows);
    //printf("matrix U columna =%d , fila=%d\n",u.cols,u.rows);
    //printf("matrix VT columna =%d , fila=%d\n",vt.cols,vt.rows);
    float val=0.0;
    float val2=0.0;
    int countKeyFrame=0;
    int h=std::min(a.cols,a.rows);
    int k=h;
    for(int ii=0;ii<h;ii++)
    {
        val+=pow(w.at<float>(ii,1),2);
    }
   // cout<<"---------tomada "<<oo<<"--numero de frames --"<<a.rows<<endl;
    for(int uu=0;uu<k;uu++){
    for(int ii=0;ii<uu;ii++)
    {
        val2+=pow(w.at<float>(ii,1),2);
    }
            float vall=sqrt(val2)/sqrt(val);
            file_out<<uu<<" ";
            //printf("valor de %d cuadros claves = %f \n",uu,vall);
            file_out<<vall<<endl;

            if(vall>alpha && vall<1.0){countKeyFrame++;}


            val2=0;
    }

   // printf("valor de %d cuadros claves en tomada \n",countKeyFrame);
   numKeyframes.push_back(countKeyFrame);

   //cout<<numKeyframes.size()<<endl;

    //cout<<"key-frames"<<numKeyframes[oo]<<endl;
    string pp=direcname+"/"+mainDir[oo];
    //datapr.getKeyFrames(dirmatrix[oo],numKeyframes[oo],pp,vect);
    datapr.getKeyFrames2(a,numKeyframes[oo],vect);
    //cout<<"salliio"<<endl;

    val=0;

    file_out.close();
    //cout<<vect<<endl;
    keyFramesvector.push_back(vect);
    cout<<"tomada : "<<oo<<endl;
    cout<<keyFramesvector[oo]<<endl;
}

//cout<<"numero de tomadas : "<<keyFramesvector.size()<<endl;









/*********************************************************************************
*/



    bool showImages = true;
    string filmPath = "c:/Users/Omar/Documents/FMI Resources/hachiko";

    vector<Tomada> tomadaVector;
    tomadaVector = getFilmInTomadas(filmPath);

    cout<<"pasee 1"<<endl;
    findQuadroClaveForVector(tomadaVector,keyFramesvector);
    cout<<"pasee 2"<<endl;
    calculateCoherenceTomadas(tomadaVector);
    cout<<"pasee 3"<<endl;
    vector<Tomada>::iterator it2;
    /*for (it2=tomadaVector.begin(); it2<tomadaVector.end(); it2++)
        cout << ' ' << *it2<< '\n';*/
    vector<Cena> cenaVector;
    cenaVector=generateCenas(tomadaVector);
    cout<<"pasee 4"<<endl;
    cout << cenaVector.size() << endl;
    removeCenasMinusculas(cenaVector);
    cout << cenaVector.size() << endl;
    cout<<"pasee 5"<<endl;
    // -- Imprimir vecto
    //cout << "myvector contains:\n";
    vector<Cena>::iterator it;
    /*for (it=cenaVector.begin(); it<cenaVector.end(); it++)
        cout << ' ' << *it<< '\n';*/
    //showCenas(cenaVector);
    ofstream myfile;
    myfile.open ("c:/Users/Omar/Documents/FMI Resources/hachiko_p.xml");
    Mat image;
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    for(int k =0; k<cenaVector.size(); k++)
    {
        Cena cena = cenaVector[k];
        myfile << "<Scene id=\"" << k << "\">\n";

        string maxS,minS;
        maxS = minS = cena.getTomadaVector().front().getTomadaName();
        for(int i =0; i<cena.getTomadaVector().size(); i++)
        {
            Tomada t = cena.getTomadaVector()[i];
            maxS = maxS.compare(t.getTomadaName())>0?maxS:t.getTomadaName();
            minS = minS.compare(t.getTomadaName())<0?minS:t.getTomadaName();
            for(int j =0; j<t.getQuadroClaveVector().size(); j++)
            {
                Quadro q = t.getQuadroClaveVector()[j];
                //cout << t.getTomadaName()+"/"q.getQuadroPath();
                if(showImages)
                {
                    image = imread(t.getTomadaName()+"/"+q.getQuadroPath(),CV_LOAD_IMAGE_COLOR);
                    imshow( "Display window", image );
                    waitKey(0);
                }
            }
        }
        String str2 = minS;
        str2 = str2.substr (str2.size()-4,4);
        myfile << "<Shot_Ini id=\"" << atoi(str2.c_str()) << "\"/>\n";
        str2 = maxS;
        str2 = str2.substr (str2.size()-4,4);
        myfile << "<Shot_End id=\"" << atoi(str2.c_str()) << "\"/>\n";
        if(showImages)
        {
            image = cv::Mat::zeros(250,250,CV_8UC3);
            putText(image, "Cena "+k,cv::Point(50,50), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255),1,8,false);
            imshow( "Display window", image );
            waitKey(0);
        }
        myfile << "</Scene>\n";
    }
    myfile.close();
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

void findQuadroClaveForVector(vector<Tomada>& tomadaVector,vector<vector<int> > keyframes)
{
    vector<Tomada>::iterator it;
    int i=0;
    for (it=tomadaVector.begin(); it<tomadaVector.end(); it++)
    {
        findQuadroClave(it,keyframes[i]);
        i++;
    }
}
void findQuadroClave(vector<Tomada>::iterator& tomada,vector<int> keys)
{
    vector<Mat> histQuadro;
    for(int ii=0;ii<keys.size();ii++)
    {
            Quadro quadro = tomada->getQuadroVector()[ii];

            histQuadro = getVectorForQuadro(tomada->getTomadaName()+"/"+quadro.getQuadroPath());
            quadro.setPlanoVector(histQuadro);
            tomada->addQuadroChaveVector(quadro);

    }
/*
    Quadro quadro = tomada->getQuadroVector().front();

    histQuadro = getVectorForQuadro(tomada->getTomadaName()+"/"+quadro.getQuadroPath());
    quadro.setPlanoVector(histQuadro);
    tomada->addQuadroChaveVector(quadro);

    quadro = tomada->getQuadroVector().back();
    histQuadro = getVectorForQuadro(tomada->getTomadaName()+"/"+quadro.getQuadroPath());
    quadro.setPlanoVector(histQuadro);
    tomada->addQuadroChaveVector(quadro);*/
}

void calculateCoherenceTomadas(vector<Tomada>& tomadaVector)
{
    for(int i = 0 ; i< MAX_VALUE; i++)
    {
        for(int j = 0 ; j< MAX_VALUE; j++)
        {
            scComarations[i][j] = -1;
            scComarationsFull[i][j][0] = -1;
            scComarationsFull[i][j][1] = -1;
            scComarationsFull[i][j][2] = -1;
        }
    }
    int max_k = NUM_TOMADA_CHECK,i=0;
    double maxValue,tm;
    vector<Tomada>::iterator it;
    for (it=tomadaVector.begin(); it<tomadaVector.end(); it++)
    {
        maxValue = INFINITY;
        int k = i-max_k<0?0:i-max_k;
        k = i==0?1:k;
        for( ; k<=i/*+max_k*/ && k < tomadaVector.size(); k++)
        {
            //cout << i << "," <<k<<"," <<tomadaVector.size()<<"\n";
            if(scComarations[i][k] == -1)
            {
                scComarations[i][k]=compareQuadroClave(tomadaVector[i],tomadaVector[k],i,k);
                scComarations[k][i]=scComarations[i][k];
            }
            tm = 1+(0.05*(max_k-abs(i-k)));
            maxValue = min(maxValue,scComarations[i][k]*tm);
            if(k==i-1) k++;
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
double compareQuadroClave(Tomada tomada1,Tomada tomada2,int a,int b) //BSC tecnic
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
    double minCompareQuadros = INFINITY;
    maxCompareQuadros = -1*INFINITY;
    double medCompareQuadros = 0;
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
        minCompareQuadros = min(minCompareQuadros,compareQuadros);
        maxCompareQuadros = max(maxCompareQuadros,compareQuadros);
        medCompareQuadros += compareQuadros;
    }
    scComarationsFull[a][b][0] = minCompareQuadros;
    scComarationsFull[a][b][1] = maxCompareQuadros;
    scComarationsFull[a][b][2] = medCompareQuadros/(tomada1.getQuadroClaveVector().size() * tomada2.getQuadroClaveVector().size());
    scComarationsFull[b][a][0] = scComarationsFull[a][b][0];
    scComarationsFull[b][a][1] = scComarationsFull[a][b][1];
    scComarationsFull[b][a][2] = scComarationsFull[a][b][2];
    return minCompareQuadros;
}

void removeCenasMinusculas(vector<Cena>& cenaVector)
{
    vector<Cena>::iterator it;
    int i = 0;
    for (it=cenaVector.begin(); it<cenaVector.end(); it++)
    {
        Cena cena = *it;
        if(cena.getTomadaVector().size() == 1)
        {
            int k = i-1;
            double maxCa=-1*INFINITY,minCa=INFINITY,medCa=0;
            double maxCp=-1*INFINITY,minCp=INFINITY,medCp=0;
            int cnt = 0;
            bool fusionDer = true;
            Tomada tomada1,tomada2;
            if(k>=0)
            {
                if(scComarations[i][k] == -1)
                {
                    scComarations[i][k]=compareQuadroClave(cena.getTomadaVector()[i],cena.getTomadaVector()[k],i,k);
                    scComarations[k][i]=scComarations[i][k];
                }
                minCa = scComarationsFull[i][k][0];
                maxCa = scComarationsFull[i][k][1];
                medCa = scComarationsFull[i][k][2];
            }
            k = i+1;
            if(k<cenaVector.size())
            {
                if(scComarations[i][k] == -1)
                {
                    scComarations[i][k]=compareQuadroClave(cena.getTomadaVector()[i],cena.getTomadaVector()[k],i,k);
                    scComarations[k][i]=scComarations[i][k];
                }
                minCp = scComarationsFull[i][k][0];
                maxCp = scComarationsFull[i][k][1];
                medCp = scComarationsFull[i][k][2];
            }
            //reglas
            if(maxCa>=maxCp && medCa>=medCp)
                fusionDer = false;
            if(fusionDer || maxCp>=maxCa*1.2)
                fusionDer = false;
            if(fusionDer || minCa>=minCp*1.1)
                fusionDer = false;
            if(minCa == INFINITY) fusionDer = true;
            if(minCp == INFINITY) fusionDer = false;
            if(fusionDer && cenaVector[i].getTomadaVector().size() == 1) {
                    cout << "=================>" << i<<"\n";
                    it++;
                    i++;

            }
            joinCenas(cenaVector,i,fusionDer);

        }
        i++;
    }
}
void joinCenas(vector<Cena>& cenaVector,int i,bool fusionDer)
{
    int j = fusionDer?i+1:i-1;
    Cena cena = cenaVector[i];
    Tomada tomada;
    tomada = cena.getTomadaVector().at(0);
    Cena cenaD = cenaVector[j];
    cout << cenaD.getTomadaVector().size() << " ";
    cenaVector[j].addTomada(tomada);
    cout << cenaD.getTomadaVector().size() << " " << i << " " << j <<endl;
    cenaVector.erase(cenaVector.begin()+i);
}
