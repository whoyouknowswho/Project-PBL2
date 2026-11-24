#include "CuaSo.h"
#include <objidl.h>
#include <gdiplus.h>
#include <algorithm>
#include <cmath>

using namespace std;

namespace {
const int CAO_CHAO = 96, CAO_SO_LIEU = 112;

struct KhungBieuDo {
    RECT truc;
    TrucBieuDo thang;
    vector<POINT> diem;
};

// Dung chung cho ve va re chuot de diem bat dung vi tri.
KhungBieuDo tinhKhungBieuDo(RECT the, const vector<double>& doanhThu) {
    KhungBieuDo k;
    k.truc = {the.left + 92, the.top + 96, the.right - 36, the.bottom - 70};
    k.thang = tinhTrucBieuDo(*max_element(doanhThu.begin(), doanhThu.end()));
    for (int i = 0; i < 12; i++) {
        int x = k.truc.left + (k.truc.right - k.truc.left) * i / 11;
        int y = k.truc.bottom - static_cast<int>((k.truc.bottom - k.truc.top) * doanhThu[i] / k.thang.tran);
        k.diem.push_back({x, y});
    }
    return k;
}

Gdiplus::Color mau(COLORREF c, BYTE a = 255) { return Gdiplus::Color(a, GetRValue(c), GetGValue(c), GetBValue(c)); }

int yNoiDung(bool khach) { return BoCuc::Y0 + (khach ? CAO_CHAO + 16 : 0) + CAO_SO_LIEU + 16 + 44 + 20; }
}

void CuaSoNhaHang::trangChu() {
    bool khach = dichVu.laKhachHang();
    int x = BoCuc::X0, phai = phaiNoiDung(), day = dayNoiDung();
    int yNut = BoCuc::Y0 + (khach ? CAO_CHAO + 16 : 0) + CAO_SO_LIEU + 16;
    int y = yNoiDung(khach);
    if (khach) { trangChuKhachHang(); return; }
    if (dichVu.laQuanLy()) {
        nut(L"Quản lý thực đơn", THUC_DON, x, yNut, 210, KieuNut::Chinh, Icon::THUC_DON, 44);
        nut(L"Xem báo cáo", BAO_CAO, x + 222, yNut, 180, KieuNut::Phu, Icon::BAO_CAO, 44);
        trangBieuDoDoanhThu();
        return;
    }
    nut(L"Tạo đơn bán hàng", BAN_HANG, x, yNut, 210, KieuNut::Chinh, Icon::THEM, 44);
    themThe({x, y, phai, day});
    themDauThe(x + 24, y + 18, phai - x - 48, Icon::CANH_BAO, L"Tồn kho thấp", L"Cần kiểm tra và nhập thêm");
    bang(BANG_CHINH, x + 12, y + 72, phai - x - 24, day - y - 84,
         {{L"Mã món", 110}, {L"Tên món", 300}, {L"Loại", 180}, {L"Còn lại", 110}, {L"Tình trạng", 190}}, 4);
    canCot(BANG_CHINH, 3, LVCFMT_RIGHT);
    for (size_t i = 0; i < dichVu.thucDon().size(); i++) {
        const Mon& mon = *dichVu.thucDon()[i];
        if (mon.getTonKho() <= 5)
            dongBang(BANG_CHINH, {rong(mon.getMa()), rong(mon.getTen()), mon.getLoai() == "Mon an" ? L"Món ăn" : L"Nước uống",
                                  so(mon.getTonKho()), mon.getTonKho() == 0 ? L"Hết hàng" : L"Sắp hết"},
                     static_cast<int>(i));
    }
}

void CuaSoNhaHang::trangChuKhachHang() {
    const KhachHang& khach = dichVu.hoSoCuaToi();
    int x = BoCuc::X0, phai = phaiNoiDung(), day = dayNoiDung();
    int yNut = BoCuc::Y0 + CAO_CHAO + 16 + CAO_SO_LIEU + 16, y = yNoiDung(true);
    nut(L"Đặt món", DAT_MON, x, yNut, 180, KieuNut::Chinh, Icon::BAN_HANG, 44);
    nut(L"Thông tin của tôi", HO_SO_CUA_TOI, x + 192, yNut, 190, KieuNut::Phu, Icon::HO_SO, 44);
    nut(L"Hóa đơn của tôi", HOA_DON_CUA_TOI, x + 394, yNut, 190, KieuNut::Phu, Icon::HOA_DON, 44);
    themThe({x, y, phai, day});
    themDauThe(x + 24, y + 18, phai - x - 48, Icon::HOA_DON, L"Lịch sử mua hàng · " + rong(khach.getTen()),
               L"Hóa đơn mới nhất ở đầu.");
    bang(BANG_CHINH, x + 12, y + 72, phai - x - 24, day - y - 84, {{L"Mã hóa đơn", 260}, {L"Ngày lập", 150}, {L"Thanh toán", 180}});
    canCot(BANG_CHINH, 2, LVCFMT_RIGHT);
    hoaDonBaoCao = dichVu.hoaDonCuaToi();
    reverse(hoaDonBaoCao.begin(), hoaDonBaoCao.end());
    for (size_t i = 0; i < hoaDonBaoCao.size(); i++) {
        const HoaDon& h = hoaDonBaoCao[i];
        dongBang(BANG_CHINH, {rong(h.getMa()), rong(h.getNgayLap()), tien(h.tongTien())}, static_cast<int>(i));
    }
    if (hoaDonBaoCao.empty()) thanhCong(L"Bạn chưa có hóa đơn. Chọn Đặt món để gửi đơn cho nhà hàng.");
    else thanhCong(L"Chọn Hóa đơn của tôi để xem chi tiết các lần mua hàng.");
}

void CuaSoNhaHang::veThongKeTongQuan(HDC dc) {
    bool khach = dichVu.laKhachHang();
    int x = BoCuc::X0, phai = phaiNoiDung(), y = BoCuc::Y0;
    vector<wstring> ten, giaTri;
    vector<wchar_t> icon;
    if (khach) {
        const KhachHang& k = dichVu.hoSoCuaToi();
        RECT chao = {x, y, phai, y + CAO_CHAO};
        int luu = SaveDC(dc);
        veHinhBoGoc(dc, chao, 16, Mau::NEN_TOI);
        HRGN vung = CreateRoundRectRgn(chao.left, chao.top, chao.right + 1, chao.bottom + 1, 32, 32);
        SelectClipRgn(dc, vung);
        veChuyenSac(dc, chao, Mau::NEN_TOI, Mau::NEN_TOI_CUOI, true);
        veVongTron(dc, chao.right - 60, chao.top + 10, 90, Mau::CHINH, 60);
        veVongTron(dc, chao.right - 190, chao.bottom + 10, 50, RGB(255,255,255), 18);
        RestoreDC(dc, luu);
        DeleteObject(vung);
        veChu(dc, L"Xin chào, " + rong(k.getTen()) + L"!", {x + 28, y + 18, phai - 220, y + 52}, font.tieuDe, Mau::CHU_TREN_TOI);
        veChu(dc, L"Cùng khám phá thực đơn hôm nay và tích điểm để nhận thêm ưu đãi thành viên.",
              {x + 28, y + 56, phai - 220, y + 80}, font.thuong, Mau::CHU_TREN_TOI_MO);
        y += CAO_CHAO + 16;
        ten = {L"ĐIỂM TÍCH LŨY", L"HẠNG THÀNH VIÊN", L"ƯU ĐÃI HIỆN TẠI", L"HÓA ĐƠN CỦA BẠN"};
        giaTri = {so(k.getDiemTichLuy()), tenHangVip(k.getHangVip()), so(k.getPhanTramGiam()) + L"%",
                  to_wstring(dichVu.hoaDonCuaToi().size())};
        icon = {Icon::NGOI_SAO, Icon::HO_SO, Icon::TIEN, Icon::HOA_DON};
    } else {
        ten = {L"KHÁCH HÀNG", L"MÓN TRONG THỰC ĐƠN", L"CẦN NHẬP THÊM", L"TRONG GIỎ HÀNG"};
        int thap = 0;
        for (const unique_ptr<Mon>& mon : dichVu.thucDon()) if (mon->getTonKho() <= 5) thap++;
        giaTri = {to_wstring(dichVu.khachHang().size()), to_wstring(dichVu.thucDon().size()), to_wstring(thap),
                  to_wstring(gioHang.size())};
        icon = {Icon::KHACH, Icon::THUC_DON, Icon::CANH_BAO, Icon::BAN_HANG};
        if (dichVu.laQuanLy()) {
            ten[3] = L"DOANH THU HÔM NAY";
            giaTri[3] = tien(dichVu.baoCao(homNay()).doanhThu);
            icon[3] = Icon::TIEN;
        }
    }
    int w = (phai - x - 3 * 16) / 4;
    for (int i = 0; i < 4; i++) {
        int trai = x + i * (w + 16), phaiThe = i == 3 ? phai : trai + w;
        RECT the = {trai, y, phaiThe, y + CAO_SO_LIEU};
        veThe(dc, the);
        bool canhBao = !khach && i == 2;
        veVongTron(dc, trai + 40, y + 40, 20, canhBao ? Mau::CANH_BAO_NEN : Mau::PHU_NEN);
        veIcon(dc, icon[i], {trai + 20, y + 20, trai + 60, y + 60}, font.icon, canhBao ? Mau::CANH_BAO : Mau::CHINH);
        // Nhan dai thi xuong dong, can giua theo bieu tuong.
        RECT nhanR = {trai + 72, 0, phaiThe - 14, 0};
        HGDIOBJ fontCu = SelectObject(dc, font.nhoDam);
        DrawTextW(dc, ten[i].c_str(), -1, &nhanR, DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);
        SelectObject(dc, fontCu);
        int caoNhan = min(40, static_cast<int>(nhanR.bottom));
        veChu(dc, ten[i], {trai + 72, y + 40 - caoNhan / 2, phaiThe - 14, y + 40 + caoNhan / 2 + 2}, font.nhoDam,
              Mau::CHU_MO, DT_LEFT | DT_WORDBREAK);
        veChu(dc, giaTri[i], {trai + 20, y + 66, phaiThe - 16, y + 98},
              fontVua(dc, giaTri[i], phaiThe - 16 - trai - 20, font.so, font.dam),
              canhBao ? Mau::CANH_BAO : i == 0 ? Mau::CHINH : Mau::CHU);
    }
}

// ---------------------------------------------------------------- Bieu do doanh thu (quan ly)

RECT CuaSoNhaHang::vungBieuDo() const { return {BoCuc::X0, yNoiDung(false), phaiNoiDung(), dayNoiDung()}; }

void CuaSoNhaHang::trangBieuDoDoanhThu() {
    int namHienTai = stoi(homNay().substr(6));
    if (namBieuDo == 0) namBieuDo = namHienTai;
    doanhThuNam = dichVu.doanhThuTheoNam(namBieuDo);
    vector<int> nam = doanhThuNam.cacNam;
    if (find(nam.begin(), nam.end(), namHienTai) == nam.end()) nam.push_back(namHienTai);
    quickSort(nam, [](int a, int b) { return a > b; });
    vector<wstring> luaChon;
    int chonNam = 0;
    for (size_t i = 0; i < nam.size(); i++) {
        luaChon.push_back(to_wstring(nam[i]));
        if (nam[i] == namBieuDo) chonNam = static_cast<int>(i);
    }
    RECT the = vungBieuDo();
    themThe(the);
    HWND nhanNam = nhan(L"Năm doanh thu", the.right - 24 - 132 - 120, the.top + 26, 112);
    SendMessageW(nhanNam, WM_SETFONT, reinterpret_cast<WPARAM>(font.nhoDam), TRUE);
    mauChuNhan[nhanNam] = Mau::CHU_NHAN;
    chon(L"", LOC_NAM_DOANHTHU, the.right - 24 - 132, the.top - 8, 132, luaChon);
    chonMuc(LOC_NAM_DOANHTHU, chonNam);
    thangDangXem = -1;
    thanhCong(L"Doanh thu 12 tháng · Chọn năm để xem lại · Rê chuột lên điểm để xem số tiền.");
}

void CuaSoNhaHang::napBieuDoDoanhThu() {
    int nam = stoi(duLieu(LOC_NAM_DOANHTHU));
    DoanhThuNam moi = dichVu.doanhThuTheoNam(nam);
    namBieuDo = nam;
    doanhThuNam = moi;
    thangDangXem = -1;
    InvalidateRect(cuaSo, nullptr, FALSE);
}

void CuaSoNhaHang::diChuyenBieuDo(int x, int y) {
    RECT the = vungBieuDo();
    KhungBieuDo k = tinhKhungBieuDo(the, doanhThuNam.cacThang);
    int thang = -1;
    for (int i = 0; i < 12; i++)
        if (abs(x - k.diem[i].x) <= 14 && abs(y - k.diem[i].y) <= 14) { thang = i; break; }
    if (thang != thangDangXem) {
        thangDangXem = thang;
        InvalidateRect(cuaSo, &the, FALSE);
    }
    TRACKMOUSEEVENT theoDoi = {sizeof(theoDoi), TME_LEAVE, cuaSo, 0};
    TrackMouseEvent(&theoDoi);
}

void CuaSoNhaHang::veBieuDoDoanhThu(HDC dc) {
    RECT the = vungBieuDo();
    KhungBieuDo k = tinhKhungBieuDo(the, doanhThuNam.cacThang);
    int trai = the.left + 24, phai = the.right - 24;
    veChu(dc, L"Doanh thu từng tháng", {trai, the.top + 16, phai - 280, the.top + 40}, font.dam, Mau::CHU);
    veChu(dc, L"Năm " + to_wstring(namBieuDo) + L" · Doanh thu sau giảm giá thành viên · Đơn vị: " + k.thang.tenDonVi,
          {trai, the.top + 42, phai - 280, the.top + 62}, font.nho, Mau::CHU_MO);
    wstring tong = L"Tổng năm: " + tien(doanhThuNam.tongTien);
    veChu(dc, tong, {trai, the.top + 64, phai, the.top + 86}, font.nut, Mau::CHINH);

    // Luoi va nhan truc.
    for (int i = 0; i <= 4; i++) {
        int y = k.truc.bottom - (k.truc.bottom - k.truc.top) * i / 4;
        veDuong(dc, k.truc.left, y, k.truc.right, y, i == 0 ? Mau::VIEN_O : RGB(243,236,228));
        veChu(dc, nhanTruc(k.thang.tran * i / 4 / k.thang.donVi), {the.left + 12, y - 12, k.truc.left - 14, y + 12},
              font.nho, Mau::CHU_MO, DT_RIGHT);
    }
    for (int i = 0; i < 12; i++)
        veChu(dc, L"T" + to_wstring(i + 1), {k.diem[i].x - 22, k.truc.bottom + 10, k.diem[i].x + 22, k.truc.bottom + 32},
              font.nho, i == thangDangXem ? Mau::CHINH : Mau::CHU_MO, DT_CENTER);

    {
        Gdiplus::Graphics g(dc);
        g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
        vector<Gdiplus::PointF> diem;
        for (const POINT& p : k.diem) diem.push_back(Gdiplus::PointF(static_cast<float>(p.x), static_cast<float>(p.y)));
        Gdiplus::RectF vungTruc(static_cast<float>(k.truc.left - 8), static_cast<float>(k.truc.top - 12),
                                static_cast<float>(k.truc.right - k.truc.left + 16),
                                static_cast<float>(k.truc.bottom - k.truc.top + 12));
        g.SetClip(vungTruc);
        Gdiplus::GraphicsPath vung;
        vung.AddCurve(diem.data(), static_cast<INT>(diem.size()), 0.35f);
        vung.AddLine(diem.back(), Gdiplus::PointF(diem.back().X, static_cast<float>(k.truc.bottom)));
        vung.AddLine(Gdiplus::PointF(diem.back().X, static_cast<float>(k.truc.bottom)),
                     Gdiplus::PointF(diem.front().X, static_cast<float>(k.truc.bottom)));
        vung.CloseFigure();
        Gdiplus::LinearGradientBrush toMau(Gdiplus::PointF(0, static_cast<float>(k.truc.top)),
                                           Gdiplus::PointF(0, static_cast<float>(k.truc.bottom + 1)),
                                           mau(Mau::CHINH, 70), mau(Mau::CHINH, 0));
        g.FillPath(&toMau, &vung);
        Gdiplus::Pen but(mau(Mau::CHINH), 3.0f);
        but.SetLineJoin(Gdiplus::LineJoinRound);
        g.DrawCurve(&but, diem.data(), static_cast<INT>(diem.size()), 0.35f);
        g.ResetClip();
        if (thangDangXem >= 0 && thangDangXem < 12) {
            Gdiplus::Pen gach(mau(Mau::CHINH, 120), 1.0f);
            gach.SetDashStyle(Gdiplus::DashStyleDash);
            g.DrawLine(&gach, diem[thangDangXem].X, static_cast<float>(k.truc.top), diem[thangDangXem].X,
                       static_cast<float>(k.truc.bottom));
        }
        Gdiplus::SolidBrush trang(Gdiplus::Color(255, 255, 255, 255)), cam(mau(Mau::CHINH));
        Gdiplus::Pen vienDiem(mau(Mau::CHINH), 2.0f);
        for (int i = 0; i < 12; i++) {
            float r = i == thangDangXem ? 6.5f : 4.5f;
            Gdiplus::RectF o(diem[i].X - r, diem[i].Y - r, 2 * r, 2 * r);
            g.FillEllipse(i == thangDangXem ? static_cast<Gdiplus::Brush*>(&cam) : &trang, o);
            g.DrawEllipse(&vienDiem, o);
        }
    }

    if (doanhThuNam.soHoaDon == 0)
        veChu(dc, L"Chưa có doanh thu trong năm này", {k.truc.left, k.truc.top + 20, k.truc.right, k.truc.top + 50},
              font.thuong, Mau::CHU_MO, DT_CENTER);
    if (thangDangXem >= 0 && thangDangXem < 12) {
        POINT p = k.diem[thangDangXem];
        int x = max(static_cast<int>(the.left) + 14, min(static_cast<int>(p.x) - 100, static_cast<int>(the.right) - 224));
        int y = max(static_cast<int>(k.truc.top) - 8, static_cast<int>(p.y) - 80);
        RECT hop = {x, y, x + 210, y + 62};
        veHinhBoGoc(dc, hop, 12, Mau::NEN_TOI);
        veChu(dc, L"Tháng " + to_wstring(thangDangXem + 1) + L"/" + to_wstring(namBieuDo), {x + 14, y + 6, x + 200, y + 28},
              font.nho, Mau::CHU_TREN_TOI_MO);
        veChu(dc, tien(doanhThuNam.cacThang[thangDangXem]), {x + 14, y + 28, x + 200, y + 56}, font.dam, Mau::CHU_TREN_TOI);
    }
    wstring ghiChu = to_wstring(doanhThuNam.soHoaDon) + L" hóa đơn đã thanh toán trong năm";
    if (doanhThuNam.soHoaDonBoQua > 0)
        ghiChu += L" · Bỏ qua " + to_wstring(doanhThuNam.soHoaDonBoQua) + L" hóa đơn có ngày hoặc số tiền chưa hợp lệ";
    veChu(dc, ghiChu, {trai, the.bottom - 32, phai, the.bottom - 10}, font.nho, Mau::CHU_MO);
}
