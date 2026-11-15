#ifndef TAIKHOAN_H
#define TAIKHOAN_H

#include <string>
#include <vector>

using namespace std;

struct TaiKhoan {
    string tenDangNhap;
    string matKhau;
    string vaiTro; // QUANLY, NHANVIEN hoac KHACHHANG
};

class QuanLyTaiKhoan {
    friend class DichVuNhaHang;
    vector<TaiKhoan> danhSach;
    TaiKhoan chuanBi(const string& ten, const string& matKhau, const string& vaiTro) const;
    vector<vector<string>> cacDongTaiKhoan(const vector<TaiKhoan>& duLieu) const;
    void luu(const vector<TaiKhoan>& duLieu) const;

public:
    bool docDuLieu();
    const TaiKhoan* tim(const string& tenDangNhap) const;
    bool kiemTraMatKhau(const string& tenDangNhap, const string& matKhau) const;
    void tao(const string& tenDangNhap, const string& matKhau, const string& vaiTro);
    void doiMatKhau(const string& tenDangNhap, const string& matKhauMoi);
};

#endif
