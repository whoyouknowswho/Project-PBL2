#include "ChuDe.h"
#include <objidl.h>
#include <gdiplus.h>
#include <algorithm>

using namespace std;

namespace {
HFONT taoFont(int co, int doDam, const wchar_t* ten = L"Segoe UI") {
    return CreateFontW(-co, 0, 0, 0, doDam, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, ten);
}

Gdiplus::Color mauGdi(COLORREF c, BYTE alpha = 255) {
    return Gdiplus::Color(alpha, GetRValue(c), GetGValue(c), GetBValue(c));
}

void duongBoGoc(Gdiplus::GraphicsPath& duong, Gdiplus::RectF r, float banKinh) {
    float d = banKinh * 2;
    d = min(d, min(r.Width, r.Height));
    if (d <= 0.5f) { duong.AddRectangle(r); return; }
    duong.AddArc(r.X, r.Y, d, d, 180, 90);
    duong.AddArc(r.X + r.Width - d, r.Y, d, d, 270, 90);
    duong.AddArc(r.X + r.Width - d, r.Y + r.Height - d, d, d, 0, 90);
    duong.AddArc(r.X, r.Y + r.Height - d, d, d, 90, 90);
    duong.CloseFigure();
}

void chuanBi(Gdiplus::Graphics& g) {
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
}

int CALLBACK timFont(const LOGFONTW*, const TEXTMETRICW*, DWORD, LPARAM thamSo) {
    *reinterpret_cast<bool*>(thamSo) = true;
    return 0;
}
}

BoFont taoBoFont() {
    BoFont f;
    f.nho = taoFont(14, FW_NORMAL);
    f.nhoDam = taoFont(14, FW_SEMIBOLD);
    f.thuong = taoFont(15, FW_NORMAL);
    f.nut = taoFont(15, FW_SEMIBOLD);
    f.dam = taoFont(17, FW_SEMIBOLD);
    f.tieuDe = taoFont(26, FW_BOLD);
    f.so = taoFont(24, FW_BOLD);
    f.icon = taoFont(16, FW_NORMAL, L"Segoe MDL2 Assets");
    f.iconLon = taoFont(20, FW_NORMAL, L"Segoe MDL2 Assets");
    return f;
}

void xoaBoFont(BoFont& f) {
    for (HFONT* h : {&f.nho, &f.nhoDam, &f.thuong, &f.nut, &f.dam, &f.tieuDe, &f.so, &f.icon, &f.iconLon}) {
        if (*h) DeleteObject(*h);
        *h = nullptr;
    }
}

bool coFontIcon() {
    static int daKiem = -1;
    if (daKiem < 0) {
        bool co = false;
        LOGFONTW lf = {};
        lf.lfCharSet = DEFAULT_CHARSET;
        lstrcpyW(lf.lfFaceName, L"Segoe MDL2 Assets");
        HDC dc = GetDC(nullptr);
        EnumFontFamiliesExW(dc, &lf, timFont, reinterpret_cast<LPARAM>(&co), 0);
        ReleaseDC(nullptr, dc);
        daKiem = co ? 1 : 0;
    }
    return daKiem == 1;
}

SIZE doChu(HDC dc, const wstring& chu, HFONT font) {
    HGDIOBJ cu = SelectObject(dc, font);
    SIZE kq = {0, 0};
    GetTextExtentPoint32W(dc, chu.c_str(), static_cast<int>(chu.size()), &kq);
    SelectObject(dc, cu);
    return kq;
}

HFONT fontVua(HDC dc, const wstring& chu, int rong, HFONT lon, HFONT nho) {
    return doChu(dc, chu, lon).cx <= rong ? lon : nho;
}

void veChu(HDC dc, const wstring& chu, RECT vung, HFONT font, COLORREF mau, UINT can) {
    if (chu.empty()) return;
    HGDIOBJ cu = SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, mau);
    UINT kieu = can | DT_NOPREFIX;
    if (!(can & DT_WORDBREAK)) kieu |= DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
    DrawTextW(dc, chu.c_str(), -1, &vung, kieu);
    SelectObject(dc, cu);
}

void veIcon(HDC dc, wchar_t kyTu, RECT vung, HFONT font, COLORREF mau) {
    if (!kyTu || !coFontIcon()) return;
    wchar_t chu[2] = {kyTu, 0};
    HGDIOBJ cu = SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, mau);
    DrawTextW(dc, chu, 1, &vung, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    SelectObject(dc, cu);
}

void veHinhBoGoc(HDC dc, RECT r, int banKinh, COLORREF nen, COLORREF vien, float doDay) {
    if (r.right <= r.left || r.bottom <= r.top) return;
    Gdiplus::Graphics g(dc);
    chuanBi(g);
    Gdiplus::GraphicsPath duong;
    duongBoGoc(duong, Gdiplus::RectF(static_cast<float>(r.left), static_cast<float>(r.top),
                                     static_cast<float>(r.right - r.left), static_cast<float>(r.bottom - r.top)),
               static_cast<float>(banKinh));
    Gdiplus::SolidBrush co(mauGdi(nen));
    g.FillPath(&co, &duong);
    if (vien != CLR_INVALID) {
        Gdiplus::GraphicsPath vienDuong;
        float le = doDay / 2;
        duongBoGoc(vienDuong, Gdiplus::RectF(r.left + le, r.top + le, r.right - r.left - doDay, r.bottom - r.top - doDay),
                   max(0.0f, banKinh - le));
        Gdiplus::Pen but(mauGdi(vien), doDay);
        g.DrawPath(&but, &vienDuong);
    }
}

void veVienBoGoc(HDC dc, RECT r, int banKinh, COLORREF mau, float doDay, BYTE alpha) {
    if (r.right <= r.left || r.bottom <= r.top) return;
    Gdiplus::Graphics g(dc);
    chuanBi(g);
    Gdiplus::GraphicsPath duong;
    float le = doDay / 2;
    duongBoGoc(duong, Gdiplus::RectF(r.left + le, r.top + le, r.right - r.left - doDay, r.bottom - r.top - doDay),
               max(0.0f, banKinh - le));
    Gdiplus::Pen but(mauGdi(mau, alpha), doDay);
    g.DrawPath(&but, &duong);
}

void veThe(HDC dc, RECT r, int banKinh) {
    if (r.right <= r.left || r.bottom <= r.top) return;
    {
        Gdiplus::Graphics g(dc);
        chuanBi(g);
        // Bong do mem: vai lop mo dan, lech xuong duoi.
        const int lop[3][3] = {{1, 2, 14}, {3, 4, 8}, {6, 7, 4}};
        for (const auto& l : lop) {
            Gdiplus::GraphicsPath bong;
            duongBoGoc(bong, Gdiplus::RectF(static_cast<float>(r.left - l[0] + 1), static_cast<float>(r.top - l[0] + l[1]),
                                            static_cast<float>(r.right - r.left + 2 * l[0] - 2),
                                            static_cast<float>(r.bottom - r.top + 2 * l[0] - l[1])),
                       static_cast<float>(banKinh + l[0]));
            Gdiplus::SolidBrush co(Gdiplus::Color(static_cast<BYTE>(l[2]), 70, 40, 20));
            g.FillPath(&co, &bong);
        }
    }
    veHinhBoGoc(dc, r, banKinh, Mau::THE, Mau::VIEN, 1.0f);
}

void phuMo(HDC dc, RECT r, int banKinh, COLORREF mau, BYTE alpha) {
    if (r.right <= r.left || r.bottom <= r.top) return;
    Gdiplus::Graphics g(dc);
    chuanBi(g);
    Gdiplus::GraphicsPath duong;
    duongBoGoc(duong, Gdiplus::RectF(static_cast<float>(r.left), static_cast<float>(r.top),
                                     static_cast<float>(r.right - r.left), static_cast<float>(r.bottom - r.top)),
               static_cast<float>(banKinh));
    Gdiplus::SolidBrush co(mauGdi(mau, alpha));
    g.FillPath(&co, &duong);
}

int doRongHuyHieu(HDC dc, const wstring& chu, HFONT font) { return doChu(dc, chu, font).cx + 22; }

void veHuyHieu(HDC dc, const wstring& chu, RECT r, COLORREF nen, COLORREF mauChu, HFONT font) {
    veHinhBoGoc(dc, r, (r.bottom - r.top) / 2, nen);
    veChu(dc, chu, r, font, mauChu, DT_CENTER);
}

void veChuyenSac(HDC dc, RECT r, COLORREF dau, COLORREF cuoi, bool ngang) {
    if (r.right <= r.left || r.bottom <= r.top) return;
    Gdiplus::Graphics g(dc);
    Gdiplus::RectF vung(static_cast<float>(r.left), static_cast<float>(r.top),
                        static_cast<float>(r.right - r.left), static_cast<float>(r.bottom - r.top));
    Gdiplus::RectF khung(vung.X - 1, vung.Y - 1, vung.Width + 2, vung.Height + 2);
    Gdiplus::LinearGradientBrush co(khung, mauGdi(dau), mauGdi(cuoi),
                                    ngang ? Gdiplus::LinearGradientModeHorizontal : Gdiplus::LinearGradientModeVertical);
    g.FillRectangle(&co, vung);
}

void veVongTron(HDC dc, int cx, int cy, int banKinh, COLORREF mau, BYTE alpha) {
    Gdiplus::Graphics g(dc);
    chuanBi(g);
    Gdiplus::SolidBrush co(mauGdi(mau, alpha));
    g.FillEllipse(&co, static_cast<float>(cx - banKinh), static_cast<float>(cy - banKinh),
                  static_cast<float>(2 * banKinh), static_cast<float>(2 * banKinh));
}

void veDuong(HDC dc, int x1, int y1, int x2, int y2, COLORREF mau, float doDay) {
    Gdiplus::Graphics g(dc);
    chuanBi(g);
    Gdiplus::Pen but(mauGdi(mau), doDay);
    g.DrawLine(&but, static_cast<float>(x1), static_cast<float>(y1) + 0.5f,
               static_cast<float>(x2), static_cast<float>(y2) + 0.5f);
}

void toNen(HDC dc, RECT r, COLORREF mau) {
    HBRUSH co = CreateSolidBrush(mau);
    FillRect(dc, &r, co);
    DeleteObject(co);
}
