#ifndef GIAODIEN_H
#define GIAODIEN_H

#include <memory>
#include <string>
#include <vector>
#include "../Module1_ConNguoi/KhachHang.h"
#include "../Module1_ConNguoi/NhanVien.h"
#include "../Module2_ThucDon/Mon.h"

using namespace std;

namespace GiaoDien {
extern const string XANH, DO, LAM, VANG, RESET;
void khoiTao();
string nhapChuoi(const string& thongBao, bool choRong = false);
string nhapMatKhau(const string& thongBao);
int nhapInt(const string& thongBao, int min = 0, int max = 1000000000);
double nhapDouble(const string& thongBao, double min = 0);
bool xacNhan(const string& thongBao);
void dungManHinh();
void xoaManHinh();
void tieuDe(const string& s);
string canTrai(string s, int doRong);
string canPhai(string s, int doRong);
string canGiua(string s, int doRong);
void menuChinh(const string& tenDangNhap, bool laQuanLy);
void tieuDeThongKe();
void hienThiKhachHang(const vector<KhachHang>& dsKhachHang);
void hienThiNhanVien(const vector<NhanVien>& dsNhanVien);
void hienThiThucDon(const vector<const Mon*>& danhSach);
void hienThiThongKe(const string& phamVi, int so, double dt,
                      const string& monBanChay,
                      const vector<unique_ptr<Mon>>& dsMon);
}  // namespace GiaoDien
#endif
