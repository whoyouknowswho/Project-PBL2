#include "NhanVien.h"

#include <sstream>

using namespace std;

NhanVien::NhanVien(string maNhanVien, string hoTen, string dienThoai, string gioiTinhMoi, string chucVuMoi,
                   double luong, double tienPhuCap)
    : Nguoi(maNhanVien, hoTen, dienThoai, gioiTinhMoi)
    , chucVu(chucVuMoi)
    , luongCoBan(luong)
    , phuCap(tienPhuCap) {
}

const string& NhanVien::getChucVu() const {
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

vector<string> NhanVien::cacCot() const {
    return {ma, ten, soDienThoai, gioiTinh, chucVu, to_string(luongCoBan), to_string(phuCap)};
}
