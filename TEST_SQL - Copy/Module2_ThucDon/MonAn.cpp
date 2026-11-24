#include "MonAn.h"

MonAn::MonAn(string maMon, string tenMon, double giaMon, int soLuongTon, int mucDoCay, bool laMonChay)
    : Mon(maMon, tenMon, giaMon, soLuongTon), doCay(mucDoCay), monChay(laMonChay) {}

int MonAn::getDoCay() const { return doCay; }
bool MonAn::laMonChay() const { return monChay; }
string MonAn::getLoai() const { return "Mon an"; }

string MonAn::hienThiChiTiet() const {
    return "Cay " + to_string(doCay) + "/5, " + (monChay ? "Chay" : "Man");
}

vector<string> MonAn::cacCot() const {
    return {"MONAN", ma, ten, to_string(gia), to_string(tonKho), to_string(doCay), to_string(monChay), hinhAnh};
}
