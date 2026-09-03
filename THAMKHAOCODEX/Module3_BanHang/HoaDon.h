#ifndef HOADON_H
#define HOADON_H
#include <string>
#include <vector>
struct ChiTietHoaDon {
    std::string maMon, tenMon;
    int soLuong = 0;
    double donGia = 0;
    double thanhTien() const {
        return soLuong * donGia;
    }
};
class HoaDon {
    std::string maHoaDon, ngayLap, maKhachHang, maNhanVien;
    std::vector<ChiTietHoaDon> gioHang;
    double phanTramGiam = 0;

   public:
    HoaDon() = default;
    HoaDon(std::string, std::string, std::string, std::string);
    const std::string& getMa() const;
    const std::string& getNgayLap() const;
    const std::vector<ChiTietHoaDon>& getGioHang() const;
    void themMon(const ChiTietHoaDon&);
    void setGiamGia(double);
    double tamTinh() const;
    double tienGiam() const;
    double tongTien() const;
    void inHoaDon() const;
    std::string toFile() const;
    static HoaDon fromFile(const std::string&);
};
#endif
