#include "CuaSo.h"
#include "LuoiMon.h"
#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"
#include <commdlg.h>
#include <objidl.h>
#include <gdiplus.h>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
wstring chuTim(const string& chu) {
    wstring k = rong(chu);
    if (!k.empty()) CharLowerBuffW(&k[0], static_cast<DWORD>(k.size()));
    return k;
}
}

// Thuc don: thanh cong cu o tren, luoi the mon ben duoi; quan ly co them the bieu mau ben phai.
void CuaSoNhaHang::trangThucDon() {
    bool quanLy = dichVu.laQuanLy();
    int phai = phaiNoiDung(), day = dayNoiDung();
    int rongForm = 364, xForm = phai - rongForm;
    int phaiLuoi = quanLy ? xForm - BoCuc::KHOANG : phai;
    int x = BoCuc::X0, y = BoCuc::Y0;

    // Vung hep (cua so nho, co bieu mau quan ly): o "Chi hien mon con hang" xuong dong rieng.
    bool hep = phaiLuoi - x < 680;
    int caoCongCu = hep ? 164 : 128;
    themThe({x, y, phaiLuoi, y + caoCongCu});
    int rongSapXep = 176, rongTim = phaiLuoi - x - 48 - rongSapXep - 12;
    nhap(L"", TIM_KIEM, x + 24, y - 4, rongTim);
    SendMessageW(o(TIM_KIEM), EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Tìm món theo mã hoặc tên"));
    chon(L"", SAP_XEP, x + 24 + rongTim + 12, y - 4, rongSapXep,
         {L"Mặc định", L"Tên A–Z", L"Giá tăng", L"Giá giảm"});
    int rongTab = hep ? (phaiLuoi - x - 48 - 12) / 2 : 160;
    nut(L"Thức ăn", NUT_NHOM_THUCAN, x + 24, y + 72, rongTab, nhomThucDon == 0 ? KieuNut::TabChon : KieuNut::Tab);
    nut(L"Nước uống", NUT_NHOM_NUOC, x + 36 + rongTab, y + 72, rongTab, nhomThucDon == 1 ? KieuNut::TabChon : KieuNut::Tab);
    if (hep) danhDau(L"Chỉ hiện món còn hàng", CON_HANG, x + 24, y + 124, 240);
    else danhDau(L"Chỉ hiện món còn hàng", CON_HANG, x + 380, y + 78, min(240, phaiLuoi - x - 380 - 16));
    taoLuoiMon(cuaSo, DANH_SACH_THUCDON, {x - 6, y + caoCongCu + 12, phaiLuoi + 6, day}, &font, &anh);

    if (!quanLy) return;
    int fx = xForm + 24, w = rongForm - 48, nua = (w - 16) / 2;
    themThe({xForm, y, phai, y + 568});
    themDauThe(fx, y + 20, w, Icon::THUC_DON, L"Thông tin món", L"Chọn thẻ món để sửa hoặc thêm mới.");
    nhap(L"Mã món (tự động)", O_MA, fx, y + 68, nua);
    SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    chon(L"Loại", O_LOAI, fx + nua + 16, y + 68, w - nua - 16, {L"Món ăn", L"Nước uống"});
    nhap(L"Tên món", O_TEN, fx, y + 134, w);
    nhap(L"Giá bán (đ)", O_GIA, fx, y + 200, nua, L"0");
    nhap(L"Tồn kho", O_TON, fx + nua + 16, y + 200, w - nua - 16, L"0");
    nhap(L"Độ cay (0–5)", O_CAY, fx, y + 266, nua, L"0");
    HWND nhanSize = nhan(L"Size nước", fx + nua + 16, y + 266, w - nua - 16, false, NHAN_SIZE);
    SendMessageW(nhanSize, WM_SETFONT, reinterpret_cast<WPARAM>(font.nhoDam), TRUE);
    mauChuNhan[nhanSize] = Mau::CHU_NHAN;
    chon(L"", O_SIZE, fx + nua + 16, y + 266, w - nua - 16, {L"S", L"M", L"L"});
    chonMuc(O_SIZE, 1);
    danhDau(L"Món chay", O_CHAY, fx + nua + 16, y + 296, w - nua - 16);
    nut(L"Chọn hình ảnh cho món...", NUT_CHON_ANH, fx, y + 340, w, KieuNut::Phu, Icon::THEM);
    nut(L"Lưu thông tin", NUT_LUU, fx, y + 392, w - 112, KieuNut::Chinh, Icon::LUU);
    nut(L"Xóa", NUT_XOA, fx + w - 100, y + 392, 100, KieuNut::NguyHiem, Icon::XOA);
    nut(L"Thêm mới / Làm trống", NUT_MOI, fx, y + 440, w, KieuNut::Phu, Icon::THEM);
    nhap(L"Nhập thêm tồn kho", O_NHAPKHO, fx, y + 488, nua, L"1");
    nut(L"Nhập kho", NUT_NHAPKHO, fx + nua + 16, y + 512, w - nua - 16, KieuNut::Phu, Icon::THEM);
    EnableWindow(o(NUT_XOA), !maDangSua.empty());
}

int CuaSoNhaHang::monThucDonDaChon() const {
    HWND luoi = o(DANH_SACH_THUCDON);
    if (!luoi) return -1;
    return static_cast<int>(SendMessageW(luoi, LM_LAYCHON, 0, 0));
}

void CuaSoNhaHang::doiLoaiMon() {
    if (trangHienTai != THUC_DON || !dichVu.laQuanLy()) return;
    bool monAn = luaChon(O_LOAI) == 0;
    if (maDangSua.empty() && o(O_MA)) {
        gan(O_MA, rong(dichVu.maMonMoi(monAn ? "Mon an" : "Nuoc uong")));
        SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    }
    EnableWindow(o(O_CAY), monAn);
    EnableWindow(o(O_CHAY), monAn);
    EnableWindow(o(O_SIZE), !monAn);
    // Size nuoc va o Mon chay dung chung mot cho.
    ShowWindow(o(O_CHAY), monAn ? SW_SHOW : SW_HIDE);
    ShowWindow(o(NHAN_SIZE), monAn ? SW_HIDE : SW_SHOW);
    ShowWindow(o(O_SIZE), monAn ? SW_HIDE : SW_SHOW);
    EnableWindow(o(NUT_NHAPKHO), !maDangSua.empty());
    InvalidateRect(cuaSo, nullptr, FALSE);
}

void CuaSoNhaHang::capNhatNutAnh() {
    if (!o(NUT_CHON_ANH)) return;
    if (anhDangChon.empty()) {
        gan(NUT_CHON_ANH, L"Chọn hình ảnh cho món...");
        return;
    }
    wstring tenTep = rong(anhDangChon);
    size_t viTri = tenTep.find_last_of(L"\\/");
    if (viTri != wstring::npos) tenTep = tenTep.substr(viTri + 1);
    gan(NUT_CHON_ANH, L"Ảnh: " + tenTep);
}

void CuaSoNhaHang::chonAnhMon() {
    if (trangHienTai != THUC_DON || !dichVu.laQuanLy()) return;
    wchar_t duongDan[MAX_PATH] = {};
    OPENFILENAMEW hop = {};
    hop.lStructSize = sizeof(hop);
    hop.hwndOwner = cuaSo;
    hop.lpstrFilter = L"Tệp hình ảnh (*.png;*.jpg;*.jpeg;*.bmp)\0*.png;*.jpg;*.jpeg;*.bmp\0Tất cả tệp (*.*)\0*.*\0";
    hop.lpstrFile = duongDan;
    hop.nMaxFile = MAX_PATH;
    hop.lpstrTitle = L"Chọn hình ảnh cho món ăn / thức uống";
    hop.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    if (!GetOpenFileNameW(&hop)) return;
    Gdiplus::Bitmap bmp(duongDan);
    if (bmp.GetLastStatus() != Gdiplus::Ok || bmp.GetWidth() == 0 || bmp.GetHeight() == 0)
        throw runtime_error("Tệp hình ảnh không hợp lệ hoặc không thể đọc được.");
    anhDangChon = hep(duongDan);
    capNhatNutAnh();
}

void CuaSoNhaHang::napThucDonAnh() {
    HWND luoi = o(DANH_SACH_THUCDON);
    if (!luoi) return;
    bool quanLy = dichVu.laQuanLy();
    wstring tim = chuTim(duLieu(TIM_KIEM));
    const auto& menu = dichVu.thucDon();
    vector<int> ketQua;
    int soThucAn = 0, soNuoc = 0;
    for (size_t i = 0; i < menu.size(); i++) {
        const Mon& mon = *menu[i];
        bool laNuoc = mon.getLoai() == "Nuoc uong";
        if (laNuoc) soNuoc++; else soThucAn++;
        if (laNuoc != (nhomThucDon == 1)) continue;
        if (chuTim(mon.getMa() + " " + mon.getTen()).find(tim) == wstring::npos) continue;
        if (daDanhDau(CON_HANG) && mon.getTonKho() == 0) continue;
        ketQua.push_back(static_cast<int>(i));
    }
    int cach = luaChon(SAP_XEP);
    if (cach > 0) stable_sort(ketQua.begin(), ketQua.end(), [&](int a, int b) {
        if (cach == 1) return chuTim(menu[a]->getTen()) < chuTim(menu[b]->getTen());
        if (cach == 2) return menu[a]->getGia() < menu[b]->getGia();
        return menu[a]->getGia() > menu[b]->getGia();
    });
    vector<TheMon> cacThe;
    int chon = -1;
    for (int i : ketQua) {
        const Mon& mon = *menu[i];
        TheMon t;
        t.duLieu = i;
        t.maMon = mon.getMa();
        t.loaiMon = mon.getLoai();
        t.tenMon = mon.getTen();
        t.hinhAnh = mon.getHinhAnh();
        t.ten = rong(mon.getTen());
        t.gia = tien(mon.getGia());
        const MonAn* thucAn = dynamic_cast<const MonAn*>(&mon);
        if (thucAn) t.thongTin = L"Độ cay: " + to_wstring(thucAn->getDoCay()) + L"/5 · " + (thucAn->laMonChay() ? L"Món chay" : L"Món mặn");
        else t.thongTin = L"Size " + rong(dynamic_cast<const NuocUong&>(mon).getSizeLy());
        t.thongTin += L" · " + rong(mon.getMa());
        if (quanLy) {
            t.huyHieu = mon.getTonKho() == 0 ? L"Hết hàng" : L"Tồn kho: " + to_wstring(mon.getTonKho());
            mauHuyHieu(mon.getTonKho() == 0 ? L"Hết hàng" : mon.getTonKho() <= 5 ? L"Sắp hết" : L"", t.nenHuyHieu, t.mauHuyHieu);
            if (mon.getTonKho() > 5) { t.nenHuyHieu = Mau::THE; t.mauHuyHieu = Mau::CHU_NHAN; }
        } else if (mon.getTonKho() == 0) {
            t.huyHieu = L"Hết hàng";
            mauHuyHieu(t.huyHieu, t.nenHuyHieu, t.mauHuyHieu);
            t.mo = true;
        }
        if (mon.getMa() == maDangSua) chon = i;
        cacThe.push_back(t);
    }
    luoiMonDat(luoi, cacThe, chon);
    gan(NUT_NHOM_THUCAN, L"Thức ăn (" + to_wstring(soThucAn) + L")");
    gan(NUT_NHOM_NUOC, L"Nước uống (" + to_wstring(soNuoc) + L")");
    datKieuNut(NUT_NHOM_THUCAN, nhomThucDon == 0 ? KieuNut::TabChon : KieuNut::Tab);
    datKieuNut(NUT_NHOM_NUOC, nhomThucDon == 1 ? KieuNut::TabChon : KieuNut::Tab);
    thanhCong(to_wstring(ketQua.size()) + L" món · Ảnh minh họa · " +
              (quanLy ? L"Chọn món để chỉnh sửa thông tin." : L"Chọn Thức ăn / Nước uống hoặc sắp xếp theo giá."));
}
