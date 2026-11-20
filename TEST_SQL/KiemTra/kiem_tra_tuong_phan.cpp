// Kiem tra do tuong phan chu/nen cua bang mau (chuan WCAG 2.x).
//   g++ -std=c++14 KiemTra/kiem_tra_tuong_phan.cpp -o %TEMP%\tp.exe && %TEMP%\tp.exe
#include "../Module7_CuaSo/ChuDe.h"
#include <cmath>
#include <cstdio>

namespace {
double kenh(int c) {
    double v = c / 255.0;
    return v <= 0.03928 ? v / 12.92 : std::pow((v + 0.055) / 1.055, 2.4);
}
double doSang(COLORREF c) {
    return 0.2126 * kenh(GetRValue(c)) + 0.7152 * kenh(GetGValue(c)) + 0.0722 * kenh(GetBValue(c));
}
double doTuongPhan(COLORREF a, COLORREF b) {
    double x = doSang(a), y = doSang(b);
    if (x < y) std::swap(x, y);
    return (x + 0.05) / (y + 0.05);
}
int loi = 0;
void can(const char* ten, COLORREF chu, COLORREF nen, double toiThieu) {
    double tp = doTuongPhan(chu, nen);
    bool dat = tp >= toiThieu;
    std::printf("%-34s %5.2f  (>= %.1f) %s\n", ten, tp, toiThieu, dat ? "ok" : "SAI");
    if (!dat) loi++;
}
}

int main() {
    const COLORREF TRANG = RGB(255, 255, 255);
    can("CHU / NEN", Mau::CHU, Mau::NEN, 4.5);
    can("CHU_NHAN / NEN", Mau::CHU_NHAN, Mau::NEN, 4.5);
    can("CHU_MO / NEN", Mau::CHU_MO, Mau::NEN, 4.5);
    can("CHU_MO / THE", Mau::CHU_MO, Mau::THE, 4.5);
    can("CHU_THANH_BEN / THANH_BEN", Mau::CHU_THANH_BEN, Mau::THANH_BEN, 4.5);
    can("PHU_CHU / PHU_NEN", Mau::PHU_CHU, Mau::PHU_NEN, 4.5);
    can("trang / CHINH (chu nut dam)", TRANG, Mau::CHINH, 3.0);
    can("LOI / LOI_NEN", Mau::LOI, Mau::LOI_NEN, 4.5);
    can("TOT / TOT_NEN", Mau::TOT, Mau::TOT_NEN, 4.5);
    can("CANH_BAO / CANH_BAO_NEN", Mau::CANH_BAO, Mau::CANH_BAO_NEN, 3.0);
    can("CHU_TREN_TOI / NEN_TOI", Mau::CHU_TREN_TOI, Mau::NEN_TOI, 7.0);
    can("CHU_TREN_TOI_MO / NEN_TOI", Mau::CHU_TREN_TOI_MO, Mau::NEN_TOI, 4.5);
    std::printf("%s (%d loi)\n", loi ? "THAT BAI" : "DAT", loi);
    return loi != 0;
}
