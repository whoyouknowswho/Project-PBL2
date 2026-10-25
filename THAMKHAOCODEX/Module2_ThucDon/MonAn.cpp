#include "MonAn.h"

#include <iostream>
#include <sstream>

using namespace std;

MonAn::MonAn(string maMon, string tenMon, double giaMon, int soLuongTon, int mucDoCay, bool laMonChay)
    : Mon(maMon, tenMon, giaMon, soLuongTon), doCay(mucDoCay), monChay(laMonChay) {
}

int MonAn::getDoCay() const {
    return doCay;
}

bool MonAn::laMonChay() const {
    return monChay;
}

string MonAn::getLoai() const {
    return "Mon an";
}

string MonAn::hienThiChiTiet() const {
    string loaiMon = "Man";
    if (monChay) {
        loaiMon = "Chay";
    }
    return "Cay " + to_string(doCay) + "/5, " + loaiMon;
}

string MonAn::taoTenKhiBan() const {
    return ten;
}

void MonAn::capNhatChiTiet() {
    cout << "Do cay (0-5): ";
    cin >> doCay;
    cout << "Mon chay (1/0): ";
    cin >> monChay;
}

string MonAn::toFile() const {
    ostringstream dongDuLieu;
    dongDuLieu << "MONAN|" << ma << '|' << ten << '|' << gia << '|' << tonKho << '|' << doCay << '|'
      << monChay;
    return dongDuLieu.str();
}
