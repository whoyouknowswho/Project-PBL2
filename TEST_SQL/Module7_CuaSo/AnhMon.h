#ifndef ANHMON_NHAHANG_H
#define ANHMON_NHAHANG_H

#include "ChuDe.h"
#include <map>

namespace Gdiplus { class Bitmap; }

// Anh PNG nhung trong exe (UngDung.rc).
const int LOGO_BACH_KHOA = 101;
const int LOGO_CNTT = 102;
const int ANH_THUC_DON = 103;

// Giu logo, anh minh hoa va anh rieng cua tung mon (DuLieu/AnhMon/<ma>.png).
// Phai giai phong truoc khi tat GDI+.
class KhoAnh {
    Gdiplus::Bitmap* bachKhoa = nullptr;
    Gdiplus::Bitmap* cntt = nullptr;
    Gdiplus::Bitmap* minhHoa = nullptr;
    map<string, Gdiplus::Bitmap*> rieng; // nullptr: da thu nap va khong dung duoc

public:
    KhoAnh() = default;
    KhoAnh(const KhoAnh&) = delete;
    KhoAnh& operator=(const KhoAnh&) = delete;
    ~KhoAnh();
    void napTaiNguyen();
    void giaiPhong();
    void veLogo(HDC dc, int maTaiNguyen, RECT vung) const;
    void veAnhMon(HDC dc, const string& maMon, const string& loai, const string& tenMon, RECT vung, int banKinh);
};

#endif
