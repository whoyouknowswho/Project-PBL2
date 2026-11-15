#include "KetNoiSqlServer.h"
#include "../Module7_CuaSo/TienIch.h"
#include <windows.h>
#include <sqlext.h>
#include <cstdlib>
#include <stdexcept>

using namespace std;

namespace {
struct BangSql {
    const wchar_t* ten;
    const wchar_t* cot;
    const wchar_t* sapXep;
};

// Danh sach 6 bang trong SQL Server va cac cot tuong ung.
const BangSql cacBang[] = {
    {L"dbo.ThucDon",   L"loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa,hinhAnh", L"ma"},
    {L"dbo.KhachHang", L"ma,ten,dienThoai,gioiTinh,diem,email",                         L"ma"},
    {L"dbo.NhanVien",  L"ma,ten,dienThoai,gioiTinh,chucVu,luong,phuCap",                L"ma"},
    {L"dbo.HoaDon",    L"ma,ngay,maKhach,maNhanVien,giamGia,chiTiet",                   L"ma"},
    {L"dbo.DonDat",    L"noiDung",                                                      L"thuTu"},
    {L"dbo.TaiKhoan",  L"tenDangNhap,matKhau,vaiTro,maLienKet",                         L"tenDangNhap"}
};

string layLoiOdbc(SQLSMALLINT loai, SQLHANDLE handle) {
    SQLWCHAR trangThai[6] = {}, thongBao[1024] = {};
    SQLINTEGER ma = 0;
    SQLSMALLINT doDai = 0;
    SQLGetDiagRecW(loai, handle, 1, trangThai, &ma, thongBao, 1024, &doDai);
    return "Lỗi SQL Server: " + hep(thongBao);
}

// Chuyen chuoi C++ sang chuoi N'...' an toan trong SQL Server.
wstring chuoiSql(const string& s) {
    wstring kq = L"N'";
    for (wchar_t c : rong(s)) {
        if (c == L'\'') kq += L"''";
        else kq += c;
    }
    return kq + L"'";
}

// Quan ly 1 phien ket noi ODBC: tu mo khi khoi tao va tu dong khi ra khoi ham.
struct KetNoi {
    SQLHENV env = SQL_NULL_HENV;
    SQLHDBC db = SQL_NULL_HDBC;
    SQLHSTMT lenh = SQL_NULL_HSTMT;

    KetNoi() {
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
        SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
        SQLAllocHandle(SQL_HANDLE_DBC, env, &db);

        wstring chuoi = L"Driver={ODBC Driver 18 for SQL Server};Server=.\\SQLEXPRESS;"
                        L"Database=QuanLyNhaHang_PBL2;Trusted_Connection=yes;TrustServerCertificate=yes;";
        if (const wchar_t* cauHinh = _wgetenv(L"PBL2_SQL_CONNECTION"))
            chuoi = cauHinh;

        SQLRETURN kq = SQLDriverConnectW(db, nullptr, (SQLWCHAR*)chuoi.c_str(), SQL_NTS,
                                         nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT);
        if (!SQL_SUCCEEDED(kq)) {
            string loi = layLoiOdbc(SQL_HANDLE_DBC, db);
            dong();
            throw runtime_error(loi);
        }
        SQLAllocHandle(SQL_HANDLE_STMT, db, &lenh);
    }

    ~KetNoi() { dong(); }

    void dong() {
        if (lenh) { SQLFreeHandle(SQL_HANDLE_STMT, lenh); lenh = SQL_NULL_HSTMT; }
        if (db)   { SQLDisconnect(db); SQLFreeHandle(SQL_HANDLE_DBC, db); db = SQL_NULL_HDBC; }
        if (env)  { SQLFreeHandle(SQL_HANDLE_ENV, env); env = SQL_NULL_HENV; }
    }

    void chay(const wstring& sql) {
        SQLFreeStmt(lenh, SQL_CLOSE);
        SQLRETURN kq = SQLExecDirectW(lenh, (SQLWCHAR*)sql.c_str(), SQL_NTS);
        if (!SQL_SUCCEEDED(kq) && kq != SQL_NO_DATA)
            throw runtime_error(layLoiOdbc(SQL_HANDLE_STMT, lenh));
    }
};
}

Sql::CacDong Sql::doc(Bang bang) {
    KetNoi kn;
    if (bang == ThucDon) {
        kn.chay(L"IF COL_LENGTH('dbo.ThucDon','hinhAnh') IS NULL "
                L"ALTER TABLE dbo.ThucDon ADD hinhAnh nvarchar(500);");
    }
    if (bang == TaiKhoan) {
        kn.chay(L"IF COL_LENGTH('dbo.TaiKhoan','matKhau') IS NULL BEGIN "
                L"DROP TABLE IF EXISTS dbo.TaiKhoan; "
                L"CREATE TABLE dbo.TaiKhoan(tenDangNhap varchar(50) PRIMARY KEY, matKhau nvarchar(100), vaiTro varchar(20), maLienKet varchar(50)); END "
                L"ELSE BEGIN "
                L"IF COL_LENGTH('dbo.TaiKhoan','maLienKet') IS NULL ALTER TABLE dbo.TaiKhoan ADD maLienKet varchar(50); "
                L"IF COL_LENGTH('dbo.TaiKhoan','maNhanVien') IS NOT NULL ALTER TABLE dbo.TaiKhoan DROP COLUMN maNhanVien; "
                L"IF COL_LENGTH('dbo.TaiKhoan','maKhachHang') IS NOT NULL ALTER TABLE dbo.TaiKhoan DROP COLUMN maKhachHang; "
                L"END");
    }
    const BangSql& b = cacBang[bang];
    kn.chay(wstring(L"SELECT ") + b.cot + L" FROM " + b.ten + L" ORDER BY " + b.sapXep);

    SQLSMALLINT soCot = 0;
    SQLNumResultCols(kn.lenh, &soCot);

    CacDong ketQua;
    while (SQL_SUCCEEDED(SQLFetch(kn.lenh))) {
        Dong dong;
        for (int i = 1; i <= soCot; ++i) {
            SQLWCHAR boDem[8192] = {};
            SQLLEN doDai = 0;
            SQLGetData(kn.lenh, i, SQL_C_WCHAR, boDem, sizeof(boDem), &doDai);
            dong.push_back(doDai == SQL_NULL_DATA ? "" : hep(boDem));
        }
        ketQua.push_back(dong);
    }
    return ketQua;
}

void Sql::luu(const vector<pair<Bang, CacDong>>& thayDoi) {
    KetNoi kn;
    kn.chay(L"BEGIN TRAN");
    try {
        for (const auto& muc : thayDoi) {
            const BangSql& b = cacBang[muc.first];
            kn.chay(wstring(L"DELETE FROM ") + b.ten);
            for (const Dong& dong : muc.second) {
                wstring giaTri;
                for (size_t i = 0; i < dong.size(); ++i) {
                    if (i > 0) giaTri += L", ";
                    giaTri += chuoiSql(dong[i]);
                }
                kn.chay(wstring(L"INSERT INTO ") + b.ten + L" (" + b.cot + L") VALUES (" + giaTri + L")");
            }
        }
        kn.chay(L"COMMIT");
    } catch (...) {
        try { kn.chay(L"IF @@TRANCOUNT > 0 ROLLBACK"); } catch (...) {}
        throw;
    }
}
