#include "HoaDon.h"

#include <iomanip>
#include <sstream>

using namespace std;

namespace {
string dinhDangSo(double giaTri) {
    ostringstream s;
    s << fixed << setprecision(6) << giaTri;
    string kq = s.str();
    if (kq.find('.') != string::npos) {
        while (!kq.empty() && kq.back() == '0') kq.pop_back();
        if (!kq.empty() && kq.back() == '.') kq.pop_back();
    }
    return (kq.empty() || kq == "-0") ? "0" : kq;
}

vector<string> tach(const string& chuoi, char dauPhanCach) {
    vector<string> cacTruong;
    stringstream luongChuoi(chuoi);
    string truong;
    while (getline(luongChuoi, truong, dauPhanCach))
        cacTruong.push_back(truong);
    return cacTruong;
}
}  // namespace
HoaDon::HoaDon(string ma, string ngay, string maKhach, string maNhanVienMoi)
    : maHoaDon(ma), ngayLap(ngay), maKhachHang(maKhach), maNhanVien(maNhanVienMoi) {
}

const string& HoaDon::getMa() const {
    return maHoaDon;
}

const string& HoaDon::getNgayLap() const {
    return ngayLap;
}

const string& HoaDon::getMaKhachHang() const {
    return maKhachHang;
}

const vector<ChiTietHoaDon>& HoaDon::getGioHang() const {
    return gioHang;
}

void HoaDon::themMon(const ChiTietHoaDon& chiTietMoi) {
    for (ChiTietHoaDon& chiTiet : gioHang)
        // Cung ma nuoc nhung khac size/da duoc xem la hai lua chon rieng.
        if (chiTiet.maMon == chiTietMoi.maMon && chiTiet.tenMon == chiTietMoi.tenMon) {
            chiTiet.soLuong += chiTietMoi.soLuong;
            return;
        }
    gioHang.push_back(chiTietMoi);
}

void HoaDon::setGiamGia(double phanTram) {
    if (phanTram >= 0 && phanTram <= 100) {
        phanTramGiam = phanTram;
    } else {
        phanTramGiam = 0;
    }
}

double HoaDon::tamTinh() const {
    double tongTienHang = 0;
    for (const ChiTietHoaDon& chiTiet : gioHang)
        tongTienHang += chiTiet.thanhTien();
    return tongTienHang;
}

double HoaDon::tienGiam() const {
    return tamTinh() * phanTramGiam / 100;
}

double HoaDon::tongTien() const {
    return tamTinh() - tienGiam();
}

vector<string> HoaDon::cacCot() const {
    ostringstream dongDuLieu;
    for (size_t i = 0; i < gioHang.size(); ++i) {
        if (i)
            dongDuLieu << ';';
        dongDuLieu << gioHang[i].maMon << ',' << gioHang[i].tenMon << ',' << gioHang[i].soLuong
                   << ',' << dinhDangSo(gioHang[i].donGia);
    }
    return {maHoaDon, ngayLap, maKhachHang, maNhanVien, dinhDangSo(phanTramGiam), dongDuLieu.str()};
}

HoaDon HoaDon::tuCacCot(const vector<string>& cacTruong) {
    if (cacTruong.size() < 6)
        return HoaDon();
    HoaDon hoaDon(cacTruong[0], cacTruong[1], cacTruong[2], cacTruong[3]);
    try { if (!cacTruong[4].empty()) hoaDon.setGiamGia(stod(cacTruong[4])); } catch (...) {}
    for (const string& dongChiTiet : tach(cacTruong[5], ';')) {
        vector<string> truongChiTiet = tach(dongChiTiet, ',');
        if (truongChiTiet.size() >= 4) {
            string tenMon = truongChiTiet[1];
            for (size_t i = 2; i + 2 < truongChiTiet.size(); i++) {
                tenMon += "," + truongChiTiet[i];
            }
            size_t cuoi = truongChiTiet.size();
            try {
                hoaDon.themMon({truongChiTiet[0], tenMon, stoi(truongChiTiet[cuoi - 2]),
                                stod(truongChiTiet[cuoi - 1])});
            } catch (...) {}
        }
    }
    return hoaDon;
}

// Bảng DonDat lưu bản tạm tính tại thời điểm khách đặt.
string HoaDon::maHoaDuTinh() const {
    const auto cot = cacCot();
    return cot[0] + "|" + cot[1] + "|" + cot[2] + "|" + cot[3] + "|" + cot[4] + "|" + cot[5];
}

HoaDon HoaDon::docDuTinh(const string& duTinh) {
    return tuCacCot(tach(duTinh, '|'));
}
