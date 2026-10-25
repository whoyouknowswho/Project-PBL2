#ifndef QUANLY_H
#define QUANLY_H
#include <memory>
#include <vector>

#include "../Module1_ConNguoi/KhachHang.h"
#include "../Module1_ConNguoi/NhanVien.h"
#include "../Module2_ThucDon/Mon.h"
#include "../Module3_BanHang/HoaDon.h"
#include "../Module6_TaiKhoan/TaiKhoan.h"

using namespace std;
class QuanLy {
    vector<KhachHang> dsKhachHang;
    vector<NhanVien> dsNhanVien;
    // unique_ptr tu giai phong mon; MonAn va NuocUong dung chung lop cha Mon.
    vector<unique_ptr<Mon>> dsMon;
    vector<HoaDon> dsHoaDon;
    QuanLyTaiKhoan quanLyTaiKhoan;
    TaiKhoan taiKhoanHienTai;
    bool laQuanLy() const;
    bool dangNhap();
    bool taoTaiKhoan(bool lanDau = false);
    void doiMatKhau();
    void menuKhachHang();
    void menuNhanVien();
    void menuThucDon();
    void banHang();
    void menuThongKe() const;
    void hienThiKhachHang() const;
    void hienThiNhanVien() const;
    void hienThiThucDon(int loc = 0, int sapXep = 0, bool chiConHang = false) const;
    KhachHang* timKhach(const string& maCanTim);
    NhanVien* timNhanVien(const string& maCanTim);
    Mon* timMon(const string& maCanTim);

   public:
    void docDuLieu();
    void ghiDuLieu() const;
    void chay();
};
#endif
