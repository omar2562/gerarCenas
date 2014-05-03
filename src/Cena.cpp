#include "Cena.h"
#include <vector>

Cena::Cena()
{
    //ctor
}

Cena::~Cena()
{
    //dtor
}
ostream& operator<<(ostream &strm, const Cena &cena)
{
    vector<Tomada> tom;
    tom = cena.tomadaVector;
    strm << "Cena(" << cena.tomadaVector.size() << "\n";
    vector<Tomada>::iterator it;
    for (it=tom.begin(); it<tom.end(); it++)
        strm << " \t" << (*it) << ";\n";
    strm << ")\n";
    return strm;
}
