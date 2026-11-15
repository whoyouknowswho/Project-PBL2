#include "TaiKhoan.h"
#include "../Module5_SQL/KetNoiSqlServer.h"
#include <stdexcept>

using namespace std;

namespace {
bool tenHopLe(const string& ten) {
    if (ten.empty() || ten.size() > 32) return false;
    for (char c : ten) {
        bool chuCai = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        bool chuSo = (c >= '0' && c <= '9');
        if (!chuCai && !chuSo && c != '_' && c != '-') return false;
    }
    return true;
}

void kiemTraMatKhauMoi(const string& matKhau) {
    if (matKhau.empty() || matKhau.size() > 128)
        throw runtime_error("Mật khẩu không được để trống (tối đa 128 ký tự).");
}
}

bool QuanLyTaiKhoan::docDuLieu() {
    danhSach.clear();
    auto cacDong = Sql::doc(Sql::TaiKhoan);
    if (cacDong.empty()) return false;

    for (const auto& cot : cacDong) {
        if (cot.size() < 3 || cot[0].empty()) continue;
        danhSach.push_back({cot[0], cot[1], cot[2]});
    }
    return !danhSach.empty();
}

const TaiKhoan* QuanLyTaiKhoan::tim(const string& tenDangNhap) const {
    for (const TaiKhoan& tk : danhSach) {
        if (tk.tenDangNhap == tenDangNhap) return &tk;
    }
    return nullptr;
}

bool QuanLyTaiKhoan::kiemTraMatKhau(const string& tenDangNhap, const string& matKhau) const {
    const TaiKhoan* tk = tim(tenDangNhap);
    return tk != nullptr && tk->matKhau == matKhau;
}

vector<vector<string>> QuanLyTaiKhoan::cacDongTaiKhoan(const vector<TaiKhoan>& duLieu) const {
    vector<vector<string>> noiDung;
    for (const TaiKhoan& tk : duLieu) {
        noiDung.push_back({tk.tenDangNhap, tk.matKhau, tk.vaiTro});
    }
    return noiDung;
}

void QuanLyTaiKhoan::luu(const vector<TaiKhoan>& duLieu) const {
    Sql::luu({{Sql::TaiKhoan, cacDongTaiKhoan(duLieu)}});
}

TaiKhoan QuanLyTaiKhoan::chuanBi(const string& tenDangNhap, const string& matKhau,
                                 const string& vaiTro) const {
    if (!tenHopLe(tenDangNhap))
        throw runtime_error("Tên đăng nhập: 1-32 chữ cái, chữ số, _ hoặc -.");
    if (tim(tenDangNhap) != nullptr)
        throw runtime_error("Tên đăng nhập đã tồn tại.");
    if (vaiTro != "QUANLY" && vaiTro != "NHANVIEN" && vaiTro != "KHACHHANG")
        throw runtime_error("Vai trò không hợp lệ.");
    kiemTraMatKhauMoi(matKhau);
    return {tenDangNhap, matKhau, vaiTro};
}

void QuanLyTaiKhoan::tao(const string& tenDangNhap, const string& matKhau,
                         const string& vaiTro) {
    TaiKhoan tk = chuanBi(tenDangNhap, matKhau, vaiTro);
    vector<TaiKhoan> banMoi = danhSach;
    banMoi.push_back(tk);
    luu(banMoi);
    danhSach = banMoi;
}

void QuanLyTaiKhoan::doiMatKhau(const string& tenDangNhap, const string& matKhauMoi) {
    kiemTraMatKhauMoi(matKhauMoi);
    vector<TaiKhoan> banMoi = danhSach;
    for (TaiKhoan& tk : banMoi) {
        if (tk.tenDangNhap == tenDangNhap) {
            tk.matKhau = matKhauMoi;
            luu(banMoi);
            danhSach = banMoi;
            return;
        }
    }
    throw runtime_error("Không tìm thấy tài khoản.");
}
