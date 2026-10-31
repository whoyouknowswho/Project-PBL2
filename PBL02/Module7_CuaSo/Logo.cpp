#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "Logo.h"
#include <objidl.h>
#include <gdiplus.h>
#include <algorithm>
#include <cstring>
#include <stdexcept>

using namespace std;

HBITMAP taiLogo(int maTaiNguyen) {
    HMODULE ungDung = GetModuleHandleW(nullptr);
    HRSRC taiNguyen = FindResourceW(ungDung, MAKEINTRESOURCEW(maTaiNguyen), MAKEINTRESOURCEW(10));
    DWORD doDai = taiNguyen ? SizeofResource(ungDung, taiNguyen) : 0;
    HGLOBAL duLieu = taiNguyen ? LoadResource(ungDung, taiNguyen) : nullptr;
    const void* byteAnh = duLieu ? LockResource(duLieu) : nullptr;
    if (!byteAnh || doDai == 0) throw runtime_error("Không đọc được logo đã nhúng trong ứng dụng.");

    HGLOBAL boNho = GlobalAlloc(GMEM_MOVEABLE, doDai);
    if (!boNho) throw runtime_error("Không đủ bộ nhớ để tải logo.");
    void* dich = GlobalLock(boNho);
    if (!dich) { GlobalFree(boNho); throw runtime_error("Không truy cập được bộ nhớ ảnh."); }
    memcpy(dich, byteAnh, doDai);
    GlobalUnlock(boNho);

    IStream* luong = nullptr;
    if (FAILED(CreateStreamOnHGlobal(boNho, TRUE, &luong))) {
        GlobalFree(boNho);
        throw runtime_error("Không tạo được luồng đọc logo.");
    }
    Gdiplus::GdiplusStartupInput thietLap;
    ULONG_PTR phien = 0;
    if (Gdiplus::GdiplusStartup(&phien, &thietLap, nullptr) != Gdiplus::Ok) {
        luong->Release();
        throw runtime_error("Không khởi tạo được bộ đọc ảnh Windows.");
    }
    HBITMAP logo = nullptr;
    {
        Gdiplus::Bitmap anh(luong);
        if (anh.GetLastStatus() == Gdiplus::Ok) {
            anh.GetHBITMAP(Gdiplus::Color(255,255,255), &logo);
        }
    }
    Gdiplus::GdiplusShutdown(phien);
    luong->Release();
    if (!logo) throw runtime_error("Ảnh logo PNG không hợp lệ.");
    return logo;
}

void veLogo(HDC dc, HBITMAP logo, RECT vung) {
    BITMAP kichThuoc = {};
    if (!logo || !GetObjectW(logo, sizeof(kichThuoc), &kichThuoc)) return;
    // Co vua khung va giu nguyen ti le; khong cat hoac keo gian logo.
    double tiLe = min(static_cast<double>(vung.right-vung.left)/kichThuoc.bmWidth,
                      static_cast<double>(vung.bottom-vung.top)/kichThuoc.bmHeight);
    int w = max(1, static_cast<int>(kichThuoc.bmWidth*tiLe));
    int h = max(1, static_cast<int>(kichThuoc.bmHeight*tiLe));
    int x = vung.left+(vung.right-vung.left-w)/2;
    int y = vung.top+(vung.bottom-vung.top-h)/2;
    HDC anh = CreateCompatibleDC(dc);
    HGDIOBJ cu = SelectObject(anh, logo);
    int cheDoCu = SetStretchBltMode(dc, HALFTONE);
    POINT gocCu; SetBrushOrgEx(dc, 0, 0, &gocCu);
    StretchBlt(dc, x, y, w, h, anh, 0, 0, kichThuoc.bmWidth, kichThuoc.bmHeight, SRCCOPY);
    SetBrushOrgEx(dc, gocCu.x, gocCu.y, nullptr);
    SetStretchBltMode(dc, cheDoCu);
    SelectObject(anh, cu); DeleteDC(anh);
}

HBITMAP taiAnhTep(const wstring& duongDan) {
    if(GetFileAttributesW(duongDan.c_str())==INVALID_FILE_ATTRIBUTES) return nullptr;
    Gdiplus::GdiplusStartupInput thietLap;
    ULONG_PTR phien=0;
    if(Gdiplus::GdiplusStartup(&phien,&thietLap,nullptr)!=Gdiplus::Ok) return nullptr;
    HBITMAP ketQua=nullptr;
    {
        Gdiplus::Bitmap anh(duongDan.c_str());
        if(anh.GetLastStatus()==Gdiplus::Ok)
            anh.GetHBITMAP(Gdiplus::Color(255,255,255),&ketQua);
    }
    Gdiplus::GdiplusShutdown(phien);
    return ketQua;
}

void veAnhCat(HDC dc, HBITMAP anh, RECT nguon, RECT dich) {
    if(!anh || nguon.right<=nguon.left || nguon.bottom<=nguon.top ||
       dich.right<=dich.left || dich.bottom<=dich.top) return;
    int sw=nguon.right-nguon.left, sh=nguon.bottom-nguon.top;
    int dw=dich.right-dich.left, dh=dich.bottom-dich.top;
    double tiLe=max(static_cast<double>(dw)/sw,static_cast<double>(dh)/sh);
    int catW=max(1,static_cast<int>(dw/tiLe)), catH=max(1,static_cast<int>(dh/tiLe));
    int sx=nguon.left+(sw-catW)/2, sy=nguon.top+(sh-catH)/2;
    HDC boNho=CreateCompatibleDC(dc);
    HGDIOBJ cu=SelectObject(boNho,anh);
    int cheDo=SetStretchBltMode(dc,HALFTONE);
    POINT goc; SetBrushOrgEx(dc,0,0,&goc);
    StretchBlt(dc,dich.left,dich.top,dw,dh,boNho,sx,sy,catW,catH,SRCCOPY);
    SetBrushOrgEx(dc,goc.x,goc.y,nullptr);
    SetStretchBltMode(dc,cheDo);
    SelectObject(boNho,cu); DeleteDC(boNho);
}
