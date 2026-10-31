#ifndef HOADON_H
#define HOADON_H
#include <string>
#include <vector>

using namespace std;
struct ChiTietHoaDon {
    string maMon, tenMon;
    int soLuong = 0;
    double donGia = 0;
    double thanhTien() const {
        return soLuong * donGia;
    }
};
class HoaDon {
    string maHoaDon, ngayLap, maKhachHang, maNhanVien;
    vector<ChiTietHoaDon> gioHang;
    double phanTramGiam = 0;

   public:
    HoaDon() = default;
    HoaDon(string maHoaDon, string ngayLap, string maKhachHang, string maNhanVien);
    const string& getMa() const;
    const string& getNgayLap() const;
    const string& getMaKhachHang() const;
    const vector<ChiTietHoaDon>& getGioHang() const;
    void themMon(const ChiTietHoaDon& chiTiet);
    void setGiamGia(double phanTram);
    double tamTinh() const;
    double tienGiam() const;
    double tongTien() const;
    string toFile() const;
    static HoaDon fromFile(const string& dongDuLieu);
};
#endif
