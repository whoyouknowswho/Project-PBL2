#include "CuaSo.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

void CuaSoNhaHang::trangTaiKhoan() {
    int x = BoCuc::X0 + 28, w = 504, y = BoCuc::Y0;
    themThe({BoCuc::X0, y, BoCuc::X0 + 560, y + 520});
    themDauThe(x, y + 24, w, Icon::TAI_KHOAN, L"Cấp tài khoản mới", L"Liên kết đúng hồ sơ nhân viên hoặc khách hàng.");
    nhap(L"Tên đăng nhập", O_USER, x, y + 88, w);
    chon(L"Vai trò", O_ROLE, x, y + 168, w, {L"Nhân viên", L"Quản lý", L"Khách hàng"});
    chon(L"Hồ sơ liên kết (nhân viên hoặc khách hàng)", O_NHANVIEN, x, y + 248, w, {});
    capNhatHoSoTaiKhoan();
    int nua = (w - 16) / 2;
    nhap(L"Mật khẩu (8–128 byte)", O_PASS, x, y + 328, nua, L"", true);
    nhap(L"Nhập lại mật khẩu", O_CONFIRM, x + nua + 16, y + 328, w - nua - 16, L"", true);
    nut(L"Tạo tài khoản", NUT_TAOTAIKHOAN, x, y + 416, w, KieuNut::Chinh, Icon::TAI_KHOAN, 44);
    nhan(L"Tên gồm chữ không dấu, chữ số, _ hoặc - (tối đa 32).", x, y + 472, w);
}

void CuaSoNhaHang::trangMatKhau() {
    int x = BoCuc::X0 + 28, w = 504, y = BoCuc::Y0;
    themThe({BoCuc::X0, y, BoCuc::X0 + 560, y + 444});
    themDauThe(x, y + 24, w, Icon::MAT_KHAU, L"Bảo vệ tài khoản của bạn", L"Nhập đúng mật khẩu hiện tại để đổi.");
    nhap(L"Mật khẩu hiện tại", O_OLDPASS, x, y + 88, w, L"", true);
    nhap(L"Mật khẩu mới", O_PASS, x, y + 168, w, L"", true);
    nhap(L"Nhập lại mật khẩu mới", O_CONFIRM, x, y + 248, w, L"", true);
    nut(L"Cập nhật mật khẩu", NUT_DOIMATKHAU, x, y + 340, w, KieuNut::Chinh, Icon::LUU, 44);
    nhan(L"Mật khẩu mới từ 8 đến 128 byte.", x, y + 396, w);
}

void CuaSoNhaHang::trangHoSoKhachHang() {
    themThe({BoCuc::X0, BoCuc::Y0, phaiNoiDung(), BoCuc::Y0 + 490});
}

void CuaSoNhaHang::veHoSoKhachHang(HDC dc) {
    const KhachHang& k = dichVu.hoSoCuaToi();
    int trai = BoCuc::X0 + 28, phai = phaiNoiDung() - 28, y = BoCuc::Y0;
    wstring ten = rong(k.getTen());
    wstring chuDau = ten.empty() ? L"?" : ten.substr(0, 1);
    CharUpperBuffW(&chuDau[0], 1);
    veVongTron(dc, trai + 30, y + 58, 30, Mau::CHINH);
    veChu(dc, chuDau, {trai, y + 28, trai + 60, y + 88}, font.tieuDe, RGB(255,255,255), DT_CENTER);
    veChu(dc, ten, {trai + 76, y + 28, phai, y + 62}, font.tieuDe, Mau::CHU);
    wstring hang = tenHangVip(k.getHangVip());
    COLORREF nen, mau;
    mauHuyHieu(hang, nen, mau);
    wstring nhanHang = L"Hạng " + hang;
    int rongHang = doRongHuyHieu(dc, nhanHang, font.nhoDam);
    veHuyHieu(dc, nhanHang, {trai + 76, y + 66, trai + 76 + rongHang, y + 90}, nen, mau, font.nhoDam);
    veChu(dc, L"Mã khách hàng: " + rong(k.getMa()), {trai + 88 + rongHang, y + 66, phai, y + 90}, font.nho, Mau::CHU_MO);
    RECT ke = {trai, y + 112, phai, y + 113};
    toNen(dc, ke, Mau::VIEN);

    wstring email = rong(k.getEmail());
    const pair<wstring, wstring> truong[8] = {
        {L"Mã khách hàng", rong(k.getMa())}, {L"Điểm tích lũy", so(k.getDiemTichLuy())},
        {L"Họ và tên", ten}, {L"Hạng thành viên", hang},
        {L"Điện thoại", rong(k.getSoDienThoai())}, {L"Ưu đãi giảm giá", so(k.getPhanTramGiam()) + L"%"},
        {L"Giới tính", tenGioiTinh(k.getGioiTinh())}, {L"Email", email.empty() ? L"—" : email}};
    int cot = (phai - trai - 24) / 2;
    for (int i = 0; i < 8; i++) {
        int x = trai + (i % 2) * (cot + 24), dinh = y + 132 + (i / 2) * 76;
        veChu(dc, truong[i].first, {x, dinh, x + cot, dinh + 20}, font.nhoDam, Mau::CHU_MO);
        RECT o = {x, dinh + 24, x + cot, dinh + 64};
        veHinhBoGoc(dc, o, 10, Mau::O_CHI_DOC, Mau::VIEN);
        veChu(dc, truong[i].second, {o.left + 14, o.top, o.right - 12, o.bottom}, font.thuong, Mau::CHU);
    }
    veIcon(dc, Icon::THONG_TIN, {trai, y + 442, trai + 18, y + 464}, font.icon, Mau::CHINH);
    veChu(dc, L"Liên hệ nhân viên nếu cần cập nhật thông tin.", {trai + 26, y + 442, phai, y + 464}, font.nho, Mau::CHU_MO);
}

void CuaSoNhaHang::trangLichSuKhachHang() {
    const KhachHang& k = dichVu.hoSoCuaToi();
    int x = BoCuc::X0, y = BoCuc::Y0, phai = phaiNoiDung(), day = dayNoiDung();
    themThe({x, y, phai, day});
    nut(L"Xem hóa đơn đã chọn", NUT_HOADON, phai - 24 - 230, y + 18, 230, KieuNut::Chinh, Icon::HOA_DON);
    themDauThe(x + 24, y + 18, phai - x - 48 - 242, Icon::HOA_DON, L"Khách hàng: " + rong(k.getMa() + " - " + k.getTen()),
               L"Chọn hóa đơn rồi nhấn Xem, hoặc nhấp đúp vào dòng.");
    bang(BANG_CHINH, x + 12, y + 72, phai - x - 24, day - y - 84,
         {{L"Mã hóa đơn", 280}, {L"Ngày lập", 150}, {L"Số loại món", 130}, {L"Thanh toán", 180}});
    canCot(BANG_CHINH, 2, LVCFMT_CENTER);
    canCot(BANG_CHINH, 3, LVCFMT_RIGHT);
    hoaDonBaoCao = dichVu.hoaDonCuaToi();
    reverse(hoaDonBaoCao.begin(), hoaDonBaoCao.end());
    for (size_t i = 0; i < hoaDonBaoCao.size(); i++) {
        const HoaDon& h = hoaDonBaoCao[i];
        dongBang(BANG_CHINH, {rong(h.getMa()), rong(h.getNgayLap()), to_wstring(h.getGioHang().size()), tien(h.tongTien())},
                 static_cast<int>(i));
    }
    EnableWindow(o(NUT_HOADON), !hoaDonBaoCao.empty());
    thanhCong(L"Có " + to_wstring(hoaDonBaoCao.size()) + L" hóa đơn thuộc tài khoản của bạn.");
}

string CuaSoNhaHang::vaiTroDuocChon() const {
    int vaiTro = luaChon(O_ROLE);
    if (vaiTro == 0) return "NHANVIEN";
    if (vaiTro == 1) return "QUANLY";
    if (vaiTro == 2) return "KHACHHANG";
    throw runtime_error("Hãy chọn vai trò tài khoản.");
}

void CuaSoNhaHang::capNhatHoSoTaiKhoan() {
    string vaiTro = vaiTroDuocChon();
    SendMessageW(o(O_NHANVIEN), CB_RESETCONTENT, 0, 0);
    vector<wstring> ds;
    if (vaiTro == "NHANVIEN") {
        for (const NhanVien& n : dichVu.nhanVien()) ds.push_back(rong(n.getMa() + " - " + n.getTen()));
    } else if (vaiTro == "KHACHHANG") {
        for (const KhachHang& k : dichVu.khachHang()) ds.push_back(rong(k.getMa() + " - " + k.getTen()));
    } else {
        ds.push_back(L"Không cần liên kết hồ sơ");
    }
    for (const wstring& muc : ds) SendMessageW(o(O_NHANVIEN), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(muc.c_str()));
    if (!ds.empty()) chonMuc(O_NHANVIEN, 0);
    EnableWindow(o(O_NHANVIEN), vaiTro != "QUANLY");
    InvalidateRect(o(O_NHANVIEN), nullptr, FALSE);
}

string CuaSoNhaHang::maLienKetDuocChon() const {
    string vaiTro = vaiTroDuocChon();
    if (vaiTro == "QUANLY") return "";
    int i = luaChon(O_NHANVIEN);
    if (i < 0) throw runtime_error("Chưa có hồ sơ liên kết. Hãy thêm khách hàng hoặc nhân viên trước.");
    if (vaiTro == "NHANVIEN") return dichVu.nhanVien().at(i).getMa();
    return dichVu.khachHang().at(i).getMa();
}
