#include "CuaSo.h"
#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

// Trang Khach hang / Nhan vien: the bang ben trai, the bieu mau ben phai.
void CuaSoNhaHang::trangDanhSach() {
    bool khach = trangHienTai == KHACH_HANG;
    int phai = phaiNoiDung(), day = dayNoiDung();
    int rongForm = 364, xForm = phai - rongForm;
    int xBang = BoCuc::X0, rongBang = xForm - BoCuc::KHOANG - xBang;

    themThe({xBang, BoCuc::Y0, xForm - BoCuc::KHOANG, day});
    themDauThe(xBang + 24, BoCuc::Y0 + 20, rongBang - 48, khach ? Icon::KHACH : Icon::NHAN_VIEN,
               khach ? L"Danh sách khách hàng" : L"Danh sách nhân viên",
               L"Chọn một dòng để xem và chỉnh sửa thông tin.");
    nhap(L"", TIM_KIEM, xBang + 24, BoCuc::Y0 + 52, rongBang - 48);
    SendMessageW(o(TIM_KIEM), EM_SETCUEBANNER, TRUE,
                 reinterpret_cast<LPARAM>(khach ? L"Tìm theo mã, tên, số điện thoại hoặc email"
                                                : L"Tìm theo mã, tên hoặc số điện thoại"));
    vector<pair<wstring, int>> cot;
    if (khach) cot = {{L"Mã KH", 84}, {L"Họ tên", 160}, {L"Điện thoại", 120}, {L"Giới tính", 92}, {L"Điểm", 66}, {L"Hạng VIP", 104}};
    else cot = {{L"Mã NV", 84}, {L"Họ tên", 170}, {L"Điện thoại", 120}, {L"Chức vụ", 120}, {L"Tổng lương", 130}};
    bang(BANG_CHINH, xBang + 16, BoCuc::Y0 + 132, rongBang - 32, day - BoCuc::Y0 - 148, cot, khach ? 5 : -1);
    if (khach) canCot(BANG_CHINH, 4, LVCFMT_RIGHT);
    else canCot(BANG_CHINH, 4, LVCFMT_RIGHT);

    int x = xForm + 24, w = rongForm - 48, nua = (w - 16) / 2, y = BoCuc::Y0;
    themThe({xForm, y, phai, y + 480});
    themDauThe(x, y + 20, w, Icon::HO_SO, L"Sửa / Xóa thông tin", L"Chọn một dòng bên trái để chỉnh sửa hoặc xóa.");
    if (khach) {
        nhap(L"Mã khách hàng (tự động)", O_MA, x, y + 76, nua);
        nhap(L"Điểm tích lũy", O_DIEM, x + nua + 16, y + 76, w - nua - 16, L"0");
    } else {
        nhap(L"Mã nhân viên (tự động)", O_MA, x, y + 76, nua);
        nhap(L"Chức vụ", O_CHUCVU, x + nua + 16, y + 76, w - nua - 16);
    }
    SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    nhap(L"Họ và tên", O_TEN, x, y + 148, w);
    nhap(L"Điện thoại", O_SDT, x, y + 220, nua);
    chon(L"Giới tính", O_GIOITINH, x + nua + 16, y + 220, w - nua - 16, {L"Nam", L"Nữ", L"Khác"});
    if (khach) {
        nhap(L"Email", O_EMAIL, x, y + 292, w);
    } else {
        nhap(L"Lương cơ bản (đ)", O_LUONG, x, y + 292, nua, L"0");
        nhap(L"Phụ cấp (đ)", O_PHUCAP, x + nua + 16, y + 292, w - nua - 16, L"0");
    }
    nut(L"Lưu sửa đổi", NUT_LUU, x, y + 380, w - 112, KieuNut::Chinh, Icon::LUU);
    nut(L"Xóa", NUT_XOA, x + w - 100, y + 380, 100, KieuNut::NguyHiem, Icon::XOA);
    nhan(L"Thêm mới nhân viên/khách hàng tại mục Tạo tài khoản.", x, y + 436, w);
    EnableWindow(o(NUT_LUU), !maDangSua.empty());
    EnableWindow(o(NUT_XOA), !maDangSua.empty());
}

void CuaSoNhaHang::napBang() {
    if (trangHienTai == THUC_DON) { napThucDonAnh(); return; }
    bool cu = dangVeBang;
    dangVeBang = true;
    ListView_DeleteAllItems(o(BANG_CHINH));
    wstring tuKhoa = chu(TIM_KIEM);
    if (trangHienTai == KHACH_HANG) {
        const vector<KhachHang>& ds = dichVu.khachHang();
        for (size_t i = 0; i < ds.size(); i++) {
            const KhachHang& k = ds[i];
            if (!khopTuKhoa(k.getMa() + " " + k.getTen() + " " + k.getSoDienThoai() + " " + k.getEmail(), tuKhoa)) continue;
            dongBang(BANG_CHINH, {rong(k.getMa()), rong(k.getTen()), rong(k.getSoDienThoai()), tenGioiTinh(k.getGioiTinh()),
                                  so(k.getDiemTichLuy()), tenHangVip(k.getHangVip())}, static_cast<int>(i));
        }
    } else if (trangHienTai == NHAN_VIEN) {
        const vector<NhanVien>& ds = dichVu.nhanVien();
        for (size_t i = 0; i < ds.size(); i++) {
            const NhanVien& n = ds[i];
            if (!khopTuKhoa(n.getMa() + " " + n.getTen() + " " + n.getSoDienThoai(), tuKhoa)) continue;
            dongBang(BANG_CHINH, {rong(n.getMa()), rong(n.getTen()), rong(n.getSoDienThoai()), rong(n.getChucVu()),
                                  tien(n.tinhLuong())}, static_cast<int>(i));
        }
    }
    dangVeBang = cu;
    thanhCong(L"Có " + to_wstring(ListView_GetItemCount(o(BANG_CHINH))) + L" kết quả. Dữ liệu được lưu khi nhấn Lưu.");
}

void CuaSoNhaHang::chonDong() {
    int viTri = trangHienTai == THUC_DON ? monThucDonDaChon() : dongChon(BANG_CHINH);
    if (viTri < 0 || (trangHienTai == THUC_DON && !dichVu.laQuanLy())) return;
    if (trangHienTai == KHACH_HANG || trangHienTai == NHAN_VIEN) {
        const Nguoi* nguoi = nullptr;
        if (trangHienTai == KHACH_HANG) {
            const KhachHang& k = dichVu.khachHang().at(viTri);
            nguoi = &k;
            gan(O_DIEM, so(k.getDiemTichLuy()));
            gan(O_EMAIL, rong(k.getEmail()));
        } else {
            const NhanVien& n = dichVu.nhanVien().at(viTri);
            nguoi = &n;
            gan(O_CHUCVU, rong(n.getChucVu()));
            gan(O_LUONG, so(n.getLuongCoBan()));
            gan(O_PHUCAP, so(n.getPhuCap()));
        }
        maDangSua = nguoi->getMa();
        gan(O_MA, rong(maDangSua));
        gan(O_TEN, rong(nguoi->getTen()));
        gan(O_SDT, rong(nguoi->getSoDienThoai()));
        int gt = 2;
        if (nguoi->getGioiTinh() == "Nam") gt = 0;
        if (nguoi->getGioiTinh() == "Nu" || nguoi->getGioiTinh() == "Nữ") gt = 1;
        chonMuc(O_GIOITINH, gt);
    } else if (trangHienTai == THUC_DON) {
        const Mon& m = *dichVu.thucDon().at(viTri);
        maDangSua = m.getMa();
        anhDangChon = m.getHinhAnh();
        gan(O_MA, rong(maDangSua));
        gan(O_TEN, rong(m.getTen()));
        gan(O_GIA, so(m.getGia()));
        gan(O_TON, so(m.getTonKho()));
        capNhatNutAnh();
        const MonAn* a = dynamic_cast<const MonAn*>(&m);
        if (a) {
            chonMuc(O_LOAI, 0);
            gan(O_CAY, so(a->getDoCay()));
            datDanhDau(O_CHAY, a->laMonChay());
        } else {
            const NuocUong& n = dynamic_cast<const NuocUong&>(m);
            chonMuc(O_LOAI, 1);
            int size = 1;
            if (n.getSizeLy() == "S") size = 0;
            if (n.getSizeLy() == "L") size = 2;
            chonMuc(O_SIZE, size);
        }
        doiLoaiMon();
    } else if (trangHienTai == DON_DAT) {
        chiTietDonDat();
        return;
    } else if (trangHienTai == BAO_CAO) {
        EnableWindow(o(NUT_HOADON), TRUE);
        return;
    } else {
        return;
    }
    SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    EnableWindow(o(NUT_LUU), TRUE);
    EnableWindow(o(NUT_XOA), TRUE);
    InvalidateRect(cuaSo, nullptr, FALSE);
}

void CuaSoNhaHang::luuBieuMau() {
    bool themMoi = maDangSua.empty();
    string ma = duLieu(O_MA);
    if ((trangHienTai == KHACH_HANG || trangHienTai == NHAN_VIEN) && themMoi)
        throw runtime_error("Hãy chọn một dòng trong bảng để sửa thông tin.");
    if (!themMoi && ma != maDangSua) throw runtime_error("Không thể thay đổi mã của bản ghi đang sửa.");
    if (trangHienTai == KHACH_HANG) {
        int diem = static_cast<int>(soNhap(O_DIEM, "Điểm tích lũy", true));
        KhachHang k(ma, duLieu(O_TEN), duLieu(O_SDT), duLieu(O_GIOITINH), diem, duLieu(O_EMAIL));
        dichVu.luuKhachHang(k, themMoi);
    } else if (trangHienTai == NHAN_VIEN) {
        double luong = soNhap(O_LUONG, "Lương"), phuCap = soNhap(O_PHUCAP, "Phụ cấp");
        NhanVien n(ma, duLieu(O_TEN), duLieu(O_SDT), duLieu(O_GIOITINH), duLieu(O_CHUCVU), luong, phuCap);
        dichVu.luuNhanVien(n, themMoi);
    } else if (trangHienTai == THUC_DON) {
        DuLieuMon m;
        m.ma = ma;
        m.ten = duLieu(O_TEN);
        m.gia = soNhap(O_GIA, "Giá");
        m.tonKho = static_cast<int>(soNhap(O_TON, "Tồn kho", true));
        if (luaChon(O_LOAI) == 0) {
            m.loai = "Mon an";
            m.doCay = static_cast<int>(soNhap(O_CAY, "Độ cay", true));
        } else {
            m.loai = "Nuoc uong";
        }
        m.sizeLy = duLieu(O_SIZE);
        m.monChay = daDanhDau(O_CHAY);
        m.hinhAnh = anhDangChon;
        ma = dichVu.luuMon(m, themMoi);
        gan(O_MA, rong(ma));
        anh.xoaDemRieng();
    }
    maDangSua = ma;
    napBang();
    SendMessageW(o(O_MA), EM_SETREADONLY, TRUE, 0);
    EnableWindow(o(NUT_XOA), TRUE);
    doiLoaiMon();
    InvalidateRect(cuaSo, nullptr, FALSE);
    thanhCong(L"Đã lưu thông tin thành công.");
}

void CuaSoNhaHang::xoaBanGhi() {
    if (maDangSua.empty()) throw runtime_error("Chọn một dòng trong bảng trước khi xóa.");
    if (!xacNhan(L"Xóa bản ghi " + rong(maDangSua) + L"? Thao tác này không thể hoàn tác.")) return;
    if (trangHienTai == KHACH_HANG) dichVu.xoaKhachHang(maDangSua);
    if (trangHienTai == NHAN_VIEN) dichVu.xoaNhanVien(maDangSua);
    if (trangHienTai == THUC_DON) dichVu.xoaMon(maDangSua);
    hienTrang(trangHienTai);
    thanhCong(L"Đã xóa bản ghi.");
}
