#include "QuanLy.h"

#include "../Module5_CoSoDuLieu/SqlServer.h"
#include <sstream>

#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"

using namespace std;

namespace {
vector<string> tach(const string& chuoi, char dauPhanCach = '|') {
    vector<string> cacTruong;
    stringstream luongChuoi(chuoi);
    string truong;
    while (getline(luongChuoi, truong, dauPhanCach)) {
        cacTruong.push_back(truong);
    }
    return cacTruong;
}
}  // namespace

KhachHang* QuanLy::timKhach(const string& maCanTim) {
    for (KhachHang& khachHang : dsKhachHang) {
        if (khachHang.getMa() == maCanTim) {
            return &khachHang;
        }
    }
    return nullptr;
}

NhanVien* QuanLy::timNhanVien(const string& maCanTim) {
    for (NhanVien& nhanVien : dsNhanVien) {
        if (nhanVien.getMa() == maCanTim) {
            return &nhanVien;
        }
    }
    return nullptr;
}

Mon* QuanLy::timMon(const string& maCanTim) {
    for (unique_ptr<Mon>& mon : dsMon) {
        if (mon->getMa() == maCanTim) {
            return mon.get();
        }
    }
    return nullptr;
}

// Moi ham doc phu trach mot nhom, de theo doi va sua rieng tung phan.
void QuanLy::docDuLieu() {
    docThucDon();
    docKhachHang();
    docNhanVien();
    docHoaDon();
}

void QuanLy::docThucDon() {
    dsMon.clear();
    string dongDuLieu;
    istringstream luongDuLieu(SqlServer::instance().doc("Mon"));
    while (getline(luongDuLieu, dongDuLieu)) {
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() < 7) {
                continue;
            }
            if (cacTruong[0] == "MONAN") {
                dsMon.push_back(make_unique<MonAn>(cacTruong[1], cacTruong[2], stod(cacTruong[3]),
                                                   stoi(cacTruong[4]), stoi(cacTruong[5]),
                                                   stoi(cacTruong[6])));
            } else if (cacTruong[0] == "NUOC") {
                dsMon.push_back(make_unique<NuocUong>(cacTruong[1], cacTruong[2],
                                                      stod(cacTruong[3]), stoi(cacTruong[4]),
                                                      cacTruong[5], stoi(cacTruong[6])));
            }
        } catch (...) {
            // Giu cach xu ly cu: bo qua dong khong chuyen duoc sang doi tuong.
        }
    }
}

void QuanLy::docKhachHang() {
    dsKhachHang.clear();
    string dongDuLieu;
    istringstream luongDuLieu(SqlServer::instance().doc("KhachHang"));
    while (getline(luongDuLieu, dongDuLieu)) {
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() >= 5) {
                dsKhachHang.push_back(KhachHang(cacTruong[0], cacTruong[1], cacTruong[2],
                                                cacTruong[3], stoi(cacTruong[4])));
            }
        } catch (...) {
            // Giu cach xu ly cu: bo qua dong khong chuyen duoc sang doi tuong.
        }
    }
}

void QuanLy::docNhanVien() {
    dsNhanVien.clear();
    string dongDuLieu;
    istringstream luongDuLieu(SqlServer::instance().doc("NhanVien"));
    while (getline(luongDuLieu, dongDuLieu)) {
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() >= 7) {
                dsNhanVien.push_back(NhanVien(cacTruong[0], cacTruong[1], cacTruong[2],
                                              cacTruong[3], cacTruong[4], stod(cacTruong[5]),
                                              stod(cacTruong[6])));
            }
        } catch (...) {
            // Giu cach xu ly cu: bo qua dong khong chuyen duoc sang doi tuong.
        }
    }
}

void QuanLy::docHoaDon() {
    dsHoaDon.clear();
    string dongDuLieu;
    istringstream luongDuLieu(SqlServer::instance().doc("HoaDon"));
    while (getline(luongDuLieu, dongDuLieu)) {
        try {
            HoaDon hoaDon = HoaDon::fromDuLieu(dongDuLieu);
            if (!hoaDon.getMa().empty()) {
                dsHoaDon.push_back(hoaDon);
            }
        } catch (...) {
            // Giu cach xu ly cu: bo qua dong khong chuyen duoc sang doi tuong.
        }
    }
}
