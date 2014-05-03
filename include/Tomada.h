#ifndef TOMADA_H
#define TOMADA_H
#include <cv.h>
#include <iostream>
#include <vector>
#include "Quadro.h"

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
    void setCoherence(double val)
    {
        coherence = val;
    }
    double getCoherence()
    {
        return coherence;
    }
    void setTomadaName(string name)
    {
        tomadaName = name;
    }
    vector<Quadro> getQuadroVector()
    {
        return quadroVector;
    }
    void addQuadroVector(Quadro quadro)
    {
        quadroVector.push_back(quadro);
    }
    void setQuadroVector(vector<Quadro> vec)
    {
        quadroVector = vec;
    }
    vector<Quadro> getQuadroClaveVector()
    {
        return quadroClaveVector;
    }
    void addQuadroChaveVector(Quadro quadro)
    {
        quadroClaveVector.push_back(quadro);
    }
    void setQuadroClaveVector(vector<Quadro>& vec)
    {
        quadroClaveVector = vec;
    }
protected:
private:
    string tomadaName;
    double coherence;
    vector<Quadro> quadroVector;
    vector<Quadro> quadroClaveVector;
    friend ostream& operator<<(ostream &strm, const Tomada &tomada);
};

#endif // TOMADA_H
