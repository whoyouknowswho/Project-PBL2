#include "../Module7_CuaSo/DichVuNhaHang.h"
#include <windows.h>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>

using namespace std;

void phaiBaoLoi(const function<void()>& thaoTac) {
    bool coLoi=false;
    try { thaoTac(); } catch(const exception&) { coLoi=true; }
    assert(coLoi);
}

int main() {
    // Chi chay trong thu muc fixture rieng do script kiem tra tao.
    assert(GetFileAttributesA("fixture-gui-test")!=INVALID_FILE_ATTRIBUTES);
    DichVuNhaHang dv;
    assert(dv.khoiTao());
    phaiBaoLoi([&]{dv.khachHang();});
    phaiBaoLoi([&]{dv.thucDon();});
    phaiBaoLoi([&]{dv.taoTaiKhoan("qa","Password123","Password123",false,"NV_TEST");});
    phaiBaoLoi([&]{dv.taoTaiKhoan("qa","short","short",true,"");});
    dv.taoTaiKhoan("qa","Password123","Password123",true,"");
    assert(!dv.dangNhap("qa","wrong"));
    assert(dv.dangNhap("qa","Password123"));
    dv.luuNhanVien(NhanVien("NV_TEST","Nhan vien thu","090123","Nam","Thu ngan",100,20),true);
    dv.taoTaiKhoan("staff","StaffPass123","StaffPass123",false,"NV_TEST");
    dv.luuKhachHang(KhachHang("KH_TEST","Khach thu","090456","Nu",200),true);
    phaiBaoLoi([&]{dv.luuKhachHang(KhachHang("KH_TEST","Trung ma","090456","Nu",0),true);});
    phaiBaoLoi([&]{dv.luuKhachHang(KhachHang("KH_BAD","Ten|sai","090456","Nu",0),true);});
    DuLieuMon com; com.ma="MA_TEST"; com.ten="Cơm thử"; com.gia=45000; com.tonKho=10;
    dv.luuMon(com,true);
    DuLieuMon tra; tra.ma="NU_TEST"; tra.ten="Trà thử"; tra.loai="Nuoc uong"; tra.gia=25000; tra.tonKho=10;
    dv.luuMon(tra,true);
    dv.nhapKho("MA_TEST",2);
    assert(dv.thucDon()[0]->getTonKho()==12);
    assert(dv.dangNhap("staff","StaffPass123"));
    phaiBaoLoi([&]{dv.nhanVien();});
    phaiBaoLoi([&]{dv.baoCao("");});
    phaiBaoLoi([&]{dv.luuMon(com,false);});
    phaiBaoLoi([&]{dv.xoaMon("MA_TEST");});
    phaiBaoLoi([&]{dv.nhapKho("MA_TEST",1);});
    phaiBaoLoi([&]{dv.xoaNhanVien("NV_TEST");});
    phaiBaoLoi([&]{dv.taoTaiKhoan("hack","Password123","Password123",true,"");});
    dv.luuKhachHang(KhachHang("KH_TEST","Khách mới","090456","Nữ",200),false);
    vector<YeuCauMon> gio={{"NU_TEST","M",1,true},{"NU_TEST","L",2,false},{"MA_TEST","",1,false}};
    phaiBaoLoi([&]{dv.xemTruocHoaDon("HD_BAD","31/02/2026","KH_TEST","fake",gio);});
    vector<YeuCauMon> quaKho={{"NU_TEST","M",6,true},{"NU_TEST","L",5,false}};
    phaiBaoLoi([&]{dv.thanhToan("HD_BAD","27/10/2026","KH_TEST","fake",quaKho);});
    assert(dv.thucDon()[1]->getTonKho()==10);
    HoaDon truoc=dv.xemTruocHoaDon("HD_GUI","27/10/2026","KH_TEST","fake",gio);
    assert(abs(truoc.tongTien()-114000)<0.1);
    assert(dv.thucDon()[1]->getTonKho()==10);
    HoaDon hd=dv.thanhToan("HD_GUI","27/10/2026","KH_TEST","fake",gio);
    assert(hd.toFile().find("|NV_TEST|")!=string::npos);
    assert(hd.getGioHang().size()==3);
    assert(dv.thucDon()[0]->getTonKho()==11);
    assert(dv.thucDon()[1]->getTonKho()==7);
    assert(dv.khachHang()[0].getDiemTichLuy()==211);
    phaiBaoLoi([&]{dv.thanhToan("HD_GUI","27/10/2026","KH_TEST","fake",gio);});
    dv.doiMatKhau("StaffPass123","StaffNew123","StaffNew123");
    dv.dangXuat();
    assert(!dv.dangNhap("staff","StaffPass123"));
    assert(dv.dangNhap("staff","StaffNew123"));
    dv.dangXuat();
    phaiBaoLoi([&]{dv.taoTaiKhoan("bypass","Password123","Password123",true,"");});
    DichVuNhaHang moLai;
    assert(!moLai.khoiTao());
    assert(moLai.dangNhap("qa","Password123"));
    BaoCaoKinhDoanh bc=moLai.baoCao("10/2026");
    assert(bc.soHoaDon==1 && abs(bc.doanhThu-114000)<0.1);
    assert(bc.hoaDon[0].getGioHang().size()==3);
    assert(bc.hoaDon[0].getGioHang()[0].tenMon.find(", Co da)")!=string::npos);
    assert(moLai.baoCao("11/2026").soHoaDon==0);
    phaiBaoLoi([&]{moLai.baoCao("13/2026");});
    // Loi ghi tep: khong bao thanh cong va phuc hoi du lieu trong bo nho.
    assert(CreateDirectoryA("DuLieu/menu.txt.gui-new",nullptr));
    phaiBaoLoi([&]{moLai.luuKhachHang(KhachHang("KH_TEST","KHONG_DUOC_LUU","090456","Nu",0),false);});
    assert(moLai.khachHang()[0].getTen()=="Khách mới");
    assert(RemoveDirectoryA("DuLieu/menu.txt.gui-new"));
    moLai.xoaMon("NU_TEST");
    moLai.xoaKhachHang("KH_TEST");
    moLai.xoaNhanVien("NV_TEST");
    assert(!moLai.dangNhap("staff","StaffNew123"));
    cout<<"PASS: login, roles, CRUD, input validation, stock, discount, checkout, invoice reload, report, password, write failure rollback\n";
}
