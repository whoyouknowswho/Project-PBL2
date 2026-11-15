#include "CuaSo.h"
#include "LuoiMon.h"
#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
const int RONG_GIO = 360;

wstring chuThuong(wstring s) {
    if (!s.empty()) CharLowerBuffW(&s[0], static_cast<DWORD>(s.size()));
    return s;
}
}

string CuaSoNhaHang::khachDaChon() const {
    if (!o(O_KHACH)) return "";
    int i = luaChon(O_KHACH) - 1;
    if (i >= 0 && static_cast<size_t>(i) < dichVu.khachHang().size()) return dichVu.khachHang()[i].getMa();
    return "";
}

string CuaSoNhaHang::nhanVienDaChon() const {
    return dichVu.phien().maLienKet.empty() ? dichVu.phien().tenDangNhap : dichVu.phien().maLienKet;
}

// Cot trai (tim, nhom, luoi mon, so luong) va the gio hang ben phai; dung chung cho Ban hang va Dat mon.
void CuaSoNhaHang::dungLuoiDatMon(int x, int y, int w, int h) {
    bool datMon = trangHienTai == DAT_MON;
    int day = y + h;
    // Cot hep (cua so nho): o tim mot dong, hai nut nhom xuong dong duoi.
    bool hep = w < 600;
    int caoCongCu = hep ? 116 : 64;
    int rongTab = hep ? (w - 48 - 12) / 2 : 150;
    int rongTim = hep ? w - 48 : w - 48 - 2 * rongTab - 24;
    int xTab = hep ? x + 24 : x + 24 + rongTim + 12, yTab = hep ? y + 64 : y + 12;
    themThe({x, y, x + w, y + caoCongCu});
    nhap(L"", TIM_KIEM, x + 24, y - 12, rongTim);
    SendMessageW(o(TIM_KIEM), EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Tìm món theo tên hoặc mã"));
    nut(L"Thức ăn", NUT_NHOM_THUCAN, xTab, yTab, rongTab, nhomDatMon == 0 ? KieuNut::TabChon : KieuNut::Tab);
    nut(L"Nước uống", NUT_NHOM_NUOC, xTab + rongTab + 12, yTab, rongTab, nhomDatMon == 1 ? KieuNut::TabChon : KieuNut::Tab);
    taoLuoiMon(cuaSo, DANH_SACH_MON, {x - 6, y + caoCongCu + 12, x + w + 6, day - 130}, &font, &anh);

    int yDuoi = day - 118;
    themThe({x, yDuoi, x + w, day});
    nut(L"", NUT_GIAM_SL, x + 20, yDuoi + 12, 40, KieuNut::Phu, Icon::TRU);
    nhap(L"", O_SOLUONG, x + 68, yDuoi - 12, 64, L"1");
    nut(L"", NUT_TANG_SL, x + 140, yDuoi + 12, 40, KieuNut::Phu, Icon::THEM);
    HWND nhanSize = nhan(L"Size nước", x + 200, yDuoi + 22, 90, false, NHAN_SIZE);
    SendMessageW(nhanSize, WM_SETFONT, reinterpret_cast<WPARAM>(font.nhoDam), TRUE);
    mauChuNhan[nhanSize] = Mau::CHU_NHAN;
    chon(L"", O_SIZE, x + 292, yDuoi - 12, 88, {L"S", L"M", L"L"});
    chonMuc(O_SIZE, 1);
    ShowWindow(o(NHAN_SIZE), SW_HIDE);
    ShowWindow(o(O_SIZE), SW_HIDE);
    EnableWindow(o(O_SIZE), FALSE);
    int rongNut = min(220, w - 40 - 160);
    HWND nhanChon = nhan(L"Chọn món ở trên để thêm vào giỏ", x + 20, yDuoi + 62, w - 60 - rongNut, false, NHAN_MON_CHON, 44);
    mauChuNhan[nhanChon] = Mau::CHU_NHAN;
    nut(L"Thêm món đã chọn", NUT_THEM_GIO, x + w - 20 - rongNut, yDuoi + 62, rongNut, KieuNut::Chinh, Icon::THEM, 44);

    int xg = phaiNoiDung() - RONG_GIO, wg = RONG_GIO;
    themThe({xg, y, xg + wg, day});
    themDauThe(xg + 20, y + 18, wg - 40, Icon::BAN_HANG, datMon ? L"Giỏ món của bạn" : L"Giỏ hàng",
               datMon ? L"Nhân viên xác nhận giá khi thanh toán." : L"Tồn kho chỉ trừ khi thanh toán.");
    bang(BANG_GIO, xg + 12, y + 72, wg - 24, max(80, h - 72 - 232), {{L"Món / tùy chọn", 170}, {L"SL", 44}, {L"Thành tiền", 116}});
    canCot(BANG_GIO, 1, LVCFMT_CENTER);
    canCot(BANG_GIO, 2, LVCFMT_RIGHT);
    tao(L"STATIC", L"", SS_LEFT, O_TONG, 0, 0, 1, 1);
    ShowWindow(o(O_TONG), SW_HIDE);
    int nua = (wg - 52) / 2;
    nut(L"Xóa món", NUT_XOA_GIO, xg + 20, day - 108, nua, KieuNut::NguyHiem, Icon::XOA);
    nut(datMon ? L"Hủy giỏ" : L"Hủy giỏ hàng", NUT_HUY_GIO, xg + 32 + nua, day - 108, wg - 52 - nua, KieuNut::Phu, Icon::HUY);
    if (datMon) nut(L"Gửi đơn đặt món", NUT_GUI_DON, xg + 20, day - 60, wg - 40, KieuNut::Chinh, Icon::DON_DAT, 44);
    else nut(L"Thanh toán & lưu hóa đơn", NUT_THANHTOAN, xg + 20, day - 60, wg - 40, KieuNut::Chinh, Icon::TIEN, 44);
}

RECT CuaSoNhaHang::vungTongGio() const {
    int y = trangHienTai == DAT_MON ? BoCuc::Y0 : BoCuc::Y0 + 100;
    int xg = phaiNoiDung() - RONG_GIO, day = dayNoiDung();
    RECT bangR = {xg, y + 72, xg + RONG_GIO, y + 72 + max(80, day - y - 72 - 232)};
    return {xg + 20, bangR.bottom + 10, xg + RONG_GIO - 20, day - 118};
}

void CuaSoNhaHang::trangBanHang() {
    if (maHoaDon.empty()) maHoaDon = maHoaDonMoi();
    if (ngayDon.empty()) ngayDon = homNay();
    int x = BoCuc::X0, y = BoCuc::Y0, w = phaiNoiDung() - x;
    themThe({x, y, x + w, y + 88});
    int o4 = (w - 48 - 3 * 16) / 4;
    nhap(L"Mã hóa đơn (tự động)", O_MA, x + 24, y + 8, o4, rong(maHoaDon));
    SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    nhap(L"Ngày lập (dd/mm/yyyy)", O_NGAY, x + 24 + (o4 + 16), y + 8, o4, rong(ngayDon));
    vector<wstring> kh = {L"Khách lẻ"};
    int chonKhach = 0, i = 1;
    for (const KhachHang& k : dichVu.khachHang()) {
        kh.push_back(rong(k.getMa() + " - " + k.getTen()));
        if (k.getMa() == khachDon) chonKhach = i;
        i++;
    }
    chon(L"Khách hàng", O_KHACH, x + 24 + 2 * (o4 + 16), y + 8, o4, kh);
    chonMuc(O_KHACH, chonKhach);
    nhap(L"Nhân viên bán hàng", O_NHANVIEN, x + 24 + 3 * (o4 + 16), y + 8, w - 48 - 3 * (o4 + 16),
         rong(nhanVienDaChon()));
    SendMessageW(o(O_NHANVIEN), EM_SETREADONLY, TRUE, 0);
    int top = y + 100;
    dungLuoiDatMon(x, top, phaiNoiDung() - RONG_GIO - BoCuc::KHOANG - x, dayNoiDung() - top);
}

void CuaSoNhaHang::trangDatMon() {
    int x = BoCuc::X0, y = BoCuc::Y0;
    dungLuoiDatMon(x, y, phaiNoiDung() - RONG_GIO - BoCuc::KHOANG - x, dayNoiDung() - y);
    thanhCong(L"Chọn Thức ăn hoặc Nước uống · Ảnh minh họa · Đơn chưa gửi chỉ lưu trong giỏ hiện tại.");
}

int CuaSoNhaHang::monDatDaChon() const {
    HWND luoi = o(DANH_SACH_MON);
    if (!luoi) return -1;
    return static_cast<int>(SendMessageW(luoi, LM_LAYCHON, 0, 0));
}

void CuaSoNhaHang::chonMonDat() {
    int i = monDatDaChon();
    bool hopLe = i >= 0 && static_cast<size_t>(i) < dichVu.thucDon().size();
    bool nuoc = false, conHang = false;
    if (hopLe) {
        const Mon& mon = *dichVu.thucDon()[i];
        maMonDangChon = mon.getMa();
        nuoc = mon.getLoai() == "Nuoc uong";
        int daChon = 0;
        for (const YeuCauMon& muc : gioHang) if (muc.maMon == mon.getMa()) daChon += muc.soLuong;
        conHang = mon.getTonKho() > daChon;
        gan(NHAN_MON_CHON, L"Đang chọn: " + rong(mon.getTen()));
    } else {
        maMonDangChon.clear();
        gan(NHAN_MON_CHON, L"Chọn món ở trên để thêm vào giỏ");
    }
    EnableWindow(o(O_SIZE), nuoc);
    ShowWindow(o(NHAN_SIZE), nuoc ? SW_SHOW : SW_HIDE);
    ShowWindow(o(O_SIZE), nuoc ? SW_SHOW : SW_HIDE);
    EnableWindow(o(NUT_THEM_GIO), hopLe && conHang);
}

void CuaSoNhaHang::napMonDat() {
    HWND luoi = o(DANH_SACH_MON);
    if (!luoi) return;
    bool datMon = trangHienTai == DAT_MON;
    wstring tim = chuThuong(chu(TIM_KIEM));
    const auto& menu = dichVu.thucDon();
    vector<TheMon> cacThe;
    int thucAn = 0, nuoc = 0, chon = -1;
    for (size_t i = 0; i < menu.size(); i++) {
        const Mon& mon = *menu[i];
        bool laNuoc = mon.getLoai() == "Nuoc uong";
        if (laNuoc) nuoc++; else thucAn++;
        if (laNuoc != (nhomDatMon == 1)) continue;
        if (chuThuong(rong(mon.getTen() + " " + mon.getMa())).find(tim) == wstring::npos) continue;
        int trongGio = 0;
        for (const YeuCauMon& muc : gioHang) if (muc.maMon == mon.getMa()) trongGio += muc.soLuong;
        int con = mon.getTonKho() - trongGio;
        TheMon t;
        t.duLieu = static_cast<int>(i);
        t.maMon = mon.getMa();
        t.loaiMon = mon.getLoai();
        t.tenMon = mon.getTen();
        t.hinhAnh = mon.getHinhAnh();
        t.ten = rong(mon.getTen());
        t.gia = tien(mon.getGia());
        const MonAn* thucAnPtr = dynamic_cast<const MonAn*>(&mon);
        if (thucAnPtr) t.thongTin = L"Độ cay: " + to_wstring(thucAnPtr->getDoCay()) + L"/5 · " +
                                    (thucAnPtr->laMonChay() ? L"Món chay" : L"Món mặn");
        else t.thongTin = L"Chọn size S / M / L";
        if (con <= 0) {
            t.huyHieu = datMon ? L"Đã hết món" : L"Hết hàng";
            mauHuyHieu(t.huyHieu, t.nenHuyHieu, t.mauHuyHieu);
            t.mo = true;
        } else if (!datMon) {
            t.huyHieu = L"Còn " + to_wstring(con);
            t.nenHuyHieu = Mau::THE;
            t.mauHuyHieu = con <= 5 ? Mau::CANH_BAO : Mau::CHU_NHAN;
        }
        if (mon.getMa() == maMonDangChon) chon = static_cast<int>(i);
        cacThe.push_back(t);
    }
    luoiMonDat(luoi, cacThe, chon);
    gan(NUT_NHOM_THUCAN, L"Thức ăn (" + to_wstring(thucAn) + L")");
    gan(NUT_NHOM_NUOC, L"Nước uống (" + to_wstring(nuoc) + L")");
    datKieuNut(NUT_NHOM_THUCAN, nhomDatMon == 0 ? KieuNut::TabChon : KieuNut::Tab);
    datKieuNut(NUT_NHOM_NUOC, nhomDatMon == 1 ? KieuNut::TabChon : KieuNut::Tab);
    chonMonDat();
}

void CuaSoNhaHang::tinhTongGio(double& tamTinh, double& giam) const {
    tamTinh = 0;
    for (const YeuCauMon& muc : gioHang)
        for (const unique_ptr<Mon>& mon : dichVu.thucDon())
            if (mon->getMa() == muc.maMon) tamTinh += mon->getGia() * muc.soLuong;
    double phanTram = 0;
    if (dichVu.laKhachHang()) {
        phanTram = dichVu.hoSoCuaToi().getPhanTramGiam();
    } else {
        string kh = khachDaChon();
        for (const KhachHang& k : dichVu.khachHang()) if (k.getMa() == kh) phanTram = k.getPhanTramGiam();
    }
    giam = tamTinh * phanTram / 100;
}

void CuaSoNhaHang::napGio() {
    HWND bangGio = o(BANG_GIO);
    if (!bangGio) return;
    ListView_DeleteAllItems(bangGio);
    for (size_t i = 0; i < gioHang.size(); i++) {
        const YeuCauMon& muc = gioHang[i];
        for (const unique_ptr<Mon>& mon : dichVu.thucDon()) {
            if (mon->getMa() != muc.maMon) continue;
            string ten = mon->getTen();
            if (mon->getLoai() == "Nuoc uong") ten += " (" + muc.sizeLy + ")";
            dongBang(BANG_GIO, {rong(ten), so(muc.soLuong), tien(mon->getGia() * muc.soLuong)}, static_cast<int>(i));
        }
    }
    double tamTinh, giam;
    tinhTongGio(tamTinh, giam);
    if (trangHienTai == DAT_MON)
        gan(O_TONG, L"Tạm tính: " + tien(tamTinh) + L"\r\nGiảm giá: " + tien(giam) + L"\r\nDự kiến: " + tien(tamTinh - giam));
    else
        gan(O_TONG, L"Tạm tính: " + tien(tamTinh) + L"   •   Giảm: " + tien(giam) + L"\r\nThanh toán: " + tien(tamTinh - giam));
    bool coMon = !gioHang.empty();
    ShowWindow(bangGio, coMon ? SW_SHOW : SW_HIDE);
    EnableWindow(o(NUT_THANHTOAN), coMon);
    EnableWindow(o(NUT_GUI_DON), coMon);
    EnableWindow(o(NUT_XOA_GIO), coMon);
    EnableWindow(o(NUT_HUY_GIO), coMon);
    InvalidateRect(cuaSo, nullptr, FALSE);
}

void CuaSoNhaHang::veTongGio(HDC dc) {
    if (!o(BANG_GIO)) return;
    RECT r = vungTongGio();
    if (gioHang.empty()) {
        int y = BoCuc::Y0 + (trangHienTai == DAT_MON ? 0 : 100) + 72;
        int giua = (r.left + r.right) / 2;
        veVongTron(dc, giua, y + 56, 32, Mau::PHU_NEN);
        veIcon(dc, Icon::BAN_HANG, {giua - 32, y + 24, giua + 32, y + 88}, font.iconLon, Mau::CHINH);
        veChu(dc, L"Giỏ món đang trống.", {r.left, y + 100, r.right, y + 124}, font.nut, Mau::CHU, DT_CENTER);
        veChu(dc, L"Chọn món bên trái để bắt đầu.", {r.left, y + 124, r.right, y + 146}, font.nho, Mau::CHU_MO, DT_CENTER);
    }
    double tamTinh, giam;
    tinhTongGio(tamTinh, giam);
    int y = r.bottom - 104;
    veChu(dc, L"Tạm tính", {r.left, y, r.right, y + 24}, font.thuong, Mau::CHU_MO);
    veChu(dc, tien(tamTinh), {r.left, y, r.right, y + 24}, font.thuong, Mau::CHU, DT_RIGHT);
    veChu(dc, L"Giảm giá", {r.left, y + 28, r.right, y + 52}, font.thuong, Mau::CHU_MO);
    veChu(dc, giam > 0 ? L"− " + tien(giam) : tien(giam), {r.left, y + 28, r.right, y + 52}, font.thuong, Mau::TOT, DT_RIGHT);
    RECT hop = {r.left - 6, y + 60, r.right + 6, y + 104};
    veHinhBoGoc(dc, hop, 10, Mau::PHU_NEN);
    veChu(dc, trangHienTai == DAT_MON ? L"Dự kiến" : L"Thanh toán", {hop.left + 12, hop.top, hop.right, hop.bottom},
          font.nut, Mau::PHU_CHU);
    veChu(dc, tien(tamTinh - giam), {hop.left, hop.top, hop.right - 12, hop.bottom}, font.so, Mau::CHINH, DT_RIGHT);
}

void CuaSoNhaHang::doiSoLuong(int buoc) {
    int sl = 1;
    try { sl = stoi(duLieu(O_SOLUONG)); } catch (...) { sl = 1; }
    sl = max(1, min(999, sl + buoc));
    gan(O_SOLUONG, to_wstring(sl));
}

void CuaSoNhaHang::themVaoGio() {
    int i = monDatDaChon();
    if (i < 0) throw runtime_error("Chọn một món trong danh sách bên trái trước.");
    const Mon& mon = *dichVu.thucDon().at(i);
    int sl = static_cast<int>(soNhap(O_SOLUONG, "Số lượng", true));
    int daChon = 0;
    for (const YeuCauMon& muc : gioHang) if (muc.maMon == mon.getMa()) daChon += muc.soLuong;
    if (sl < 1 || sl > mon.getTonKho() - daChon) throw runtime_error("Số lượng phải lớn hơn 0 và không vượt tồn kho còn lại.");
    YeuCauMon muc;
    muc.maMon = mon.getMa();
    muc.soLuong = sl;
    muc.sizeLy = mon.getLoai() == "Nuoc uong" ? duLieu(O_SIZE) : "";
    muc.coDa = false;
    bool gop = false;
    for (YeuCauMon& cu : gioHang) {
        if (cu.maMon == muc.maMon && cu.sizeLy == muc.sizeLy) {
            cu.soLuong += sl;
            gop = true;
            break;
        }
    }
    if (!gop) gioHang.push_back(muc);
    napGio();
    napMonDat();
    thanhCong(L"Đã thêm " + rong(mon.getTen()) + L" vào giỏ. Tồn kho chỉ trừ khi thanh toán.");
}

void CuaSoNhaHang::thanhToan() {
    string ma = duLieu(O_MA), ngay = duLieu(O_NGAY), kh = khachDaChon(), nv = nhanVienDaChon();
    HoaDon truoc = dichVu.xemTruocHoaDon(ma, ngay, kh, nv, gioHang);
    if (!xacNhan(L"Xác nhận thanh toán " + tien(truoc.tongTien()) + L" và lưu hóa đơn?")) return;
    HoaDon daLuu = dichVu.thanhToan(ma, ngay, kh, nv, gioHang);
    gioHang.clear();
    maHoaDon = maHoaDonMoi();
    gan(O_MA, rong(maHoaDon));
    hienTrang(BAN_HANG);
    thanhCong(L"Đã thanh toán và lưu hóa đơn " + rong(daLuu.getMa()));
    xemHoaDon(cuaSo, daLuu, font);
}
