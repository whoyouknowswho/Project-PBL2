#include "TienIch.h"
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

namespace {
wstring chuThuong(wstring s) {
    if (!s.empty()) CharLowerBuffW(&s[0], static_cast<DWORD>(s.size()));
    return s;
}
bool chua(const wstring& s, const wchar_t* tu) { return s.find(tu) != wstring::npos; }
}

wstring rong(const string& utf8) {
    if (utf8.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    wstring chu(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), &chu[0], n);
    return chu;
}

string hep(const wstring& unicode) {
    if (unicode.empty()) return "";
    int n = WideCharToMultiByte(CP_UTF8, 0, unicode.data(), static_cast<int>(unicode.size()),
                                nullptr, 0, nullptr, nullptr);
    string chu(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, unicode.data(), static_cast<int>(unicode.size()),
                        &chu[0], n, nullptr, nullptr);
    return chu;
}

wstring so(double giaTri) {
    wostringstream luong;
    luong << fixed << setprecision(0) << giaTri;
    return luong.str();
}

wstring tien(double soTien) {
    wstring chu = so(soTien);
    size_t dau = (!chu.empty() && chu[0] == L'-') ? 1 : 0;
    for (int i = static_cast<int>(chu.size()) - 3; i > static_cast<int>(dau); i -= 3) chu.insert(i, L".");
    return chu + L" đ";
}

string homNay() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    ostringstream s;
    s << setfill('0') << setw(2) << t.wDay << '/' << setw(2) << t.wMonth << '/' << setw(4) << t.wYear;
    return s.str();
}

string maHoaDonMoi() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    ostringstream s;
    s << "HD" << t.wYear << setfill('0') << setw(2) << t.wMonth << setw(2) << t.wDay
      << '-' << GetTickCount64();
    return s.str();
}

bool khopTuKhoa(const string& noiDung, const wstring& tuKhoa) {
    return chuThuong(rong(noiDung)).find(chuThuong(tuKhoa)) != wstring::npos;
}

wstring tenHangVip(const string& hang) {
    if (hang == "Bac") return L"Bạc";
    if (hang == "Vang") return L"Vàng";
    if (hang == "Kim Cuong") return L"Kim cương";
    return L"Thường";
}

wstring tenTrangThaiDon(const string& trangThai) {
    if (trangThai == "XONG") return L"Đã thanh toán";
    if (trangThai == "HUY") return L"Đã hủy";
    return L"Chờ xử lý";
}

wstring tenGioiTinh(const string& gioiTinh) {
    if (gioiTinh == "Nu") return L"Nữ";
    return rong(gioiTinh);
}

int oAnhMinhHoa(const string& loai, const string& tenMon) {
    wstring ten = chuThuong(rong(tenMon));
    if (ten.empty()) return 5;
    if (loai == "Mon an") {
        if (chua(ten, L"com ga") || chua(ten, L"cơm gà")) return 0;
        if (chua(ten, L"rau xao") || chua(ten, L"rau xào")) return 1;
        if (chua(ten, L"mi tom") || chua(ten, L"mì tôm")) return 2;
    } else if (loai == "Nuoc uong") {
        if (chua(ten, L"tra dao") || chua(ten, L"trà đào")) return 3;
        if (chua(ten, L"nuoc cam") || chua(ten, L"nước cam")) return 4;
    }
    return 5;
}

bool maAnhHopLe(const string& ma) {
    if (ma.empty()) return false;
    for (char c : ma) {
        bool hopLe = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                     (c >= '0' && c <= '9') || c == '_' || c == '-';
        if (!hopLe) return false;
    }
    return true;
}

TrucBieuDo tinhTrucBieuDo(double lonNhat) {
    TrucBieuDo k;
    if (lonNhat <= 0) lonNhat = 1000000;
    double bac = pow(10, floor(log10(lonNhat / 4)));
    k.tran = ceil(lonNhat / 4 / bac) * bac * 4;
    if (k.tran >= 1000000000) { k.donVi = 1000000000; k.tenDonVi = L"tỷ đồng"; }
    else if (k.tran >= 1000000) { k.donVi = 1000000; k.tenDonVi = L"triệu đồng"; }
    else if (k.tran >= 1000) { k.donVi = 1000; k.tenDonVi = L"nghìn đồng"; }
    else { k.donVi = 1; k.tenDonVi = L"đồng"; }
    return k;
}

wstring nhanTruc(double giaTri) {
    wostringstream s;
    s << fixed << setprecision(fabs(giaTri - round(giaTri)) < 0.000001 ? 0 : 2) << giaTri;
    return s.str();
}

wstring kyBaoCao(const string& phamVi) {
    if (phamVi.size() == 10) return L"Ngày " + rong(phamVi);
    if (phamVi.size() == 7) return L"Tháng " + rong(phamVi);
    return L"Toàn bộ thời gian";
}
