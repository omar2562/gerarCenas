#ifndef CENA_H
#define CENA_H

#include <iostream>
#include <vector>
#include "Tomada.h"

using namespace std;

class Cena
{
public:
    Cena();
    virtual ~Cena();
    vector<Tomada> getTomadaVector()
    {
        return tomadaVector;
    }
    void addTomada(Tomada tomada)
    {
        tomadaVector.push_back(tomada);
    }
    void setTomadaVector(vector<Tomada>& vec)
    {
        tomadaVector = vec;
    }
protected:
private:
    vector<Tomada> tomadaVector;
    friend ostream& operator<<(ostream &strm, const Cena &cena);
};

#endif // CENA_H
