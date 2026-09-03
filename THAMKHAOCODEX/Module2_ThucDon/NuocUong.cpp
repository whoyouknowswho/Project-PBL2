#include "NuocUong.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>
NuocUong::NuocUong(std::string m, std::string t, double g, int ton, std::string s, bool da)
    : Mon(std::move(m), std::move(t), g, ton), sizeLy(std::move(s)), coDa(da) {
}
const std::string& NuocUong::getSizeLy() const {
    return sizeLy;
}
bool NuocUong::getCoDa() const {
    return coDa;
}
std::string NuocUong::getLoai() const {
    return "Nuoc uong";
}
std::string NuocUong::hienThiChiTiet() const {
    return "Size " + sizeLy + ", " + (coDa ? "Co da" : "Khong da");
}
void NuocUong::inChiTietRieng() const {
    std::cout << std::left << std::setw(9) << sizeLy << " │ "
              << std::setw(12) << (coDa ? "Co da" : "Khong da");
}
std::string NuocUong::taoTenKhiBan() const {
    std::string sizeChon;
    while (true) {
        std::cout << "Chon size (S/M/L): ";
        std::getline(std::cin, sizeChon);
        if (sizeChon == "s" || sizeChon == "S") { sizeChon = "S"; break; }
        if (sizeChon == "m" || sizeChon == "M") { sizeChon = "M"; break; }
        if (sizeChon == "l" || sizeChon == "L") { sizeChon = "L"; break; }
        std::cout << "Size khong hop le. Vui long chon S, M hoac L!\n";
    }
    std::string luaChonDa;
    while (true) {
        std::cout << "Chon da (1-Co da, 0-Khong da): ";
        std::getline(std::cin, luaChonDa);
        if (luaChonDa == "1" || luaChonDa == "0") break;
        std::cout << "Lua chon khong hop le. Vui long nhap 1 hoac 0!\n";
    }
    return ten + " (Size " + sizeChon + ", " +
           (luaChonDa == "1" ? "Co da" : "Khong da") + ")";
}
void NuocUong::capNhatChiTiet() {
    std::cout << "Size (S/M/L): ";
    std::cin >> sizeLy;
    std::cout << "Co da (1/0): ";
    std::cin >> coDa;
}
std::string NuocUong::toFile() const {
    std::ostringstream o;
    o << "NUOC|" << ma << '|' << ten << '|' << gia << '|' << tonKho << '|' << sizeLy << '|' << coDa;
    return o.str();
}
