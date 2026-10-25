#include "NuocUong.h"

#include <iostream>
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

string NuocUong::taoTenKhiBan() const {
    string sizeChon;
    while (true) {
        cout << "Chon size (S/M/L): ";
        getline(cin, sizeChon);
        if (sizeChon == "s" || sizeChon == "S") {
            sizeChon = "S";
            break;
        }
        if (sizeChon == "m" || sizeChon == "M") {
            sizeChon = "M";
            break;
        }
        if (sizeChon == "l" || sizeChon == "L") {
            sizeChon = "L";
            break;
        }
        cout << "Size khong hop le. Vui long chon S, M hoac L!\n";
    }
    string luaChonDa;
    while (true) {
        cout << "Chon da (1-Co da, 0-Khong da): ";
        getline(cin, luaChonDa);
        if (luaChonDa == "1" || luaChonDa == "0") {
            break;
        }
        cout << "Lua chon khong hop le. Vui long nhap 1 hoac 0!\n";
    }
    string trangThaiDa = "Khong da";
    if (luaChonDa == "1") {
        trangThaiDa = "Co da";
    }
    return ten + " (Size " + sizeChon + ", " + trangThaiDa + ")";
}

void NuocUong::capNhatChiTiet() {
    cout << "Size (S/M/L): ";
    cin >> sizeLy;
    cout << "Co da (1/0): ";
    cin >> coDa;
}

string NuocUong::toFile() const {
    ostringstream dongDuLieu;
    dongDuLieu << "NUOC|" << ma << '|' << ten << '|' << gia << '|' << tonKho << '|' << sizeLy << '|' << coDa;
    return dongDuLieu.str();
}
