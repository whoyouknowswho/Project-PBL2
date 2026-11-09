#ifndef TIENICH_NHAHANG_H
#define TIENICH_NHAHANG_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

// Cac ham thuan dung chung cho giao dien; khong phu thuoc cua so hay du lieu.
wstring rong(const string& utf8);
string hep(const wstring& unicode);
wstring so(double giaTri);
wstring tien(double soTien);
string homNay();
string maHoaDonMoi();

bool khopTuKhoa(const string& noiDung, const wstring& tuKhoa);
wstring tenHangVip(const string& hang);
wstring tenTrangThaiDon(const string& trangThai);
wstring tenGioiTinh(const string& gioiTinh);
int oAnhMinhHoa(const string& loai, const string& tenMon);
bool maAnhHopLe(const string& ma);

struct TrucBieuDo {
    double tran = 1, donVi = 1;
    wstring tenDonVi;
};
TrucBieuDo tinhTrucBieuDo(double lonNhat);
wstring nhanTruc(double giaTri);
wstring kyBaoCao(const string& phamVi);

#endif
