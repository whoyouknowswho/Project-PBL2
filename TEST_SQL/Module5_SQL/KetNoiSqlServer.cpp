#include "KetNoiSqlServer.h"
#include "../Module7_CuaSo/TienIch.h"
#include <sqlext.h>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <stdexcept>

using namespace std;

namespace {
struct BangSql {
    const wchar_t* ten;
    const wchar_t* cot;
    const wchar_t* sapXep;
};
const BangSql cacBang[] = {
    {L"dbo.ThucDon", L"loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa", L"ma"},
    {L"dbo.KhachHang", L"ma,ten,dienThoai,gioiTinh,diem,email", L"ma"},
    {L"dbo.NhanVien", L"ma,ten,dienThoai,gioiTinh,chucVu,luong,phuCap", L"ma"},
    {L"dbo.HoaDon", L"ma,ngay,maKhach,maNhanVien,giamGia,chiTiet", L"ma"},
    {L"dbo.DonDat", L"noiDung", L"thuTu"},
    {L"dbo.TaiKhoan", L"thuatToan,tenDangNhap,vaiTro,maLienKet,muoi,matKhauBam", L"tenDangNhap"}
};

void kiemTraOdbc(SQLRETURN kq, SQLSMALLINT loai, SQLHANDLE handle) {
    if (SQL_SUCCEEDED(kq)) return;
    SQLWCHAR trangThai[6] = {}, thongBao[1024] = {};
    SQLINTEGER ma;
    SQLSMALLINT dai;
    SQLGetDiagRecW(loai, handle, 1, trangThai, &ma, thongBao, 1024, &dai);
    throw runtime_error("Lỗi SQL Server: " + hep(thongBao));
}

struct TaiNguyenOdbc {
    SQLSMALLINT loai;
    SQLHANDLE handle = SQL_NULL_HANDLE;
    TaiNguyenOdbc(SQLSMALLINT kieu, SQLHANDLE cha) : loai(kieu) {
        if (!SQL_SUCCEEDED(SQLAllocHandle(loai, cha, &handle)))
            throw runtime_error("Không cấp phát được kết nối ODBC.");
    }
    ~TaiNguyenOdbc() { SQLFreeHandle(loai, handle); }
    TaiNguyenOdbc(const TaiNguyenOdbc&) = delete;
};

struct KetNoi {
    TaiNguyenOdbc moiTruong{SQL_HANDLE_ENV, SQL_NULL_HANDLE};
    unique_ptr<TaiNguyenOdbc> ketNoi;
    SQLHDBC db = SQL_NULL_HDBC;
    KetNoi() {
        kiemTraOdbc(SQLSetEnvAttr(moiTruong.handle, SQL_ATTR_ODBC_VERSION,
                                  (SQLPOINTER)SQL_OV_ODBC3, 0), SQL_HANDLE_ENV, moiTruong.handle);
        ketNoi = make_unique<TaiNguyenOdbc>(SQL_HANDLE_DBC, moiTruong.handle);
        db = ketNoi->handle;
        wstring chuoi = L"Driver={ODBC Driver 18 for SQL Server};Server=.\\SQLEXPRESS;"
                        L"Database=QuanLyNhaHang_PBL2;Trusted_Connection=yes;Encrypt=yes;TrustServerCertificate=yes;";
        if (const wchar_t* cauHinh = _wgetenv(L"PBL2_SQL_CONNECTION")) chuoi = cauHinh;
        kiemTraOdbc(SQLDriverConnectW(db, nullptr, &chuoi[0], SQL_NTS, nullptr, 0, nullptr,
                                     SQL_DRIVER_NOPROMPT), SQL_HANDLE_DBC, db);
    }
    ~KetNoi() { if (db) SQLDisconnect(db); }
};

void chay(SQLHSTMT lenh, const wstring& sql) {
    kiemTraOdbc(SQLExecDirectW(lenh, (SQLWCHAR*)sql.c_str(), SQL_NTS), SQL_HANDLE_STMT, lenh);
}

string docCot(SQLHSTMT lenh, SQLUSMALLINT cot) {
    wstring giaTri;
    SQLWCHAR phan[1024];
    SQLLEN dai;
    for (;;) {
        SQLRETURN kq = SQLGetData(lenh, cot, SQL_C_WCHAR, phan, sizeof(phan), &dai);
        if (kq == SQL_NO_DATA) break;
        kiemTraOdbc(kq, SQL_HANDLE_STMT, lenh);
        if (dai == SQL_NULL_DATA) break;
        giaTri += phan;
        if (kq == SQL_SUCCESS) break;
    }
    return hep(giaTri);
}
}

void Sql::khoiTao() {
    // Các danh sách được nạp một lần vào bộ nhớ. Chỉ một ứng dụng được sửa cùng database.
    static KetNoi phien;
    TaiNguyenOdbc lenh(SQL_HANDLE_STMT, phien.db);
    chay(lenh.handle, L"DECLARE @k int; EXEC @k=sys.sp_getapplock @Resource='PBL2_GUI', "
                       L"@LockMode='Exclusive', @LockOwner='Session', @LockTimeout=0; "
                       L"IF @k<0 THROW 50001, 'Database dang mo o mot phien khac.', 1;");
}

Sql::CacDong Sql::doc(Bang bang) {
    KetNoi kn;
    TaiNguyenOdbc lenh(SQL_HANDLE_STMT, kn.db);
    const auto& b = cacBang[bang];
    chay(lenh.handle, wstring(L"SELECT ") + b.cot + L" FROM " + b.ten + L" ORDER BY " + b.sapXep);
    SQLSMALLINT soCot;
    kiemTraOdbc(SQLNumResultCols(lenh.handle, &soCot), SQL_HANDLE_STMT, lenh.handle);
    CacDong ketQua;
    SQLRETURN kq;
    while ((kq = SQLFetch(lenh.handle)) != SQL_NO_DATA) {
        kiemTraOdbc(kq, SQL_HANDLE_STMT, lenh.handle);
        Dong dong;
        for (int i = 1; i <= soCot; ++i) dong.push_back(docCot(lenh.handle, i));
        ketQua.push_back(std::move(dong));
    }
    return ketQua;
}

void Sql::luu(const vector<pair<Bang, CacDong>>& thayDoi) {
    KetNoi kn;
    kiemTraOdbc(SQLSetConnectAttr(kn.db, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0),
                SQL_HANDLE_DBC, kn.db);
    try {
        for (const auto& muc : thayDoi) {
            const auto& b = cacBang[muc.first];
            TaiNguyenOdbc lenh(SQL_HANDLE_STMT, kn.db);
            chay(lenh.handle, wstring(L"DELETE FROM ") + b.ten);
            SQLFreeStmt(lenh.handle, SQL_CLOSE);
            wstring cot = b.cot, thamSo = L"?";
            for (wchar_t c : cot) if (c == L',') thamSo += L",?";
            wstring sql = wstring(L"INSERT INTO ") + b.ten + L" (" + cot + L") VALUES (" + thamSo + L")";
            kiemTraOdbc(SQLPrepareW(lenh.handle, &sql[0], SQL_NTS), SQL_HANDLE_STMT, lenh.handle);
            for (const auto& dong : muc.second) {
                vector<wstring> giaTri;
                for (const string& v : dong) giaTri.push_back(rong(v));
                vector<SQLLEN> doDai(dong.size(), SQL_NTS);
                for (size_t i = 0; i < dong.size(); ++i) {
                    auto& v = giaTri[i];
                    kiemTraOdbc(SQLBindParameter(lenh.handle, i + 1, SQL_PARAM_INPUT, SQL_C_WCHAR,
                        SQL_WVARCHAR, v.size() > 4000 ? 0 : max<size_t>(1, v.size()), 0,
                        (SQLPOINTER)v.c_str(), (v.size() + 1) * sizeof(wchar_t), &doDai[i]),
                        SQL_HANDLE_STMT, lenh.handle);
                }
                kiemTraOdbc(SQLExecute(lenh.handle), SQL_HANDLE_STMT, lenh.handle);
                SQLFreeStmt(lenh.handle, SQL_CLOSE);
            }
        }
        kiemTraOdbc(SQLEndTran(SQL_HANDLE_DBC, kn.db, SQL_COMMIT), SQL_HANDLE_DBC, kn.db);
    } catch (...) {
        SQLEndTran(SQL_HANDLE_DBC, kn.db, SQL_ROLLBACK);
        throw;
    }
}
