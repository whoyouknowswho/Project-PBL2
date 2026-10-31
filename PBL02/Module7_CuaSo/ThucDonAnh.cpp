#include "CuaSo.h"
#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"
#include <algorithm>

using namespace std;

namespace {
wstring chuTim(const string& chu) {
    wstring k=rong(chu);
    if(!k.empty()) CharLowerBuffW(&k[0],static_cast<DWORD>(k.size()));
    return k;
}
void veChuMon(HDC dc,const wstring& chu,RECT r,HFONT font,COLORREF mau) {
    SelectObject(dc,font); SetTextColor(dc,mau); SetBkMode(dc,TRANSPARENT);
    DrawTextW(dc,chu.c_str(),-1,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
}
}

int CuaSoNhaHang::monThucDonDaChon() const {
    int dong=static_cast<int>(SendMessageW(o(DANH_SACH_THUCDON),LB_GETCURSEL,0,0));
    if(dong==LB_ERR) return -1;
    return static_cast<int>(SendMessageW(o(DANH_SACH_THUCDON),LB_GETITEMDATA,dong,0));
}

void CuaSoNhaHang::napThucDonAnh() {
    HWND ds=o(DANH_SACH_THUCDON);
    if(!ds) return;
    bool cu=dangVeBang; dangVeBang=true;
    SendMessageW(ds,WM_SETREDRAW,FALSE,0);
    SendMessageW(ds,LB_RESETCONTENT,0,0);
    wstring tim=chuTim(duLieu(TIM_KIEM));
    vector<int> ketQua;
    const auto& menu=dichVu.thucDon();
    int soThucAn=0, soNuoc=0;
    for(size_t i=0;i<menu.size();i++) {
        const Mon& mon=*menu[i];
        bool laNuoc=mon.getLoai()=="Nuoc uong";
        if(laNuoc) soNuoc++; else soThucAn++;
        if(laNuoc!=(nhomThucDon==1)) continue;
        if(chuTim(mon.getMa()+" "+mon.getTen()).find(tim)==wstring::npos) continue;
        if(daDanhDau(CON_HANG) && mon.getTonKho()==0) continue;
        ketQua.push_back(static_cast<int>(i));
    }
    int cach=luaChon(SAP_XEP);
    if(cach>0) stable_sort(ketQua.begin(),ketQua.end(),[&](int a,int b) {
        if(cach==1) return chuTim(menu[a]->getTen())<chuTim(menu[b]->getTen());
        if(cach==2) return menu[a]->getGia()<menu[b]->getGia();
        return menu[a]->getGia()>menu[b]->getGia();
    });
    int chon=-1;
    for(int i:ketQua) {
        wstring ten=rong(menu[i]->getTen());
        int dong=static_cast<int>(SendMessageW(ds,LB_ADDSTRING,0,reinterpret_cast<LPARAM>(ten.c_str())));
        SendMessageW(ds,LB_SETITEMDATA,dong,i);
        if(menu[i]->getMa()==maDangSua) chon=dong;
    }
    if(ketQua.empty()) {
        int dong=static_cast<int>(SendMessageW(ds,LB_ADDSTRING,0,reinterpret_cast<LPARAM>(L"Không có món phù hợp")));
        SendMessageW(ds,LB_SETITEMDATA,dong,-1);
    }
    gan(NUT_NHOM_THUCAN,L"Thức ăn ("+to_wstring(soThucAn)+L")");
    gan(NUT_NHOM_NUOC,L"Nước uống ("+to_wstring(soNuoc)+L")");
    kieuNut[o(NUT_NHOM_THUCAN)]=nhomThucDon==0?1:0;
    kieuNut[o(NUT_NHOM_NUOC)]=nhomThucDon==1?1:0;
    InvalidateRect(o(NUT_NHOM_THUCAN),nullptr,TRUE);
    InvalidateRect(o(NUT_NHOM_NUOC),nullptr,TRUE);
    SendMessageW(ds,LB_SETCURSEL,chon,0);
    SendMessageW(ds,WM_SETREDRAW,TRUE,0);
    InvalidateRect(ds,nullptr,TRUE);
    dangVeBang=cu;
    thanhCong(to_wstring(ketQua.size())+L" món · Ảnh minh họa · "+
        (dichVu.laQuanLy()?L"Chọn món để chỉnh sửa thông tin.":L"Chọn Thức ăn / Nước uống hoặc sắp xếp theo giá."));
}

void CuaSoNhaHang::veMonThucDon(const DRAWITEMSTRUCT* dong) {
    if(dong->itemID==static_cast<UINT>(-1)) return;
    HDC dc=dong->hDC; RECT r=dong->rcItem;
    FillRect(dc,&r,nen);
    int i=static_cast<int>(dong->itemData);
    if(i<0 || static_cast<size_t>(i)>=dichVu.thucDon().size()) {
        veChuMon(dc,L"Không có món phù hợp. Hãy thử tìm kiếm hoặc đổi bộ lọc.",r,fontChu,RGB(108,122,144));
        return;
    }
    const Mon& mon=*dichVu.thucDon()[i];
    bool chon=(dong->itemState&ODS_SELECTED)!=0;
    RECT the=r; InflateRect(&the,-3,-5);
    HBRUSH mau=CreateSolidBrush(chon?RGB(237,244,255):RGB(255,255,255));
    HPEN vien=CreatePen(PS_SOLID,chon?2:1,chon?RGB(40,104,222):RGB(225,232,242));
    HGDIOBJ cuMau=SelectObject(dc,mau), cuVien=SelectObject(dc,vien);
    RoundRect(dc,the.left,the.top,the.right,the.bottom,16,16);
    SelectObject(dc,cuMau); SelectObject(dc,cuVien); DeleteObject(mau); DeleteObject(vien);
    veAnhMon(dc,mon,{the.left+12,the.top+12,the.left+138,the.bottom-12});
    int x=the.left+156, phai=the.right-16;
    veChuMon(dc,rong(mon.getTen()),{x,the.top+12,phai,the.top+42},fontDam,RGB(26,40,64));
    veChuMon(dc,tien(mon.getGia()),{x,the.top+46,phai,the.top+74},fontDam,RGB(33,94,192));
    wstring chiTiet;
    const MonAn* thucAn=dynamic_cast<const MonAn*>(&mon);
    if(thucAn) chiTiet=L"Độ cay: "+to_wstring(thucAn->getDoCay())+L"/5 · "+(thucAn->laMonChay()?L"Món chay":L"Món mặn");
    else {
        const NuocUong& nuoc=dynamic_cast<const NuocUong&>(mon);
        chiTiet=L"Size "+rong(nuoc.getSizeLy());
    }
    veChuMon(dc,chiTiet,{x,the.top+78,phai,the.top+102},fontNho,RGB(88,105,128));
    wstring loai=thucAn?L"Thức ăn":L"Nước uống";
    wstring ghiChu=loai+L" · "+rong(mon.getMa());
    if(dichVu.laQuanLy()) ghiChu+=L" · Tồn kho: "+to_wstring(mon.getTonKho());
    else if(mon.getTonKho()==0) ghiChu+=L" · Hết hàng";
    veChuMon(dc,ghiChu,{x,the.top+108,phai,the.top+134},fontNho,
        mon.getTonKho()==0?RGB(185,44,44):RGB(108,122,144));
    if(dong->itemState&ODS_FOCUS) { RECT r=the; InflateRect(&r,-5,-5); DrawFocusRect(dc,&r); }
}
