#include "MonAn.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>
MonAn::MonAn(std::string m, std::string t, double g, int ton, int cay, bool chay)
    : Mon(std::move(m), std::move(t), g, ton), doCay(cay), monChay(chay) {
}
int MonAn::getDoCay() const {
    return doCay;
}
bool MonAn::laMonChay() const {
    return monChay;
}
std::string MonAn::getLoai() const {
    return "Mon an";
}
std::string MonAn::hienThiChiTiet() const {
    return "Cay " + std::to_string(doCay) + "/5, " + (monChay ? "Chay" : "Man");
}
void MonAn::inChiTietRieng() const {
    std::cout << std::right << std::setw(7) << doCay << "/5 │ ";
    if (monChay)
        std::cout << "\033[32m" << std::left << std::setw(12) << "[CHAY]" << "\033[0m";
    else
        std::cout << std::left << std::setw(12) << "Man";
}
std::string MonAn::taoTenKhiBan() const {
    return ten;
}
void MonAn::capNhatChiTiet() {
    std::cout << "Do cay (0-5): ";
    std::cin >> doCay;
    std::cout << "Mon chay (1/0): ";
    std::cin >> monChay;
}
std::string MonAn::toFile() const {
    std::ostringstream o;
    o << "MONAN|" << ma << '|' << ten << '|' << gia << '|' << tonKho << '|' << doCay << '|'
      << monChay;
    return o.str();
}
