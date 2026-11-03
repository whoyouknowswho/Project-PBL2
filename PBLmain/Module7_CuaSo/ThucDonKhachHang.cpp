#include "CuaSo.h"
#include "Logo.h"
#include <algorithm>

using namespace std;

namespace {
void viet(HDC dc,const wstring& chu,RECT vung,HFONT font,COLORREF mau) {
    SelectObject(dc,font); SetTextColor(dc,mau); SetBkMode(dc,TRANSPARENT);
    DrawTextW(dc,chu.c_str(),-1,&vung,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
}

int oAnhMinhHoa(const Mon& mon) {
    wstring ten=rong(mon.getTen());
    if(ten.empty()) return 5;
    CharLowerBuffW(&ten[0],static_cast<DWORD>(ten.size()));
    if(mon.getLoai()=="Mon an") {
        if(ten.find(L"com ga")!=wstring::npos || ten.find(L"cơm gà")!=wstring::npos) return 0;
        if(ten.find(L"rau xao")!=wstring::npos || ten.find(L"rau xào")!=wstring::npos) return 1;
        if(ten.find(L"mi tom")!=wstring::npos || ten.find(L"mì tôm")!=wstring::npos) return 2;
    } else {
        if(ten.find(L"tra dao")!=wstring::npos || ten.find(L"trà đào")!=wstring::npos) return 3;
        if(ten.find(L"nuoc cam")!=wstring::npos || ten.find(L"nước cam")!=wstring::npos) return 4;
    }
    return 5;
}

bool maAnhHopLe(const string& ma) {
    if(ma.empty()) return false;
    for(char c:ma) if(!((c>='A'&&c<='Z') || (c>='a'&&c<='z') ||
                      (c>='0'&&c<='9') || c=='_' || c=='-')) return false;
    return true;
}
}

void CuaSoNhaHang::trangDatMon() {
    int x=264, w=rongCuaSo-290, gioW=360, monW=w-gioW-24, phai=x+monW+24;
    nhap(L"Tìm món theo tên hoặc mã",TIM_KIEM,x,128,monW);
    int nua=(monW-12)/2;
    nut(L"Thức ăn",NUT_NHOM_THUCAN,x,198,nua,nhomDatMon==0?1:0);
    nut(L"Nước uống",NUT_NHOM_NUOC,x+nua+12,198,monW-nua-12,nhomDatMon==1?1:0);
    HWND danhSach=tao(L"LISTBOX",L"",WS_TABSTOP|WS_VSCROLL|LBS_NOTIFY|LBS_OWNERDRAWFIXED|
                     LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT,DANH_SACH_MON,x,250,monW,caoCuaSo-484);
    SendMessageW(danhSach,LB_SETITEMHEIGHT,0,128);
    nhap(L"Số lượng",O_SOLUONG,x,caoCuaSo-214,94,L"1");
    HWND nhanSize=tao(L"STATIC",L"Size nước uống",SS_LEFT|SS_CENTERIMAGE,NHAN_SIZE,x+110,caoCuaSo-214,144,24);
    SendMessageW(nhanSize,WM_SETFONT,reinterpret_cast<WPARAM>(fontNho),TRUE);
    chon(L"",O_SIZE,x+110,caoCuaSo-214,144,{L"S",L"M",L"L"});
    chonMuc(O_SIZE,1);
    ShowWindow(o(NHAN_SIZE),SW_HIDE); ShowWindow(o(O_SIZE),SW_HIDE);
    tao(L"STATIC",L"Chọn món ở trên để thêm vào giỏ",SS_LEFT|SS_CENTERIMAGE,
        NHAN_MON_CHON,x,caoCuaSo-144,monW,26);
    nut(L"+ Thêm món đã chọn",NUT_THEM_GIO,x,caoCuaSo-96,monW,1);

    nhan(L"Giỏ món của bạn",phai,128,gioW,true);
    nhan(L"Nhân viên xác nhận giá khi thanh toán.",phai,164,gioW);
    HWND gio=bang(BANG_GIO,phai,202,gioW,caoCuaSo-470,
        {{L"Món / tùy chọn",174},{L"SL",40},{L"Thành tiền",122}});
    LVCOLUMNW cot={}; cot.mask=LVCF_FMT; cot.fmt=LVCFMT_RIGHT;
    ListView_SetColumn(gio,2,&cot);
    tao(L"STATIC",L"Giỏ món đang trống.\nChọn món bên trái để bắt đầu.",SS_CENTER,
        NHAN_GIO_TRONG,phai+16,280,gioW-32,68);
    HWND tong=tao(L"STATIC",L"",SS_LEFT,O_TONG,phai,caoCuaSo-254,gioW,88);
    SendMessageW(tong,WM_SETFONT,reinterpret_cast<WPARAM>(fontDam),TRUE);
    nut(L"Xóa món",NUT_XOA_GIO,phai,caoCuaSo-146,174,2);
    nut(L"Hủy giỏ",NUT_HUY_GIO,phai+186,caoCuaSo-146,174);
    nut(L"Gửi đơn đặt món",NUT_GUI_DON,phai,caoCuaSo-96,gioW,1);
    thanhCong(L"Chọn Thức ăn hoặc Nước uống · Ảnh minh họa · Đơn chưa gửi chỉ lưu trong giỏ hiện tại.");
}

int CuaSoNhaHang::monDatDaChon() const {
    int dong=static_cast<int>(SendMessageW(o(DANH_SACH_MON),LB_GETCURSEL,0,0));
    if(dong==LB_ERR) return -1;
    return static_cast<int>(SendMessageW(o(DANH_SACH_MON),LB_GETITEMDATA,dong,0));
}

void CuaSoNhaHang::chonMonDat() {
    int i=monDatDaChon();
    bool hopLe=i>=0 && static_cast<size_t>(i)<dichVu.thucDon().size();
    bool nuoc=false, conHang=false;
    if(hopLe) {
        const Mon& mon=*dichVu.thucDon()[i];
        maMonDangChon=mon.getMa();
        nuoc=mon.getLoai()=="Nuoc uong";
        int daChon=0;
        for(const YeuCauMon& muc:gioHang) if(muc.maMon==mon.getMa()) daChon+=muc.soLuong;
        conHang=mon.getTonKho()>daChon;
        gan(NHAN_MON_CHON,L"Đang chọn: "+rong(mon.getTen()));
    } else {
        maMonDangChon.clear();
        gan(NHAN_MON_CHON,L"Chọn món ở trên để thêm vào giỏ");
    }
    EnableWindow(o(O_SIZE),nuoc);
    ShowWindow(o(NHAN_SIZE),nuoc?SW_SHOW:SW_HIDE);
    ShowWindow(o(O_SIZE),nuoc?SW_SHOW:SW_HIDE);
    EnableWindow(o(NUT_THEM_GIO),hopLe && conHang);
}

void CuaSoNhaHang::napMonDat() {
    if(!o(DANH_SACH_MON)) return;
    bool cu=dangVeBang; dangVeBang=true;
    HWND danhSach=o(DANH_SACH_MON);
    int gocCu=static_cast<int>(SendMessageW(danhSach,LB_GETTOPINDEX,0,0));
    SendMessageW(danhSach,WM_SETREDRAW,FALSE,0);
    SendMessageW(danhSach,LB_RESETCONTENT,0,0);
    wstring tim=chu(TIM_KIEM);
    if(!tim.empty()) CharLowerBuffW(&tim[0],static_cast<DWORD>(tim.size()));
    int thucAn=0, nuoc=0, dongChon=-1;
    for(size_t i=0;i<dichVu.thucDon().size();i++) {
        const Mon& mon=*dichVu.thucDon()[i];
        bool laNuoc=mon.getLoai()=="Nuoc uong";
        if(laNuoc) nuoc++; else thucAn++;
        if(laNuoc!=(nhomDatMon==1)) continue;
        wstring ten=rong(mon.getTen()+" "+mon.getMa());
        if(!ten.empty()) CharLowerBuffW(&ten[0],static_cast<DWORD>(ten.size()));
        if(ten.find(tim)==wstring::npos) continue;
        wstring nhan=rong(mon.getTen());
        int dong=static_cast<int>(SendMessageW(danhSach,LB_ADDSTRING,0,reinterpret_cast<LPARAM>(nhan.c_str())));
        SendMessageW(danhSach,LB_SETITEMDATA,dong,static_cast<LPARAM>(i));
        if(mon.getMa()==maMonDangChon) dongChon=dong;
    }
    if(SendMessageW(danhSach,LB_GETCOUNT,0,0)==0) {
        int dong=static_cast<int>(SendMessageW(danhSach,LB_ADDSTRING,0,reinterpret_cast<LPARAM>(L"Không có món phù hợp")));
        SendMessageW(danhSach,LB_SETITEMDATA,dong,-1);
    }
    gan(NUT_NHOM_THUCAN,L"Thức ăn ("+to_wstring(thucAn)+L")");
    gan(NUT_NHOM_NUOC,L"Nước uống ("+to_wstring(nuoc)+L")");
    kieuNut[o(NUT_NHOM_THUCAN)]=nhomDatMon==0?1:0;
    kieuNut[o(NUT_NHOM_NUOC)]=nhomDatMon==1?1:0;
    InvalidateRect(o(NUT_NHOM_THUCAN),nullptr,TRUE);
    InvalidateRect(o(NUT_NHOM_NUOC),nullptr,TRUE);
    SendMessageW(danhSach,LB_SETCURSEL,dongChon,0);
    if(gocCu>=0) SendMessageW(danhSach,LB_SETTOPINDEX,gocCu,0);
    SendMessageW(danhSach,WM_SETREDRAW,TRUE,0);
    InvalidateRect(danhSach,nullptr,TRUE);
    dangVeBang=cu;
    chonMonDat();
}

void CuaSoNhaHang::veMonDat(const DRAWITEMSTRUCT* dong) {
    if(dong->itemID==static_cast<UINT>(-1)) return;
    HDC dc=dong->hDC;
    RECT vung=dong->rcItem;
    FillRect(dc,&vung,nen);
    int i=static_cast<int>(dong->itemData);
    if(i<0 || static_cast<size_t>(i)>=dichVu.thucDon().size()) {
        viet(dc,L"Không có món phù hợp",vung,fontChu,RGB(108,122,144)); return;
    }
    const Mon& mon=*dichVu.thucDon()[i];
    bool chon=(dong->itemState&ODS_SELECTED)!=0;
    RECT the=vung; InflateRect(&the,-2,-4);
    HBRUSH mau=CreateSolidBrush(chon?RGB(237,244,255):RGB(255,255,255));
    HPEN vien=CreatePen(PS_SOLID,chon?2:1,chon?RGB(40,104,222):RGB(225,232,242));
    HGDIOBJ cuMau=SelectObject(dc,mau), cuVien=SelectObject(dc,vien);
    RoundRect(dc,the.left,the.top,the.right,the.bottom,14,14);
    SelectObject(dc,cuMau); SelectObject(dc,cuVien); DeleteObject(mau); DeleteObject(vien);
    veAnhMon(dc,mon,{the.left+10,the.top+10,the.left+110,the.bottom-10});
    int trai=the.left+124, phai=the.right-14;
    viet(dc,rong(mon.getTen()),{trai,the.top+12,phai,the.top+40},fontDam,RGB(26,40,64));
    viet(dc,tien(mon.getGia()),{trai,the.top+43,phai,the.top+69},fontDam,RGB(33,94,192));
    int trongGio=0;
    for(const YeuCauMon& muc:gioHang) if(muc.maMon==mon.getMa()) trongGio+=muc.soLuong;
    int con=mon.getTonKho()-trongGio;
    if(con<=0)
        viet(dc,L"Đã hết món",{trai,the.top+76,phai,the.top+101},fontNho,RGB(185,44,44));
    if(dong->itemState&ODS_FOCUS) { RECT r=the; InflateRect(&r,-5,-5); DrawFocusRect(dc,&r); }
}

void CuaSoNhaHang::veAnhMon(HDC dc,const Mon& mon,RECT khung) {
    HBITMAP anh=anhThucDon;
    if(!anhMonRieng.count(mon.getMa())) {
        HBITMAP rieng=nullptr;
        if(maAnhHopLe(mon.getMa())) rieng=taiAnhTep(L"DuLieu/AnhMon/"+rong(mon.getMa())+L".png");
        anhMonRieng[mon.getMa()]=rieng;
    }
    if(anhMonRieng[mon.getMa()]) anh=anhMonRieng[mon.getMa()];
    BITMAP kichThuoc={}; GetObjectW(anh,sizeof(kichThuoc),&kichThuoc);
    RECT nguon={0,0,kichThuoc.bmWidth,kichThuoc.bmHeight};
    if(anh==anhThucDon) {
        int viTri=oAnhMinhHoa(mon), cot=viTri%3, catDoc=kichThuoc.bmHeight*474/1000;
        nguon={cot*kichThuoc.bmWidth/3,viTri<3?0:catDoc,
               (cot+1)*kichThuoc.bmWidth/3,viTri<3?catDoc:kichThuoc.bmHeight};
    }
    veAnhCat(dc,anh,nguon,khung);
}
