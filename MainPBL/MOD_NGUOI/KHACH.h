#ifndef KHACH_H
#define KHACH_H

#include "NGUOI.h"
class KHACH : public NGUOI {
private:
    int diem = 0;
    string vip;
    void updatevip();
public:
    KHACH();
    KHACH(string, string, string, string, int);
    int getdiem() const;
    const string& getvip() const;
    void setdiem(int);
    void congdiem(int);
    void show() const override;
    string toFile() const;
};

#endif