#include "QuanLy.h"

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
