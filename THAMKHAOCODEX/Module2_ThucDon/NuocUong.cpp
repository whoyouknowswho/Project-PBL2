#include "NuocUong.h"

#include <sstream>

using namespace std;

NuocUong::NuocUong(string maMon, string tenMon, double giaMon, int soLuongTon, string kichCo, bool themDa)
    : Mon(maMon, tenMon, giaMon, soLuongTon), sizeLy(kichCo), coDa(themDa) {
}

const string& NuocUong::getSizeLy() const {
    return sizeLy;
}

bool NuocUong::getCoDa() const {
    return coDa;
}

string NuocUong::getLoai() const {
    return "Nuoc uong";
}

string NuocUong::hienThiChiTiet() const {
    string trangThaiDa = "Khong da";
    if (coDa) {
        trangThaiDa = "Co da";
    }
    return "Size " + sizeLy + ", " + trangThaiDa;
}

string NuocUong::toFile() const {
    ostringstream dongDuLieu;
    dongDuLieu << "NUOC|" << ma << '|' << ten << '|' << gia << '|' << tonKho << '|' << sizeLy << '|' << coDa;
    return dongDuLieu.str();
}
