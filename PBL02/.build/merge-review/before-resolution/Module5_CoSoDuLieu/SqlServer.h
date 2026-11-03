#ifndef SQL_SERVER_H
#define SQL_SERVER_H
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <map>
#include <string>
#include <vector>

// Mot dong gom cac gia tri theo thu tu cot trong bang SQL.
using DongDuLieu = std::vector<std::string>;
// Moi bang tra cuu dong theo khoa chinh; moi database tra cuu bang theo ten.
using BangDuLieu = std::map<std::string, DongDuLieu>;
using DuLieuSQL = std::map<std::string, BangDuLieu>;

class SqlServer {
    SQLHENV moiTruong = SQL_NULL_HENV;
    SQLHDBC ketNoi = SQL_NULL_HDBC;
    bool daKetNoi = false;

    // Giu ban da doc de phat hien thay doi tu SSMS truoc khi luu.
    DuLieuSQL duLieuDaDoc;
    std::map<std::string, std::string> noiDungDaDoc;

    SqlServer();
    void dongKetNoi();
    void huyGiaoDich();

   public:
    ~SqlServer();
    SqlServer(const SqlServer&) = delete;
    SqlServer& operator=(const SqlServer&) = delete;
    static SqlServer& instance();  // Ca chuong trinh dung chung mot ket noi.
    void khoiTao();
    void taiLai();
    std::string doc(const std::string& tenNhom) const;
    void luu(const std::map<std::string, std::string>& thayDoi);
};
#endif
