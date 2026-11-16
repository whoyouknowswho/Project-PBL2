#include "AnhMon.h"
#include <objidl.h>
#include <gdiplus.h>
#include <algorithm>
#include <cstring>
#include <stdexcept>

using namespace std;

namespace {
// Chep anh sang mot Bitmap doc lap de khong giu luong hoac khoa tep goc.
Gdiplus::Bitmap* banSao(Gdiplus::Bitmap& goc) {
    UINT w = goc.GetWidth(), h = goc.GetHeight();
    if (goc.GetLastStatus() != Gdiplus::Ok || w == 0 || h == 0) return nullptr;
    Gdiplus::Bitmap* moi = new Gdiplus::Bitmap(static_cast<INT>(w), static_cast<INT>(h), PixelFormat32bppPARGB);
    if (moi->GetLastStatus() != Gdiplus::Ok) { delete moi; return nullptr; }
    {
        Gdiplus::Graphics g(moi);
        g.DrawImage(&goc, 0, 0, static_cast<INT>(w), static_cast<INT>(h));
    }
    return moi;
}

Gdiplus::Bitmap* taiTuTaiNguyen(int ma) {
    HMODULE ungDung = GetModuleHandleW(nullptr);
    HRSRC taiNguyen = FindResourceW(ungDung, MAKEINTRESOURCEW(ma), MAKEINTRESOURCEW(10));
    DWORD doDai = taiNguyen ? SizeofResource(ungDung, taiNguyen) : 0;
    HGLOBAL duLieu = taiNguyen ? LoadResource(ungDung, taiNguyen) : nullptr;
    const void* byteAnh = duLieu ? LockResource(duLieu) : nullptr;
    if (!byteAnh || doDai == 0) throw runtime_error("Không đọc được ảnh đã nhúng trong ứng dụng.");
    HGLOBAL boNho = GlobalAlloc(GMEM_MOVEABLE, doDai);
    if (!boNho) throw runtime_error("Không đủ bộ nhớ để tải ảnh.");
    void* dich = GlobalLock(boNho);
    if (!dich) { GlobalFree(boNho); throw runtime_error("Không truy cập được bộ nhớ ảnh."); }
    memcpy(dich, byteAnh, doDai);
    GlobalUnlock(boNho);
    IStream* luong = nullptr;
    if (FAILED(CreateStreamOnHGlobal(boNho, TRUE, &luong))) {
        GlobalFree(boNho);
        throw runtime_error("Không tạo được luồng đọc ảnh.");
    }
    Gdiplus::Bitmap* ketQua = nullptr;
    {
        Gdiplus::Bitmap goc(luong);
        ketQua = banSao(goc);
    }
    luong->Release();
    if (!ketQua) throw runtime_error("Ảnh PNG nhúng trong ứng dụng không hợp lệ.");
    return ketQua;
}

Gdiplus::Bitmap* taiTuTep(const wstring& duongDan) {
    if (GetFileAttributesW(duongDan.c_str()) == INVALID_FILE_ATTRIBUTES) return nullptr;
    Gdiplus::Bitmap goc(duongDan.c_str());
    return banSao(goc);
}

void veCat(HDC dc, Gdiplus::Bitmap* anh, Gdiplus::RectF nguon, RECT dich, int banKinh) {
    if (!anh || nguon.Width <= 0 || nguon.Height <= 0 || dich.right <= dich.left || dich.bottom <= dich.top) return;
    float dw = static_cast<float>(dich.right - dich.left), dh = static_cast<float>(dich.bottom - dich.top);
    float tiLe = max(dw / nguon.Width, dh / nguon.Height);
    float catW = dw / tiLe, catH = dh / tiLe;
    float sx = nguon.X + (nguon.Width - catW) / 2, sy = nguon.Y + (nguon.Height - catH) / 2;
    Gdiplus::Graphics g(dc);
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
    Gdiplus::RectF vung(static_cast<float>(dich.left), static_cast<float>(dich.top), dw, dh);
    if (banKinh > 0) {
        float d = min(2.0f * banKinh, min(dw, dh));
        Gdiplus::GraphicsPath duong;
        duong.AddArc(vung.X, vung.Y, d, d, 180, 90);
        duong.AddArc(vung.X + dw - d, vung.Y, d, d, 270, 90);
        duong.AddArc(vung.X + dw - d, vung.Y + dh - d, d, d, 0, 90);
        duong.AddArc(vung.X, vung.Y + dh - d, d, d, 90, 90);
        duong.CloseFigure();
        g.SetClip(&duong);
    }
    Gdiplus::ImageAttributes thuocTinh;
    thuocTinh.SetWrapMode(Gdiplus::WrapModeTileFlipXY);
    g.DrawImage(anh, vung, sx, sy, catW, catH, Gdiplus::UnitPixel, &thuocTinh);
}
}

KhoAnh::~KhoAnh() { giaiPhong(); }

void KhoAnh::napTaiNguyen() {
    giaiPhong();
    bachKhoa = taiTuTaiNguyen(LOGO_BACH_KHOA);
    cntt = taiTuTaiNguyen(LOGO_CNTT);
    minhHoa = taiTuTaiNguyen(ANH_THUC_DON);
}

void KhoAnh::giaiPhong() {
    delete bachKhoa; bachKhoa = nullptr;
    delete cntt; cntt = nullptr;
    delete minhHoa; minhHoa = nullptr;
    xoaDemRieng();
}

void KhoAnh::xoaDemRieng() {
    for (auto& muc : rieng) delete muc.second;
    rieng.clear();
}

void KhoAnh::veLogo(HDC dc, int maTaiNguyen, RECT vung) const {
    Gdiplus::Bitmap* anh = maTaiNguyen == LOGO_BACH_KHOA ? bachKhoa : maTaiNguyen == LOGO_CNTT ? cntt : minhHoa;
    if (!anh) return;
    float w = static_cast<float>(anh->GetWidth()), h = static_cast<float>(anh->GetHeight());
    float vw = static_cast<float>(vung.right - vung.left), vh = static_cast<float>(vung.bottom - vung.top);
    if (w <= 0 || h <= 0 || vw <= 0 || vh <= 0) return;
    float tiLe = min(vw / w, vh / h);
    float rw = w * tiLe, rh = h * tiLe;
    Gdiplus::Graphics g(dc);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
    g.DrawImage(anh, Gdiplus::RectF(vung.left + (vw - rw) / 2, vung.top + (vh - rh) / 2, rw, rh));
}

void KhoAnh::veAnhMon(HDC dc, const string& ma, const string& loai, const string& tenMon,
                      const string& hinhAnh, RECT vung, int banKinh) {
    string khoa = ma + "|" + hinhAnh;
    if (!rieng.count(khoa)) {
        Gdiplus::Bitmap* bmp = !hinhAnh.empty() ? taiTuTep(rong(hinhAnh)) : nullptr;
        if (!bmp && !hinhAnh.empty()) {
            wstring w = rong(hinhAnh);
            size_t p = w.find_last_of(L"\\/");
            if (p != wstring::npos) bmp = taiTuTep(L"DuLieu/AnhMon/" + w.substr(p + 1));
        }
        if (!bmp && maAnhHopLe(ma)) bmp = taiTuTep(L"DuLieu/AnhMon/" + rong(ma) + L".png");
        rieng[khoa] = bmp;
    }
    Gdiplus::Bitmap* anh = rieng[khoa];
    if (anh) {
        veCat(dc, anh, Gdiplus::RectF(0, 0, static_cast<float>(anh->GetWidth()), static_cast<float>(anh->GetHeight())),
              vung, banKinh);
        return;
    }
    if (!minhHoa) return;
    // Anh minh hoa gom 6 o: 3 cot, hang tren cao 47,4%.
    float w = static_cast<float>(minhHoa->GetWidth()), h = static_cast<float>(minhHoa->GetHeight());
    int viTri = oAnhMinhHoa(loai, tenMon), cot = viTri % 3;
    float catDoc = h * 474 / 1000;
    Gdiplus::RectF nguon(cot * w / 3, viTri < 3 ? 0 : catDoc, w / 3, viTri < 3 ? catDoc : h - catDoc);
    veCat(dc, minhHoa, nguon, vung, banKinh);
}
