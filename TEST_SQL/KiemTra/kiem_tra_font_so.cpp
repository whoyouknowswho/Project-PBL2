// Kiem tra font so lieu lon (BoFont::so) dung chu so cung chieu cao (lining figures):
// chu so kieu "old-style" (vd. Georgia) lam "0" thap nhu chu "o", so tien kho doc.
//   g++ -std=c++14 KiemTra/kiem_tra_font_so.cpp Module7_CuaSo/ChuDe.cpp -lgdiplus -lgdi32 -o %TEMP%\fs.exe && %TEMP%\fs.exe
#include "../Module7_CuaSo/ChuDe.h"
#include <cstdio>
#include <cstdlib>

int caoChuSo(HDC dc, wchar_t c) {
    GLYPHMETRICS gm = {};
    MAT2 m = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
    if (GetGlyphOutlineW(dc, c, GGO_METRICS, &gm, 0, nullptr, &m) == GDI_ERROR) return -1;
    return static_cast<int>(gm.gmBlackBoxY);
}

int main() {
    BoFont f = taoBoFont();
    HDC dc = CreateCompatibleDC(nullptr);
    SelectObject(dc, f.so);
    int thapNhat = 1000, caoNhat = 0;
    for (wchar_t c = L'0'; c <= L'9'; c++) {
        int h = caoChuSo(dc, c);
        if (h < thapNhat) thapNhat = h;
        if (h > caoNhat) caoNhat = h;
    }
    DeleteDC(dc);
    xoaBoFont(f);
    bool dat = thapNhat > 0 && caoNhat - thapNhat <= 1;
    std::printf("Chieu cao chu so 0-9 cua font so lieu: %d..%d px -> %s\n", thapNhat, caoNhat,
                dat ? "DAT (0 loi)" : "THAT BAI (1 loi)");
    return dat ? 0 : 1;
}
