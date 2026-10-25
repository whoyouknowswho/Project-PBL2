#ifndef NGUOI_H
#define NGUOI_H

#include <iostream>
#include <string>
using namespace std;

class NGUOI {
protected:
    string ma;
    string hoten;
    string SDT;
    string SEX;

public:
    NGUOI();
    NGUOI(string, string, string, string);
    virtual ~NGUOI() = default;
    const string& getma() const;
    const string& getten() const;
    const string& getSDT() const;
    const string& getSEX() const;
    void update(const string&, const string&, const string&);
    virtual void show() const;
};

#endif