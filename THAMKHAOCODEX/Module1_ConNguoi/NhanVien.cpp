#include "NhanVien.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
NhanVien::NhanVien(std::string m, std::string t, std::string s, std::string g, std::string cv,
                   double l, double pc)
    : Nguoi(std::move(m), std::move(t), std::move(s), std::move(g))
    , chucVu(std::move(cv))
    , luongCoBan(l)
    , phuCap(pc) {
}
const std::string& NhanVien::getChucVu() const {
    return chucVu;
}
double NhanVien::getLuongCoBan() const {
    return luongCoBan;
}
double NhanVien::getPhuCap() const {
    return phuCap;
}
double NhanVien::tinhLuong() const {
    return luongCoBan + phuCap;
}
void NhanVien::capNhatCongViec(const std::string& cv, double l, double pc) {
    chucVu = cv;
    luongCoBan = l;
    phuCap = pc;
}
void NhanVien::hienThiThongTin() const {
    std::cout << std::left << std::setw(10) << ma << std::setw(22) << ten << std::setw(15)
              << soDienThoai << std::setw(12) << gioiTinh << std::setw(16) << chucVu << std::right
              << std::setw(12) << std::fixed << std::setprecision(0) << tinhLuong() << '\n';
}
std::string NhanVien::toFile() const {
    std::ostringstream o;
    o << ma << '|' << ten << '|' << soDienThoai << '|' << gioiTinh << '|' << chucVu << '|'
      << luongCoBan << '|' << phuCap;
    return o.str();
}
