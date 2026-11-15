#define NOMINMAX
#include "TienIch.h"
#include "DichVuNhaHang.h"
#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"
#include <windows.h>
#include <cmath>
#include <cctype>
#include <map>
#include <iomanip>
#include <sstream>
#include <utility>
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace {
void kiemTraChuoi(const string& giaTri, const string& ten, bool laMa = false) {
    if (giaTri.empty() || giaTri.size() > 160 ||
        giaTri.find_first_of("|;,\r\n") != string::npos) {
        throw runtime_error(ten + ": không để trống, tối đa 160 byte và không chứa | ; , hoặc xuống dòng.");
    }
    if (laMa) {
        for (unsigned char kyTu : giaTri) {
            if (!((kyTu >= 'a' && kyTu <= 'z') || (kyTu >= 'A' && kyTu <= 'Z') ||
                  (kyTu >= '0' && kyTu <= '9') || kyTu == '_' || kyTu == '-')) {
                throw runtime_error(ten + " chỉ gồm chữ không dấu, chữ số, _ hoặc -.");
            }
        }
    }
}

void kiemTraSo(double so, const string& ten) {
    if (!isfinite(so) || so < 0 || so > 1000000000) {
        throw runtime_error(ten + " phải nằm trong khoảng 0 đến 1.000.000.000.");
    }
}

bool dienThoaiHopLe(const string& dienThoai) {
    return dienThoai.size() >= 9 && dienThoai.size() <= 15 &&
        all_of(dienThoai.begin(), dienThoai.end(), [](unsigned char c) { return c >= '0' && c <= '9'; });
}

bool emailHopLe(const string& email) {
    return email.find('@') != string::npos;
}

bool ngayHopLe(const string& ngay) {
    if (ngay.size() != 10 || ngay[2] != '/' || ngay[5] != '/') return false;
    for (size_t i = 0; i < ngay.size(); i++) {
        if (i != 2 && i != 5 && (ngay[i] < '0' || ngay[i] > '9')) return false;
    }
    int d = stoi(ngay.substr(0, 2));
    int m = stoi(ngay.substr(3, 2));
    int y = stoi(ngay.substr(6));
    if (y < 1900 || y > 9999 || m < 1 || m > 12) return false;
    int soNgay[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)) soNgay[1] = 29;
    return d >= 1 && d <= soNgay[m - 1];
}
}

bool DichVuNhaHang::khoiTao() {
    docDuLieuSql();
    docDonDat();
    canThietLap = !duLieu.quanLyTaiKhoan.docDuLieu();
    return canThietLap;
}

bool DichVuNhaHang::ngayDatHopLe(const string& ngay) const { return ngayHopLe(ngay); }

bool DichVuNhaHang::daDangNhap() const { return !duLieu.taiKhoanHienTai.tenDangNhap.empty(); }
bool DichVuNhaHang::laQuanLy() const { return daDangNhap() && phien().vaiTro == "QUANLY"; }
bool DichVuNhaHang::laKhachHang() const { return daDangNhap() && phien().vaiTro == "KHACHHANG"; }
const TaiKhoan& DichVuNhaHang::phien() const { return duLieu.taiKhoanHienTai; }

void DichVuNhaHang::yeuCauDangNhap() const {
    if (!daDangNhap()) throw runtime_error("Bạn cần đăng nhập để sử dụng chức năng này.");
}
void DichVuNhaHang::yeuCauQuanLy() const {
    if (!laQuanLy()) throw runtime_error("Chức năng này chỉ dành cho quản lý.");
}

void DichVuNhaHang::yeuCauNhanSu() const {
    yeuCauDangNhap();
    if (phien().vaiTro != "QUANLY" && phien().vaiTro != "NHANVIEN") {
        throw runtime_error("Chức năng này chỉ dành cho nhân viên và quản lý.");
    }
}

void DichVuNhaHang::yeuCauNhanVien() const {
    yeuCauDangNhap();
    if (phien().vaiTro != "NHANVIEN")
        throw runtime_error("Bán hàng và xử lý đơn đặt chỉ dành cho nhân viên.");
}

bool DichVuNhaHang::dangNhap(const string& ten, const string& matKhau) {
    dangXuat();
    if (!duLieu.quanLyTaiKhoan.kiemTraMatKhau(ten, matKhau)) return false;
    const TaiKhoan* taiKhoan = duLieu.quanLyTaiKhoan.tim(ten);
    if (taiKhoan->vaiTro == "NHANVIEN" && !taiKhoan->maLienKet.empty() && duLieu.timNhanVien(taiKhoan->maLienKet) == nullptr) return false;
    if (taiKhoan->vaiTro == "KHACHHANG" && !taiKhoan->maLienKet.empty() && duLieu.timKhach(taiKhoan->maLienKet) == nullptr) return false;
    duLieu.taiKhoanHienTai = *taiKhoan;
    return true;
}
void DichVuNhaHang::dangXuat() { duLieu.taiKhoanHienTai = TaiKhoan(); }

void DichVuNhaHang::taoTaiKhoan(const string& ten, const string& matKhau,
                               const string& xacNhan, const string& vaiTro, const string& maHoSo) {
    if (!canThietLap) yeuCauQuanLy();
    if (ten == "0") throw runtime_error("Tên đăng nhập không được là 0.");
    if (matKhau != xacNhan) throw runtime_error("Hai mật khẩu không trùng nhau.");
    if (canThietLap && vaiTro != "QUANLY") throw runtime_error("Tài khoản đầu tiên phải là quản lý.");
    string maLienKet;
    if (vaiTro == "NHANVIEN") {
        if (duLieu.timNhanVien(maHoSo) == nullptr)
            throw runtime_error("Mã nhân viên không tồn tại. Hãy thêm hồ sơ nhân viên trước.");
        maLienKet = maHoSo;
    } else if (vaiTro == "KHACHHANG") {
        if (duLieu.timKhach(maHoSo) == nullptr)
            throw runtime_error("Mã khách hàng không tồn tại. Hãy thêm hồ sơ khách hàng trước.");
        maLienKet = maHoSo;
    }
    duLieu.quanLyTaiKhoan.tao(ten, matKhau, vaiTro, maLienKet);
    canThietLap = false;
}

void DichVuNhaHang::doiMatKhau(const string& cu, const string& moi, const string& xacNhan) {
    yeuCauDangNhap();
    if (!duLieu.quanLyTaiKhoan.kiemTraMatKhau(phien().tenDangNhap, cu)) throw runtime_error("Mật khẩu hiện tại chưa đúng.");
    if (moi != xacNhan) throw runtime_error("Hai mật khẩu mới không trùng nhau.");
    duLieu.quanLyTaiKhoan.doiMatKhau(phien().tenDangNhap, moi);
}

const vector<KhachHang>& DichVuNhaHang::khachHang() const { yeuCauNhanSu(); return duLieu.dsKhachHang; }
const vector<NhanVien>& DichVuNhaHang::nhanVien() const { yeuCauQuanLy(); return duLieu.dsNhanVien; }
const vector<unique_ptr<Mon>>& DichVuNhaHang::thucDon() const { yeuCauDangNhap(); return duLieu.dsMon; }

void DichVuNhaHang::luuThayDoi(bool kemTaiKhoan) {
    Sql::CacDong mon, khachHang, nhanVien, hoaDon;
    for (const auto& m : duLieu.dsMon) mon.push_back(m->cacCot());
    for (const auto& kh : duLieu.dsKhachHang) khachHang.push_back(kh.cacCot());
    for (const auto& nv : duLieu.dsNhanVien) nhanVien.push_back(nv.cacCot());
    for (const auto& hd : duLieu.dsHoaDon) hoaDon.push_back(hd.cacCot());
    vector<pair<Sql::Bang, Sql::CacDong>> thayDoi = {
        {Sql::ThucDon, mon}, {Sql::KhachHang, khachHang}, {Sql::NhanVien, nhanVien},
        {Sql::HoaDon, hoaDon}, {Sql::DonDat, noiDungDonDat()}
    };
    if (kemTaiKhoan)
        thayDoi.emplace_back(Sql::TaiKhoan, duLieu.quanLyTaiKhoan.cacDongTaiKhoan(duLieu.quanLyTaiKhoan.danhSach));
    try {
        Sql::luu(thayDoi);
    } catch (...) {
        docDuLieuSql();
        docDonDat();
        if (kemTaiKhoan) duLieu.quanLyTaiKhoan.docDuLieu();
        throw;
    }
}

void DichVuNhaHang::luuKhachHang(const KhachHang& khach, bool themMoi) {
    yeuCauNhanSu();
    kiemTraChuoi(khach.getMa(), "Mã khách hàng", true);
    kiemTraChuoi(khach.getTen(), "Họ tên");
    KhachHang* cu = duLieu.timKhach(khach.getMa());
    kiemTraChuoi(khach.getSoDienThoai(), "Điện thoại");
    if ((themMoi || !cu || cu->getSoDienThoai() != khach.getSoDienThoai()) &&
        !dienThoaiHopLe(khach.getSoDienThoai())) throw runtime_error("Điện thoại cần có từ 9 đến 15 chữ số.");
    if (!khach.getEmail().empty() && !emailHopLe(khach.getEmail())) throw runtime_error("Email cần có ký tự @.");
    kiemTraChuoi(khach.getGioiTinh(), "Giới tính");
    kiemTraSo(khach.getDiemTichLuy(), "Điểm tích lũy");
    if (themMoi && cu != nullptr) throw runtime_error("Mã khách hàng đã tồn tại.");
    if (!themMoi && cu == nullptr) throw runtime_error("Không tìm thấy khách hàng cần sửa.");
    if (cu) *cu = khach;
    else duLieu.dsKhachHang.push_back(khach);
    luuThayDoi();
}
void DichVuNhaHang::luuNhanVien(const NhanVien& nv, bool themMoi) {
    yeuCauQuanLy();
    kiemTraChuoi(nv.getMa(), "Mã nhân viên", true);
    kiemTraChuoi(nv.getTen(), "Họ tên");
    NhanVien* cu = duLieu.timNhanVien(nv.getMa());
    kiemTraChuoi(nv.getSoDienThoai(), "Điện thoại");
    if ((themMoi || !cu || cu->getSoDienThoai() != nv.getSoDienThoai()) &&
        !dienThoaiHopLe(nv.getSoDienThoai())) throw runtime_error("Điện thoại cần có từ 9 đến 15 chữ số.");
    kiemTraChuoi(nv.getGioiTinh(), "Giới tính");
    kiemTraChuoi(nv.getChucVu(), "Chức vụ");
    kiemTraSo(nv.getLuongCoBan(), "Lương");
    kiemTraSo(nv.getPhuCap(), "Phụ cấp");
    if (themMoi && cu) throw runtime_error("Mã nhân viên đã tồn tại.");
    if (!themMoi && !cu) throw runtime_error("Không tìm thấy nhân viên cần sửa.");
    if (cu) *cu = nv;
    else duLieu.dsNhanVien.push_back(nv);
    luuThayDoi();
}
string DichVuNhaHang::maMonMoi(const string& loai) const {
    string tienTo = (loai == "Nuoc uong" || loai == "NUOC") ? "NU" : "MA";
    for (int i = 1; i < 1000000; i++) {
        ostringstream ma;
        ma << tienTo << setfill('0') << setw(4) << i;
        string ungVien = ma.str();
        bool trung = false;
        for (const auto& m : duLieu.dsMon) {
            if (m->getMa() == ungVien) { trung = true; break; }
        }
        if (!trung) return ungVien;
    }
    return tienTo + "0001";
}

string DichVuNhaHang::luuMon(const DuLieuMon& mon, bool themMoi) {
    yeuCauQuanLy();
    string maLuu = themMoi ? maMonMoi(mon.loai) : mon.ma;
    kiemTraChuoi(maLuu, "Mã món", true);
    kiemTraChuoi(mon.ten, "Tên món");
    kiemTraSo(mon.gia, "Giá");
    kiemTraSo(mon.tonKho, "Tồn kho");
    if (mon.hinhAnh.size() > 500) throw runtime_error("Đường dẫn hình ảnh quá dài (tối đa 500 ký tự).");
    unique_ptr<Mon> moi;
    if (mon.loai == "Mon an") {
        if (mon.doCay < 0 || mon.doCay > 5) throw runtime_error("Độ cay phải từ 0 đến 5.");
        moi = make_unique<MonAn>(maLuu, mon.ten, mon.gia, mon.tonKho, mon.doCay, mon.monChay);
    } else if (mon.loai == "Nuoc uong") {
        if (mon.sizeLy != "S" && mon.sizeLy != "M" && mon.sizeLy != "L") throw runtime_error("Size phải là S, M hoặc L.");
        moi = make_unique<NuocUong>(maLuu, mon.ten, mon.gia, mon.tonKho, mon.sizeLy, mon.coDa);
    } else throw runtime_error("Loại món không hợp lệ.");
    moi->setHinhAnh(mon.hinhAnh);
    bool timThay = false;
    for (unique_ptr<Mon>& cu : duLieu.dsMon) {
        if (cu->getMa() == maLuu) {
            if (themMoi) throw runtime_error("Mã món đã tồn tại.");
            cu = move(moi);
            timThay = true;
            break;
        }
    }
    if (!timThay) {
        if (!themMoi) throw runtime_error("Không tìm thấy món cần sửa.");
        duLieu.dsMon.push_back(move(moi));
    }
    luuThayDoi();
    return maLuu;
}
void DichVuNhaHang::xoaKhachHang(const string& ma) {
    yeuCauNhanSu();
    for (size_t i = 0; i < duLieu.dsKhachHang.size(); i++) {
        if (duLieu.dsKhachHang[i].getMa() == ma) {
            duLieu.dsKhachHang.erase(duLieu.dsKhachHang.begin() + i);
            auto& dsTK = duLieu.quanLyTaiKhoan.danhSach;
            dsTK.erase(remove_if(dsTK.begin(), dsTK.end(),
                [&](const TaiKhoan& tk) { return tk.vaiTro == "KHACHHANG" && tk.maLienKet == ma; }), dsTK.end());
            luuThayDoi(true);
            return;
        }
    }
    throw runtime_error("Không tìm thấy khách hàng.");
}
void DichVuNhaHang::xoaNhanVien(const string& ma) {
    yeuCauQuanLy();
    for (size_t i = 0; i < duLieu.dsNhanVien.size(); i++) {
        if (duLieu.dsNhanVien[i].getMa() == ma) {
            duLieu.dsNhanVien.erase(duLieu.dsNhanVien.begin() + i);
            auto& dsTK = duLieu.quanLyTaiKhoan.danhSach;
            dsTK.erase(remove_if(dsTK.begin(), dsTK.end(),
                [&](const TaiKhoan& tk) { return tk.vaiTro == "NHANVIEN" && tk.maLienKet == ma; }), dsTK.end());
            luuThayDoi(true);
            return;
        }
    }
    throw runtime_error("Không tìm thấy nhân viên.");
}
void DichVuNhaHang::xoaMon(const string& ma) {
    yeuCauQuanLy();
    for (size_t i = 0; i < duLieu.dsMon.size(); i++) {
        if (duLieu.dsMon[i]->getMa() == ma) {
            duLieu.dsMon.erase(duLieu.dsMon.begin() + i);
            luuThayDoi(); return;
        }
    }
    throw runtime_error("Không tìm thấy món.");
}
void DichVuNhaHang::nhapKho(const string& ma, int soLuong) {
    yeuCauQuanLy();
    Mon* mon = duLieu.timMon(ma);
    if (!mon) throw runtime_error("Chọn món cần nhập kho.");
    if (soLuong < 1 || soLuong > 1000000000 - mon->getTonKho()) throw runtime_error("Số lượng nhập kho không hợp lệ.");
    mon->congTonKho(soLuong);
    luuThayDoi();
}

HoaDon DichVuNhaHang::xemTruocHoaDon(const string& ma, const string& ngay, const string& khach,
                                    const string& /*nhanVien*/, const vector<YeuCauMon>& gioHang) {
    yeuCauNhanVien();
    kiemTraChuoi(ma, "Mã hóa đơn", true);
    if (!ngayHopLe(ngay)) throw runtime_error("Ngày phải có dạng dd/mm/yyyy và tồn tại trên lịch.");
    for (const HoaDon& hd : duLieu.dsHoaDon) {
        if (hd.getMa() == ma) throw runtime_error("Mã hóa đơn đã tồn tại.");
    }
    string nguoiBan = phien().maLienKet.empty() ? phien().tenDangNhap : phien().maLienKet;
    HoaDon duTinh = lapDuTinh(ma, ngay, khach, gioHang);
    HoaDon hoaDon(ma, ngay, khach, nguoiBan);
    for (const ChiTietHoaDon& ct : duTinh.getGioHang()) hoaDon.themMon(ct);
    KhachHang* kh = duLieu.timKhach(khach);
    if (kh) hoaDon.setGiamGia(kh->getPhanTramGiam());
    return hoaDon;
}

HoaDon DichVuNhaHang::lapDuTinh(const string& ma, const string& ngay, const string& khach,
                                const vector<YeuCauMon>& gioHang) {
    if (gioHang.empty() || gioHang.size() > 200) throw runtime_error("Giỏ hàng cần có từ 1 đến 200 lựa chọn món.");
    KhachHang* kh = duLieu.timKhach(khach);
    if (!khach.empty() && !kh) throw runtime_error("Khách hàng đã bị xóa. Hãy chọn lại.");
    HoaDon hoaDon(ma, ngay, khach, "");
    map<string, int> tongSoLuong;
    for (const YeuCauMon& muc : gioHang) {
        Mon* mon = duLieu.timMon(muc.maMon);
        if (!mon || muc.soLuong < 1 || muc.soLuong > 1000000000) throw runtime_error("Món hoặc số lượng trong giỏ không hợp lệ.");
        if (muc.soLuong > mon->getTonKho() - tongSoLuong[muc.maMon]) throw runtime_error("Không đủ tồn kho cho món: " + mon->getTen());
        tongSoLuong[muc.maMon] += muc.soLuong;
        string ten = mon->getTen();
        if (mon->getLoai() == "Nuoc uong") {
            if (muc.sizeLy != "S" && muc.sizeLy != "M" && muc.sizeLy != "L") throw runtime_error("Size nước uống không hợp lệ.");
            ten += " (Size " + muc.sizeLy + ")";
        }
        hoaDon.themMon({muc.maMon, ten, muc.soLuong, mon->getGia()});
    }
    if (kh) hoaDon.setGiamGia(kh->getPhanTramGiam());
    if (hoaDon.tamTinh() <= 0) throw runtime_error("Hóa đơn cần có tổng tiền lớn hơn 0.");
    if (kh && hoaDon.tongTien() / 10000 > 1000000000 - kh->getDiemTichLuy()) throw runtime_error("Điểm tích lũy vượt giới hạn.");
    return hoaDon;
}
HoaDon DichVuNhaHang::thanhToan(const string& ma, const string& ngay, const string& khach,
                                const string& nhanVien, const vector<YeuCauMon>& gioHang) {
    HoaDon hoaDon = xemTruocHoaDon(ma, ngay, khach, nhanVien, gioHang);
    for (const YeuCauMon& muc : gioHang) duLieu.timMon(muc.maMon)->truTonKho(muc.soLuong);
    KhachHang* kh = duLieu.timKhach(khach);
    if (kh) kh->congDiem(static_cast<int>(hoaDon.tongTien() / 10000));
    duLieu.dsHoaDon.push_back(hoaDon);
    luuThayDoi();
    return hoaDon;
}

BaoCaoKinhDoanh DichVuNhaHang::baoCao(const string& phamVi) const {
    yeuCauQuanLy();
    if (!phamVi.empty()) {
        string ngay = phamVi;
        if (phamVi.size() == 7) ngay = "01/" + phamVi;
        if (!ngayHopLe(ngay)) throw runtime_error("Bộ lọc là dd/mm/yyyy, mm/yyyy hoặc để trống.");
    }
    BaoCaoKinhDoanh baoCao;
    map<string, pair<string, int>> thongKe;
    for (const HoaDon& hd : duLieu.dsHoaDon) {
        bool phuHop = phamVi.empty() || hd.getNgayLap() == phamVi;
        if (phamVi.size() == 7 && hd.getNgayLap().size() == 10) phuHop = hd.getNgayLap().substr(3) == phamVi;
        if (!phuHop) continue;
        baoCao.doanhThu += hd.tongTien();
        baoCao.soHoaDon++;
        baoCao.hoaDon.push_back(hd);
        for (const ChiTietHoaDon& ct : hd.getGioHang()) {
            thongKe[ct.maMon].first = ct.tenMon;
            thongKe[ct.maMon].second += ct.soLuong;
        }
    }
    int nhieuNhat = 0;
    for (const pair<const string, pair<string, int>>& muc : thongKe) {
        if (muc.second.second > nhieuNhat) {
            nhieuNhat = muc.second.second;
            baoCao.monBanChay = muc.second.first + " (" + to_string(nhieuNhat) + ")";
        }
    }
    return baoCao;
}

DoanhThuNam DichVuNhaHang::doanhThuTheoNam(int nam) const {
    yeuCauQuanLy();
    if(nam<1900 || nam>9999) throw runtime_error("Năm báo cáo không hợp lệ.");
    DoanhThuNam ketQua;
    ketQua.cacNam.push_back(nam);
    for(const HoaDon& hoaDon:duLieu.dsHoaDon) {
        const string& ngay=hoaDon.getNgayLap();
        double tien=hoaDon.tongTien();
        if(!ngayHopLe(ngay) || !isfinite(tien) || tien<0) {
            ketQua.soHoaDonBoQua++;
            continue;
        }
        int namDon=stoi(ngay.substr(6)), thang=stoi(ngay.substr(3,2));
        ketQua.cacNam.push_back(namDon);
        if(namDon!=nam) continue;
        ketQua.cacThang[thang-1]+=tien;
        ketQua.tongTien+=tien;
        ketQua.soHoaDon++;
    }
    sort(ketQua.cacNam.rbegin(),ketQua.cacNam.rend());
    ketQua.cacNam.erase(unique(ketQua.cacNam.begin(),ketQua.cacNam.end()),ketQua.cacNam.end());
    return ketQua;
}

// Cac API nay khong nhan ma khach tu giao dien: luon lay tu phien dang nhap.
const KhachHang& DichVuNhaHang::hoSoCuaToi() const {
    if (!laKhachHang()) throw runtime_error("Bạn cần đăng nhập tài khoản khách hàng.");
    string maCanTim = phien().maLienKet.empty() ? phien().tenDangNhap : phien().maLienKet;
    for (const KhachHang& khach : duLieu.dsKhachHang) {
        if (khach.getMa() == maCanTim) return khach;
    }
    static KhachHang macDinh;
    macDinh = KhachHang(maCanTim, phien().tenDangNhap, "", "Khác", 0, "");
    return macDinh;
}

vector<HoaDon> DichVuNhaHang::hoaDonCuaToi() const {
    const KhachHang& khach = hoSoCuaToi();
    vector<HoaDon> ketQua;
    for (const HoaDon& hoaDon : duLieu.dsHoaDon) {
        if (hoaDon.getMaKhachHang() == khach.getMa()) ketQua.push_back(hoaDon);
    }
    return ketQua;
}

HoaDon DichVuNhaHang::hoaDonDuocXem(const string& maHoaDon) const {
    yeuCauDangNhap();
    if (!laQuanLy() && !laKhachHang()) throw runtime_error("Bạn không có quyền xem lịch sử hóa đơn.");
    string maKH = laKhachHang() ? hoSoCuaToi().getMa() : "";
    for (const HoaDon& hoaDon : duLieu.dsHoaDon) {
        if (hoaDon.getMa() != maHoaDon) continue;
        if (laQuanLy() || hoaDon.getMaKhachHang() == maKH) return hoaDon;
        break;
    }
    throw runtime_error("Không tìm thấy hóa đơn hoặc bạn không có quyền xem hóa đơn này.");
}

string DichVuNhaHang::taoTaiKhoanNhanVien(const string& tenDangNhap, const string& matKhau, const string& xacNhan,
                                          const string& hoTen, const string& dienThoai, const string& gioiTinh,
                                          const string& chucVu, double luong, double phuCap) {
    yeuCauQuanLy();
    if (matKhau != xacNhan) throw runtime_error("Hai mật khẩu không trùng nhau.");
    if (tenDangNhap == "0") throw runtime_error("Tên đăng nhập không được là 0.");
    kiemTraChuoi(hoTen, "Họ và tên");
    if (!dienThoaiHopLe(dienThoai)) throw runtime_error("Số điện thoại cần có từ 9 đến 15 chữ số.");
    kiemTraChuoi(gioiTinh, "Giới tính");
    kiemTraChuoi(chucVu, "Chức vụ");
    kiemTraSo(luong, "Lương");
    kiemTraSo(phuCap, "Phụ cấp");

    string maMoi;
    for (int i = 1; i < 1000000; i++) {
        ostringstream ma;
        ma << "NV" << setfill('0') << setw(4) << i;
        string ungVien = ma.str();
        if (duLieu.timNhanVien(ungVien) == nullptr) { maMoi = ungVien; break; }
    }
    TaiKhoan taiKhoan = duLieu.quanLyTaiKhoan.chuanBi(tenDangNhap, matKhau, "NHANVIEN", maMoi);
    vector<NhanVien> nvCu = duLieu.dsNhanVien;
    vector<TaiKhoan> taiKhoanCu = duLieu.quanLyTaiKhoan.danhSach;
    try {
        duLieu.dsNhanVien.push_back(NhanVien(maMoi, hoTen, dienThoai, gioiTinh, chucVu, luong, phuCap));
        duLieu.quanLyTaiKhoan.danhSach.push_back(taiKhoan);
        luuThayDoi(true);
    } catch (...) {
        duLieu.dsNhanVien = nvCu;
        duLieu.quanLyTaiKhoan.danhSach = taiKhoanCu;
        throw;
    }
    return maMoi;
}

string DichVuNhaHang::taoTaiKhoanKhachHang(const string& tenDangNhap, const string& matKhau, const string& xacNhan,
                                           const string& hoTen, const string& dienThoai, const string& gioiTinh,
                                           const string& email) {
    if (matKhau != xacNhan) throw runtime_error("Hai mật khẩu không trùng nhau.");
    if (tenDangNhap == "0") throw runtime_error("Tên đăng nhập không được là 0.");
    size_t dau = hoTen.find_first_not_of(" \t");
    size_t cuoi = hoTen.find_last_not_of(" \t");
    string tenSach;
    if (dau != string::npos) tenSach = hoTen.substr(dau, cuoi - dau + 1);
    kiemTraChuoi(tenSach, "Họ và tên");
    if (!dienThoaiHopLe(dienThoai)) throw runtime_error("Số điện thoại cần có từ 9 đến 15 chữ số.");
    if (!email.empty() && !emailHopLe(email)) throw runtime_error("Email cần có ký tự @ (ví dụ: ten@gmail.com).");
    if (gioiTinh != "Nam" && gioiTinh != "Nữ" && gioiTinh != "Nu" && gioiTinh != "Khác") {
        throw runtime_error("Hãy chọn giới tính hợp lệ.");
    }
    string maMoi;
    for (int i = 1; i < 1000000; i++) {
        ostringstream ma;
        ma << "KH" << setfill('0') << setw(4) << i;
        string ungVien = ma.str();
        if (duLieu.timKhach(ungVien) == nullptr) { maMoi = ungVien; break; }
    }
    TaiKhoan taiKhoan = duLieu.quanLyTaiKhoan.chuanBi(tenDangNhap, matKhau, "KHACHHANG", maMoi);
    vector<KhachHang> khachCu = duLieu.dsKhachHang;
    vector<TaiKhoan> taiKhoanCu = duLieu.quanLyTaiKhoan.danhSach;
    try {
        duLieu.dsKhachHang.push_back(KhachHang(maMoi, tenSach, dienThoai, gioiTinh, 0, email));
        duLieu.quanLyTaiKhoan.danhSach.push_back(taiKhoan);
        luuThayDoi(true);
    } catch (...) {
        duLieu.dsKhachHang = khachCu;
        duLieu.quanLyTaiKhoan.danhSach = taiKhoanCu;
        throw;
    }
    return maMoi;
}

string DichVuNhaHang::dangKyKhachHang(const string& hoTen, const string& dienThoai, const string& email,
                                     const string& gioiTinh, const string& tenDangNhap,
                                     const string& matKhau, const string& xacNhan) {
    if (canThietLap || duLieu.quanLyTaiKhoan.danhSach.empty()) throw runtime_error("Nhà hàng cần tạo tài khoản quản lý đầu tiên trước.");
    if (daDangNhap()) throw runtime_error("Hãy đăng xuất trước khi đăng ký tài khoản mới.");
    if (!emailHopLe(email)) throw runtime_error("Email cần có ký tự @ (ví dụ: ten@gmail.com).");
    return taoTaiKhoanKhachHang(tenDangNhap, matKhau, xacNhan, hoTen, dienThoai, gioiTinh, email);
}

int soNguyen(const string& s) {
    try { return s.empty() ? 0 : stoi(s); } catch (...) { return 0; }
}

double soThuc(const string& s) {
    try { return s.empty() ? 0.0 : stod(s); } catch (...) { return 0.0; }
}

void DichVuNhaHang::docDuLieuSql() {
    duLieu.dsKhachHang.clear();
    duLieu.dsNhanVien.clear();
    duLieu.dsMon.clear();
    duLieu.dsHoaDon.clear();
    for (const auto& c : Sql::doc(Sql::ThucDon)) {
        if (c.size() < 7 || c[1].empty()) continue;
        unique_ptr<Mon> m;
        if (c[0] == "NUOC")
            m = make_unique<NuocUong>(c[1], c[2], soThuc(c[3]), soNguyen(c[4]), c[5].empty() ? "M" : c[5], soNguyen(c[6]));
        else
            m = make_unique<MonAn>(c[1], c[2], soThuc(c[3]), soNguyen(c[4]), soNguyen(c[5]), soNguyen(c[6]));
        if (c.size() >= 8) m->setHinhAnh(c[7]);
        duLieu.dsMon.push_back(move(m));
    }
    for (const auto& c : Sql::doc(Sql::KhachHang)) {
        if (c.size() < 6 || c[0].empty()) continue;
        duLieu.dsKhachHang.emplace_back(c[0], c[1], c[2], c[3].empty() ? "Khác" : c[3], soNguyen(c[4]), c[5]);
    }
    for (const auto& c : Sql::doc(Sql::NhanVien)) {
        if (c.size() < 7 || c[0].empty()) continue;
        duLieu.dsNhanVien.emplace_back(c[0], c[1], c[2], c[3].empty() ? "Khác" : c[3], c[4], soThuc(c[5]), soThuc(c[6]));
    }
    for (const auto& c : Sql::doc(Sql::HoaDon)) {
        if (c.size() < 6 || c[0].empty()) continue;
        duLieu.dsHoaDon.push_back(HoaDon::tuCacCot(c));
    }
}
