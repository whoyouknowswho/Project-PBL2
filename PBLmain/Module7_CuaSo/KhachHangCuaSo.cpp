#include "CuaSo.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
wstring tenHang(const string& hang) {
    if(hang=="Bac") return L"Bạc";
    if(hang=="Vang") return L"Vàng";
    if(hang=="Kim Cuong") return L"Kim cương";
    return L"Thường";
}
}

void CuaSoNhaHang::trangChuKhachHang() {
    const KhachHang& khach=dichVu.hoSoCuaToi();
    nut(L"Đặt món",DAT_MON,264,266,180,1);
    nut(L"Thông tin của tôi",HO_SO_CUA_TOI,460,266,190);
    nut(L"Hóa đơn của tôi",HOA_DON_CUA_TOI,666,266,190);
    nhan(L"Lịch sử mua hàng · "+rong(khach.getTen()),264,324,rongCuaSo-290,true);
    bang(BANG_CHINH,264,360,rongCuaSo-290,caoCuaSo-416,
         {{L"Mã hóa đơn",260},{L"Ngày lập",150},{L"Thanh toán",180}});
    hoaDonBaoCao=dichVu.hoaDonCuaToi();
    reverse(hoaDonBaoCao.begin(),hoaDonBaoCao.end());
    for(size_t i=0;i<hoaDonBaoCao.size();i++) {
        const HoaDon& h=hoaDonBaoCao[i];
        dongBang(BANG_CHINH,{rong(h.getMa()),rong(h.getNgayLap()),tien(h.tongTien())},static_cast<int>(i));
    }
    if(hoaDonBaoCao.empty()) thanhCong(L"Bạn chưa có hóa đơn. Chọn Đặt món để gửi đơn cho nhà hàng.");
    else thanhCong(L"Chọn Hóa đơn của tôi để xem chi tiết các lần mua hàng.");
}

void CuaSoNhaHang::trangHoSoKhachHang() {
    const KhachHang& k=dichVu.hoSoCuaToi();
    int x=290,w=(rongCuaSo-350)/2, phai=x+w+26;
    theBieuMau={264,130,rongCuaSo-24,560};
    nhan(L"Thông tin thành viên",x,152,rongCuaSo-340,true);
    nhap(L"Mã khách hàng",O_MA,x,206,w,rong(k.getMa()));
    nhap(L"Điểm tích lũy",O_DIEM,phai,206,w,so(k.getDiemTichLuy()));
    nhap(L"Họ và tên",O_TEN,x,282,w,rong(k.getTen()));
    nhap(L"Hạng thành viên",O_HANG,phai,282,w,tenHang(k.getHangVip()));
    nhap(L"Điện thoại",O_SDT,x,358,w,rong(k.getSoDienThoai()));
    nhap(L"Ưu đãi giảm giá",O_UUDAI,phai,358,w,so(k.getPhanTramGiam())+L"%");
    nhap(L"Giới tính",O_GIOITINH,x,434,w,rong(k.getGioiTinh()));
    nhap(L"Email",O_EMAIL,phai,434,w,rong(k.getEmail()));
    nhan(L"Liên hệ nhân viên nếu cần cập nhật thông tin.",x,510,rongCuaSo-340);
    for(int id:{O_MA,O_TEN,O_SDT,O_GIOITINH,O_EMAIL,O_DIEM,O_HANG,O_UUDAI}) {
        SendMessageW(o(id),EM_SETREADONLY,TRUE,0);
    }
}

void CuaSoNhaHang::trangLichSuKhachHang() {
    const KhachHang& k=dichVu.hoSoCuaToi();
    nhan(L"Khách hàng: "+rong(k.getMa()+" - "+k.getTen()),264,138,rongCuaSo-290,true);
    nut(L"Xem hóa đơn đã chọn",NUT_HOADON,264,186,230,1);
    bang(BANG_CHINH,264,256,rongCuaSo-290,caoCuaSo-312,
         {{L"Mã hóa đơn",280},{L"Ngày lập",150},{L"Số loại món",130},{L"Thanh toán",180}});
    hoaDonBaoCao=dichVu.hoaDonCuaToi();
    reverse(hoaDonBaoCao.begin(),hoaDonBaoCao.end());
    for(size_t i=0;i<hoaDonBaoCao.size();i++) {
        const HoaDon& h=hoaDonBaoCao[i];
        dongBang(BANG_CHINH,{rong(h.getMa()),rong(h.getNgayLap()),to_wstring(h.getGioHang().size()),tien(h.tongTien())},static_cast<int>(i));
    }
    EnableWindow(o(NUT_HOADON),!hoaDonBaoCao.empty());
    thanhCong(L"Có "+to_wstring(hoaDonBaoCao.size())+L" hóa đơn thuộc tài khoản của bạn.");
}

string CuaSoNhaHang::vaiTroDuocChon() const {
    int vaiTro=luaChon(O_ROLE);
    if(vaiTro==0) return "NHANVIEN";
    if(vaiTro==1) return "QUANLY";
    if(vaiTro==2) return "KHACHHANG";
    throw runtime_error("Hãy chọn vai trò tài khoản.");
}

void CuaSoNhaHang::capNhatHoSoTaiKhoan() {
    string vaiTro=vaiTroDuocChon();
    SendMessageW(o(O_NHANVIEN),CB_RESETCONTENT,0,0);
    vector<wstring> ds;
    if(vaiTro=="NHANVIEN") {
        for(const NhanVien& n:dichVu.nhanVien()) ds.push_back(rong(n.getMa()+" - "+n.getTen()));
    } else if(vaiTro=="KHACHHANG") {
        for(const KhachHang& k:dichVu.khachHang()) ds.push_back(rong(k.getMa()+" - "+k.getTen()));
    } else ds.push_back(L"Không cần liên kết hồ sơ");
    for(const wstring& muc:ds) SendMessageW(o(O_NHANVIEN),CB_ADDSTRING,0,reinterpret_cast<LPARAM>(muc.c_str()));
    if(!ds.empty()) chonMuc(O_NHANVIEN,0);
    EnableWindow(o(O_NHANVIEN),vaiTro!="QUANLY");
}

string CuaSoNhaHang::maLienKetDuocChon() const {
    string vaiTro=vaiTroDuocChon();
    if(vaiTro=="QUANLY") return "";
    int i=luaChon(O_NHANVIEN);
    if(i<0) throw runtime_error("Chưa có hồ sơ liên kết. Hãy thêm khách hàng hoặc nhân viên trước.");
    if(vaiTro=="NHANVIEN") return dichVu.nhanVien().at(i).getMa();
    return dichVu.khachHang().at(i).getMa();
}
