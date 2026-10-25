#include "TaiKhoan.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <windows.h>
#include <bcrypt.h>

using namespace std;

namespace {
const char* TEP_TAI_KHOAN = "DuLieu/taikhoan.txt";
const unsigned long long SO_VONG_BAM = 600000;

// Phan ma hoa dung thu vien Windows, tach khoi logic dang nhap.
string sangHex(const vector<unsigned char>& duLieu) {
    const string kyTuHex = "0123456789abcdef";
    string ketQua;
    for (unsigned char kyTu : duLieu) {
        ketQua += kyTuHex[kyTu / 16];
        ketQua += kyTuHex[kyTu % 16];
    }
    return ketQua;
}

bool laHex(const string& chuoi, size_t doDai) {
    if (chuoi.size() != doDai) {
        return false;
    }
    for (char kyTu : chuoi) {
        if (string("0123456789abcdef").find(kyTu) == string::npos) {
            return false;
        }
    }
    return true;
}

string taoMuoi() {
    vector<unsigned char> duLieu(16);
    if (BCryptGenRandom(NULL, duLieu.data(), duLieu.size(),
                        BCRYPT_USE_SYSTEM_PREFERRED_RNG) < 0) {
        throw runtime_error("Khong tao duoc du lieu bao mat.");
    }
    return sangHex(duLieu);
}

string bamMatKhau(const string& matKhau, const string& muoi) {
    BCRYPT_ALG_HANDLE thuatToan = NULL;
    if (BCryptOpenAlgorithmProvider(&thuatToan, BCRYPT_SHA256_ALGORITHM, NULL,
                                    BCRYPT_ALG_HANDLE_HMAC_FLAG) < 0) {
        throw runtime_error("Khong khoi tao duoc thuat toan bam.");
    }
    vector<unsigned char> matKhauBytes(matKhau.begin(), matKhau.end());
    vector<unsigned char> muoiBytes(muoi.begin(), muoi.end());
    vector<unsigned char> ketQua(32);
    NTSTATUS trangThai = BCryptDeriveKeyPBKDF2(
        thuatToan, matKhauBytes.data(), matKhauBytes.size(),
        muoiBytes.data(), muoiBytes.size(), SO_VONG_BAM,
        ketQua.data(), ketQua.size(), 0);
    BCryptCloseAlgorithmProvider(thuatToan, 0);
    if (trangThai < 0) {
        throw runtime_error("Khong bam duoc mat khau.");
    }
    return sangHex(ketQua);
}

bool tenHopLe(const string& ten) {
    if (ten.empty() || ten.size() > 32) {
        return false;
    }
    for (char kyTu : ten) {
        bool chuCai = (kyTu >= 'a' && kyTu <= 'z') || (kyTu >= 'A' && kyTu <= 'Z');
        bool chuSo = kyTu >= '0' && kyTu <= '9';
        if (!chuCai && !chuSo && kyTu != '_' && kyTu != '-') {
            return false;
        }
    }
    return true;
}

void kiemTraMatKhauMoi(const string& matKhau) {
    if (matKhau.size() < 8 || matKhau.size() > 128) {
        throw runtime_error("Mat khau phai dai tu 8 den 128 ky tu.");
    }
}
}  // namespace

bool QuanLyTaiKhoan::docDuLieu() {
    danhSach.clear();
    DWORD thuocTinh = GetFileAttributesA(TEP_TAI_KHOAN);
    if (thuocTinh == INVALID_FILE_ATTRIBUTES) {
        DWORD loi = GetLastError();
        if (loi == ERROR_FILE_NOT_FOUND) {
            return false;
        }
        throw runtime_error("Khong truy cap duoc DuLieu/taikhoan.txt. Hay chay tu thu muc du an.");
    }
    ifstream tep(TEP_TAI_KHOAN);
    string dong;
    bool coQuanLy = false;
    while (getline(tep, dong)) {
        stringstream luong(dong);
        vector<string> truong;
        string giaTri;
        while (getline(luong, giaTri, '|')) {
            truong.push_back(giaTri);
        }
        if (truong.size() != 6 || truong[0] != "PBKDF2-SHA256-600000" ||
            !tenHopLe(truong[1]) || tim(truong[1]) != nullptr ||
            (truong[2] != "QUANLY" && truong[2] != "NHANVIEN") ||
            (truong[2] == "NHANVIEN" && !tenHopLe(truong[3])) ||
            !laHex(truong[4], 32) || !laHex(truong[5], 64) ||
            (!dong.empty() && dong.back() == '|')) {
            throw runtime_error("Tep tai khoan khong hop le. Khong tu dong tao lai tai khoan.");
        }
        TaiKhoan taiKhoan = {truong[1], truong[2], truong[3], truong[4], truong[5]};
        danhSach.push_back(taiKhoan);
        if (taiKhoan.vaiTro == "QUANLY") {
            coQuanLy = true;
        }
    }
    if (!tep.eof() || !coQuanLy) {
        throw runtime_error("Khong doc duoc tai khoan quan ly. Hay kiem tra tep tai khoan.");
    }
    return true;
}

const TaiKhoan* QuanLyTaiKhoan::tim(const string& tenDangNhap) const {
    for (const TaiKhoan& taiKhoan : danhSach) {
        if (taiKhoan.tenDangNhap == tenDangNhap) {
            return &taiKhoan;
        }
    }
    return nullptr;
}

bool QuanLyTaiKhoan::kiemTraMatKhau(const string& tenDangNhap, const string& matKhau) const {
    const TaiKhoan* taiKhoan = tim(tenDangNhap);
    if (taiKhoan == nullptr || matKhau.size() > 128) {
        return false;
    }
    string ketQua = bamMatKhau(matKhau, taiKhoan->muoi);
    unsigned int khacNhau = 0;
    for (size_t i = 0; i < ketQua.size(); i++) {
        khacNhau |= ketQua[i] ^ taiKhoan->matKhauBam[i];
    }
    return khacNhau == 0;
}

void QuanLyTaiKhoan::luu(const vector<TaiKhoan>& duLieu) const {
    string tepTam = string(TEP_TAI_KHOAN) + ".tmp";
    ofstream tep(tepTam, ios::trunc);
    for (const TaiKhoan& taiKhoan : duLieu) {
        tep << "PBKDF2-SHA256-600000|" << taiKhoan.tenDangNhap << '|'
            << taiKhoan.vaiTro << '|' << taiKhoan.maNhanVien << '|'
            << taiKhoan.muoi << '|' << taiKhoan.matKhauBam << '\n';
    }
    tep.close();
    if (!tep || !MoveFileExA(tepTam.c_str(), TEP_TAI_KHOAN,
                             MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        throw runtime_error("Khong luu duoc tai khoan. Thay doi chua duoc ap dung.");
    }
}

void QuanLyTaiKhoan::tao(const string& tenDangNhap, const string& matKhau,
                         const string& vaiTro, const string& maNhanVien) {
    if (!tenHopLe(tenDangNhap)) {
        throw runtime_error("Ten dang nhap: 1-32 chu cai, chu so, _ hoac -.");
    }
    if (tim(tenDangNhap) != nullptr) {
        throw runtime_error("Ten dang nhap da ton tai.");
    }
    if ((vaiTro != "QUANLY" && vaiTro != "NHANVIEN") ||
        (vaiTro == "NHANVIEN" && !tenHopLe(maNhanVien))) {
        throw runtime_error("Vai tro hoac ma nhan vien khong hop le.");
    }
    kiemTraMatKhauMoi(matKhau);
    TaiKhoan taiKhoan;
    taiKhoan.tenDangNhap = tenDangNhap;
    taiKhoan.vaiTro = vaiTro;
    taiKhoan.maNhanVien = maNhanVien;
    taiKhoan.muoi = taoMuoi();
    taiKhoan.matKhauBam = bamMatKhau(matKhau, taiKhoan.muoi);
    vector<TaiKhoan> banMoi = danhSach;
    banMoi.push_back(taiKhoan);
    luu(banMoi);
    danhSach = banMoi;
}

void QuanLyTaiKhoan::doiMatKhau(const string& tenDangNhap, const string& matKhauMoi) {
    kiemTraMatKhauMoi(matKhauMoi);
    vector<TaiKhoan> banMoi = danhSach;
    for (TaiKhoan& taiKhoan : banMoi) {
        if (taiKhoan.tenDangNhap == tenDangNhap) {
            taiKhoan.muoi = taoMuoi();
            taiKhoan.matKhauBam = bamMatKhau(matKhauMoi, taiKhoan.muoi);
            luu(banMoi);
            danhSach = banMoi;
            return;
        }
    }
    throw runtime_error("Khong tim thay tai khoan.");
}
