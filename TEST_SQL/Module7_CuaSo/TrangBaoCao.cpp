#include "CuaSo.h"
#include <algorithm>

using namespace std;

namespace {
const int CAO_LOC = 116, CAO_SO_LIEU = 104;
}

void CuaSoNhaHang::trangBaoCao() {
    int x = BoCuc::X0, y = BoCuc::Y0, phai = phaiNoiDung(), day = dayNoiDung();
    themThe({x, y, phai, y + CAO_LOC});
    int rongChip = 104 + 120 + 100 + 24;
    int rongO = phai - x - 48 - 132 - 24 - rongChip;
    nhap(L"Ngày / tháng cần xem", LOC_BAO_CAO, x + 24, y + 12, rongO, rong(phamViBaoCao));
    SendMessageW(o(LOC_BAO_CAO), EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"dd/mm/yyyy hoặc mm/yyyy"));
    nut(L"Áp dụng", NUT_BAOCAO, x + 24 + rongO + 12, y + 36, 120, KieuNut::Chinh);
    int xc = phai - 24 - rongChip;
    nut(L"Hôm nay", NUT_BC_HOMNAY, xc, y + 38, 104, KieuNut::Chip, 0, 36);
    nut(L"Tháng này", NUT_BC_THANG, xc + 116, y + 38, 120, KieuNut::Chip, 0, 36);
    nut(L"Tất cả", NUT_BC_TATCA, xc + 248, y + 38, 100, KieuNut::Chip, 0, 36);
    HWND ky = nhan(L"", x + 24, y + 86, (phai - x) / 2 - 24, false, NHAN_KY_BAOCAO);
    SendMessageW(ky, WM_SETFONT, reinterpret_cast<WPARAM>(font.nhoDam), TRUE);
    mauChuNhan[ky] = Mau::CHINH;
    HWND tong = tao(L"STATIC", L"", SS_RIGHT | SS_ENDELLIPSIS | SS_NOPREFIX | SS_CENTERIMAGE, O_TONG,
                    (x + phai) / 2, y + 86, (phai - x) / 2 - 24, 20);
    SendMessageW(tong, WM_SETFONT, reinterpret_cast<WPARAM>(font.nho), TRUE);

    int yBang = y + CAO_LOC + 16 + CAO_SO_LIEU + 16;
    int trai = (phai - x - BoCuc::KHOANG) * 2 / 3, xPhai = x + trai + BoCuc::KHOANG;
    themThe({x, yBang, x + trai, day});
    nut(L"Xem hóa đơn đã chọn", NUT_HOADON, x + trai - 24 - 210, yBang + 18, 210, KieuNut::Phu, Icon::HOA_DON);
    themDauThe(x + 24, yBang + 18, trai - 48 - 222, Icon::HOA_DON, L"Danh sách hóa đơn",
               L"Mới nhất ở đầu · Chọn dòng hoặc nhấp đúp để xem");
    bang(BANG_CHINH, x + 12, yBang + 72, trai - 24, day - yBang - 84,
         {{L"Mã hóa đơn", 3}, {L"Ngày lập", 2}, {L"Thanh toán", 2}});
    canCot(BANG_CHINH, 2, LVCFMT_RIGHT);
    themThe({xPhai, yBang, phai, day});
    themDauThe(xPhai + 24, yBang + 18, phai - xPhai - 48, Icon::CANH_BAO, L"Cần nhập thêm", L"Tồn kho hiện tại (≤ 5)");
    bang(BANG_TON, xPhai + 12, yBang + 72, phai - xPhai - 24, day - yBang - 84, {{L"Tên món", 3}, {L"Còn lại", 2}}, 1);
    canCot(BANG_TON, 1, LVCFMT_RIGHT);
    napBaoCao();
}

void CuaSoNhaHang::napBaoCao() {
    // Chi thay ket qua dang hien sau khi bo loc da duoc kiem tra thanh cong.
    BaoCaoKinhDoanh ketQua = dichVu.baoCao(duLieu(LOC_BAO_CAO));
    phamViBaoCao = duLieu(LOC_BAO_CAO);
    baoCaoHienTai = ketQua;
    hoaDonBaoCao = ketQua.hoaDon;
    reverse(hoaDonBaoCao.begin(), hoaDonBaoCao.end());
    bool cu = dangVeBang;
    dangVeBang = true;
    ListView_DeleteAllItems(o(BANG_CHINH));
    ListView_DeleteAllItems(o(BANG_TON));
    for (size_t i = 0; i < hoaDonBaoCao.size(); i++) {
        const HoaDon& hd = hoaDonBaoCao[i];
        dongBang(BANG_CHINH, {rong(hd.getMa()), rong(hd.getNgayLap()), tien(hd.tongTien())}, static_cast<int>(i));
    }
    if (hoaDonBaoCao.empty()) dongBang(BANG_CHINH, {L"Chưa có hóa đơn", L"", L""}, -1);
    int canNhap = 0;
    for (const unique_ptr<Mon>& mon : dichVu.thucDon()) {
        if (mon->getTonKho() > 5) continue;
        dongBang(BANG_TON, {rong(mon->getTen()), mon->getTonKho() == 0 ? L"Hết hàng" : so(mon->getTonKho())}, canNhap++);
    }
    if (canNhap == 0) dongBang(BANG_TON, {L"Tồn kho ổn định", L""}, -1);
    gan(NHAN_KY_BAOCAO, L"Đang xem: " + kyBaoCao(phamViBaoCao));
    gan(O_TONG, L"Hiển thị " + to_wstring(hoaDonBaoCao.size()) + L" hóa đơn  ·  " + to_wstring(canNhap) +
                L" món cần nhập thêm  ·  Doanh thu đã trừ giảm giá thành viên.");
    EnableWindow(o(NUT_HOADON), FALSE);
    for (int id : {NUT_BC_HOMNAY, NUT_BC_THANG, NUT_BC_TATCA}) {
        bool chon = (id == NUT_BC_HOMNAY && phamViBaoCao == homNay()) ||
                    (id == NUT_BC_THANG && phamViBaoCao == homNay().substr(3)) ||
                    (id == NUT_BC_TATCA && phamViBaoCao.empty());
        datKieuNut(id, chon ? KieuNut::ChipChon : KieuNut::Chip);
    }
    dangVeBang = cu;
    thanhCong(L"Báo cáo chỉ đọc · Chọn thời gian rồi nhấn Áp dụng hoặc dùng nút lọc nhanh.");
    InvalidateRect(cuaSo, nullptr, FALSE);
}

void CuaSoNhaHang::veThongKeBaoCao(HDC dc) {
    int x = BoCuc::X0, w = phaiNoiDung() - x, y = BoCuc::Y0 + CAO_LOC + 16;
    int rongThe = (w - 2 * BoCuc::KHOANG) / 3;
    const wstring nhanThe[] = {L"DOANH THU", L"SỐ HÓA ĐƠN", L"MÓN BÁN CHẠY"};
    const wstring giaTri[] = {tien(baoCaoHienTai.doanhThu), to_wstring(baoCaoHienTai.soHoaDon), rong(baoCaoHienTai.monBanChay)};
    const wstring ghiChu[] = {L"Sau giảm giá thành viên", L"Đã thanh toán trong kỳ", L"Số lượng bán trong kỳ"};
    const wchar_t icon[] = {Icon::TIEN, Icon::HOA_DON, Icon::NGOI_SAO};
    for (int i = 0; i < 3; i++) {
        int trai = x + i * (rongThe + BoCuc::KHOANG), phai = i == 2 ? x + w : trai + rongThe;
        RECT the = {trai, y, phai, y + CAO_SO_LIEU};
        veThe(dc, the);
        veVongTron(dc, trai + 42, y + 52, 22, Mau::PHU_NEN);
        veIcon(dc, icon[i], {trai + 20, y + 30, trai + 64, y + 74}, font.iconLon, Mau::CHINH);
        veChu(dc, nhanThe[i], {trai + 80, y + 16, phai - 16, y + 36}, font.nhoDam, Mau::CHU_MO);
        veChu(dc, giaTri[i], {trai + 80, y + 38, phai - 16, y + 70},
              i == 2 ? font.dam : fontVua(dc, giaTri[i], phai - 16 - trai - 80, font.so, font.dam),
              i == 0 ? Mau::CHINH : Mau::CHU);
        veChu(dc, ghiChu[i], {trai + 80, y + 72, phai - 16, y + 92}, font.nho, Mau::CHU_MO);
    }
}
