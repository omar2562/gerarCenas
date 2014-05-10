#ifndef CUADKEY_H
#define CUADKEY_H
#include "opencv/cv.h"
#include "readFil.h"




class cuadKey
{
public:

    vector<int> quantH;
    vector<int> quantS;
    vector<int> quantV;
    int sizefeatures;
virtual ~cuadKey();
    cuadKey();
    readFil tempDir;
    //vector<cv::Mat> getcuadKeys();
    void compHist(cv::Mat base,cv::Mat test,int hue,int bins_hue,int sat,int bins_sat);
    //vector<vector<pair(string,cv::Mat)> > getcuadKeys(vector<string> namedir,string mane);
    vector<int> getFeatures(cv::Mat img);
    cv::Mat getRank(vector<string> name,string pathgen,string pathgen2);
    void getMatHSV(vector<cv::Mat> imgTom,vector<string> name,string pathgen);
    void getRankfromFile(vector<string> name,string pathgen,cv::Mat& a);
    void getKeyFrames(vector<string> namesFrames,int numkFrames,string path,vector<int>& a);
    void  getKeyFrames2(cv::Mat tmda,int num,vector<int>& a);
};

#endif // CUADKEY_H
