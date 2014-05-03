#include "Quadro.h"

Quadro::Quadro()
{
    //ctor
}

Quadro::~Quadro()
{
    //dtor
}

ostream& operator<<(ostream &strm, const Quadro &quadro)
{
    return strm << quadro.quadroPath <<": " << quadro.planoVector.size();
}
