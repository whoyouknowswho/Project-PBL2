#include "QuanLy.h"
#include "../Module5_GiaoDien/GiaoDien.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <utility>

#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"

using namespace std;


using namespace GiaoDien;

namespace {
vector<string> tach(const string& chuoi, char dauPhanCach = '|') {
    vector<string> cacTruong;
    stringstream luongChuoi(chuoi);
    string truong;
    while (getline(luongChuoi, truong, dauPhanCach))
        cacTruong.push_back(truong);
    return cacTruong;
}

string chuThuong(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));
    }
    return s;
}

// Cac ham so sanh dung de sap xep thuc don.
bool soSanhTen(const Mon* monThuNhat, const Mon* monThuHai) {
    return chuThuong(monThuNhat->getTen()) < chuThuong(monThuHai->getTen());
}

bool soSanhGiaTang(const Mon* monThuNhat, const Mon* monThuHai) {
    return monThuNhat->getGia() < monThuHai->getGia();
}

bool soSanhGiaGiam(const Mon* monThuNhat, const Mon* monThuHai) {
    return monThuNhat->getGia() > monThuHai->getGia();
}

struct ThongKeMon {
    string tenMon;
    int soLuong = 0;
};

struct MonDaTruKho {
    Mon* mon;
    int soLuong;
};

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
                dsMon.push_back(make_unique<NuocUong>(cacTruong[1], cacTruong[2], stod(cacTruong[3]),
                                                           stoi(cacTruong[4]), cacTruong[5], stoi(cacTruong[6])));
        } catch (...) {
            cout << DO << "Bo qua mot dong loi trong menu.txt\n" << RESET;
        }
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/khachhang.txt");
    while (getline(tepDuLieu, dongDuLieu))
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() >= 5)
                dsKhachHang.push_back(KhachHang(cacTruong[0], cacTruong[1], cacTruong[2], cacTruong[3], stoi(cacTruong[4])));
        } catch (...) {
        }
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/nhanvien.txt");
    while (getline(tepDuLieu, dongDuLieu))
        try {
            vector<string> cacTruong = tach(dongDuLieu);
            if (cacTruong.size() >= 7)
                dsNhanVien.push_back(NhanVien(cacTruong[0], cacTruong[1], cacTruong[2], cacTruong[3], cacTruong[4], stod(cacTruong[5]),
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

void QuanLy::ghiDuLieu() const {
    // Dinh dang: cac truong cach nhau boi |; chi tiet hoa don cach nhau boi ; va ,
    ofstream tepDuLieu("DuLieu/menu.txt");
    for (const unique_ptr<Mon>& x : dsMon)
        tepDuLieu << x->toFile() << '\n';
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/khachhang.txt");
    for (const KhachHang& x : dsKhachHang)
        tepDuLieu << x.toFile() << '\n';
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/nhanvien.txt");
    for (const NhanVien& x : dsNhanVien)
        tepDuLieu << x.toFile() << '\n';
    tepDuLieu.close();
    tepDuLieu.open("DuLieu/hoadon.txt");
    for (const HoaDon& x : dsHoaDon)
        tepDuLieu << x.toFile() << '\n';
}

void QuanLy::hienThiKhachHang() const {
    GiaoDien::hienThiKhachHang(dsKhachHang);
}

void QuanLy::hienThiNhanVien() const {
    GiaoDien::hienThiNhanVien(dsNhanVien);
}

void QuanLy::hienThiThucDon(int loc, int sapXep, bool chiConHang) const {
    vector<const Mon*> danhSach;
    for (const unique_ptr<Mon>& mon : dsMon) {
        bool dungLoai = loc == 0 || (loc == 1 && mon->getLoai() == "Mon an") ||
                        (loc == 2 && mon->getLoai() == "Nuoc uong");
        if (dungLoai && (!chiConHang || mon->getTonKho() > 0))
            danhSach.push_back(mon.get());
    }
    if (sapXep == 1) {
        sort(danhSach.begin(), danhSach.end(), soSanhTen);
    } else if (sapXep == 2) {
        sort(danhSach.begin(), danhSach.end(), soSanhGiaTang);
    } else if (sapXep == 3) {
        sort(danhSach.begin(), danhSach.end(), soSanhGiaGiam);
    }

    GiaoDien::hienThiThucDon(danhSach);
}

void QuanLy::menuKhachHang() {
    while (true) {
        hienThiKhachHang();
        cout << "\n1.Them  2.Sua  3.Xoa  4.Tim kiem  0.Quay lai\n";
        int luaChon = nhapInt("Lua chon: ", 0, 4);
        if (luaChon == 0)
            return;
        if (luaChon == 1) {
            string maKhach = nhapChuoi("Ma KH: ");
            if (timKhach(maKhach)) {
                cout << DO << "Ma da ton tai!\n" << RESET;
            } else {
                string hoTen = nhapChuoi("Ho ten: ");
                string soDienThoai = nhapChuoi("SDT: ");
                string gioiTinh = nhapChuoi("Gioi tinh: ");
                int diemBanDau = nhapInt("Diem ban dau: ");
                KhachHang khachMoi(maKhach, hoTen, soDienThoai, gioiTinh, diemBanDau);
                dsKhachHang.push_back(khachMoi);
                ghiDuLieu();
                cout << XANH << "Them thanh cong!\n" << RESET;
            }
        } else if (luaChon == 2) {
            KhachHang* khachHang = timKhach(nhapChuoi("Ma can sua: "));
            if (!khachHang)
                cout << DO << "Khong tim thay!\n" << RESET;
            else {
                string tenMoi = nhapChuoi("Ten moi: ");
                string soDienThoaiMoi = nhapChuoi("SDT moi: ");
                string gioiTinhMoi = nhapChuoi("Gioi tinh moi: ");
                khachHang->capNhatThongTin(tenMoi, soDienThoaiMoi, gioiTinhMoi);
                khachHang->setDiemTichLuy(nhapInt("Diem moi: "));
                ghiDuLieu();
                cout << XANH << "Cap nhat thanh cong!\n" << RESET;
            }
        } else if (luaChon == 3) {
            string maKhach = nhapChuoi("Ma can xoa: ");
            // Tim vi tri can xoa; size() nghia la chua tim thay.
            size_t viTri = dsKhachHang.size();
            for (size_t i = 0; i < dsKhachHang.size(); i++) {
                if (dsKhachHang[i].getMa() == maKhach) {
                    viTri = i;
                    break;
                }
            }
            if (viTri == dsKhachHang.size())
                cout << DO << "Khong tim thay!\n" << RESET;
            else if (xacNhan("Ban chac chan muon xoa")) {
                dsKhachHang.erase(dsKhachHang.begin() + viTri);
                ghiDuLieu();
                cout << XANH << "Da xoa!\n" << RESET;
            }
        } else {
            string tuKhoa = chuThuong(nhapChuoi("Nhap ma/ten/SDT: "));
            bool timThay = false;
            for (const KhachHang& khachHang : dsKhachHang)
                if (chuThuong(khachHang.getMa()).find(tuKhoa) != string::npos ||
                    chuThuong(khachHang.getTen()).find(tuKhoa) != string::npos ||
                    khachHang.getSoDienThoai().find(tuKhoa) != string::npos) {
                    khachHang.hienThiThongTin();
                    timThay = true;
                }
            if (!timThay)
                cout << DO << "Khong co ket qua.\n" << RESET;
        }
        dungManHinh();
    }
}

void QuanLy::menuNhanVien() {
    if (!laQuanLy()) {
        cout << DO << "Ban khong co quyen su dung chuc nang nay.\n" << RESET;
        dungManHinh();
        return;
    }

    while (true) {
        hienThiNhanVien();
        cout << "\n1.Them  2.Sua  3.Xoa  4.Tim kiem  0.Quay lai\n";
        int luaChon = nhapInt("Lua chon: ", 0, 4);
        if (luaChon == 0)
            return;
        if (luaChon == 1) {
            string maNhanVien = nhapChuoi("Ma NV: ");
            if (timNhanVien(maNhanVien))
                cout << DO << "Ma da ton tai!\n" << RESET;
            else {
                string hoTen = nhapChuoi("Ho ten: ");
                string dienThoai = nhapChuoi("SDT: ");
                string gioiTinh = nhapChuoi("Gioi tinh: ");
                string chucVu = nhapChuoi("Chuc vu: ");
                double luong = nhapDouble("Luong co ban: ");
                double phuCap = nhapDouble("Phu cap: ");
                dsNhanVien.push_back(NhanVien(maNhanVien, hoTen, dienThoai, gioiTinh, chucVu, luong, phuCap));
                ghiDuLieu();
                cout << XANH << "Them thanh cong!\n" << RESET;
            }
        } else if (luaChon == 2) {
            NhanVien* nhanVien = timNhanVien(nhapChuoi("Ma can sua: "));
            if (!nhanVien)
                cout << DO << "Khong tim thay!\n" << RESET;
            else {
                string tenMoi = nhapChuoi("Ten moi: ");
                string soDienThoaiMoi = nhapChuoi("SDT moi: ");
                string gioiTinhMoi = nhapChuoi("Gioi tinh moi: ");
                nhanVien->capNhatThongTin(tenMoi, soDienThoaiMoi, gioiTinhMoi);
                string chucVuMoi = nhapChuoi("Chuc vu moi: ");
                double luongMoi = nhapDouble("Luong moi: ");
                double phuCapMoi = nhapDouble("Phu cap moi: ");
                nhanVien->capNhatCongViec(chucVuMoi, luongMoi, phuCapMoi);
                ghiDuLieu();
                cout << XANH << "Cap nhat thanh cong!\n" << RESET;
            }
        } else if (luaChon == 3) {
            string maNhanVien = nhapChuoi("Ma can xoa: ");
            // Tim vi tri can xoa; size() nghia la chua tim thay.
            size_t viTri = dsNhanVien.size();
            for (size_t i = 0; i < dsNhanVien.size(); i++) {
                if (dsNhanVien[i].getMa() == maNhanVien) {
                    viTri = i;
                    break;
                }
            }
            if (viTri == dsNhanVien.size())
                cout << DO << "Khong tim thay!\n" << RESET;
            else if (xacNhan("Ban chac chan muon xoa")) {
                dsNhanVien.erase(dsNhanVien.begin() + viTri);
                ghiDuLieu();
                cout << XANH << "Da xoa!\n" << RESET;
            }
        } else {
            string tuKhoa = chuThuong(nhapChuoi("Nhap ma/ten/SDT: "));
            bool timThay = false;
            for (const NhanVien& nhanVien : dsNhanVien)
                if (chuThuong(nhanVien.getMa()).find(tuKhoa) != string::npos ||
                    chuThuong(nhanVien.getTen()).find(tuKhoa) != string::npos ||
                    nhanVien.getSoDienThoai().find(tuKhoa) != string::npos) {
                    nhanVien.hienThiThongTin();
                    timThay = true;
                }
            if (!timThay)
                cout << DO << "Khong co ket qua.\n" << RESET;
        }
        dungManHinh();
    }
}

void QuanLy::menuThucDon() {
    if (!laQuanLy()) {
        hienThiThucDon();
        dungManHinh();
        return;
    }

    int boLoc = 0, sapXep = 0;
    bool chiConHang = false;
    while (true) {
        hienThiThucDon(boLoc, sapXep, chiConHang);
        cout << "\n1.Them  2.Sua  3.Xoa  4.Tim kiem  5.Nhap them kho"
                     "  6.Loc/Sap xep  0.Quay lai\n";
        int luaChon = nhapInt("Lua chon: ", 0, 6);
        if (luaChon == 0)
            return;
        if (luaChon == 1) {
            int loaiMon = nhapInt("Loai (1-Mon an, 2-Nuoc uong): ", 1, 2);
            string maMon = nhapChuoi("Ma mon: ");
            if (timMon(maMon))
                cout << DO << "Ma da ton tai!\n" << RESET;
            else {
                string tenMon = nhapChuoi("Ten mon: ");
                double giaMon = nhapDouble("Gia: ");
                int tonKho = nhapInt("Ton kho: ");
                if (loaiMon == 1) {
                    int doCay = nhapInt("Do cay (0-5): ", 0, 5);
                    bool monChay = nhapInt("Mon chay (1/0): ", 0, 1) == 1;
                    dsMon.push_back(make_unique<MonAn>(maMon, tenMon, giaMon, tonKho, doCay, monChay));
                } else {
                    string sizeLy = nhapChuoi("Size S/M/L: ");
                    bool coDa = nhapInt("Co da (1/0): ", 0, 1) == 1;
                    dsMon.push_back(make_unique<NuocUong>(maMon, tenMon, giaMon, tonKho, sizeLy, coDa));
                }
                ghiDuLieu();
                cout << XANH << "Them thanh cong!\n" << RESET;
            }
        } else if (luaChon == 2) {
            Mon* mon = timMon(nhapChuoi("Ma can sua: "));
            if (!mon)
                cout << DO << "Khong tim thay!\n" << RESET;
            else {
                string tenMoi = nhapChuoi("Ten moi: ");
                double giaMoi = nhapDouble("Gia moi: ");
                int tonKhoMoi = nhapInt("Ton kho moi: ");
                mon->capNhatCoBan(tenMoi, giaMoi, tonKhoMoi);
                mon->capNhatChiTiet();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                ghiDuLieu();
                cout << XANH << "Cap nhat thanh cong!\n" << RESET;
            }
        } else if (luaChon == 3) {
            string maMon = nhapChuoi("Ma can xoa: ");
            // Tim vi tri can xoa; size() nghia la chua tim thay.
            size_t viTri = dsMon.size();
            for (size_t i = 0; i < dsMon.size(); i++) {
                if (dsMon[i]->getMa() == maMon) {
                    viTri = i;
                    break;
                }
            }
            if (viTri == dsMon.size())
                cout << DO << "Khong tim thay!\n" << RESET;
            else if (xacNhan("Ban chac chan muon xoa")) {
                dsMon.erase(dsMon.begin() + viTri);
                ghiDuLieu();
                cout << XANH << "Da xoa!\n" << RESET;
            }
        } else if (luaChon == 4) {
            string tuKhoa = chuThuong(nhapChuoi("Nhap ma/ten mon: "));
            bool timThay = false;
            for (const unique_ptr<Mon>& mon : dsMon)
                if (chuThuong(mon->getMa()).find(tuKhoa) != string::npos ||
                    chuThuong(mon->getTen()).find(tuKhoa) != string::npos) {
                    cout << mon->getMa() << " | " << mon->getTen() << " | " << mon->hienThiChiTiet()
                              << '\n';
                    timThay = true;
                }
            if (!timThay)
                cout << DO << "Khong co ket qua.\n" << RESET;
        } else if (luaChon == 5) {
            Mon* mon = timMon(nhapChuoi("Ma mon: "));
            if (!mon)
                cout << DO << "Khong tim thay!\n" << RESET;
            else {
                mon->congTonKho(nhapInt("So luong nhap them: ", 1));
                ghiDuLieu();
                cout << XANH << "Da cap nhat ton kho!\n" << RESET;
            }
        } else {
            tieuDe("TUY CHON HIEN THI THUC DON");
            boLoc = nhapInt("Xem (0-Tat ca, 1-Mon an, 2-Nuoc uong): ", 0, 2);
            sapXep = nhapInt("Sap xep (0-Mac dinh, 1-Ten, 2-Gia tang, 3-Gia giam): ", 0, 3);
            chiConHang = nhapInt("Chi hien mon con hang (1-Co, 0-Khong): ", 0, 1) == 1;
        }
        dungManHinh();
    }
}

void QuanLy::banHang() {
    tieuDe("TAO HOA DON MOI");
    string maHoaDon = nhapChuoi("Ma hoa don: ");
    bool trungMa = false;
    for (const HoaDon& hoaDon : dsHoaDon) {
        if (hoaDon.getMa() == maHoaDon) {
            trungMa = true;
            break;
        }
    }
    if (trungMa) {
        cout << DO << "Ma hoa don da ton tai!\n" << RESET;
        dungManHinh();
        return;
    }
    string ngayLap = nhapChuoi("Ngay (dd/mm/yyyy): ");
    string maKhachHang = nhapChuoi("Ma KH (Enter neu khach le): ", true);
    string maNhanVien = taiKhoanHienTai.maNhanVien;
    if (laQuanLy()) {
        maNhanVien = nhapChuoi("Ma nhan vien: ");
    }
    if (timNhanVien(maNhanVien) == nullptr) {
        cout << DO << "Ma nhan vien khong ton tai.\n" << RESET;
        dungManHinh();
        return;
    }
    HoaDon hoaDonMoi(maHoaDon, ngayLap, maKhachHang, maNhanVien);
    vector<MonDaTruKho> daTru;
    while (true) {
        hienThiThucDon();
        string maMon = nhapChuoi("Ma mon (0 de thanh toan): ");
        if (maMon == "0")
            break;
        Mon* mon = timMon(maMon);
        if (!mon) {
            cout << DO << "Khong tim thay mon!\n" << RESET;
            continue;
        }
        int soLuong = nhapInt("So luong: ", 1);
        if (!mon->truTonKho(soLuong)) {
            cout << DO << "Khong du ton kho!\n" << RESET;
            continue;
        }
        // MonAn giu nguyen ten; NuocUong se hoi size va co da/khong da tai day.
        string tenKhiBan = mon->taoTenKhiBan();
        hoaDonMoi.themMon({mon->getMa(), tenKhiBan, soLuong, mon->getGia()});
        daTru.push_back({mon, soLuong});
        cout << XANH << "Da them vao gio!\n" << RESET;
    }
    if (hoaDonMoi.tamTinh() == 0) {
        for (const MonDaTruKho& daTruKho : daTru) {
            daTruKho.mon->congTonKho(daTruKho.soLuong);
        }
        cout << DO << "Hoa don rong, da huy.\n" << RESET;
        dungManHinh();
        return;
    }
    KhachHang* khachHang = timKhach(maKhachHang);
    if (khachHang) {
        hoaDonMoi.setGiamGia(khachHang->getPhanTramGiam());
        khachHang->congDiem(static_cast<int>(hoaDonMoi.tongTien() / 10000));
    }
    dsHoaDon.push_back(hoaDonMoi);
    ghiDuLieu();
    hoaDonMoi.inHoaDon();
    dungManHinh();
}

void QuanLy::menuThongKe() const {
    if (!laQuanLy()) {
        cout << DO << "Ban khong co quyen su dung chuc nang nay.\n" << RESET;
        dungManHinh();
        return;
    }

    GiaoDien::tieuDeThongKe();
    string ngayCanXem = nhapChuoi("Nhap dd/mm/yyyy, mm/yyyy hoac Enter de xem tat ca: ", true);
    double tongDoanhThu = 0;
    int soHoaDon = 0;
    map<string, ThongKeMon> thongKeTheoMa;
    for (const HoaDon& hoaDon : dsHoaDon)
        if (ngayCanXem.empty() || hoaDon.getNgayLap().find(ngayCanXem) != string::npos) {
            tongDoanhThu += hoaDon.tongTien();
            ++soHoaDon;
            for (const ChiTietHoaDon& chiTiet : hoaDon.getGioHang()) {
                thongKeTheoMa[chiTiet.maMon].tenMon = chiTiet.tenMon;
                thongKeTheoMa[chiTiet.maMon].soLuong += chiTiet.soLuong;
            }
        }
    string monBanChay = "Chua co du lieu";
    int soLuongLonNhat = 0;
    bool daTimThayMon = false;
    // map sap xep theo ma mon; giu mon dau tien neu bang so luong.
    for (const pair<const string, ThongKeMon>& muc : thongKeTheoMa) {
        const ThongKeMon& thongKe = muc.second;
        if (!daTimThayMon || thongKe.soLuong > soLuongLonNhat) {
            soLuongLonNhat = thongKe.soLuong;
            monBanChay = thongKe.tenMon + " (" + to_string(thongKe.soLuong) + " phan)";
            daTimThayMon = true;
        }
    }
    string phamVi = ngayCanXem;
    if (ngayCanXem.empty()) {
        phamVi = "Tat ca hoa don";
    }

    GiaoDien::hienThiThongKe(phamVi, soHoaDon, tongDoanhThu, monBanChay, dsMon);
    dungManHinh();
}

void QuanLy::chay() {
    docDuLieu();
    if (!quanLyTaiKhoan.docDuLieu()) {
        bool daTao = taoTaiKhoan(true);
        dungManHinh();
        if (!daTao) {
            return;
        }
    }
    if (!dangNhap()) {
        return;
    }
    while (true) {
        GiaoDien::menuChinh(taiKhoanHienTai.tenDangNhap, laQuanLy());
        int luaChonLonNhat = 5;
        if (laQuanLy()) {
            luaChonLonNhat = 8;
        }
        int luaChon = nhapInt("Lua chon cua ban: ", 0, luaChonLonNhat);
        // Menu nhan vien danh so 1-5; doi sang ma chuc nang dung chung.
        int chucNang = luaChon;
        if (!laQuanLy()) {
            switch (luaChon) {
                case 2:
                    chucNang = 3; // Xem thuc don
                    break;
                case 3:
                    chucNang = 4; // Ban hang
                    break;
                case 4:
                    chucNang = 7; // Doi mat khau
                    break;
                case 5:
                    chucNang = 8; // Dang xuat
                    break;
            }
        }
        // Kiem tra quyen tai noi xu ly, khong chi an muc menu.
        if (!laQuanLy() && (chucNang == 2 || chucNang == 5 || chucNang == 6)) {
            cout << DO << "Ban khong co quyen su dung chuc nang nay.\n" << RESET;
            dungManHinh();
            continue;
        }
        switch (chucNang) {
            case 1:
                menuKhachHang();
                break;
            case 2:
                menuNhanVien();
                break;
            case 3:
                menuThucDon();
                break;
            case 4:
                banHang();
                break;
            case 5:
                menuThongKe();
                break;
            case 6:
                taoTaiKhoan();
                dungManHinh();
                break;
            case 7:
                doiMatKhau();
                dungManHinh();
                break;
            case 8:
                ghiDuLieu();
                taiKhoanHienTai = TaiKhoan();
                if (!dangNhap()) {
                    return;
                }
                break;
            case 0:
                ghiDuLieu();
                cout << XANH << "Da luu du lieu. Hen gap lai!\n" << RESET;
                return;
        }
    }
}
