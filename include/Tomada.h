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
    int getInitQuadro()
    {
        return initQuadro;
    }
    void setInitQuadro(int val)
    {
        initQuadro = val;
    }
    int getEndQuadro()
    {
        return endQuadro;
    }
    void setEndQuadro(int val)
    {
        endQuadro = val;
    }
    int getNumTomada()
    {
        return numTomada;
    }
    void setNumTomada(int val)
    {
        numTomada = val;
    }
    string getPathTomada()
    {
        return pathTomada;
    }
    void setPathTomada(string val)
    {
        pathTomada = val;
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
    int initQuadro;
    int endQuadro;
    int numTomada;
    string pathTomada;
    vector<Mat> quadroClaveVector;
    friend ostream& operator<<(ostream &strm, const Tomada &tomada);
};

#endif // TOMADA_H
