#ifndef QUANLY_H
#define QUANLY_H
#include <memory>
#include <vector>

#include "../Module1_ConNguoi/KhachHang.h"
#include "../Module1_ConNguoi/NhanVien.h"
#include "../Module2_ThucDon/Mon.h"
#include "../Module3_BanHang/HoaDon.h"
class QuanLy {
    std::vector<KhachHang> dsKhachHang;
    std::vector<NhanVien> dsNhanVien;
    std::vector<std::unique_ptr<Mon>> dsMon;
    std::vector<HoaDon> dsHoaDon;
    void menuKhachHang();
    void menuNhanVien();
    void menuThucDon();
    void banHang();
    void menuThongKe() const;
    void hienThiKhachHang() const;
    void hienThiNhanVien() const;
    void hienThiThucDon(int loc = 0, int sapXep = 0, bool chiConHang = false) const;
    KhachHang* timKhach(const std::string&);
    NhanVien* timNhanVien(const std::string&);
    Mon* timMon(const std::string&);

   public:
    void docDuLieu();
    void ghiDuLieu() const;
    void chay();
};
#endif
