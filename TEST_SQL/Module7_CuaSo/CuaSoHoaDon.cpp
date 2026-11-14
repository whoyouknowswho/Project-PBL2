#include "CuaSo.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {
// Moi cua so giu ban sao hoa don va bang mon cua rieng no.
struct CuaSoHoaDon {
    HoaDon hoaDon;
    wstring khachHang, nhanVien;
    const BoFont* font = nullptr;
    HWND bangMon = nullptr, nutDong = nullptr;
    int rong = 900, cao = 700;
};

const int ID_BANG = 1, ID_DONG = 2;

void sapXep(HWND h, CuaSoHoaDon& cs) {
    RECT r;
    GetClientRect(h, &r);
    cs.rong = r.right;
    cs.cao = r.bottom;
    int rongBang = cs.rong - 96;
    MoveWindow(cs.bangMon, 48, 228, rongBang, max(100, cs.cao - 228 - 236), TRUE);
    int cotStt = 44, cotSl = 60, cotGia = 136, cotTien = 150;
    ListView_SetColumnWidth(cs.bangMon, 0, cotStt);
    ListView_SetColumnWidth(cs.bangMon, 1, max(180, rongBang - cotStt - cotSl - cotGia - cotTien - 22));
    ListView_SetColumnWidth(cs.bangMon, 2, cotSl);
    ListView_SetColumnWidth(cs.bangMon, 3, cotGia);
    ListView_SetColumnWidth(cs.bangMon, 4, cotTien);
    MoveWindow(cs.nutDong, cs.rong - 24 - 190, cs.cao - 58, 190, 42, TRUE);
    InvalidateRect(h, nullptr, FALSE);
}

void taoBang(HWND h, CuaSoHoaDon& cs) {
    cs.bangMon = CreateWindowExW(0, WC_LISTVIEWW, L"Chi tiết các món trong hóa đơn",
                                 WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                                 48, 228, 800, 220, h, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_BANG)),
                                 GetModuleHandleW(nullptr), nullptr);
    if (!cs.bangMon) throw runtime_error("Không tạo được bảng hóa đơn.");
    ganKieuBang(cs.bangMon, *cs.font, -1, 48);
    const vector<wstring> tenCot = {L"#", L"Món / mã món", L"SL", L"Đơn giá", L"Thành tiền"};
    for (size_t i = 0; i < tenCot.size(); i++) {
        LVCOLUMNW cot = {};
        cot.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
        cot.pszText = const_cast<wchar_t*>(tenCot[i].c_str());
        cot.cx = 130;
        cot.fmt = (i == 0 || i == 2) ? LVCFMT_CENTER : i >= 3 ? LVCFMT_RIGHT : LVCFMT_LEFT;
        ListView_InsertColumn(cs.bangMon, static_cast<int>(i), &cot);
    }
    const vector<ChiTietHoaDon>& gio = cs.hoaDon.getGioHang();
    for (size_t i = 0; i < gio.size(); i++) {
        const ChiTietHoaDon& ct = gio[i];
        vector<wstring> cacCot = {to_wstring(i + 1), rong(ct.tenMon), to_wstring(ct.soLuong), tien(ct.donGia),
                                  tien(ct.thanhTien())};
        LVITEMW dong = {};
        dong.mask = LVIF_TEXT;
        dong.iItem = static_cast<int>(i);
        dong.pszText = const_cast<wchar_t*>(cacCot[0].c_str());
        int viTri = ListView_InsertItem(cs.bangMon, &dong);
        for (size_t cot = 1; cot < cacCot.size(); cot++)
            ListView_SetItemText(cs.bangMon, viTri, static_cast<int>(cot), const_cast<wchar_t*>(cacCot[cot].c_str()));
    }
}

LRESULT veDong(NMLVCUSTOMDRAW* ve, CuaSoHoaDon& cs) {
    const BoFont& f = *cs.font;
    return veDongBang(ve, f, [&](HDC dc, int dong, int cot, RECT o, bool) {
        if (dong < 0 || static_cast<size_t>(dong) >= cs.hoaDon.getGioHang().size()) return false;
        const ChiTietHoaDon& ct = cs.hoaDon.getGioHang()[dong];
        if (cot == 1) {
            veChu(dc, rong(ct.tenMon), {o.left + 12, o.top + 4, o.right - 8, o.top + 26}, f.nut, Mau::CHU);
            veChu(dc, rong(ct.maMon), {o.left + 12, o.top + 25, o.right - 8, o.bottom - 3}, f.nho, Mau::CHU_MO);
            return true;
        }
        if (cot == 4) {
            veChu(dc, tien(ct.thanhTien()), {o.left + 8, o.top, o.right - 10, o.bottom}, f.nut, Mau::CHU, DT_RIGHT);
            return true;
        }
        if (cot == 0) {
            veChu(dc, to_wstring(dong + 1), o, f.thuong, Mau::CHU_MO, DT_CENTER);
            return true;
        }
        return false;
    });
}

void veO(HDC dc, const BoFont& f, const wstring& nhan, const wstring& giaTri, RECT r) {
    veChu(dc, nhan, {r.left, r.top, r.right, r.top + 20}, f.nhoDam, Mau::CHU_MO);
    veChu(dc, giaTri, {r.left, r.top + 22, r.right, r.bottom}, f.nut, Mau::CHU);
}

void veHoaDon(HDC dich, CuaSoHoaDon& cs) {
    const BoFont& f = *cs.font;
    int w = cs.rong, h = cs.cao;
    HDC dc = CreateCompatibleDC(dich);
    HBITMAP anh = CreateCompatibleBitmap(dich, max(1, w), max(1, h));
    HGDIOBJ anhCu = SelectObject(dc, anh);
    RECT toan = {0, 0, w, h};
    toNen(dc, toan, Mau::NEN);
    veThe(dc, {24, 20, w - 24, h - 72}, 18);

    // Dau bien lai.
    veVongTron(dc, 66, 62, 18, Mau::CHINH);
    veIcon(dc, Icon::NHA_HANG, {48, 44, 84, 80}, f.icon, RGB(255,255,255));
    veChu(dc, L"NHÀ HÀNG", {96, 42, 400, 62}, f.nhoDam, Mau::CHINH);
    veChu(dc, L"Hóa đơn bán hàng", {96, 60, w - 240, 96}, f.tieuDe, Mau::CHU);
    wstring trangThai = L"ĐÃ THANH TOÁN";
    int rongTT = doRongHuyHieu(dc, trangThai, f.nhoDam) + 8;
    veHuyHieu(dc, trangThai, {w - 48 - rongTT, 50, w - 48, 78}, Mau::TOT_NEN, Mau::TOT, f.nhoDam);

    // Luoi thong tin.
    RECT hop = {48, 112, w - 48, 180};
    veHinhBoGoc(dc, hop, 12, Mau::O_CHI_DOC, Mau::VIEN);
    int o4 = (hop.right - hop.left - 32) / 4;
    veO(dc, f, L"MÃ HÓA ĐƠN", rong(cs.hoaDon.getMa()), {hop.left + 16, hop.top + 12, hop.left + 16 + o4 - 8, hop.bottom - 10});
    veO(dc, f, L"NGÀY LẬP", rong(cs.hoaDon.getNgayLap()), {hop.left + 16 + o4, hop.top + 12, hop.left + 16 + 2 * o4 - 8, hop.bottom - 10});
    veO(dc, f, L"KHÁCH HÀNG", cs.khachHang, {hop.left + 16 + 2 * o4, hop.top + 12, hop.left + 16 + 3 * o4 - 8, hop.bottom - 10});
    veO(dc, f, L"NHÂN VIÊN", cs.nhanVien, {hop.left + 16 + 3 * o4, hop.top + 12, hop.right - 16, hop.bottom - 10});
    veChu(dc, L"CHI TIẾT ĐƠN HÀNG", {48, 196, 400, 220}, f.nhoDam, Mau::CHU_MO);

    // Tong ket.
    int y = h - 220;
    for (int x = 48; x < w - 48; x += 12) {
        RECT gach = {x, y - 8, min(x + 6, w - 48), y - 7};
        toNen(dc, gach, Mau::VIEN_O);
    }
    long long soPhan = 0;
    for (const ChiTietHoaDon& ct : cs.hoaDon.getGioHang()) soPhan += ct.soLuong;
    int traiTong = w - 400, phai = w - 48;
    veChu(dc, to_wstring(cs.hoaDon.getGioHang().size()) + L" món  ·  " + to_wstring(soPhan) + L" phần",
          {48, y + 4, traiTong - 24, y + 28}, f.nho, Mau::CHU_MO);
    veChu(dc, L"Cảm ơn quý khách!", {48, y + 48, traiTong - 24, y + 78}, f.dam, Mau::CHU);
    veChu(dc, L"Hẹn gặp lại trong lần tới.", {48, y + 80, traiTong - 24, y + 104}, f.nho, Mau::CHU_MO);
    veChu(dc, L"Tạm tính", {traiTong, y, phai, y + 28}, f.thuong, Mau::CHU_MO);
    veChu(dc, tien(cs.hoaDon.tamTinh()), {traiTong, y, phai, y + 28}, f.thuong, Mau::CHU, DT_RIGHT);
    wstring giam = tien(cs.hoaDon.tienGiam());
    if (cs.hoaDon.tienGiam() > 0) giam = L"− " + giam;
    veChu(dc, L"Giảm giá", {traiTong, y + 32, phai, y + 60}, f.thuong, Mau::CHU_MO);
    veChu(dc, giam, {traiTong, y + 32, phai, y + 60}, f.thuong, Mau::TOT, DT_RIGHT);
    RECT tong = {traiTong - 16, y + 72, phai + 8, y + 130};
    veHinhBoGoc(dc, tong, 12, Mau::PHU_NEN);
    veChu(dc, L"TỔNG TIỀN", {tong.left + 16, tong.top, tong.left + 140, tong.bottom}, f.nhoDam, Mau::PHU_CHU);
    veChu(dc, tien(cs.hoaDon.tongTien()), {tong.left + 120, tong.top, tong.right - 16, tong.bottom}, f.so, Mau::CHINH, DT_RIGHT);

    BitBlt(dich, 0, 0, w, h, dc, 0, 0, SRCCOPY);
    SelectObject(dc, anhCu);
    DeleteObject(anh);
    DeleteDC(dc);
}

LRESULT CALLBACK thuTucHoaDon(HWND h, UINT tin, WPARAM w, LPARAM l) {
    CuaSoHoaDon* cs = reinterpret_cast<CuaSoHoaDon*>(GetWindowLongPtrW(h, GWLP_USERDATA));
    if (tin == WM_NCCREATE) {
        cs = static_cast<CuaSoHoaDon*>(reinterpret_cast<CREATESTRUCTW*>(l)->lpCreateParams);
        SetWindowLongPtrW(h, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs));
    }
    if (!cs) return DefWindowProcW(h, tin, w, l);
    switch (tin) {
        case WM_CREATE:
            try { taoBang(h, *cs); }
            catch (const exception&) { return -1; }
            cs->nutDong = CreateWindowW(L"BUTTON", L"Đóng hóa đơn", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                                        680, 650, 190, 42, h, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_DONG)),
                                        GetModuleHandleW(nullptr), nullptr);
            sapXep(h, *cs);
            return 0;
        case WM_SIZE:
            if (w != SIZE_MINIMIZED && cs->bangMon) sapXep(h, *cs);
            return 0;
        case WM_NOTIFY: {
            NMHDR* n = reinterpret_cast<NMHDR*>(l);
            if (n->hwndFrom == cs->bangMon && n->code == NM_CUSTOMDRAW) return veDong(reinterpret_cast<NMLVCUSTOMDRAW*>(l), *cs);
            return 0;
        }
        case WM_DRAWITEM: {
            DRAWITEMSTRUCT* n = reinterpret_cast<DRAWITEMSTRUCT*>(l);
            toNen(n->hDC, n->rcItem, Mau::NEN);
            veHinhBoGoc(n->hDC, n->rcItem, 10, (n->itemState & ODS_SELECTED) ? Mau::CHINH_NHAN : Mau::CHINH);
            veChu(n->hDC, L"Đóng hóa đơn", n->rcItem, cs->font->nut, RGB(255,255,255), DT_CENTER);
            if (n->itemState & ODS_FOCUS) {
                RECT r = n->rcItem;
                InflateRect(&r, -3, -3);
                veVienBoGoc(n->hDC, r, 8, RGB(255,255,255), 1.5f, 170);
            }
            return TRUE;
        }
        case WM_COMMAND:
            if (LOWORD(w) == ID_DONG || LOWORD(w) == IDCANCEL || LOWORD(w) == IDOK) DestroyWindow(h);
            return 0;
        case WM_GETMINMAXINFO: {
            MINMAXINFO* kichThuoc = reinterpret_cast<MINMAXINFO*>(l);
            kichThuoc->ptMinTrackSize = {760, 650};
            return 0;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(h, &ps);
            veHoaDon(dc, *cs);
            EndPaint(h, &ps);
            return 0;
        }
        case WM_CLOSE:
            DestroyWindow(h);
            return 0;
    }
    return DefWindowProcW(h, tin, w, l);
}
}

void xemHoaDon(HWND chu, const HoaDon& hoaDon, const BoFont& font) {
    CuaSoHoaDon cs;
    cs.hoaDon = hoaDon;
    cs.font = &font;
    cs.khachHang = hoaDon.getMaKhachHang().empty() ? L"Khách lẻ" : rong(hoaDon.getMaKhachHang());
    cs.nhanVien = hoaDon.getMaNhanVien().empty() ? L"—" : rong(hoaDon.getMaNhanVien());
    WNDCLASSW lop = {};
    lop.lpfnWndProc = thuTucHoaDon;
    lop.hInstance = GetModuleHandleW(nullptr);
    lop.lpszClassName = L"NhaHangHoaDonMoi";
    lop.hCursor = LoadCursor(nullptr, IDC_ARROW);
    if (!RegisterClassW(&lop) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        throw runtime_error("Không đăng ký được cửa sổ hóa đơn.");
    RECT r;
    GetWindowRect(chu, &r);
    MONITORINFO manHinh = {};
    manHinh.cbSize = sizeof(manHinh);
    GetMonitorInfoW(MonitorFromWindow(chu, MONITOR_DEFAULTTONEAREST), &manHinh);
    int w = min(940, static_cast<int>(manHinh.rcWork.right - manHinh.rcWork.left - 32));
    int cao = min(770, static_cast<int>(manHinh.rcWork.bottom - manHinh.rcWork.top - 32));
    int x = max(static_cast<int>(manHinh.rcWork.left), (static_cast<int>(r.left + r.right) - w) / 2);
    int y = max(static_cast<int>(manHinh.rcWork.top), (static_cast<int>(r.top + r.bottom) - cao) / 2);
    x = min(x, static_cast<int>(manHinh.rcWork.right) - w);
    y = min(y, static_cast<int>(manHinh.rcWork.bottom) - cao);
    wstring tieuDe = L"Hóa đơn • " + rong(hoaDon.getMa());
    HWND h = CreateWindowExW(WS_EX_CONTROLPARENT, lop.lpszClassName, tieuDe.c_str(),
                             WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_CLIPCHILDREN,
                             x, y, w, cao, chu, nullptr, lop.hInstance, &cs);
    if (!h) throw runtime_error("Không mở được cửa sổ hóa đơn.");
    EnableWindow(chu, FALSE);
    ShowWindow(h, SW_SHOW);
    SetFocus(cs.nutDong);
    MSG tin = {};
    while (IsWindow(h)) {
        int ketQua = GetMessageW(&tin, nullptr, 0, 0);
        if (ketQua <= 0) {
            DestroyWindow(h);
            if (ketQua == 0) PostQuitMessage(static_cast<int>(tin.wParam));
            break;
        }
        if (!IsDialogMessageW(h, &tin)) {
            TranslateMessage(&tin);
            DispatchMessageW(&tin);
        }
    }
    if (IsWindow(chu)) {
        EnableWindow(chu, TRUE);
        SetForegroundWindow(chu);
    }
}
