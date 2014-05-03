#ifndef TOMADA_H
#define TOMADA_H
#include <cv.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class Tomada
{
public:
    Tomada();
    virtual ~Tomada();
    string getTomadaName()
    {
        return tomadaName;
    }
    void setTomadaName(string name)
    {
        tomadaName = name;
    }
    vector<string> getQuadrosNameVector()
    {
        return quadrosNameVector;
    }
    void addQuadrosNameVector(string quadro)
    {
        quadrosNameVector.push_back(quadro);
    }
    void setQuadrosNameVector(vector<string> vec)
    {
        quadrosNameVector = vec;
    }
    vector<Mat> getQuadroClaveVector()
    {
        return quadroClaveVector;
    }
    void addQuadroChave(Mat quadro)
    {
        quadroClaveVector.push_back(quadro);
    }
    void setQuadroClaveVector(vector<Mat>& vec)
    {
        quadroClaveVector = vec;
    }
protected:
private:
    string tomadaName;
    vector<string> quadrosNameVector;
    vector<Mat> quadroClaveVector;
    friend ostream& operator<<(ostream &strm, const Tomada &tomada);
};

#endif // TOMADA_H
