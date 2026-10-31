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
    friend class DichVuNhaHang;
    vector<KhachHang> dsKhachHang;
    vector<NhanVien> dsNhanVien;
    // unique_ptr tu giai phong mon; MonAn va NuocUong dung chung lop cha Mon.
    vector<unique_ptr<Mon>> dsMon;
    vector<HoaDon> dsHoaDon;
    QuanLyTaiKhoan quanLyTaiKhoan;
    TaiKhoan taiKhoanHienTai;
    KhachHang* timKhach(const string& maCanTim);
    NhanVien* timNhanVien(const string& maCanTim);
    Mon* timMon(const string& maCanTim);

   public:
    void docDuLieu();
};
#endif
