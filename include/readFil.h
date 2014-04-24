#ifndef READFIL_H
#define READFIL_H
#include<iostream>
#include<vector>
#include <dirent.h>

using namespace std;


class readFil
{
public:
    string name;
    DIR *dpdf;
    vector<string > nameTomada;

    virtual ~readFil();
    readFil();
    readFil(string a);
    void readAllFil(string dirname);
    vector<string> getArrTomada();
    void readAllFil2(string dirname,vector<string> vec);
    vector<string> getNameTomada();
};


#endif // READFIL_H
