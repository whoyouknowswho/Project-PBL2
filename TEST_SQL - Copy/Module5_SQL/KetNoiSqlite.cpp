#include "KetNoiSqlite.h"
#include <sqlite3.h>
#include <direct.h>
#include <cstdlib>
#include <stdexcept>

using namespace std;

namespace {
struct BangSql {
    const char* ten;
    const char* cot;
    const char* thamSo;
    const char* sapXep;
    int soCot;
};

const BangSql cacBang[] = {
    {"ThucDon",   "loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa,hinhAnh", "?,?,?,?,?,?,?,?", "ma",          8},
    {"KhachHang", "ma,ten,dienThoai,gioiTinh,diem,email",                         "?,?,?,?,?,?",     "ma",          6},
    {"NhanVien",  "ma,ten,dienThoai,gioiTinh,chucVu,luong,phuCap",                "?,?,?,?,?,?,?",   "ma",          7},
    {"HoaDon",    "ma,ngay,maKhach,maNhanVien,giamGia,chiTiet",                   "?,?,?,?,?,?",     "ma",          6},
    {"DonDat",    "noiDung",                                                      "?",               "thuTu",       1},
    {"TaiKhoan",  "tenDangNhap,matKhau,vaiTro,maLienKet",                         "?,?,?,?",         "tenDangNhap", 4}
};

const char* TAO_BANG =
    "CREATE TABLE IF NOT EXISTS ThucDon (loai TEXT, ma TEXT PRIMARY KEY, ten TEXT, gia REAL, tonKho INTEGER, doCayHoacSize TEXT, monChayHoacCoDa INTEGER, hinhAnh TEXT);"
    "CREATE TABLE IF NOT EXISTS KhachHang (ma TEXT PRIMARY KEY, ten TEXT, dienThoai TEXT, gioiTinh TEXT, diem INTEGER, email TEXT);"
    "CREATE TABLE IF NOT EXISTS NhanVien (ma TEXT PRIMARY KEY, ten TEXT, dienThoai TEXT, gioiTinh TEXT, chucVu TEXT, luong REAL, phuCap REAL);"
    "CREATE TABLE IF NOT EXISTS HoaDon (ma TEXT PRIMARY KEY, ngay TEXT, maKhach TEXT, maNhanVien TEXT, giamGia REAL, chiTiet TEXT);"
    "CREATE TABLE IF NOT EXISTS DonDat (thuTu INTEGER PRIMARY KEY AUTOINCREMENT, noiDung TEXT);"
    "CREATE TABLE IF NOT EXISTS TaiKhoan (tenDangNhap TEXT PRIMARY KEY, matKhau TEXT, vaiTro TEXT, maLienKet TEXT);";

struct Lenh {
    sqlite3_stmt* s = nullptr;
    ~Lenh() { if (s) sqlite3_finalize(s); }
};

struct KetNoi {
    sqlite3* db = nullptr;

    KetNoi() {
        _mkdir("DuLieu");
        const char* duongDan = getenv("PBL2_SQL_CONNECTION");
        if (!duongDan || !*duongDan) duongDan = "DuLieu/QuanLyNhaHang.db";
        if (sqlite3_open(duongDan, &db) != SQLITE_OK) {
            string loi = "Lỗi mở SQLite: " + string(db ? sqlite3_errmsg(db) : "");
            if (db) sqlite3_close(db);
            throw runtime_error(loi);
        }
        sqlite3_busy_timeout(db, 5000);
        chay(TAO_BANG);
    }

    ~KetNoi() { if (db) sqlite3_close(db); }

    string loi() const { return "Lỗi SQLite: " + string(sqlite3_errmsg(db)); }

    void chay(const string& sql) {
        char* err = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            string tb = "Lỗi SQLite: " + string(err ? err : sqlite3_errmsg(db));
            sqlite3_free(err);
            throw runtime_error(tb);
        }
    }
};
}  // namespace

Sql::CacDong Sql::doc(Bang bang) {
    KetNoi kn;
    const BangSql& b = cacBang[bang];
    string sql = string("SELECT ") + b.cot + " FROM " + b.ten + " ORDER BY " + b.sapXep;

    Lenh lenh;
    if (sqlite3_prepare_v2(kn.db, sql.c_str(), -1, &lenh.s, nullptr) != SQLITE_OK)
        throw runtime_error(kn.loi());

    int soCot = sqlite3_column_count(lenh.s);
    CacDong ketQua;
    while (sqlite3_step(lenh.s) == SQLITE_ROW) {
        Dong dong(soCot);
        for (int i = 0; i < soCot; ++i) {
            const unsigned char* txt = sqlite3_column_text(lenh.s, i);
            dong[i] = txt ? reinterpret_cast<const char*>(txt) : "";
        }
        ketQua.push_back(move(dong));
    }
    return ketQua;
}

void Sql::luu(const vector<pair<Bang, CacDong>>& thayDoi) {
    KetNoi kn;
    kn.chay("BEGIN IMMEDIATE TRANSACTION;");
    try {
        for (const auto& muc : thayDoi) {
            const BangSql& b = cacBang[muc.first];
            kn.chay(string("DELETE FROM ") + b.ten + ";");

            string sql = string("INSERT INTO ") + b.ten + " (" + b.cot + ") VALUES (" + b.thamSo + ");";
            Lenh lenh;
            if (sqlite3_prepare_v2(kn.db, sql.c_str(), -1, &lenh.s, nullptr) != SQLITE_OK)
                throw runtime_error(kn.loi());

            for (const Dong& dong : muc.second) {
                sqlite3_reset(lenh.s);
                for (int i = 0; i < b.soCot; ++i) {
                    const string& gt = static_cast<size_t>(i) < dong.size() ? dong[i] : "";
                    sqlite3_bind_text(lenh.s, i + 1, gt.c_str(), static_cast<int>(gt.size()), SQLITE_TRANSIENT);
                }
                if (sqlite3_step(lenh.s) != SQLITE_DONE)
                    throw runtime_error(kn.loi());
            }
        }
        kn.chay("COMMIT;");
    } catch (...) {
        try { kn.chay("ROLLBACK;"); } catch (...) {}
        throw;
    }
}
