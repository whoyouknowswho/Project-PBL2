#ifndef CHUDE_NHAHANG_H
#define CHUDE_NHAHANG_H

#include "TienIch.h"

// Bang mau theo phong cach Claude: nen nga am, chu gan den, mot mau nhan dat nung,
// vien manh thay cho bong do dam. Thanh ben sang cung tong voi nen.
namespace Mau {
const COLORREF NEN = RGB(250,249,245);
const COLORREF THE = RGB(255,255,255);
const COLORREF VIEN = RGB(232,230,220);
const COLORREF VIEN_O = RGB(218,215,204);
const COLORREF THANH_BEN = RGB(240,238,230);
const COLORREF THANH_BEN_RE = RGB(232,230,220);
const COLORREF CHU_THANH_BEN = RGB(61,61,58);
const COLORREF CHINH = RGB(217,119,87);
const COLORREF CHINH_RE = RGB(198,97,63);
const COLORREF CHINH_NHAN = RGB(176,85,48);
const COLORREF CHINH_NHAT = RGB(251,243,239);
const COLORREF PHU_NEN = RGB(245,233,226);
const COLORREF PHU_RE = RGB(240,221,210);
const COLORREF PHU_NHAN = RGB(232,205,190);
const COLORREF PHU_CHU = RGB(154,74,44);
const COLORREF LOI = RGB(181,74,60);
const COLORREF LOI_NEN = RGB(250,236,232);
const COLORREF LOI_RE = RGB(245,218,211);
const COLORREF TOT = RGB(66,110,73);
const COLORREF TOT_NEN = RGB(234,242,234);
const COLORREF CANH_BAO = RGB(169,119,42);
const COLORREF CANH_BAO_NEN = RGB(250,242,222);
const COLORREF XAM_NEN = RGB(240,238,230);
const COLORREF CHU = RGB(20,20,19);
const COLORREF CHU_NHAN = RGB(61,61,58);
const COLORREF CHU_MO = RGB(107,106,100);
const COLORREF CHU_KHOA = RGB(176,174,165);
const COLORREF O_CHI_DOC = RGB(247,246,241);
const COLORREF NUT_KHOA = RGB(238,236,229);
const COLORREF DONG_XEN = RGB(251,250,247);
// Khoi nen toi con giu lai (panel man dang nhap, banner chao khach, hop so lieu bieu do).
const COLORREF NEN_TOI = RGB(38,37,34);
const COLORREF CHU_TREN_TOI = RGB(240,238,230);
const COLORREF CHU_TREN_TOI_MO = RGB(176,174,165);
}

// Ma ky tu trong font Segoe MDL2 Assets.
namespace Icon {
const wchar_t TRANG_CHU = 0xE80F, KHACH = 0xE716, NHAN_VIEN = 0xE77B, THUC_DON = 0xE719,
    BAN_HANG = 0xE7BF, DON_DAT = 0xE8A5, BAO_CAO = 0xE9D2, TAI_KHOAN = 0xE8FA, MAT_KHAU = 0xE72E,
    HO_SO = 0xE779, HOA_DON = 0xE7C3, DANG_XUAT = 0xF3B1, LICH = 0xE787, TIM = 0xE721,
    THEM = 0xE710, TRU = 0xE738, XOA = 0xE74D, LUU = 0xE74E, LAM_MOI = 0xE72C,
    MUI_TEN_XUONG = 0xE70D, DANH_DAU = 0xE73E, THONG_TIN = 0xE946, TIEN = 0xE8C7,
    CANH_BAO = 0xE7BA, BIEU_DO = 0xE9D9, NGOI_SAO = 0xE734, NHA_HANG = 0xED56, HUY = 0xE711;
}

struct BoFont {
    HFONT nho = nullptr, nhoDam = nullptr, thuong = nullptr, nut = nullptr, dam = nullptr;
    HFONT tieuDe = nullptr, so = nullptr, icon = nullptr, iconLon = nullptr;
};
BoFont taoBoFont();
void xoaBoFont(BoFont& font);

bool coFontIcon();
SIZE doChu(HDC dc, const wstring& chu, HFONT font);
// Font lon neu chu vua chieu rong, nguoc lai font nho (tranh cat so tien bang "...").
HFONT fontVua(HDC dc, const wstring& chu, int rong, HFONT lon, HFONT nho);
void veChu(HDC dc, const wstring& chu, RECT vung, HFONT font, COLORREF mau, UINT can = DT_LEFT);
void veIcon(HDC dc, wchar_t kyTu, RECT vung, HFONT font, COLORREF mau);
void veHinhBoGoc(HDC dc, RECT vung, int banKinh, COLORREF nen,
                 COLORREF vien = CLR_INVALID, float doDay = 1.0f);
void veVienBoGoc(HDC dc, RECT vung, int banKinh, COLORREF mau, float doDay = 1.0f, BYTE alpha = 255);
void veThe(HDC dc, RECT vung, int banKinh = 14);
void phuMo(HDC dc, RECT vung, int banKinh, COLORREF mau, BYTE alpha);
void veHuyHieu(HDC dc, const wstring& chu, RECT vung, COLORREF nen, COLORREF mauChu, HFONT font);
int doRongHuyHieu(HDC dc, const wstring& chu, HFONT font);
void veChuyenSac(HDC dc, RECT vung, COLORREF dau, COLORREF cuoi, bool ngang = false);
void veVongTron(HDC dc, int cx, int cy, int banKinh, COLORREF mau, BYTE alpha = 255);
void veDuong(HDC dc, int x1, int y1, int x2, int y2, COLORREF mau, float doDay = 1.0f);
void toNen(HDC dc, RECT vung, COLORREF mau);

#endif
