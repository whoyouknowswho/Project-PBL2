#ifndef LOGO_NHA_HANG_H
#define LOGO_NHA_HANG_H

#include <windows.h>

// Anh PNG goc duoc nhung trong exe, khong phu thuoc duong dan ben ngoai.
const int LOGO_BACH_KHOA = 101;
const int LOGO_CNTT = 102;
HBITMAP taiLogo(int maTaiNguyen);
void veLogo(HDC dc, HBITMAP logo, RECT vung);

#endif
