#ifndef CHUDE_NHAHANG_H
#define CHUDE_NHAHANG_H

#include "TienIch.h"

// Bang mau "am ap kiem nha hang" dung chung cho moi man hinh.
namespace Mau {
const COLORREF NEN = RGB(250,246,240);
const COLORREF THE = RGB(255,255,255);
const COLORREF VIEN = RGB(237,228,216);
const COLORREF VIEN_O = RGB(222,209,195);
const COLORREF THANH_BEN = RGB(43,29,22);
const COLORREF THANH_BEN_RE = RGB(61,42,32);
const COLORREF CHU_THANH_BEN = RGB(214,198,186);
const COLORREF CHINH = RGB(217,98,43);
const COLORREF CHINH_RE = RGB(196,85,31);
const COLORREF CHINH_NHAN = RGB(168,71,25);
const COLORREF CHINH_NHAT = RGB(255,248,242);
const COLORREF PHU_NEN = RGB(251,237,227);
const COLORREF PHU_RE = RGB(247,224,207);
const COLORREF PHU_NHAN = RGB(240,208,186);
const COLORREF PHU_CHU = RGB(138,59,18);
const COLORREF LOI = RGB(192,57,43);
const COLORREF LOI_NEN = RGB(253,236,234);
const COLORREF LOI_RE = RGB(250,218,214);
const COLORREF TOT = RGB(46,139,87);
const COLORREF TOT_NEN = RGB(230,244,236);
const COLORREF CANH_BAO = RGB(183,121,31);
const COLORREF CANH_BAO_NEN = RGB(255,245,219);
const COLORREF XAM_NEN = RGB(239,234,230);
const COLORREF CHU = RGB(45,36,32);
const COLORREF CHU_NHAN = RGB(94,80,72);
const COLORREF CHU_MO = RGB(138,123,112);
const COLORREF CHU_KHOA = RGB(184,172,162);
const COLORREF O_CHI_DOC = RGB(251,248,243);
const COLORREF NUT_KHOA = RGB(240,235,229);
const COLORREF DONG_XEN = RGB(253,250,246);
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
