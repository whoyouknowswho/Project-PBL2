#include "HoaDon.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
namespace {
std::vector<std::string> tach(const std::string& s, char d) {
    std::vector<std::string> v;
    std::stringstream ss(s);
    std::string x;
    while (std::getline(ss, x, d))
        v.push_back(x);
    return v;
}
std::string dinhDangTien(double tien) {
    std::string s = std::to_string(static_cast<long long>(tien));
    for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3)
        s.insert(i, ".");
    return s + " VND";
}
std::string catChuoi(std::string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong)
        return s.substr(0, doRong - 3) + "...";
    return s;
}
}  // namespace
HoaDon::HoaDon(std::string m, std::string n, std::string kh, std::string nv)
    : maHoaDon(std::move(m))
    , ngayLap(std::move(n))
    , maKhachHang(std::move(kh))
    , maNhanVien(std::move(nv)) {
}
const std::string& HoaDon::getMa() const {
    return maHoaDon;
}
const std::string& HoaDon::getNgayLap() const {
    return ngayLap;
}
const std::vector<ChiTietHoaDon>& HoaDon::getGioHang() const {
    return gioHang;
}
void HoaDon::themMon(const ChiTietHoaDon& ct) {
    for (auto& x : gioHang)
        // Cung ma nuoc nhung khac size/da duoc xem la hai lua chon rieng.
        if (x.maMon == ct.maMon && x.tenMon == ct.tenMon) {
            x.soLuong += ct.soLuong;
            return;
        }
    gioHang.push_back(ct);
}
void HoaDon::setGiamGia(double p) {
    phanTramGiam = p >= 0 && p <= 100 ? p : 0;
}
double HoaDon::tamTinh() const {
    double s = 0;
    for (const auto& x : gioHang)
        s += x.thanhTien();
    return s;
}
double HoaDon::tienGiam() const {
    return tamTinh() * phanTramGiam / 100;
}
double HoaDon::tongTien() const {
    return tamTinh() - tienGiam();
}
void HoaDon::inHoaDon() const {
    const std::string khach = maKhachHang.empty() ? "Khach le" : maKhachHang;
    const std::string tieuDe = "HOA DON " + maHoaDon;
    const int leTrai = (76 - static_cast<int>(tieuDe.size())) / 2;

    std::cout << "\n\033[1;36m┌────────────────────────────────────────────────────────────────────────────┐\n"
              << "│" << std::string(leTrai, ' ') << tieuDe
              << std::string(76 - leTrai - tieuDe.size(), ' ') << "│\n"
              << "├────────────────────────────────────────────────────────────────────────────┤\n\033[0m"
              << "│ " << std::left << std::setw(23) << catChuoi("Ngay: " + ngayLap, 23)
              << std::setw(27) << catChuoi("Khach hang: " + khach, 27)
              << std::setw(24) << catChuoi("Nhan vien: " + maNhanVien, 24) << " │\n"
              << "├──────────┬────────────────────────┬────────┬──────────────┬────────────────┤\n"
              << "\033[1;33m│ " << std::left << std::setw(8) << "Ma mon" << " │ "
              << std::setw(22) << "Ten mon" << " │ " << std::right << std::setw(6) << "SL"
              << " │ " << std::setw(12) << "Don gia" << " │ " << std::setw(14) << "Thanh tien"
              << " │\033[0m\n"
              << "├──────────┼────────────────────────┼────────┼──────────────┼────────────────┤\n";
    for (const auto& x : gioHang) {
        std::cout << "│ " << std::left << std::setw(8) << catChuoi(x.maMon, 8) << " │ "
                  << std::setw(22) << catChuoi(x.tenMon, 22) << " │ " << std::right << std::setw(6) << x.soLuong
                  << " │ " << std::setw(12) << dinhDangTien(x.donGia).substr(0, dinhDangTien(x.donGia).size() - 4)
                  << " │ " << std::setw(14) << dinhDangTien(x.thanhTien()).substr(0, dinhDangTien(x.thanhTien()).size() - 4)
                  << " │\n";
    }
    std::cout << "├──────────┴────────────────────────┴────────┴──────────────┴────────────────┤\n"
              << "│ " << std::left << std::setw(60) << "Tam tinh:"
              << std::right << std::setw(14) << dinhDangTien(tamTinh()) << " │\n"
              << "│ " << std::left << std::setw(60) << ("Giam gia (" + std::to_string(static_cast<int>(phanTramGiam)) + "%):")
              << std::right << std::setw(14) << ("-" + dinhDangTien(tienGiam())) << " │\n"
              << "├────────────────────────────────────────────────────────────────────────────┤\n"
              << "\033[1;32m│ " << std::left << std::setw(60) << "THANH TOAN"
              << std::right << std::setw(14) << dinhDangTien(tongTien()) << " │\033[0m\n"
              << "\033[1;36m└────────────────────────────────────────────────────────────────────────────┘\033[0m\n";
}
std::string HoaDon::toFile() const {
    std::ostringstream o;
    o << maHoaDon << '|' << ngayLap << '|' << maKhachHang << '|' << maNhanVien << '|'
      << phanTramGiam << '|';
    for (size_t i = 0; i < gioHang.size(); ++i) {
        if (i)
            o << ';';
        o << gioHang[i].maMon << ',' << gioHang[i].tenMon << ',' << gioHang[i].soLuong << ','
          << gioHang[i].donGia;
    }
    return o.str();
}
HoaDon HoaDon::fromFile(const std::string& line) {
    auto p = tach(line, '|');
    if (p.size() < 6)
        return {};
    HoaDon h(p[0], p[1], p[2], p[3]);
    h.setGiamGia(std::stod(p[4]));
    for (const auto& r : tach(p[5], ';')) {
        auto x = tach(r, ',');
        if (x.size() == 4)
            h.themMon({x[0], x[1], std::stoi(x[2]), std::stod(x[3])});
    }
    return h;
}
