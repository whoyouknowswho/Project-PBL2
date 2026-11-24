#include "NuocUong.h"

NuocUong::NuocUong(string maMon, string tenMon, double giaMon, int soLuongTon, string kichCo, bool themDa)
    : Mon(maMon, tenMon, giaMon, soLuongTon), sizeLy(kichCo), coDa(themDa) {}

const string& NuocUong::getSizeLy() const { return sizeLy; }
bool NuocUong::getCoDa() const { return coDa; }
string NuocUong::getLoai() const { return "Nuoc uong"; }
string NuocUong::hienThiChiTiet() const { return "Size " + sizeLy; }

vector<string> NuocUong::cacCot() const {
    return {"NUOC", ma, ten, to_string(gia), to_string(tonKho), sizeLy, to_string(coDa), hinhAnh};
}
