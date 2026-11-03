#include "MonAn.h"

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

string MonAn::toFile() const {
    ostringstream dongDuLieu;
    dongDuLieu << "MONAN|" << ma << '|' << ten << '|' << gia << '|' << tonKho << '|' << doCay << '|'
      << monChay;
    return dongDuLieu.str();
}
