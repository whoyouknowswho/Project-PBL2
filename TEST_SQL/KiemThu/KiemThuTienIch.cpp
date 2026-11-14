// Kiem thu don vi cho cac ham thuan trong Module7_CuaSo/TienIch.cpp.
#include "../Module7_CuaSo/TienIch.h"
#include <cstdio>

using namespace std;

static int soDat = 0, soTruot = 0;
#define KIEM(dieuKien) do { if (dieuKien) soDat++; else { soTruot++; \
    printf("TRUOT dong %d: %s\n", __LINE__, #dieuKien); } } while (0)

int main() {
    KIEM(tien(0)==L"0 đ"); KIEM(tien(999)==L"999 đ"); KIEM(tien(45000)==L"45.000 đ");
    KIEM(tien(1234.6)==L"1.235 đ"); KIEM(tien(1000000000)==L"1.000.000.000 đ");
    KIEM(tien(-500)==L"-500 đ"); KIEM(tien(-5000)==L"-5.000 đ");
    KIEM(hep(rong("Tô Đông Hải"))=="Tô Đông Hải");
    KIEM(khopTuKhoa("KH0001 Tô Đông Hải 0905995740", L"đông")); KIEM(khopTuKhoa("KH01 Le", L"kh01"));
    KIEM(khopTuKhoa("abc", L"")); KIEM(!khopTuKhoa("abc", L"x"));
    KIEM(tenHangVip("Kim Cuong")==L"Kim cương"); KIEM(tenHangVip("Bac")==L"Bạc"); KIEM(tenHangVip("")==L"Thường");
    KIEM(tenTrangThaiDon("XONG")==L"Đã thanh toán"); KIEM(tenTrangThaiDon("HUY")==L"Đã hủy"); KIEM(tenTrangThaiDon("CHO")==L"Chờ xử lý");
    KIEM(oAnhMinhHoa("Mon an","Com ga")==0); KIEM(oAnhMinhHoa("Mon an","Rau xào")==1); KIEM(oAnhMinhHoa("Mon an","Mi Tom")==2);
    KIEM(oAnhMinhHoa("Nuoc uong","Tra dao")==3); KIEM(oAnhMinhHoa("Nuoc uong","Nước cam")==4);
    KIEM(oAnhMinhHoa("Mon an","Pho bo")==5); KIEM(oAnhMinhHoa("Nuoc uong","Com ga")==5);
    KIEM(maAnhHopLe("MA01")); KIEM(maAnhHopLe("A_b-1")); KIEM(!maAnhHopLe("")); KIEM(!maAnhHopLe("../x"));
    TrucBieuDo a=tinhTrucBieuDo(0); KIEM(a.tran==1200000 && a.donVi==1000000 && a.tenDonVi==L"triệu đồng");
    TrucBieuDo b=tinhTrucBieuDo(45000); KIEM(b.tran==80000 && b.donVi==1000 && b.tenDonVi==L"nghìn đồng");
    TrucBieuDo c=tinhTrucBieuDo(5e9); KIEM(c.tran==8e9 && c.tenDonVi==L"tỷ đồng");
    KIEM(nhanTruc(80)==L"80"); KIEM(nhanTruc(1.2)==L"1.20");
    KIEM(kyBaoCao("")==L"Toàn bộ thời gian"); KIEM(kyBaoCao("09/11/2026")==L"Ngày 09/11/2026"); KIEM(kyBaoCao("11/2026")==L"Tháng 11/2026");
    KIEM(homNay().size()==10 && homNay()[2]=='/' && homNay()[5]=='/');
    KIEM(maHoaDonMoi().rfind("HD",0)==0);
    KIEM(tenGioiTinh("Nu")==L"Nữ"); KIEM(tenGioiTinh("Nam")==L"Nam"); KIEM(tenGioiTinh("Khác")==L"Khác");
    if (soTruot) { printf("TRUOT %d/%d\n", soTruot, soDat + soTruot); return 1; }
    printf("PASS %d/%d\n", soDat, soDat);
    return 0;
}
