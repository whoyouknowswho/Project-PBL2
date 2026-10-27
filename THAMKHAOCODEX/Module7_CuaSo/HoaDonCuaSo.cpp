#include "CuaSo.h"
#include <sstream>

using namespace std;

namespace {
LRESULT CALLBACK thuTucHoaDon(HWND h,UINT tin,WPARAM w,LPARAM l) {
    switch(tin) {
        case WM_CREATE: {
            const wstring* noiDung=static_cast<const wstring*>(reinterpret_cast<CREATESTRUCTW*>(l)->lpCreateParams);
            HWND edit=CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",noiDung->c_str(),
                WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_MULTILINE|ES_READONLY|ES_AUTOVSCROLL|WS_VSCROLL,
                20,20,650,500,h,reinterpret_cast<HMENU>(1),GetModuleHandleW(nullptr),nullptr);
            HFONT font=CreateFontW(-16,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
            SetWindowLongPtrW(h,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(font));
            SendMessageW(edit,WM_SETFONT,reinterpret_cast<WPARAM>(font),TRUE);
            HWND btn=CreateWindowW(L"BUTTON",L"Đóng",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON,
                540,540,130,38,h,reinterpret_cast<HMENU>(2),GetModuleHandleW(nullptr),nullptr);
            SendMessageW(btn,WM_SETFONT,reinterpret_cast<WPARAM>(font),TRUE);
            return 0;
        }
        case WM_SIZE: {
            int width=LOWORD(l),height=HIWORD(l);
            MoveWindow(GetDlgItem(h,1),20,20,width-40,height-90,TRUE);
            MoveWindow(GetDlgItem(h,2),width-150,height-54,130,36,TRUE); return 0;
        }
        case WM_COMMAND: if(LOWORD(w)==2) DestroyWindow(h); return 0;
        case WM_GETMINMAXINFO: {
            MINMAXINFO* kichThuoc=reinterpret_cast<MINMAXINFO*>(l);
            kichThuoc->ptMinTrackSize={460,400}; return 0;
        }
        case WM_CLOSE: DestroyWindow(h); return 0;
        case WM_DESTROY: DeleteObject(reinterpret_cast<HFONT>(GetWindowLongPtrW(h,GWLP_USERDATA))); return 0;
    }
    return DefWindowProcW(h,tin,w,l);
}
}

void xemHoaDon(HWND chu,const HoaDon& hoaDon) {
    stringstream ss(hoaDon.toFile()); string muc; vector<string> meta;
    while(getline(ss,muc,'|') && meta.size()<5) meta.push_back(muc);
    wostringstream s;
    s<<L"NHÀ HÀNG • HÓA ĐƠN BÁN HÀNG\r\n\r\n"<<L"Mã: "<<rong(hoaDon.getMa())
     <<L"\r\nNgày lập: "<<rong(hoaDon.getNgayLap());
    if(meta.size()>=4) s<<L"\r\nKhách hàng: "<<(meta[2].empty()?L"Khách lẻ":rong(meta[2]))<<L"\r\nNhân viên: "<<rong(meta[3]);
    s<<L"\r\n\r\n--------------------------------------------------------\r\n";
    for(const ChiTietHoaDon& ct:hoaDon.getGioHang()) {
        s<<rong(ct.tenMon)<<L"  ["<<rong(ct.maMon)<<L"]\r\n    "<<ct.soLuong<<L" × "<<tien(ct.donGia)<<L"  =  "<<tien(ct.thanhTien())<<L"\r\n\r\n";
    }
    s<<L"--------------------------------------------------------\r\nTạm tính: "<<tien(hoaDon.tamTinh())
     <<L"\r\nGiảm giá: "<<tien(hoaDon.tienGiam())<<L"\r\nTHANH TOÁN: "<<tien(hoaDon.tongTien())
     <<L"\r\n\r\nCảm ơn quý khách. Hẹn gặp lại!";
    wstring noiDung=s.str();
    WNDCLASSW lop={}; lop.lpfnWndProc=thuTucHoaDon; lop.hInstance=GetModuleHandleW(nullptr);
    lop.lpszClassName=L"NhaHangHoaDon"; lop.hCursor=LoadCursor(nullptr,IDC_ARROW); lop.hbrBackground=reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
    RegisterClassW(&lop);
    RECT r; GetWindowRect(chu,&r);
    HWND h=CreateWindowExW(WS_EX_CONTROLPARENT,lop.lpszClassName,L"Hóa đơn đã lưu",
        WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_CLIPCHILDREN,
        r.left+120,r.top+50,720,650,chu,nullptr,lop.hInstance,&noiDung);
    if(!h) return;
    EnableWindow(chu,FALSE); ShowWindow(h,SW_SHOW); SetFocus(GetDlgItem(h,2));
    MSG tin;
    while(IsWindow(h) && GetMessageW(&tin,nullptr,0,0)>0) {
        if(!IsDialogMessageW(h,&tin)) { TranslateMessage(&tin); DispatchMessageW(&tin); }
    }
    EnableWindow(chu,TRUE); SetForegroundWindow(chu);
}
