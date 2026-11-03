#include "CuaSo.h"
#include <algorithm>

using namespace std;

namespace {
void canCot(HWND bang, int cot, int can) {
    LVCOLUMNW thongTin = {};
    thongTin.mask = LVCF_FMT;
    thongTin.fmt = can;
    ListView_SetColumn(bang,cot,&thongTin);
}

void veChu(HDC dc, const wstring& chu, RECT vung, HFONT font, COLORREF mau, bool nhieuDong=false) {
    SelectObject(dc,font);
    SetTextColor(dc,mau);
    SetBkMode(dc,TRANSPARENT);
    DrawTextW(dc,chu.c_str(),-1,&vung,DT_LEFT|DT_NOPREFIX|DT_END_ELLIPSIS|
              (nhieuDong?DT_WORDBREAK:DT_SINGLELINE|DT_VCENTER));
}
}

void CuaSoNhaHang::trangBaoCao() {
    int x=264, w=rongCuaSo-290, nhapW=w-560;
    nhap(L"Ngày / tháng cần xem",LOC_BAO_CAO,x,128,nhapW,rong(phamViBaoCao));
    SendMessageW(o(LOC_BAO_CAO),EM_SETCUEBANNER,TRUE,
                 reinterpret_cast<LPARAM>(L"dd/mm/yyyy hoặc mm/yyyy"));
    nut(L"Áp dụng",NUT_BAOCAO,x+nhapW+12,152,132,1);
    nut(L"Hôm nay",NUT_BC_HOMNAY,x+w-400,152,104);
    nut(L"Tháng này",NUT_BC_THANG,x+w-284,152,136);
    nut(L"Tất cả",NUT_BC_TATCA,x+w-136,152,136);
    tao(L"STATIC",L"",SS_LEFT|SS_CENTERIMAGE,NHAN_KY_BAOCAO,x,202,w,24);

    int trai=(w-20)*2/3, phai=w-trai-20, xPhai=x+trai+20;
    nhan(L"Danh sách hóa đơn",x,372,trai-224,true);
    nut(L"Xem hóa đơn đã chọn",NUT_HOADON,x+trai-218,366,218);
    nhan(L"Mới nhất ở đầu · Chọn dòng hoặc nhấp đúp để xem",x,410,trai);
    nhan(L"Cần nhập thêm",xPhai,372,phai,true);
    nhan(L"Tồn kho hiện tại (≤ 5)",xPhai,410,phai);
    HWND hoaDon=bang(BANG_CHINH,x,444,trai,caoCuaSo-532,
        {{L"Mã hóa đơn",trai-284},{L"Ngày lập",128},{L"Thanh toán",132}});
    HWND tonKho=bang(BANG_TON,xPhai,444,phai,caoCuaSo-532,
        {{L"Tên món",phai-124},{L"Còn lại",100}});
    SetWindowTextW(hoaDon,L"Danh sách hóa đơn");
    SetWindowTextW(tonKho,L"Tồn kho hiện tại");
    canCot(hoaDon,2,LVCFMT_RIGHT);
    canCot(tonKho,1,LVCFMT_RIGHT);

    // Tang khoang cach giua cac dong, dung chung anh rong cho hai bang.
    khoangDongBaoCao=ImageList_Create(1,32,ILC_COLOR32,1,1);
    if(khoangDongBaoCao) {
        for(HWND bang:{hoaDon,tonKho}) {
            SetWindowLongPtrW(bang,GWL_STYLE,GetWindowLongPtrW(bang,GWL_STYLE)|LVS_SHAREIMAGELISTS);
            ListView_SetImageList(bang,khoangDongBaoCao,LVSIL_SMALL);
        }
    }
    tao(L"STATIC",L"",SS_LEFT|SS_CENTERIMAGE,O_TONG,x,caoCuaSo-76,w,24);
    napBaoCao();
}

void CuaSoNhaHang::napBaoCao() {
    // Chi thay ket qua dang hien sau khi bo loc da duoc kiem tra thanh cong.
    BaoCaoKinhDoanh ketQua=dichVu.baoCao(duLieu(LOC_BAO_CAO));
    phamViBaoCao=duLieu(LOC_BAO_CAO);
    baoCaoHienTai=ketQua;
    hoaDonBaoCao=ketQua.hoaDon;
    reverse(hoaDonBaoCao.begin(),hoaDonBaoCao.end());
    bool cu=dangVeBang; dangVeBang=true;
    ListView_DeleteAllItems(o(BANG_CHINH));
    ListView_DeleteAllItems(o(BANG_TON));
    for(size_t i=0;i<hoaDonBaoCao.size();i++) {
        const HoaDon& hd=hoaDonBaoCao[i];
        dongBang(BANG_CHINH,{rong(hd.getMa()),rong(hd.getNgayLap()),tien(hd.tongTien())},static_cast<int>(i));
    }
    if(hoaDonBaoCao.empty()) dongBang(BANG_CHINH,{L"Chưa có hóa đơn",L"",L""},-1);
    int canNhap=0;
    for(const unique_ptr<Mon>& mon:dichVu.thucDon()) {
        if(mon->getTonKho()>5) continue;
        dongBang(BANG_TON,{rong(mon->getTen()),mon->getTonKho()==0?L"Hết hàng":so(mon->getTonKho())},canNhap++);
    }
    if(canNhap==0) dongBang(BANG_TON,{L"Tồn kho ổn định",L""},-1);
    wstring ky=L"Toàn bộ thời gian";
    if(phamViBaoCao.size()==10) ky=L"Ngày "+rong(phamViBaoCao);
    if(phamViBaoCao.size()==7) ky=L"Tháng "+rong(phamViBaoCao);
    gan(NHAN_KY_BAOCAO,L"Đang xem: "+ky);
    gan(O_TONG,L"Hiển thị "+to_wstring(hoaDonBaoCao.size())+L" hóa đơn  ·  "+
        to_wstring(canNhap)+L" món cần nhập thêm  ·  Doanh thu đã trừ giảm giá thành viên.");
    EnableWindow(o(NUT_HOADON),FALSE);
    for(int id:{NUT_BC_HOMNAY,NUT_BC_THANG,NUT_BC_TATCA}) {
        bool chon=(id==NUT_BC_HOMNAY && phamViBaoCao==homNay()) ||
            (id==NUT_BC_THANG && phamViBaoCao==homNay().substr(3)) ||
            (id==NUT_BC_TATCA && phamViBaoCao.empty());
        kieuNut[o(id)]=chon?1:0;
        InvalidateRect(o(id),nullptr,TRUE);
    }
    dangVeBang=cu;
    thanhCong(L"Báo cáo chỉ đọc · Chọn thời gian rồi nhấn Áp dụng hoặc dùng nút lọc nhanh.");
    InvalidateRect(cuaSo,nullptr,FALSE);
}

void CuaSoNhaHang::veThongKeBaoCao(HDC dc) {
    int x=264, w=rongCuaSo-290, theW=(w-32)/3;
    const wstring nhan[]={L"DOANH THU",L"SỐ HÓA ĐƠN",L"MÓN BÁN CHẠY"};
    const wstring giaTri[]={tien(baoCaoHienTai.doanhThu),to_wstring(baoCaoHienTai.soHoaDon),
        rong(baoCaoHienTai.monBanChay)};
    const wstring ghiChu[]={L"Sau giảm giá thành viên",L"Đã thanh toán trong kỳ",L"Số lượng bán trong kỳ"};
    for(int i=0;i<3;i++) {
        int trai=x+i*(theW+16), phai=i==2?x+w:trai+theW;
        HBRUSH nenThe=CreateSolidBrush(RGB(255,255,255));
        HPEN vien=CreatePen(PS_SOLID,1,RGB(225,232,242));
        HGDIOBJ nenCu=SelectObject(dc,nenThe), vienCu=SelectObject(dc,vien);
        RoundRect(dc,trai,242,phai,350,16,16);
        SelectObject(dc,nenCu); SelectObject(dc,vienCu);
        DeleteObject(nenThe); DeleteObject(vien);
        veChu(dc,nhan[i],{trai+18,252,phai-18,274},fontNho,RGB(108,122,144));
        veChu(dc,giaTri[i],{trai+18,278,phai-18,316},fontDam,
              i==0?RGB(33,94,192):RGB(26,40,64));
        veChu(dc,ghiChu[i],{trai+18,320,phai-18,342},fontNho,RGB(108,122,144));
    }
}

LRESULT CuaSoNhaHang::veDongBaoCao(NMLVCUSTOMDRAW* dong) {
    if(dong->nmcd.dwDrawStage==CDDS_PREPAINT) return CDRF_NOTIFYITEMDRAW;
    if(dong->nmcd.dwDrawStage==CDDS_ITEMPREPAINT) return CDRF_NOTIFYSUBITEMDRAW;
    if(dong->nmcd.dwDrawStage!=(CDDS_ITEMPREPAINT|CDDS_SUBITEM)) return CDRF_DODEFAULT;
    bool daChon=(ListView_GetItemState(dong->nmcd.hdr.hwndFrom,
        static_cast<int>(dong->nmcd.dwItemSpec),LVIS_SELECTED)&LVIS_SELECTED)!=0;
    if(!daChon) {
        dong->clrTextBk=dong->nmcd.dwItemSpec%2?RGB(246,249,253):RGB(255,255,255);
        dong->clrText=RGB(26,40,64);
        if(dong->nmcd.lItemlParam==-1) dong->clrText=RGB(108,122,144);
        else if(dong->nmcd.hdr.idFrom==BANG_TON) {
            wchar_t soLuong[32]={};
            ListView_GetItemText(dong->nmcd.hdr.hwndFrom,static_cast<int>(dong->nmcd.dwItemSpec),1,soLuong,32);
            dong->clrText=wcscmp(soLuong,L"Hết hàng")==0?RGB(185,44,44):RGB(162,94,14);
        }
    }
    return CDRF_NEWFONT;
}
