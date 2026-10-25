#include "GiaoDien.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


namespace GiaoDien {
const string XANH = "\033[32m", DO = "\033[31m", LAM = "\033[36m", VANG = "\033[33m",
                  RESET = "\033[0m";

void khoiTao() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

string nhapChuoi(const string& thongBao, bool choRong) {
    while (true) {
        cout << thongBao;
        string s;
        if (!getline(cin, s)) {
            throw runtime_error("Da dong luong nhap. Chuong trinh ket thuc.");
        }
        if (choRong || !s.empty())
            return s;
        cout << DO << "Khong duoc de trong!\n" << RESET;
    }
}

// Doc tung phim va hien dau * thay cho ky tu mat khau.
string nhapMatKhau(const string& thongBao) {
    cout << thongBao << flush;
#ifdef _WIN32
    DWORD cheDo = 0;
    if (GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &cheDo)) {
        wstring matKhauUnicode;
        int phim = 0;
        int soLanLap = 0;
        while (true) {
            if (soLanLap == 0) {
                INPUT_RECORD suKien;
                DWORD soSuKien = 0;
                // Doc su kien ban phim: khong in ky tu goc ra console.
                if (!ReadConsoleInputW(GetStdHandle(STD_INPUT_HANDLE), &suKien, 1, &soSuKien)) {
                    throw runtime_error("Khong doc duoc ban phim.");
                }
                if (soSuKien == 0 || suKien.EventType != KEY_EVENT ||
                    !suKien.Event.KeyEvent.bKeyDown) {
                    continue;
                }
                phim = suKien.Event.KeyEvent.uChar.UnicodeChar;
                if (phim == 0) { // Bo qua mui ten, F1, Delete,...
                    continue;
                }
                soLanLap = max(1, static_cast<int>(suKien.Event.KeyEvent.wRepeatCount));
            }
            soLanLap--;
            if (phim == 13) { // Enter
                break;
            }
            if (phim == 3 || phim == 26) {
                cout << '\n';
                throw runtime_error("Da huy nhap mat khau.");
            }
            if (phim == 8) { // Backspace
                if (!matKhauUnicode.empty()) {
                    wchar_t kyTuCuoi = matKhauUnicode.back();
                    matKhauUnicode.pop_back();
                    // Mot ky tu Unicode ngoai BMP gom hai ma UTF-16.
                    if (kyTuCuoi >= 0xDC00 && kyTuCuoi <= 0xDFFF &&
                        !matKhauUnicode.empty() &&
                        matKhauUnicode.back() >= 0xD800 && matKhauUnicode.back() <= 0xDBFF) {
                        matKhauUnicode.pop_back();
                    }
                    if (kyTuCuoi < 0xD800 || kyTuCuoi > 0xDBFF) {
                        cout << "\b \b" << flush;
                    }
                }
                continue;
            }
            if (phim >= 32) {
                matKhauUnicode += static_cast<wchar_t>(phim);
                if (phim < 0xD800 || phim > 0xDBFF) {
                    cout << '*' << flush;
                }
            }
        }
        cout << '\n';
        if (matKhauUnicode.empty()) {
            return "";
        }
        // Doi ve UTF-8 de khop voi mat khau da luu truoc day.
        int doDai = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
            matKhauUnicode.data(), static_cast<int>(matKhauUnicode.size()),
            NULL, 0, NULL, NULL);
        if (doDai == 0) {
            throw runtime_error("Mat khau chua ky tu Unicode khong hop le.");
        }
        string matKhau(doDai, '\0');
        WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
            matKhauUnicode.data(), static_cast<int>(matKhauUnicode.size()),
            &matKhau[0], doDai, NULL, NULL);
        return matKhau;
    }
#endif
    // Giu cach doc theo dong khi dau vao la tep/pipe (kiem tra tu dong).
    string matKhau;
    if (!getline(cin, matKhau)) {
        throw runtime_error("Da dong luong nhap. Chuong trinh ket thuc.");
    }
    cout << '\n';
    return matKhau;
}

int nhapInt(const string& thongBao, int min, int max) {
    while (true) {
        cout << thongBao;
        int x;
        if (cin >> x && x >= min && x <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        if (cin.eof()) {
            throw runtime_error("Da dong luong nhap. Chuong trinh ket thuc.");
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << DO << "Vui long nhap so hop le!\n" << RESET;
    }
}

double nhapDouble(const string& thongBao, double min) {
    while (true) {
        cout << thongBao;
        double x;
        if (cin >> x && x >= min) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        if (cin.eof()) {
            throw runtime_error("Da dong luong nhap. Chuong trinh ket thuc.");
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << DO << "Vui long nhap so hop le!\n" << RESET;
    }
}

bool xacNhan(const string& thongBao) {
    string s = nhapChuoi(thongBao + " (y/n): ");
    return s == "y" || s == "Y";
}

void dungManHinh() {
    cout << VANG << "\nNhan Enter de quay lai menu..." << RESET;
    cin.get();
}

void xoaManHinh() {
    cout << "\033[2J\033[H" << flush;
}

void tieuDe(const string& s) {
    xoaManHinh();
    cout << LAM << "\n+==================================================================+\n| "
              << left << setw(64) << s
              << " |\n+==================================================================+\n"
              << RESET;
}

string canTrai(string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong) {
        s = s.substr(0, doRong - 3) + "...";
    }
    return s + string(doRong - static_cast<int>(s.size()), ' ');
}

string canPhai(string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong) {
        s = s.substr(0, doRong);
    }
    return string(doRong - static_cast<int>(s.size()), ' ') + s;
}

string canGiua(string s, int doRong) {
    if (static_cast<int>(s.size()) > doRong) {
        s = s.substr(0, doRong);
    }
    int trai = (doRong - static_cast<int>(s.size())) / 2;
    return string(trai, ' ') + s +
           string(doRong - trai - static_cast<int>(s.size()), ' ');
}

void hienThiKhachHang(const vector<KhachHang>& dsKhachHang) {
    tieuDe("DANH SACH KHACH HANG");
    cout << left << setw(10) << "Ma" << setw(24) << "Ho ten" << setw(15)
              << "SDT" << setw(12) << "Gioi tinh" << setw(10) << "Diem" << "Hang VIP\n"
              << string(82, '-') << '\n';
    for (const KhachHang& x : dsKhachHang)
        x.hienThiThongTin();
    if (dsKhachHang.empty())
        cout << "(Danh sach trong)\n";
}

void hienThiNhanVien(const vector<NhanVien>& dsNhanVien) {
    tieuDe("DANH SACH NHAN VIEN");
    cout << left << setw(10) << "Ma" << setw(22) << "Ho ten" << setw(15)
              << "SDT" << setw(12) << "Gioi tinh" << setw(16) << "Chuc vu" << right
              << setw(12) << "Tong luong\n"
              << string(87, '-') << '\n';
    for (const NhanVien& x : dsNhanVien)
        x.hienThiThongTin();
    if (dsNhanVien.empty())
        cout << "(Danh sach trong)\n";
}

// Them dau cham ngan cach hang nghin, vi du: 45000 -> 45.000 VND.
string dinhDangTien(double tien) {
    string ketQua = to_string(static_cast<long long>(tien));
    for (int i = static_cast<int>(ketQua.size()) - 3; i > 0; i -= 3) {
        ketQua.insert(i, ".");
    }
    return ketQua + " VND";
}

void inBangThucDon(const vector<const Mon*>& danhSach,
                   const vector<const Mon*>& trang, const string& loai) {
    bool monAn = loai == "Mon an";
    int soLuong = 0;
    for (const Mon* mon : danhSach) {
        if (mon->getLoai() == loai) {
            soLuong++;
        }
    }
    if (soLuong == 0) {
        return;
    }
    string tenBang = "DANH SACH THUC DON - ";
    if (monAn) {
        tenBang += "MON AN";
    } else {
        tenBang += "NUOC UONG";
    }
    int doRongBang = 79;
    int leTrai = max(0, (doRongBang - static_cast<int>(tenBang.size())) / 2);
    cout << "\n\033[1;36m" << string(leTrai, ' ') << tenBang << "\033[0m\n";
    cout << "┌──────────┬────────────────────────┬──────────────┬────────┬─────────────────┐\n";
    cout << "\033[1;33m│ " << left << setw(8) << "Ma mon" << " │ "
              << setw(22) << "Ten mon" << " │ " << right << setw(12) << "Gia"
              << " │ " << setw(6) << "Ton" << " │ " << left << setw(15)
              << "Trang thai" << " │\033[0m\n";
    cout << "├──────────┼────────────────────────┼──────────────┼────────┼─────────────────┤\n";
    for (const Mon* mon : trang) {
        if (mon->getLoai() != loai) {
            continue;
        }
        string trangThai = "Con hang";
        string mau = "";
        if (mon->getTonKho() == 0) {
            trangThai = "[HET HANG]";
            mau = "\033[2;31m";
        } else if (mon->getTonKho() < 5) {
            trangThai = "[SAP HET]";
            mau = "\033[33m";
        }
        cout << mau << "│ " << canTrai(mon->getMa(), 8) << " │ "
                  << canTrai(mon->getTen(), 22) << " │ "
                  << canPhai(dinhDangTien(mon->getGia()), 12) << " │ "
                  << canPhai(to_string(mon->getTonKho()), 6) << " │ "
                  << canTrai(trangThai, 15);
        cout << " │" << RESET << '\n';
    }
    cout << "└──────────┴────────────────────────┴──────────────┴────────┴─────────────────┘\n";
    }

void hienThiThucDon(const vector<const Mon*>& danhSach) {

    const int kichThuocTrang = 15;
    int tongTrang = max(1, static_cast<int>((danhSach.size() + kichThuocTrang - 1) / kichThuocTrang));
    for (int trangSo = 0; trangSo < tongTrang; ++trangSo) {
    tieuDe("THUC DON NHA HANG");
    int dau = trangSo * kichThuocTrang;
    int cuoi = min(static_cast<int>(danhSach.size()), dau + kichThuocTrang);
    vector<const Mon*> trang(danhSach.begin() + dau, danhSach.begin() + cuoi);
    inBangThucDon(danhSach, trang, "Mon an");
    inBangThucDon(danhSach, trang, "Nuoc uong");
    if (danhSach.empty()) {
        cout << VANG << "\n(Thuc don khong co mon phu hop)\n" << RESET;
    }
    if (tongTrang > 1 && trangSo + 1 < tongTrang) {
        cout << VANG << "Trang " << trangSo + 1 << '/' << tongTrang
                  << " - Nhan Enter xem tiep, Q de dung: " << RESET;
        string lenh;
        getline(cin, lenh);
        if (lenh == "q" || lenh == "Q") {
            break;
        }
    }
    }
}

void inMucMenu(const string& so, const string& noiDung, const string& mau) {
    const int rongMenu = 66;
    cout << "│" << mau << canTrai("   " + so + "  " + noiDung, rongMenu)
         << RESET << "│\n";
}

void menuChinh(const string& tenDangNhap, bool laQuanLy) {
    xoaManHinh();
    const int rongMenu = 66;
    cout << LAM
              << "┌──────────────────────────────────────────────────────────────────┐\n"
              << "│" << canGiua("HE THONG QUAN LY NHA HANG", rongMenu) << "│\n"
              << "├──────────────────────────────────────────────────────────────────┤\n"
              << RESET
              << "│" << canTrai("", rongMenu) << "│\n";
    string vaiTro = "Nhan vien";
    if (laQuanLy) {
        vaiTro = "Quan ly";
    }
    inMucMenu("", tenDangNhap + " - " + vaiTro, XANH);
    inMucMenu("1.", "Quan ly khach hang", VANG);
    if (laQuanLy) {
        inMucMenu("2.", "Quan ly nhan vien", VANG);
        inMucMenu("3.", "Quan ly thuc don", VANG);
        inMucMenu("4.", "Ban hang - Lap hoa don", VANG);
        inMucMenu("5.", "Thong ke va bao cao", VANG);
        inMucMenu("6.", "Tao tai khoan", VANG);
        inMucMenu("7.", "Doi mat khau", VANG);
        inMucMenu("8.", "Dang xuat", VANG);
    } else {
        inMucMenu("2.", "Xem thuc don", VANG);
        inMucMenu("3.", "Ban hang - Lap hoa don", VANG);
        inMucMenu("4.", "Doi mat khau", VANG);
        inMucMenu("5.", "Dang xuat", VANG);
    }
    inMucMenu("0.", "Luu du lieu va thoat", DO);
    cout << "│" << canTrai("", rongMenu) << "│\n"
              << LAM
              << "└──────────────────────────────────────────────────────────────────┘\n"
              << RESET;
}

void tieuDeThongKe() {
    xoaManHinh();
    cout << LAM
              << "┌────────────────────────────────────────────────────────────────────────────┐\n"
              << "│" << canGiua("CHON PHAM VI THONG KE", 76) << "│\n"
              << "└────────────────────────────────────────────────────────────────────────────┘\n"
              << RESET;
}

void hienThiThongKe(const string& phamVi, int so, double dt,
                      const string& monBanChay,
                      const vector<unique_ptr<Mon>>& dsMon) {
    xoaManHinh();
    const int rongBaoCao = 76;
    cout << LAM
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
              << canTrai(to_string(so) + " hoa don", 49) << " │\n"
              << "├────────────────────────┼───────────────────────────────────────────────────┤\n"
              << "│ " << canTrai("Tong doanh thu", 22) << " │ " << XANH
              << canTrai(dinhDangTien(dt), 49) << RESET << " │\n"
              << "├────────────────────────┼───────────────────────────────────────────────────┤\n"
              << "│ " << canTrai("Mon ban chay", 22) << " │ " << VANG
              << canTrai(monBanChay, 49) << RESET << " │\n"
              << "└────────────────────────┴───────────────────────────────────────────────────┘\n";

    cout << "\n" << VANG << "TON KHO THAP - CAN NHAP THEM (<= 5)" << RESET << "\n"
              << "┌──────────┬──────────────────────────────┬────────────┐\n"
              << VANG
              << "│" << canTrai(" Ma mon", 10) << "│" << canTrai(" Ten mon", 30)
              << "│" << canPhai("Con lai ", 12) << "│\n"
              << RESET
              << "├──────────┼──────────────────────────────┼────────────┤\n";
    bool co = false;
    for (const unique_ptr<Mon>& x : dsMon) {
        if (x->getTonKho() <= 5) {
            string mau = VANG;
            if (x->getTonKho() == 0) {
                mau = DO;
            }
            cout << mau << "│" << canTrai(" " + x->getMa(), 10)
                      << "│" << canTrai(" " + x->getTen(), 30)
                      << "│" << canPhai(to_string(x->getTonKho()) + " ", 12)
                      << "│" << RESET << '\n';
            co = true;
        }
    }
    if (!co)
        cout << "│" << canGiua("Khong co mon nao sap het hang", 54) << "│\n";
    cout << "└──────────┴──────────────────────────────┴────────────┘\n";
}
}  // namespace GiaoDien
