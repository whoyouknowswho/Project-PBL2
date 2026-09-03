#include "QuanLy.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>

#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"

namespace {
const std::string XANH = "\033[32m", DO = "\033[31m", LAM = "\033[36m", VANG = "\033[33m",
                  RESET = "\033[0m";
std::vector<std::string> tach(const std::string& s, char d = '|') {
    std::vector<std::string> v;
    std::stringstream ss(s);
    std::string x;
    while (std::getline(ss, x, d))
        v.push_back(x);
    return v;
}
std::string nhapChuoi(const std::string& tb, bool choRong = false) {
    while (true) {
        std::cout << tb;
        std::string s;
        std::getline(std::cin, s);
        if (choRong || !s.empty())
            return s;
        std::cout << DO << "Khong duoc de trong!\n" << RESET;
    }
}
int nhapInt(const std::string& tb, int min = 0, int max = 1000000000) {
    while (true) {
        std::cout << tb;
        int x;
        if (std::cin >> x && x >= min && x <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << DO << "Vui long nhap so hop le!\n" << RESET;
    }
}
double nhapDouble(const std::string& tb, double min = 0) {
    while (true) {
        std::cout << tb;
        double x;
        if (std::cin >> x && x >= min) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << DO << "Vui long nhap so hop le!\n" << RESET;
    }
}
bool xacNhan(const std::string& tb) {
    std::string s = nhapChuoi(tb + " (y/n): ");
    return s == "y" || s == "Y";
}
void dungManHinh() {
    std::cout << VANG << "\nNhan Enter de quay lai menu..." << RESET;
    std::cin.get();
}
// ANSI: xoa toan bo noi dung cu va dua con tro ve goc tren ben trai.
void xoaManHinh() {
    std::cout << "\033[2J\033[H" << std::flush;
}
void tieuDe(const std::string& s) {
    xoaManHinh();
    std::cout << LAM << "\n+==================================================================+\n| "
              << std::left << std::setw(64) << s
              << " |\n+==================================================================+\n"
              << RESET;
}
std::string chuThuong(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}
// Cat chuoi neu qua dai, sau do can le khi chua chen ma mau ANSI.
std::string canTrai(std::string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong) s = s.substr(0, doRong - 3) + "...";
    return s + std::string(doRong - static_cast<int>(s.size()), ' ');
}
std::string canPhai(std::string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong) s = s.substr(0, doRong);
    return std::string(doRong - static_cast<int>(s.size()), ' ') + s;
}
std::string canGiua(std::string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong) s = s.substr(0, doRong);
    int trai = (doRong - static_cast<int>(s.size())) / 2;
    return std::string(trai, ' ') + s +
           std::string(doRong - trai - static_cast<int>(s.size()), ' ');
}
}  // namespace

KhachHang* QuanLy::timKhach(const std::string& m) {
    for (auto& x : dsKhachHang)
        if (x.getMa() == m)
            return &x;
    return nullptr;
}
NhanVien* QuanLy::timNhanVien(const std::string& m) {
    for (auto& x : dsNhanVien)
        if (x.getMa() == m)
            return &x;
    return nullptr;
}
Mon* QuanLy::timMon(const std::string& m) {
    for (auto& x : dsMon)
        if (x->getMa() == m)
            return x.get();
    return nullptr;
}

void QuanLy::docDuLieu() {
    dsKhachHang.clear();
    dsNhanVien.clear();
    dsMon.clear();
    dsHoaDon.clear();
    std::string line;
    std::ifstream f("DuLieu/menu.txt");
    while (std::getline(f, line))
        try {
            auto p = tach(line);
            if (p.size() < 7)
                continue;
            if (p[0] == "MONAN")
                dsMon.push_back(std::make_unique<MonAn>(p[1], p[2], std::stod(p[3]),
                                                        std::stoi(p[4]), std::stoi(p[5]),
                                                        std::stoi(p[6])));
            else if (p[0] == "NUOC")
                dsMon.push_back(std::make_unique<NuocUong>(p[1], p[2], std::stod(p[3]),
                                                           std::stoi(p[4]), p[5], std::stoi(p[6])));
        } catch (...) {
            std::cout << DO << "Bo qua mot dong loi trong menu.txt\n" << RESET;
        }
    f.close();
    f.open("DuLieu/khachhang.txt");
    while (std::getline(f, line))
        try {
            auto p = tach(line);
            if (p.size() >= 5)
                dsKhachHang.emplace_back(p[0], p[1], p[2], p[3], std::stoi(p[4]));
        } catch (...) {
        }
    f.close();
    f.open("DuLieu/nhanvien.txt");
    while (std::getline(f, line))
        try {
            auto p = tach(line);
            if (p.size() >= 7)
                dsNhanVien.emplace_back(p[0], p[1], p[2], p[3], p[4], std::stod(p[5]),
                                        std::stod(p[6]));
        } catch (...) {
        }
    f.close();
    f.open("DuLieu/hoadon.txt");
    while (std::getline(f, line))
        try {
            HoaDon h = HoaDon::fromFile(line);
            if (!h.getMa().empty())
                dsHoaDon.push_back(h);
        } catch (...) {
        }
}
void QuanLy::ghiDuLieu() const {
    // Dinh dang: cac truong cach nhau boi |; chi tiet hoa don cach nhau boi ; va ,
    std::ofstream f("DuLieu/menu.txt");
    for (const auto& x : dsMon)
        f << x->toFile() << '\n';
    f.close();
    f.open("DuLieu/khachhang.txt");
    for (const auto& x : dsKhachHang)
        f << x.toFile() << '\n';
    f.close();
    f.open("DuLieu/nhanvien.txt");
    for (const auto& x : dsNhanVien)
        f << x.toFile() << '\n';
    f.close();
    f.open("DuLieu/hoadon.txt");
    for (const auto& x : dsHoaDon)
        f << x.toFile() << '\n';
}

void QuanLy::hienThiKhachHang() const {
    tieuDe("DANH SACH KHACH HANG");
    std::cout << std::left << std::setw(10) << "Ma" << std::setw(24) << "Ho ten" << std::setw(15)
              << "SDT" << std::setw(12) << "Gioi tinh" << std::setw(10) << "Diem" << "Hang VIP\n"
              << std::string(82, '-') << '\n';
    for (const auto& x : dsKhachHang)
        x.hienThiThongTin();
    if (dsKhachHang.empty())
        std::cout << "(Danh sach trong)\n";
}
void QuanLy::hienThiNhanVien() const {
    tieuDe("DANH SACH NHAN VIEN");
    std::cout << std::left << std::setw(10) << "Ma" << std::setw(22) << "Ho ten" << std::setw(15)
              << "SDT" << std::setw(12) << "Gioi tinh" << std::setw(16) << "Chuc vu" << std::right
              << std::setw(12) << "Tong luong\n"
              << std::string(87, '-') << '\n';
    for (const auto& x : dsNhanVien)
        x.hienThiThongTin();
    if (dsNhanVien.empty())
        std::cout << "(Danh sach trong)\n";
}
void QuanLy::hienThiThucDon(int loc, int sapXep, bool chiConHang) const {
    std::vector<const Mon*> danhSach;
    for (const auto& mon : dsMon) {
        bool dungLoai = loc == 0 || (loc == 1 && mon->getLoai() == "Mon an") ||
                        (loc == 2 && mon->getLoai() == "Nuoc uong");
        if (dungLoai && (!chiConHang || mon->getTonKho() > 0))
            danhSach.push_back(mon.get());
    }
    if (sapXep == 1)
        std::sort(danhSach.begin(), danhSach.end(), [](const Mon* a, const Mon* b) {
            return chuThuong(a->getTen()) < chuThuong(b->getTen());
        });
    else if (sapXep == 2 || sapXep == 3)
        std::sort(danhSach.begin(), danhSach.end(), [sapXep](const Mon* a, const Mon* b) {
            return sapXep == 2 ? a->getGia() < b->getGia() : a->getGia() > b->getGia();
        });

    auto dinhDangGia = [](double gia) {
        std::string s = std::to_string(static_cast<long long>(gia));
        for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3) s.insert(i, ".");
        return s + " VND";
    };
    auto inBang = [&](const std::vector<const Mon*>& trang, const std::string& loai) {
        bool monAn = loai == "Mon an";
        int soLuong = static_cast<int>(std::count_if(danhSach.begin(), danhSach.end(),
                            [&](const Mon* x) { return x->getLoai() == loai; }));
        if (soLuong == 0) return;
        std::string tenBang = "DANH SACH THUC DON - " + std::string(monAn ? "MON AN" : "NUOC UONG");
        int doRongBang = 79;
        int leTrai = std::max(0, (doRongBang - static_cast<int>(tenBang.size())) / 2);
        std::cout << "\n\033[1;36m" << std::string(leTrai, ' ') << tenBang << "\033[0m\n";
        std::cout << "┌──────────┬────────────────────────┬──────────────┬────────┬─────────────────┐\n";
        std::cout << "\033[1;33m│ " << std::left << std::setw(8) << "Ma mon" << " │ "
                  << std::setw(22) << "Ten mon" << " │ " << std::right << std::setw(12) << "Gia"
                  << " │ " << std::setw(6) << "Ton" << " │ " << std::left << std::setw(15)
                  << "Trang thai" << " │\033[0m\n";
        std::cout << "├──────────┼────────────────────────┼──────────────┼────────┼─────────────────┤\n";
        for (const Mon* mon : trang) {
            if (mon->getLoai() != loai) continue;
            std::string trangThai = mon->getTonKho() == 0 ? "[HET HANG]" :
                                    mon->getTonKho() < 5 ? "[SAP HET]" : "Con hang";
            std::string mau = mon->getTonKho() == 0 ? "\033[2;31m" :
                              mon->getTonKho() < 5 ? "\033[33m" : "";
            std::cout << mau << "│ " << canTrai(mon->getMa(), 8) << " │ "
                      << canTrai(mon->getTen(), 22) << " │ "
                      << canPhai(dinhDangGia(mon->getGia()), 12) << " │ "
                      << canPhai(std::to_string(mon->getTonKho()), 6) << " │ "
                      << canTrai(trangThai, 15);
            std::cout << " │" << RESET << '\n';
        }
        std::cout << "└──────────┴────────────────────────┴──────────────┴────────┴─────────────────┘\n";
    };

    const int kichThuocTrang = 15;
    int tongTrang = std::max(1, static_cast<int>((danhSach.size() + kichThuocTrang - 1) / kichThuocTrang));
    for (int trangSo = 0; trangSo < tongTrang; ++trangSo) {
        tieuDe("THUC DON NHA HANG");
        int dau = trangSo * kichThuocTrang;
        int cuoi = std::min(static_cast<int>(danhSach.size()), dau + kichThuocTrang);
        std::vector<const Mon*> trang(danhSach.begin() + dau, danhSach.begin() + cuoi);
        inBang(trang, "Mon an");
        inBang(trang, "Nuoc uong");
        if (danhSach.empty()) std::cout << VANG << "\n(Thuc don khong co mon phu hop)\n" << RESET;
        if (tongTrang > 1 && trangSo + 1 < tongTrang) {
            std::cout << VANG << "Trang " << trangSo + 1 << '/' << tongTrang
                      << " - Nhan Enter xem tiep, Q de dung: " << RESET;
            std::string lenh; std::getline(std::cin, lenh);
            if (lenh == "q" || lenh == "Q") break;
        }
    }
}

void QuanLy::menuKhachHang() {
    while (true) {
        hienThiKhachHang();
        std::cout << "\n1.Them  2.Sua  3.Xoa  4.Tim kiem  0.Quay lai\n";
        int c = nhapInt("Lua chon: ", 0, 4);
        if (c == 0)
            return;
        if (c == 1) {
            std::string m = nhapChuoi("Ma KH: ");
            if (timKhach(m)) {
                std::cout << DO << "Ma da ton tai!\n" << RESET;
            } else {
                dsKhachHang.emplace_back(m, nhapChuoi("Ho ten: "), nhapChuoi("SDT: "),
                                         nhapChuoi("Gioi tinh: "), nhapInt("Diem ban dau: "));
                ghiDuLieu();
                std::cout << XANH << "Them thanh cong!\n" << RESET;
            }
        } else if (c == 2) {
            auto* x = timKhach(nhapChuoi("Ma can sua: "));
            if (!x)
                std::cout << DO << "Khong tim thay!\n" << RESET;
            else {
                x->capNhatThongTin(nhapChuoi("Ten moi: "), nhapChuoi("SDT moi: "),
                                   nhapChuoi("Gioi tinh moi: "));
                x->setDiemTichLuy(nhapInt("Diem moi: "));
                ghiDuLieu();
                std::cout << XANH << "Cap nhat thanh cong!\n" << RESET;
            }
        } else if (c == 3) {
            std::string m = nhapChuoi("Ma can xoa: ");
            auto it = std::find_if(dsKhachHang.begin(), dsKhachHang.end(),
                                   [&](const auto& x) { return x.getMa() == m; });
            if (it == dsKhachHang.end())
                std::cout << DO << "Khong tim thay!\n" << RESET;
            else if (xacNhan("Ban chac chan muon xoa")) {
                dsKhachHang.erase(it);
                ghiDuLieu();
                std::cout << XANH << "Da xoa!\n" << RESET;
            }
        } else {
            std::string k = chuThuong(nhapChuoi("Nhap ma/ten/SDT: "));
            bool co = false;
            for (const auto& x : dsKhachHang)
                if (chuThuong(x.getMa()).find(k) != std::string::npos ||
                    chuThuong(x.getTen()).find(k) != std::string::npos ||
                    x.getSoDienThoai().find(k) != std::string::npos) {
                    x.hienThiThongTin();
                    co = true;
                }
            if (!co)
                std::cout << DO << "Khong co ket qua.\n" << RESET;
        }
        dungManHinh();
    }
}

void QuanLy::menuNhanVien() {
    while (true) {
        hienThiNhanVien();
        std::cout << "\n1.Them  2.Sua  3.Xoa  4.Tim kiem  0.Quay lai\n";
        int c = nhapInt("Lua chon: ", 0, 4);
        if (c == 0)
            return;
        if (c == 1) {
            std::string m = nhapChuoi("Ma NV: ");
            if (timNhanVien(m))
                std::cout << DO << "Ma da ton tai!\n" << RESET;
            else {
                std::string t = nhapChuoi("Ho ten: "), s = nhapChuoi("SDT: "),
                            g = nhapChuoi("Gioi tinh: "), cv = nhapChuoi("Chuc vu: ");
                double l = nhapDouble("Luong co ban: "), pc = nhapDouble("Phu cap: ");
                dsNhanVien.emplace_back(m, t, s, g, cv, l, pc);
                ghiDuLieu();
                std::cout << XANH << "Them thanh cong!\n" << RESET;
            }
        } else if (c == 2) {
            auto* x = timNhanVien(nhapChuoi("Ma can sua: "));
            if (!x)
                std::cout << DO << "Khong tim thay!\n" << RESET;
            else {
                x->capNhatThongTin(nhapChuoi("Ten moi: "), nhapChuoi("SDT moi: "),
                                   nhapChuoi("Gioi tinh moi: "));
                x->capNhatCongViec(nhapChuoi("Chuc vu moi: "), nhapDouble("Luong moi: "),
                                   nhapDouble("Phu cap moi: "));
                ghiDuLieu();
                std::cout << XANH << "Cap nhat thanh cong!\n" << RESET;
            }
        } else if (c == 3) {
            std::string m = nhapChuoi("Ma can xoa: ");
            auto it = std::find_if(dsNhanVien.begin(), dsNhanVien.end(),
                                   [&](const auto& x) { return x.getMa() == m; });
            if (it == dsNhanVien.end())
                std::cout << DO << "Khong tim thay!\n" << RESET;
            else if (xacNhan("Ban chac chan muon xoa")) {
                dsNhanVien.erase(it);
                ghiDuLieu();
                std::cout << XANH << "Da xoa!\n" << RESET;
            }
        } else {
            std::string k = chuThuong(nhapChuoi("Nhap ma/ten/SDT: "));
            bool co = false;
            for (const auto& x : dsNhanVien)
                if (chuThuong(x.getMa()).find(k) != std::string::npos ||
                    chuThuong(x.getTen()).find(k) != std::string::npos ||
                    x.getSoDienThoai().find(k) != std::string::npos) {
                    x.hienThiThongTin();
                    co = true;
                }
            if (!co)
                std::cout << DO << "Khong co ket qua.\n" << RESET;
        }
        dungManHinh();
    }
}

void QuanLy::menuThucDon() {
    int boLoc = 0, sapXep = 0;
    bool chiConHang = false;
    while (true) {
        hienThiThucDon(boLoc, sapXep, chiConHang);
        std::cout << "\n1.Them  2.Sua  3.Xoa  4.Tim kiem  5.Nhap them kho"
                     "  6.Loc/Sap xep  0.Quay lai\n";
        int c = nhapInt("Lua chon: ", 0, 6);
        if (c == 0)
            return;
        if (c == 1) {
            int l = nhapInt("Loai (1-Mon an, 2-Nuoc uong): ", 1, 2);
            std::string m = nhapChuoi("Ma mon: ");
            if (timMon(m))
                std::cout << DO << "Ma da ton tai!\n" << RESET;
            else {
                std::string t = nhapChuoi("Ten mon: ");
                double g = nhapDouble("Gia: ");
                int ton = nhapInt("Ton kho: ");
                if (l == 1)
                    dsMon.push_back(std::make_unique<MonAn>(m, t, g, ton,
                                                            nhapInt("Do cay (0-5): ", 0, 5),
                                                            nhapInt("Mon chay (1/0): ", 0, 1)));
                else
                    dsMon.push_back(std::make_unique<NuocUong>(
                        m, t, g, ton, nhapChuoi("Size S/M/L: "), nhapInt("Co da (1/0): ", 0, 1)));
                ghiDuLieu();
                std::cout << XANH << "Them thanh cong!\n" << RESET;
            }
        } else if (c == 2) {
            Mon* x = timMon(nhapChuoi("Ma can sua: "));
            if (!x)
                std::cout << DO << "Khong tim thay!\n" << RESET;
            else {
                x->capNhatCoBan(nhapChuoi("Ten moi: "), nhapDouble("Gia moi: "),
                                nhapInt("Ton kho moi: "));
                x->capNhatChiTiet();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ghiDuLieu();
                std::cout << XANH << "Cap nhat thanh cong!\n" << RESET;
            }
        } else if (c == 3) {
            std::string m = nhapChuoi("Ma can xoa: ");
            auto it = std::find_if(dsMon.begin(), dsMon.end(),
                                   [&](const auto& x) { return x->getMa() == m; });
            if (it == dsMon.end())
                std::cout << DO << "Khong tim thay!\n" << RESET;
            else if (xacNhan("Ban chac chan muon xoa")) {
                dsMon.erase(it);
                ghiDuLieu();
                std::cout << XANH << "Da xoa!\n" << RESET;
            }
        } else if (c == 4) {
            std::string k = chuThuong(nhapChuoi("Nhap ma/ten mon: "));
            bool co = false;
            for (const auto& x : dsMon)
                if (chuThuong(x->getMa()).find(k) != std::string::npos ||
                    chuThuong(x->getTen()).find(k) != std::string::npos) {
                    std::cout << x->getMa() << " | " << x->getTen() << " | " << x->hienThiChiTiet()
                              << '\n';
                    co = true;
                }
            if (!co)
                std::cout << DO << "Khong co ket qua.\n" << RESET;
        } else if (c == 5) {
            Mon* x = timMon(nhapChuoi("Ma mon: "));
            if (!x)
                std::cout << DO << "Khong tim thay!\n" << RESET;
            else {
                x->congTonKho(nhapInt("So luong nhap them: ", 1));
                ghiDuLieu();
                std::cout << XANH << "Da cap nhat ton kho!\n" << RESET;
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
    std::string ma = nhapChuoi("Ma hoa don: ");
    if (std::any_of(dsHoaDon.begin(), dsHoaDon.end(),
                    [&](const auto& x) { return x.getMa() == ma; })) {
        std::cout << DO << "Ma hoa don da ton tai!\n" << RESET;
        dungManHinh();
        return;
    }
    std::string ngay = nhapChuoi("Ngay (dd/mm/yyyy): "),
                kh = nhapChuoi("Ma KH (Enter neu khach le): ", true),
                nv = nhapChuoi("Ma nhan vien: ");
    HoaDon h(ma, ngay, kh, nv);
    std::vector<std::pair<Mon*, int>> daTru;
    while (true) {
        hienThiThucDon();
        std::string m = nhapChuoi("Ma mon (0 de thanh toan): ");
        if (m == "0")
            break;
        Mon* x = timMon(m);
        if (!x) {
            std::cout << DO << "Khong tim thay mon!\n" << RESET;
            continue;
        }
        int sl = nhapInt("So luong: ", 1);
        if (!x->truTonKho(sl)) {
            std::cout << DO << "Khong du ton kho!\n" << RESET;
            continue;
        }
        // MonAn giu nguyen ten; NuocUong se hoi size va co da/khong da tai day.
        std::string tenKhiBan = x->taoTenKhiBan();
        h.themMon({x->getMa(), tenKhiBan, sl, x->getGia()});
        daTru.push_back({x, sl});
        std::cout << XANH << "Da them vao gio!\n" << RESET;
    }
    if (h.tamTinh() == 0) {
        for (auto& p : daTru)
            p.first->congTonKho(p.second);
        std::cout << DO << "Hoa don rong, da huy.\n" << RESET;
        dungManHinh();
        return;
    }
    KhachHang* k = timKhach(kh);
    if (k) {
        h.setGiamGia(k->getPhanTramGiam());
        k->congDiem(static_cast<int>(h.tongTien() / 10000));
    }
    dsHoaDon.push_back(h);
    ghiDuLieu();
    h.inHoaDon();
    dungManHinh();
}

void QuanLy::menuThongKe() const {
    xoaManHinh();
    std::cout << LAM
              << "┌────────────────────────────────────────────────────────────────────────────┐\n"
              << "│" << canGiua("CHON PHAM VI THONG KE", 76) << "│\n"
              << "└────────────────────────────────────────────────────────────────────────────┘\n"
              << RESET;
    std::string ngay = nhapChuoi("Nhap dd/mm/yyyy, mm/yyyy hoac Enter de xem tat ca: ", true);
    double dt = 0;
    int so = 0;
    std::map<std::string, std::pair<std::string, int>> ban;
    for (const auto& h : dsHoaDon)
        if (ngay.empty() || h.getNgayLap().find(ngay) != std::string::npos) {
            dt += h.tongTien();
            ++so;
            for (const auto& x : h.getGioHang()) {
                ban[x.maMon].first = x.tenMon;
                ban[x.maMon].second += x.soLuong;
            }
        }
    auto best = std::max_element(ban.begin(), ban.end(), [](const auto& a, const auto& b) {
        return a.second.second < b.second.second;
    });
    auto dinhDangTien = [](double tien) {
        std::string s = std::to_string(static_cast<long long>(tien));
        for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3) s.insert(i, ".");
        return s + " VND";
    };
    std::string monBanChay = best == ban.end()
        ? "Chua co du lieu"
        : best->second.first + " (" + std::to_string(best->second.second) + " phan)";
    std::string phamVi = ngay.empty() ? "Tat ca hoa don" : ngay;

    xoaManHinh();
    const int rongBaoCao = 76;
    std::cout << LAM
              << "┌────────────────────────────────────────────────────────────────────────────┐\n"
              << "│" << canGiua("THONG KE - BAO CAO KINH DOANH", rongBaoCao) << "│\n"
              << "├────────────────────────────────────────────────────────────────────────────┤\n"
              << RESET
              << "│ " << canTrai("Pham vi: " + phamVi, 74) << " │\n"
              << "├────────────────────────┬───────────────────────────────────────────────────┤\n"
              << VANG
              << "│ " << canTrai("CHI TIEU", 22) << " │ "
              << canTrai("KET QUA", 49) << " │\n"
              << RESET
              << "├────────────────────────┼───────────────────────────────────────────────────┤\n"
              << "│ " << canTrai("So hoa don", 22) << " │ "
              << canTrai(std::to_string(so) + " hoa don", 49) << " │\n"
              << "├────────────────────────┼───────────────────────────────────────────────────┤\n"
              << "│ " << canTrai("Tong doanh thu", 22) << " │ " << XANH
              << canTrai(dinhDangTien(dt), 49) << RESET << " │\n"
              << "├────────────────────────┼───────────────────────────────────────────────────┤\n"
              << "│ " << canTrai("Mon ban chay", 22) << " │ " << VANG
              << canTrai(monBanChay, 49) << RESET << " │\n"
              << "└────────────────────────┴───────────────────────────────────────────────────┘\n";

    std::cout << "\n" << VANG << "TON KHO THAP - CAN NHAP THEM (<= 5)" << RESET << "\n"
              << "┌──────────┬──────────────────────────────┬────────────┐\n"
              << VANG
              << "│" << canTrai(" Ma mon", 10) << "│" << canTrai(" Ten mon", 30)
              << "│" << canPhai("Con lai ", 12) << "│\n"
              << RESET
              << "├──────────┼──────────────────────────────┼────────────┤\n";
    bool co = false;
    for (const auto& x : dsMon) {
        if (x->getTonKho() <= 5) {
            std::string mau = x->getTonKho() == 0 ? DO : VANG;
            std::cout << mau << "│" << canTrai(" " + x->getMa(), 10)
                      << "│" << canTrai(" " + x->getTen(), 30)
                      << "│" << canPhai(std::to_string(x->getTonKho()) + " ", 12)
                      << "│" << RESET << '\n';
            co = true;
        }
    }
    if (!co)
        std::cout << "│" << canGiua("Khong co mon nao sap het hang", 54) << "│\n";
    std::cout << "└──────────┴──────────────────────────────┴────────────┘\n";
    dungManHinh();
}

void QuanLy::chay() {
    docDuLieu();
    while (true) {
        xoaManHinh();
        const int rongMenu = 66;
        std::cout << LAM
                  << "┌──────────────────────────────────────────────────────────────────┐\n"
                  << "│" << canGiua("HE THONG QUAN LY NHA HANG", rongMenu) << "│\n"
                  << "├──────────────────────────────────────────────────────────────────┤\n"
                  << RESET
                  << "│" << canTrai("", rongMenu) << "│\n";
        auto inMuc = [&](const std::string& so, const std::string& noiDung,
                         const std::string& mau) {
            std::cout << "│" << mau << canTrai("   " + so + "  " + noiDung, rongMenu)
                      << RESET << "│\n";
        };
        inMuc("1.", "Quan ly khach hang", VANG);
        inMuc("2.", "Quan ly nhan vien", VANG);
        inMuc("3.", "Quan ly thuc don", VANG);
        inMuc("4.", "Ban hang - Lap hoa don", VANG);
        inMuc("5.", "Thong ke va bao cao", VANG);
        inMuc("0.", "Luu du lieu va thoat", DO);
        std::cout << "│" << canTrai("", rongMenu) << "│\n"
                  << LAM
                  << "└──────────────────────────────────────────────────────────────────┘\n"
                  << RESET;
        int c = nhapInt("Lua chon cua ban: ", 0, 5);
        switch (c) {
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
            case 0:
                ghiDuLieu();
                std::cout << XANH << "Da luu du lieu. Hen gap lai!\n" << RESET;
                return;
        }
    }
}
