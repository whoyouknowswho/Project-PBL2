#include "CuaSo.h"
#include <uxtheme.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

namespace {
bool laOSoThuc(UINT_PTR id) { return id == O_GIA || id == O_LUONG || id == O_PHUCAP; }

// Chi cho go chu so (va mot dau cham voi o so thuc, dau / voi o ngay).
LRESULT CALLBACK locKyTuNhapSo(HWND edit, UINT msg, WPARAM w, LPARAM l, UINT_PTR id, DWORD_PTR) {
    if (msg == WM_NCDESTROY) RemoveWindowSubclass(edit, locKyTuNhapSo, id);
    if (msg != WM_CHAR) return DefSubclassProc(edit, msg, w, l);
    wchar_t kyTu = static_cast<wchar_t>(w);
    if (kyTu < 32 || (kyTu >= L'0' && kyTu <= L'9')) return DefSubclassProc(edit, msg, w, l);
    if (laOSoThuc(id) && kyTu == L'.') {
        DWORD dau = 0, cuoi = 0;
        SendMessageW(edit, EM_GETSEL, reinterpret_cast<WPARAM>(&dau), reinterpret_cast<LPARAM>(&cuoi));
        int n = GetWindowTextLengthW(edit);
        wstring hienTai(static_cast<size_t>(n) + 1, L'\0');
        GetWindowTextW(edit, &hienTai[0], n + 1);
        hienTai.resize(n);
        size_t cham = hienTai.find(L'.');
        if (cham == wstring::npos || (cham >= dau && cham < cuoi)) return DefSubclassProc(edit, msg, w, l);
    }
    if (id == O_NGAY && kyTu == L'/') return DefSubclassProc(edit, msg, w, l);
    return 0;
}

struct KieuBang {
    HIMAGELIST khoangDong = nullptr;
    int cotHuyHieu = -1;
    const BoFont* font = nullptr;
};
map<HWND, KieuBang>& dsBang() {
    static map<HWND, KieuBang> ds;
    return ds;
}

// Bo dem ve: ve vao bitmap tam roi chep mot lan de khong nhap nhay.
struct BoDem {
    HDC dich, dc;
    HBITMAP anh;
    HGDIOBJ cu;
    RECT vung;
    BoDem(HDC d, RECT r) : dich(d), vung(r) {
        dc = CreateCompatibleDC(d);
        anh = CreateCompatibleBitmap(d, max(1, static_cast<int>(r.right - r.left)), max(1, static_cast<int>(r.bottom - r.top)));
        cu = SelectObject(dc, anh);
        SetViewportOrgEx(dc, -r.left, -r.top, nullptr);
    }
    ~BoDem() {
        SetViewportOrgEx(dc, 0, 0, nullptr);
        BitBlt(dich, vung.left, vung.top, vung.right - vung.left, vung.bottom - vung.top, dc, 0, 0, SRCCOPY);
        SelectObject(dc, cu);
        DeleteObject(anh);
        DeleteDC(dc);
    }
};

// Tieu de bang tu ve: nen kem nhat, chu in hoa nho.
LRESULT CALLBACK thuTucTieuDe(HWND h, UINT msg, WPARAM w, LPARAM l, UINT_PTR id, DWORD_PTR thamSo) {
    const BoFont* font = reinterpret_cast<const BoFont*>(thamSo);
    switch (msg) {
        case HDM_LAYOUT: {
            LRESULT kq = DefSubclassProc(h, msg, w, l);
            HDLAYOUT* bo = reinterpret_cast<HDLAYOUT*>(l);
            if (bo && bo->pwpos && bo->prc) {
                bo->pwpos->cy = 38;
                bo->prc->top = bo->pwpos->y + 38;
            }
            return kq;
        }
        case WM_ERASEBKGND: return 1;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(h, &ps);
            RECT r;
            GetClientRect(h, &r);
            {
                BoDem dem(dc, r);
                toNen(dem.dc, r, Mau::O_CHI_DOC);
                int n = Header_GetItemCount(h);
                for (int i = 0; i < n; i++) {
                    RECT o;
                    Header_GetItemRect(h, i, &o);
                    wchar_t chu[160] = {};
                    HDITEMW muc = {};
                    muc.mask = HDI_TEXT | HDI_FORMAT;
                    muc.pszText = chu;
                    muc.cchTextMax = 160;
                    Header_GetItem(h, i, &muc);
                    wstring noiDung = chu;
                    if (!noiDung.empty()) CharUpperBuffW(&noiDung[0], static_cast<DWORD>(noiDung.size()));
                    UINT can = (muc.fmt & HDF_RIGHT) ? DT_RIGHT : (muc.fmt & HDF_CENTER) ? DT_CENTER : DT_LEFT;
                    o.left += 12;
                    o.right -= 8;
                    veChu(dem.dc, noiDung, o, font->nhoDam, Mau::CHU_MO, can);
                }
                RECT day = {r.left, r.bottom - 1, r.right, r.bottom};
                toNen(dem.dc, day, Mau::VIEN);
            }
            EndPaint(h, &ps);
            return 0;
        }
        case WM_NCDESTROY:
            RemoveWindowSubclass(h, thuTucTieuDe, id);
            break;
    }
    return DefSubclassProc(h, msg, w, l);
}

LRESULT CALLBACK thuTucBang(HWND h, UINT msg, WPARAM w, LPARAM l, UINT_PTR id, DWORD_PTR) {
    if (msg == WM_NCDESTROY) {
        auto it = dsBang().find(h);
        if (it != dsBang().end()) {
            ListView_SetImageList(h, nullptr, LVSIL_SMALL);
            if (it->second.khoangDong) ImageList_Destroy(it->second.khoangDong);
            dsBang().erase(it);
        }
        RemoveWindowSubclass(h, thuTucBang, id);
    }
    return DefSubclassProc(h, msg, w, l);
}

bool laSo(const wstring& s) {
    if (s.empty()) return false;
    for (wchar_t c : s) if (c < L'0' || c > L'9') return false;
    return true;
}
}

// ---------------------------------------------------------------- Bang dung chung

void ganKieuBang(HWND lv, const BoFont& font, int cotHuyHieu, int chieuCaoDong) {
    ListView_SetExtendedListViewStyle(lv, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP);
    ListView_SetBkColor(lv, Mau::THE);
    ListView_SetTextBkColor(lv, Mau::THE);
    ListView_SetTextColor(lv, Mau::CHU);
    SendMessageW(lv, WM_SETFONT, reinterpret_cast<WPARAM>(font.thuong), TRUE);
    KieuBang kieu;
    kieu.cotHuyHieu = cotHuyHieu;
    kieu.font = &font;
    kieu.khoangDong = ImageList_Create(1, chieuCaoDong, ILC_COLOR32, 1, 1);
    if (kieu.khoangDong) ListView_SetImageList(lv, kieu.khoangDong, LVSIL_SMALL);
    dsBang()[lv] = kieu;
    SetWindowSubclass(lv, thuTucBang, 1, 0);
    HWND tieuDe = ListView_GetHeader(lv);
    if (tieuDe) {
        SendMessageW(tieuDe, WM_SETFONT, reinterpret_cast<WPARAM>(font.nhoDam), TRUE);
        SetWindowSubclass(tieuDe, thuTucTieuDe, 1, reinterpret_cast<DWORD_PTR>(&font));
    }
}

bool laBangTuyChinh(HWND h) { return dsBang().count(h) != 0; }

void mauHuyHieu(const wstring& chu, COLORREF& nen, COLORREF& mauChu) {
    nen = Mau::PHU_NEN;
    mauChu = Mau::PHU_CHU;
    if (chu == L"Hết hàng" || chu == L"Đã hết món") { nen = Mau::LOI_NEN; mauChu = Mau::LOI; }
    else if (chu == L"Đã hủy" || chu == L"Bạc") { nen = Mau::XAM_NEN; mauChu = Mau::CHU_NHAN; }
    else if (chu == L"Sắp hết" || chu == L"Chờ xử lý" || chu == L"Vàng" || laSo(chu)) {
        nen = Mau::CANH_BAO_NEN; mauChu = Mau::CANH_BAO;
    }
    else if (chu == L"Đã thanh toán") { nen = Mau::TOT_NEN; mauChu = Mau::TOT; }
    else if (chu == L"Kim cương") { nen = RGB(232,240,252); mauChu = RGB(52,92,170); }
}

LRESULT veDongBang(NMLVCUSTOMDRAW* ve, const BoFont& font, const VeOTuyChinh& veRieng) {
    if (ve->nmcd.dwDrawStage == CDDS_PREPAINT) return CDRF_NOTIFYITEMDRAW;
    if (ve->nmcd.dwDrawStage != CDDS_ITEMPREPAINT) return CDRF_DODEFAULT;
    HWND lv = ve->nmcd.hdr.hwndFrom;
    HDC dc = ve->nmcd.hdc;
    int dong = static_cast<int>(ve->nmcd.dwItemSpec);
    auto it = dsBang().find(lv);
    int cotHuyHieu = it == dsBang().end() ? -1 : it->second.cotHuyHieu;
    RECT hang;
    if (!ListView_GetItemRect(lv, dong, &hang, LVIR_BOUNDS)) return CDRF_DODEFAULT;
    bool daChon = (ListView_GetItemState(lv, dong, LVIS_SELECTED) & LVIS_SELECTED) != 0;
    toNen(dc, hang, daChon ? Mau::PHU_NEN : (dong % 2 ? Mau::DONG_XEN : Mau::THE));
    RECT ke = {hang.left, hang.bottom - 1, hang.right, hang.bottom};
    toNen(dc, ke, RGB(244,238,231));
    if (daChon) {
        RECT vach = {hang.left, hang.top, hang.left + 3, hang.bottom};
        toNen(dc, vach, Mau::CHINH);
    }
    bool mo = ve->nmcd.lItemlParam == -1;
    int soCot = Header_GetItemCount(ListView_GetHeader(lv));
    for (int c = 0; c < soCot; c++) {
        RECT o;
        ListView_GetSubItemRect(lv, dong, c, LVIR_BOUNDS, &o);
        if (c == 0) o.right = o.left + ListView_GetColumnWidth(lv, 0);
        if (veRieng && veRieng(dc, dong, c, o, daChon)) continue;
        wchar_t dem[512] = {};
        ListView_GetItemText(lv, dong, c, dem, 512);
        wstring noiDung = dem;
        if (noiDung.empty()) continue;
        LVCOLUMNW cot = {};
        cot.mask = LVCF_FMT;
        ListView_GetColumn(lv, c, &cot);
        int fmt = cot.fmt & LVCFMT_JUSTIFYMASK;
        UINT can = fmt == LVCFMT_RIGHT ? DT_RIGHT : fmt == LVCFMT_CENTER ? DT_CENTER : DT_LEFT;
        RECT chuR = o;
        chuR.left += 12;
        chuR.right -= 8;
        if (c == cotHuyHieu && !mo) {
            COLORREF nen, mauChu;
            mauHuyHieu(noiDung, nen, mauChu);
            int w = min(doRongHuyHieu(dc, noiDung, font.nhoDam), static_cast<int>(chuR.right - chuR.left));
            int giua = (o.top + o.bottom) / 2;
            RECT b = {chuR.left, giua - 12, chuR.left + w, giua + 12};
            if (can == DT_RIGHT) { b.right = chuR.right; b.left = chuR.right - w; }
            if (can == DT_CENTER) { b.left = (chuR.left + chuR.right - w) / 2; b.right = b.left + w; }
            veHuyHieu(dc, noiDung, b, nen, mauChu, font.nhoDam);
        } else {
            veChu(dc, noiDung, chuR, font.thuong, mo ? Mau::CHU_MO : Mau::CHU, can);
        }
    }
    return CDRF_SKIPDEFAULT;
}

// ---------------------------------------------------------------- Truy cap dieu khien

HWND CuaSoNhaHang::o(int id) const { return GetDlgItem(cuaSo, id); }

wstring CuaSoNhaHang::chu(int id) const {
    HWND h = o(id);
    if (!h) return L"";
    int n = GetWindowTextLengthW(h);
    wstring ketQua(static_cast<size_t>(n) + 1, L'\0');
    GetWindowTextW(h, &ketQua[0], n + 1);
    ketQua.resize(n);
    return ketQua;
}

string CuaSoNhaHang::duLieu(int id) const { return hep(chu(id)); }

void CuaSoNhaHang::gan(int id, const wstring& giaTri) {
    HWND h = o(id);
    if (h) SetWindowTextW(h, giaTri.c_str());
}

int CuaSoNhaHang::luaChon(int id) const { return static_cast<int>(SendMessageW(o(id), CB_GETCURSEL, 0, 0)); }

void CuaSoNhaHang::chonMuc(int id, int viTri) {
    SendMessageW(o(id), CB_SETCURSEL, viTri, 0);
    InvalidateRect(o(id), nullptr, FALSE);
}

bool CuaSoNhaHang::daDanhDau(int id) const {
    auto it = danhDauGiaTri.find(id);
    return it != danhDauGiaTri.end() && it->second;
}

void CuaSoNhaHang::datDanhDau(int id, bool giaTri) {
    danhDauGiaTri[id] = giaTri;
    if (o(id)) InvalidateRect(o(id), nullptr, FALSE);
}

double CuaSoNhaHang::soNhap(int id, const string& ten, bool nguyen) const {
    string chuoi = duLieu(id);
    size_t dau = chuoi.find_first_not_of(" \t"), cuoi = chuoi.find_last_not_of(" \t");
    chuoi = dau == string::npos ? "" : chuoi.substr(dau, cuoi - dau + 1);
    if (chuoi.empty()) throw runtime_error(ten + ": không được để trống.");
    // Chi nhan chu so (va mot dau cham thap phan neu khong phai so nguyen):
    // chan cac dang stod van chap nhan nhu 1e5, 0x10, +5, -3, inf.
    int soDauCham = 0;
    for (char c : chuoi) {
        if (c == '.') soDauCham++;
        else if (c < '0' || c > '9') throw runtime_error(ten + ": chỉ được nhập chữ số.");
    }
    if (soDauCham > (nguyen ? 0 : 1) || chuoi.front() == '.' || chuoi.back() == '.')
        throw runtime_error(nguyen ? ten + ": phải là số nguyên." : ten + ": số không hợp lệ.");
    if (chuoi.size() > 15) throw runtime_error(ten + ": số quá lớn.");
    double giaTri = stod(chuoi);
    if (giaTri > 1000000000) throw runtime_error(ten + ": số không hợp lệ (0 đến 1.000.000.000).");
    return giaTri;
}

// ---------------------------------------------------------------- Tao dieu khien

HWND CuaSoNhaHang::tao(const wchar_t* lop, const wstring& noiDung, DWORD kieu,
                      int id, int x, int y, int w, int h, DWORD moRong) {
    HWND dieuKhien = CreateWindowExW(moRong, lop, noiDung.c_str(), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | kieu,
                                     x, y, w, h, cuaSo, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                                     GetModuleHandleW(nullptr), nullptr);
    if (!dieuKhien) throw runtime_error("Không tạo được thành phần giao diện.");
    SendMessageW(dieuKhien, WM_SETFONT, reinterpret_cast<WPARAM>(font.thuong), TRUE);
    return dieuKhien;
}

HWND CuaSoNhaHang::nhan(const wstring& noiDung, int x, int y, int w, bool dam, int id, int h) {
    HWND nhanMoi = tao(L"STATIC", noiDung, SS_LEFTNOWORDWRAP | SS_ENDELLIPSIS | SS_NOPREFIX | SS_CENTERIMAGE,
                       id, x, y, w, h);
    SendMessageW(nhanMoi, WM_SETFONT, reinterpret_cast<WPARAM>(dam ? font.dam : font.nho), TRUE);
    if (dam) mauChuNhan[nhanMoi] = Mau::CHU;
    return nhanMoi;
}

HWND CuaSoNhaHang::nhap(const wstring& ten, int id, int x, int y, int w, const wstring& giaTri, bool matKhau) {
    if (!ten.empty()) {
        HWND nhanO = nhan(ten, x, y, w);
        SendMessageW(nhanO, WM_SETFONT, reinterpret_cast<WPARAM>(font.nhoDam), TRUE);
        mauChuNhan[nhanO] = Mau::CHU_NHAN;
    }
    khungNhap[id] = {x, y + 24, x + w, y + 64};
    DWORD kieu = WS_TABSTOP | ES_AUTOHSCROLL;
    if (matKhau) kieu |= ES_PASSWORD;
    if (id == O_SDT || id == O_DIEM || id == O_TON || id == O_CAY || id == O_NHAPKHO || id == O_SOLUONG)
        kieu |= ES_NUMBER;
    int le = id == TIM_KIEM ? 40 : 12;
    HWND h = tao(L"EDIT", giaTri, kieu, id, x + le, y + 24 + 10, w - le - 12, 20);
    SendMessageW(h, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 0);
    if (id == O_GIA || id == O_LUONG || id == O_PHUCAP || id == O_NGAY)
        SetWindowSubclass(h, locKyTuNhapSo, static_cast<UINT_PTR>(id), 0);
    UINT gioiHan = matKhau ? 128u : id == O_USER ? 32u : id == O_SDT ? 15u : id == O_EMAIL ? 254u : 160u;
    SendMessageW(h, EM_SETLIMITTEXT, gioiHan, 0);
    if (matKhau) SendMessageW(h, EM_SETPASSWORDCHAR, L'*', 0);
    return h;
}

HWND CuaSoNhaHang::chon(const wstring& ten, int id, int x, int y, int w, const vector<wstring>& cacMuc) {
    if (!ten.empty()) {
        HWND nhanO = nhan(ten, x, y, w);
        SendMessageW(nhanO, WM_SETFONT, reinterpret_cast<WPARAM>(font.nhoDam), TRUE);
        mauChuNhan[nhanO] = Mau::CHU_NHAN;
    }
    HWND h = tao(WC_COMBOBOXW, L"", WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS,
                 id, x, y + 24, w, 320);
    for (const wstring& muc : cacMuc) SendMessageW(h, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(muc.c_str()));
    SendMessageW(h, CB_SETCURSEL, 0, 0);
    SetWindowSubclass(h, thuTucHop, 1, reinterpret_cast<DWORD_PTR>(this));
    return h;
}

HWND CuaSoNhaHang::nut(const wstring& noiDung, int id, int x, int y, int w, KieuNut kieu, wchar_t icon, int h) {
    HWND n = tao(L"BUTTON", noiDung, WS_TABSTOP | BS_OWNERDRAW, id, x, y, w, h);
    kieuNut[n] = kieu;
    if (icon) iconNut[n] = icon;
    SetWindowSubclass(n, thuTucNut, 1, reinterpret_cast<DWORD_PTR>(this));
    return n;
}

void CuaSoNhaHang::datKieuNut(int id, KieuNut kieu) {
    HWND h = o(id);
    if (!h) return;
    kieuNut[h] = kieu;
    InvalidateRect(h, nullptr, FALSE);
}

void CuaSoNhaHang::danhDau(const wstring& ten, int id, int x, int y, int w, bool giaTri) {
    tao(L"BUTTON", ten, WS_TABSTOP | BS_OWNERDRAW, id, x, y, w, 28);
    SetWindowSubclass(o(id), thuTucNut, 1, reinterpret_cast<DWORD_PTR>(this));
    danhDauGiaTri[id] = giaTri;
}

HWND CuaSoNhaHang::bang(int id, int x, int y, int w, int h, const vector<pair<wstring, int>>& cacCot, int cotHuyHieu) {
    HWND lv = tao(WC_LISTVIEWW, L"", WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                  id, x, y, w, max(1, h - 1));
    ganKieuBang(lv, font, cotHuyHieu);
    int tong = 0;
    for (const auto& cot : cacCot) tong += max(1, cot.second);
    int rongDung = max(100, w - 22);
    for (size_t i = 0; i < cacCot.size(); i++) {
        LVCOLUMNW cot = {};
        cot.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        cot.pszText = const_cast<wchar_t*>(cacCot[i].first.c_str());
        cot.cx = max(1, cacCot[i].second) * rongDung / max(1, tong);
        cot.iSubItem = static_cast<int>(i);
        ListView_InsertColumn(lv, static_cast<int>(i), &cot);
    }
    SetWindowPos(lv, nullptr, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
    return lv;
}

void CuaSoNhaHang::dongBang(int id, const vector<wstring>& cacCot, int giaTri) {
    HWND lv = o(id);
    LVITEMW muc = {};
    muc.mask = LVIF_TEXT | LVIF_PARAM;
    muc.iItem = ListView_GetItemCount(lv);
    muc.pszText = const_cast<wchar_t*>(cacCot[0].c_str());
    muc.lParam = giaTri;
    int dong = ListView_InsertItem(lv, &muc);
    for (size_t i = 1; i < cacCot.size(); i++)
        ListView_SetItemText(lv, dong, static_cast<int>(i), const_cast<wchar_t*>(cacCot[i].c_str()));
}

void CuaSoNhaHang::canCot(int id, int cot, int kieuCan) {
    LVCOLUMNW thongTin = {};
    thongTin.mask = LVCF_FMT;
    thongTin.fmt = kieuCan;
    ListView_SetColumn(o(id), cot, &thongTin);
}

int CuaSoNhaHang::dongChon(int id) const {
    HWND lv = o(id);
    int dong = ListView_GetNextItem(lv, -1, LVNI_SELECTED);
    if (dong < 0) return -1;
    LVITEMW muc = {};
    muc.mask = LVIF_PARAM;
    muc.iItem = dong;
    if (!ListView_GetItem(lv, &muc)) return -1;
    return static_cast<int>(muc.lParam);
}

void CuaSoNhaHang::xoaDieuKhien() {
    vector<HWND> ds;
    for (HWND h = GetWindow(cuaSo, GW_CHILD); h; h = GetWindow(h, GW_HWNDNEXT)) ds.push_back(h);
    for (HWND h : ds) DestroyWindow(h);
    kieuNut.clear();
    iconNut.clear();
    mauChuNhan.clear();
    khungNhap.clear();
    danhDauGiaTri.clear();
    cacThe.clear();
    cacDauThe.clear();
    nutDangRe = nullptr;
    oDangChon = -1;
}

void CuaSoNhaHang::themThe(RECT vung) { cacThe.push_back(vung); }

void CuaSoNhaHang::themDauThe(int x, int y, int w, wchar_t icon, const wstring& tieuDe, const wstring& phu) {
    cacDauThe.push_back({x, y, w, icon, tieuDe, phu});
}

void CuaSoNhaHang::veDauThe(HDC dc) {
    for (const DauThe& d : cacDauThe) {
        int chuX = d.x;
        if (d.icon) {
            veVongTron(dc, d.x + 20, d.y + 20, 20, Mau::PHU_NEN);
            veIcon(dc, d.icon, {d.x, d.y, d.x + 40, d.y + 40}, font.icon, Mau::CHINH);
            chuX = d.x + 52;
        }
        if (d.phu.empty()) {
            veChu(dc, d.tieuDe, {chuX, d.y, d.x + d.w, d.y + 40}, font.dam, Mau::CHU);
        } else {
            veChu(dc, d.tieuDe, {chuX, d.y - 1, d.x + d.w, d.y + 21}, font.dam, Mau::CHU);
            veChu(dc, d.phu, {chuX, d.y + 21, d.x + d.w, d.y + 41}, font.nho, Mau::CHU_MO);
        }
    }
}

bool CuaSoNhaHang::trongThe(RECT r) const {
    for (const RECT& the : cacThe)
        if (r.left >= the.left && r.top >= the.top && r.right <= the.right && r.bottom <= the.bottom) return true;
    return false;
}

COLORREF CuaSoNhaHang::mauNenTai(HWND con) const {
    auto it = kieuNut.find(con);
    if (it != kieuNut.end() && it->second == KieuNut::ThanhBen) return Mau::THANH_BEN;
    RECT r;
    GetWindowRect(con, &r);
    MapWindowPoints(nullptr, cuaSo, reinterpret_cast<POINT*>(&r), 2);
    return trongThe(r) ? Mau::THE : Mau::NEN;
}

// ---------------------------------------------------------------- Ve dieu khien

void CuaSoNhaHang::veKhungNhap(HDC dc) {
    for (const auto& muc : khungNhap) {
        HWND e = o(muc.first);
        if (!e || !(GetWindowLongPtrW(e, GWL_STYLE) & WS_VISIBLE)) continue;
        bool chiDoc = !IsWindowEnabled(e) || (GetWindowLongPtrW(e, GWL_STYLE) & ES_READONLY);
        bool tieuDiem = oDangChon == muc.first && !chiDoc;
        RECT r = muc.second;
        if (tieuDiem) {
            RECT ngoai = r;
            InflateRect(&ngoai, 3, 3);
            veVienBoGoc(dc, ngoai, 13, Mau::CHINH, 3.0f, 45);
        }
        veHinhBoGoc(dc, r, 10, chiDoc ? Mau::O_CHI_DOC : Mau::THE, tieuDiem ? Mau::CHINH : Mau::VIEN_O,
                    tieuDiem ? 1.6f : 1.0f);
        if (muc.first == TIM_KIEM)
            veIcon(dc, Icon::TIM, {r.left + 10, r.top, r.left + 34, r.bottom}, font.icon,
                   tieuDiem ? Mau::CHINH : Mau::CHU_MO);
    }
}

void CuaSoNhaHang::veNut(const DRAWITEMSTRUCT* ve) {
    HWND h = ve->hwndItem;
    int id = static_cast<int>(ve->CtlID);
    RECT r = ve->rcItem;
    bool nhan = (ve->itemState & ODS_SELECTED) != 0;
    bool khoa = (ve->itemState & ODS_DISABLED) != 0;
    bool re = nutDangRe == h && !khoa;
    bool tieuDiem = (ve->itemState & ODS_FOCUS) && !(ve->itemState & ODS_NOFOCUSRECT);
    int doDai = GetWindowTextLengthW(h);
    wstring noiDung(static_cast<size_t>(doDai) + 1, L'\0');
    GetWindowTextW(h, &noiDung[0], doDai + 1);
    noiDung.resize(doDai);
    BoDem dem(ve->hDC, r);
    HDC dc = dem.dc;
    toNen(dc, r, mauNenTai(h));

    if (danhDauGiaTri.count(id)) {
        bool co = danhDauGiaTri[id];
        int giua = (r.top + r.bottom) / 2;
        RECT o = {r.left + 2, giua - 10, r.left + 22, giua + 10};
        if (co) {
            veHinhBoGoc(dc, o, 6, khoa ? Mau::CHU_KHOA : (re ? Mau::CHINH_RE : Mau::CHINH));
            veIcon(dc, Icon::DANH_DAU, o, font.icon, RGB(255,255,255));
        } else {
            veHinhBoGoc(dc, o, 6, Mau::THE, re ? Mau::CHINH : Mau::VIEN_O, 1.5f);
        }
        if (tieuDiem) {
            RECT ngoai = o;
            InflateRect(&ngoai, 3, 3);
            veVienBoGoc(dc, ngoai, 8, Mau::CHINH, 2.0f, 90);
        }
        RECT chuR = {r.left + 32, r.top, r.right, r.bottom};
        veChu(dc, noiDung, chuR, font.thuong, khoa ? Mau::CHU_KHOA : Mau::CHU);
        return;
    }

    KieuNut kieu = kieuNut.count(h) ? kieuNut[h] : KieuNut::Phu;
    wchar_t icon = iconNut.count(h) ? iconNut[h] : 0;
    if (kieu == KieuNut::ThanhBen) {
        bool dangO = id == trangHienTai;
        // Muc dang chon: nen trang vien manh + vach nhan ben trai; re chuot: nen dam hon thanh ben mot bac.
        COLORREF mau = (dangO || re) ? Mau::CHU : Mau::CHU_THANH_BEN;
        if (dangO) {
            veHinhBoGoc(dc, r, 10, Mau::THE, Mau::VIEN);
            RECT vach = {r.left + 1, r.top + 8, r.left + 4, r.bottom - 8};
            veHinhBoGoc(dc, vach, 1, Mau::CHINH);
        } else if (re || nhan) {
            veHinhBoGoc(dc, r, 10, Mau::THANH_BEN_RE);
        }
        RECT oIcon = {r.left + 12, r.top, r.left + 40, r.bottom};
        veIcon(dc, icon, oIcon, font.icon, dangO ? Mau::CHINH : mau);
        RECT chuR = {r.left + 50, r.top, r.right - 10, r.bottom};
        veChu(dc, noiDung, chuR, dangO ? font.nut : font.thuong, mau);
        return;
    }

    COLORREF nen = Mau::PHU_NEN, mau = Mau::PHU_CHU, vien = CLR_INVALID;
    int banKinh = 10;
    switch (kieu) {
        case KieuNut::Chinh:
            nen = nhan ? Mau::CHINH_NHAN : re ? Mau::CHINH_RE : Mau::CHINH;
            mau = RGB(255,255,255);
            break;
        case KieuNut::NguyHiem:
            nen = nhan ? RGB(245,200,194) : re ? Mau::LOI_RE : Mau::LOI_NEN;
            mau = Mau::LOI;
            break;
        case KieuNut::Tab: case KieuNut::Chip:
            nen = re || nhan ? Mau::O_CHI_DOC : Mau::THE;
            mau = Mau::CHU_NHAN;
            vien = re ? Mau::CHINH : Mau::VIEN_O;
            break;
        case KieuNut::TabChon: case KieuNut::ChipChon:
            nen = re || nhan ? Mau::CHINH_RE : Mau::CHINH;
            mau = RGB(255,255,255);
            break;
        default:
            nen = nhan ? Mau::PHU_NHAN : re ? Mau::PHU_RE : Mau::PHU_NEN;
            mau = Mau::PHU_CHU;
            break;
    }
    if (kieu == KieuNut::Chip || kieu == KieuNut::ChipChon) banKinh = (r.bottom - r.top) / 2;
    if (khoa) { nen = Mau::NUT_KHOA; mau = Mau::CHU_KHOA; vien = CLR_INVALID; }
    veHinhBoGoc(dc, r, banKinh, nen, vien, 1.0f);
    HFONT f = (kieu == KieuNut::Chip || kieu == KieuNut::ChipChon) ? font.nhoDam : font.nut;
    int rongChu = doChu(dc, noiDung, f).cx;
    int rongIcon = icon && coFontIcon() ? (noiDung.empty() ? 16 : 24) : 0;
    int bd = (r.left + r.right - rongChu - rongIcon) / 2;
    bd = max(static_cast<int>(r.left) + 10, bd);
    if (rongIcon) {
        RECT oIcon = {bd, r.top, bd + 16, r.bottom};
        veIcon(dc, icon, oIcon, font.icon, mau);
    }
    RECT chuR = {bd + rongIcon, r.top, r.right - 8, r.bottom};
    veChu(dc, noiDung, chuR, f, mau);
    if (tieuDiem) {
        RECT ngoai = r;
        InflateRect(&ngoai, -2, -2);
        veVienBoGoc(dc, ngoai, max(2, banKinh - 2), kieu == KieuNut::Chinh || kieu == KieuNut::TabChon ||
                    kieu == KieuNut::ChipChon ? RGB(255,255,255) : Mau::CHINH, 1.5f, 170);
    }
}

void CuaSoNhaHang::veHopChon(HWND hop, HDC dc) {
    RECT r;
    GetClientRect(hop, &r);
    toNen(dc, r, mauNenTai(hop));
    bool bat = IsWindowEnabled(hop) != FALSE;
    bool tieuDiem = GetFocus() == hop || SendMessageW(hop, CB_GETDROPPEDSTATE, 0, 0);
    if (tieuDiem && bat) {
        RECT trong = r;
        veHinhBoGoc(dc, trong, 10, Mau::THE, Mau::CHINH, 1.6f);
    } else {
        veHinhBoGoc(dc, r, 10, bat ? Mau::THE : Mau::O_CHI_DOC, Mau::VIEN_O, 1.0f);
    }
    int i = static_cast<int>(SendMessageW(hop, CB_GETCURSEL, 0, 0));
    wstring noiDung;
    if (i >= 0) {
        int n = static_cast<int>(SendMessageW(hop, CB_GETLBTEXTLEN, i, 0));
        if (n > 0) {
            noiDung.assign(static_cast<size_t>(n) + 1, L'\0');
            SendMessageW(hop, CB_GETLBTEXT, i, reinterpret_cast<LPARAM>(&noiDung[0]));
            noiDung.resize(n);
        }
    }
    RECT chuR = {r.left + 12, r.top, r.right - 36, r.bottom};
    veChu(dc, noiDung, chuR, font.thuong, bat ? Mau::CHU : Mau::CHU_KHOA);
    RECT muiTen = {r.right - 34, r.top, r.right - 10, r.bottom};
    veIcon(dc, Icon::MUI_TEN_XUONG, muiTen, font.icon, bat ? Mau::CHU_MO : Mau::CHU_KHOA);
}

void CuaSoNhaHang::veMucHop(const DRAWITEMSTRUCT* ve) {
    if (ve->itemState & ODS_COMBOBOXEDIT) {
        InvalidateRect(ve->hwndItem, nullptr, FALSE);
        return;
    }
    if (ve->itemID == static_cast<UINT>(-1)) return;
    RECT r = ve->rcItem;
    bool chon = (ve->itemState & ODS_SELECTED) != 0;
    toNen(ve->hDC, r, chon ? Mau::PHU_NEN : Mau::THE);
    UINT msgLen = ve->CtlType == ODT_LISTBOX ? LB_GETTEXTLEN : CB_GETLBTEXTLEN;
    UINT msgText = ve->CtlType == ODT_LISTBOX ? LB_GETTEXT : CB_GETLBTEXT;
    int n = static_cast<int>(SendMessageW(ve->hwndItem, msgLen, ve->itemID, 0));
    if (n <= 0) return;
    wstring noiDung(static_cast<size_t>(n) + 1, L'\0');
    SendMessageW(ve->hwndItem, msgText, ve->itemID, reinterpret_cast<LPARAM>(&noiDung[0]));
    noiDung.resize(n);
    RECT chuR = {r.left + 12, r.top, r.right - 8, r.bottom};
    veChu(ve->hDC, noiDung, chuR, font.thuong, chon ? Mau::PHU_CHU : Mau::CHU);
}

LRESULT CALLBACK CuaSoNhaHang::thuTucNut(HWND h, UINT msg, WPARAM w, LPARAM l, UINT_PTR id, DWORD_PTR thamSo) {
    CuaSoNhaHang* app = reinterpret_cast<CuaSoNhaHang*>(thamSo);
    switch (msg) {
        case WM_MOUSEMOVE:
            if (app->nutDangRe != h) {
                HWND cu = app->nutDangRe;
                app->nutDangRe = h;
                if (cu && IsWindow(cu)) InvalidateRect(cu, nullptr, FALSE);
                InvalidateRect(h, nullptr, FALSE);
                TRACKMOUSEEVENT theoDoi = {sizeof(theoDoi), TME_LEAVE, h, 0};
                TrackMouseEvent(&theoDoi);
            }
            break;
        case WM_MOUSELEAVE:
            if (app->nutDangRe == h) {
                app->nutDangRe = nullptr;
                InvalidateRect(h, nullptr, FALSE);
            }
            break;
        case WM_SETCURSOR:
            if (IsWindowEnabled(h)) {
                SetCursor(LoadCursor(nullptr, IDC_HAND));
                return TRUE;
            }
            break;
        case WM_LBUTTONDBLCLK:
            // Nhan nhanh hai lan van tinh la hai lan bam (vd. nut +/- so luong).
            return DefSubclassProc(h, WM_LBUTTONDOWN, w, l);
        case WM_ERASEBKGND:
            return 1;
        case WM_NCDESTROY:
            if (app->nutDangRe == h) app->nutDangRe = nullptr;
            RemoveWindowSubclass(h, thuTucNut, id);
            break;
    }
    return DefSubclassProc(h, msg, w, l);
}

LRESULT CALLBACK CuaSoNhaHang::thuTucHop(HWND h, UINT msg, WPARAM w, LPARAM l, UINT_PTR id, DWORD_PTR thamSo) {
    CuaSoNhaHang* app = reinterpret_cast<CuaSoNhaHang*>(thamSo);
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(h, &ps);
            RECT r;
            GetClientRect(h, &r);
            {
                BoDem dem(dc, r);
                app->veHopChon(h, dem.dc);
            }
            EndPaint(h, &ps);
            return 0;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_SETFOCUS: case WM_KILLFOCUS: case WM_ENABLE: case CB_SETCURSEL: {
            LRESULT kq = DefSubclassProc(h, msg, w, l);
            InvalidateRect(h, nullptr, FALSE);
            return kq;
        }
        case WM_NCDESTROY:
            RemoveWindowSubclass(h, thuTucHop, id);
            break;
    }
    return DefSubclassProc(h, msg, w, l);
}
