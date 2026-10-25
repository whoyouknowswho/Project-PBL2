#ifndef KHACHHANG_H
#define KHACHHANG_H
#include "Nguoi.h"

using namespace std;
class KhachHang : public Nguoi {
    int diemTichLuy = 0;
    string hangVip;
    void capNhatHangVip();

   public:
    KhachHang() = default;
    KhachHang(string maKhach, string hoTen, string dienThoai, string gioiTinh, int diem);
    int getDiemTichLuy() const;
    const string& getHangVip() const;
    void setDiemTichLuy(int diem);
    void congDiem(int diem);
    double getPhanTramGiam() const;
    void hienThiThongTin() const override;
    string toFile() const;
};
#endif
