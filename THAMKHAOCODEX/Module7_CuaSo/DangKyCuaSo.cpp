#include "CuaSo.h"
#include <algorithm>

using namespace std;

void CuaSoNhaHang::trangDangKy() {
    int x=rongCuaSo/2+64, w=rongCuaSo/2-128;
    int y=max(40,(caoCuaSo-584)/2+24);
    theBieuMau={x-32,y-24,x+w+32,y+552};
    HWND tieuDe=tao(L"STATIC",L"Đăng ký khách hàng",SS_LEFT,-1,x,y,w,44);
    SendMessageW(tieuDe,WM_SETFONT,reinterpret_cast<WPARAM>(fontLon),TRUE);
    nhan(L"Tạo tài khoản để tích điểm và xem hóa đơn của bạn.",x,y+48,w);
    nhap(L"Họ và tên",O_TEN,x,y+84,w);
    int nua=(w-16)/2;
    nhap(L"Số điện thoại (9–15 chữ số)",O_SDT,x,y+150,nua);
    chon(L"Giới tính",O_GIOITINH,x+nua+16,y+150,nua,{L"Nam",L"Nữ",L"Khác"});
    chonMuc(O_GIOITINH,2);
    nhap(L"Tên đăng nhập",O_USER,x,y+216,w);
    nhap(L"Mật khẩu (8–128 byte)",O_PASS,x,y+282,w,L"",true);
    nhap(L"Nhập lại mật khẩu",O_CONFIRM,x,y+348,w,L"",true);
    nut(L"Tạo tài khoản khách hàng",NUT_DANGKY,x,y+430,w,1);
    nut(L"Quay lại đăng nhập",NUT_VE_DANGNHAP,x,y+480,w);
    SetFocus(o(O_TEN));
}
