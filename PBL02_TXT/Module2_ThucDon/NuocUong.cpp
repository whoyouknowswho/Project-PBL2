#include "NuocUong.h"
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

NuocUong::NuocUong(string maMon, string tenMon, double giaMon, int soLuongTon, string kichCo, bool themDa)
    : Mon(maMon, tenMon, giaMon, soLuongTon), sizeLy(kichCo), coDa(themDa) {}

const string& NuocUong::getSizeLy() const { return sizeLy; }
bool NuocUong::getCoDa() const { return coDa; }
string NuocUong::getLoai() const { return "Nuoc uong"; }
string NuocUong::hienThiChiTiet() const { return "Size " + sizeLy; }

vector<string> NuocUong::cacCot() const {
    return {"NUOC", ma, ten, dinhDangSo(gia), to_string(tonKho), sizeLy, to_string(coDa), hinhAnh};
}
