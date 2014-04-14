#ifndef TOMADA_H
#define TOMADA_H
#include <iostream>

using namespace std;

class Tomada
{
    public:
        Tomada();
        virtual ~Tomada();
        int getInitQuadro() { return initQuadro; }
        void setInitQuadro(int val) { initQuadro = val; }
        int getEndQuadro() { return endQuadro; }
        void setEndQuadro(int val) { endQuadro = val; }
        int getNumTomada() { return numTomada; }
        void setNumTomada(int val) { numTomada = val; }
        string getPathTomada() { return pathTomada; }
        void setPathTomada(string val) { pathTomada = val; }
    protected:
    private:
        int initQuadro;
        int endQuadro;
        int numTomada;
        string pathTomada;
        friend ostream& operator<<(ostream &strm, const Tomada &tomada);
};

#endif // TOMADA_H
