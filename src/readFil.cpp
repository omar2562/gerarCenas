#include "readFil.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <algorithm>

using namespace std;

readFil::readFil()
{};
readFil::~readFil()
{
    //dtor
}
readFil::readFil(string a){name=a;}
void readFil::readAllFil(string dirname)
{

    nameTomada.clear();

    struct dirent *epdf;
    struct dirent *epdf2;
    //string direcname="/media/01CE6504CC3A4B10/pirates2";
    dpdf = opendir(dirname.c_str());
    string  aa="";
    string bb="";
    if (dpdf != NULL){
        while (epdf = readdir(dpdf)){



            if((strcmp(epdf->d_name,"..") && strcmp(epdf->d_name,".")))
                nameTomada.push_back(epdf->d_name);

        }
        std::sort(nameTomada.begin(),nameTomada.end());
    }
    else{cout<<"error en leer archivo que paso ???"<<endl;}
    closedir(dpdf);

};
void readFil::readAllFil2(string dirname,vector<string> vec)
{

};
vector<string> readFil::getArrTomada()
{
}
vector<string> readFil::getNameTomada()
{
    return nameTomada;
}
