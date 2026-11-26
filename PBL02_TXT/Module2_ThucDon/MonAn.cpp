#include "MonAn.h"
#include <iomanip>
#include <sstream>

namespace {
string dinhDangSo(double giaTri) {
    ostringstream s;
    s << fixed << setprecision(6) << giaTri;
    string kq = s.str();
    if (kq.find('.') != string::npos) {
        while (!kq.empty() && kq.back() == '0') kq.pop_back();
        if (!kq.empty() && kq.back() == '.') kq.pop_back();
    }
    return (kq.empty() || kq == "-0") ? "0" : kq;
}
}

MonAn::MonAn(string maMon, string tenMon, double giaMon, int soLuongTon, int mucDoCay, bool laMonChay)
    : Mon(maMon, tenMon, giaMon, soLuongTon), doCay(mucDoCay), monChay(laMonChay) {}

int MonAn::getDoCay() const { return doCay; }
bool MonAn::laMonChay() const { return monChay; }
string MonAn::getLoai() const { return "Mon an"; }

string MonAn::hienThiChiTiet() const {
    return "Cay " + to_string(doCay) + "/5, " + (monChay ? "Chay" : "Man");
}

vector<string> MonAn::cacCot() const {
    return {"MONAN", ma, ten, dinhDangSo(gia), to_string(tonKho), to_string(doCay), to_string(monChay), hinhAnh};
}
