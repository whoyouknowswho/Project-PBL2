#include "KhachHang.h"

#include <iomanip>
#include <iostream>
#include <utility>
KhachHang::KhachHang(std::string m, std::string t, std::string s, std::string g, int d)
    : Nguoi(std::move(m), std::move(t), std::move(s), std::move(g)), diemTichLuy(d) {
    capNhatHangVip();
}
void KhachHang::capNhatHangVip() {
    hangVip = diemTichLuy >= 1000  ? "Kim Cuong"
              : diemTichLuy >= 500 ? "Vang"
              : diemTichLuy >= 200 ? "Bac"
                                   : "Thuong";
}
int KhachHang::getDiemTichLuy() const {
    return diemTichLuy;
}
const std::string& KhachHang::getHangVip() const {
    return hangVip;
}
void KhachHang::setDiemTichLuy(int d) {
    diemTichLuy = d < 0 ? 0 : d;
    capNhatHangVip();
}
void KhachHang::congDiem(int d) {
    setDiemTichLuy(diemTichLuy + d);
}
double KhachHang::getPhanTramGiam() const {
    return hangVip == "Kim Cuong" ? 15 : hangVip == "Vang" ? 10 : hangVip == "Bac" ? 5 : 0;
}
void KhachHang::hienThiThongTin() const {
    std::cout << std::left << std::setw(10) << ma << std::setw(24) << ten << std::setw(15)
              << soDienThoai << std::setw(12) << gioiTinh << std::setw(10) << diemTichLuy << hangVip
              << '\n';
}
std::string KhachHang::toFile() const {
    return ma + '|' + ten + '|' + soDienThoai + '|' + gioiTinh + '|' + std::to_string(diemTichLuy);
}
