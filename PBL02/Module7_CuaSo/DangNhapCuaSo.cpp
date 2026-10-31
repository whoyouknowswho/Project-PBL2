#include "CuaSo.h"
#include "Logo.h"
#include <algorithm>

using namespace std;

namespace {
void veChuDangNhap(HDC dc, const wstring& chu, RECT r, HFONT font,
                  COLORREF mau, UINT can = DT_LEFT) {
    HGDIOBJ cu = SelectObject(dc,font);
    SetBkMode(dc,TRANSPARENT); SetTextColor(dc,mau);
    DrawTextW(dc,chu.c_str(),-1,&r,can|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_NOPREFIX);
    SelectObject(dc,cu);
}

void veKhungDangNhap(HDC dc, RECT r, COLORREF mau, int boGoc) {
    HBRUSH brush = CreateSolidBrush(mau);
    HGDIOBJ cuBrush = SelectObject(dc,brush);
    HGDIOBJ cuPen = SelectObject(dc,GetStockObject(NULL_PEN));
    RoundRect(dc,r.left,r.top,r.right,r.bottom,boGoc,boGoc);
    SelectObject(dc,cuBrush); SelectObject(dc,cuPen); DeleteObject(brush);
}
}

void CuaSoNhaHang::veNhanDienDangNhap(HDC dc) {
    int nua = rongCuaSo/2;
    int le = 40;
    if(nua>=600) le=48;
    int cu = SaveDC(dc);
    IntersectClipRect(dc,0,0,nua,caoCuaSo);

    // Nen xanh chuyen sac nhe, duoc ve bang GDI cua Windows.
    for(int i=0;i<64;i++) {
        RECT dai = {0,caoCuaSo*i/64,nua,caoCuaSo*(i+1)/64+1};
        HBRUSH brush = CreateSolidBrush(RGB(15+i*6/64,29+i*12/64,53+i*22/64));
        FillRect(dc,&dai,brush); DeleteObject(brush);
    }
    // Can giua toan bo cum logo va tieu de theo chieu cao cua so.
    int tren=max(60,(caoCuaSo-334)/2);
    int giua=nua/2;
    RECT khung={le,tren,nua-le,tren+224};
    RECT bong={le+3,tren+8,nua-le+3,tren+232};
    veKhungDangNhap(dc,bong,RGB(10,23,45),24);
    veKhungDangNhap(dc,khung,RGB(255,255,255),24);

    // Hai logo cung chieu cao, giu nguyen ti le va anh goc.
    veLogo(dc,logoBachKhoa,{le+20,tren+20,giua-20,tren+154});
    veLogo(dc,logoCntt,{giua+20,tren+20,nua-le-20,tren+154});
    HPEN net=CreatePen(PS_SOLID,1,RGB(228,234,243));
    HGDIOBJ netCu=SelectObject(dc,net);
    MoveToEx(dc,giua,tren+28,nullptr);
    LineTo(dc,giua,tren+198);
    SelectObject(dc,netCu); DeleteObject(net);

    COLORREF mauNhan=RGB(44,62,90);
    veChuDangNhap(dc,L"ĐẠI HỌC BÁCH KHOA",{le+8,tren+166,giua-8,tren+190},fontNho,mauNhan,DT_CENTER);
    veChuDangNhap(dc,L"ĐÀ NẴNG",{le+8,tren+190,giua-8,tren+212},fontNho,mauNhan,DT_CENTER);
    veChuDangNhap(dc,L"KHOA CÔNG NGHỆ",{giua+8,tren+166,nua-le-8,tren+190},fontNho,mauNhan,DT_CENTER);
    veChuDangNhap(dc,L"THÔNG TIN",{giua+8,tren+190,nua-le-8,tren+212},fontNho,mauNhan,DT_CENTER);

    veChuDangNhap(dc,L"Hệ thống quản lý bán hàng",
                   {le,tren+256,nua-le,tren+310},fontLon,RGB(255,255,255),DT_CENTER);
    veKhungDangNhap(dc,{giua-28,tren+330,giua+28,tren+334},RGB(112,170,255),4);
    RestoreDC(dc,cu);
}
