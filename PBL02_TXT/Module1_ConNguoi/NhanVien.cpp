#include "NhanVien.h"
#include <iomanip>
#include <sstream>

namespace {
string dinhDangSo(double giaTri) {
    ostringstream s;
    s << fixed << setprecision(6) << giaTri;
    string kq = s.str();
    if (kq.find('.') != string::npos) {
        while (!kq.empty() && kq.back() == '0') kq.pop_back();
        if (!kq.empty() && kq.back() == '.') kq.pop_back();
    }
    return (kq.empty() || kq == "-0") ? "0" : kq;
}
}

NhanVien::NhanVien(string maNhanVien, string hoTen, string dienThoai, string gioiTinhMoi,
                   string chucVuMoi, double luong, double tienPhuCap)
    : Nguoi(maNhanVien, hoTen, dienThoai, gioiTinhMoi), chucVu(chucVuMoi), luongCoBan(luong), phuCap(tienPhuCap) {}

const string& NhanVien::getChucVu() const { return chucVu; }
double NhanVien::getLuongCoBan() const { return luongCoBan; }
double NhanVien::getPhuCap() const { return phuCap; }
double NhanVien::tinhLuong() const { return luongCoBan + phuCap; }

vector<string> NhanVien::cacCot() const {
    return {ma, ten, soDienThoai, gioiTinh, chucVu, dinhDangSo(luongCoBan), dinhDangSo(phuCap)};
}
