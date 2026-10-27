#include "CuaSo.h"
#include <stdexcept>

using namespace std;

string CuaSoNhaHang::khachDaChon() const {
    int i=luaChon(O_KHACH)-1;
    if(i>=0 && static_cast<size_t>(i)<dichVu.khachHang().size()) return dichVu.khachHang()[i].getMa();
    return "";
}
string CuaSoNhaHang::nhanVienDaChon() const {
    if(!dichVu.laQuanLy()) return dichVu.phien().maNhanVien;
    int i=luaChon(O_NHANVIEN);
    if(i>=0 && static_cast<size_t>(i)<dichVu.nhanVien().size()) return dichVu.nhanVien()[i].getMa();
    return "";
}

void CuaSoNhaHang::trangBanHang() {
    int w=(rongCuaSo-314)/2, x=264, phai=x+w+24;
    if(maHoaDon.empty()) maHoaDon=maHoaDonMoi();
    if(ngayDon.empty()) ngayDon=homNay();
    int fieldW=(rongCuaSo-326)/4;
    nhap(L"Mã hóa đơn (tự động)",O_MA,x,130,fieldW,rong(maHoaDon));
    SendMessageW(o(O_MA),EM_SETREADONLY,TRUE,0);
    nhap(L"Ngày lập (dd/mm/yyyy)",O_NGAY,x+fieldW+12,130,fieldW,rong(ngayDon));
    vector<wstring> kh={L"Khách lẻ"};
    int chonKhach=0, i=1;
    for(const KhachHang& k:dichVu.khachHang()) {
        kh.push_back(rong(k.getMa()+" - "+k.getTen()));
        if(k.getMa()==khachDon) chonKhach=i;
        i++;
    }
    chon(L"Khách hàng",O_KHACH,x+2*(fieldW+12),130,fieldW,kh); chonMuc(O_KHACH,chonKhach);
    if(dichVu.laQuanLy()) {
        vector<wstring> nv;
        int chonNV=0; i=0;
        for(const NhanVien& n:dichVu.nhanVien()) {
            nv.push_back(rong(n.getMa()+" - "+n.getTen()));
            if(n.getMa()==nhanVienDon) chonNV=i;
            i++;
        }
        chon(L"Nhân viên bán hàng",O_NHANVIEN,x+3*(fieldW+12),130,fieldW,nv); chonMuc(O_NHANVIEN,chonNV);
    } else {
        nhap(L"Nhân viên bán hàng",O_NHANVIEN,x+3*(fieldW+12),130,fieldW,rong(dichVu.phien().maNhanVien));
        SendMessageW(o(O_NHANVIEN),EM_SETREADONLY,TRUE,0);
    }
    nhap(L"Tìm món theo tên hoặc mã",TIM_KIEM,x,204,w-106);
    nhap(L"Số lượng",O_SOLUONG,x+w-94,204,94,L"1");
    chon(L"Size (nước uống)",O_SIZE,x,270,160,{L"S",L"M",L"L"}); chonMuc(O_SIZE,1);
    danhDau(L"Có đá",O_DA,x+180,298,110,true);
    EnableWindow(o(O_SIZE),FALSE); EnableWindow(o(O_DA),FALSE);
    bang(BANG_CHINH,x,344,w,caoCuaSo-454,{{L"Mã",80},{L"Tên món",175},{L"Giá bán",110},{L"Còn lại",80}});
    nut(L"+ Thêm món đã chọn",NUT_THEM_GIO,x,caoCuaSo-96,w,1);
    nhan(L"Giỏ hàng",phai,208,w,true);
    bang(BANG_GIO,phai,244,w,caoCuaSo-480,{{L"Món đã chọn",225},{L"SL",55},{L"Thành tiền",145}});
    tao(L"STATIC",L"",SS_LEFT,O_TONG,phai,caoCuaSo-216,w,64);
    nut(L"Xóa món",NUT_XOA_GIO,phai,caoCuaSo-146,(w-12)/2,2);
    nut(L"Hủy giỏ hàng",NUT_HUY_GIO,phai+(w+12)/2,caoCuaSo-146,(w-12)/2);
    nut(L"Thanh toán & lưu hóa đơn",NUT_THANHTOAN,phai,caoCuaSo-96,w,1);
}

void CuaSoNhaHang::napBanHang() {
    bool cu=dangVeBang; dangVeBang=true;
    ListView_DeleteAllItems(o(BANG_CHINH));
    wstring tuKhoa=chu(TIM_KIEM);
    if(!tuKhoa.empty()) CharLowerBuffW(&tuKhoa[0],static_cast<DWORD>(tuKhoa.size()));
    for(size_t i=0;i<dichVu.thucDon().size();i++) {
        const Mon& mon=*dichVu.thucDon()[i];
        wstring ten=rong(mon.getMa()+" "+mon.getTen());
        if(!ten.empty()) CharLowerBuffW(&ten[0],static_cast<DWORD>(ten.size()));
        if(ten.find(tuKhoa)==wstring::npos) continue;
        int trongGio=0;
        for(const YeuCauMon& muc:gioHang) if(muc.maMon==mon.getMa()) trongGio+=muc.soLuong;
        dongBang(BANG_CHINH,{rong(mon.getMa()),rong(mon.getTen()),tien(mon.getGia()),so(mon.getTonKho()-trongGio)},static_cast<int>(i));
    }
    dangVeBang=cu;
}

void CuaSoNhaHang::napGio() {
    ListView_DeleteAllItems(o(BANG_GIO));
    double tamTinh=0,phanTram=0;
    for(size_t i=0;i<gioHang.size();i++) {
        const YeuCauMon& muc=gioHang[i];
        for(const unique_ptr<Mon>& mon:dichVu.thucDon()) {
            if(mon->getMa()!=muc.maMon) continue;
            string ten=mon->getTen();
            if(mon->getLoai()=="Nuoc uong") {
                ten+=" ("+muc.sizeLy;
                ten+=muc.coDa?", có đá)":", không đá)";
            }
            double thanhTien=mon->getGia()*muc.soLuong; tamTinh+=thanhTien;
            dongBang(BANG_GIO,{rong(ten),so(muc.soLuong),tien(thanhTien)},static_cast<int>(i));
        }
    }
    string kh=khachDaChon();
    for(const KhachHang& k:dichVu.khachHang()) if(k.getMa()==kh) phanTram=k.getPhanTramGiam();
    double giam=tamTinh*phanTram/100;
    gan(O_TONG,L"Tạm tính: "+tien(tamTinh)+L"   •   Giảm: "+tien(giam)+L"\r\nThanh toán: "+tien(tamTinh-giam));
    EnableWindow(o(NUT_THANHTOAN),!gioHang.empty());
    EnableWindow(o(NUT_XOA_GIO),!gioHang.empty());
    EnableWindow(o(NUT_HUY_GIO),!gioHang.empty());
}

void CuaSoNhaHang::themVaoGio() {
    int i=dongChon(BANG_CHINH);
    if(i<0) throw runtime_error("Chọn một món trong bảng bên trái trước.");
    const Mon& mon=*dichVu.thucDon().at(i);
    int sl=static_cast<int>(soNhap(O_SOLUONG,"Số lượng",true));
    int daChon=0;
    for(const YeuCauMon& muc:gioHang) if(muc.maMon==mon.getMa()) daChon+=muc.soLuong;
    if(sl<1 || sl>mon.getTonKho()-daChon) throw runtime_error("Số lượng phải lớn hơn 0 và không vượt tồn kho còn lại.");
    YeuCauMon muc; muc.maMon=mon.getMa(); muc.soLuong=sl;
    if(mon.getLoai()=="Nuoc uong") { muc.sizeLy=duLieu(O_SIZE); muc.coDa=daDanhDau(O_DA); }
    else { muc.sizeLy=""; muc.coDa=false; }
    bool gop=false;
    for(YeuCauMon& cu:gioHang) {
        if(cu.maMon==muc.maMon && cu.sizeLy==muc.sizeLy && cu.coDa==muc.coDa) {
            cu.soLuong+=sl; gop=true; break;
        }
    }
    if(!gop) gioHang.push_back(muc);
    napGio(); napBanHang();
    thanhCong(L"Đã thêm "+rong(mon.getTen())+L" vào giỏ. Tồn kho chỉ trừ khi thanh toán.");
}

void CuaSoNhaHang::thanhToan() {
    string ma=duLieu(O_MA),ngay=duLieu(O_NGAY),kh=khachDaChon(),nv=nhanVienDaChon();
    HoaDon truoc=dichVu.xemTruocHoaDon(ma,ngay,kh,nv,gioHang);
    if(!xacNhan(L"Xác nhận thanh toán "+tien(truoc.tongTien())+L" và lưu hóa đơn?")) return;
    HoaDon daLuu=dichVu.thanhToan(ma,ngay,kh,nv,gioHang);
    gioHang.clear(); maHoaDon=maHoaDonMoi(); gan(O_MA,rong(maHoaDon));
    hienTrang(BAN_HANG); thanhCong(L"Đã thanh toán và lưu hóa đơn "+rong(daLuu.getMa()));
    xemHoaDon(cuaSo,daLuu);
}

void CuaSoNhaHang::trangBaoCao() {
    nhap(L"Phạm vi: dd/mm/yyyy, mm/yyyy hoặc để trống xem tất cả",LOC_BAO_CAO,264,130,430);
    nut(L"Xem báo cáo",NUT_BAOCAO,710,154,164,1);
    nut(L"Xem hóa đơn đã chọn",NUT_HOADON,264,240,230);
    tao(L"STATIC",L"",SS_LEFT,O_TONG,264,198,rongCuaSo-290,36);
    int w=rongCuaSo-290, trai=w*62/100;
    bang(BANG_CHINH,264,300,trai,caoCuaSo-356,{{L"Mã hóa đơn",220},{L"Ngày lập",110},{L"Thanh toán",150}});
    nhan(L"Tồn kho thấp (≤ 5)",264+trai+20,248,w-trai-20,true);
    bang(BANG_TON,264+trai+20,300,w-trai-20,caoCuaSo-356,{{L"Món",190},{L"Còn lại",80}});
    napBaoCao();
}
void CuaSoNhaHang::napBaoCao() {
    BaoCaoKinhDoanh bc=dichVu.baoCao(duLieu(LOC_BAO_CAO));
    hoaDonBaoCao=bc.hoaDon;
    bool cu=dangVeBang; dangVeBang=true;
    ListView_DeleteAllItems(o(BANG_CHINH)); ListView_DeleteAllItems(o(BANG_TON));
    for(size_t i=0;i<hoaDonBaoCao.size();i++) {
        const HoaDon& hd=hoaDonBaoCao[i];
        dongBang(BANG_CHINH,{rong(hd.getMa()),rong(hd.getNgayLap()),tien(hd.tongTien())},static_cast<int>(i));
    }
    for(const unique_ptr<Mon>& mon:dichVu.thucDon()) if(mon->getTonKho()<=5) dongBang(BANG_TON,{rong(mon->getTen()),so(mon->getTonKho())},0);
    gan(O_TONG,L"Hóa đơn: "+so(bc.soHoaDon)+L"     •     Doanh thu: "+tien(bc.doanhThu)+L"     •     Bán chạy: "+rong(bc.monBanChay));
    dangVeBang=cu;
}
