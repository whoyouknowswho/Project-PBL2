#ifndef DICHVU_NHAHANG_H
#define DICHVU_NHAHANG_H

#include "../Module4_QuanLy/QuanLy.h"

using namespace std;

// Du lieu cua bieu mau; khong chua bat ky thanh phan Win32 nao.
struct DuLieuMon {
    string ma, ten, loai = "Mon an", sizeLy = "M";
    double gia = 0;
    int tonKho = 0, doCay = 0;
    bool monChay = false, coDa = true;
};

struct YeuCauMon {
    string maMon, sizeLy = "M";
    int soLuong = 1;
    bool coDa = true;
};

struct BaoCaoKinhDoanh {
    double doanhThu = 0;
    int soHoaDon = 0;
    string monBanChay = "Chưa có dữ liệu";
    vector<HoaDon> hoaDon;
};

class DichVuNhaHang {
    QuanLy duLieu;
    bool canThietLap = false;
    void yeuCauDangNhap() const;
    void yeuCauQuanLy() const;
    void luuThayDoi();

public:
    bool khoiTao(); // true: can tao tai khoan quan ly dau tien
    bool dangNhap(const string& ten, const string& matKhau);
    void dangXuat();
    bool daDangNhap() const;
    bool laQuanLy() const;
    const TaiKhoan& phien() const;
    void taoTaiKhoan(const string& ten, const string& matKhau,
                    const string& xacNhan, bool quanLy, const string& maNhanVien);
    void doiMatKhau(const string& cu, const string& moi, const string& xacNhan);

    const vector<KhachHang>& khachHang() const;
    const vector<NhanVien>& nhanVien() const;
    const vector<unique_ptr<Mon>>& thucDon() const;
    void luuKhachHang(const KhachHang& khach, bool themMoi);
    void luuNhanVien(const NhanVien& nhanVien, bool themMoi);
    void luuMon(const DuLieuMon& mon, bool themMoi);
    void xoaKhachHang(const string& ma);
    void xoaNhanVien(const string& ma);
    void xoaMon(const string& ma);
    void nhapKho(const string& ma, int soLuong);
    HoaDon xemTruocHoaDon(const string& ma, const string& ngay, const string& khach,
                         const string& nhanVien, const vector<YeuCauMon>& gioHang);
    HoaDon thanhToan(const string& ma, const string& ngay, const string& khach,
                     const string& nhanVien, const vector<YeuCauMon>& gioHang);
    BaoCaoKinhDoanh baoCao(const string& phamVi) const;
};

#endif
