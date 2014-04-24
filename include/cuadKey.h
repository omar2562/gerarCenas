#ifndef CUADKEY_H
#define CUADKEY_H
#include "opencv/cv.h"
#include "readFil.h"




class cuadKey
{
public:
virtual ~cuadKey();
    cuadKey();
    readFil tempDir;
    //vector<cv::Mat> getcuadKeys();
    void compHist(cv::Mat base,cv::Mat test,int hue,int bins_hue,int sat,int bins_sat);
    //vector<vector<pair(string,cv::Mat)> > getcuadKeys(vector<string> namedir,string mane);


};

#endif // CUADKEY_H
