#include "HoaDon.h"

#include <sstream>

using namespace std;

namespace {
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
    : maHoaDon(ma)
    , ngayLap(ngay)
    , maKhachHang(maKhach)
    , maNhanVien(maNhanVienMoi) {
}

const string& HoaDon::getMa() const {
    return maHoaDon;
}

const string& HoaDon::getNgayLap() const {
    return ngayLap;
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

string HoaDon::toFile() const {
    ostringstream dongDuLieu;
    dongDuLieu << maHoaDon << '|' << ngayLap << '|' << maKhachHang << '|' << maNhanVien << '|'
      << phanTramGiam << '|';
    for (size_t i = 0; i < gioHang.size(); ++i) {
        if (i)
            dongDuLieu << ';';
        dongDuLieu << gioHang[i].maMon << ',' << gioHang[i].tenMon << ',' << gioHang[i].soLuong << ','
          << gioHang[i].donGia;
    }
    return dongDuLieu.str();
}

HoaDon HoaDon::fromFile(const string& dongDuLieu) {
    vector<string> cacTruong = tach(dongDuLieu, '|');
    if (cacTruong.size() < 6)
        return HoaDon();
    HoaDon hoaDon(cacTruong[0], cacTruong[1], cacTruong[2], cacTruong[3]);
    hoaDon.setGiamGia(stod(cacTruong[4]));
    for (const string& dongChiTiet : tach(cacTruong[5], ';')) {
        vector<string> truongChiTiet = tach(dongChiTiet, ',');
        // Ten nuoc co the chua dau phay: "Tra (Size M, Co da)".
        // Hai truong cuoi luon la so luong va don gia.
        if (truongChiTiet.size() >= 4) {
            string tenMon = truongChiTiet[1];
            for (size_t i = 2; i + 2 < truongChiTiet.size(); i++) {
                tenMon += "," + truongChiTiet[i];
            }
            size_t cuoi = truongChiTiet.size();
            hoaDon.themMon({truongChiTiet[0], tenMon,
                           stoi(truongChiTiet[cuoi - 2]), stod(truongChiTiet[cuoi - 1])});
        }
    }
    return hoaDon;
}
