#include "../Module2_ThucDon/NuocUong.h"
#include "../Module2_ThucDon/MonAn.h"
// Giu nguyen API hien thi cua cac lop; tap trung dinh dang tai module giao dien.
#include "../Module1_ConNguoi/KhachHang.h"
#include "../Module1_ConNguoi/NhanVien.h"
#include "../Module3_BanHang/HoaDon.h"
#include <iomanip>
#include <iostream>

using namespace std;


namespace {
string dinhDangTien(double tien) {
    string s = to_string(static_cast<long long>(tien));
    for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3)
        s.insert(i, ".");
    return s + " VND";
}

string catChuoi(string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong)
        return s.substr(0, doRong - 3) + "...";
    return s;
}
}

void KhachHang::hienThiThongTin() const {
    cout << left << setw(10) << ma << setw(24) << ten << setw(15)
              << soDienThoai << setw(12) << gioiTinh << setw(10) << diemTichLuy << hangVip
              << '\n';
}

void NhanVien::hienThiThongTin() const {
    cout << left << setw(10) << ma << setw(22) << ten << setw(15)
              << soDienThoai << setw(12) << gioiTinh << setw(16) << chucVu << right
              << setw(12) << fixed << setprecision(0) << tinhLuong() << '\n';
}

void HoaDon::inHoaDon() const {
    string khach = maKhachHang;
    if (maKhachHang.empty()) {
        khach = "Khach le";
    }
    const string tieuDe = "HOA DON " + maHoaDon;
    const int leTrai = (76 - static_cast<int>(tieuDe.size())) / 2;

    cout << "\n\033[1;36m┌────────────────────────────────────────────────────────────────────────────┐\n"
              << "│" << string(leTrai, ' ') << tieuDe
              << string(76 - leTrai - tieuDe.size(), ' ') << "│\n"
              << "├────────────────────────────────────────────────────────────────────────────┤\n\033[0m"
              << "│ " << left << setw(23) << catChuoi("Ngay: " + ngayLap, 23)
              << setw(27) << catChuoi("Khach hang: " + khach, 27)
              << setw(24) << catChuoi("Nhan vien: " + maNhanVien, 24) << " │\n"
              << "├──────────┬────────────────────────┬────────┬──────────────┬────────────────┤\n"
              << "\033[1;33m│ " << left << setw(8) << "Ma mon" << " │ "
              << setw(22) << "Ten mon" << " │ " << right << setw(6) << "SL"
              << " │ " << setw(12) << "Don gia" << " │ " << setw(14) << "Thanh tien"
              << " │\033[0m\n"
              << "├──────────┼────────────────────────┼────────┼──────────────┼────────────────┤\n";
    for (const ChiTietHoaDon& x : gioHang) {
        cout << "│ " << left << setw(8) << catChuoi(x.maMon, 8) << " │ "
                  << setw(22) << catChuoi(x.tenMon, 22) << " │ " << right << setw(6) << x.soLuong
                  << " │ " << setw(12) << dinhDangTien(x.donGia).substr(0, dinhDangTien(x.donGia).size() - 4)
                  << " │ " << setw(14) << dinhDangTien(x.thanhTien()).substr(0, dinhDangTien(x.thanhTien()).size() - 4)
                  << " │\n";
    }
    cout << "├──────────┴────────────────────────┴────────┴──────────────┴────────────────┤\n"
              << "│ " << left << setw(60) << "Tam tinh:"
              << right << setw(14) << dinhDangTien(tamTinh()) << " │\n"
              << "│ " << left << setw(60) << ("Giam gia (" + to_string(static_cast<int>(phanTramGiam)) + "%):")
              << right << setw(14) << ("-" + dinhDangTien(tienGiam())) << " │\n"
              << "├────────────────────────────────────────────────────────────────────────────┤\n"
              << "\033[1;32m│ " << left << setw(60) << "THANH TOAN"
              << right << setw(14) << dinhDangTien(tongTien()) << " │\033[0m\n"
              << "\033[1;36m└────────────────────────────────────────────────────────────────────────────┘\033[0m\n";
}

void MonAn::inChiTietRieng() const {
    cout << right << setw(7) << doCay << "/5 │ ";
    if (monChay)
        cout << "\033[32m" << left << setw(12) << "[CHAY]" << "\033[0m";
    else
        cout << left << setw(12) << "Man";
}

void NuocUong::inChiTietRieng() const {
    string trangThaiDa = "Khong da";
    if (coDa) {
        trangThaiDa = "Co da";
    }
    cout << left << setw(9) << sizeLy << " │ "
         << setw(12) << trangThaiDa;
}
