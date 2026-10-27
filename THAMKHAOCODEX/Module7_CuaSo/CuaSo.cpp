#include "CuaSo.h"
#include "Logo.h"
#include <uxtheme.h>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {
const COLORREF NEN = RGB(245,247,251), CHU = RGB(26,40,64), MO = RGB(108,122,144);
const COLORREF XANH = RGB(40,104,222), THANH_BEN = RGB(17,30,52);
void chuVe(HDC dc, const wstring& chu, RECT vung, HFONT font, COLORREF mau, UINT can = DT_LEFT) {
    SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, mau);
    DrawTextW(dc, chu.c_str(), -1, &vung, can | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
}
void the(HDC dc, RECT r, COLORREF mau) {
    HBRUSH brush = CreateSolidBrush(mau);
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(227,233,242));
    HGDIOBJ cuBrush = SelectObject(dc, brush), cuPen = SelectObject(dc, pen);
    RoundRect(dc, r.left, r.top, r.right, r.bottom, 16, 16);
    SelectObject(dc, cuBrush); SelectObject(dc, cuPen);
    DeleteObject(brush); DeleteObject(pen);
}
}

wstring rong(const string& utf8) {
    if (utf8.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    wstring chu(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), &chu[0], n);
    return chu;
}
string hep(const wstring& unicode) {
    if (unicode.empty()) return "";
    int n = WideCharToMultiByte(CP_UTF8, 0, unicode.data(), static_cast<int>(unicode.size()), nullptr, 0, nullptr, nullptr);
    string chu(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, unicode.data(), static_cast<int>(unicode.size()), &chu[0], n, nullptr, nullptr);
    return chu;
}
wstring so(double giaTri) {
    wostringstream luong;
    luong << fixed << setprecision(0) << giaTri;
    return luong.str();
}
wstring tien(double soTien) {
    wstring chu = so(soTien);
    for (int i = static_cast<int>(chu.size()) - 3; i > 0; i -= 3) chu.insert(i, L".");
    return chu + L" đ";
}
string homNay() {
    SYSTEMTIME t; GetLocalTime(&t);
    ostringstream s;
    s << setfill('0') << setw(2) << t.wDay << '/' << setw(2) << t.wMonth << '/' << setw(4) << t.wYear;
    return s.str();
}
string maHoaDonMoi() {
    SYSTEMTIME t; GetLocalTime(&t);
    ostringstream s;
    s << "HD" << t.wYear << setfill('0') << setw(2) << t.wMonth << setw(2) << t.wDay
      << '-' << GetTickCount64();
    return s.str();
}

HWND CuaSoNhaHang::o(int id) const { return GetDlgItem(cuaSo, id); }
wstring CuaSoNhaHang::chu(int id) const {
    HWND h = o(id);
    int n = GetWindowTextLengthW(h);
    wstring ketQua(n + 1, L'\0');
    GetWindowTextW(h, &ketQua[0], n + 1);
    ketQua.resize(n);
    return ketQua;
}
string CuaSoNhaHang::duLieu(int id) const { return hep(chu(id)); }
void CuaSoNhaHang::gan(int id, const wstring& giaTri) { SetWindowTextW(o(id), giaTri.c_str()); }
int CuaSoNhaHang::luaChon(int id) const { return static_cast<int>(SendMessageW(o(id), CB_GETCURSEL, 0, 0)); }
void CuaSoNhaHang::chonMuc(int id, int viTri) { SendMessageW(o(id), CB_SETCURSEL, viTri, 0); }
bool CuaSoNhaHang::daDanhDau(int id) const { return SendMessageW(o(id), BM_GETCHECK, 0, 0) == BST_CHECKED; }

double CuaSoNhaHang::soNhap(int id, const string& ten, bool nguyen) const {
    string chuoi = duLieu(id);
    size_t daDoc = 0;
    double giaTri;
    try { giaTri = stod(chuoi, &daDoc); }
    catch (...) { throw runtime_error(ten + ": vui lòng nhập số."); }
    if (daDoc != chuoi.size() || !isfinite(giaTri) || giaTri < 0 || giaTri > 1000000000 ||
        (nguyen && floor(giaTri) != giaTri)) throw runtime_error(ten + ": số không hợp lệ (0 đến 1.000.000.000).");
    return giaTri;
}

HWND CuaSoNhaHang::tao(const wchar_t* lop, const wstring& noiDung, DWORD kieu,
                      int id, int x, int y, int w, int h, DWORD moRong) {
    HWND dieuKhien = CreateWindowExW(moRong, lop, noiDung.c_str(), WS_CHILD | WS_VISIBLE | kieu,
                                     x, y, w, h, cuaSo, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                                     GetModuleHandleW(nullptr), nullptr);
    if (!dieuKhien) throw runtime_error("Không tạo được thành phần giao diện.");
    SendMessageW(dieuKhien, WM_SETFONT, reinterpret_cast<WPARAM>(fontChu), TRUE);
    return dieuKhien;
}
void CuaSoNhaHang::nhan(const wstring& noiDung, int x, int y, int w, bool dam) {
    HWND h = tao(L"STATIC", noiDung, SS_LEFT | SS_CENTERIMAGE, -1, x, y, w, 24);
    SendMessageW(h, WM_SETFONT, reinterpret_cast<WPARAM>(dam ? fontDam : fontNho), TRUE);
}
HWND CuaSoNhaHang::nut(const wstring& noiDung, int id, int x, int y, int w, int kieu) {
    HWND h = tao(L"BUTTON", noiDung, WS_TABSTOP | BS_OWNERDRAW, id, x, y, w, 40);
    kieuNut[h] = kieu;
    return h;
}
HWND CuaSoNhaHang::nhap(const wstring& ten, int id, int x, int y, int w, const wstring& giaTri, bool matKhau) {
    if (!ten.empty()) nhan(ten, x, y, w);
    DWORD kieu = WS_TABSTOP | ES_AUTOHSCROLL;
    if (matKhau) kieu |= ES_PASSWORD;
    HWND h = tao(L"EDIT", giaTri, kieu, id, x, y + 26, w, 32, WS_EX_CLIENTEDGE);
    SendMessageW(h, EM_SETLIMITTEXT, matKhau ? 128 : 160, 0);
    if (matKhau) SendMessageW(h, EM_SETPASSWORDCHAR, L'*', 0);
    return h;
}
HWND CuaSoNhaHang::chon(const wstring& ten, int id, int x, int y, int w, const vector<wstring>& cacMuc) {
    if (!ten.empty()) nhan(ten, x, y, w);
    HWND h = tao(L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, id, x, y + 26, w, 260);
    for (const wstring& muc : cacMuc) SendMessageW(h, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(muc.c_str()));
    chonMuc(id, 0);
    return h;
}
void CuaSoNhaHang::danhDau(const wstring& ten, int id, int x, int y, int w, bool giaTri) {
    HWND h = tao(L"BUTTON", ten, WS_TABSTOP | BS_AUTOCHECKBOX, id, x, y, w, 26);
    SendMessageW(h, BM_SETCHECK, giaTri ? BST_CHECKED : BST_UNCHECKED, 0);
}
HWND CuaSoNhaHang::bang(int id, int x, int y, int w, int h, const vector<pair<wstring,int>>& cacCot) {
    HWND lv = tao(WC_LISTVIEWW, L"", WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                   id, x, y, w, h, WS_EX_CLIENTEDGE);
    ListView_SetExtendedListViewStyle(lv, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP);
    ListView_SetBkColor(lv, RGB(255,255,255));
    ListView_SetTextBkColor(lv, RGB(255,255,255));
    ListView_SetTextColor(lv, CHU);
    SetWindowTheme(lv, L"Explorer", nullptr);
    for (size_t i = 0; i < cacCot.size(); i++) {
        LVCOLUMNW cot = {};
        cot.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        cot.pszText = const_cast<wchar_t*>(cacCot[i].first.c_str());
        cot.cx = cacCot[i].second;
        ListView_InsertColumn(lv, static_cast<int>(i), &cot);
    }
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
    for (size_t i = 1; i < cacCot.size(); i++) ListView_SetItemText(lv, dong, static_cast<int>(i), const_cast<wchar_t*>(cacCot[i].c_str()));
}
int CuaSoNhaHang::dongChon(int id) const {
    HWND lv = o(id);
    int dong = ListView_GetNextItem(lv, -1, LVNI_SELECTED);
    if (dong < 0) return -1;
    LVITEMW muc = {}; muc.mask = LVIF_PARAM; muc.iItem = dong;
    if (!ListView_GetItem(lv, &muc)) return -1;
    return static_cast<int>(muc.lParam);
}

void CuaSoNhaHang::xoaDieuKhien() {
    kieuNut.clear();
    vector<HWND> ds;
    for (HWND h = GetWindow(cuaSo, GW_CHILD); h; h = GetWindow(h, GW_HWNDNEXT)) ds.push_back(h);
    for (HWND h : ds) DestroyWindow(h);
}

void CuaSoNhaHang::thanhBen() {
    nut(L"Tổng quan", TRANG_CHU, 18, 160, 200, 3);
    nut(L"Khách hàng", KHACH_HANG, 18, 210, 200, 3);
    int y = 260;
    if (dichVu.laQuanLy()) { nut(L"Nhân viên", NHAN_VIEN, 18, y, 200, 3); y += 50; }
    nut(L"Thực đơn", THUC_DON, 18, y, 200, 3); y += 50;
    nut(L"Bán hàng", BAN_HANG, 18, y, 200, 3); y += 50;
    if (dichVu.laQuanLy()) {
        nut(L"Báo cáo", BAO_CAO, 18, y, 200, 3); y += 50;
        nut(L"Tạo tài khoản", TAI_KHOAN, 18, y, 200, 3); y += 50;
    }
    nut(L"Đổi mật khẩu", MAT_KHAU, 18, y, 200, 3);
    nut(L"Đăng xuất", DANG_XUAT, 18, caoCuaSo - 85, 200, 3);
}

void CuaSoNhaHang::hienTrang(int trangMoi, bool giuNhap) {
    if (dichVu.daDangNhap() && !dichVu.laQuanLy() &&
        (trangMoi == NHAN_VIEN || trangMoi == BAO_CAO || trangMoi == TAI_KHOAN)) {
        throw runtime_error("Chức năng này chỉ dành cho quản lý.");
    }
    if (dichVu.daDangNhap() && trangHienTai == BAN_HANG && o(O_NGAY)) {
        maHoaDon = duLieu(O_MA); ngayDon = duLieu(O_NGAY);
        khachDon = khachDaChon(); nhanVienDon = nhanVienDaChon();
    }
    map<int,wstring> nhapCu;
    map<int,int> chonCu;
    if (giuNhap) {
        const vector<int> cacO = {TIM_KIEM, LOC_BAO_CAO, O_MA, O_TEN, O_SDT, O_DIEM, O_CHUCVU,
            O_LUONG, O_PHUCAP, O_GIA, O_TON, O_CAY, O_NHAPKHO, O_NGAY, O_SOLUONG,
            O_USER, O_PASS, O_CONFIRM, O_OLDPASS};
        for (int id : cacO) if (o(id)) nhapCu[id] = chu(id);
        for (int id : {LOC_LOAI, SAP_XEP, O_GIOITINH, O_LOAI, O_SIZE, O_KHACH, O_NHANVIEN, O_ROLE})
            if (o(id)) chonCu[id] = luaChon(id);
        for (int id : {CON_HANG, O_CHAY, O_DA}) if (o(id)) chonCu[id] = daDanhDau(id) ? 1 : 0;
    }
    dangVeBang = true;
    SendMessageW(cuaSo, WM_SETREDRAW, FALSE, 0);
    xoaDieuKhien();
    if (!giuNhap) maDangSua.clear();
    trangHienTai = trangMoi;
    theBieuMau = {};
    if (!dichVu.daDangNhap()) trangDangNhap();
    else {
        thanhBen();
        switch (trangHienTai) {
            case TRANG_CHU: trangChu(); break;
            case KHACH_HANG: case NHAN_VIEN: case THUC_DON: trangDanhSach(); break;
            case BAN_HANG: trangBanHang(); break;
            case BAO_CAO: trangBaoCao(); break;
            case TAI_KHOAN: trangTaiKhoan(); break;
            case MAT_KHAU: trangMatKhau(); break;
        }
    }
    for (const pair<const int,wstring>& muc : nhapCu) if (o(muc.first)) gan(muc.first, muc.second);
    for (const pair<const int,int>& muc : chonCu) {
        if (o(muc.first)) {
            if (muc.first == CON_HANG || muc.first == O_CHAY || muc.first == O_DA)
                SendMessageW(o(muc.first), BM_SETCHECK, muc.second ? BST_CHECKED : BST_UNCHECKED, 0);
            else chonMuc(muc.first, muc.second);
        }
    }
    if (dichVu.daDangNhap()) {
        if (trangHienTai == KHACH_HANG || trangHienTai == NHAN_VIEN || trangHienTai == THUC_DON) { napBang(); doiLoaiMon(); }
        if (trangHienTai == BAN_HANG) { napBanHang(); napGio(); }
    }
    if (!maDangSua.empty() && o(O_MA)) SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    dangVeBang = false;
    SendMessageW(cuaSo, WM_SETREDRAW, TRUE, 0);
    RedrawWindow(cuaSo, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CuaSoNhaHang::ve(HDC dc) {
    RECT toan = {0,0,rongCuaSo,caoCuaSo}; FillRect(dc, &toan, nen);
    if (!dichVu.daDangNhap()) {
        veNhanDienDangNhap(dc);
        the(dc, theBieuMau, RGB(255,255,255));
        return;
    }
    RECT ben = {0,0,236,caoCuaSo};
    HBRUSH brush = CreateSolidBrush(THANH_BEN); FillRect(dc, &ben, brush); DeleteObject(brush);
    chuVe(dc, L"NHÀ HÀNG", {25,34,216,70}, fontDam, RGB(255,255,255));
    chuVe(dc, L"Không gian làm việc", {25,75,216,100}, fontNho, RGB(155,177,209));
    chuVe(dc, rong(dichVu.phien().tenDangNhap), {25,caoCuaSo-145,218,caoCuaSo-118}, fontDam, RGB(255,255,255));
    wstring vaiTro = dichVu.laQuanLy() ? L"Quản lý" : L"Nhân viên";
    chuVe(dc, vaiTro, {25,caoCuaSo-119,218,caoCuaSo-95}, fontNho, RGB(155,177,209));
    map<int,wstring> tieuDe = {{TRANG_CHU,L"Tổng quan"},{KHACH_HANG,L"Khách hàng"},{NHAN_VIEN,L"Nhân viên"},
        {THUC_DON,L"Thực đơn"},{BAN_HANG,L"Bán hàng"},{BAO_CAO,L"Báo cáo kinh doanh"},
        {TAI_KHOAN,L"Tạo tài khoản"},{MAT_KHAU,L"Đổi mật khẩu"}};
    map<int,wstring> moTa = {{TRANG_CHU,L"Một ngày làm việc hiệu quả bắt đầu từ đây."},
        {KHACH_HANG,L"Tìm kiếm, chăm sóc và cập nhật thông tin khách hàng."},
        {NHAN_VIEN,L"Quản lý hồ sơ và thu nhập của đội ngũ."},
        {THUC_DON,L"Món ăn, nước uống và tồn kho trong một danh sách."},
        {BAN_HANG,L"Chọn món, thêm vào giỏ và thanh toán nhanh chóng."},
        {BAO_CAO,L"Theo dõi hóa đơn, doanh thu và các món cần nhập thêm."},
        {TAI_KHOAN,L"Cấp quyền truy cập cho quản lý và nhân viên."},
        {MAT_KHAU,L"Cập nhật mật khẩu cho tài khoản đang đăng nhập."}};
    chuVe(dc, tieuDe[trangHienTai], {264,26,rongCuaSo-240,70}, fontLon, CHU);
    chuVe(dc, moTa[trangHienTai], {266,76,rongCuaSo-30,104}, fontNho, MO);
    chuVe(dc, rong(homNay()), {rongCuaSo-205,34,rongCuaSo-26,66}, fontChu, MO, DT_RIGHT);
    if (theBieuMau.right > 0) the(dc, theBieuMau, RGB(255,255,255));
    if (trangHienTai == TRANG_CHU) {
        vector<wstring> ten = {L"KHÁCH HÀNG", L"MÓN TRONG THỰC ĐƠN", L"CẦN NHẬP THÊM", L"TRONG GIỎ HÀNG"};
        int thap = 0;
        for (const unique_ptr<Mon>& mon : dichVu.thucDon()) if (mon->getTonKho() <= 5) thap++;
        vector<wstring> giaTri = {to_wstring(dichVu.khachHang().size()),to_wstring(dichVu.thucDon().size()),to_wstring(thap),to_wstring(gioHang.size())};
        if (dichVu.laQuanLy()) { ten[3] = L"DOANH THU HÔM NAY"; giaTri[3] = tien(dichVu.baoCao(homNay()).doanhThu); }
        int w = (rongCuaSo - 290) / 4;
        for (int i=0; i<4; i++) {
            int x=264+i*w;
            the(dc, {x,130,x+w-14,244}, RGB(255,255,255));
            chuVe(dc,ten[i],{x+18,146,x+w-24,172},fontNho,MO);
            chuVe(dc,giaTri[i],{x+18,184,x+w-24,226},fontDam,i==2?RGB(191,108,22):XANH);
        }
    }
    RECT chan = {250,caoCuaSo-33,rongCuaSo-20,caoCuaSo-6};
    chuVe(dc, thongBao, chan, fontNho, MO);
}

void CuaSoNhaHang::veNut(const DRAWITEMSTRUCT* n) {
    int kieu = kieuNut[n->hwndItem];
    bool chon = (n->itemState & ODS_SELECTED) != 0;
    bool khoa = (n->itemState & ODS_DISABLED) != 0;
    COLORREF bg = RGB(233,239,248), fg = CHU;
    if (kieu == 1) { bg=XANH; fg=RGB(255,255,255); }
    if (kieu == 2) { bg=RGB(255,235,235); fg=RGB(174,44,44); }
    if (kieu == 3) {
        bg = static_cast<int>(n->CtlID) == trangHienTai ? RGB(39,65,108) : THANH_BEN;
        fg = RGB(229,237,249);
    }
    if (chon) bg = RGB(53,88,145);
    if (khoa) { bg = RGB(234,236,240); fg = MO; }
    HBRUSH nenNut = CreateSolidBrush(kieu == 3 ? THANH_BEN : NEN);
    FillRect(n->hDC, &n->rcItem, nenNut);
    DeleteObject(nenNut);
    HBRUSH brush = CreateSolidBrush(bg);
    HGDIOBJ cuBrush = SelectObject(n->hDC, brush), cuPen = SelectObject(n->hDC, GetStockObject(NULL_PEN));
    RoundRect(n->hDC,n->rcItem.left,n->rcItem.top,n->rcItem.right,n->rcItem.bottom,10,10);
    SelectObject(n->hDC,cuBrush); SelectObject(n->hDC,cuPen); DeleteObject(brush);
    int doDai = GetWindowTextLengthW(n->hwndItem);
    wstring noiDung(doDai + 1, L'\0');
    GetWindowTextW(n->hwndItem, &noiDung[0], doDai + 1);
    noiDung.resize(doDai);
    RECT r=n->rcItem;
    if (kieu==3) r.left+=18;
    chuVe(n->hDC,noiDung,r,fontChu,fg,kieu==3?DT_LEFT:DT_CENTER);
    if (n->itemState & ODS_FOCUS) { InflateRect(&r,-4,-4); DrawFocusRect(n->hDC,&r); }
}

void CuaSoNhaHang::baoLoi(const string& noiDung) {
    MessageBoxW(cuaSo, rong(noiDung).c_str(), L"Vui lòng kiểm tra", MB_OK | MB_ICONWARNING);
}
void CuaSoNhaHang::thanhCong(const wstring& noiDung) { thongBao=noiDung; InvalidateRect(cuaSo,nullptr,FALSE); }
bool CuaSoNhaHang::xacNhan(const wstring& noiDung) {
    return MessageBoxW(cuaSo,noiDung.c_str(),L"Xác nhận",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDYES;
}

LRESULT CALLBACK CuaSoNhaHang::thuTuc(HWND h, UINT tin, WPARAM w, LPARAM l) {
    CuaSoNhaHang* app=reinterpret_cast<CuaSoNhaHang*>(GetWindowLongPtrW(h,GWLP_USERDATA));
    if (tin==WM_NCCREATE) {
        app=static_cast<CuaSoNhaHang*>(reinterpret_cast<CREATESTRUCTW*>(l)->lpCreateParams);
        app->cuaSo=h; SetWindowLongPtrW(h,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(app));
    }
    if (!app) return DefWindowProcW(h,tin,w,l);
    try {
        switch(tin) {
            case WM_CREATE: {
                app->fontChu=CreateFontW(-16,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
                app->fontNho=CreateFontW(-14,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
                app->fontDam=CreateFontW(-18,0,0,0,FW_SEMIBOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
                app->fontLon=CreateFontW(-30,0,0,0,FW_SEMIBOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
                app->logoBachKhoa=taiLogo(LOGO_BACH_KHOA);
                app->logoCntt=taiLogo(LOGO_CNTT);
                app->nen=CreateSolidBrush(NEN); app->trang=CreateSolidBrush(RGB(255,255,255));
                RECT r; GetClientRect(h,&r); app->rongCuaSo=r.right; app->caoCuaSo=r.bottom;
                app->hienTrang(TRANG_CHU); return 0;
            }
            case WM_SIZE:
                if(w!=SIZE_MINIMIZED) { app->rongCuaSo=LOWORD(l); app->caoCuaSo=HIWORD(l); SetTimer(h,1,180,nullptr); }
                return 0;
            case WM_TIMER:
                if(w==1) { KillTimer(h,1); app->hienTrang(app->trangHienTai,true); } return 0;
            case WM_GETMINMAXINFO: {
                MINMAXINFO* m=reinterpret_cast<MINMAXINFO*>(l); m->ptMinTrackSize={1120,760}; return 0;
            }
            case WM_COMMAND: if(!app->dangVeBang) app->xuLyNut(LOWORD(w),HIWORD(w)); return 0;
            case WM_NOTIFY: {
                NMHDR* n=reinterpret_cast<NMHDR*>(l);
                if(!app->dangVeBang && n->idFrom==BANG_CHINH && n->code==LVN_ITEMCHANGED) {
                    NMLISTVIEW* lv=reinterpret_cast<NMLISTVIEW*>(l);
                    if((lv->uNewState & LVIS_SELECTED) && !(lv->uOldState & LVIS_SELECTED)) app->chonDong();
                }
                if(!app->dangVeBang && n->idFrom==BANG_CHINH && n->code==NM_DBLCLK && app->trangHienTai==BAO_CAO) app->xuLyNut(NUT_HOADON,BN_CLICKED);
                return 0;
            }
            case WM_DRAWITEM: app->veNut(reinterpret_cast<DRAWITEMSTRUCT*>(l)); return TRUE;
            case WM_CTLCOLORSTATIC: {
                HDC dc=reinterpret_cast<HDC>(w); SetTextColor(dc,MO); SetBkMode(dc,TRANSPARENT);
                RECT r; GetWindowRect(reinterpret_cast<HWND>(l),&r); MapWindowPoints(nullptr,h,reinterpret_cast<POINT*>(&r),2);
                if(r.left>=app->theBieuMau.left && r.top>=app->theBieuMau.top && r.right<=app->theBieuMau.right && r.bottom<=app->theBieuMau.bottom) return reinterpret_cast<LRESULT>(app->trang);
                return reinterpret_cast<LRESULT>(app->nen);
            }
            case WM_CTLCOLOREDIT: SetTextColor(reinterpret_cast<HDC>(w),CHU); SetBkColor(reinterpret_cast<HDC>(w),RGB(255,255,255)); return reinterpret_cast<LRESULT>(app->trang);
            case WM_ERASEBKGND: return 1;
            case WM_PAINT: { PAINTSTRUCT ps; HDC dc=BeginPaint(h,&ps); app->ve(dc); EndPaint(h,&ps); return 0; }
            case WM_CLOSE:
                if(!app->gioHang.empty() && !app->xacNhan(L"Giỏ hàng chưa thanh toán sẽ bị hủy. Bạn muốn thoát?")) return 0;
                DestroyWindow(h); return 0;
            case WM_DESTROY:
                DeleteObject(app->fontChu); DeleteObject(app->fontNho); DeleteObject(app->fontDam); DeleteObject(app->fontLon);
                DeleteObject(app->logoBachKhoa); DeleteObject(app->logoCntt);
                DeleteObject(app->nen); DeleteObject(app->trang); PostQuitMessage(0); return 0;
        }
    } catch(const exception& e) {
        app->dangVeBang=false;
        SendMessageW(h,WM_SETREDRAW,TRUE,0);
        app->baoLoi(e.what());
        if(tin==WM_CREATE) return -1;
    }
    return DefWindowProcW(h,tin,w,l);
}

int CuaSoNhaHang::chay(HINSTANCE ungDung, int cheDoHien) {
    thietLap=dichVu.khoiTao();
    INITCOMMONCONTROLSEX cc={sizeof(cc),ICC_LISTVIEW_CLASSES|ICC_STANDARD_CLASSES}; InitCommonControlsEx(&cc);
    WNDCLASSEXW lop={}; lop.cbSize=sizeof(lop); lop.hInstance=ungDung; lop.lpfnWndProc=thuTuc;
    lop.lpszClassName=L"NhaHangCuaSo"; lop.hCursor=LoadCursor(nullptr,IDC_ARROW); lop.hIcon=LoadIcon(nullptr,IDI_APPLICATION);
    if(!RegisterClassExW(&lop)) throw runtime_error("Không đăng ký được cửa sổ.");
    RECT manHinh; SystemParametersInfoW(SPI_GETWORKAREA,0,&manHinh,0);
    int w=min(1320,static_cast<int>(manHinh.right-manHinh.left-40));
    int h=min(880,static_cast<int>(manHinh.bottom-manHinh.top-40));
    HWND win=CreateWindowExW(WS_EX_CONTROLPARENT,lop.lpszClassName,L"Nhà hàng • Quản lý & bán hàng",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
        manHinh.left+(manHinh.right-manHinh.left-w)/2,manHinh.top+(manHinh.bottom-manHinh.top-h)/2,w,h,nullptr,nullptr,ungDung,this);
    if(!win) throw runtime_error("Không mở được cửa sổ ứng dụng.");
    ShowWindow(win,cheDoHien); UpdateWindow(win);
    MSG tin;
    while(GetMessageW(&tin,nullptr,0,0)>0) {
        if(tin.message==WM_KEYDOWN && tin.wParam==VK_RETURN && !dichVu.daDangNhap()) {
            try { xuLyNut(NUT_DANGNHAP,BN_CLICKED); }
            catch(const exception& loi) { baoLoi(loi.what()); }
            continue;
        }
        if(!IsDialogMessageW(win,&tin)) { TranslateMessage(&tin); DispatchMessageW(&tin); }
    }
    return static_cast<int>(tin.wParam);
}

int chayGiaoDienCuaSo() {
    // DuLieu luon nam canh tep exe, ke ca khi mo tu Explorer hay shortcut.
    wchar_t duongDan[32768];
    DWORD n=GetModuleFileNameW(nullptr,duongDan,32768);
    if(n==0 || n>=32768) throw runtime_error("Không xác định được thư mục ứng dụng.");
    wstring thuMuc(duongDan,n); thuMuc=thuMuc.substr(0,thuMuc.find_last_of(L"\\/"));
    if(!SetCurrentDirectoryW(thuMuc.c_str())) throw runtime_error("Không mở được thư mục ứng dụng.");
    CuaSoNhaHang app;
    return app.chay(GetModuleHandleW(nullptr),SW_SHOWDEFAULT);
}
