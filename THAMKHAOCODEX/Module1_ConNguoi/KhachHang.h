#ifndef KHACHHANG_H
#define KHACHHANG_H
#include "Nguoi.h"
class KhachHang : public Nguoi {
    int diemTichLuy = 0;
    std::string hangVip;
    void capNhatHangVip();

   public:
    KhachHang() = default;
    KhachHang(std::string, std::string, std::string, std::string, int);
    int getDiemTichLuy() const;
    const std::string& getHangVip() const;
    void setDiemTichLuy(int);
    void congDiem(int);
    double getPhanTramGiam() const;
    void hienThiThongTin() const override;
    std::string toFile() const;
};
#endif
