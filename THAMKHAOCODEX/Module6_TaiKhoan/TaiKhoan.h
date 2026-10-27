#ifndef TAIKHOAN_H
#define TAIKHOAN_H

#include <string>
#include <vector>

using namespace std;

struct TaiKhoan {
    string tenDangNhap;
    string vaiTro;       // QUANLY, NHANVIEN hoac KHACHHANG
    string maNhanVien;   // Bat buoc doi voi tai khoan nhan vien
    string muoi;         // Du lieu ngau nhien rieng cho tung mat khau
    string matKhauBam;   // Khong luu mat khau goc
    string maKhachHang;  // Chi dung cho tai khoan khach hang
};

class QuanLyTaiKhoan {
    friend class DichVuNhaHang;
    vector<TaiKhoan> danhSach;
    TaiKhoan chuanBi(const string& ten, const string& matKhau,
                    const string& vaiTro, const string& maLienKet) const;
    string noiDungTep(const vector<TaiKhoan>& duLieu) const;
    void luu(const vector<TaiKhoan>& duLieu) const;

public:
    // Tra ve false chi khi chua co tep tai khoan (lan chay dau).
    bool docDuLieu();
    const TaiKhoan* tim(const string& tenDangNhap) const;
    bool kiemTraMatKhau(const string& tenDangNhap, const string& matKhau) const;
    void tao(const string& tenDangNhap, const string& matKhau,
             const string& vaiTro, const string& maLienKet);
    void doiMatKhau(const string& tenDangNhap, const string& matKhauMoi);
};

#endif
