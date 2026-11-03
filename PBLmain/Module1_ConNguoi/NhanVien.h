#ifndef NHANVIEN_H
#define NHANVIEN_H
#include "Nguoi.h"

using namespace std;
class NhanVien : public Nguoi {
    string chucVu;
    double luongCoBan = 0, phuCap = 0;

   public:
    NhanVien() = default;
    NhanVien(string maNhanVien, string hoTen, string dienThoai, string gioiTinh,
             string chucVu, double luong, double phuCap = 0);
    const string& getChucVu() const;
    double getLuongCoBan() const;
    double getPhuCap() const;
    double tinhLuong() const;
    string toFile() const override;
};
#endif
