#include "TaiKhoan.h"
#include "../Module5_LuuTru/LuuTruTxt.h"
#include <stdexcept>

using namespace std;

namespace {
bool tenHopLe(const string& ten) {
    if (ten.size() < 4 || ten.size() > 32) return false;
    for (char c : ten) {
        bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                  (c >= '0' && c <= '9') || c == '_' || c == '-';
        if (!ok) return false;
    }
    return true;
}

void kiemTraMatKhauMoi(const string& matKhau) {
    if (matKhau.size() < 6 || matKhau.size() > 128)
        throw runtime_error("Mật khẩu cần từ 6 đến 128 ký tự.");
    if (matKhau.find_first_of(" \t\r\n") != string::npos)
        throw runtime_error("Mật khẩu không được chứa khoảng trắng.");
}
}  // namespace

bool QuanLyTaiKhoan::docDuLieu() {
    danhSach.clear();
    for (const auto& cot : LuuTru::doc(LuuTru::TaiKhoan)) {
        if (cot.size() >= 4 && !cot[0].empty())
            danhSach.push_back({cot[0], cot[1], cot[2], cot[3]});
    }
    return !danhSach.empty();
}

const TaiKhoan* QuanLyTaiKhoan::tim(const string& tenDangNhap) const {
    for (const TaiKhoan& tk : danhSach)
        if (tk.tenDangNhap == tenDangNhap) return &tk;
    return nullptr;
}

bool QuanLyTaiKhoan::kiemTraMatKhau(const string& tenDangNhap, const string& matKhau) const {
    const TaiKhoan* tk = tim(tenDangNhap);
    return tk != nullptr && tk->matKhau == matKhau;
}

vector<vector<string>> QuanLyTaiKhoan::cacDongTaiKhoan(const vector<TaiKhoan>& duLieu) const {
    vector<vector<string>> noiDung;
    for (const TaiKhoan& tk : duLieu)
        noiDung.push_back({tk.tenDangNhap, tk.matKhau, tk.vaiTro, tk.maLienKet});
    return noiDung;
}

void QuanLyTaiKhoan::luu(const vector<TaiKhoan>& duLieu) const {
    LuuTru::luu({{LuuTru::TaiKhoan, cacDongTaiKhoan(duLieu)}});
}

TaiKhoan QuanLyTaiKhoan::chuanBi(const string& tenDangNhap, const string& matKhau,
                                 const string& vaiTro, const string& maLienKet) const {
    if (!tenHopLe(tenDangNhap))
        throw runtime_error("Tên đăng nhập: 4-32 ký tự, chỉ gồm chữ không dấu, chữ số, _ hoặc -.");
    if (tim(tenDangNhap) != nullptr)
        throw runtime_error("Tên đăng nhập đã tồn tại.");
    if (vaiTro != "QUANLY" && vaiTro != "NHANVIEN" && vaiTro != "KHACHHANG")
        throw runtime_error("Vai trò không hợp lệ.");
    if (vaiTro == "KHACHHANG" && !maLienKet.empty()) {
        for (const TaiKhoan& cu : danhSach)
            if (cu.vaiTro == "KHACHHANG" && cu.maLienKet == maLienKet)
                throw runtime_error("Khách hàng này đã có tài khoản.");
    }
    kiemTraMatKhauMoi(matKhau);
    return {tenDangNhap, matKhau, vaiTro, maLienKet};
}

void QuanLyTaiKhoan::tao(const string& tenDangNhap, const string& matKhau,
                         const string& vaiTro, const string& maLienKet) {
    TaiKhoan tk = chuanBi(tenDangNhap, matKhau, vaiTro, maLienKet);
    vector<TaiKhoan> banMoi = danhSach;
    banMoi.push_back(tk);
    luu(banMoi);
    danhSach = move(banMoi);
}

void QuanLyTaiKhoan::doiMatKhau(const string& tenDangNhap, const string& matKhauMoi) {
    kiemTraMatKhauMoi(matKhauMoi);
    vector<TaiKhoan> banMoi = danhSach;
    for (TaiKhoan& tk : banMoi) {
        if (tk.tenDangNhap == tenDangNhap) {
            tk.matKhau = matKhauMoi;
            luu(banMoi);
            danhSach = move(banMoi);
            return;
        }
    }
    throw runtime_error("Không tìm thấy tài khoản.");
}
