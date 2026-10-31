#include "CuaSo.h"
#include "../Module2_ThucDon/MonAn.h"
#include "../Module2_ThucDon/NuocUong.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
wstring chuThuong(wstring s) {
    if (!s.empty()) CharLowerBuffW(&s[0], static_cast<DWORD>(s.size()));
    return s;
}
bool khop(const string& s, const wstring& tuKhoa) { return chuThuong(rong(s)).find(chuThuong(tuKhoa)) != wstring::npos; }
}

void CuaSoNhaHang::trangDangNhap() {
    int x=rongCuaSo/2+64, w=rongCuaSo/2-128;
    int y=max(60,(caoCuaSo-492)/2+28);
    theBieuMau={x-32,y-28,x+w+32,y+456};
    HWND tieuDe=tao(L"STATIC",thietLap?L"Thiết lập lần đầu":L"Chào mừng trở lại",SS_LEFT,-1,x,y,w,44);
    SendMessageW(tieuDe,WM_SETFONT,reinterpret_cast<WPARAM>(fontLon),TRUE);
    nhan(thietLap?L"Tạo tài khoản quản lý để bắt đầu.":L"Đăng nhập để tiếp tục.",x,y+50,w);
    nhap(L"Tên đăng nhập",O_USER,x,y+108,w);
    nhap(L"Mật khẩu",O_PASS,x,y+188,w,L"",true);
    if(thietLap) nhap(L"Nhập lại mật khẩu",O_CONFIRM,x,y+268,w,L"",true);
    else nhan(L"Khách mới có thể tự đăng ký tài khoản bên dưới.",x,y+272,w);
    nut(thietLap?L"Tạo tài khoản quản lý":L"Đăng nhập",NUT_DANGNHAP,x,y+348,w,1);
    if(thietLap) nhan(L"Nhấn Enter hoặc chọn nút để tiếp tục.",x,y+410,w);
    else nut(L"Đăng ký khách hàng",NUT_MO_DANGKY,x,y+400,w);
    SetFocus(o(O_USER));
}

void CuaSoNhaHang::trangChu() {
    if(dichVu.laKhachHang()) { trangChuKhachHang(); return; }
    if(dichVu.laQuanLy()) {
        nut(L"Quản lý thực đơn",THUC_DON,264,266,210,1);
        nut(L"Xem báo cáo",BAO_CAO,490,266,180);
        trangBieuDoDoanhThu();
        return;
    } else {
        nut(L"+ Tạo đơn bán hàng",BAN_HANG,264,266,210,1);
        nut(L"Xem thực đơn",THUC_DON,490,266,180);
    }
    nhan(L"Tồn kho thấp · Cần kiểm tra và nhập thêm",264,324,600,true);
    bang(BANG_CHINH,264,358,rongCuaSo-290,caoCuaSo-414,
         {{L"Mã món",110},{L"Tên món",300},{L"Loại",180},{L"Còn lại",110},{L"Tình trạng",190}});
    for(size_t i=0;i<dichVu.thucDon().size();i++) {
        const Mon& mon=*dichVu.thucDon()[i];
        if(mon.getTonKho()<=5) dongBang(BANG_CHINH,{rong(mon.getMa()),rong(mon.getTen()),
            mon.getLoai()=="Mon an"?L"Món ăn":L"Nước uống",so(mon.getTonKho()),
            mon.getTonKho()==0?L"Hết hàng":L"Sắp hết"},static_cast<int>(i));
    }
}

void CuaSoNhaHang::trangDanhSach() {
    bool coBieuMau=trangHienTai!=THUC_DON || dichVu.laQuanLy();
    int bangW=rongCuaSo-290;
    int x=rongCuaSo-334, w=290;
    if(coBieuMau) { bangW-=354; theBieuMau={x-20,130,rongCuaSo-24,caoCuaSo-52}; }
    int timW=bangW;
    if(trangHienTai==THUC_DON) timW=bangW-180;
    nhap(trangHienTai==THUC_DON?L"Tìm món theo mã hoặc tên":L"Tìm theo mã, tên hoặc số điện thoại",TIM_KIEM,264,130,timW);
    if(trangHienTai==THUC_DON) {
        chon(L"Sắp xếp",SAP_XEP,264+bangW-168,130,168,{L"Mặc định",L"Tên A–Z",L"Giá tăng",L"Giá giảm"});
        int nua=(bangW-12)/2;
        nut(L"Thức ăn",NUT_NHOM_THUCAN,264,202,nua,nhomThucDon==0?1:0);
        nut(L"Nước uống",NUT_NHOM_NUOC,264+nua+12,202,bangW-nua-12,nhomThucDon==1?1:0);
        danhDau(L"Chỉ hiện món còn hàng",CON_HANG,264,252,250);
        int dau=292;
        HWND ds=tao(L"LISTBOX",L"",WS_TABSTOP|WS_VSCROLL|LBS_NOTIFY|LBS_OWNERDRAWFIXED|
            LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT,DANH_SACH_THUCDON,264,dau,bangW,caoCuaSo-dau-56);
        SendMessageW(ds,LB_SETITEMHEIGHT,0,160);
    } else {
        nhan(L"Chọn một dòng để xem và chỉnh sửa thông tin.",264,190,bangW);
        vector<pair<wstring,int>> cot;
        if(trangHienTai==KHACH_HANG) cot={{L"Mã KH",90},{L"Họ tên",200},{L"Điện thoại",130},{L"Giới tính",95},{L"Điểm",75},{L"Hạng VIP",105}};
        else cot={{L"Mã NV",90},{L"Họ tên",185},{L"Điện thoại",125},{L"Chức vụ",140},{L"Tổng lương",140}};
        bang(BANG_CHINH,264,226,bangW,caoCuaSo-282,cot);
    }
    if(!coBieuMau) return;
    nhan(L"Thông tin chi tiết",x,144,w,true);
    if(trangHienTai==THUC_DON) {
        nhap(L"Mã món",O_MA,x,182,134);
        chon(L"Loại",O_LOAI,x+148,182,142,{L"Món ăn",L"Nước uống"});
        nhap(L"Tên món",O_TEN,x,246,w);
        nhap(L"Giá bán (đ)",O_GIA,x,310,134,L"0");
        nhap(L"Tồn kho",O_TON,x+148,310,142,L"0");
        nhap(L"Độ cay (0–5)",O_CAY,x,374,134,L"0");
        chon(L"Size nước",O_SIZE,x+148,374,142,{L"S",L"M",L"L"}); chonMuc(O_SIZE,1);
        danhDau(L"Món chay",O_CHAY,x,442,134);
        danhDau(L"Có đá",O_DA,x+148,442,142,true);
    } else {
        nhap(trangHienTai==KHACH_HANG?L"Mã khách hàng":L"Mã nhân viên",O_MA,x,182,w);
        nhap(L"Họ và tên",O_TEN,x,246,w);
        nhap(L"Điện thoại",O_SDT,x,310,148);
        chon(L"Giới tính",O_GIOITINH,x+160,310,130,{L"Nam",L"Nữ",L"Khác"});
        if(trangHienTai==KHACH_HANG) nhap(L"Điểm tích lũy",O_DIEM,x,374,w,L"0");
        else {
            nhap(L"Chức vụ",O_CHUCVU,x,374,w);
            nhap(L"Lương cơ bản (đ)",O_LUONG,x,438,140,L"0");
            nhap(L"Phụ cấp (đ)",O_PHUCAP,x+152,438,138,L"0");
        }
    }
    int y=trangHienTai==THUC_DON?486:518;
    nut(L"Lưu thông tin",NUT_LUU,x,y,178,1);
    nut(L"Xóa",NUT_XOA,x+190,y,100,2);
    nut(L"+ Thêm mới / Làm trống",NUT_MOI,x,y+50,w);
    if(trangHienTai==THUC_DON) {
        nhap(L"Nhập thêm tồn kho",O_NHAPKHO,x,586,130,L"1");
        nut(L"Nhập kho",NUT_NHAPKHO,x+144,610,146);
    } else nhan(L"Mã được giữ cố định khi sửa bản ghi.",x,632,w);
    EnableWindow(o(NUT_XOA),!maDangSua.empty());
}

void CuaSoNhaHang::doiLoaiMon() {
    if(trangHienTai!=THUC_DON || !dichVu.laQuanLy()) return;
    bool monAn=luaChon(O_LOAI)==0;
    EnableWindow(o(O_CAY),monAn); EnableWindow(o(O_CHAY),monAn);
    EnableWindow(o(O_SIZE),!monAn); EnableWindow(o(O_DA),!monAn);
    EnableWindow(o(NUT_NHAPKHO),!maDangSua.empty());
}

void CuaSoNhaHang::napBang() {
    if(trangHienTai==THUC_DON) { napThucDonAnh(); return; }
    bool cu=dangVeBang; dangVeBang=true;
    ListView_DeleteAllItems(o(BANG_CHINH));
    wstring tuKhoa=chu(TIM_KIEM);
    if(trangHienTai==KHACH_HANG) {
        const vector<KhachHang>& ds=dichVu.khachHang();
        for(size_t i=0;i<ds.size();i++) {
            const KhachHang& k=ds[i];
            if(!khop(k.getMa()+" "+k.getTen()+" "+k.getSoDienThoai(),tuKhoa)) continue;
            dongBang(BANG_CHINH,{rong(k.getMa()),rong(k.getTen()),rong(k.getSoDienThoai()),rong(k.getGioiTinh()),so(k.getDiemTichLuy()),rong(k.getHangVip())},static_cast<int>(i));
        }
    } else if(trangHienTai==NHAN_VIEN) {
        const vector<NhanVien>& ds=dichVu.nhanVien();
        for(size_t i=0;i<ds.size();i++) {
            const NhanVien& n=ds[i];
            if(!khop(n.getMa()+" "+n.getTen()+" "+n.getSoDienThoai(),tuKhoa)) continue;
            dongBang(BANG_CHINH,{rong(n.getMa()),rong(n.getTen()),rong(n.getSoDienThoai()),rong(n.getChucVu()),tien(n.tinhLuong())},static_cast<int>(i));
        }
    }
    dangVeBang=cu;
    thanhCong(L"Có "+to_wstring(ListView_GetItemCount(o(BANG_CHINH)))+L" kết quả. Dữ liệu được lưu khi nhấn Lưu.");
}

void CuaSoNhaHang::chonDong() {
    int viTri=trangHienTai==THUC_DON?monThucDonDaChon():dongChon(BANG_CHINH);
    if(viTri<0 || (trangHienTai==THUC_DON && !dichVu.laQuanLy())) return;
    if(trangHienTai==KHACH_HANG || trangHienTai==NHAN_VIEN) {
        const Nguoi* nguoi=nullptr;
        if(trangHienTai==KHACH_HANG) {
            const KhachHang& k=dichVu.khachHang().at(viTri); nguoi=&k; gan(O_DIEM,so(k.getDiemTichLuy()));
        } else {
            const NhanVien& n=dichVu.nhanVien().at(viTri); nguoi=&n;
            gan(O_CHUCVU,rong(n.getChucVu())); gan(O_LUONG,so(n.getLuongCoBan())); gan(O_PHUCAP,so(n.getPhuCap()));
        }
        maDangSua=nguoi->getMa();
        gan(O_MA,rong(maDangSua)); gan(O_TEN,rong(nguoi->getTen())); gan(O_SDT,rong(nguoi->getSoDienThoai()));
        int gt=2;
        if(nguoi->getGioiTinh()=="Nam") gt=0;
        if(nguoi->getGioiTinh()=="Nu" || nguoi->getGioiTinh()=="Nữ") gt=1;
        chonMuc(O_GIOITINH,gt);
    } else if(trangHienTai==THUC_DON) {
        const Mon& m=*dichVu.thucDon().at(viTri); maDangSua=m.getMa();
        gan(O_MA,rong(maDangSua)); gan(O_TEN,rong(m.getTen())); gan(O_GIA,so(m.getGia())); gan(O_TON,so(m.getTonKho()));
        const MonAn* a=dynamic_cast<const MonAn*>(&m);
        if(a) {
            chonMuc(O_LOAI,0); gan(O_CAY,so(a->getDoCay())); SendMessageW(o(O_CHAY),BM_SETCHECK,a->laMonChay()?BST_CHECKED:BST_UNCHECKED,0);
        } else {
            const NuocUong& n=dynamic_cast<const NuocUong&>(m); chonMuc(O_LOAI,1);
            int size=1; if(n.getSizeLy()=="S") size=0; if(n.getSizeLy()=="L") size=2;
            chonMuc(O_SIZE,size); SendMessageW(o(O_DA),BM_SETCHECK,n.getCoDa()?BST_CHECKED:BST_UNCHECKED,0);
        }
        doiLoaiMon();
    } else if(trangHienTai==BAO_CAO) {
        EnableWindow(o(NUT_HOADON),true); return;
    } else if(trangHienTai==DON_DAT) {
        chiTietDonDat(); return;
    } else if(trangHienTai==BAN_HANG || trangHienTai==DAT_MON) {
        const Mon& mon=*dichVu.thucDon().at(viTri);
        bool nuoc=mon.getLoai()=="Nuoc uong";
        EnableWindow(o(O_SIZE),nuoc); EnableWindow(o(O_DA),nuoc);
        return;
    } else return;
    SendMessageW(o(O_MA),EM_SETREADONLY,TRUE,0);
    EnableWindow(o(NUT_XOA),TRUE);
}

void CuaSoNhaHang::luuBieuMau() {
    bool themMoi=maDangSua.empty();
    string ma=duLieu(O_MA);
    if(!themMoi && ma!=maDangSua) throw runtime_error("Không thể thay đổi mã của bản ghi đang sửa.");
    if(trangHienTai==KHACH_HANG) {
        int diem=static_cast<int>(soNhap(O_DIEM,"Điểm tích lũy",true));
        KhachHang k(ma,duLieu(O_TEN),duLieu(O_SDT),duLieu(O_GIOITINH),diem);
        dichVu.luuKhachHang(k,themMoi);
    } else if(trangHienTai==NHAN_VIEN) {
        double luong=soNhap(O_LUONG,"Lương"),phuCap=soNhap(O_PHUCAP,"Phụ cấp");
        NhanVien n(ma,duLieu(O_TEN),duLieu(O_SDT),duLieu(O_GIOITINH),duLieu(O_CHUCVU),luong,phuCap);
        dichVu.luuNhanVien(n,themMoi);
    } else if(trangHienTai==THUC_DON) {
        DuLieuMon m; m.ma=ma; m.ten=duLieu(O_TEN); m.gia=soNhap(O_GIA,"Giá"); m.tonKho=static_cast<int>(soNhap(O_TON,"Tồn kho",true));
        if(luaChon(O_LOAI)==0) { m.loai="Mon an"; m.doCay=static_cast<int>(soNhap(O_CAY,"Độ cay",true)); }
        else m.loai="Nuoc uong";
        m.sizeLy=duLieu(O_SIZE); m.monChay=daDanhDau(O_CHAY); m.coDa=daDanhDau(O_DA);
        dichVu.luuMon(m,themMoi);
    }
    maDangSua=ma; napBang();
    SendMessageW(o(O_MA),EM_SETREADONLY,TRUE,0); EnableWindow(o(NUT_XOA),TRUE); doiLoaiMon();
    thanhCong(L"Đã lưu thông tin thành công.");
}

void CuaSoNhaHang::xoaBanGhi() {
    if(maDangSua.empty()) throw runtime_error("Chọn một dòng trong bảng trước khi xóa.");
    if(!xacNhan(L"Xóa bản ghi "+rong(maDangSua)+L"? Thao tác này không thể hoàn tác.")) return;
    if(trangHienTai==KHACH_HANG) dichVu.xoaKhachHang(maDangSua);
    if(trangHienTai==NHAN_VIEN) dichVu.xoaNhanVien(maDangSua);
    if(trangHienTai==THUC_DON) dichVu.xoaMon(maDangSua);
    hienTrang(trangHienTai); thanhCong(L"Đã xóa bản ghi.");
}

void CuaSoNhaHang::trangTaiKhoan() {
    int x=290,w=440;
    theBieuMau={264,130,780,caoCuaSo-60};
    nhan(L"Cấp tài khoản mới",x,154,w,true);
    nhap(L"Tên đăng nhập",O_USER,x,200,w);
    chon(L"Vai trò",O_ROLE,x,268,w,{L"Nhân viên",L"Quản lý",L"Khách hàng"});
    chon(L"Hồ sơ liên kết (nhân viên hoặc khách hàng)",O_NHANVIEN,x,336,w,{});
    capNhatHoSoTaiKhoan();
    nhap(L"Mật khẩu (8–128 byte)",O_PASS,x,404,w,L"",true);
    nhap(L"Nhập lại mật khẩu",O_CONFIRM,x,472,w,L"",true);
    nut(L"Tạo tài khoản",NUT_TAOTAIKHOAN,x,554,w,1);
    nhan(L"Tên gồm chữ không dấu, chữ số, _ hoặc - (tối đa 32).",x,614,w);
}
void CuaSoNhaHang::trangMatKhau() {
    int x=290,w=440;
    theBieuMau={264,130,780,570};
    nhan(L"Bảo vệ tài khoản của bạn",x,154,w,true);
    nhap(L"Mật khẩu hiện tại",O_OLDPASS,x,206,w,L"",true);
    nhap(L"Mật khẩu mới",O_PASS,x,280,w,L"",true);
    nhap(L"Nhập lại mật khẩu mới",O_CONFIRM,x,354,w,L"",true);
    nut(L"Cập nhật mật khẩu",NUT_DOIMATKHAU,x,446,w,1);
    nhan(L"Mật khẩu mới từ 8 đến 128 byte.",x,510,w);
}

void CuaSoNhaHang::xuLyNut(int id,int suKien) {
    if(id==DANH_SACH_THUCDON && suKien==LBN_SELCHANGE) { chonDong(); return; }
    if(id==LOC_NAM_DOANHTHU && suKien==CBN_SELCHANGE) { napBieuDoDoanhThu(); return; }
    if(id==DANH_SACH_MON && suKien==LBN_SELCHANGE) { chonMonDat(); return; }
    if(id==DANH_SACH_MON && suKien==LBN_DBLCLK) { themVaoGio(); return; }
    if(suKien==EN_CHANGE && (id==TIM_KIEM)) {
        if(trangHienTai==BAN_HANG || trangHienTai==DAT_MON) napBanHang(); else napBang(); return;
    }
    if(suKien==CBN_SELCHANGE && id==SAP_XEP) { napBang(); return; }
    if(suKien==CBN_SELCHANGE && id==O_LOAI) { doiLoaiMon(); return; }
    if(suKien==CBN_SELCHANGE && id==O_KHACH) { napGio(); return; }
    if(suKien==CBN_SELCHANGE && id==O_ROLE) { capNhatHoSoTaiKhoan(); return; }
    if(suKien!=BN_CLICKED) return;
    if(!dichVu.daDangNhap() && id==NUT_MO_DANGKY && !thietLap) {
        dangDangKy=true;
        hienTrang(TRANG_CHU);
        return;
    }
    if(!dichVu.daDangNhap() && id==NUT_VE_DANGNHAP) {
        dangDangKy=false;
        hienTrang(TRANG_CHU);
        return;
    }
    if(!dichVu.daDangNhap() && id==NUT_DANGKY && dangDangKy && !thietLap) {
        string ten=duLieu(O_USER);
        string ma=dichVu.dangKyKhachHang(duLieu(O_TEN),duLieu(O_SDT),duLieu(O_GIOITINH),
                                        ten,duLieu(O_PASS),duLieu(O_CONFIRM));
        dangDangKy=false;
        soLanSai=0;
        hienTrang(TRANG_CHU);
        gan(O_USER,rong(ten));
        SetFocus(o(O_PASS));
        wstring noiDung=L"Đăng ký thành công! Mã khách hàng của bạn: "+rong(ma)+
                       L".\nHãy đăng nhập bằng tài khoản vừa tạo.";
        MessageBoxW(cuaSo,noiDung.c_str(),L"Đăng ký thành công",MB_OK|MB_ICONINFORMATION);
        return;
    }
    if(id==NUT_DANGNHAP && !dichVu.daDangNhap() && !dangDangKy) {
        if(thietLap) {
            string ten=duLieu(O_USER);
            dichVu.taoTaiKhoan(ten,duLieu(O_PASS),duLieu(O_CONFIRM),"QUANLY","");
            thietLap=false; hienTrang(TRANG_CHU); gan(O_USER,rong(ten)); SetFocus(o(O_PASS));
            MessageBoxW(cuaSo,L"Đã tạo tài khoản quản lý. Hãy đăng nhập để tiếp tục.",L"Thành công",MB_OK|MB_ICONINFORMATION);
        } else {
            string ten=duLieu(O_USER);
            if(!dichVu.dangNhap(ten,duLieu(O_PASS))) {
                soLanSai++; gan(O_PASS,L"");
                if(soLanSai>=3) { baoLoi("Đăng nhập sai 3 lần. Hãy mở lại ứng dụng để thử lại."); DestroyWindow(cuaSo); }
                else { baoLoi("Sai tài khoản, mật khẩu hoặc hồ sơ liên kết không còn tồn tại."); SetFocus(o(O_PASS)); }
                return;
            }
            soLanSai=0; thongBao=L"Xin chào, "+rong(ten)+L"!";
            hienTrang(TRANG_CHU);
        }
        return;
    }
    if(!dichVu.daDangNhap()) return;
    if((id>=TRANG_CHU && id<=MAT_KHAU) || id==HO_SO_CUA_TOI || id==HOA_DON_CUA_TOI || id==DAT_MON || id==DON_DAT) { hienTrang(id); return; }
    if(id==DANG_XUAT) {
        if(!xacNhan(gioHang.empty()?L"Bạn muốn đăng xuất?":L"Giỏ hàng chưa thanh toán sẽ bị hủy. Bạn muốn đăng xuất?")) return;
        gioHang.clear(); hoaDonBaoCao.clear(); maHoaDon.clear(); ngayDon.clear(); khachDon.clear(); nhanVienDon.clear();
        maMonDangChon.clear(); nhomDatMon=0; nhomThucDon=0;
        dichVu.dangXuat(); hienTrang(TRANG_CHU); return;
    }
    switch(id) {
        case NUT_NHOM_THUCAN: case NUT_NHOM_NUOC:
            if(trangHienTai==THUC_DON) {
                nhomThucDon=id==NUT_NHOM_NUOC?1:0;
                gan(TIM_KIEM,L""); napThucDonAnh();
            } else {
                nhomDatMon=id==NUT_NHOM_NUOC?1:0;
                maMonDangChon.clear(); gan(TIM_KIEM,L""); napMonDat();
            }
            break;
        case NUT_GUI_DON: case NUT_HUY_DON: case NUT_THANHTOAN_DON: case NUT_LAMMOI_DON:
            xuLyDonDat(id); break;
        case NUT_LUU: luuBieuMau(); break;
        case NUT_MOI: hienTrang(trangHienTai); SetFocus(o(O_MA)); break;
        case NUT_XOA: xoaBanGhi(); break;
        case CON_HANG: napBang(); break;
        case NUT_NHAPKHO: {
            dichVu.nhapKho(maDangSua,static_cast<int>(soNhap(O_NHAPKHO,"Số lượng nhập",true)));
            string ma=maDangSua; hienTrang(THUC_DON); thanhCong(L"Đã nhập thêm tồn kho cho "+rong(ma)); break;
        }
        case NUT_TAOTAIKHOAN:
            dichVu.taoTaiKhoan(duLieu(O_USER),duLieu(O_PASS),duLieu(O_CONFIRM),vaiTroDuocChon(),maLienKetDuocChon());
            hienTrang(TAI_KHOAN); thanhCong(L"Đã tạo tài khoản thành công."); break;
        case NUT_DOIMATKHAU:
            dichVu.doiMatKhau(duLieu(O_OLDPASS),duLieu(O_PASS),duLieu(O_CONFIRM));
            hienTrang(MAT_KHAU); thanhCong(L"Đã đổi mật khẩu thành công."); break;
        case NUT_THEM_GIO: themVaoGio(); break;
        case NUT_XOA_GIO: {
            int i=dongChon(BANG_GIO);
            if(i<0) throw runtime_error("Chọn món trong giỏ cần xóa.");
            gioHang.erase(gioHang.begin()+i); napGio(); if(trangHienTai==DAT_MON) napMonDat(); break;
        }
        case NUT_HUY_GIO:
            if(!gioHang.empty() && xacNhan(L"Hủy toàn bộ giỏ hàng?")) { gioHang.clear(); napGio(); if(trangHienTai==DAT_MON) napMonDat(); }
            break;
        case NUT_THANHTOAN: thanhToan(); break;
        case NUT_BC_HOMNAY: gan(LOC_BAO_CAO,rong(homNay())); napBaoCao(); break;
        case NUT_BC_THANG: gan(LOC_BAO_CAO,rong(homNay().substr(3))); napBaoCao(); break;
        case NUT_BC_TATCA: gan(LOC_BAO_CAO,L""); napBaoCao(); break;
        case NUT_BAOCAO: napBaoCao(); break;
        case NUT_HOADON: {
            int i=dongChon(BANG_CHINH);
            if(i<0) throw runtime_error("Chọn một hóa đơn trong danh sách.");
            xemHoaDon(cuaSo,dichVu.hoaDonDuocXem(hoaDonBaoCao.at(i).getMa())); break;
        }
    }
}
