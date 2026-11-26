#include "CuaSo.h"
#include <objidl.h>
#include <gdiplus.h>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
wstring tieuDeTrang(int trang, bool khach) {
    switch (trang) {
        case TRANG_CHU: return L"Tổng quan";
        case KHACH_HANG: return L"Khách hàng";
        case NHAN_VIEN: return L"Nhân viên";
        case DAT_MON: return L"Đặt món";
        case DON_DAT: return khach ? L"Đơn đặt của tôi" : L"Đơn khách đặt";
        case THUC_DON: return L"Thực đơn";
        case BAN_HANG: return L"Bán hàng";
        case BAO_CAO: return L"Báo cáo kinh doanh";
        case TAI_KHOAN: return L"Tạo tài khoản";
        case MAT_KHAU: return L"Đổi mật khẩu";
        case HO_SO_CUA_TOI: return L"Thông tin của tôi";
        case HOA_DON_CUA_TOI: return L"Hóa đơn của tôi";
    }
    return L"";
}

wstring moTaTrang(int trang, bool khach) {
    switch (trang) {
        case TRANG_CHU: return khach ? L"Chào mừng bạn. Cùng khám phá thực đơn và ưu đãi thành viên."
                                     : L"Một ngày làm việc hiệu quả bắt đầu từ đây.";
        case KHACH_HANG: return L"Tìm kiếm, chăm sóc và cập nhật thông tin khách hàng.";
        case NHAN_VIEN: return L"Quản lý hồ sơ và thu nhập của đội ngũ.";
        case THUC_DON: return L"Khám phá món ăn và nước uống qua hình ảnh, giá và thông tin chi tiết.";
        case BAN_HANG: return L"Chọn món, thêm vào giỏ và thanh toán nhanh chóng.";
        case DAT_MON: return L"Chọn món yêu thích và gửi đơn để nhân viên tiếp nhận.";
        case DON_DAT: return L"Đơn chờ có thể hủy. Giá và ưu đãi được xác nhận lại khi thanh toán.";
        case BAO_CAO: return L"Theo dõi hóa đơn, doanh thu và các món cần nhập thêm.";
        case TAI_KHOAN: return L"Cấp tài khoản cho quản lý, nhân viên và khách hàng.";
        case HO_SO_CUA_TOI: return L"Thông tin cá nhân, điểm tích lũy và ưu đãi thành viên.";
        case HOA_DON_CUA_TOI: return L"Xem lại các hóa đơn mua hàng của riêng bạn.";
        case MAT_KHAU: return L"Cập nhật mật khẩu cho tài khoản đang đăng nhập.";
    }
    return L"";
}

wchar_t iconTrang(int trang) {
    switch (trang) {
        case TRANG_CHU: return Icon::TRANG_CHU;
        case KHACH_HANG: return Icon::KHACH;
        case NHAN_VIEN: return Icon::NHAN_VIEN;
        case THUC_DON: return Icon::THUC_DON;
        case BAN_HANG: return Icon::BAN_HANG;
        case DAT_MON: return Icon::BAN_HANG;
        case DON_DAT: return Icon::DON_DAT;
        case BAO_CAO: return Icon::BAO_CAO;
        case TAI_KHOAN: return Icon::TAI_KHOAN;
        case MAT_KHAU: return Icon::MAT_KHAU;
        case HO_SO_CUA_TOI: return Icon::HO_SO;
        case HOA_DON_CUA_TOI: return Icon::HOA_DON;
        case DANG_XUAT: return Icon::DANG_XUAT;
    }
    return 0;
}
}

// ---------------------------------------------------------------- Chuyen trang

void CuaSoNhaHang::thanhBen() {
    bool khach = dichVu.laKhachHang();
    vector<pair<int, wstring>> muc;
    if (khach) {
        muc = {{TRANG_CHU, L"Tổng quan"}, {THUC_DON, L"Thực đơn"}, {HOA_DON_CUA_TOI, L"Hóa đơn của tôi"},
               {DAT_MON, L"Đặt món"}, {DON_DAT, L"Đơn đặt của tôi"}, {MAT_KHAU, L"Đổi mật khẩu"},
               {HO_SO_CUA_TOI, L"Thông tin của tôi"}};
    } else {
        muc = {{TRANG_CHU, L"Tổng quan"}, {KHACH_HANG, L"Khách hàng"}};
        if (dichVu.laQuanLy()) {
            muc.push_back({NHAN_VIEN, L"Nhân viên"});
            muc.push_back({THUC_DON, L"Thực đơn"});
            muc.push_back({BAO_CAO, L"Báo cáo"});
            muc.push_back({TAI_KHOAN, L"Tạo tài khoản"});
        } else {
            muc.push_back({BAN_HANG, L"Bán hàng"});
            muc.push_back({DON_DAT, L"Đơn khách đặt"});
        }
        muc.push_back({MAT_KHAU, L"Đổi mật khẩu"});
    }
    int y = 112;
    for (const auto& m : muc) {
        nut(m.second, m.first, 16, y, 216, KieuNut::ThanhBen, iconTrang(m.first), 44);
        y += 50;
    }
    nut(L"Đăng xuất", DANG_XUAT, 16, caoCuaSo - 66, 216, KieuNut::ThanhBen, Icon::DANG_XUAT, 44);
}

void CuaSoNhaHang::hienTrang(int trangMoi, bool giuNhap) {
    if (dichVu.laQuanLy() && (trangMoi == BAN_HANG || trangMoi == DON_DAT))
        throw runtime_error("Quản lý sử dụng các chức năng quản trị. Bán hàng và xử lý đơn dành cho nhân viên.");
    if (dichVu.laKhachHang() && trangMoi != TRANG_CHU && trangMoi != THUC_DON &&
        trangMoi != HO_SO_CUA_TOI && trangMoi != HOA_DON_CUA_TOI && trangMoi != MAT_KHAU &&
        trangMoi != DAT_MON && trangMoi != DON_DAT)
        throw runtime_error("Tài khoản khách hàng không có quyền truy cập chức năng này.");
    if (dichVu.daDangNhap() && !dichVu.laKhachHang() &&
        (trangMoi == HO_SO_CUA_TOI || trangMoi == HOA_DON_CUA_TOI || trangMoi == DAT_MON))
        throw runtime_error("Trang này dành cho tài khoản khách hàng.");
    if (dichVu.daDangNhap() && !dichVu.laQuanLy() &&
        (trangMoi == NHAN_VIEN || trangMoi == BAO_CAO || trangMoi == TAI_KHOAN))
        throw runtime_error("Chức năng này chỉ dành cho quản lý.");
    if (dichVu.daDangNhap() && !dichVu.laQuanLy() && !dichVu.laKhachHang() && trangMoi == THUC_DON)
        throw runtime_error("Tài khoản nhân viên không sử dụng mục Thực đơn.");
    if (dichVu.daDangNhap() && trangHienTai == BAN_HANG && o(O_NGAY)) {
        maHoaDon = duLieu(O_MA);
        ngayDon = duLieu(O_NGAY);
        khachDon = duLieu(O_KHACH);
        nhanVienDon = nhanVienDaChon();
    }
    map<int, wstring> nhapCu;
    map<int, int> chonCu;
    map<int, bool> danhDauCu;
    if (giuNhap) {
        const vector<int> cacO = {TIM_KIEM, LOC_BAO_CAO, O_MA, O_TEN, O_SDT, O_DIEM, O_CHUCVU,
            O_LUONG, O_PHUCAP, O_GIA, O_TON, O_CAY, O_NHAPKHO, O_ANH, O_NGAY, O_KHACH, O_SOLUONG,
            O_USER, O_PASS, O_CONFIRM, O_OLDPASS, O_EMAIL};
        for (int id : cacO) if (o(id)) nhapCu[id] = chu(id);
        for (int id : {LOC_LOAI, SAP_XEP, O_GIOITINH, O_LOAI, O_SIZE, O_NHANVIEN, O_ROLE})
            if (o(id) && !(trangHienTai == HO_SO_CUA_TOI && id == O_GIOITINH)) chonCu[id] = luaChon(id);
        danhDauCu = danhDauGiaTri;
    }
    dangVeBang = true;
    SendMessageW(cuaSo, WM_SETREDRAW, FALSE, 0);
    xoaDieuKhien();
    if (!giuNhap) { maDangSua.clear(); anhDangChon.clear(); }
    trangHienTai = trangMoi;
    if (!dichVu.daDangNhap()) {
        if (dangDangKy && !thietLap) trangDangKy();
        else trangDangNhap();
    } else {
        thanhBen();
        switch (trangHienTai) {
            case TRANG_CHU: trangChu(); break;
            case KHACH_HANG: case NHAN_VIEN: trangDanhSach(); break;
            case THUC_DON: trangThucDon(); break;
            case BAN_HANG: trangBanHang(); break;
            case DAT_MON: trangDatMon(); break;
            case DON_DAT: trangDonDat(); break;
            case BAO_CAO: trangBaoCao(); break;
            case HOA_DON_CUA_TOI: trangLichSuKhachHang(); break;
            case TAI_KHOAN: trangTaiKhoan(); break;
            case MAT_KHAU: trangMatKhau(); break;
            case HO_SO_CUA_TOI: trangHoSoKhachHang(); break;
            default: break;
        }
    }
    for (const auto& muc : nhapCu) if (o(muc.first)) gan(muc.first, muc.second);
    for (const auto& muc : chonCu) if (o(muc.first)) chonMuc(muc.first, muc.second);
    for (const auto& muc : danhDauCu) if (o(muc.first) && danhDauGiaTri.count(muc.first)) danhDauGiaTri[muc.first] = muc.second;
    if (dichVu.daDangNhap() && (trangHienTai == KHACH_HANG || trangHienTai == NHAN_VIEN || trangHienTai == THUC_DON)) {
        napBang();
        doiLoaiMon();
        capNhatNutAnh();
    }
    if (dichVu.daDangNhap() && (trangHienTai == BAN_HANG || trangHienTai == DAT_MON)) {
        if (trangHienTai == BAN_HANG) capNhatKhachBanHang();
        napMonDat();
        napGio();
    }
    if (!maDangSua.empty() && o(O_MA)) SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    dangVeBang = false;
    SendMessageW(cuaSo, WM_SETREDRAW, TRUE, 0);
    RedrawWindow(cuaSo, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

// ---------------------------------------------------------------- Ve cua so

void CuaSoNhaHang::veThanhBen(HDC dc) {
    RECT ben = {0, 0, BoCuc::THANH_BEN, caoCuaSo};
    toNen(dc, ben, Mau::THANH_BEN);
    RECT mepPhai = {BoCuc::THANH_BEN - 1, 0, BoCuc::THANH_BEN, caoCuaSo};
    toNen(dc, mepPhai, Mau::VIEN);
    veVongTron(dc, 44, 50, 22, Mau::CHINH);
    RECT oIcon = {22, 28, 66, 72};
    veIcon(dc, Icon::NHA_HANG, oIcon, font.iconLon, RGB(255,255,255));
    veChu(dc, L"NHÀ HÀNG", {78, 28, 236, 50}, font.dam, Mau::CHU);
    veChu(dc, dichVu.laKhachHang() ? L"Không gian khách hàng" : L"Không gian làm việc",
          {78, 50, 236, 72}, font.nho, Mau::CHU_MO);
    RECT ke = {20, 94, BoCuc::THANH_BEN - 20, 95};
    toNen(dc, ke, Mau::VIEN);

    RECT the = {16, caoCuaSo - 140, BoCuc::THANH_BEN - 16, caoCuaSo - 80};
    veHinhBoGoc(dc, the, 12, Mau::THE, Mau::VIEN);
    wstring ten = rong(dichVu.phien().tenDangNhap);
    wstring chuDau = ten.empty() ? L"?" : ten.substr(0, 1);
    CharUpperBuffW(&chuDau[0], 1);
    veVongTron(dc, 48, the.top + 30, 18, Mau::CHINH);
    veChu(dc, chuDau, {30, the.top + 12, 66, the.top + 48}, font.dam, RGB(255,255,255), DT_CENTER);
    veChu(dc, ten, {76, the.top + 9, the.right - 10, the.top + 31}, font.nut, Mau::CHU);
    wstring vaiTro = dichVu.laQuanLy() ? L"Quản lý" : dichVu.laKhachHang() ? L"Khách hàng" : L"Nhân viên";
    veChu(dc, vaiTro, {76, the.top + 31, the.right - 10, the.top + 51}, font.nho, Mau::CHU_MO);
}

void CuaSoNhaHang::veDauTrang(HDC dc) {
    bool khach = dichVu.laKhachHang();
    int phai = phaiNoiDung();
    wstring ngay = rong(homNay());
    int rongNgay = doChu(dc, ngay, font.nhoDam).cx + 52;
    RECT oNgay = {phai - rongNgay, 32, phai, 68};
    veHinhBoGoc(dc, oNgay, 18, Mau::THE, Mau::VIEN);
    veIcon(dc, Icon::LICH, {oNgay.left + 12, oNgay.top, oNgay.left + 32, oNgay.bottom}, font.icon, Mau::CHINH);
    veChu(dc, ngay, {oNgay.left + 38, oNgay.top, oNgay.right - 10, oNgay.bottom}, font.nhoDam, Mau::CHU_NHAN);
    veChu(dc, tieuDeTrang(trangHienTai, khach), {BoCuc::X0, 24, oNgay.left - 16, 62}, font.tieuDe, Mau::CHU);
    veChu(dc, moTaTrang(trangHienTai, khach), {BoCuc::X0, 66, phai, 92}, font.thuong, Mau::CHU_MO);
}

void CuaSoNhaHang::veTrang(HDC dc) {
    switch (trangHienTai) {
        case HO_SO_CUA_TOI: veHoSoKhachHang(dc); break;
        case BAN_HANG: case DAT_MON: veTongGio(dc); break;
        case BAO_CAO: veThongKeBaoCao(dc); break;
        case TRANG_CHU:
            veThongKeTongQuan(dc);
            if (dichVu.laQuanLy()) veBieuDoDoanhThu(dc);
            break;
        default: break;
    }
}

void CuaSoNhaHang::ve(HDC dc) {
    RECT toan = {0, 0, rongCuaSo, caoCuaSo};
    toNen(dc, toan, Mau::NEN);
    if (!dichVu.daDangNhap()) {
        veManDangNhap(dc);
        for (const RECT& the : cacThe) veThe(dc, the, 20);
        veKhungNhap(dc);
        return;
    }
    veThanhBen(dc);
    veDauTrang(dc);
    for (const RECT& the : cacThe) veThe(dc, the);
    veDauThe(dc);
    veKhungNhap(dc);
    veTrang(dc);
    RECT chan = {BoCuc::X0, caoCuaSo - BoCuc::CHAN, phaiNoiDung(), caoCuaSo - 6};
    veIcon(dc, Icon::THONG_TIN, {chan.left, chan.top, chan.left + 18, chan.bottom}, font.icon, Mau::CHU_MO);
    veChu(dc, thongBao, {chan.left + 26, chan.top, chan.right, chan.bottom}, font.nho, Mau::CHU_MO);
}

// ---------------------------------------------------------------- Xu ly lenh

void CuaSoNhaHang::xuLyNut(int id, int suKien) {
    if (suKien == BN_CLICKED && danhDauGiaTri.count(id)) datDanhDau(id, !daDanhDau(id));
    if (suKien == CBN_SELCHANGE && id == O_ROLE) { vaiTroTao = luaChon(O_ROLE); hienTrang(TAI_KHOAN, true); return; }
    if (suKien == EN_CHANGE && id == TIM_KIEM) {
        if (trangHienTai == BAN_HANG || trangHienTai == DAT_MON) napMonDat();
        else napBang();
        return;
    }
    if (suKien == EN_CHANGE && id == O_KHACH) {
        if (trangHienTai == BAN_HANG && !dangChonGoiYKhach) { capNhatKhachBanHang(true); napGio(); }
        return;
    }
    if (id == GOI_Y_KHACH && suKien == LBN_SELCHANGE) { chonGoiYKhach(); return; }
    if (id == GOI_Y_KHACH && suKien == LBN_KILLFOCUS) {
        if (GetFocus() != o(O_KHACH)) anGoiYKhach();
        return;
    }
    if (id == DANH_SACH_MON && suKien == LBN_SELCHANGE) { chonMonDat(); return; }
    if (id == DANH_SACH_MON && suKien == LBN_DBLCLK) { themVaoGio(); return; }
    if (suKien == CBN_SELCHANGE && id == LOC_NAM_DOANHTHU) { napBieuDoDoanhThu(); return; }
    if (id == DANH_SACH_THUCDON && suKien == LBN_SELCHANGE) { chonDong(); return; }
    if (suKien == CBN_SELCHANGE && id == SAP_XEP) { napBang(); return; }
    if (suKien == CBN_SELCHANGE && id == O_LOAI) { doiLoaiMon(); return; }
    if (suKien != BN_CLICKED) return;
    if (!dichVu.daDangNhap() && id == NUT_MO_DANGKY && !thietLap) {
        dangDangKy = true;
        hienTrang(TRANG_CHU);
        return;
    }
    if (!dichVu.daDangNhap() && id == NUT_VE_DANGNHAP) {
        dangDangKy = false;
        hienTrang(TRANG_CHU);
        return;
    }
    if (!dichVu.daDangNhap() && id == NUT_DANGKY && dangDangKy && !thietLap) {
        string ten = duLieu(O_USER);
        string ma = dichVu.dangKyKhachHang(duLieu(O_TEN), duLieu(O_SDT), duLieu(O_EMAIL), duLieu(O_GIOITINH),
                                           ten, duLieu(O_PASS), duLieu(O_CONFIRM));
        dangDangKy = false;
        soLanSai = 0;
        hienTrang(TRANG_CHU);
        gan(O_USER, rong(ten));
        SetFocus(o(O_PASS));
        wstring noiDung = L"Đăng ký thành công! Mã khách hàng của bạn: " + rong(ma) +
                          L".\nHãy đăng nhập bằng tài khoản vừa tạo.";
        MessageBoxW(cuaSo, noiDung.c_str(), L"Đăng ký thành công", MB_OK | MB_ICONINFORMATION);
        return;
    }
    if (id == NUT_DANGNHAP && !dichVu.daDangNhap() && !dangDangKy) {
        string ten = duLieu(O_USER);
        if (thietLap) {
            dichVu.taoTaiKhoan(ten, duLieu(O_PASS), duLieu(O_CONFIRM), "QUANLY");
            thietLap = false;
            hienTrang(TRANG_CHU);
            gan(O_USER, rong(ten));
            SetFocus(o(O_PASS));
            MessageBoxW(cuaSo, L"Đã tạo tài khoản quản lý. Hãy đăng nhập để tiếp tục.", L"Thành công",
                        MB_OK | MB_ICONINFORMATION);
        } else {
            if (!dichVu.dangNhap(ten, duLieu(O_PASS))) {
                soLanSai++;
                gan(O_PASS, L"");
                if (soLanSai >= 3) {
                    baoLoi("Đăng nhập sai 3 lần. Hãy mở lại ứng dụng để thử lại.");
                    DestroyWindow(cuaSo);
                } else {
                    baoLoi("Sai tài khoản hoặc mật khẩu.");
                    SetFocus(o(O_PASS));
                }
                return;
            }
            soLanSai = 0;
            thongBao = L"Xin chào, " + rong(ten) + L"!";
            hienTrang(TRANG_CHU);
        }
        return;
    }
    if (!dichVu.daDangNhap()) return;
    if ((id >= TRANG_CHU && id <= MAT_KHAU) || id == HO_SO_CUA_TOI || id == HOA_DON_CUA_TOI ||
        id == DAT_MON || id == DON_DAT) {
        hienTrang(id);
        return;
    }
    if (id == DANG_XUAT) {
        if (!xacNhan(gioHang.empty() ? L"Bạn muốn đăng xuất?"
                                     : L"Giỏ hàng chưa thanh toán sẽ bị hủy. Bạn muốn đăng xuất?")) return;
        gioHang.clear();
        hoaDonBaoCao.clear();
        maHoaDon.clear();
        ngayDon.clear();
        khachDon.clear();
        nhanVienDon.clear();
        maMonDangChon.clear();
        nhomDatMon = 0;
        nhomThucDon = 0;
        dichVu.dangXuat();
        hienTrang(TRANG_CHU);
        return;
    }
    switch (id) {
        case NUT_LUU: luuBieuMau(); break;
        case NUT_MOI: hienTrang(trangHienTai); SetFocus(o(O_TEN)); break;
        case NUT_XOA: xoaBanGhi(); break;
        case NUT_CHON_ANH: chonAnhMon(); break;
        case CON_HANG: napBang(); break;
        case NUT_NHOM_THUCAN: case NUT_NHOM_NUOC:
            if (trangHienTai == THUC_DON) {
                nhomThucDon = id == NUT_NHOM_NUOC ? 1 : 0;
                gan(TIM_KIEM, L"");
                napThucDonAnh();
            } else {
                nhomDatMon = id == NUT_NHOM_NUOC ? 1 : 0;
                maMonDangChon.clear();
                gan(TIM_KIEM, L"");
                napMonDat();
            }
            break;
        case NUT_GUI_DON: case NUT_HUY_DON: case NUT_THANHTOAN_DON: case NUT_LAMMOI_DON:
            xuLyDonDat(id);
            break;
        case NUT_THEM_GIO: themVaoGio(); break;
        case NUT_GIAM_SL: doiSoLuong(-1); break;
        case NUT_TANG_SL: doiSoLuong(1); break;
        case NUT_XOA_GIO: {
            int i = dongChon(BANG_GIO);
            if (i < 0) throw runtime_error("Chọn món trong giỏ cần xóa.");
            gioHang.erase(gioHang.begin() + i);
            napGio();
            napMonDat();
            break;
        }
        case NUT_HUY_GIO:
            if (!gioHang.empty() && xacNhan(L"Hủy toàn bộ giỏ hàng?")) {
                gioHang.clear();
                napGio();
                napMonDat();
            }
            break;
        case NUT_THANHTOAN: thanhToan(); break;
        case NUT_BC_HOMNAY: gan(LOC_BAO_CAO, rong(homNay())); napBaoCao(); break;
        case NUT_BC_THANG: gan(LOC_BAO_CAO, rong(homNay().substr(3))); napBaoCao(); break;
        case NUT_BC_TATCA: gan(LOC_BAO_CAO, L""); napBaoCao(); break;
        case NUT_BAOCAO: napBaoCao(); break;
        case NUT_HOADON: {
            int i = dongChon(BANG_CHINH);
            if (i < 0) throw runtime_error("Chọn một hóa đơn trong danh sách.");
            xemHoaDon(cuaSo, dichVu.hoaDonDuocXem(hoaDonBaoCao.at(i).getMa()), font);
            break;
        }
        case NUT_NHAPKHO: {
            dichVu.nhapKho(maDangSua, static_cast<int>(soNhap(O_NHAPKHO, "Số lượng nhập", true)));
            string ma = maDangSua;
            hienTrang(THUC_DON);
            thanhCong(L"Đã nhập thêm tồn kho cho " + rong(ma));
            break;
        }
        case NUT_TAOTAIKHOAN: {
            string vaiTro = vaiTroDuocChon();
            wstring tb = L"Đã tạo tài khoản quản lý thành công.";
            if (vaiTro == "QUANLY") {
                dichVu.taoTaiKhoan(duLieu(O_USER), duLieu(O_PASS), duLieu(O_CONFIRM), "QUANLY");
            } else if (vaiTro == "NHANVIEN") {
                double luong = soNhap(O_LUONG, "Lương"), phuCap = soNhap(O_PHUCAP, "Phụ cấp");
                string ma = dichVu.taoTaiKhoanNhanVien(duLieu(O_USER), duLieu(O_PASS), duLieu(O_CONFIRM),
                    duLieu(O_TEN), duLieu(O_SDT), duLieu(O_GIOITINH), duLieu(O_CHUCVU), luong, phuCap);
                tb = L"Đã tạo tài khoản và hồ sơ nhân viên (Mã tự động: " + rong(ma) + L").";
            } else {
                string ma = dichVu.taoTaiKhoanKhachHang(duLieu(O_USER), duLieu(O_PASS), duLieu(O_CONFIRM),
                    duLieu(O_TEN), duLieu(O_SDT), duLieu(O_GIOITINH), duLieu(O_EMAIL));
                tb = L"Đã tạo tài khoản và hồ sơ khách hàng (Mã tự động: " + rong(ma) + L").";
            }
            hienTrang(TAI_KHOAN);
            thanhCong(tb);
            break;
        }
        case NUT_DOIMATKHAU:
            dichVu.doiMatKhau(duLieu(O_OLDPASS), duLieu(O_PASS), duLieu(O_CONFIRM));
            hienTrang(MAT_KHAU);
            thanhCong(L"Đã đổi mật khẩu thành công.");
            break;
    }
}

void CuaSoNhaHang::xuLyThongBao(NMHDR* n, LRESULT& ketQua) {
    if (n->code == NM_CUSTOMDRAW && laBangTuyChinh(n->hwndFrom)) {
        ketQua = veDongBang(reinterpret_cast<NMLVCUSTOMDRAW*>(n), font);
        return;
    }
    if (dangVeBang || n->idFrom != BANG_CHINH) return;
    if (n->code == LVN_ITEMCHANGED) {
        NMLISTVIEW* lv = reinterpret_cast<NMLISTVIEW*>(n);
        if ((lv->uNewState & LVIS_SELECTED) && !(lv->uOldState & LVIS_SELECTED)) chonDong();
    }
    if (n->code == NM_DBLCLK && (trangHienTai == BAO_CAO || trangHienTai == HOA_DON_CUA_TOI))
        xuLyNut(NUT_HOADON, BN_CLICKED);
}

void CuaSoNhaHang::baoLoi(const string& noiDung) {
    MessageBoxW(cuaSo, rong(noiDung).c_str(), L"Vui lòng kiểm tra", MB_OK | MB_ICONWARNING);
}

void CuaSoNhaHang::thanhCong(const wstring& noiDung) {
    thongBao = noiDung;
    RECT chan = {BoCuc::X0, caoCuaSo - BoCuc::CHAN, rongCuaSo, caoCuaSo};
    InvalidateRect(cuaSo, &chan, FALSE);
}

bool CuaSoNhaHang::xacNhan(const wstring& noiDung) {
    return MessageBoxW(cuaSo, noiDung.c_str(), L"Xác nhận", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES;
}

// ---------------------------------------------------------------- Thu tuc cua so

LRESULT CALLBACK CuaSoNhaHang::thuTuc(HWND h, UINT tin, WPARAM w, LPARAM l) {
    CuaSoNhaHang* app = reinterpret_cast<CuaSoNhaHang*>(GetWindowLongPtrW(h, GWLP_USERDATA));
    if (tin == WM_NCCREATE) {
        app = static_cast<CuaSoNhaHang*>(reinterpret_cast<CREATESTRUCTW*>(l)->lpCreateParams);
        app->cuaSo = h;
        SetWindowLongPtrW(h, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    }
    if (!app) return DefWindowProcW(h, tin, w, l);
    try {
        switch (tin) {
            case WM_CREATE: {
                app->font = taoBoFont();
                app->anh.napTaiNguyen();
                app->coNen = CreateSolidBrush(Mau::NEN);
                app->coThe = CreateSolidBrush(Mau::THE);
                app->coChiDoc = CreateSolidBrush(Mau::O_CHI_DOC);
                RECT r;
                GetClientRect(h, &r);
                app->rongCuaSo = r.right;
                app->caoCuaSo = r.bottom;
                app->hienTrang(TRANG_CHU);
                return 0;
            }
            case WM_SIZE:
                if (w != SIZE_MINIMIZED) {
                    app->rongCuaSo = LOWORD(l);
                    app->caoCuaSo = HIWORD(l);
                    SetTimer(h, 1, 180, nullptr);
                }
                return 0;
            case WM_TIMER:
                if (w == 1) {
                    KillTimer(h, 1);
                    app->hienTrang(app->trangHienTai, true);
                }
                return 0;
            case WM_GETMINMAXINFO: {
                MINMAXINFO* m = reinterpret_cast<MINMAXINFO*>(l);
                m->ptMinTrackSize = {1120, 760};
                return 0;
            }
            case WM_COMMAND: {
                int id = LOWORD(w), suKien = HIWORD(w);
                HWND con = reinterpret_cast<HWND>(l);
                if (con && app->khungNhap.count(id) && (suKien == EN_SETFOCUS || suKien == EN_KILLFOCUS)) {
                    if (suKien == EN_SETFOCUS) {
                        app->oDangChon = id;
                        if (id != O_KHACH) app->anGoiYKhach();
                        else if (app->trangHienTai == BAN_HANG && !app->dangChonGoiYKhach) app->capNhatKhachBanHang(true);
                    } else {
                        if (app->oDangChon == id) app->oDangChon = -1;
                        if (id == O_KHACH && GetFocus() != app->o(GOI_Y_KHACH)) app->anGoiYKhach();
                    }
                    RECT r = app->khungNhap[id];
                    InflateRect(&r, 5, 5);
                    InvalidateRect(h, &r, FALSE);
                }
                if (con && (suKien == CBN_DROPDOWN || suKien == CBN_CLOSEUP)) InvalidateRect(con, nullptr, FALSE);
                if (!app->dangVeBang) app->xuLyNut(id, suKien);
                return 0;
            }
            case WM_NOTIFY: {
                LRESULT kq = 0;
                app->xuLyThongBao(reinterpret_cast<NMHDR*>(l), kq);
                return kq;
            }
            case WM_MEASUREITEM: {
                MEASUREITEMSTRUCT* m = reinterpret_cast<MEASUREITEMSTRUCT*>(l);
                if (m->CtlType == ODT_COMBOBOX || m->CtlType == ODT_LISTBOX) {
                    m->itemHeight = m->itemID == static_cast<UINT>(-1) ? 34 : 32;
                    return TRUE;
                }
                break;
            }
            case WM_DRAWITEM: {
                DRAWITEMSTRUCT* d = reinterpret_cast<DRAWITEMSTRUCT*>(l);
                if (d->CtlType == ODT_COMBOBOX || d->CtlType == ODT_LISTBOX) app->veMucHop(d);
                else if (d->CtlType == ODT_BUTTON) app->veNut(d);
                return TRUE;
            }
            case WM_CTLCOLORSTATIC: {
                HDC dc = reinterpret_cast<HDC>(w);
                HWND con = reinterpret_cast<HWND>(l);
                wchar_t lop[16] = {};
                GetClassNameW(con, lop, 16);
                if (lstrcmpiW(lop, L"Edit") == 0) {
                    SetTextColor(dc, Mau::CHU);
                    SetBkColor(dc, Mau::O_CHI_DOC);
                    return reinterpret_cast<LRESULT>(app->coChiDoc);
                }
                auto it = app->mauChuNhan.find(con);
                SetTextColor(dc, it == app->mauChuNhan.end() ? Mau::CHU_MO : it->second);
                SetBkMode(dc, TRANSPARENT);
                RECT r;
                GetWindowRect(con, &r);
                MapWindowPoints(nullptr, h, reinterpret_cast<POINT*>(&r), 2);
                bool trang = app->trongThe(r);
                SetBkColor(dc, trang ? Mau::THE : Mau::NEN);
                return reinterpret_cast<LRESULT>(trang ? app->coThe : app->coNen);
            }
            case WM_CTLCOLOREDIT: {
                HDC dc = reinterpret_cast<HDC>(w);
                SetTextColor(dc, Mau::CHU);
                SetBkColor(dc, Mau::THE);
                return reinterpret_cast<LRESULT>(app->coThe);
            }
            case WM_CTLCOLORLISTBOX: {
                HDC dc = reinterpret_cast<HDC>(w);
                SetBkColor(dc, Mau::THE);
                return reinterpret_cast<LRESULT>(app->coThe);
            }
            case WM_LBUTTONDOWN: {
                app->anGoiYKhach();
                POINT p = {static_cast<short>(LOWORD(l)), static_cast<short>(HIWORD(l))};
                for (const auto& muc : app->khungNhap) {
                    HWND e = app->o(muc.first);
                    if (e && PtInRect(&muc.second, p) && IsWindowEnabled(e)) { SetFocus(e); break; }
                }
                return 0;
            }
            case WM_MOUSEMOVE:
                if (app->dichVu.daDangNhap() && app->trangHienTai == TRANG_CHU && app->dichVu.laQuanLy())
                    app->diChuyenBieuDo(static_cast<short>(LOWORD(l)), static_cast<short>(HIWORD(l)));
                return 0;
            case WM_MOUSELEAVE:
                if (app->thangDangXem != -1) {
                    app->thangDangXem = -1;
                    InvalidateRect(h, nullptr, FALSE);
                }
                return 0;
            case WM_ERASEBKGND:
                return 1;
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC dc = BeginPaint(h, &ps);
                HDC boNho = CreateCompatibleDC(dc);
                HBITMAP anhDem = CreateCompatibleBitmap(dc, max(1, app->rongCuaSo), max(1, app->caoCuaSo));
                HGDIOBJ cu = SelectObject(boNho, anhDem);
                app->ve(boNho);
                BitBlt(dc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left,
                       ps.rcPaint.bottom - ps.rcPaint.top, boNho, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
                SelectObject(boNho, cu);
                DeleteObject(anhDem);
                DeleteDC(boNho);
                EndPaint(h, &ps);
                return 0;
            }
            case WM_CLOSE:
                if (!app->gioHang.empty() && !app->xacNhan(L"Giỏ hàng chưa thanh toán sẽ bị hủy. Bạn muốn thoát?"))
                    return 0;
                DestroyWindow(h);
                return 0;
            case WM_DESTROY:
                app->anh.giaiPhong();
                xoaBoFont(app->font);
                DeleteObject(app->coNen);
                DeleteObject(app->coThe);
                DeleteObject(app->coChiDoc);
                PostQuitMessage(0);
                return 0;
        }
    } catch (const exception& e) {
        app->dangVeBang = false;
        SendMessageW(h, WM_SETREDRAW, TRUE, 0);
        // Luu that bai thi dich vu nap lai du lieu: lam moi luoi mon de khong dung chi so cu.
        if (tin != WM_CREATE && app->dichVu.daDangNhap()) {
            try {
                if (app->trangHienTai == THUC_DON) app->napThucDonAnh();
                if (app->trangHienTai == BAN_HANG || app->trangHienTai == DAT_MON) { app->napMonDat(); app->napGio(); }
            } catch (...) {}
        }
        RedrawWindow(h, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN);
        app->baoLoi(e.what());
        if (tin == WM_CREATE) return -1;
        return 0;
    }
    return DefWindowProcW(h, tin, w, l);
}

int CuaSoNhaHang::chay(HINSTANCE ungDung, int cheDoHien) {
    thietLap = dichVu.khoiTao();
    INITCOMMONCONTROLSEX cc = {sizeof(cc), ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES};
    InitCommonControlsEx(&cc);
    WNDCLASSEXW lop = {};
    lop.cbSize = sizeof(lop);
    lop.hInstance = ungDung;
    lop.lpfnWndProc = thuTuc;
    lop.lpszClassName = L"NhaHangCuaSo";
    lop.hCursor = LoadCursor(nullptr, IDC_ARROW);
    lop.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    if (!RegisterClassExW(&lop)) throw runtime_error("Không đăng ký được cửa sổ.");
    RECT manHinh;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &manHinh, 0);
    int w = min(1320, static_cast<int>(manHinh.right - manHinh.left - 40));
    int h = min(880, static_cast<int>(manHinh.bottom - manHinh.top - 40));
    HWND win = CreateWindowExW(WS_EX_CONTROLPARENT, lop.lpszClassName, L"Nhà hàng • Quản lý & bán hàng",
                               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                               manHinh.left + (manHinh.right - manHinh.left - w) / 2,
                               manHinh.top + (manHinh.bottom - manHinh.top - h) / 2, w, h,
                               nullptr, nullptr, ungDung, this);
    if (!win) throw runtime_error("Không mở được cửa sổ ứng dụng.");
    ShowWindow(win, cheDoHien);
    UpdateWindow(win);
    MSG tin;
    while (GetMessageW(&tin, nullptr, 0, 0) > 0) {
        if (tin.message == WM_KEYDOWN && tin.wParam == VK_RETURN && dichVu.daDangNhap() &&
            trangHienTai == BAO_CAO && GetFocus() == o(LOC_BAO_CAO)) {
            try { xuLyNut(NUT_BAOCAO, BN_CLICKED); }
            catch (const exception& loi) { baoLoi(loi.what()); }
            continue;
        }
        if (tin.message == WM_KEYDOWN && tin.wParam == VK_RETURN && !dichVu.daDangNhap() && IsWindow(cuaSo)) {
            HWND dangChon = GetFocus();
            bool dangMoGioiTinh = o(O_GIOITINH) && SendMessageW(o(O_GIOITINH), CB_GETDROPPEDSTATE, 0, 0);
            if (!dangMoGioiTinh) {
                int nutBam = dangDangKy ? NUT_DANGKY : NUT_DANGNHAP;
                if (dangChon == o(NUT_MO_DANGKY)) nutBam = NUT_MO_DANGKY;
                if (dangChon == o(NUT_VE_DANGNHAP)) nutBam = NUT_VE_DANGNHAP;
                try { xuLyNut(nutBam, BN_CLICKED); }
                catch (const exception& loi) { baoLoi(loi.what()); }
                continue;
            }
        }
        if (!IsWindow(cuaSo) || !IsDialogMessageW(cuaSo, &tin)) {
            TranslateMessage(&tin);
            DispatchMessageW(&tin);
        }
    }
    return static_cast<int>(tin.wParam);
}

int chayGiaoDienCuaSo() {
    // DuLieu luon nam canh tep exe, ke ca khi mo tu Explorer hay shortcut.
    wchar_t duongDan[32768];
    DWORD n = GetModuleFileNameW(nullptr, duongDan, 32768);
    if (n == 0 || n >= 32768) throw runtime_error("Không xác định được thư mục ứng dụng.");
    wstring thuMuc(duongDan, n);
    thuMuc = thuMuc.substr(0, thuMuc.find_last_of(L"\\/"));
    if (!SetCurrentDirectoryW(thuMuc.c_str())) throw runtime_error("Không mở được thư mục ứng dụng.");
    // Mot bo du lieu chi mo mot cua so, tranh hai phien ghi de don cua nhau.
    wstring tenKhoa = L"Local\\PBL2_NhaHang_";
    for (wchar_t c : thuMuc) tenKhoa += (c == L'\\' || c == L'/') ? L'_' : c;
    struct KhoaUngDung {
        HANDLE giaTri;
        ~KhoaUngDung() { if (giaTri) CloseHandle(giaTri); }
    } khoa{CreateMutexW(nullptr, FALSE, tenKhoa.c_str())};
    if (!khoa.giaTri) throw runtime_error("Không tạo được khóa dữ liệu ứng dụng.");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBoxW(nullptr, L"Chương trình đang mở. Hãy dùng cửa sổ hiện có và đăng xuất để chuyển tài khoản.",
                    L"Nhà hàng", MB_OK | MB_ICONINFORMATION);
        return 0;
    }
    // GDI+ phai song lau hon cua so (anh duoc giai phong trong WM_DESTROY).
    struct PhienGdiPlus {
        ULONG_PTR ma = 0;
        PhienGdiPlus() {
            Gdiplus::GdiplusStartupInput thietLap;
            if (Gdiplus::GdiplusStartup(&ma, &thietLap, nullptr) != Gdiplus::Ok)
                throw runtime_error("Không khởi tạo được bộ vẽ đồ họa Windows (GDI+).");
        }
        ~PhienGdiPlus() { Gdiplus::GdiplusShutdown(ma); }
    } gdiPlus;
    CuaSoNhaHang app;
    return app.chay(GetModuleHandleW(nullptr), SW_SHOWDEFAULT);
}
