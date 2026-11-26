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
    // Anh da thu nho + bo goc san theo dung kich thuoc o the, de cuon/re chuot khong phai noi suy lai.
    // Moi mon chi giu mot ban; kich thuoc doi thi tao lai.
    struct AnhThuNho { int rong = 0, cao = 0, banKinh = 0; Gdiplus::Bitmap* bmp = nullptr; };
    map<string, AnhThuNho> demThuNho;
    void xoaDemThuNho();

public:
    KhoAnh() = default;
    KhoAnh(const KhoAnh&) = delete;
    KhoAnh& operator=(const KhoAnh&) = delete;
    ~KhoAnh();
    void napTaiNguyen();
    void giaiPhong();
    void xoaDemRieng();
    void veLogo(HDC dc, int maTaiNguyen, RECT vung) const;
    void veAnhMon(HDC dc, const string& maMon, const string& loai, const string& tenMon,
                  const string& hinhAnh, RECT vung, int banKinh);
};

#endif
