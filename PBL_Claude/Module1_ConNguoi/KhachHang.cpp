#include "KhachHang.h"

using namespace std;

KhachHang::KhachHang(string maKhach, string hoTen, string dienThoai, string gioiTinhMoi, int diem, string emailMoi)
    : Nguoi(maKhach, hoTen, dienThoai, gioiTinhMoi), diemTichLuy(diem), email(emailMoi) {
    capNhatHangVip();
}

void KhachHang::capNhatHangVip() {
    if (diemTichLuy >= 1000) {
        hangVip = "Kim Cuong";
    } else if (diemTichLuy >= 500) {
        hangVip = "Vang";
    } else if (diemTichLuy >= 200) {
        hangVip = "Bac";
    } else {
        hangVip = "Thuong";
    }
}

int KhachHang::getDiemTichLuy() const {
    return diemTichLuy;
}

const string& KhachHang::getHangVip() const {
    return hangVip;
}

void KhachHang::setDiemTichLuy(int diem) {
    if (diem < 0) {
        diemTichLuy = 0;
    } else {
        diemTichLuy = diem;
    }
    capNhatHangVip();
}

void KhachHang::congDiem(int diem) {
    setDiemTichLuy(diemTichLuy + diem);
}

const string& KhachHang::getEmail() const {
    return email;
}

double KhachHang::getPhanTramGiam() const {
    if (hangVip == "Kim Cuong") {
        return 15;
    }
    if (hangVip == "Vang") {
        return 10;
    }
    if (hangVip == "Bac") {
        return 5;
    }
    return 0;
}

string KhachHang::toFile() const {
    string dong = ma + '|' + ten + '|' + soDienThoai + '|' + gioiTinh + '|' + to_string(diemTichLuy);
    if (!email.empty()) {
        dong += '|';
        dong += email;
    }
    return dong;
}
