#include "Nguoi.h"

using namespace std;

Nguoi::Nguoi(string maNguoi, string hoTen, string dienThoai, string gioiTinhMoi)
    : ma(maNguoi), ten(hoTen), soDienThoai(dienThoai), gioiTinh(gioiTinhMoi) {
}

const string& Nguoi::getMa() const {
    return ma;
}

const string& Nguoi::getTen() const {
    return ten;
}

const string& Nguoi::getSoDienThoai() const {
    return soDienThoai;
}

const string& Nguoi::getGioiTinh() const {
    return gioiTinh;
}

void Nguoi::capNhatThongTin(const string& hoTen, const string& dienThoai, const string& gioiTinhMoi) {
    ten = hoTen;
    soDienThoai = dienThoai;
    gioiTinh = gioiTinhMoi;
}
