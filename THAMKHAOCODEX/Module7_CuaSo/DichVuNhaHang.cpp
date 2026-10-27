#define NOMINMAX
#include "DichVuNhaHang.h"
#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"
#include <windows.h>
#include <cmath>
#include <fstream>
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
    duLieu.docDuLieu();
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
    if (taiKhoan->vaiTro == "NHANVIEN" && duLieu.timNhanVien(taiKhoan->maNhanVien) == nullptr) return false;
    if (taiKhoan->vaiTro == "KHACHHANG" && duLieu.timKhach(taiKhoan->maKhachHang) == nullptr) return false;
    duLieu.taiKhoanHienTai = *taiKhoan;
    return true;
}
void DichVuNhaHang::dangXuat() { duLieu.taiKhoanHienTai = TaiKhoan(); }

void DichVuNhaHang::taoTaiKhoan(const string& ten, const string& matKhau,
                               const string& xacNhan, const string& vaiTro, const string& maLienKet) {
    if (!canThietLap) yeuCauQuanLy();
    if (ten == "0") throw runtime_error("Tên đăng nhập không được là 0.");
    if (matKhau != xacNhan) throw runtime_error("Hai mật khẩu không trùng nhau.");
    if (canThietLap && vaiTro != "QUANLY") throw runtime_error("Tài khoản đầu tiên phải là quản lý.");
    if (vaiTro == "NHANVIEN" && duLieu.timNhanVien(maLienKet) == nullptr) {
        throw runtime_error("Mã nhân viên không tồn tại. Hãy thêm hồ sơ nhân viên trước.");
    }
    if (vaiTro == "KHACHHANG" && duLieu.timKhach(maLienKet) == nullptr) {
        throw runtime_error("Mã khách hàng không tồn tại. Hãy thêm hồ sơ khách hàng trước.");
    }
    string ma = maLienKet;
    if (vaiTro == "QUANLY") ma.clear();
    duLieu.quanLyTaiKhoan.tao(ten, matKhau, vaiTro, ma);
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

// Chuan bi tat ca tep truoc khi thay the. Neu ghi loi, phuc hoi tep da thay.
void DichVuNhaHang::luuThayDoi(bool kemTaiKhoan) {
    vector<string> tenTep = {"menu.txt", "khachhang.txt", "nhanvien.txt", "hoadon.txt", "dondatmon.txt"};
    if (kemTaiKhoan) tenTep.push_back("taikhoan.txt");
    vector<string> noiDung(tenTep.size());
    for (const unique_ptr<Mon>& mon : duLieu.dsMon) noiDung[0] += mon->toFile() + "\n";
    for (const KhachHang& khach : duLieu.dsKhachHang) noiDung[1] += khach.toFile() + "\n";
    for (const NhanVien& nv : duLieu.dsNhanVien) noiDung[2] += nv.toFile() + "\n";
    for (const HoaDon& hd : duLieu.dsHoaDon) noiDung[3] += hd.toFile() + "\n";
    noiDung[4] = noiDungDonDat();
    if (kemTaiKhoan) noiDung[5] = duLieu.quanLyTaiKhoan.noiDungTep(duLieu.quanLyTaiKhoan.danhSach);
    int daThay = 0;
    vector<bool> tepCuTonTai(tenTep.size(), false);
    try {
        for (size_t i = 0; i < tenTep.size(); i++) {
            string tep = "DuLieu/" + tenTep[i];
            ofstream moi(tep + ".gui-new", ios::binary | ios::trunc);
            moi << noiDung[i];
            moi.close();
            if (!moi) throw runtime_error("Không ghi được dữ liệu. Kiểm tra quyền ghi thư mục DuLieu.");
            DWORD thuocTinh = GetFileAttributesA(tep.c_str());
            tepCuTonTai[i] = thuocTinh != INVALID_FILE_ATTRIBUTES;
            if (tepCuTonTai[i] && !CopyFileA(tep.c_str(), (tep + ".gui-backup").c_str(), FALSE)) {
                throw runtime_error("Không sao lưu được dữ liệu trước khi lưu.");
            }
        }
        for (const string& ten : tenTep) {
            string tep = "DuLieu/" + ten;
            if (!MoveFileExA((tep + ".gui-new").c_str(), tep.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
                throw runtime_error("Không thay thế được tệp dữ liệu. Thao tác đã được hủy.");
            }
            daThay++;
        }
    } catch (...) {
        for (int i = 0; i < daThay; i++) {
            string tep = "DuLieu/" + tenTep[i];
            if (tepCuTonTai[i]) MoveFileExA((tep + ".gui-backup").c_str(), tep.c_str(), MOVEFILE_REPLACE_EXISTING);
            else DeleteFileA(tep.c_str());
        }
        duLieu.docDuLieu();
        docDonDat();
        if (kemTaiKhoan) duLieu.quanLyTaiKhoan.docDuLieu();
        throw;
    }
    for (const string& ten : tenTep) DeleteFileA(("DuLieu/" + ten + ".gui-backup").c_str());
}

void DichVuNhaHang::luuKhachHang(const KhachHang& khach, bool themMoi) {
    yeuCauNhanSu();
    kiemTraChuoi(khach.getMa(), "Mã khách hàng", true);
    kiemTraChuoi(khach.getTen(), "Họ tên");
    kiemTraChuoi(khach.getSoDienThoai(), "Điện thoại");
    kiemTraChuoi(khach.getGioiTinh(), "Giới tính");
    kiemTraSo(khach.getDiemTichLuy(), "Điểm tích lũy");
    KhachHang* cu = duLieu.timKhach(khach.getMa());
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
    kiemTraChuoi(nv.getSoDienThoai(), "Điện thoại");
    kiemTraChuoi(nv.getGioiTinh(), "Giới tính");
    kiemTraChuoi(nv.getChucVu(), "Chức vụ");
    kiemTraSo(nv.getLuongCoBan(), "Lương");
    kiemTraSo(nv.getPhuCap(), "Phụ cấp");
    NhanVien* cu = duLieu.timNhanVien(nv.getMa());
    if (themMoi && cu) throw runtime_error("Mã nhân viên đã tồn tại.");
    if (!themMoi && !cu) throw runtime_error("Không tìm thấy nhân viên cần sửa.");
    if (cu) *cu = nv;
    else duLieu.dsNhanVien.push_back(nv);
    luuThayDoi();
}
void DichVuNhaHang::luuMon(const DuLieuMon& mon, bool themMoi) {
    yeuCauQuanLy();
    kiemTraChuoi(mon.ma, "Mã món", true);
    kiemTraChuoi(mon.ten, "Tên món");
    kiemTraSo(mon.gia, "Giá");
    kiemTraSo(mon.tonKho, "Tồn kho");
    unique_ptr<Mon> moi;
    if (mon.loai == "Mon an") {
        if (mon.doCay < 0 || mon.doCay > 5) throw runtime_error("Độ cay phải từ 0 đến 5.");
        moi = make_unique<MonAn>(mon.ma, mon.ten, mon.gia, mon.tonKho, mon.doCay, mon.monChay);
    } else if (mon.loai == "Nuoc uong") {
        if (mon.sizeLy != "S" && mon.sizeLy != "M" && mon.sizeLy != "L") throw runtime_error("Size phải là S, M hoặc L.");
        moi = make_unique<NuocUong>(mon.ma, mon.ten, mon.gia, mon.tonKho, mon.sizeLy, mon.coDa);
    } else throw runtime_error("Loại món không hợp lệ.");
    bool timThay = false;
    for (unique_ptr<Mon>& cu : duLieu.dsMon) {
        if (cu->getMa() == mon.ma) {
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
}
void DichVuNhaHang::xoaKhachHang(const string& ma) {
    yeuCauNhanSu();
    for (size_t i = 0; i < duLieu.dsKhachHang.size(); i++) {
        if (duLieu.dsKhachHang[i].getMa() == ma) {
            duLieu.dsKhachHang.erase(duLieu.dsKhachHang.begin() + i);
            luuThayDoi(); return;
        }
    }
    throw runtime_error("Không tìm thấy khách hàng.");
}
void DichVuNhaHang::xoaNhanVien(const string& ma) {
    yeuCauQuanLy();
    for (size_t i = 0; i < duLieu.dsNhanVien.size(); i++) {
        if (duLieu.dsNhanVien[i].getMa() == ma) {
            duLieu.dsNhanVien.erase(duLieu.dsNhanVien.begin() + i);
            luuThayDoi(); return;
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
    string nguoiBan = phien().maNhanVien;
    if (!duLieu.timNhanVien(nguoiBan)) throw runtime_error("Cần chọn nhân viên bán hàng hợp lệ.");
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
            string da = "Khong da";
            if (muc.coDa) da = "Co da";
            ten += " (Size " + muc.sizeLy + ", " + da + ")";
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
    for (const KhachHang& khach : duLieu.dsKhachHang) {
        if (khach.getMa() == phien().maKhachHang) return khach;
    }
    throw runtime_error("Hồ sơ khách hàng không còn tồn tại.");
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
    if (laKhachHang()) hoSoCuaToi();
    for (const HoaDon& hoaDon : duLieu.dsHoaDon) {
        if (hoaDon.getMa() != maHoaDon) continue;
        if (laQuanLy() || hoaDon.getMaKhachHang() == phien().maKhachHang) return hoaDon;
        break;
    }
    throw runtime_error("Không tìm thấy hóa đơn hoặc bạn không có quyền xem hóa đơn này.");
}

// Dang ky cong khai chi tao ho so MOI va quyen KHACHHANG.
// Ma khach, diem va vai tro khong lay tu bieu mau.
string DichVuNhaHang::dangKyKhachHang(const string& hoTen, const string& dienThoai,
                                     const string& gioiTinh, const string& tenDangNhap,
                                     const string& matKhau, const string& xacNhan) {
    if (canThietLap || duLieu.quanLyTaiKhoan.danhSach.empty()) throw runtime_error("Nhà hàng cần tạo tài khoản quản lý đầu tiên trước.");
    if (daDangNhap()) throw runtime_error("Hãy đăng xuất trước khi đăng ký tài khoản mới.");
    if (matKhau != xacNhan) throw runtime_error("Hai mật khẩu không trùng nhau.");
    if (tenDangNhap == "0") throw runtime_error("Tên đăng nhập không được là 0.");
    size_t dau = hoTen.find_first_not_of(" \t");
    size_t cuoi = hoTen.find_last_not_of(" \t");
    string tenSach;
    if (dau != string::npos) tenSach = hoTen.substr(dau, cuoi-dau+1);
    kiemTraChuoi(tenSach, "Họ và tên");
    if (dienThoai.size() < 9 || dienThoai.size() > 15) throw runtime_error("Số điện thoại cần có từ 9 đến 15 chữ số.");
    for (char c : dienThoai) {
        if (c < '0' || c > '9') throw runtime_error("Số điện thoại chỉ gồm chữ số, không có khoảng trắng.");
    }
    if (gioiTinh != "Nam" && gioiTinh != "Nữ" && gioiTinh != "Nu" && gioiTinh != "Khác") {
        throw runtime_error("Hãy chọn giới tính hợp lệ.");
    }
    for (const KhachHang& khach : duLieu.dsKhachHang) {
        string soCu;
        for (char c : khach.getSoDienThoai()) if (c >= '0' && c <= '9') soCu += c;
        if (soCu == dienThoai) {
            throw runtime_error("Số điện thoại đã có hồ sơ. Liên hệ nhân viên để được cấp hoặc hỗ trợ tài khoản.");
        }
    }
    string maMoi;
    for (int i = 1; i < 1000000000; i++) {
        ostringstream ma;
        ma << "KH" << setfill('0') << setw(4) << i;
        string ungVien = ma.str();
        bool daDung = duLieu.timKhach(ungVien) != nullptr;
        // Khong tai su dung ma cua khach da xoa, tranh lo hoa don cu.
        for (const HoaDon& hd : duLieu.dsHoaDon) {
            if (hd.getMaKhachHang() == ungVien) { daDung = true; break; }
        }
        for (const TaiKhoan& tk : duLieu.quanLyTaiKhoan.danhSach) {
            if (tk.maKhachHang == ungVien) { daDung = true; break; }
        }
        for (const DonDatMon& don : cacDonDat) {
            if (don.duTinh.getMaKhachHang() == ungVien) { daDung = true; break; }
        }
        if (!daDung) { maMoi = ungVien; break; }
    }
    if (maMoi.empty()) throw runtime_error("Không tạo được mã khách hàng mới.");
    TaiKhoan taiKhoan = duLieu.quanLyTaiKhoan.chuanBi(tenDangNhap, matKhau, "KHACHHANG", maMoi);
    vector<KhachHang> khachCu = duLieu.dsKhachHang;
    vector<TaiKhoan> taiKhoanCu = duLieu.quanLyTaiKhoan.danhSach;
    try {
        duLieu.dsKhachHang.push_back(KhachHang(maMoi, tenSach, dienThoai, gioiTinh, 0));
        duLieu.quanLyTaiKhoan.danhSach.push_back(taiKhoan);
        luuThayDoi(true);
    } catch (...) {
        duLieu.dsKhachHang = khachCu;
        duLieu.quanLyTaiKhoan.danhSach = taiKhoanCu;
        throw;
    }
    return maMoi;
}
