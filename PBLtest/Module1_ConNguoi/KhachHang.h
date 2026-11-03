#ifndef KHACHHANG_H
#define KHACHHANG_H
#include "Nguoi.h"

using namespace std;
class KhachHang : public Nguoi {
    int diemTichLuy = 0;
    string hangVip;
    string email;
    void capNhatHangVip();

   public:
    KhachHang() = default;
    KhachHang(string maKhach, string hoTen, string dienThoai, string gioiTinh, int diem, string email = "");
    int getDiemTichLuy() const;
    const string& getHangVip() const;
    const string& getEmail() const;
    void setDiemTichLuy(int diem);
    void congDiem(int diem);
    double getPhanTramGiam() const;
    string toFile() const override;
};
#endif
