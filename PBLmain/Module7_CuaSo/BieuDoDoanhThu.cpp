#include "CuaSo.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

namespace {
struct KhungBieuDo {
    RECT the, truc;
    double tran=1, donVi=1;
    wstring tenDonVi;
    vector<POINT> diem;
};

KhungBieuDo tinhKhung(int rong,int cao,const vector<double>& doanhThu) {
    KhungBieuDo k;
    k.the={264,326,rong-26,cao-54};
    k.truc={k.the.left+100,k.the.top+92,k.the.right-30,k.the.bottom-80};
    double lonNhat=*max_element(doanhThu.begin(),doanhThu.end());
    if(lonNhat<=0) lonNhat=1000000;
    double bac=pow(10,floor(log10(lonNhat/4)));
    k.tran=ceil(lonNhat/4/bac)*bac*4;
    if(k.tran>=1000000000) { k.donVi=1000000000; k.tenDonVi=L"tỷ đồng"; }
    else if(k.tran>=1000000) { k.donVi=1000000; k.tenDonVi=L"triệu đồng"; }
    else if(k.tran>=1000) { k.donVi=1000; k.tenDonVi=L"nghìn đồng"; }
    else k.tenDonVi=L"đồng";
    for(int i=0;i<12;i++) {
        int x=k.truc.left+(k.truc.right-k.truc.left)*i/11;
        int y=k.truc.bottom-static_cast<int>((k.truc.bottom-k.truc.top)*doanhThu[i]/k.tran);
        k.diem.push_back({x,y});
    }
    return k;
}

void chuBieuDo(HDC dc,const wstring& chu,RECT vung,HFONT font,COLORREF mau,UINT can=DT_LEFT) {
    SelectObject(dc,font); SetBkMode(dc,TRANSPARENT); SetTextColor(dc,mau);
    DrawTextW(dc,chu.c_str(),-1,&vung,can|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
}

wstring nhanTruc(double so) {
    wostringstream s;
    s<<fixed<<setprecision(abs(so-round(so))<0.000001?0:2)<<so;
    return s.str();
}
}

void CuaSoNhaHang::trangBieuDoDoanhThu() {
    int namHienTai=stoi(homNay().substr(6));
    if(namBieuDo==0) namBieuDo=namHienTai;
    doanhThuNam=dichVu.doanhThuTheoNam(namBieuDo);
    vector<int> nam=doanhThuNam.cacNam;
    if(find(nam.begin(),nam.end(),namHienTai)==nam.end()) nam.push_back(namHienTai);
    sort(nam.rbegin(),nam.rend());
    vector<wstring> luaChon;
    int chonNam=0;
    for(size_t i=0;i<nam.size();i++) {
        luaChon.push_back(to_wstring(nam[i]));
        if(nam[i]==namBieuDo) chonNam=static_cast<int>(i);
    }
    chon(L"Năm doanh thu",LOC_NAM_DOANHTHU,rongCuaSo-174,250,148,luaChon);
    chonMuc(LOC_NAM_DOANHTHU,chonNam);
    capNhatGoiYDoanhThu();
    thanhCong(L"Doanh thu 12 tháng · Chọn năm để xem lại · Rê chuột lên điểm để xem số tiền.");
}

void CuaSoNhaHang::napBieuDoDoanhThu() {
    int nam=stoi(duLieu(LOC_NAM_DOANHTHU));
    DoanhThuNam moi=dichVu.doanhThuTheoNam(nam);
    namBieuDo=nam; doanhThuNam=moi;
    capNhatGoiYDoanhThu();
    InvalidateRect(cuaSo,nullptr,FALSE);
}

void CuaSoNhaHang::capNhatGoiYDoanhThu() {
    thangDangXem=-1;
}

void CuaSoNhaHang::diChuyenBieuDo(int x,int y) {
    KhungBieuDo k=tinhKhung(rongCuaSo,caoCuaSo,doanhThuNam.cacThang);
    int thang=-1;
    for(int i=0;i<12;i++)
        if(abs(x-k.diem[i].x)<=14 && abs(y-k.diem[i].y)<=14) { thang=i; break; }
    if(thang!=thangDangXem) {
        thangDangXem=thang;
        InvalidateRect(cuaSo,&k.the,FALSE);
    }
    TRACKMOUSEEVENT theoDoi={}; theoDoi.cbSize=sizeof(theoDoi);
    theoDoi.dwFlags=TME_LEAVE; theoDoi.hwndTrack=cuaSo;
    TrackMouseEvent(&theoDoi);
}

void CuaSoNhaHang::veBieuDoDoanhThu(HDC dc) {
    KhungBieuDo k=tinhKhung(rongCuaSo,caoCuaSo,doanhThuNam.cacThang);
    HBRUSH trang=CreateSolidBrush(RGB(255,255,255));
    HPEN vien=CreatePen(PS_SOLID,1,RGB(225,232,242));
    HGDIOBJ cuBrush=SelectObject(dc,trang), cuPen=SelectObject(dc,vien);
    RoundRect(dc,k.the.left,k.the.top,k.the.right,k.the.bottom,16,16);
    SelectObject(dc,cuBrush); SelectObject(dc,cuPen); DeleteObject(trang); DeleteObject(vien);
    chuBieuDo(dc,L"Doanh thu từng tháng",{k.the.left+22,k.the.top+14,k.the.right-320,k.the.top+44},fontDam,RGB(26,40,64));
    chuBieuDo(dc,L"Tổng năm: "+tien(doanhThuNam.tongTien),
        {k.the.right-320,k.the.top+14,k.the.right-22,k.the.top+44},fontDam,RGB(33,94,192),DT_RIGHT);
    chuBieuDo(dc,L"Năm "+to_wstring(namBieuDo)+L" · Doanh thu sau giảm giá thành viên",
        {k.the.left+22,k.the.top+46,k.the.right-22,k.the.top+70},fontNho,RGB(108,122,144));
    chuBieuDo(dc,L"Đơn vị: "+k.tenDonVi,{k.the.left+22,k.the.top+70,k.the.right-22,k.the.top+90},fontNho,RGB(108,122,144));

    HPEN luoi=CreatePen(PS_SOLID,1,RGB(232,237,244));
    cuPen=SelectObject(dc,luoi);
    for(int i=0;i<=4;i++) {
        int y=k.truc.bottom-(k.truc.bottom-k.truc.top)*i/4;
        MoveToEx(dc,k.truc.left,y,nullptr); LineTo(dc,k.truc.right,y);
        chuBieuDo(dc,nhanTruc(k.tran*i/4/k.donVi),
            {k.the.left+14,y-12,k.truc.left-12,y+12},fontNho,RGB(108,122,144),DT_RIGHT);
    }
    SelectObject(dc,cuPen); DeleteObject(luoi);

    vector<POINT> nenVung=k.diem;
    nenVung.push_back({k.truc.right,k.truc.bottom}); nenVung.push_back({k.truc.left,k.truc.bottom});
    HBRUSH xanhNhat=CreateSolidBrush(RGB(238,245,255));
    cuBrush=SelectObject(dc,xanhNhat); cuPen=SelectObject(dc,GetStockObject(NULL_PEN));
    Polygon(dc,nenVung.data(),static_cast<int>(nenVung.size()));
    SelectObject(dc,cuBrush); SelectObject(dc,cuPen); DeleteObject(xanhNhat);
    HPEN duong=CreatePen(PS_SOLID,3,RGB(40,104,222));
    cuPen=SelectObject(dc,duong);
    Polyline(dc,k.diem.data(),static_cast<int>(k.diem.size()));
    HBRUSH diem=CreateSolidBrush(RGB(255,255,255)); cuBrush=SelectObject(dc,diem);
    for(int i=0;i<12;i++) {
        Ellipse(dc,k.diem[i].x-4,k.diem[i].y-4,k.diem[i].x+5,k.diem[i].y+5);
        chuBieuDo(dc,L"T"+to_wstring(i+1),{k.diem[i].x-22,k.truc.bottom+10,k.diem[i].x+22,k.truc.bottom+34},
            fontNho,RGB(108,122,144),DT_CENTER);
    }
    SelectObject(dc,cuBrush); SelectObject(dc,cuPen); DeleteObject(diem); DeleteObject(duong);
    if(doanhThuNam.soHoaDon==0)
        chuBieuDo(dc,L"Chưa có doanh thu trong năm này",{k.truc.left,k.truc.top+20,k.truc.right,k.truc.top+50},
            fontChu,RGB(108,122,144),DT_CENTER);
    if(thangDangXem>=0 && thangDangXem<12) {
        POINT p=k.diem[thangDangXem];
        int x=max(static_cast<int>(k.the.left+14),min(static_cast<int>(p.x-100),static_cast<int>(k.the.right-224)));
        int y=max(static_cast<int>(k.truc.top),static_cast<int>(p.y-76));
        RECT thongTin={x,y,x+210,y+62};
        HBRUSH mau=CreateSolidBrush(RGB(26,47,80));
        HGDIOBJ cu=SelectObject(dc,mau), vienCu=SelectObject(dc,GetStockObject(NULL_PEN));
        RoundRect(dc,thongTin.left,thongTin.top,thongTin.right,thongTin.bottom,12,12);
        SelectObject(dc,cu); SelectObject(dc,vienCu); DeleteObject(mau);
        chuBieuDo(dc,L"Tháng "+to_wstring(thangDangXem+1)+L"/"+to_wstring(namBieuDo),
            {x+12,y+5,x+198,y+27},fontNho,RGB(190,210,237));
        chuBieuDo(dc,tien(doanhThuNam.cacThang[thangDangXem]),
            {x+12,y+28,x+198,y+56},fontDam,RGB(255,255,255));
    }
    wstring ghiChu=to_wstring(doanhThuNam.soHoaDon)+L" hóa đơn đã thanh toán trong năm";
    if(doanhThuNam.soHoaDonBoQua>0)
        ghiChu+=L" · Bỏ qua "+to_wstring(doanhThuNam.soHoaDonBoQua)+L" hóa đơn có ngày hoặc số tiền chưa hợp lệ";
    chuBieuDo(dc,ghiChu,{k.the.left+22,k.the.bottom-36,k.the.right-22,k.the.bottom-12},fontNho,RGB(108,122,144));
}
