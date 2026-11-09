#include "DichVuNhaHang.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// Moi dong luu mot don; coDa van duoc doc de tuong thich voi du lieu cu.
string DichVuNhaHang::noiDungDonDat() const {
    ostringstream tep;
    for (const DonDatMon& don : cacDonDat) {
        tep << quoted(don.trangThai) << ' ' << quoted(don.maHoaDon) << ' '
            << quoted(don.duTinh.toFile()) << ' ' << don.cacMon.size();
        for (const YeuCauMon& mon : don.cacMon)
            tep << ' ' << quoted(mon.maMon) << ' ' << mon.soLuong << ' '
                << quoted(mon.sizeLy) << ' ' << mon.coDa;
        tep << '\n';
    }
    return tep.str();
}

void DichVuNhaHang::docDonDat() {
    ifstream tep("DuLieu/dondatmon.txt");
    vector<DonDatMon> ketQua;
    string dong;
    while (getline(tep, dong)) {
        if (dong.empty()) continue;
        istringstream doc(dong);
        DonDatMon don;
        string duTinh;
        int soMon = 0;
        if (!(doc >> quoted(don.trangThai) >> quoted(don.maHoaDon) >> quoted(duTinh) >> soMon) ||
            soMon < 1 || soMon > 200 ||
            (don.trangThai != "CHO" && don.trangThai != "HUY" && don.trangThai != "XONG"))
            throw runtime_error("Tệp đơn đặt món không hợp lệ. Hãy kiểm tra dữ liệu.");
        don.duTinh = HoaDon::fromFile(duTinh);
        if (don.duTinh.getMa().empty() || don.duTinh.getMaKhachHang().empty())
            throw runtime_error("Đơn đặt món thiếu mã đơn hoặc mã khách hàng.");
        for (int i = 0; i < soMon; i++) {
            YeuCauMon mon;
            if (!(doc >> quoted(mon.maMon) >> mon.soLuong >> quoted(mon.sizeLy) >> mon.coDa) ||
                mon.soLuong < 1 || mon.soLuong > 1000000000)
                throw runtime_error("Chi tiết đơn đặt món không hợp lệ.");
            don.cacMon.push_back(mon);
        }
        ketQua.push_back(don);
    }
    cacDonDat = ketQua;
}

DonDatMon& DichVuNhaHang::timDonDat(const string& ma) {
    yeuCauDangNhap();
    for (DonDatMon& don : cacDonDat) {
        if (don.duTinh.getMa() != ma) continue;
        if (laKhachHang() && don.duTinh.getMaKhachHang() != phien().maKhachHang) break;
        return don;
    }
    throw runtime_error("Không tìm thấy đơn đặt hoặc bạn không có quyền xem đơn này.");
}

vector<DonDatMon> DichVuNhaHang::danhSachDonDat() const {
    yeuCauDangNhap();
    vector<DonDatMon> ketQua;
    for (const DonDatMon& don : cacDonDat)
        if (!laKhachHang() || don.duTinh.getMaKhachHang() == phien().maKhachHang)
            ketQua.push_back(don);
    return ketQua;
}

HoaDon DichVuNhaHang::duTinhDatMon(const vector<YeuCauMon>& gioHang) {
    const KhachHang& khach = hoSoCuaToi();
    return lapDuTinh("", "", khach.getMa(), gioHang);
}

string DichVuNhaHang::datMon(const string& ngay, const vector<YeuCauMon>& gioHang) {
    // Ngay va mon duoc kiem tra lai o dich vu, khong tin tong tien tu giao dien.
    HoaDon duTinh = duTinhDatMon(gioHang);
    string ma = "DM";
    for (unsigned long long i = 1; ; i++) {
        string ungVien = "DM" + to_string(i);
        bool trung = false;
        for (const DonDatMon& don : cacDonDat) if (don.duTinh.getMa() == ungVien) trung = true;
        if (!trung) { ma = ungVien; break; }
    }
    // Dung chung kiem tra ngay cua hoa don qua ham duoc tach rieng.
    if (!ngayDatHopLe(ngay)) throw runtime_error("Ngày đặt không hợp lệ (dd/mm/yyyy).");
    DonDatMon don;
    don.duTinh = HoaDon(ma, ngay, phien().maKhachHang, "");
    for (const ChiTietHoaDon& ct : duTinh.getGioHang()) don.duTinh.themMon(ct);
    don.duTinh.setGiamGia(hoSoCuaToi().getPhanTramGiam());
    don.cacMon = gioHang;
    vector<DonDatMon> cu = cacDonDat;
    try { cacDonDat.push_back(don); luuThayDoi(); }
    catch (...) { cacDonDat = cu; throw; }
    return ma;
}

void DichVuNhaHang::huyDonDat(const string& ma) {
    if (!laKhachHang()) yeuCauNhanVien();
    DonDatMon& don = timDonDat(ma);
    if (don.trangThai != "CHO") throw runtime_error("Chỉ có thể hủy đơn đang chờ.");
    vector<DonDatMon> cu = cacDonDat;
    don.trangThai = "HUY";
    try { luuThayDoi(); } catch (...) { cacDonDat = cu; throw; }
}

HoaDon DichVuNhaHang::xemTruocDonDat(const string& ma, const string& ngay, const string& nhanVien) {
    yeuCauNhanVien();
    const DonDatMon& don = timDonDat(ma);
    if (don.trangThai != "CHO") throw runtime_error("Đơn này đã hủy hoặc đã thanh toán.");
    return xemTruocHoaDon("HD_" + ma, ngay, don.duTinh.getMaKhachHang(), nhanVien, don.cacMon);
}

HoaDon DichVuNhaHang::thanhToanDonDat(const string& ma, const string& ngay, const string& nhanVien) {
    HoaDon hoaDon = xemTruocDonDat(ma, ngay, nhanVien);
    vector<DonDatMon> cu = cacDonDat;
    DonDatMon& don = timDonDat(ma);
    string khach = don.duTinh.getMaKhachHang();
    for (const YeuCauMon& mon : don.cacMon) duLieu.timMon(mon.maMon)->truTonKho(mon.soLuong);
    duLieu.timKhach(khach)->congDiem(static_cast<int>(hoaDon.tongTien() / 10000));
    duLieu.dsHoaDon.push_back(hoaDon);
    don.trangThai = "XONG";
    don.maHoaDon = hoaDon.getMa();
    try { luuThayDoi(); } catch (...) { cacDonDat = cu; throw; }
    return hoaDon;
}
