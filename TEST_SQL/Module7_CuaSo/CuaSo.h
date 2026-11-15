#ifndef CUASO_NHAHANG_H
#define CUASO_NHAHANG_H

#include "TienIch.h"
#include "ChuDe.h"
#include "AnhMon.h"
#include "DichVuNhaHang.h"
#include <commctrl.h>
#include <functional>
#include <map>

using namespace std;

enum Trang { TRANG_CHU = 100, KHACH_HANG, NHAN_VIEN, THUC_DON, BAN_HANG, BAO_CAO,
             TAI_KHOAN, MAT_KHAU, DANG_XUAT, HO_SO_CUA_TOI, HOA_DON_CUA_TOI, DAT_MON, DON_DAT };
enum DieuKhien {
    TIM_KIEM = 200, LOC_LOAI, SAP_XEP, CON_HANG, LOC_BAO_CAO,
    BANG_CHINH = 210, BANG_GIO, BANG_TON,
    DANH_SACH_MON = 214,
    LOC_NAM_DOANHTHU,
    DANH_SACH_THUCDON,
    O_MA = 300, O_TEN, O_SDT, O_GIOITINH, O_DIEM, O_CHUCVU, O_LUONG, O_PHUCAP,
    O_LOAI, O_GIA, O_TON, O_CAY, O_CHAY, O_SIZE, O_NHAPKHO, O_ANH,
    O_NGAY = 330, O_KHACH, O_NHANVIEN, O_SOLUONG, O_TONG,
    O_USER = 350, O_PASS, O_CONFIRM, O_ROLE, O_OLDPASS, O_EMAIL = 355,
    O_HANG = 360, O_UUDAI, NHAN_HOSO, NHAN_SIZE = 363,
    NHAN_KY_BAOCAO = 370,
    NHAN_MON_CHON, NHAN_GIO_TRONG,
    NUT_LUU = 400, NUT_MOI, NUT_XOA, NUT_NHAPKHO, NUT_THEM_GIO,
    NUT_XOA_GIO, NUT_HUY_GIO, NUT_THANHTOAN, NUT_BAOCAO, NUT_HOADON,
    NUT_DANGNHAP, NUT_TAOTAIKHOAN, NUT_DOIMATKHAU,
    NUT_MO_DANGKY, NUT_DANGKY, NUT_VE_DANGNHAP,
    NUT_GUI_DON, NUT_HUY_DON, NUT_THANHTOAN_DON, NUT_LAMMOI_DON,
    NUT_BC_HOMNAY, NUT_BC_THANG, NUT_BC_TATCA,
    NUT_NHOM_THUCAN, NUT_NHOM_NUOC,
    NUT_GIAM_SL, NUT_TANG_SL, NUT_CHON_ANH
};

enum class KieuNut { Phu, Chinh, NguyHiem, ThanhBen, Tab, TabChon, Chip, ChipChon };

// Kich thuoc bo cuc chung (pixel).
namespace BoCuc {
const int THANH_BEN = 248, X0 = 280, LE_PHAI = 32, Y0 = 124, CHAN = 44, KHOANG = 20, DEM = 24;
const int CAO_NHAP = 64; // nhan 20 + khoang 4 + khung 40
}

// Bang ListView dung chung cho cua so chinh va cua so hoa don.
using VeOTuyChinh = function<bool(HDC dc, int dong, int cot, RECT vung, bool daChon)>;
void ganKieuBang(HWND bang, const BoFont& font, int cotHuyHieu, int chieuCaoDong = 40);
bool laBangTuyChinh(HWND h);
LRESULT veDongBang(NMLVCUSTOMDRAW* ve, const BoFont& font, const VeOTuyChinh& veRieng = nullptr);
void mauHuyHieu(const wstring& chu, COLORREF& nen, COLORREF& mauChu);

void xemHoaDon(HWND chu, const HoaDon& hoaDon, const BoFont& font);
int chayGiaoDienCuaSo();

class CuaSoNhaHang {
    HWND cuaSo = nullptr;
    BoFont font;
    KhoAnh anh;
    HBRUSH coNen = nullptr, coThe = nullptr, coChiDoc = nullptr;
    DichVuNhaHang dichVu;

    // Trang thai giao dien dung chung.
    bool thietLap = false, dangVeBang = false, dangDangKy = false;
    int trangHienTai = TRANG_CHU, soLanSai = 0;
    int rongCuaSo = 1280, caoCuaSo = 820;
    struct DauThe {
        int x, y, w;
        wchar_t icon;
        wstring tieuDe, phu;
    };
    vector<RECT> cacThe;
    vector<DauThe> cacDauThe;
    map<int, RECT> khungNhap;
    int oDangChon = -1;
    map<HWND, KieuNut> kieuNut;
    map<HWND, wchar_t> iconNut;
    map<int, bool> danhDauGiaTri;
    map<HWND, COLORREF> mauChuNhan;
    HWND nutDangRe = nullptr;
    wstring thongBao = L"Sẵn sàng";

    // Trang thai nghiep vu giu giua cac trang (nhu ban cu).
    int nhomDatMon = 0, nhomThucDon = 0, vaiTroTao = 0;
    string maMonDangChon, maDangSua, anhDangChon, maHoaDon, ngayDon, khachDon, nhanVienDon;
    vector<YeuCauMon> gioHang;
    vector<HoaDon> hoaDonBaoCao;
    vector<DonDatMon> donDatHienThi;
    BaoCaoKinhDoanh baoCaoHienTai;
    string phamViBaoCao;
    int namBieuDo = 0;
    DoanhThuNam doanhThuNam;
    int thangDangXem = -1;

    // DieuKhien.cpp
    HWND tao(const wchar_t* lop, const wstring& noiDung, DWORD kieu,
             int id, int x, int y, int w, int h, DWORD moRong = 0);
    HWND nhan(const wstring& noiDung, int x, int y, int w, bool dam = false, int id = -1, int h = 20);
    HWND nhap(const wstring& ten, int id, int x, int y, int w,
              const wstring& giaTri = L"", bool matKhau = false);
    HWND chon(const wstring& ten, int id, int x, int y, int w, const vector<wstring>& cacMuc);
    HWND nut(const wstring& noiDung, int id, int x, int y, int w,
             KieuNut kieu = KieuNut::Phu, wchar_t icon = 0, int h = 40);
    void datKieuNut(int id, KieuNut kieu);
    void danhDau(const wstring& ten, int id, int x, int y, int w, bool giaTri = false);
    bool daDanhDau(int id) const;
    void datDanhDau(int id, bool giaTri);
    HWND bang(int id, int x, int y, int w, int h, const vector<pair<wstring, int>>& cot, int cotHuyHieu = -1);
    void dongBang(int id, const vector<wstring>& cot, int giaTri);
    void canCot(int id, int cot, int kieuCan);
    int dongChon(int id) const;
    HWND o(int id) const;
    wstring chu(int id) const;
    string duLieu(int id) const;
    void gan(int id, const wstring& giaTri);
    int luaChon(int id) const;
    void chonMuc(int id, int viTri);
    double soNhap(int id, const string& ten, bool nguyen = false) const;
    void xoaDieuKhien();
    void themThe(RECT vung);
    void themDauThe(int x, int y, int w, wchar_t icon, const wstring& tieuDe, const wstring& phu = L"");
    void veDauThe(HDC dc);
    bool trongThe(RECT vung) const;
    COLORREF mauNenTai(HWND con) const;
    void veNut(const DRAWITEMSTRUCT* ve);
    void veHopChon(HWND hop, HDC dc);
    void veMucHop(const DRAWITEMSTRUCT* ve);
    void veKhungNhap(HDC dc);
    static LRESULT CALLBACK thuTucNut(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
    static LRESULT CALLBACK thuTucHop(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

    // CuaSo.cpp
    int phaiNoiDung() const { return rongCuaSo - BoCuc::LE_PHAI; }
    int dayNoiDung() const { return caoCuaSo - BoCuc::CHAN - 8; }
    void hienTrang(int trangMoi, bool giuNhap = false);
    void thanhBen();
    void ve(HDC dc);
    void veThanhBen(HDC dc);
    void veDauTrang(HDC dc);
    void veTrang(HDC dc);
    void xuLyNut(int id, int suKien);
    void xuLyThongBao(NMHDR* thongBao, LRESULT& ketQua);
    void baoLoi(const string& noiDung);
    void thanhCong(const wstring& noiDung);
    bool xacNhan(const wstring& noiDung);
    static LRESULT CALLBACK thuTuc(HWND, UINT, WPARAM, LPARAM);

    // TrangDangNhap.cpp
    void trangDangNhap();
    void trangDangKy();
    void veManDangNhap(HDC dc);

    // TrangCaNhan.cpp
    void trangTaiKhoan();
    void trangMatKhau();
    void trangHoSoKhachHang();
    void veHoSoKhachHang(HDC dc);
    void trangLichSuKhachHang();
    string vaiTroDuocChon() const;

    // TrangDanhSach.cpp
    void trangDanhSach();
    void napBang();
    void chonDong();
    void luuBieuMau();
    void xoaBanGhi();

    // TrangThucDon.cpp
    void trangThucDon();
    void napThucDonAnh();
    int monThucDonDaChon() const;
    void doiLoaiMon();
    void chonAnhMon();
    void capNhatNutAnh();

    // TrangBanHang.cpp
    void trangBanHang();
    void trangDatMon();
    void dungLuoiDatMon(int x, int y, int w, int h);
    void napMonDat();
    int monDatDaChon() const;
    void chonMonDat();
    void napGio();
    void themVaoGio();
    void thanhToan();
    void doiSoLuong(int buoc);
    void tinhTongGio(double& tamTinh, double& giam) const;
    RECT vungTongGio() const;
    string khachDaChon() const;
    string nhanVienDaChon() const;
    void veTongGio(HDC dc);

    // TrangDonDat.cpp
    void trangDonDat();
    void chiTietDonDat();
    void xuLyDonDat(int id);

    // TrangBaoCao.cpp
    void trangBaoCao();
    void napBaoCao();
    void veThongKeBaoCao(HDC dc);

    // TrangTongQuan.cpp
    void trangChu();
    void trangChuKhachHang();
    void veThongKeTongQuan(HDC dc);
    void trangBieuDoDoanhThu();
    void napBieuDoDoanhThu();
    RECT vungBieuDo() const;
    void veBieuDoDoanhThu(HDC dc);
    void diChuyenBieuDo(int x, int y);

public:
    int chay(HINSTANCE ungDung, int cheDoHien);
};

#endif
