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
#include <utility>
#include <vector>

using namespace std;

// Thuat toan QuickSort (Chia de tri - O(N log N)):
template <typename T, typename SoSanh>
void phanHoachQuickSort(vector<T>& a, int trai, int phai, SoSanh nhoHon) {
    if (trai >= phai) return;
    int i = trai, j = phai;
    T chot = a[trai + (phai - trai) / 2];
    while (i <= j) {
        while (nhoHon(a[i], chot)) i++;
        while (nhoHon(chot, a[j])) j--;
        if (i <= j) { swap(a[i++], a[j--]); }
    }
    if (trai < j) phanHoachQuickSort(a, trai, j, nhoHon);
    if (i < phai) phanHoachQuickSort(a, i, phai, nhoHon);
}

template <typename T, typename SoSanh>
void quickSort(vector<T>& a, SoSanh nhoHon) {
    if (a.size() > 1) phanHoachQuickSort(a, 0, static_cast<int>(a.size()) - 1, nhoHon);
}

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
