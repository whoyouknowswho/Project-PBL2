#include "LuoiMon.h"
#include <algorithm>

using namespace std;

namespace {
const int RONG_TOI_THIEU = 200, CAO_THE = 236, CAO_THE_TOI_THIEU = 150, KHOANG = 16, DEM = 6;

struct TrangThaiLuoi {
    vector<TheMon> the;
    int chon = -1, re = -1, cuon = 0;
    const BoFont* font = nullptr;
    KhoAnh* anh = nullptr;
};

struct BoCucLuoi {
    int soCot = 1, rongThe = RONG_TOI_THIEU, caoThe = CAO_THE, tongCao = 0;
};

TrangThaiLuoi* trangThai(HWND h) { return reinterpret_cast<TrangThaiLuoi*>(GetWindowLongPtrW(h, GWLP_USERDATA)); }

BoCucLuoi tinhBoCuc(HWND h, const TrangThaiLuoi& tt) {
    RECT r;
    GetClientRect(h, &r);
    BoCucLuoi b;
    int rong = max(1, static_cast<int>(r.right) - 2 * DEM);
    b.soCot = max(1, (rong + KHOANG) / (RONG_TOI_THIEU + KHOANG));
    b.rongThe = max(80, (rong - (b.soCot - 1) * KHOANG) / b.soCot);
    // Luoi thap (cua so nho): thu gon phan anh de van thay tron mot the.
    b.caoThe = max(CAO_THE_TOI_THIEU, min(CAO_THE, static_cast<int>(r.bottom) - 2 * DEM - 2));
    int soHang = (static_cast<int>(tt.the.size()) + b.soCot - 1) / b.soCot;
    b.tongCao = soHang > 0 ? soHang * (b.caoThe + KHOANG) - KHOANG + 2 * DEM : 0;
    return b;
}

RECT vungThe(const BoCucLuoi& b, int i, int cuon) {
    int cot = i % b.soCot, hang = i / b.soCot;
    int x = DEM + cot * (b.rongThe + KHOANG), y = DEM + hang * (b.caoThe + KHOANG) - cuon;
    return {x, y, x + b.rongThe, y + b.caoThe};
}

void capNhatCuon(HWND h, TrangThaiLuoi& tt) {
    RECT r;
    GetClientRect(h, &r);
    BoCucLuoi b = tinhBoCuc(h, tt);
    int toiDa = max(0, b.tongCao - static_cast<int>(r.bottom));
    tt.cuon = max(0, min(tt.cuon, toiDa));
    SCROLLINFO si = {sizeof(si), SIF_RANGE | SIF_PAGE | SIF_POS, 0, max(0, b.tongCao - 1),
                     static_cast<UINT>(max(1, static_cast<int>(r.bottom))), tt.cuon, 0};
    SetScrollInfo(h, SB_VERT, &si, TRUE);
}

int timThe(HWND h, const TrangThaiLuoi& tt, int x, int y) {
    BoCucLuoi b = tinhBoCuc(h, tt);
    POINT p = {x, y};
    for (int i = 0; i < static_cast<int>(tt.the.size()); i++) {
        RECT r = vungThe(b, i, tt.cuon);
        if (PtInRect(&r, p)) return i;
    }
    return -1;
}

void baoCha(HWND h, int suKien) {
    SendMessageW(GetParent(h), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(h), suKien), reinterpret_cast<LPARAM>(h));
}

void hienThe(HWND h, TrangThaiLuoi& tt, int i) {
    RECT r;
    GetClientRect(h, &r);
    BoCucLuoi b = tinhBoCuc(h, tt);
    RECT the = vungThe(b, i, 0);
    if (the.top - DEM < tt.cuon) tt.cuon = the.top - DEM;
    if (the.bottom + DEM > tt.cuon + r.bottom) tt.cuon = the.bottom + DEM - r.bottom;
    capNhatCuon(h, tt);
}

void chonThe(HWND h, TrangThaiLuoi& tt, int i) {
    if (i < 0 || i >= static_cast<int>(tt.the.size())) return;
    tt.chon = i;
    hienThe(h, tt, i);
    InvalidateRect(h, nullptr, FALSE);
    baoCha(h, LBN_SELCHANGE);
}

void veLuoi(HWND h, HDC dc, TrangThaiLuoi& tt) {
    RECT r;
    GetClientRect(h, &r);
    toNen(dc, r, Mau::NEN);
    const BoFont& f = *tt.font;
    if (tt.the.empty()) {
        RECT chu = {r.left + 16, r.top + 40, r.right - 16, r.top + 80};
        veChu(dc, L"Không có món phù hợp. Hãy thử tìm kiếm hoặc đổi bộ lọc.", chu, f.thuong, Mau::CHU_MO, DT_CENTER);
        return;
    }
    BoCucLuoi b = tinhBoCuc(h, tt);
    for (int i = 0; i < static_cast<int>(tt.the.size()); i++) {
        RECT o = vungThe(b, i, tt.cuon);
        if (o.bottom < r.top - 8 || o.top > r.bottom + 8) continue;
        const TheMon& t = tt.the[i];
        bool chon = i == tt.chon, re = i == tt.re;
        veThe(dc, o, 14);
        if (chon) veHinhBoGoc(dc, o, 14, Mau::CHINH_NHAT, Mau::CHINH, 2.0f);
        else if (re) veVienBoGoc(dc, o, 14, Mau::CHINH_RE, 1.5f);
        RECT anh = {o.left + 10, o.top + 10, o.right - 10, o.bottom - 98};
        if (tt.anh && !t.maMon.empty()) tt.anh->veAnhMon(dc, t.maMon, t.loaiMon, t.tenMon, t.hinhAnh, anh, 10);
        if (t.mo) phuMo(dc, anh, 10, RGB(255,255,255), 140);
        if (!t.huyHieu.empty()) {
            int w = min(doRongHuyHieu(dc, t.huyHieu, f.nhoDam), static_cast<int>(anh.right - anh.left) - 16);
            RECT hh = {anh.left + 8, anh.top + 8, anh.left + 8 + w, anh.top + 32};
            veHuyHieu(dc, t.huyHieu, hh, t.nenHuyHieu, t.mauHuyHieu, f.nhoDam);
        }
        COLORREF mauTen = t.mo ? Mau::CHU_MO : Mau::CHU;
        veChu(dc, t.ten, {o.left + 14, o.bottom - 90, o.right - 14, o.bottom - 64}, f.dam, mauTen);
        veChu(dc, t.thongTin, {o.left + 14, o.bottom - 62, o.right - 14, o.bottom - 40}, f.nho, Mau::CHU_MO);
        veChu(dc, t.gia, {o.left + 14, o.bottom - 34, o.right - 14, o.bottom - 10}, f.dam,
              t.mo ? Mau::CHU_MO : Mau::CHINH);
    }
}

LRESULT CALLBACK thuTucLuoi(HWND h, UINT msg, WPARAM w, LPARAM l) {
    TrangThaiLuoi* tt = trangThai(h);
    switch (msg) {
        case WM_NCCREATE: {
            CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(l);
            SetWindowLongPtrW(h, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
            break;
        }
        case WM_NCDESTROY:
            delete tt;
            SetWindowLongPtrW(h, GWLP_USERDATA, 0);
            break;
        case WM_SIZE:
            if (tt) { capNhatCuon(h, *tt); InvalidateRect(h, nullptr, FALSE); }
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(h, &ps);
            RECT r;
            GetClientRect(h, &r);
            HDC boNho = CreateCompatibleDC(dc);
            HBITMAP anhDem = CreateCompatibleBitmap(dc, max(1, static_cast<int>(r.right)), max(1, static_cast<int>(r.bottom)));
            HGDIOBJ cu = SelectObject(boNho, anhDem);
            if (tt) veLuoi(h, boNho, *tt);
            BitBlt(dc, 0, 0, r.right, r.bottom, boNho, 0, 0, SRCCOPY);
            SelectObject(boNho, cu);
            DeleteObject(anhDem);
            DeleteDC(boNho);
            EndPaint(h, &ps);
            return 0;
        }
        case WM_VSCROLL: {
            if (!tt) return 0;
            RECT r;
            GetClientRect(h, &r);
            SCROLLINFO si = {};
            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;
            GetScrollInfo(h, SB_VERT, &si);
            switch (LOWORD(w)) {
                case SB_LINEUP: tt->cuon -= 48; break;
                case SB_LINEDOWN: tt->cuon += 48; break;
                case SB_PAGEUP: tt->cuon -= r.bottom; break;
                case SB_PAGEDOWN: tt->cuon += r.bottom; break;
                case SB_THUMBTRACK: case SB_THUMBPOSITION: tt->cuon = si.nTrackPos; break;
                case SB_TOP: tt->cuon = 0; break;
                case SB_BOTTOM: tt->cuon = si.nMax; break;
            }
            capNhatCuon(h, *tt);
            InvalidateRect(h, nullptr, FALSE);
            return 0;
        }
        case WM_MOUSEWHEEL:
            if (tt) {
                tt->cuon -= GET_WHEEL_DELTA_WPARAM(w) * 72 / WHEEL_DELTA;
                capNhatCuon(h, *tt);
                InvalidateRect(h, nullptr, FALSE);
            }
            return 0;
        case WM_MOUSEMOVE:
            if (tt) {
                int i = timThe(h, *tt, static_cast<short>(LOWORD(l)), static_cast<short>(HIWORD(l)));
                if (i != tt->re) {
                    tt->re = i;
                    InvalidateRect(h, nullptr, FALSE);
                    TRACKMOUSEEVENT theoDoi = {sizeof(theoDoi), TME_LEAVE, h, 0};
                    TrackMouseEvent(&theoDoi);
                }
            }
            return 0;
        case WM_MOUSELEAVE:
            if (tt && tt->re != -1) { tt->re = -1; InvalidateRect(h, nullptr, FALSE); }
            return 0;
        case WM_SETCURSOR:
            if (tt && LOWORD(l) == HTCLIENT && tt->re >= 0) { SetCursor(LoadCursor(nullptr, IDC_HAND)); return TRUE; }
            break;
        case WM_LBUTTONDOWN:
            SetFocus(h);
            if (tt) chonThe(h, *tt, timThe(h, *tt, static_cast<short>(LOWORD(l)), static_cast<short>(HIWORD(l))));
            return 0;
        case WM_LBUTTONDBLCLK:
            if (tt && timThe(h, *tt, static_cast<short>(LOWORD(l)), static_cast<short>(HIWORD(l))) >= 0) baoCha(h, LBN_DBLCLK);
            return 0;
        case WM_GETDLGCODE:
            return DLGC_WANTARROWS;
        case WM_KEYDOWN:
            if (tt && !tt->the.empty()) {
                int n = static_cast<int>(tt->the.size()), soCot = tinhBoCuc(h, *tt).soCot, i = max(0, tt->chon);
                if (tt->chon < 0 && (w == VK_LEFT || w == VK_RIGHT || w == VK_UP || w == VK_DOWN)) { chonThe(h, *tt, 0); return 0; }
                if (w == VK_LEFT) chonThe(h, *tt, max(0, i - 1));
                else if (w == VK_RIGHT) chonThe(h, *tt, min(n - 1, i + 1));
                else if (w == VK_UP) chonThe(h, *tt, max(0, i - soCot));
                else if (w == VK_DOWN) chonThe(h, *tt, min(n - 1, i + soCot));
                else if (w == VK_RETURN && tt->chon >= 0) baoCha(h, LBN_DBLCLK);
            }
            return 0;
        case LM_CHONMUC:
            if (tt && static_cast<int>(w) >= 0 && static_cast<int>(w) < static_cast<int>(tt->the.size())) {
                chonThe(h, *tt, static_cast<int>(w));
                return 1;
            }
            return 0;
        case LM_LAYCHON:
            return tt && tt->chon >= 0 && tt->chon < static_cast<int>(tt->the.size()) ? tt->the[tt->chon].duLieu : -1;
        case LM_SOMUC:
            return tt ? static_cast<LRESULT>(tt->the.size()) : 0;
        case LM_CAOTHE:
            return tt ? tinhBoCuc(h, *tt).caoThe : 0;
    }
    return DefWindowProcW(h, msg, w, l);
}
}

HWND taoLuoiMon(HWND cha, int id, RECT vung, const BoFont* font, KhoAnh* anh) {
    static bool daDangKy = false;
    if (!daDangKy) {
        WNDCLASSW lop = {};
        lop.style = CS_DBLCLKS;
        lop.lpfnWndProc = thuTucLuoi;
        lop.hInstance = GetModuleHandleW(nullptr);
        lop.hCursor = LoadCursor(nullptr, IDC_ARROW);
        lop.lpszClassName = L"NhaHangLuoiMon";
        if (!RegisterClassW(&lop) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) return nullptr;
        daDangKy = true;
    }
    TrangThaiLuoi* tt = new TrangThaiLuoi();
    tt->font = font;
    tt->anh = anh;
    HWND h = CreateWindowExW(0, L"NhaHangLuoiMon", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL,
                             vung.left, vung.top, vung.right - vung.left, vung.bottom - vung.top, cha,
                             reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), GetModuleHandleW(nullptr), tt);
    if (!h) delete tt;
    else capNhatCuon(h, *tt);
    return h;
}

void luoiMonDat(HWND luoi, const vector<TheMon>& cacThe, int duLieuChon) {
    TrangThaiLuoi* tt = trangThai(luoi);
    if (!tt) return;
    tt->the = cacThe;
    tt->chon = -1;
    tt->re = -1;
    for (size_t i = 0; i < cacThe.size(); i++)
        if (duLieuChon >= 0 && cacThe[i].duLieu == duLieuChon) tt->chon = static_cast<int>(i);
    capNhatCuon(luoi, *tt);
    InvalidateRect(luoi, nullptr, FALSE);
}
