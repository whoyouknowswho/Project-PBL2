#include "CuaSo.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

void CuaSoNhaHang::trangDonDat() {
    donDatHienThi = dichVu.danhSachDonDat();
    reverse(donDatHienThi.begin(), donDatHienThi.end());
    bool khach = dichVu.laKhachHang();
    int x = BoCuc::X0, phai = phaiNoiDung(), day = dayNoiDung(), y = BoCuc::Y0;
    int caoTren = (day - y - BoCuc::KHOANG) * 11 / 20;

    themThe({x, y, phai, y + caoTren});
    int xNut = phai - 24 - 120;
    nut(L"Làm mới", NUT_LAMMOI_DON, xNut, y + 20, 120, KieuNut::Phu, Icon::LAM_MOI);
    xNut -= 12 + 176;
    nut(L"Hủy đơn đã chọn", NUT_HUY_DON, xNut, y + 20, 176, KieuNut::NguyHiem, Icon::HUY);
    if (!khach) {
        xNut -= 12 + 176;
        nut(L"Thanh toán đơn", NUT_THANHTOAN_DON, xNut, y + 20, 176, KieuNut::Chinh, Icon::TIEN);
    }
    themDauThe(x + 24, y + 20, xNut - x - 48, Icon::DON_DAT, khach ? L"Đơn đặt của tôi" : L"Danh sách đơn",
               L"Chỉ đơn chờ mới được hủy hoặc thanh toán.");
    bang(BANG_CHINH, x + 12, y + 76, phai - x - 24, caoTren - 88,
         {{L"Mã đơn", 95}, {L"Ngày đặt", 110}, {L"Khách hàng", 130}, {L"Trạng thái", 150},
          {L"Tạm tính sau giảm", 170}, {L"Hóa đơn", 150}}, 3);
    canCot(BANG_CHINH, 4, LVCFMT_RIGHT);

    int yDuoi = y + caoTren + BoCuc::KHOANG;
    themThe({x, yDuoi, phai, day});
    themDauThe(x + 24, yDuoi + 18, phai - x - 48, Icon::HOA_DON, L"Chi tiết đơn đã chọn");
    bang(BANG_GIO, x + 12, yDuoi + 68, phai - x - 24, day - yDuoi - 80,
         {{L"Món / size / đá", 380}, {L"Số lượng", 90}, {L"Đơn giá lúc đặt", 155}, {L"Thành tiền", 155}});
    canCot(BANG_GIO, 1, LVCFMT_CENTER);
    canCot(BANG_GIO, 2, LVCFMT_RIGHT);
    canCot(BANG_GIO, 3, LVCFMT_RIGHT);

    for (size_t i = 0; i < donDatHienThi.size(); i++) {
        const DonDatMon& don = donDatHienThi[i];
        dongBang(BANG_CHINH, {rong(don.duTinh.getMa()), rong(don.duTinh.getNgayLap()), rong(don.duTinh.getMaKhachHang()),
                              tenTrangThaiDon(don.trangThai), tien(don.duTinh.tongTien()), rong(don.maHoaDon)},
                 static_cast<int>(i));
    }
    EnableWindow(o(NUT_HUY_DON), FALSE);
    EnableWindow(o(NUT_THANHTOAN_DON), FALSE);
    thanhCong(donDatHienThi.empty() ? L"Chưa có đơn đặt món." : L"Đơn đã gửi được lưu lại khi đóng chương trình.");
}

void CuaSoNhaHang::chiTietDonDat() {
    int i = dongChon(BANG_CHINH);
    if (i < 0 || static_cast<size_t>(i) >= donDatHienThi.size()) return;
    const DonDatMon& don = donDatHienThi[i];
    ListView_DeleteAllItems(o(BANG_GIO));
    for (const ChiTietHoaDon& ct : don.duTinh.getGioHang())
        dongBang(BANG_GIO, {rong(ct.tenMon), so(ct.soLuong), tien(ct.donGia), tien(ct.thanhTien())}, 0);
    EnableWindow(o(NUT_HUY_DON), don.trangThai == "CHO");
    EnableWindow(o(NUT_THANHTOAN_DON), don.trangThai == "CHO");
}

void CuaSoNhaHang::xuLyDonDat(int id) {
    if (id == NUT_GUI_DON) {
        HoaDon duTinh = dichVu.duTinhDatMon(gioHang);
        if (!xacNhan(L"Gửi đơn với số tiền tạm tính " + tien(duTinh.tongTien()) +
                     L"?\nGiá, ưu đãi và tồn kho được xác nhận lại khi thanh toán tại nhà hàng.")) return;
        string ma = dichVu.datMon(homNay(), gioHang);
        gioHang.clear();
        hienTrang(DON_DAT);
        thanhCong(L"Đã gửi đơn " + rong(ma) + L". Nhân viên sẽ tiếp nhận đơn của bạn.");
        return;
    }
    if (id == NUT_LAMMOI_DON) { hienTrang(DON_DAT); return; }
    int i = dongChon(BANG_CHINH);
    if (i < 0) throw runtime_error("Hãy chọn một đơn đặt món.");
    string ma = donDatHienThi.at(i).duTinh.getMa();
    if (id == NUT_HUY_DON) {
        if (!xacNhan(L"Hủy đơn " + rong(ma) + L"?")) return;
        dichVu.huyDonDat(ma);
        hienTrang(DON_DAT);
        thanhCong(L"Đã hủy đơn " + rong(ma));
    } else if (id == NUT_THANHTOAN_DON) {
        string nv = nhanVienDaChon();
        HoaDon truoc = dichVu.xemTruocDonDat(ma, homNay(), nv);
        if (!xacNhan(L"Xác nhận thanh toán đơn " + rong(ma) + L": " + tien(truoc.tongTien()) +
                     L"?\nĐây là tổng theo giá và ưu đãi hiện tại. Tồn kho và điểm sẽ được cập nhật.")) return;
        HoaDon hoaDon = dichVu.thanhToanDonDat(ma, homNay(), nv);
        hienTrang(DON_DAT);
        thanhCong(L"Đã thanh toán đơn " + rong(ma));
        xemHoaDon(cuaSo, hoaDon, font);
    }
}
