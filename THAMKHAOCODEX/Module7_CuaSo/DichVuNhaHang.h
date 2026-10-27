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

struct DoanhThuNam {
    vector<double> cacThang = vector<double>(12,0);
    vector<int> cacNam;
    double tongTien = 0;
    int soHoaDon = 0, soHoaDonBoQua = 0;
};

struct DonDatMon {
    HoaDon duTinh;
    vector<YeuCauMon> cacMon;
    string trangThai = "CHO", maHoaDon;
};

class DichVuNhaHang {
    QuanLy duLieu;
    bool canThietLap = false;
    vector<DonDatMon> cacDonDat;
    void docDonDat();
    string noiDungDonDat() const;
    bool ngayDatHopLe(const string& ngay) const;
    DonDatMon& timDonDat(const string& ma);
    HoaDon lapDuTinh(const string& ma, const string& ngay, const string& khach,
                     const vector<YeuCauMon>& gioHang);
    void yeuCauDangNhap() const;
    void yeuCauQuanLy() const;
    void yeuCauNhanSu() const;
    void yeuCauNhanVien() const;
    void luuThayDoi(bool kemTaiKhoan = false);

public:
    HoaDon duTinhDatMon(const vector<YeuCauMon>& gioHang);
    string datMon(const string& ngay, const vector<YeuCauMon>& gioHang);
    vector<DonDatMon> danhSachDonDat() const;
    void huyDonDat(const string& ma);
    HoaDon xemTruocDonDat(const string& ma, const string& ngay, const string& nhanVien);
    HoaDon thanhToanDonDat(const string& ma, const string& ngay, const string& nhanVien);
    bool khoiTao(); // true: can tao tai khoan quan ly dau tien
    bool dangNhap(const string& ten, const string& matKhau);
    void dangXuat();
    bool daDangNhap() const;
    bool laQuanLy() const;
    bool laKhachHang() const;
    const TaiKhoan& phien() const;
    void taoTaiKhoan(const string& ten, const string& matKhau,
                    const string& xacNhan, const string& vaiTro, const string& maLienKet);
    void doiMatKhau(const string& cu, const string& moi, const string& xacNhan);
    string dangKyKhachHang(const string& hoTen, const string& dienThoai,
                          const string& gioiTinh, const string& tenDangNhap,
                          const string& matKhau, const string& xacNhan);

    const vector<KhachHang>& khachHang() const;
    const vector<NhanVien>& nhanVien() const;
    const vector<unique_ptr<Mon>>& thucDon() const;
    const KhachHang& hoSoCuaToi() const;
    vector<HoaDon> hoaDonCuaToi() const;
    HoaDon hoaDonDuocXem(const string& maHoaDon) const;
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
    DoanhThuNam doanhThuTheoNam(int nam) const;
};

#endif
