#ifndef CUASO_NHAHANG_H
#define CUASO_NHAHANG_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <commctrl.h>
#include <map>
#include "DichVuNhaHang.h"

using namespace std;

enum Trang { TRANG_CHU = 100, KHACH_HANG, NHAN_VIEN, THUC_DON, BAN_HANG, BAO_CAO,
             TAI_KHOAN, MAT_KHAU, DANG_XUAT };
enum DieuKhien {
    TIM_KIEM = 200, LOC_LOAI, SAP_XEP, CON_HANG, LOC_BAO_CAO,
    BANG_CHINH = 210, BANG_GIO, BANG_TON,
    O_MA = 300, O_TEN, O_SDT, O_GIOITINH, O_DIEM, O_CHUCVU, O_LUONG, O_PHUCAP,
    O_LOAI, O_GIA, O_TON, O_CAY, O_CHAY, O_SIZE, O_DA, O_NHAPKHO,
    O_NGAY = 330, O_KHACH, O_NHANVIEN, O_SOLUONG, O_TONG,
    O_USER = 350, O_PASS, O_CONFIRM, O_ROLE, O_OLDPASS,
    NUT_LUU = 400, NUT_MOI, NUT_XOA, NUT_NHAPKHO, NUT_THEM_GIO,
    NUT_XOA_GIO, NUT_HUY_GIO, NUT_THANHTOAN, NUT_BAOCAO, NUT_HOADON,
    NUT_DANGNHAP, NUT_TAOTAIKHOAN, NUT_DOIMATKHAU
};

wstring rong(const string& utf8);
string hep(const wstring& unicode);
wstring tien(double soTien);
wstring so(double giaTri);
string homNay();
string maHoaDonMoi();
void xemHoaDon(HWND chu, const HoaDon& hoaDon);

class CuaSoNhaHang {
    HWND cuaSo = nullptr;
    HFONT fontChu = nullptr, fontNho = nullptr, fontDam = nullptr, fontLon = nullptr;
    HBRUSH nen = nullptr, trang = nullptr;
    map<HWND, int> kieuNut;
    DichVuNhaHang dichVu;
    bool thietLap = false, dangVeBang = false;
    int trangHienTai = TRANG_CHU, soLanSai = 0;
    int rongCuaSo = 1280, caoCuaSo = 820;
    RECT theBieuMau = {};
    string maDangSua, maHoaDon, ngayDon, khachDon, nhanVienDon;
    wstring thongBao = L"Sẵn sàng";
    vector<YeuCauMon> gioHang;
    vector<HoaDon> hoaDonBaoCao;

    HWND tao(const wchar_t* lop, const wstring& chu, DWORD kieu,
             int id, int x, int y, int w, int h, DWORD moRong = 0);
    void nhan(const wstring& chu, int x, int y, int w, bool dam = false);
    HWND nut(const wstring& chu, int id, int x, int y, int w, int kieu = 0);
    HWND nhap(const wstring& ten, int id, int x, int y, int w,
              const wstring& giaTri = L"", bool matKhau = false);
    HWND chon(const wstring& ten, int id, int x, int y, int w,
              const vector<wstring>& cacMuc);
    void danhDau(const wstring& ten, int id, int x, int y, int w, bool giaTri = false);
    HWND bang(int id, int x, int y, int w, int h,
              const vector<pair<wstring, int>>& cot);
    void dongBang(int id, const vector<wstring>& cot, int giaTri);
    int dongChon(int id) const;
    HWND o(int id) const;
    wstring chu(int id) const;
    string duLieu(int id) const;
    void gan(int id, const wstring& giaTri);
    int luaChon(int id) const;
    void chonMuc(int id, int viTri);
    bool daDanhDau(int id) const;
    double soNhap(int id, const string& ten, bool nguyen = false) const;
    void xoaDieuKhien();
    void hienTrang(int trangMoi, bool giuNhap = false);
    void ve(HDC dc);
    void veNut(const DRAWITEMSTRUCT* nut);
    void thanhBen();
    void trangDangNhap();
    void trangChu();
    void trangDanhSach();
    void trangBanHang();
    void trangBaoCao();
    void trangTaiKhoan();
    void trangMatKhau();
    void napBang();
    void napBanHang();
    void napGio();
    void napBaoCao();
    void chonDong();
    void luuBieuMau();
    void xoaBanGhi();
    void themVaoGio();
    void thanhToan();
    string khachDaChon() const;
    string nhanVienDaChon() const;
    void doiLoaiMon();
    void xuLyNut(int id, int suKien);
    void baoLoi(const string& noiDung);
    void thanhCong(const wstring& noiDung);
    bool xacNhan(const wstring& noiDung);
    static LRESULT CALLBACK thuTuc(HWND, UINT, WPARAM, LPARAM);

public:
    int chay(HINSTANCE ungDung, int cheDoHien);
};

int chayGiaoDienCuaSo();
#endif
