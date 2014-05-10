#include "cuadKey.h"
#define divH 7
#define divS 2
#define divV 2

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>

#include <sstream>

using namespace cv;
using namespace std;
template<typename T>
ostream& operator<< (ostream& out, const vector<T> v) {
    int last = v.size() - 1;
    out << "[";
    for(int i = 0; i < last; i++)
        out << v[i] << ", ";
    out << v[last] << "]";
    return out;
}
bool myfunction (pair<float,int> i,pair<float,int> j) { return (i.first>j.first); }

cuadKey::cuadKey()
{

    int i=180/divH;
    for(int u=0.0;u<divH;u++){quantH.push_back(((u+1)*i));}
    //quantH.push_back(179);

    int s=256/divS;
    for(int u=0.0;u<divS;u++){quantS.push_back((u+1)*s);}
    //quantS.push_back(255);


    int v=256/divV;
    for(int u=0.0;u<divV;u++){quantV.push_back((u+1)*v);}
    //quantV.push_back(255);


    sizefeatures=(divH-1)*divS*divV + (divV*(divS-1))+divV;






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

    cout<<hsv_base.channels()<<endl;
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
       cout<<hist_test.rows<<endl;
        cout<<hist_test.cols<<endl;
        cout<<hist_test<<endl;

       normalize( hist_test, hist_test, 0, 1, NORM_MINMAX, -1, Mat() );



       /// aplicando metodos de comparacion
       for( int i = 0; i < 4; i++ )
          {
              int compare_method = i;

            double base_base = compareHist( hist_base, hist_base, compare_method );

            double base_test = compareHist( hist_base, hist_test, compare_method );


            printf( " metodo [%d] #define array[] {'a', 'b', 'c'}Perfecto, Base-Test(1): %f, %f \n", i, base_base, base_test);
          }

}

/*
name= fila  de vector<vector<string>>  == tomada
pathgen= path general;
*/
cv::Mat cuadKey::getRank(vector<string> name,string pathgen,string pathgen2)
{
    cv::Mat M(sizefeatures,name.size(), CV_32F,0.0);

    cv::Mat img,hsvimg;vector<int> ftures;
    string path=pathgen2+".txt";
    std::ofstream file_out(path.c_str(),std::ofstream::out);

    for(int y=0;y<name.size();y++)
    {
        img=imread(pathgen+"/"+name[y]);
        cvtColor( img, hsvimg, CV_BGR2HSV);

         //imshow( "Display window", hsvimg );                   // Show our image inside it.
        //waitKey(0);
       // cout<<name[y]<<" :"<<endl;
        ftures=getFeatures(hsvimg);


        for(std::size_t i = 0; i < ftures.size(); ++i)
        file_out << ftures[i]<<" ";

        file_out<<endl;
        //for(int uu=0;uu<ftures.size();uu++){M.at<double>(uu,y)=ftures[uu];}
        ftures.clear();
    }
    file_out.close();
    return M;
}

void cuadKey::getMatHSV(vector<cv::Mat> imgTom,vector<string> name,string pathgen)
{

        for(int y=0;y<name.size();y++)
        {   cv::Mat hsvimg;
            cv::Mat img;
            img=imread(pathgen+"/"+name[y]);
            cvtColor( img, hsvimg, CV_BGR2HSV);
            imgTom.push_back(img);
        }


}
/*
    obtiene el vector caracteristico apartir de una imagen HSV

*/
vector<int> cuadKey::getFeatures(cv::Mat img)//imgamen en HSV
{
int vectsize=(divH-1)*divS*divV + (divV*(divS-1))+divV;
vector<int> features(vectsize,0);
//cout<<"tamano del vector caracteristco = "<<vectsize<<endl;
std::pair<std::vector<int>::iterator,std::vector<int>::iterator> boundH;
std::pair<std::vector<int>::iterator,std::vector<int>::iterator> boundS;
std::pair<std::vector<int>::iterator,std::vector<int>::iterator> boundV;

for (int y=0;y<img.rows;y++)
for(int x=0;x<img.cols;x++)
{
            Vec3b p=img.at<cv::Vec3b>(y,x);

            //printf("H=%d, S=%d, V=%d \n", p[0], p[1], p[2]);

            boundH=std::equal_range (quantH.begin(), quantH.end(),p[0]);//buscando H
            //printf("H=%d, rango=%d, rango=%d \n", p[0], boundH.first-quantH.begin(), boundH.second-quantH.begin());

            boundS=std::equal_range (quantS.begin(), quantS.end(),p[1]);//buscando S
            //printf("S=%d, rango=%d, rango=%d \n", p[1], boundS.first-quantS.begin(), boundS.second-quantS.begin());

            boundV=std::equal_range (quantV.begin(), quantV.end(),p[2]);//buscando V
            //printf("V=%d, rango=%d, rango=%d \n", p[2], boundV.first-quantV.begin(), boundV.second-quantV.begin());


            //se quantiza los valores para diferentes intervalos
            int ff=(((boundH.first-quantH.begin())%divH)*divS*divV+((boundS.first-quantS.begin())*divV)+(boundV.first-quantV.begin()));
            features[ff]=features[ff]+1;


}






//cout<<features<<endl;
return features;

//mycount = std::count (myvector.begin(), myvector.end(), 20);
//  std::cout << "20 appears " << mycount  << " times.\n";





        //vector de caracteristicas de la forma


}

void cuadKey::getKeyFrames(vector<string> namesFrames,int numkFrames,string path,vector<int>& vectt)
{

            vector<pair<float,int> > diff;
            string imgname=path+"/"+namesFrames[0];
            cv::Mat img=imread(imgname,0);
            //namedWindow( "Display window", WINDOW_AUTOSIZE );
            //imshow("imagen",img);
            //waitKey(0);
            int histSize = 256;    // bin size
            float range[] = { 0, 255 };
            const float *ranges[] = { range };

            // Calculate histogram
            MatND hist;
            calcHist( &img, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );
            normalize( hist, hist, 0, 1, NORM_MINMAX, -1, Mat() );
       // cout<<hist<<endl;
        for(int y=1;y<namesFrames.size();y++){

            string imgname=path+"/"+namesFrames[y];
            cv::Mat gray=imread(imgname,0);

            MatND hist2;
            calcHist( &gray, 1, 0, Mat(), hist2, 1, &histSize, ranges, true, false );

            normalize( hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat() );



            pair<float,int>p;
            float base_base = compareHist( hist, hist2, 3 );
            p.first=base_base;
            p.second=y;
            //cout<<base_base<<endl;
            diff.push_back(p);


        }
//se obtiene los valores distantes del inicial
sort(diff.begin(),diff.end(),myfunction);
vector<pair<float,int> >::iterator it;

//el primer frame va por defecto
    vectt.push_back(1);
    int mm=0;
    int count=1;
    for(it=diff.begin();it!=diff.end();it++)
    {
        //cout<<it->first<<" - "<<it->second<<endl;
        mm=it->second;
    if(count<numkFrames){vectt.push_back(mm);count=count+3;}
        }

}

void  cuadKey::getKeyFrames2(cv::Mat a,int num,vector<int>& vectt)
{
        vector<pair<float,int> >diff;
        vector<float> vect;
        for(int uu=0;uu<a.cols;uu++){vect.push_back(a.at<float>(0,uu));}
        for(int u=1;u<a.rows;u++)
        {
            pair<float,int>p;
            float val=0.0;
            for(int w=0;w<a.cols;w++)
            {

                    val+=pow(vect[w]-a.at<float>(u,w),2);

            }
            p.first=sqrt(val);
            p.second=u;
            diff.push_back(p);

        }
sort(diff.begin(),diff.end(),myfunction);
    vector<pair<float,int> >::iterator it;
    //cout<<diff<<endl;

    vectt.push_back(1);
    int mm=0;
    int count=1;
    for(it=diff.begin();it!=diff.end();it++)
    {
        //cout<<it->first<<" - "<<it->second<<endl;
        mm=it->second;


    if(count<num){vectt.push_back(mm);count=count+3;}
        }
       //cout<<vectt<<endl;

}




void cuadKey::getRankfromFile(vector<string> name,string pathgen,cv::Mat& matrix)
{



//for(int yy=0;yy<matrix.cols;yy++)
//for(int xx=0;xx<matrix.rows;xx++){matrix.at<uchar>(xx,yy)=9;}

        std::ifstream file(pathgen.c_str());
        string line;

        int  x=0,y=0;

        while(getline(file,line))
        {
            istringstream iss(line);
            int n;
            y=0;
            while(iss>>n)
            {
               // cout<<"x"<<x<<" ,"<<y<<", n"<<n<<endl;
            if(x<matrix.rows && y<matrix.cols){
            matrix.at<float>(x,y++)=n;}
            }
          //  for(int yy=0;yy<matrix.cols;yy++){cout<<matrix.at<double>(x,yy)<<" ";}
            x++;
            //cout<<y<<endl;
            //v.clear();

        }



        //cout<<"termino bien linea final"<<endl;
       // for(int yy=0;yy<matrix.cols;yy++){cout<<matrix.at<double>(0,yy)<<" ";}

        //cout<<"algo esta mal"<<endl;
        file.close();



}



