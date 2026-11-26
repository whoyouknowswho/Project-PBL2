#include "KhachHang.h"
#include <algorithm>

KhachHang::KhachHang(string maKhach, string hoTen, string dienThoai, string gioiTinhMoi, int diem, string emailMoi)
    : Nguoi(maKhach, hoTen, dienThoai, gioiTinhMoi), diemTichLuy(diem), email(emailMoi) {
    capNhatHangVip();
}

void KhachHang::capNhatHangVip() {
    if (diemTichLuy >= 1000) hangVip = "Kim Cuong";
    else if (diemTichLuy >= 500) hangVip = "Vang";
    else if (diemTichLuy >= 200) hangVip = "Bac";
    else hangVip = "Thuong";
}

int KhachHang::getDiemTichLuy() const { return diemTichLuy; }
const string& KhachHang::getHangVip() const { return hangVip; }
const string& KhachHang::getEmail() const { return email; }

void KhachHang::setDiemTichLuy(int diem) {
    diemTichLuy = max(0, diem);
    capNhatHangVip();
}

void KhachHang::congDiem(int diem) { setDiemTichLuy(diemTichLuy + diem); }

double KhachHang::getPhanTramGiam() const {
    if (hangVip == "Kim Cuong") return 15;
    if (hangVip == "Vang") return 10;
    if (hangVip == "Bac") return 5;
    return 0;
}

vector<string> KhachHang::cacCot() const {
    return {ma, ten, soDienThoai, gioiTinh, to_string(diemTichLuy), email};
}
