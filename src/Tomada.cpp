#include "Tomada.h"

Tomada::Tomada()
{
    //ctor
}

Tomada::~Tomada()
{
    //dtor
}

ostream& operator<<(ostream &strm, const Tomada &tomada) {
  return strm << tomada.tomadaName <<":" << tomada.coherence;
}
