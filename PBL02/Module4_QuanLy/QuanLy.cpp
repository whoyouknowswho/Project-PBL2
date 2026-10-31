#include "QuanLy.h"

#include <fstream>
#include <sstream>

#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"

using namespace std;

namespace {
vector<string> tach(const string& chuoi, char dauPhanCach = '|') {
    vector<string> cacTruong;
    stringstream luongChuoi(chuoi);
    string truong;
    while (getline(luongChuoi, truong, dauPhanCach))
        cacTruong.push_back(truong);
    return cacTruong;
}
}  // namespace

KhachHang* QuanLy::timKhach(const string& maCanTim) {
    for (KhachHang& khachHang : dsKhachHang)
        if (khachHang.getMa() == maCanTim)
            return &khachHang;
    return nullptr;
}

NhanVien* QuanLy::timNhanVien(const string& maCanTim) {
    for (NhanVien& nhanVien : dsNhanVien)
        if (nhanVien.getMa() == maCanTim)
            return &nhanVien;
    return nullptr;
}

Mon* QuanLy::timMon(const string& maCanTim) {
    for (unique_ptr<Mon>& mon : dsMon)
        if (mon->getMa() == maCanTim)
            return mon.get();
    return nullptr;
}

void QuanLy::docDuLieu() {
    dsKhachHang.clear();
    dsNhanVien.clear();
    dsMon.clear();
    dsHoaDon.clear();
    string dongDuLieu;
    ifstream tepDuLieu("DuLieu/menu.txt");
    while (getline(tepDuLieu, dongDuLieu))
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() < 7)
                continue;
            if (cacTruong[0] == "MONAN")
                dsMon.push_back(make_unique<MonAn>(cacTruong[1], cacTruong[2], stod(cacTruong[3]),
                                                   stoi(cacTruong[4]), stoi(cacTruong[5]),
                                                   stoi(cacTruong[6])));
            else if (cacTruong[0] == "NUOC")
                dsMon.push_back(make_unique<NuocUong>(cacTruong[1], cacTruong[2],
                                                      stod(cacTruong[3]), stoi(cacTruong[4]),
                                                      cacTruong[5], stoi(cacTruong[6])));
        } catch (...) {
        }
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/khachhang.txt");
    while (getline(tepDuLieu, dongDuLieu))
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() >= 5)
                dsKhachHang.push_back(KhachHang(cacTruong[0], cacTruong[1], cacTruong[2],
                                                cacTruong[3], stoi(cacTruong[4])));
        } catch (...) {
        }
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/nhanvien.txt");
    while (getline(tepDuLieu, dongDuLieu))
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() >= 7)
                dsNhanVien.push_back(NhanVien(cacTruong[0], cacTruong[1], cacTruong[2],
                                              cacTruong[3], cacTruong[4], stod(cacTruong[5]),
                                              stod(cacTruong[6])));
        } catch (...) {
        }
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/hoadon.txt");
    while (getline(tepDuLieu, dongDuLieu))
        try {
            HoaDon hoaDon = HoaDon::fromFile(dongDuLieu);
            if (!hoaDon.getMa().empty())
                dsHoaDon.push_back(hoaDon);
        } catch (...) {
        }
}
