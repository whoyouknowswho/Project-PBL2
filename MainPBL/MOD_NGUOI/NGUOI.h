#ifndef NGUOI_H
#define NGUOI_H

#include <string>
#include <iostream>
using namespace std;

class NGUOI
{
protected:
    string ma;
    string hoten;
    string SDT;
    string SEX;

public:
    NGUOI();
    NGUOI(string, string, string, string);
    ~NGUOI();
    const string &getma() const;
    const string &getten() const;
    const string &getSDT() const;
    const string &getSEX() const;
    void update(const string &, const string &, const string &, const string &);
    virtual void show() const;
};

#endif