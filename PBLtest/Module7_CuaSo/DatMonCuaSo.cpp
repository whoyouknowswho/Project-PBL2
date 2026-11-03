#include "CuaSo.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
wstring trangThaiDon(const string& trangThai) {
    if(trangThai=="XONG") return L"Đã thanh toán";
    if(trangThai=="HUY") return L"Đã hủy";
    return L"Chờ xử lý";
}
}


void CuaSoNhaHang::trangDonDat() {
    donDatHienThi=dichVu.danhSachDonDat();
    reverse(donDatHienThi.begin(),donDatHienThi.end());
    int x=264,w=rongCuaSo-290;
    nut(L"Làm mới",NUT_LAMMOI_DON,x,130,120);
    nut(L"Hủy đơn đã chọn",NUT_HUY_DON,x+134,130,180,2);
    if(!dichVu.laKhachHang()) {
        nut(L"Thanh toán đơn",NUT_THANHTOAN_DON,x+328,130,190,1);

    }
    nhan(L"Chọn đơn để xem món bên dưới. Chỉ đơn chờ mới được hủy hoặc thanh toán.",x,190,w);
    bang(BANG_CHINH,x,226,w,(caoCuaSo-370)/2,
         {{L"Mã đơn",95},{L"Ngày đặt",110},{L"Khách hàng",130},{L"Trạng thái",150},{L"Tạm tính sau giảm",170},{L"Hóa đơn",150}});
    int y=226+(caoCuaSo-370)/2+20;
    nhan(L"Chi tiết đơn đã chọn",x,y,w,true);
    bang(BANG_GIO,x,y+36,w,caoCuaSo-y-92,
         {{L"Món / size / đá",380},{L"Số lượng",90},{L"Đơn giá lúc đặt",155},{L"Thành tiền",155}});
    for(size_t i=0;i<donDatHienThi.size();i++) {
        const DonDatMon& don=donDatHienThi[i];
        dongBang(BANG_CHINH,{rong(don.duTinh.getMa()),rong(don.duTinh.getNgayLap()),
            rong(don.duTinh.getMaKhachHang()),trangThaiDon(don.trangThai),
            tien(don.duTinh.tongTien()),rong(don.maHoaDon)},static_cast<int>(i));
    }
    EnableWindow(o(NUT_HUY_DON),FALSE);
    EnableWindow(o(NUT_THANHTOAN_DON),FALSE);
    thanhCong(donDatHienThi.empty()?L"Chưa có đơn đặt món.":L"Đơn đã gửi được lưu lại khi đóng chương trình.");
}

void CuaSoNhaHang::chiTietDonDat() {
    int i=dongChon(BANG_CHINH);
    if(i<0 || static_cast<size_t>(i)>=donDatHienThi.size()) return;
    const DonDatMon& don=donDatHienThi[i];
    ListView_DeleteAllItems(o(BANG_GIO));
    for(const ChiTietHoaDon& ct:don.duTinh.getGioHang())
        dongBang(BANG_GIO,{rong(ct.tenMon),so(ct.soLuong),tien(ct.donGia),tien(ct.thanhTien())},0);
    EnableWindow(o(NUT_HUY_DON),don.trangThai=="CHO");
    EnableWindow(o(NUT_THANHTOAN_DON),don.trangThai=="CHO");
}

void CuaSoNhaHang::xuLyDonDat(int id) {
    if(id==NUT_GUI_DON) {
        HoaDon duTinh=dichVu.duTinhDatMon(gioHang);
        if(!xacNhan(L"Gửi đơn với số tiền tạm tính "+tien(duTinh.tongTien())+
            L"?\nGiá, ưu đãi và tồn kho được xác nhận lại khi thanh toán tại nhà hàng.")) return;
        string ma=dichVu.datMon(homNay(),gioHang);
        gioHang.clear(); hienTrang(DON_DAT);
        thanhCong(L"Đã gửi đơn "+rong(ma)+L". Nhân viên sẽ tiếp nhận đơn của bạn.");
        return;
    }
    if(id==NUT_LAMMOI_DON) { hienTrang(DON_DAT); return; }
    int i=dongChon(BANG_CHINH);
    if(i<0) throw runtime_error("Hãy chọn một đơn đặt món.");
    string ma=donDatHienThi.at(i).duTinh.getMa();
    if(id==NUT_HUY_DON) {
        if(!xacNhan(L"Hủy đơn "+rong(ma)+L"?")) return;
        dichVu.huyDonDat(ma); hienTrang(DON_DAT);
        thanhCong(L"Đã hủy đơn "+rong(ma));
    } else if(id==NUT_THANHTOAN_DON) {
        string nv=nhanVienDaChon();
        HoaDon truoc=dichVu.xemTruocDonDat(ma,homNay(),nv);
        if(!xacNhan(L"Xác nhận thanh toán đơn "+rong(ma)+L": "+tien(truoc.tongTien())+
            L"?\nĐây là tổng theo giá và ưu đãi hiện tại. Tồn kho và điểm sẽ được cập nhật.")) return;
        HoaDon hoaDon=dichVu.thanhToanDonDat(ma,homNay(),nv);
        hienTrang(DON_DAT); thanhCong(L"Đã thanh toán đơn "+rong(ma));
        xemHoaDon(cuaSo,hoaDon);
    }
}
