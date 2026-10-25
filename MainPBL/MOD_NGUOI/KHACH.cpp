
#include "KHACH.h"

#include <iostream>
#include <string>
using namespace std;

KHACH::KHACH(string a, string b, string c, string d, int e) : NGUOI(a, b, c, d), diem(e) {
    updatevip();
}

void KHACH::updatevip() {
    if (diem >= 1000) {
        vip = "Kim Cuong";
    } else if (diem >= 500) {
        vip = "Vang";
    } else if (diem >= 200) {
        vip = "Bac";
    } else {
        vip = "Thuong";
    }
}

int KHACH::getdiem() const {
    return diem
}

const string& KHACH::getvip() const {
    return vip;
}
void KHACH::setdiem(int a) {
    if (this->diem < 0) {
        this->diem = 0
    } else {
        this->diem = a;
    }
    updatevip();
}
void KHACH::congdiem(int a) {
    setdiem(this->diem + a);
}
string KHACH::toFile() const {
    return ma + '|' + hoten + '|' + SDT + '|' + SEX + '|' + to_string(diem);
}