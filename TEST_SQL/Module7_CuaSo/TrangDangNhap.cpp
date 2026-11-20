#include "CuaSo.h"
#include <algorithm>

using namespace std;

namespace {
const COLORREF NAU_DO = RGB(122,52,24);
const COLORREF CHU_SANG = RGB(240,222,208);
}

void CuaSoNhaHang::trangDangNhap() {
    int nua = rongCuaSo / 2;
    int w = min(440, nua - 128);
    int x = nua + (nua - w) / 2;
    int caoNoiDung = thietLap ? 422 : 400;
    int y = max(64, (caoCuaSo - caoNoiDung) / 2);
    themThe({x - 40, y - 40, x + w + 40, y + caoNoiDung + 40});
    HWND tieuDe = tao(L"STATIC", thietLap ? L"Thiết lập lần đầu" : L"Chào mừng trở lại",
                      SS_LEFT | SS_NOPREFIX, -1, x, y, w, 38);
    SendMessageW(tieuDe, WM_SETFONT, reinterpret_cast<WPARAM>(font.tieuDe), TRUE);
    mauChuNhan[tieuDe] = Mau::CHU;
    nhan(thietLap ? L"Tạo tài khoản quản lý để bắt đầu." : L"Đăng nhập để tiếp tục.", x, y + 44, w);
    nhap(L"Tên đăng nhập", O_USER, x, y + 88, w);
    nhap(L"Mật khẩu", O_PASS, x, y + 168, w, L"", true);
    if (thietLap) nhap(L"Nhập lại mật khẩu", O_CONFIRM, x, y + 248, w, L"", true);
    else nhan(L"Khách mới có thể tự đăng ký tài khoản bên dưới.", x, y + 250, w);
    int yNut = thietLap ? y + 336 : y + 296;
    nut(thietLap ? L"Tạo tài khoản quản lý" : L"Đăng nhập", NUT_DANGNHAP, x, yNut, w, KieuNut::Chinh, 0, 46);
    if (thietLap) nhan(L"Nhấn Enter hoặc chọn nút để tiếp tục.", x, yNut + 62, w);
    else nut(L"Đăng ký khách hàng", NUT_MO_DANGKY, x, yNut + 58, w, KieuNut::Phu, Icon::TAI_KHOAN, 46);
    SetFocus(o(O_USER));
}

void CuaSoNhaHang::trangDangKy() {
    int nua = rongCuaSo / 2;
    int w = min(460, nua - 112);
    int x = nua + (nua - w) / 2;
    int caoNoiDung = 560;
    int y = max(52, (caoCuaSo - caoNoiDung) / 2);
    themThe({x - 36, y - 32, x + w + 36, y + caoNoiDung + 28});
    HWND tieuDe = tao(L"STATIC", L"Đăng ký khách hàng", SS_LEFT | SS_NOPREFIX, -1, x, y, w, 38);
    SendMessageW(tieuDe, WM_SETFONT, reinterpret_cast<WPARAM>(font.tieuDe), TRUE);
    mauChuNhan[tieuDe] = Mau::CHU;
    nhan(L"Tạo tài khoản để tích điểm và xem hóa đơn của bạn.", x, y + 44, w);
    nhap(L"Họ và tên", O_TEN, x, y + 80, w);
    int nuaO = (w - 16) / 2;
    nhap(L"Số điện thoại (10 số, đầu 0)", O_SDT, x, y + 156, nuaO);
    chon(L"Giới tính", O_GIOITINH, x + nuaO + 16, y + 156, w - nuaO - 16, {L"Nam", L"Nữ", L"Khác"});
    chonMuc(O_GIOITINH, 2);
    nhap(L"Email", O_EMAIL, x, y + 232, w);
    nhap(L"Tên đăng nhập", O_USER, x, y + 308, w);
    nhap(L"Mật khẩu", O_PASS, x, y + 384, nuaO, L"", true);
    nhap(L"Nhập lại mật khẩu", O_CONFIRM, x + nuaO + 16, y + 384, w - nuaO - 16, L"", true);
    nut(L"Tạo tài khoản khách hàng", NUT_DANGKY, x, y + 468, w, KieuNut::Chinh, 0, 44);
    nut(L"Quay lại đăng nhập", NUT_VE_DANGNHAP, x, y + 520, w, KieuNut::Phu, 0, 40);
    SetFocus(o(O_TEN));
}

void CuaSoNhaHang::veManDangNhap(HDC dc) {
    int nua = rongCuaSo / 2;
    RECT trai = {0, 0, nua, caoCuaSo};
    int luu = SaveDC(dc);
    IntersectClipRect(dc, 0, 0, nua, caoCuaSo);
    veChuyenSac(dc, trai, Mau::THANH_BEN, NAU_DO);
    veVongTron(dc, nua - 30, 70, 180, Mau::CHINH, 40);
    veVongTron(dc, 30, caoCuaSo - 40, 140, RGB(255,255,255), 12);
    veVongTron(dc, nua - 110, caoCuaSo - 150, 56, RGB(255,196,150), 26);

    int rongThe = min(nua - 96, 520);
    int x = (nua - rongThe) / 2;
    int tren = max(48, (caoCuaSo - 440) / 2);
    // Thuong hieu.
    veVongTron(dc, x + 22, tren + 22, 22, Mau::CHINH);
    veIcon(dc, Icon::NHA_HANG, {x, tren, x + 44, tren + 44}, font.iconLon, RGB(255,255,255));
    veChu(dc, L"NHÀ HÀNG", {x + 56, tren + 1, x + rongThe, tren + 23}, font.dam, RGB(255,255,255));
    veChu(dc, L"Quản lý & bán hàng", {x + 56, tren + 23, x + rongThe, tren + 43}, font.nho, CHU_SANG);

    // The logo truong va khoa.
    int dinh = tren + 76;
    RECT the = {x, dinh, x + rongThe, dinh + 224};
    veHinhBoGoc(dc, the, 22, Mau::THE);
    int giua = (the.left + the.right) / 2;
    anh.veLogo(dc, LOGO_BACH_KHOA, {the.left + 24, dinh + 22, giua - 24, dinh + 152});
    anh.veLogo(dc, LOGO_CNTT, {giua + 24, dinh + 22, the.right - 24, dinh + 152});
    RECT ke = {giua, dinh + 28, giua + 1, dinh + 196};
    toNen(dc, ke, Mau::VIEN);
    veChu(dc, L"ĐẠI HỌC BÁCH KHOA", {the.left + 8, dinh + 162, giua - 8, dinh + 184}, font.nhoDam, Mau::CHU_NHAN, DT_CENTER);
    veChu(dc, L"ĐÀ NẴNG", {the.left + 8, dinh + 184, giua - 8, dinh + 206}, font.nhoDam, Mau::CHU_NHAN, DT_CENTER);
    veChu(dc, L"KHOA CÔNG NGHỆ", {giua + 8, dinh + 162, the.right - 8, dinh + 184}, font.nhoDam, Mau::CHU_NHAN, DT_CENTER);
    veChu(dc, L"THÔNG TIN", {giua + 8, dinh + 184, the.right - 8, dinh + 206}, font.nhoDam, Mau::CHU_NHAN, DT_CENTER);

    // Tieu de he thong.
    int yChu = the.bottom + 36;
    veChu(dc, L"Hệ thống quản lý nhà hàng", {x - 20, yChu, x + rongThe + 20, yChu + 40}, font.tieuDe,
          RGB(255,255,255), DT_CENTER);
    RECT vach = {nua / 2 - 28, yChu + 52, nua / 2 + 28, yChu + 56};
    veHinhBoGoc(dc, vach, 2, Mau::CHINH);
    veChu(dc, L"Khách hàng, thực đơn, bán hàng và báo cáo — gọn gàng trong một nơi.",
          {x - 20, yChu + 68, x + rongThe + 20, yChu + 112}, font.thuong, CHU_SANG, DT_CENTER | DT_WORDBREAK);
    RestoreDC(dc, luu);
}
