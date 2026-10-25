#include "NHANVIEN.h"

#include <iostream>
#include <string>
using namespace std;

NHANVIEN::NHANVIEN(string a, string b, string c, string d, string a1, double b1, double b2)
    : NGUOI(a, b, c, d), chucvu(a1), luong(b1), phucap(b2) {
}
NHANVIEN::~NHANVIEN() {
}

const string& NHANVIEN::getchucvu() const {
    return (this->chucvu);
}
double NHANVIEN::getluong() const {
    return (this->luong);
}
double NHANVIEN::getphucap() const {
    return (this->phucap);
}
double NHANVIEN::tinhluong() const {
    return this->luong + this->phucap;
}
void NHANVIEN::updateVIEC(const string& a, double b, double c) const {
    this->chucvu = a;
    this->luong = b;
    this->phucap = c;
}
string NHANVIEN::tofile() const {
    return ma + "|" + hoten + "|" + SDT + "|" + SEX + "|" + chucvu + "|" + to_string(luong) + "|" +
           to_string(phucap);
}