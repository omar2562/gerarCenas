#ifndef QUADRO_H
#define QUADRO_H
#include <cv.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class Quadro
{
public:
    Quadro();
    virtual ~Quadro();
    string getQuadroPath()
    {
        return quadroPath;
    }
    void setQuadroPath(string val)
    {
        quadroPath = val;
    }
    vector<Mat> getPlanoVector()
    {
        return planoVector;
    }
    void addPlanoVector(Mat quadro)
    {
        planoVector.push_back(quadro);
    }
    void setPlanoVector(vector<Mat>& vec)
    {
        planoVector = vec;
    }
protected:
private:
    string quadroPath;
    vector<Mat> planoVector;
    friend ostream& operator<<(ostream &strm, const Quadro &quadro);
};

#endif // QUADRO_H
