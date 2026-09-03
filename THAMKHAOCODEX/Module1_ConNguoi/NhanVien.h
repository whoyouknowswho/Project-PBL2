#ifndef NHANVIEN_H
#define NHANVIEN_H
#include "Nguoi.h"
class NhanVien : public Nguoi {
    std::string chucVu;
    double luongCoBan = 0, phuCap = 0;

   public:
    NhanVien() = default;
    NhanVien(std::string, std::string, std::string, std::string, std::string, double, double = 0);
    const std::string& getChucVu() const;
    double getLuongCoBan() const;
    double getPhuCap() const;
    double tinhLuong() const;
    void capNhatCongViec(const std::string&, double, double);
    void hienThiThongTin() const override;
    std::string toFile() const;
};
#endif
