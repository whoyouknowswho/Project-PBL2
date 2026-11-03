#include "CuaSo.h"
#include <uxtheme.h>
#include <algorithm>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {
const COLORREF NEN = RGB(242,246,251);
const COLORREF CHU = RGB(26,40,64);
const COLORREF CHU_MO = RGB(104,120,142);
const COLORREF XANH = RGB(37,99,220);

// Moi cua so giu mot ban sao hoa don, font chu va bang mon cua rieng no.
struct CuaSoHoaDon {
    HoaDon hoaDon;
    wstring khachHang, nhanVien;
    HWND bangMon = nullptr, nutDong = nullptr;
    HFONT fontChu = nullptr, fontNho = nullptr, fontDam = nullptr;
    HFONT fontTieuDe = nullptr, fontTong = nullptr;
    HIMAGELIST chieuCaoDong = nullptr;
    int rong = 900, cao = 700;
};

HFONT taoFont(int co, int doDam = FW_NORMAL) {
    return CreateFontW(-co,0,0,0,doDam,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
                       0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
}

void veChu(HDC dc, const wstring& chu, RECT vung, HFONT font,
           COLORREF mau = CHU, UINT can = DT_LEFT) {
    HGDIOBJ cu = SelectObject(dc,font);
    SetBkMode(dc,TRANSPARENT);
    SetTextColor(dc,mau);
    DrawTextW(dc,chu.c_str(),-1,&vung,can|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_NOPREFIX);
    SelectObject(dc,cu);
}

void veThe(HDC dc, RECT vung, COLORREF mau, int boGoc = 14) {
    HBRUSH brush = CreateSolidBrush(mau);
    HGDIOBJ brushCu = SelectObject(dc,brush);
    HGDIOBJ penCu = SelectObject(dc,GetStockObject(NULL_PEN));
    RoundRect(dc,vung.left,vung.top,vung.right,vung.bottom,boGoc,boGoc);
    SelectObject(dc,brushCu); SelectObject(dc,penCu); DeleteObject(brush);
}

void veDuong(HDC dc, int x1, int x2, int y) {
    HPEN pen = CreatePen(PS_SOLID,1,RGB(225,232,242));
    HGDIOBJ cu = SelectObject(dc,pen);
    MoveToEx(dc,x1,y,nullptr); LineTo(dc,x2,y);
    SelectObject(dc,cu); DeleteObject(pen);
}

void sapXep(HWND h, CuaSoHoaDon& cs) {
    RECT r; GetClientRect(h,&r); cs.rong=r.right; cs.cao=r.bottom;
    int bangRong=cs.rong-80;
    MoveWindow(cs.bangMon,40,258,bangRong,max(100,cs.cao-486),TRUE);
    int cotStt=42, cotSl=52, cotGia=132, cotTien=146;
    ListView_SetColumnWidth(cs.bangMon,0,cotStt);
    ListView_SetColumnWidth(cs.bangMon,1,max(180,bangRong-cotStt-cotSl-cotGia-cotTien-22));
    ListView_SetColumnWidth(cs.bangMon,2,cotSl);
    ListView_SetColumnWidth(cs.bangMon,3,cotGia);
    ListView_SetColumnWidth(cs.bangMon,4,cotTien);
    MoveWindow(cs.nutDong,cs.rong-190,cs.cao-48,150,36,TRUE);
    InvalidateRect(h,nullptr,FALSE);
}

void taoBang(HWND h, CuaSoHoaDon& cs) {
    cs.bangMon=CreateWindowExW(0,WC_LISTVIEWW,L"Chi tiết các món trong hóa đơn",
        WS_CHILD|WS_VISIBLE|WS_TABSTOP|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS,
        40,258,800,220,h,reinterpret_cast<HMENU>(1),GetModuleHandleW(nullptr),nullptr);
    if(!cs.bangMon) throw runtime_error("Không tạo được bảng hóa đơn.");
    ListView_SetExtendedListViewStyle(cs.bangMon,LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER|LVS_EX_LABELTIP);
    ListView_SetBkColor(cs.bangMon,RGB(255,255,255));
    ListView_SetTextBkColor(cs.bangMon,RGB(255,255,255));
    SetWindowTheme(cs.bangMon,L"Explorer",nullptr);
    SendMessageW(cs.bangMon,WM_SETFONT,reinterpret_cast<WPARAM>(cs.fontChu),TRUE);
    SendMessageW(ListView_GetHeader(cs.bangMon),WM_SETFONT,reinterpret_cast<WPARAM>(cs.fontNho),TRUE);
    cs.chieuCaoDong=ImageList_Create(1,46,ILC_COLOR32,1,1);
    ListView_SetImageList(cs.bangMon,cs.chieuCaoDong,LVSIL_SMALL);
    const vector<wstring> tenCot={L"#",L"MÓN / MÃ MÓN",L"SL",L"ĐƠN GIÁ",L"THÀNH TIỀN"};
    for(size_t i=0;i<tenCot.size();i++) {
        LVCOLUMNW cot={}; cot.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
        cot.pszText=const_cast<wchar_t*>(tenCot[i].c_str()); cot.cx=130;
        cot.fmt=LVCFMT_LEFT;
        if(i==0 || i==2) cot.fmt=LVCFMT_CENTER;
        if(i>=3) cot.fmt=LVCFMT_RIGHT;
        ListView_InsertColumn(cs.bangMon,static_cast<int>(i),&cot);
    }
    const vector<ChiTietHoaDon>& gio=cs.hoaDon.getGioHang();
    for(size_t i=0;i<gio.size();i++) {
        const ChiTietHoaDon& ct=gio[i];
        vector<wstring> cacCot={to_wstring(i+1),rong(ct.tenMon+" ["+ct.maMon+"]"),
            to_wstring(ct.soLuong),tien(ct.donGia),tien(ct.thanhTien())};
        LVITEMW dong={}; dong.mask=LVIF_TEXT; dong.iItem=static_cast<int>(i);
        dong.pszText=const_cast<wchar_t*>(cacCot[0].c_str());
        int viTri=ListView_InsertItem(cs.bangMon,&dong);
        for(size_t cot=1;cot<cacCot.size();cot++)
            ListView_SetItemText(cs.bangMon,viTri,static_cast<int>(cot),const_cast<wchar_t*>(cacCot[cot].c_str()));
    }
}

LRESULT veDong(NMLVCUSTOMDRAW* ve, CuaSoHoaDon& cs) {
    if(ve->nmcd.dwDrawStage==CDDS_PREPAINT) return CDRF_NOTIFYITEMDRAW;
    if(ve->nmcd.dwDrawStage==CDDS_ITEMPREPAINT) return CDRF_NOTIFYSUBITEMDRAW;
    if(ve->nmcd.dwDrawStage!=(CDDS_ITEMPREPAINT|CDDS_SUBITEM)) return CDRF_DODEFAULT;
    int dong=static_cast<int>(ve->nmcd.dwItemSpec), cot=ve->iSubItem;
    if(dong<0 || static_cast<size_t>(dong)>=cs.hoaDon.getGioHang().size()) return CDRF_DODEFAULT;
    const ChiTietHoaDon& ct=cs.hoaDon.getGioHang()[dong];
    RECT r; ListView_GetSubItemRect(cs.bangMon,dong,cot,LVIR_BOUNDS,&r);
    if(cot==0) r.right=r.left+ListView_GetColumnWidth(cs.bangMon,0);
    COLORREF mauNen=dong%2==0?RGB(255,255,255):RGB(246,249,253);
    if(ListView_GetItemState(cs.bangMon,dong,LVIS_SELECTED)) mauNen=RGB(230,239,255);
    HBRUSH brush=CreateSolidBrush(mauNen); FillRect(ve->nmcd.hdc,&r,brush); DeleteObject(brush);
    if(cot==1) {
        RECT ten={r.left+12,r.top+3,r.right-12,r.top+25};
        RECT ma={r.left+12,r.top+25,r.right-12,r.bottom-2};
        veChu(ve->nmcd.hdc,rong(ct.tenMon),ten,cs.fontChu);
        veChu(ve->nmcd.hdc,rong(ct.maMon),ma,cs.fontNho,CHU_MO);
    } else {
        wstring chu;
        UINT can=DT_CENTER;
        if(cot==0) chu=to_wstring(dong+1);
        if(cot==2) chu=to_wstring(ct.soLuong);
        if(cot==3) { chu=tien(ct.donGia); can=DT_RIGHT; }
        if(cot==4) { chu=tien(ct.thanhTien()); can=DT_RIGHT; }
        r.left+=8; r.right-=12;
        veChu(ve->nmcd.hdc,chu,r,cot==4?cs.fontDam:cs.fontChu,cot==0?CHU_MO:CHU,can);
    }
    return CDRF_SKIPDEFAULT;
}

void veHoaDon(HDC dich, CuaSoHoaDon& cs) {
    // Ve vao bo dem de tranh nhap nhay khi keo gian cua so.
    HDC dc=CreateCompatibleDC(dich);
    HBITMAP anh=CreateCompatibleBitmap(dich,max(1,cs.rong),max(1,cs.cao));
    HGDIOBJ anhCu=SelectObject(dc,anh);
    RECT toan={0,0,cs.rong,cs.cao};
    HBRUSH brush=CreateSolidBrush(NEN); FillRect(dc,&toan,brush); DeleteObject(brush);
    veThe(dc,{20,18,cs.rong-20,cs.cao-64},RGB(255,255,255),18);
    veChu(dc,L"NHÀ HÀNG",{40,30,300,52},cs.fontDam,XANH);
    veChu(dc,L"Hóa đơn bán hàng",{40,56,cs.rong-200,94},cs.fontTieuDe);
    veThe(dc,{cs.rong-202,36,cs.rong-40,66},RGB(229,247,237),12);
    veChu(dc,L"ĐÃ THANH TOÁN",{cs.rong-200,36,cs.rong-42,66},cs.fontNho,RGB(30,126,81),DT_CENTER);
    int giua=cs.rong/2+30;
    veChu(dc,L"MÃ HÓA ĐƠN",{40,110,giua-20,132},cs.fontNho,CHU_MO);
    veChu(dc,rong(cs.hoaDon.getMa()),{40,134,giua-20,160},cs.fontChu);
    veChu(dc,L"NGÀY LẬP",{giua,110,cs.rong-40,132},cs.fontNho,CHU_MO);
    veChu(dc,rong(cs.hoaDon.getNgayLap()),{giua,134,cs.rong-40,160},cs.fontChu);
    veChu(dc,L"KHÁCH HÀNG",{40,172,giua-20,194},cs.fontNho,CHU_MO);
    veChu(dc,cs.khachHang,{40,196,giua-20,222},cs.fontChu);
    veChu(dc,L"NHÂN VIÊN",{giua,172,cs.rong-40,194},cs.fontNho,CHU_MO);
    veChu(dc,cs.nhanVien,{giua,196,cs.rong-40,222},cs.fontChu);
    veChu(dc,L"CHI TIẾT ĐƠN HÀNG",{40,230,400,252},cs.fontNho,CHU_MO);
    veDuong(dc,40,cs.rong-40,cs.cao-218);
    long long soPhan=0;
    for(const ChiTietHoaDon& ct:cs.hoaDon.getGioHang()) soPhan+=ct.soLuong;
    int y=cs.cao-206;
    veChu(dc,to_wstring(cs.hoaDon.getGioHang().size())+L" món  ·  "+to_wstring(soPhan)+L" phần",
          {40,y+10,cs.rong-385,y+36},cs.fontNho,CHU_MO);
    veChu(dc,L"Cảm ơn quý khách!",{40,y+56,cs.rong-385,y+86},cs.fontDam);
    veChu(dc,L"Hẹn gặp lại trong lần tới.",{40,y+88,cs.rong-385,y+114},cs.fontNho,CHU_MO);
    int trai=cs.rong-364, phai=cs.rong-40;
    veChu(dc,L"Tạm tính",{trai,y,phai-165,y+28},cs.fontChu,CHU_MO);
    veChu(dc,tien(cs.hoaDon.tamTinh()),{phai-165,y,phai,y+28},cs.fontChu,CHU,DT_RIGHT);
    wstring giam=tien(cs.hoaDon.tienGiam());
    if(cs.hoaDon.tienGiam()>0) giam=L"− "+giam;
    veChu(dc,L"Giảm giá",{trai,y+32,phai-165,y+60},cs.fontChu,CHU_MO);
    veChu(dc,giam,{phai-165,y+32,phai,y+60},cs.fontChu,RGB(30,126,81),DT_RIGHT);
    veThe(dc,{trai-14,y+76,phai+8,y+130},RGB(234,241,255),12);
    veChu(dc,L"TỔNG TIỀN",{trai,y+82,trai+106,y+124},cs.fontNho,XANH);
    veChu(dc,tien(cs.hoaDon.tongTien()),{trai+106,y+82,phai-4,y+124},cs.fontTong,XANH,DT_RIGHT);
    BitBlt(dich,0,0,cs.rong,cs.cao,dc,0,0,SRCCOPY);
    SelectObject(dc,anhCu); DeleteObject(anh); DeleteDC(dc);
}

LRESULT CALLBACK thuTucHoaDon(HWND h,UINT tin,WPARAM w,LPARAM l) {
    CuaSoHoaDon* cs=reinterpret_cast<CuaSoHoaDon*>(GetWindowLongPtrW(h,GWLP_USERDATA));
    if(tin==WM_NCCREATE) {
        cs=static_cast<CuaSoHoaDon*>(reinterpret_cast<CREATESTRUCTW*>(l)->lpCreateParams);
        SetWindowLongPtrW(h,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(cs));
    }
    if(!cs) return DefWindowProcW(h,tin,w,l);
    switch(tin) {
        case WM_CREATE:
            cs->fontChu=taoFont(16); cs->fontNho=taoFont(13);
            cs->fontDam=taoFont(17,FW_SEMIBOLD); cs->fontTieuDe=taoFont(28,FW_SEMIBOLD);
            cs->fontTong=taoFont(23,FW_BOLD);
            try { taoBang(h,*cs); }
            catch(const exception&) { return -1; }
            cs->nutDong=CreateWindowW(L"BUTTON",L"Đóng hóa đơn",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_OWNERDRAW,
                680,650,150,36,h,reinterpret_cast<HMENU>(2),GetModuleHandleW(nullptr),nullptr);
            sapXep(h,*cs); return 0;
        case WM_SIZE: if(w!=SIZE_MINIMIZED && cs->bangMon) sapXep(h,*cs); return 0;
        case WM_NOTIFY: {
            NMHDR* n=reinterpret_cast<NMHDR*>(l);
            if(n->hwndFrom==cs->bangMon && n->code==NM_CUSTOMDRAW) return veDong(reinterpret_cast<NMLVCUSTOMDRAW*>(l),*cs);
            return 0;
        }
        case WM_DRAWITEM: {
            DRAWITEMSTRUCT* n=reinterpret_cast<DRAWITEMSTRUCT*>(l);
            HBRUSH bg=CreateSolidBrush(NEN); FillRect(n->hDC,&n->rcItem,bg); DeleteObject(bg);
            COLORREF mau=XANH;
            if(n->itemState&ODS_SELECTED) mau=RGB(28,76,171);
            veThe(n->hDC,n->rcItem,mau,10);
            veChu(n->hDC,L"Đóng hóa đơn",n->rcItem,cs->fontChu,RGB(255,255,255),DT_CENTER);
            if(n->itemState&ODS_FOCUS) { RECT r=n->rcItem; InflateRect(&r,-4,-4); DrawFocusRect(n->hDC,&r); }
            return TRUE;
        }
        case WM_COMMAND: if(LOWORD(w)==2 || LOWORD(w)==IDCANCEL) DestroyWindow(h); return 0;
        case WM_GETMINMAXINFO: {
            MINMAXINFO* kichThuoc=reinterpret_cast<MINMAXINFO*>(l);
            kichThuoc->ptMinTrackSize={760,650}; return 0;
        }
        case WM_ERASEBKGND: return 1;
        case WM_PAINT: { PAINTSTRUCT ps; HDC dc=BeginPaint(h,&ps); veHoaDon(dc,*cs); EndPaint(h,&ps); return 0; }
        case WM_CLOSE: DestroyWindow(h); return 0;
        case WM_DESTROY:
            ListView_SetImageList(cs->bangMon,nullptr,LVSIL_SMALL);
            if(cs->chieuCaoDong) ImageList_Destroy(cs->chieuCaoDong);
            DeleteObject(cs->fontChu); DeleteObject(cs->fontNho); DeleteObject(cs->fontDam);
            DeleteObject(cs->fontTieuDe); DeleteObject(cs->fontTong); return 0;
    }
    return DefWindowProcW(h,tin,w,l);
}
}

void xemHoaDon(HWND chu,const HoaDon& hoaDon) {
    CuaSoHoaDon cs; cs.hoaDon=hoaDon;
    stringstream luong(hoaDon.toFile()); string muc; vector<string> meta;
    while(getline(luong,muc,'|') && meta.size()<4) meta.push_back(muc);
    cs.khachHang=L"Khách lẻ"; cs.nhanVien=L"—";
    if(meta.size()>=4) {
        if(!meta[2].empty()) cs.khachHang=rong(meta[2]);
        if(!meta[3].empty()) cs.nhanVien=rong(meta[3]);
    }
    WNDCLASSW lop={}; lop.lpfnWndProc=thuTucHoaDon; lop.hInstance=GetModuleHandleW(nullptr);
    lop.lpszClassName=L"NhaHangHoaDonDep"; lop.hCursor=LoadCursor(nullptr,IDC_ARROW);
    if(!RegisterClassW(&lop) && GetLastError()!=ERROR_CLASS_ALREADY_EXISTS) throw runtime_error("Không đăng ký được cửa sổ hóa đơn.");
    RECT r; GetWindowRect(chu,&r);
    MONITORINFO manHinh={}; manHinh.cbSize=sizeof(manHinh);
    GetMonitorInfoW(MonitorFromWindow(chu,MONITOR_DEFAULTTONEAREST),&manHinh);
    int w=min(940,static_cast<int>(manHinh.rcWork.right-manHinh.rcWork.left-32));
    int cao=min(770,static_cast<int>(manHinh.rcWork.bottom-manHinh.rcWork.top-32));
    int x=max(static_cast<int>(manHinh.rcWork.left),(static_cast<int>(r.left+r.right)-w)/2);
    int y=max(static_cast<int>(manHinh.rcWork.top),(static_cast<int>(r.top+r.bottom)-cao)/2);
    x=min(x,static_cast<int>(manHinh.rcWork.right)-w); y=min(y,static_cast<int>(manHinh.rcWork.bottom)-cao);
    wstring tieuDe=L"Hóa đơn • "+rong(hoaDon.getMa());
    HWND h=CreateWindowExW(WS_EX_CONTROLPARENT,lop.lpszClassName,tieuDe.c_str(),
        WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_CLIPCHILDREN,
        x,y,w,cao,chu,nullptr,lop.hInstance,&cs);
    if(!h) throw runtime_error("Không mở được cửa sổ hóa đơn.");
    EnableWindow(chu,FALSE); ShowWindow(h,SW_SHOW); SetFocus(cs.nutDong);
    MSG tin={};
    while(IsWindow(h)) {
        int ketQua=GetMessageW(&tin,nullptr,0,0);
        if(ketQua<=0) {
            DestroyWindow(h);
            if(ketQua==0) PostQuitMessage(static_cast<int>(tin.wParam));
            break;
        }
        if(!IsDialogMessageW(h,&tin)) { TranslateMessage(&tin); DispatchMessageW(&tin); }
    }
    if(IsWindow(chu)) { EnableWindow(chu,TRUE); SetForegroundWindow(chu); }
}
