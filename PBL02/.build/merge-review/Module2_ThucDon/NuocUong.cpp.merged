#include "NuocUong.h"

#include <sstream>
#include <iomanip>

using namespace std;

NuocUong::NuocUong(string maMon, string tenMon, double giaMon, int soLuongTon, string kichCo,
                   bool themDa)
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
    return "Size " + sizeLy;
}

string NuocUong::toDuLieu() const {
    ostringstream dongDuLieu;
    dongDuLieu << fixed << setprecision(4);
    dongDuLieu << "NUOC|" << ma << '|' << ten << '|' << gia << '|' << tonKho << '|' << sizeLy << '|'
               << coDa;
    return dongDuLieu.str();
}
