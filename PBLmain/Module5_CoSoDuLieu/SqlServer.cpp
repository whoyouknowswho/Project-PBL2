// Luong chay de doc code:
// 1. khoiTao(): doc cau hinh va ket noi SQL Server.
// 2. taiLai() -> docTatCaBang(): SELECT du lieu tu cac bang.
// 3. doc(): dua du lieu da doc cho cac module nghiep vu.
// 4. luu() -> luuCacThayDoi(): INSERT / UPDATE / DELETE cac dong thay doi.
// thucThiSQL() la ham chung gui lenh SQL va nhan ket qua qua ODBC.
// Dau ? trong cau SQL la vi tri gia tri duoc truyen rieng (tham so).
// Giao dich giup tranh luu nua chung khi mot cau lenh bi loi.

#include "SqlServer.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;
namespace {
struct CauTrucBang {
    string ten;
    vector<string> tenCot;
    int soCotKhoa;
};
const vector<CauTrucBang> CAC_BANG = {
    {"KhachHang", {"Ma", "HoTen", "DienThoai", "GioiTinh", "Diem", "Email"}, 1},
    {"NhanVien", {"Ma", "HoTen", "DienThoai", "GioiTinh", "ChucVu", "Luong", "PhuCap"}, 1},
    {"Mon", {"Ma", "Loai", "Ten", "Gia", "TonKho", "DoCay", "MonChay", "SizeLy", "CoDa"}, 1},
    {"TaiKhoan", {"TenDangNhap", "VaiTro", "MaLienKet", "Muoi", "MatKhauBam", "ThuatToan"}, 1},
    {"HoaDon", {"Ma", "NgayGoc", "MaKhachHang", "MaNhanVien", "GiamGia"}, 1},
    {"DonDatMon", {"Ma", "NgayGoc", "MaKhachHang", "GiamGia", "TrangThai", "MaHoaDon"}, 1},
    {"ChiTietHoaDon", {"MaHoaDon", "Dong", "MaMon", "TenMon", "SoLuong", "DonGia"}, 2},
    {"ChiTietDuTinh", {"MaDon", "Dong", "MaMon", "TenMon", "SoLuong", "DonGia"}, 2},
    {"ChiTietDonDat", {"MaDon", "Dong", "MaMon", "SoLuong", "SizeLy", "CoDa"}, 2}};
const vector<string> CAC_NHOM = {"Mon", "KhachHang", "NhanVien", "HoaDon", "DonDatMon", "TaiKhoan"};
wstring sangUnicode(const string& chuoi) {
    if (chuoi.empty()) {
        return {};
    }
    int soKyTu = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, chuoi.data(), (int)chuoi.size(),
                                     nullptr, 0);
    if (!soKyTu) {
        throw runtime_error("Du lieu khong phai UTF-8 hop le.");
    }
    wstring ketQua(soKyTu, 0);
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, chuoi.data(), (int)chuoi.size(), &ketQua[0],
                        soKyTu);
    return ketQua;
}
string sangUtf8(const wstring& chuoi) {
    if (chuoi.empty()) {
        return {};
    }
    int soByte = WideCharToMultiByte(CP_UTF8, 0, chuoi.data(), (int)chuoi.size(), nullptr, 0,
                                     nullptr, nullptr);
    string ketQua(soByte, 0);
    WideCharToMultiByte(CP_UTF8, 0, chuoi.data(), (int)chuoi.size(), &ketQua[0], soByte, nullptr,
                        nullptr);
    return ketQua;
}
void kiemTraLoiSQL(SQLRETURN ketQua, SQLSMALLINT loaiDoiTuong, SQLHANDLE doiTuong) {
    if (SQL_SUCCEEDED(ketQua)) {
        return;
    }
    SQLWCHAR maTrangThai[6], noiDungLoi[2048];
    SQLINTEGER maLoi;
    SQLSMALLINT doDai;
    string thongBao = "SQL Server: ";
    for (SQLSMALLINT i = 1; SQL_SUCCEEDED(SQLGetDiagRecW(loaiDoiTuong, doiTuong, i, maTrangThai,
                                                         &maLoi, noiDungLoi, 2048, &doDai));
         ++i) {
        thongBao += sangUtf8(wstring((wchar_t*)noiDungLoi, min<int>(doDai, 2047))) + " ";
    }
    throw runtime_error(thongBao);
}
struct CauLenhSQL {
    SQLHSTMT maCauLenh = SQL_NULL_HSTMT;
    explicit CauLenhSQL(SQLHDBC ketNoi) {
        kiemTraLoiSQL(SQLAllocHandle(SQL_HANDLE_STMT, ketNoi, &maCauLenh), SQL_HANDLE_DBC, ketNoi);
        SQLSetStmtAttr(maCauLenh, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, 0);
    }
    ~CauLenhSQL() {
        if (maCauLenh) {
            SQLFreeHandle(SQL_HANDLE_STMT, maCauLenh);
        }
    }
};
vector<DongDuLieu> thucThiSQL(SQLHDBC ketNoi, const string& sql, const DongDuLieu& thamSo = {}) {
    CauLenhSQL cauLenh(ketNoi);
    wstring sqlUnicode = sangUnicode(sql);
    if (!thamSo.empty()) {
        kiemTraLoiSQL(SQLPrepareW(cauLenh.maCauLenh, (SQLWCHAR*)sqlUnicode.c_str(), SQL_NTS),
                      SQL_HANDLE_STMT, cauLenh.maCauLenh);
    }
    vector<wstring> giaTriUnicode;
    giaTriUnicode.reserve(thamSo.size());
    vector<SQLLEN> doDaiThamSo(thamSo.size(), SQL_NTS);
    for (const string& giaTri : thamSo) {
        giaTriUnicode.push_back(sangUnicode(giaTri));
    }
    for (size_t i = 0; i < giaTriUnicode.size(); ++i) {
        kiemTraLoiSQL(
            SQLBindParameter(
                cauLenh.maCauLenh, (SQLUSMALLINT)i + 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
                max<size_t>(1, giaTriUnicode[i].size()), 0, (SQLPOINTER)giaTriUnicode[i].c_str(),
                (SQLLEN)((giaTriUnicode[i].size() + 1) * sizeof(wchar_t)), &doDaiThamSo[i]),
            SQL_HANDLE_STMT, cauLenh.maCauLenh);
    }
    if (thamSo.empty()) {
        kiemTraLoiSQL(SQLExecDirectW(cauLenh.maCauLenh, (SQLWCHAR*)sqlUnicode.c_str(), SQL_NTS),
                      SQL_HANDLE_STMT, cauLenh.maCauLenh);
    } else {
        kiemTraLoiSQL(SQLExecute(cauLenh.maCauLenh), SQL_HANDLE_STMT, cauLenh.maCauLenh);
    }
    vector<DongDuLieu> ketQua;
    do {
        SQLSMALLINT soCot = 0;
        kiemTraLoiSQL(SQLNumResultCols(cauLenh.maCauLenh, &soCot), SQL_HANDLE_STMT,
                      cauLenh.maCauLenh);
        if (soCot) {
            SQLRETURN trangThai;
            while ((trangThai = SQLFetch(cauLenh.maCauLenh)) != SQL_NO_DATA) {
                kiemTraLoiSQL(trangThai, SQL_HANDLE_STMT, cauLenh.maCauLenh);
                DongDuLieu dong;
                for (SQLUSMALLINT cot = 1; cot <= soCot; ++cot) {
                    SQLWCHAR boDem[4096] = {};
                    SQLLEN doDai = 0;
                    trangThai = SQLGetData(cauLenh.maCauLenh, cot, SQL_C_WCHAR, boDem,
                                           sizeof(boDem), &doDai);
                    kiemTraLoiSQL(trangThai, SQL_HANDLE_STMT, cauLenh.maCauLenh);
                    if (doDai == SQL_NO_TOTAL || doDai >= (SQLLEN)sizeof(boDem)) {
                        throw runtime_error("Cot SQL vuot gioi han doc.");
                    }
                    dong.push_back(doDai == SQL_NULL_DATA ? "" : sangUtf8((wchar_t*)boDem));
                }
                ketQua.push_back(move(dong));
            }
        }
        SQLRETURN ketQuaTiep = SQLMoreResults(cauLenh.maCauLenh);
        if (ketQuaTiep == SQL_NO_DATA) {
            break;
        }
        kiemTraLoiSQL(ketQuaTiep, SQL_HANDLE_STMT, cauLenh.maCauLenh);
    } while (true);
    return ketQua;
}
string ghepChuoi(const DongDuLieu& cacTruong, const string& dauPhanCach) {
    string ketQua;
    for (size_t i = 0; i < cacTruong.size(); ++i) {
        if (i) {
            ketQua += dauPhanCach;
        }
        ketQua += cacTruong[i];
    }
    return ketQua;
}
DongDuLieu tachChuoi(const string& chuoi, char dauPhanCach) {
    DongDuLieu cacTruong;
    size_t batDau = 0, viTri;
    while ((viTri = chuoi.find(dauPhanCach, batDau)) != string::npos) {
        cacTruong.push_back(chuoi.substr(batDau, viTri - batDau));
        batDau = viTri + 1;
    }
    cacTruong.push_back(chuoi.substr(batDau));
    return cacTruong;
}
string chuanHoaSo(const string& chuoi) {
    size_t soKyTuDaDoc = 0;
    double giaTri = stod(chuoi, &soKyTuDaDoc);
    if (soKyTuDaDoc != chuoi.size() || !isfinite(giaTri)) {
        throw runtime_error("So khong hop le trong du lieu.");
    }
    ostringstream ketQua;
    ketQua << fixed << setprecision(4) << giaTri;
    return ketQua.str();
}
string taoKhoa(const DongDuLieu& dong, int soCotKhoa) {
    if (soCotKhoa == 1) {
        return dong.at(0);
    }
    ostringstream khoa;
    khoa << dong.at(0) << '\t' << setw(10) << setfill('0') << stoi(dong.at(1));
    return khoa.str();
}
void themDong(DuLieuSQL& duLieu, const string& tenBang, DongDuLieu dong, int soCotKhoa = 1) {
    string khoa = taoKhoa(dong, soCotKhoa);
    BangDuLieu& bang = duLieu[tenBang];
    if (bang.count(khoa) != 0) {
        throw runtime_error("Trung khoa trong bang " + tenBang);
    }
    bang[khoa] = dong;
}
void tachHoaDon(DuLieuSQL& duLieu, const string& chuoiHoaDon, bool laDonDat,
                const string& trangThai = "", const string& maHoaDon = "") {
    DongDuLieu hoaDon = tachChuoi(chuoiHoaDon, '|');
    if (hoaDon.size() != 6 || hoaDon[0].empty() || hoaDon[5].empty()) {
        throw runtime_error("Hoa don hoac du tinh khong hop le.");
    }
    if (laDonDat) {
        themDong(duLieu, "DonDatMon",
                 {hoaDon[0], hoaDon[1], hoaDon[2], chuanHoaSo(hoaDon[4]), trangThai, maHoaDon});
    } else {
        themDong(duLieu, "HoaDon",
                 {hoaDon[0], hoaDon[1], hoaDon[2], hoaDon[3], chuanHoaSo(hoaDon[4])});
    }
    int soThuTu = 0;
    for (const string& chuoiChiTiet : tachChuoi(hoaDon[5], ';')) {
        DongDuLieu chiTiet = tachChuoi(chuoiChiTiet, ',');
        if (chiTiet.size() < 4) {
            throw runtime_error("Chi tiet hoa don khong hop le.");
        }
        DongDuLieu cacPhanTen(chiTiet.begin() + 1, chiTiet.end() - 2);
        themDong(duLieu, laDonDat ? "ChiTietDuTinh" : "ChiTietHoaDon",
                 {hoaDon[0], to_string(++soThuTu), chiTiet[0], ghepChuoi(cacPhanTen, ","),
                  chiTiet[chiTiet.size() - 2], chuanHoaSo(chiTiet.back())},
                 2);
    }
}
// Cac ham tach ben duoi chi xu ly chuoi trong bo nho, khong doc file TXT.
void tachMon(DuLieuSQL& duLieu, const DongDuLieu& truong) {
    if (truong.size() != 7 || (truong[0] != "MONAN" && truong[0] != "NUOC")) {
        throw runtime_error("Mon khong hop le");
    }
    // Thu tu SQL: Ma, Loai, Ten, Gia, TonKho, DoCay, MonChay, SizeLy, CoDa.
    DongDuLieu mon = {truong[1], truong[0], truong[2], chuanHoaSo(truong[3]), truong[4], "0",
                      "0",       "",        "0"};
    if (truong[0] == "MONAN") {
        mon[5] = truong[5];
        mon[6] = truong[6];
    } else {
        mon[7] = truong[5];
        mon[8] = truong[6];
    }
    themDong(duLieu, "Mon", mon);
}

void tachDonDat(DuLieuSQL& duLieu, const string& chuoiDon) {
    istringstream luongDon(chuoiDon);
    string trangThai, maHoaDon, duTinh;
    int soMon;
    if (!(luongDon >> quoted(trangThai) >> quoted(maHoaDon) >> quoted(duTinh) >> soMon) ||
        soMon < 1 || soMon > 200) {
        throw runtime_error("Don dat khong hop le");
    }
    tachHoaDon(duLieu, duTinh, true, trangThai, maHoaDon);
    string maDon = tachChuoi(duTinh, '|')[0];
    for (int i = 1; i <= soMon; ++i) {
        string maMon, sizeLy;
        int soLuong, coDa;
        if (!(luongDon >> quoted(maMon) >> soLuong >> quoted(sizeLy) >> coDa)) {
            throw runtime_error("Thieu chi tiet don dat");
        }
        DongDuLieu chiTiet = {maDon,  to_string(i),   maMon, to_string(soLuong),
                              sizeLy, to_string(coDa)};
        themDong(duLieu, "ChiTietDonDat", chiTiet, 2);
    }
    luongDon >> ws;
    if (!luongDon.eof()) {
        throw runtime_error("Du cot don dat");
    }
}

void tachMotDong(DuLieuSQL& duLieu, const string& tenNhom, const string& chuoiDong) {
    DongDuLieu truong = tachChuoi(chuoiDong, '|');
    if (tenNhom == "KhachHang") {
        if (truong.size() == 5) {
            truong.push_back("");
        }
        if (truong.size() != 6) {
            throw runtime_error("Khach hang can 5 hoac 6 cot");
        }
        themDong(duLieu, tenNhom, truong);
    } else if (tenNhom == "NhanVien") {
        if (truong.size() != 7) {
            throw runtime_error("Nhan vien can 7 cot");
        }
        truong[5] = chuanHoaSo(truong[5]);
        truong[6] = chuanHoaSo(truong[6]);
        themDong(duLieu, tenNhom, truong);
    } else if (tenNhom == "Mon") {
        tachMon(duLieu, truong);
    } else if (tenNhom == "TaiKhoan") {
        if (truong.size() != 6 || truong[0] != "PBKDF2-SHA256-600000") {
            throw runtime_error("Tai khoan khong hop le");
        }
        DongDuLieu taiKhoan = {truong[1], truong[2], truong[3], truong[4], truong[5], truong[0]};
        themDong(duLieu, tenNhom, taiKhoan);
    } else if (tenNhom == "HoaDon") {
        tachHoaDon(duLieu, chuoiDong, false);
    } else if (tenNhom == "DonDatMon") {
        tachDonDat(duLieu, chuoiDong);
    } else {
        throw runtime_error("Ten du lieu khong duoc ho tro");
    }
}

DuLieuSQL tachCacBang(const map<string, string>& noiDungCacNhom) {
    DuLieuSQL duLieu;
    for (const CauTrucBang& bang : CAC_BANG) {
        duLieu[bang.ten];
    }
    for (const auto& nhom : noiDungCacNhom) {
        istringstream luongNhom(nhom.second);
        string chuoiDong;
        int soDong = 0;
        while (getline(luongNhom, chuoiDong)) {
            ++soDong;
            if (!chuoiDong.empty() && chuoiDong.back() == '\r') {
                chuoiDong.pop_back();
            }
            if (soDong == 1 && chuoiDong.compare(0, 3, "\xEF\xBB\xBF") == 0) {
                chuoiDong.erase(0, 3);
            }
            if (chuoiDong.empty()) {
                continue;
            }
            try {
                tachMotDong(duLieu, nhom.first, chuoiDong);
            } catch (const exception& loi) {
                throw runtime_error(nhom.first + " dong " + to_string(soDong) + ": " + loi.what());
            }
        }
    }
    return duLieu;
}

string ghepHoaDon(const DuLieuSQL& duLieu, const DongDuLieu& hoaDon, bool laDonDat) {
    DongDuLieu thongTinChung =
        laDonDat ? DongDuLieu{hoaDon[0], hoaDon[1], hoaDon[2], "", hoaDon[3]} : hoaDon;
    DongDuLieu cacChiTiet;
    for (const auto& muc : duLieu.at(laDonDat ? "ChiTietDuTinh" : "ChiTietHoaDon")) {
        const DongDuLieu& chiTiet = muc.second;
        if (chiTiet[0] == hoaDon[0]) {
            cacChiTiet.push_back(ghepChuoi({chiTiet[2], chiTiet[3], chiTiet[4], chiTiet[5]}, ","));
        }
    }
    return ghepChuoi(thongTinChung, "|") + "|" + ghepChuoi(cacChiTiet, ";");
}
map<string, string> ghepCacBang(const DuLieuSQL& duLieu) {
    map<string, string> noiDungCacNhom;
    for (const auto& tenNhom : CAC_NHOM) {
        noiDungCacNhom[tenNhom];
    }
    for (const auto& muc : duLieu.at("KhachHang")) {
        noiDungCacNhom["KhachHang"] += ghepChuoi(muc.second, "|") + "\n";
    }
    for (const auto& muc : duLieu.at("NhanVien")) {
        noiDungCacNhom["NhanVien"] += ghepChuoi(muc.second, "|") + "\n";
    }
    for (const auto& muc : duLieu.at("Mon")) {
        const DongDuLieu& dong = muc.second;
        bool laMonAn = dong[1] == "MONAN";
        noiDungCacNhom["Mon"] +=
            ghepChuoi({dong[1], dong[0], dong[2], dong[3], dong[4], laMonAn ? dong[5] : dong[7],
                       laMonAn ? dong[6] : dong[8]},
                      "|") +
            "\n";
    }
    for (const auto& muc : duLieu.at("TaiKhoan")) {
        const DongDuLieu& dong = muc.second;
        noiDungCacNhom["TaiKhoan"] +=
            ghepChuoi({dong[5], dong[0], dong[1], dong[2], dong[3], dong[4]}, "|") + "\n";
    }
    for (const auto& muc : duLieu.at("HoaDon")) {
        noiDungCacNhom["HoaDon"] += ghepHoaDon(duLieu, muc.second, false) + "\n";
    }
    for (const auto& muc : duLieu.at("DonDatMon")) {
        const DongDuLieu& donDat = muc.second;
        vector<DongDuLieu> cacMon;
        for (const auto& mon : duLieu.at("ChiTietDonDat")) {
            if (mon.second[0] == donDat[0]) {
                cacMon.push_back(mon.second);
            }
        }
        ostringstream luongDon;
        luongDon << quoted(donDat[4]) << ' ' << quoted(donDat[5]) << ' '
                 << quoted(ghepHoaDon(duLieu, donDat, true)) << ' ' << cacMon.size();
        for (const DongDuLieu& dong : cacMon) {
            luongDon << ' ' << quoted(dong[2]) << ' ' << dong[3] << ' ' << quoted(dong[4]) << ' '
                     << dong[5];
        }
        noiDungCacNhom["DonDatMon"] += luongDon.str() + "\n";
    }
    return noiDungCacNhom;
}
DuLieuSQL docTatCaBang(SQLHDBC ketNoi) {
    DuLieuSQL duLieu;
    for (const CauTrucBang& bang : CAC_BANG) {
        duLieu[bang.ten];
        for (DongDuLieu dong : thucThiSQL(
                 ketNoi, "SELECT " + ghepChuoi(bang.tenCot, ",") + " FROM dbo." + bang.ten)) {
            themDong(duLieu, bang.ten, move(dong), bang.soCotKhoa);
        }
    }
    // Dua so DECIMAL ve cung dinh dang de so sanh chinh xac cac ban ghi.
    return tachCacBang(ghepCacBang(duLieu));
}
string dieuKienKhoa(const CauTrucBang& bang) {
    DongDuLieu cacDieuKien;
    for (int i = 0; i < bang.soCotKhoa; ++i) {
        cacDieuKien.push_back(bang.tenCot[i] + "=?");
    }
    return ghepChuoi(cacDieuKien, " AND ");
}
DongDuLieu layGiaTriKhoa(const CauTrucBang& bang, const DongDuLieu& dong) {
    DongDuLieu giaTriKhoa;
    for (int cot = 0; cot < bang.soCotKhoa; ++cot) {
        giaTriKhoa.push_back(dong[cot]);
    }
    return giaTriKhoa;
}

void xoaDongSQL(SQLHDBC ketNoi, const CauTrucBang& bang, const DongDuLieu& dong) {
    string sql = "DELETE FROM dbo." + bang.ten + " WHERE " + dieuKienKhoa(bang);
    thucThiSQL(ketNoi, sql, layGiaTriKhoa(bang, dong));
}

void themDongSQL(SQLHDBC ketNoi, const CauTrucBang& bang, const DongDuLieu& dong) {
    DongDuLieu dauHoi(dong.size(), "?");
    string sql = "INSERT INTO dbo." + bang.ten;
    sql += " (" + ghepChuoi(bang.tenCot, ",") + ")";
    sql += " VALUES (" + ghepChuoi(dauHoi, ",") + ")";
    thucThiSQL(ketNoi, sql, dong);
}

void suaDongSQL(SQLHDBC ketNoi, const CauTrucBang& bang, const DongDuLieu& dong) {
    DongDuLieu cacPhepGan;
    DongDuLieu thamSo;
    // Gia tri can sua ung voi cac dau ? trong SET.
    for (size_t cot = bang.soCotKhoa; cot < bang.tenCot.size(); ++cot) {
        cacPhepGan.push_back(bang.tenCot[cot] + "=?");
        thamSo.push_back(dong[cot]);
    }
    // Gia tri khoa ung voi cac dau ? trong WHERE.
    for (int cot = 0; cot < bang.soCotKhoa; ++cot) {
        thamSo.push_back(dong[cot]);
    }
    string sql = "UPDATE dbo." + bang.ten;
    sql += " SET " + ghepChuoi(cacPhepGan, ",");
    sql += " WHERE " + dieuKienKhoa(bang);
    thucThiSQL(ketNoi, sql, thamSo);
}

void luuCacThayDoi(SQLHDBC ketNoi, const DuLieuSQL& duLieuCu, const DuLieuSQL& duLieuMoi) {
    // 1. Xoa dong khong con trong bo nho. Xoa chi tiet truoc hoa don de giu khoa ngoai.
    for (int i = static_cast<int>(CAC_BANG.size()) - 1; i >= 0; --i) {
        const CauTrucBang& bang = CAC_BANG[i];
        const BangDuLieu& bangCu = duLieuCu.at(bang.ten);
        const BangDuLieu& bangMoi = duLieuMoi.at(bang.ten);
        for (const auto& muc : bangCu) {
            if (bangMoi.count(muc.first) == 0) {
                xoaDongSQL(ketNoi, bang, muc.second);
            }
        }
    }
    // 2. Them hoac sua dong. Them hoa don truoc cac dong chi tiet cua no.
    for (const CauTrucBang& bang : CAC_BANG) {
        const BangDuLieu& bangCu = duLieuCu.at(bang.ten);
        const BangDuLieu& bangMoi = duLieuMoi.at(bang.ten);
        for (const auto& muc : bangMoi) {
            const string& khoa = muc.first;
            const DongDuLieu& dongMoi = muc.second;
            if (bangCu.count(khoa) == 0) {
                themDongSQL(ketNoi, bang, dongMoi);
            } else if (bangCu.at(khoa) != dongMoi) {
                suaDongSQL(ketNoi, bang, dongMoi);
            }
        }
    }
}

wstring docCauHinh(const wchar_t* tenMuc, const wchar_t* macDinh) {
    wchar_t duongDan[32768], giaTri[1024];
    GetFullPathNameW(L"DuLieu\\sqlserver.ini", 32768, duongDan, nullptr);
    GetPrivateProfileStringW(L"SQLServer", tenMuc, macDinh, giaTri, 1024, duongDan);
    return giaTri;
}
wstring baoGiaTriKetNoi(wstring giaTri) {
    size_t viTri = 0;
    while ((viTri = giaTri.find(L'}', viTri)) != wstring::npos) {
        giaTri.insert(viTri, 1, L'}');
        viTri += 2;
    }
    return L"{" + giaTri + L"}";
}
}  // namespace

SqlServer::SqlServer() = default;

SqlServer::~SqlServer() {
    dongKetNoi();
}

SqlServer& SqlServer::instance() {
    static SqlServer coSoDuLieu;
    return coSoDuLieu;
}

void SqlServer::dongKetNoi() {
    if (ketNoi != SQL_NULL_HDBC) {
        SQLDisconnect(ketNoi);
        SQLFreeHandle(SQL_HANDLE_DBC, ketNoi);
        ketNoi = SQL_NULL_HDBC;
    }
    if (moiTruong != SQL_NULL_HENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, moiTruong);
        moiTruong = SQL_NULL_HENV;
    }
    daKetNoi = false;
}

void SqlServer::huyGiaoDich() {
    // Neu ket noi da mat, van giu lai loi goc de bao cho nguoi dung.
    try {
        thucThiSQL(ketNoi, "IF @@TRANCOUNT > 0 ROLLBACK");
    } catch (...) {
    }
}

void SqlServer::khoiTao() {
    if (daKetNoi) {
        return;
    }
    dongKetNoi();

    // 1. Tao moi truong ODBC va doi tuong ket noi.
    kiemTraLoiSQL(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &moiTruong), SQL_HANDLE_ENV,
                  moiTruong);
    kiemTraLoiSQL(SQLSetEnvAttr(moiTruong, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0),
                  SQL_HANDLE_ENV, moiTruong);
    kiemTraLoiSQL(SQLAllocHandle(SQL_HANDLE_DBC, moiTruong, &ketNoi), SQL_HANDLE_ENV, moiTruong);
    SQLSetConnectAttr(ketNoi, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

    // 2. Doc cau hinh va ket noi bang tai khoan Windows hien tai.
    wstring driver = docCauHinh(L"Driver", L"ODBC Driver 18 for SQL Server");
    wstring mayChu = docCauHinh(L"Server", L"localhost\\SQLEXPRESS");
    wstring tenDatabase = docCauHinh(L"Database", L"QuanLyNhaHang");
    wstring tinChungChi = docCauHinh(L"TrustServerCertificate", L"Yes");
    wstring chuoiKetNoi = L"DRIVER=" + baoGiaTriKetNoi(driver);
    chuoiKetNoi += L";SERVER=" + baoGiaTriKetNoi(mayChu);
    chuoiKetNoi += L";DATABASE=" + baoGiaTriKetNoi(tenDatabase);
    chuoiKetNoi += L";Trusted_Connection=Yes;Encrypt=Yes;TrustServerCertificate=";
    chuoiKetNoi += tinChungChi + L";APP=PBL2 NhaHang;";

    SQLRETURN ketQua = SQLDriverConnectW(ketNoi, nullptr, (SQLWCHAR*)chuoiKetNoi.c_str(), SQL_NTS,
                                         nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT);
    try {
        kiemTraLoiSQL(ketQua, SQL_HANDLE_DBC, ketNoi);
    } catch (const exception& loi) {
        throw runtime_error(string("Khong ket noi duoc SQL Server. Kiem tra dich vu SQLEXPRESS va "
                                   "DuLieu/sqlserver.ini. ") +
                            loi.what());
    }

    // 3. Thiet lap giao dich va gioi han mot cua so cho moi database.
    thucThiSQL(
        ketNoi,
        "SET XACT_ABORT ON; SET TRANSACTION ISOLATION LEVEL SERIALIZABLE; SET LOCK_TIMEOUT 10000;");
    vector<DongDuLieu> ketQuaKhoa =
        thucThiSQL(ketNoi,
                   "DECLARE @r int; EXEC @r=sys.sp_getapplock @Resource=N'PBL2_NhaHang_App', "
                   "@LockMode='Exclusive', @LockOwner='Session', @LockTimeout=0; SELECT @r;");
    if (ketQuaKhoa.empty() || stoi(ketQuaKhoa[0][0]) < 0) {
        throw runtime_error(
            "Database dang duoc mo boi mot cua so nha hang khac. Hay dong cua so do truoc.");
    }
    vector<DongDuLieu> phienBan = thucThiSQL(ketNoi, "SELECT PhienBan FROM dbo.CauHinh WHERE Id=1");
    if (phienBan.size() != 1 || phienBan[0][0] != "1") {
        throw runtime_error("Schema SQL Server khong tuong thich. Chay setup-sqlserver.ps1.");
    }

    // 4. Tai du lieu vao bo nho de cac lop nghiep vu su dung.
    daKetNoi = true;
    taiLai();
}

void SqlServer::taiLai() {
    if (!daKetNoi) {
        khoiTao();
        return;
    }
    thucThiSQL(ketNoi, "BEGIN TRANSACTION");
    try {
        DuLieuSQL duLieuMoi = docTatCaBang(ketNoi);
        thucThiSQL(ketNoi, "COMMIT");
        duLieuDaDoc = duLieuMoi;
        noiDungDaDoc = ghepCacBang(duLieuDaDoc);
    } catch (...) {
        huyGiaoDich();
        throw;
    }
}

string SqlServer::doc(const string& tenNhom) const {
    return noiDungDaDoc.at(tenNhom);
}

void SqlServer::luu(const map<string, string>& thayDoi) {
    khoiTao();

    // Chi thay cac nhom du lieu duoc truyen vao, giu nguyen cac nhom con lai.
    map<string, string> noiDungMoi = noiDungDaDoc;
    for (const auto& nhom : thayDoi) {
        if (noiDungMoi.count(nhom.first) == 0) {
            throw runtime_error("Ten du lieu khong hop le");
        }
        noiDungMoi[nhom.first] = nhom.second;
    }
    DuLieuSQL duLieuMoi = tachCacBang(noiDungMoi);

    thucThiSQL(ketNoi, "BEGIN TRANSACTION");
    try {
        DuLieuSQL duLieuHienTai = docTatCaBang(ketNoi);
        if (duLieuHienTai != duLieuDaDoc) {
            throw runtime_error(
                "Du lieu da thay doi trong SQL Server. Thao tac da huy; dong va mo lai ung dung de "
                "tai du lieu moi.");
        }

        luuCacThayDoi(ketNoi, duLieuDaDoc, duLieuMoi);
        // Doc lai gia tri thuc te sau khi SQL chuyen sang kieu DECIMAL.
        DuLieuSQL duLieuDaLuu = docTatCaBang(ketNoi);
        thucThiSQL(ketNoi, "COMMIT");
        duLieuDaDoc = duLieuDaLuu;
        noiDungDaDoc = ghepCacBang(duLieuDaDoc);
    } catch (...) {
        huyGiaoDich();
        throw;
    }
}
