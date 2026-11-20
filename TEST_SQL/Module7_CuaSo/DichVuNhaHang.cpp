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

void kiemTraKhoang(double so, double nhoNhat, double lonNhat, const string& ten, const string& khoang) {
    if (!isfinite(so) || so < nhoNhat || so > lonNhat)
        throw runtime_error(ten + " phải nằm trong khoảng " + khoang + ".");
}

string catKhoang(const string& s) {
    size_t dau = s.find_first_not_of(" \t");
    if (dau == string::npos) return "";
    size_t cuoi = s.find_last_not_of(" \t");
    return s.substr(dau, cuoi - dau + 1);
}

// So ky tu Unicode cua chuoi UTF-8 (khong dem cac byte noi tiep 10xxxxxx).
size_t soKyTu(const string& s) {
    size_t dem = 0;
    for (unsigned char c : s) if ((c & 0xC0) != 0x80) dem++;
    return dem;
}

string chuThuongAscii(string s) {
    for (char& c : s) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return s;
}

// So dien thoai Viet Nam: dung 10 chu so, bat dau bang 0.
bool dienThoaiHopLe(const string& dienThoai) {
    return dienThoai.size() == 10 && dienThoai[0] == '0' &&
        all_of(dienThoai.begin(), dienThoai.end(), [](unsigned char c) { return c >= '0' && c <= '9'; });
}

// Email dang ten@mien.duoi: mot ky tu @, phan ten gom chu/so . _ % + -,
// ten mien co it nhat mot dau cham, duoi tu 2 chu cai.
bool emailHopLe(const string& email) {
    if (email.size() < 6 || email.size() > 100) return false;
    size_t a = email.find('@');
    if (a == string::npos || a == 0 || email.find('@', a + 1) != string::npos) return false;
    string ten = email.substr(0, a), mien = email.substr(a + 1);
    if (ten.front() == '.' || ten.back() == '.' || ten.find("..") != string::npos) return false;
    for (unsigned char c : ten) {
        if (!isalnum(c) && c != '.' && c != '_' && c != '%' && c != '+' && c != '-') return false;
    }
    size_t cham = mien.rfind('.');
    if (cham == string::npos || cham == 0 || mien.size() - cham - 1 < 2) return false;
    if (mien.front() == '-' || mien.find("..") != string::npos || mien.find(".-") != string::npos ||
        mien.find("-.") != string::npos) return false;
    for (unsigned char c : mien) {
        if (!isalnum(c) && c != '.' && c != '-') return false;
    }
    for (size_t i = cham + 1; i < mien.size(); i++) {
        if (!isalpha(static_cast<unsigned char>(mien[i]))) return false;
    }
    return true;
}

// Ho ten: 2-50 ky tu, chi gom chu cai (co dau) va khoang trang.
void kiemTraHoTen(const string& hoTen, const string& ten = "Họ và tên") {
    kiemTraChuoi(hoTen, ten);
    size_t n = soKyTu(hoTen);
    if (n < 2 || n > 50) throw runtime_error(ten + " cần từ 2 đến 50 ký tự.");
    for (unsigned char c : hoTen) {
        if (c < 0x80 && !isalpha(c) && c != ' ')
            throw runtime_error(ten + " chỉ gồm chữ cái và khoảng trắng (không chứa số hay ký hiệu).");
    }
    if (hoTen.find("  ") != string::npos) throw runtime_error(ten + " không có hai khoảng trắng liền nhau.");
}

void kiemTraGioiTinh(const string& gioiTinh) {
    if (gioiTinh != "Nam" && gioiTinh != "Nữ" && gioiTinh != "Nu" && gioiTinh != "Khác")
        throw runtime_error("Hãy chọn giới tính hợp lệ (Nam, Nữ hoặc Khác).");
}

void kiemTraChucVu(const string& chucVu) {
    kiemTraChuoi(chucVu, "Chức vụ");
    size_t n = soKyTu(chucVu);
    if (n < 2 || n > 50) throw runtime_error("Chức vụ cần từ 2 đến 50 ký tự.");
    for (unsigned char c : chucVu) {
        if (c >= '0' && c <= '9') throw runtime_error("Chức vụ không được chứa chữ số.");
    }
}

void kiemTraLuong(double luong, double phuCap) {
    kiemTraKhoang(luong, 1000000, 100000000, "Lương cơ bản", "1.000.000 đến 100.000.000 đ");
    kiemTraKhoang(phuCap, 0, 20000000, "Phụ cấp", "0 đến 20.000.000 đ");
}

// So sanh ngay dd/mm/yyyy theo thu tu yyyymmdd.
long long khoaNgay(const string& ngay) {
    return stoll(ngay.substr(6)) * 10000 + stoll(ngay.substr(3, 2)) * 100 + stoll(ngay.substr(0, 2));
}

// Kiem tra day du ho so khach hang; boQuaMa la ma cua chinh ho so dang sua.
KhachHang chuanHoaKhach(const KhachHang& k, const vector<KhachHang>& ds, const string& boQuaMa,
                        bool kiemTraDinhDangSdt) {
    KhachHang sach(k.getMa(), catKhoang(k.getTen()), catKhoang(k.getSoDienThoai()), k.getGioiTinh(),
                   k.getDiemTichLuy(), catKhoang(k.getEmail()));
    kiemTraHoTen(sach.getTen());
    kiemTraChuoi(sach.getSoDienThoai(), "Điện thoại");
    if (kiemTraDinhDangSdt && !dienThoaiHopLe(sach.getSoDienThoai()))
        throw runtime_error("Số điện thoại phải gồm đúng 10 chữ số và bắt đầu bằng 0.");
    if (sach.getEmail().empty()) throw runtime_error("Email không được để trống.");
    if (!emailHopLe(sach.getEmail())) throw runtime_error("Email không hợp lệ (ví dụ đúng: ten@gmail.com).");
    kiemTraGioiTinh(sach.getGioiTinh());
    kiemTraSo(sach.getDiemTichLuy(), "Điểm tích lũy");
    for (const KhachHang& khac : ds) {
        if (khac.getMa() == boQuaMa) continue;
        if (khac.getSoDienThoai() == sach.getSoDienThoai())
            throw runtime_error("Số điện thoại đã được khách hàng " + khac.getMa() + " sử dụng.");
        if (chuThuongAscii(khac.getEmail()) == chuThuongAscii(sach.getEmail()))
            throw runtime_error("Email đã được khách hàng " + khac.getMa() + " sử dụng.");
    }
    return sach;
}

NhanVien chuanHoaNhanVien(const NhanVien& n, const vector<NhanVien>& ds, const string& boQuaMa,
                          bool kiemTraDinhDangSdt) {
    NhanVien sach(n.getMa(), catKhoang(n.getTen()), catKhoang(n.getSoDienThoai()), n.getGioiTinh(),
                  catKhoang(n.getChucVu()), n.getLuongCoBan(), n.getPhuCap());
    kiemTraHoTen(sach.getTen());
    kiemTraChuoi(sach.getSoDienThoai(), "Điện thoại");
    if (kiemTraDinhDangSdt && !dienThoaiHopLe(sach.getSoDienThoai()))
        throw runtime_error("Số điện thoại phải gồm đúng 10 chữ số và bắt đầu bằng 0.");
    kiemTraGioiTinh(sach.getGioiTinh());
    kiemTraChucVu(sach.getChucVu());
    kiemTraLuong(sach.getLuongCoBan(), sach.getPhuCap());
    for (const NhanVien& khac : ds) {
        if (khac.getMa() != boQuaMa && khac.getSoDienThoai() == sach.getSoDienThoai())
            throw runtime_error("Số điện thoại đã được nhân viên " + khac.getMa() + " sử dụng.");
    }
    return sach;
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

string luuAnhVaoThuMucDuLieu(const string& duongDan) {
    if (duongDan.empty()) return "";
    wstring nguon = rong(duongDan);
    size_t viTri = nguon.find_last_of(L"\\/");
    wstring tenTep = (viTri == wstring::npos) ? nguon : nguon.substr(viTri + 1);
    if (tenTep.empty()) return duongDan;
    CreateDirectoryW(L"DuLieu", nullptr);
    CreateDirectoryW(L"DuLieu\\AnhMon", nullptr);
    wstring dich = L"DuLieu\\AnhMon\\" + tenTep;
    if (GetFileAttributesW(nguon.c_str()) != INVALID_FILE_ATTRIBUTES) {
        wchar_t dayDuNguon[MAX_PATH] = {}, dayDuDich[MAX_PATH] = {};
        GetFullPathNameW(nguon.c_str(), MAX_PATH, dayDuNguon, nullptr);
        GetFullPathNameW(dich.c_str(), MAX_PATH, dayDuDich, nullptr);
        if (_wcsicmp(dayDuNguon, dayDuDich) != 0) {
            CopyFileW(nguon.c_str(), dich.c_str(), FALSE);
        }
    }
    if (GetFileAttributesW(dich.c_str()) != INVALID_FILE_ATTRIBUTES) {
        return "DuLieu/AnhMon/" + hep(tenTep);
    }
    return duongDan;
}
}

bool DichVuNhaHang::khoiTao() {
    docDuLieuSql();
    docDonDat();
    canThietLap = !duLieu.quanLyTaiKhoan.docDuLieu();
    return canThietLap;
}

// Ngay lap hoa don / dat mon phai ton tai tren lich va khong sau hom nay.
bool DichVuNhaHang::ngayDatHopLe(const string& ngay) const {
    return ngayHopLe(ngay) && khoaNgay(ngay) <= khoaNgay(homNay());
}

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
    if (moi == cu) throw runtime_error("Mật khẩu mới phải khác mật khẩu hiện tại.");
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
    KhachHang* cu = duLieu.timKhach(khach.getMa());
    if (themMoi && cu != nullptr) throw runtime_error("Mã khách hàng đã tồn tại.");
    if (!themMoi && cu == nullptr) throw runtime_error("Không tìm thấy khách hàng cần sửa.");
    // So dien thoai cu (du lieu truoc day) duoc giu nguyen neu nguoi dung khong sua.
    bool doiSdt = !cu || cu->getSoDienThoai() != catKhoang(khach.getSoDienThoai());
    KhachHang sach = chuanHoaKhach(khach, duLieu.dsKhachHang, khach.getMa(), doiSdt);
    if (cu) *cu = sach;
    else duLieu.dsKhachHang.push_back(sach);
    luuThayDoi();
}
void DichVuNhaHang::luuNhanVien(const NhanVien& nv, bool themMoi) {
    yeuCauQuanLy();
    kiemTraChuoi(nv.getMa(), "Mã nhân viên", true);
    NhanVien* cu = duLieu.timNhanVien(nv.getMa());
    if (themMoi && cu) throw runtime_error("Mã nhân viên đã tồn tại.");
    if (!themMoi && !cu) throw runtime_error("Không tìm thấy nhân viên cần sửa.");
    bool doiSdt = !cu || cu->getSoDienThoai() != catKhoang(nv.getSoDienThoai());
    NhanVien sach = chuanHoaNhanVien(nv, duLieu.dsNhanVien, nv.getMa(), doiSdt);
    if (cu) *cu = sach;
    else duLieu.dsNhanVien.push_back(sach);
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
    string tenMon = catKhoang(mon.ten);
    kiemTraChuoi(maLuu, "Mã món", true);
    kiemTraChuoi(tenMon, "Tên món");
    if (soKyTu(tenMon) < 2 || soKyTu(tenMon) > 100) throw runtime_error("Tên món cần từ 2 đến 100 ký tự.");
    kiemTraKhoang(mon.gia, 1000, 10000000, "Giá", "1.000 đến 10.000.000 đ");
    if (floor(mon.gia) != mon.gia) throw runtime_error("Giá phải là số nguyên (đơn vị đồng).");
    kiemTraKhoang(mon.tonKho, 0, 100000, "Tồn kho", "0 đến 100.000");
    if (mon.hinhAnh.size() > 500) throw runtime_error("Đường dẫn hình ảnh quá dài (tối đa 500 ký tự).");
    if (mon.loai != "Mon an" && mon.loai != "Nuoc uong") throw runtime_error("Loại món không hợp lệ.");
    for (const auto& khac : duLieu.dsMon) {
        if (khac->getMa() != maLuu && khac->getLoai() == mon.loai &&
            chuThuongAscii(catKhoang(khac->getTen())) == chuThuongAscii(tenMon))
            throw runtime_error("Tên món đã có trong thực đơn (" + khac->getMa() + ").");
    }
    unique_ptr<Mon> moi;
    if (mon.loai == "Mon an") {
        if (mon.doCay < 0 || mon.doCay > 5) throw runtime_error("Độ cay phải từ 0 đến 5.");
        moi = make_unique<MonAn>(maLuu, tenMon, mon.gia, mon.tonKho, mon.doCay, mon.monChay);
    } else {
        if (mon.sizeLy != "S" && mon.sizeLy != "M" && mon.sizeLy != "L") throw runtime_error("Size phải là S, M hoặc L.");
        moi = make_unique<NuocUong>(maLuu, tenMon, mon.gia, mon.tonKho, mon.sizeLy, mon.coDa);
    }
    moi->setHinhAnh(luuAnhVaoThuMucDuLieu(mon.hinhAnh));
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
    if (soLuong < 1 || soLuong > 100000 - mon->getTonKho())
        throw runtime_error("Số lượng nhập phải từ 1 và tổng tồn kho sau khi nhập không vượt 100.000.");
    mon->congTonKho(soLuong);
    luuThayDoi();
}

HoaDon DichVuNhaHang::xemTruocHoaDon(const string& ma, const string& ngay, const string& khach,
                                    const string& /*nhanVien*/, const vector<YeuCauMon>& gioHang) {
    yeuCauNhanVien();
    kiemTraChuoi(ma, "Mã hóa đơn", true);
    if (!ngayHopLe(ngay)) throw runtime_error("Ngày phải có dạng dd/mm/yyyy và tồn tại trên lịch.");
    if (!ngayDatHopLe(ngay)) throw runtime_error("Ngày lập hóa đơn không được sau hôm nay.");
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

    string maMoi;
    for (int i = 1; i < 1000000; i++) {
        ostringstream ma;
        ma << "NV" << setfill('0') << setw(4) << i;
        string ungVien = ma.str();
        if (duLieu.timNhanVien(ungVien) == nullptr) { maMoi = ungVien; break; }
    }
    NhanVien hoSo = chuanHoaNhanVien(NhanVien(maMoi, hoTen, dienThoai, gioiTinh, chucVu, luong, phuCap),
                                     duLieu.dsNhanVien, maMoi, true);
    TaiKhoan taiKhoan = duLieu.quanLyTaiKhoan.chuanBi(tenDangNhap, matKhau, "NHANVIEN", maMoi);
    vector<NhanVien> nvCu = duLieu.dsNhanVien;
    vector<TaiKhoan> taiKhoanCu = duLieu.quanLyTaiKhoan.danhSach;
    try {
        duLieu.dsNhanVien.push_back(hoSo);
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
    string maMoi;
    for (int i = 1; i < 1000000; i++) {
        ostringstream ma;
        ma << "KH" << setfill('0') << setw(4) << i;
        string ungVien = ma.str();
        if (duLieu.timKhach(ungVien) == nullptr) { maMoi = ungVien; break; }
    }
    KhachHang hoSo = chuanHoaKhach(KhachHang(maMoi, hoTen, dienThoai, gioiTinh, 0, email),
                                   duLieu.dsKhachHang, maMoi, true);
    TaiKhoan taiKhoan = duLieu.quanLyTaiKhoan.chuanBi(tenDangNhap, matKhau, "KHACHHANG", maMoi);
    vector<KhachHang> khachCu = duLieu.dsKhachHang;
    vector<TaiKhoan> taiKhoanCu = duLieu.quanLyTaiKhoan.danhSach;
    try {
        duLieu.dsKhachHang.push_back(hoSo);
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
        if (c.size() >= 8) m->setHinhAnh(luuAnhVaoThuMucDuLieu(c[7]));
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
