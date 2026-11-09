# Giao diện mới PBL_Claude — Kế hoạch triển khai

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Xóa toàn bộ code giao diện Win32 cũ trong `PBL_Claude/Module7_CuaSo` và viết lại giao diện mới tông "ấm áp kiểu nhà hàng", giữ nguyên mọi chức năng.

**Architecture:** Giữ mô hình cũ: lớp `CuaSoNhaHang` dựng lại điều khiển con mỗi khi đổi trang (`hienTrang`), mọi nghiệp vụ đi qua `DichVuNhaHang` (không sửa). Phần vẽ chuyển sang GDI+ khử răng cưa + bộ đệm đôi; điều khiển dùng chung (nút, ô nhập, hộp chọn, ô đánh dấu, bảng, lưới thẻ món) gom vào các tệp riêng để các trang chỉ lo bố cục và luồng xử lý.

**Tech Stack:** C++14, Win32 API, comctl32 v6, GDI + GDI+, MinGW-w64 (g++ 16, windres) qua `build.ps1`; kiểm thử bằng một chương trình C++ nhỏ (hàm thuần) và các kịch bản PowerShell 7 điều khiển exe qua tin nhắn Win32.

**Spec:** `docs/superpowers/specs/2026-11-09-giao-dien-moi-design.md` — đọc trước khi làm; mục 6 của spec là danh sách hành vi bắt buộc.

## Global Constraints

- Làm việc **chỉ trong** `PBL_Claude/`. Không sửa `main.cpp`, `build.ps1`, Module1–6, `DichVuNhaHang.h`, `DichVuNhaHang.cpp`, `DatMonDichVu.cpp`, `UngDung.rc`, `UngDung.manifest`, `Anh/*`.
- Không đụng dữ liệu thật trong `PBL_Claude/DuLieu`; mọi lần chạy thử dùng hộp cát trong `$env:TEMP\pbl_kiemthu\`.
- C++14, `-Wall -Wextra` không có cảnh báo mới; không thêm thư viện ngoài thư viện hệ thống đã liên kết (`bcrypt comctl32 gdi32 uxtheme gdiplus ole32`). Nếu cần `msimg32` hay thư viện khác thì không được — dùng GDI+ thay thế.
- Tên định danh tiếng Việt không dấu theo phong cách mã hiện có (`camelCase`, ví dụ `veThe`, `napGio`); chữ hiển thị tiếng Việt có dấu, giữ nguyên câu chữ bản cũ ở mọi chỗ spec nói "giữ nguyên".
- Bảng màu (spec mục 3): NEN `#FAF6F0`, THE `#FFFFFF`, VIEN `#EDE4D8`, THANH_BEN `#2B1D16`, THANH_BEN_RE `#3D2A20`, CHINH `#D9622B`, CHINH_RE `#C4551F`, CHINH_NHAN `#A84719`, PHU_NEN `#FBEDE3`, PHU_CHU `#8A3B12`, LOI `#C0392B`/`#FDECEA`, TOT `#2E8B57`/`#E6F4EC`, CANH_BAO `#B7791F`/`#FFF5DB`, CHU `#2D2420`, CHU_MO `#8A7B70`.
- Font Segoe UI cỡ 14 (nhỏ), 15 (thường), 17 đậm (đậm), 26 đậm (tiêu đề); icon Segoe MDL2 Assets 16 và 20.
- Cửa sổ tối thiểu 1120 × 760; mặc định min(1320, vùng làm việc − 40) × min(880, vùng làm việc − 40), căn giữa.
- Hộp thoại xác nhận/lỗi dùng `MessageBoxW` như cũ (tiêu đề "Vui lòng kiểm tra" cho lỗi, "Xác nhận" cho câu hỏi Yes/No mặc định No).
- **Không commit git** trừ khi người dùng yêu cầu; mỗi task kết thúc bằng build + kiểm thử xanh.

## Review Focus

1. **Tìm kiếm tiếng Việt có dấu** — gõ "đông" phải tìm thấy "Tô Đông Hải" (không phân biệt hoa thường). Kiểm thử: `khopTuKhoa` trong Task 1 và tìm khách trong Task 4.
2. **Cửa sổ nhỏ nhất và phóng to** — ở 1120 × 760 và khi phóng to, không điều khiển nào tràn ra ngoài vùng khách hoặc chồng lên nhau ở trang Bán hàng/Báo cáo/Thực đơn. Kiểm thử: Task 10 bước kiểm tra kích thước.
3. **Ảnh món hỏng hoặc thiếu** — tệp `DuLieu/AnhMon/MA01.png` là rác thì dùng ảnh minh họa, không lỗi, không treo. Kiểm thử: Task 5.
4. **Rò rỉ GDI khi chuyển trang liên tục** — chuyển qua lại 60 lần, số GDI object của tiến trình không tăng quá 40. Kiểm thử: Task 10.
5. **Số tiền rất lớn / âm** — `tien(1000000000)` = `1.000.000.000 đ`, `tien(-500)` = `-500 đ`; trục biểu đồ đổi sang "tỷ đồng". Kiểm thử: Task 1.

---

## Bản đồ tệp

| Tệp (trong `Module7_CuaSo/` trừ khi ghi khác) | Task |
| --- | --- |
| `TienIch.h`, `TienIch.cpp` — hàm thuần: chuỗi, tiền, ngày, so khớp, ảnh minh họa, trục biểu đồ | 1 |
| `KiemThu/KiemThuTienIch.cpp`, `KiemThu/ChayKiemThuDonVi.ps1` | 1 |
| `ChuDe.h`, `ChuDe.cpp` — màu, font, icon, hàm vẽ GDI+ | 2 |
| `AnhMon.h`, `AnhMon.cpp` — logo, ảnh món (GDI+) | 2 |
| `CuaSo.h`, `CuaSo.cpp`, `DieuKhien.cpp`, `TrangDangNhap.cpp` | 2 |
| `KiemThu/GiaoDien.psm1`, `KiemThu/KT2_DangNhap.ps1` | 2 |
| `TrangCaNhan.cpp`, `KiemThu/TaoDuLieuMau.ps1`, `KiemThu/KT3_TaiKhoan.ps1` | 3 |
| `TrangDanhSach.cpp`, `KiemThu/KT4_DanhSach.ps1` | 4 |
| `LuoiMon.h`, `LuoiMon.cpp`, `TrangThucDon.cpp`, `KiemThu/KT5_ThucDon.ps1` | 5 |
| `TrangBanHang.cpp`, `CuaSoHoaDon.cpp`, `KiemThu/KT6_BanHang.ps1` | 6 |
| `TrangDonDat.cpp`, `KiemThu/KT7_DonDat.ps1` | 7 |
| `TrangBaoCao.cpp` (+ Hóa đơn của tôi trong `TrangCaNhan.cpp`), `KiemThu/KT8_BaoCao.ps1` | 8 |
| `TrangTongQuan.cpp`, `KiemThu/KT9_TongQuan.ps1` | 9 |
| `README.md` (mục Cấu trúc mã nguồn), `KiemThu/ChayTatCa.ps1` | 10 |

`build.ps1` chỉ gom `main.cpp` và `Module*/*.cpp`, nên thư mục `KiemThu/` không bị biên dịch vào exe.

---

### Task 1: Xóa giao diện cũ + hàm tiện ích thuần có kiểm thử

**Files:**
- Delete: `Module7_CuaSo/{CuaSo.cpp,CuaSo.h,BieuMau.cpp,BanHangCuaSo.cpp,BaoCaoCuaSo.cpp,BieuDoDoanhThu.cpp,DangKyCuaSo.cpp,DangNhapCuaSo.cpp,DatMonCuaSo.cpp,HoaDonCuaSo.cpp,KhachHangCuaSo.cpp,ThucDonAnh.cpp,ThucDonKhachHang.cpp,Logo.cpp,Logo.h}`
- Create: `Module7_CuaSo/TienIch.h`, `Module7_CuaSo/TienIch.cpp`
- Test: `KiemThu/KiemThuTienIch.cpp`, `KiemThu/ChayKiemThuDonVi.ps1`

**Interfaces:**
- Produces (`TienIch.h`, chỉ include `<windows.h>`, `<string>`, `<vector>`; không include `DichVuNhaHang.h`):
  - `wstring rong(const string& utf8)`, `string hep(const wstring&)` — UTF-8 ↔ UTF-16.
  - `wstring so(double)` — `fixed`, `setprecision(0)`.
  - `wstring tien(double)` — như `so` + dấu chấm ngăn nghìn + `L" đ"`; xử lý dấu âm đúng.
  - `string homNay()` — `dd/mm/yyyy` theo giờ máy; `string maHoaDonMoi()` — `HD<yyyymmdd>-<GetTickCount64()>`.
  - `bool khopTuKhoa(const string& noiDung, const wstring& tuKhoa)` — chữ thường bằng `CharLowerBuffW`, tìm chuỗi con; từ khóa rỗng → true.
  - `wstring tenHangVip(const string& hang)` — `Thuong→Thường`, `Bac→Bạc`, `Vang→Vàng`, `Kim Cuong→Kim cương`, khác → `Thường`.
  - `wstring tenTrangThaiDon(const string&)` — `XONG→Đã thanh toán`, `HUY→Đã hủy`, khác → `Chờ xử lý`.
  - `int oAnhMinhHoa(const string& loai, const string& tenMon)` — 0 cơm gà, 1 rau xào, 2 mì tôm (chỉ khi `loai=="Mon an"`), 3 trà đào, 4 nước cam (chỉ khi `loai=="Nuoc uong"`), còn lại 5; so khớp không dấu và có dấu như bản cũ.
  - `bool maAnhHopLe(const string& ma)` — không rỗng, chỉ `[A-Za-z0-9_-]`.
  - `struct TrucBieuDo { double tran, donVi; wstring tenDonVi; };` và `TrucBieuDo tinhTrucBieuDo(double lonNhat)` — thuật toán bản cũ: `lonNhat<=0 → 1000000`; `bac=10^floor(log10(lonNhat/4))`; `tran=ceil(lonNhat/4/bac)*bac*4`; đơn vị tỷ/triệu/nghìn/đồng theo `tran`.
  - `wstring nhanTruc(double)` — số nguyên thì 0 chữ số thập phân, ngược lại 2.
  - `wstring kyBaoCao(const string& phamVi)` — rỗng → `Toàn bộ thời gian`, dài 10 → `Ngày <x>`, dài 7 → `Tháng <x>`.

- [ ] **Step 1: Xóa 15 tệp giao diện cũ** (danh sách ở mục Files). Kiểm tra: `Get-ChildItem Module7_CuaSo -Filter *.cpp` chỉ còn `DichVuNhaHang.cpp`, `DatMonDichVu.cpp`.

- [ ] **Step 2: Viết kiểm thử thất bại `KiemThu/KiemThuTienIch.cpp`** — macro `KIEM(dieuKien)` đếm đạt/trượt, in dòng trượt kèm số dòng, `main` trả 1 nếu có trượt và in `PASS n/n`. Các khẳng định:

```cpp
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
```

- [ ] **Step 3: Viết `KiemThu/ChayKiemThuDonVi.ps1`** — biên dịch `g++ -std=c++14 -Wall -Wextra -finput-charset=UTF-8 KiemThu/KiemThuTienIch.cpp Module7_CuaSo/TienIch.cpp -o $env:TEMP\pbl_kiemthu\kt_tienich.exe` rồi chạy, trả mã thoát của exe. Chạy: thất bại vì `TienIch.h` chưa có.

- [ ] **Step 4: Viết `TienIch.h` / `TienIch.cpp`** theo khối Interfaces.

- [ ] **Step 5: Chạy** `pwsh KiemThu/ChayKiemThuDonVi.ps1` → `PASS 39/39` (đúng số khẳng định), mã thoát 0, không cảnh báo.

---

### Task 2: Chủ đề, điều khiển dùng chung, khung cửa sổ và màn đăng nhập

**Files:**
- Create: `ChuDe.h`, `ChuDe.cpp`, `AnhMon.h`, `AnhMon.cpp`, `CuaSo.h`, `CuaSo.cpp`, `DieuKhien.cpp`, `TrangDangNhap.cpp`
- Test: `KiemThu/GiaoDien.psm1`, `KiemThu/KT2_DangNhap.ps1`

**Interfaces:**
- Consumes: toàn bộ `TienIch.h` (Task 1); `DichVuNhaHang` (không đổi).
- Produces:
  - `ChuDe.h`: `namespace Mau { const COLORREF NEN, THE, VIEN, THANH_BEN, THANH_BEN_RE, CHINH, CHINH_RE, CHINH_NHAN, PHU_NEN, PHU_CHU, LOI, LOI_NEN, TOT, TOT_NEN, CANH_BAO, CANH_BAO_NEN, CHU, CHU_MO, O_CHI_DOC; }` (giá trị ở Global Constraints; `O_CHI_DOC` = `#FBF8F3`).
    `struct BoFont { HFONT nho, thuong, dam, tieuDe, so, icon, iconLon; };` `BoFont taoBoFont();` `void xoaBoFont(BoFont&);` (`so` = Segoe UI 24 đậm cho số liệu thẻ).
    `namespace Icon { const wchar_t TRANG_CHU=0xE80F, KHACH=0xE716, NHAN_VIEN=0xE77B, THUC_DON=0xE719, BAN_HANG=0xE7BF, DON_DAT=0xE8A5, BAO_CAO=0xE9D2, TAI_KHOAN=0xE8FA, MAT_KHAU=0xE72E, HO_SO=0xE779, HOA_DON=0xE7C3, DANG_XUAT=0xF3B1, LICH=0xE787, TIM=0xE721, THEM=0xE710, XOA=0xE74D, LUU=0xE74E, LAM_MOI=0xE72C, MUI_TEN_XUONG=0xE70D, DANH_DAU=0xE73E, THONG_TIN=0xE946; }` — sau khi build, chụp màn hình kiểm tra từng icon hiển thị; icon nào ra ô vuông thì thay mã khác trong Segoe MDL2 Assets.
    Hàm vẽ (GDI+ `SmoothingModeAntiAlias` cho hình, GDI `DrawTextW` cho chữ): `void veHinhBoGoc(HDC, RECT, int banKinh, COLORREF nen, COLORREF vien=CLR_INVALID, int doDay=1)`; `void veThe(HDC, RECT, int banKinh=14)` (bóng đổ 3 lớp alpha thấp lệch xuống 2–6 px + nền trắng + viền `VIEN`); `void veChu(HDC, const wstring&, RECT, HFONT, COLORREF, UINT can=DT_LEFT)` (luôn thêm `DT_NOPREFIX`; nếu không có `DT_WORDBREAK` thì thêm `DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS`); `void veIcon(HDC, wchar_t, RECT, HFONT, COLORREF)` (bỏ qua nếu `!coFontIcon()`); `bool coFontIcon()`; `void veHuyHieu(HDC, const wstring&, RECT, COLORREF nen, COLORREF chu, HFONT)` (viên thuốc bo tròn hết cỡ); `void veChuyenSac(HDC, RECT, COLORREF tren, COLORREF duoi)`; `void veVongTron(HDC, int cx, int cy, int r, COLORREF, BYTE alpha=255)`.
  - `AnhMon.h`: `class KhoAnh { public: void napTaiNguyen(); void giaiPhong(); void veLogo(HDC, int maTaiNguyen, RECT) const; void veAnhMon(HDC, const Mon&, RECT, int banKinh); };` — tài nguyên 101/102/103 (`LOGO_BACH_KHOA`, `LOGO_CNTT`, `ANH_THUC_DON` giữ hằng như cũ); nạp PNG từ tài nguyên qua `IStream` rồi **`Clone` sang `Gdiplus::Bitmap` mới** để giải phóng luồng ngay; ảnh riêng `DuLieu/AnhMon/<ma>.png` chỉ khi `maAnhHopLe(ma)`, tải lười, nhớ cả kết quả thất bại (không thử lại mỗi lần vẽ), ảnh hỏng → dùng ảnh minh họa; ảnh minh họa cắt theo `oAnhMinhHoa` như bản cũ (lưới 3 cột, hàng trên cao 47,4 %); vẽ cắt giữa (cover) trong vùng bo góc `banKinh`; logo vẽ vừa khung giữ tỉ lệ.
  - `CuaSo.h`: giữ nguyên **giá trị** hai enum `Trang` và `DieuKhien` của bản cũ (sao lại đầy đủ) và thêm cuối enum `DieuKhien`: `NUT_GIAM_SL, NUT_TANG_SL`. Include `TienIch.h`, `ChuDe.h`, `AnhMon.h`, `DichVuNhaHang.h`. Khai báo `void xemHoaDon(HWND chu, const HoaDon&, const BoFont&);` và `int chayGiaoDienCuaSo();`. `enum class KieuNut { Phu, Chinh, NguyHiem, ThanhBen, Tab, TabChon, Chip, ChipChon };`
  - Lớp `CuaSoNhaHang` (thành viên dữ liệu của bản cũ giữ nguyên tên/ý nghĩa: `dichVu, thietLap, dangVeBang, dangDangKy, trangHienTai, soLanSai, rongCuaSo, caoCuaSo, maDangSua, maHoaDon, ngayDon, khachDon, nhanVienDon, thongBao, gioHang, hoaDonBaoCao, donDatHienThi, baoCaoHienTai, phamViBaoCao, namBieuDo, doanhThuNam, thangDangXem, nhomDatMon, nhomThucDon, maMonDangChon`; thêm `BoFont font; KhoAnh anh; vector<RECT> cacThe; map<int,RECT> khungNhap; int oDangChon=-1; map<HWND,KieuNut> kieuNut; map<int,bool> danhDauGiaTri; HWND nutDangRe=nullptr; HBRUSH coNen, coThe, coChiDoc;`).
    Hàm dựng điều khiển (trong `DieuKhien.cpp`), tọa độ là góc trên-trái của cả cụm:
    `HWND tao(const wchar_t* lop, const wstring& chu, DWORD kieu, int id, int x, int y, int w, int h, DWORD moRong=0)`;
    `HWND nhan(const wstring&, int x, int y, int w, bool dam=false, int id=-1)` (STATIC cao 20);
    `HWND nhap(const wstring& ten, int id, int x, int y, int w, const wstring& giaTri=L"", bool matKhau=false)` — nhãn ở `y` (bỏ nếu `ten` rỗng), khung 40 px ở `y+24` đăng ký vào `khungNhap[id]`, EDIT không viền cao bằng chiều cao chữ đặt giữa khung (lề trái 12); giữ y nguyên luật `ES_NUMBER`, bộ lọc ký tự (`O_GIA`, `O_LUONG`, `O_PHUCAP` cho phép một dấu `.`; `O_NGAY` cho phép `/`), giới hạn độ dài (mật khẩu 128, `O_USER` 32, `O_SDT` 15, `O_EMAIL` 254, khác 160), ký tự che `*`;
    `HWND chon(const wstring& ten, int id, int x, int y, int w, const vector<wstring>& cacMuc)` — cùng bố cục với `nhap`; `CBS_DROPDOWNLIST|CBS_OWNERDRAWFIXED|CBS_HASSTRINGS|WS_VSCROLL`; subclass tự vẽ trạng thái đóng (khung bo góc, chữ, mũi tên `Icon::MUI_TEN_XUONG`); mục danh sách vẽ trong `WM_DRAWITEM` (mục chọn nền `PHU_NEN`); chọn mục 0;
    `HWND nut(const wstring&, int id, int x, int y, int w, KieuNut=KieuNut::Phu, wchar_t icon=0, int h=40)` — `BS_OWNERDRAW`, subclass bắt rê chuột (`TrackMouseEvent`) và đặt con trỏ bàn tay; `void datKieuNut(int id, KieuNut)` vẽ lại;
    `void danhDau(const wstring&, int id, int x, int y, int w, bool giaTri=false)` — nút owner-draw, mỗi lần bấm đảo `danhDauGiaTri[id]` trước khi xử lý lệnh; `bool daDanhDau(int id) const`; `void datDanhDau(int id, bool)`;
    `HWND bang(int id, int x, int y, int w, int h, const vector<pair<wstring,int>>& cot, int cotHuyHieu=-1)` — ListView report, `LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER|LVS_EX_LABELTIP`, không viền; gọi `ganKieuBang`;
    `void dongBang(int id, const vector<wstring>&, int giaTri)`, `int dongChon(int id) const`, `HWND o(int)`, `wstring chu(int)`, `string duLieu(int)`, `void gan(int, const wstring&)`, `int luaChon(int)`, `void chonMuc(int,int)`, `double soNhap(int id, const string& ten, bool nguyen=false) const` (quy tắc như cũ: số hữu hạn 0…1.000.000.000, số nguyên khi `nguyen`; lỗi `"<ten>: vui lòng nhập số."` / `"<ten>: số không hợp lệ (0 đến 1.000.000.000)."`).
    Hàm bảng tự do (cũng dùng cho cửa sổ hóa đơn): `void ganKieuBang(HWND lv, const BoFont&, int cotHuyHieu, int chieuCaoDong=40)` — đặt image list 1×`chieuCaoDong` (giải phóng trong `WM_NCDESTROY` của subclass), subclass ListView để tự vẽ tiêu đề (nền `NEN`, chữ in hoa `nho` màu `CHU_MO`) qua `WM_NOTIFY` từ header; `LRESULT veDongBang(NMLVCUSTOMDRAW*, const BoFont&, int cotHuyHieu)` — vẽ cả dòng (`CDRF_SKIPDEFAULT` ở mức subitem): nền trắng / `#FDFAF6` xen kẽ, dòng chọn `PHU_NEN`, đệm trái 12, cột căn phải giữ theo `LVCOLUMN.fmt`; dòng có `lParam==-1` chữ `CHU_MO`; ô ở cột huy hiệu vẽ `veHuyHieu` với màu theo chữ: `Hết hàng`/`Đã hủy` → LOI (riêng `Đã hủy` nền `#EFEAE6` chữ `CHU_MO`), `Sắp hết`/`Chờ xử lý`/chuỗi số → CANH_BAO, `Đã thanh toán` → TOT.
  - Khung cửa sổ (`CuaSo.cpp`): `thuTuc` xử lý `WM_CREATE` (font, ảnh, chổi), `WM_SIZE` (hẹn giờ 180 ms rồi `hienTrang(trangHienTai,true)`), `WM_GETMINMAXINFO`, `WM_COMMAND` (bỏ qua khi `dangVeBang`; `EN_SETFOCUS/EN_KILLFOCUS/CBN_SETFOCUS/CBN_KILLFOCUS` cập nhật `oDangChon` và vẽ lại khung), `WM_NOTIFY`, `WM_MEASUREITEM`/`WM_DRAWITEM` (nút, hộp chọn), `WM_CTLCOLORSTATIC`/`WM_CTLCOLOREDIT`/`WM_CTLCOLORBTN` (chổi theo nền bên dưới: trong một `cacThe` → trắng; EDIT chỉ đọc → `O_CHI_DOC`; còn lại `NEN`), `WM_ERASEBKGND` trả 1, `WM_PAINT` vẽ qua bộ đệm đôi bằng `ve(HDC)`, `WM_CLOSE` (hỏi khi giỏ chưa trống), `WM_DESTROY` (giải phóng font, ảnh, chổi). Mọi ngoại lệ → `baoLoi`. `hienTrang` sao chép logic kiểm quyền, giữ nhập (`giuNhap`, thêm việc giữ `danhDauGiaTri`), lưu thông tin HĐ khi rời Bán hàng, đúng như bản cũ; `switch` ban đầu chỉ có trang đăng nhập/đăng ký/thiết lập — các task sau thêm `case` của mình. `ve(HDC)` khi chưa đăng nhập gọi `veManDangNhap`; khi đã đăng nhập vẽ thanh bên 248 px (thương hiệu, thẻ người dùng: vòng tròn chữ cái đầu, tên, huy hiệu vai trò), đầu trang (tiêu đề + mô tả giữ nguyên câu chữ bản cũ, ô ngày có `Icon::LICH`), các `cacThe`, chân trang có `Icon::THONG_TIN` + `thongBao`, rồi gọi `veTrang(HDC)` (task sau thêm phần vẽ riêng). `thanhBen()` dựng nút `KieuNut::ThanhBen` có icon, cùng danh sách/thứ tự mục theo vai trò như bản cũ, Đăng xuất ở đáy. Vòng lặp tin nhắn: giữ xử lý Enter ở màn đăng nhập/đăng ký và ở `LOC_BAO_CAO` như bản cũ, rồi `IsDialogMessageW`. `chayGiaoDienCuaSo` giữ đúng khóa mutex + chuyển thư mục làm việc + thông báo "Chương trình đang mở…" như bản cũ, và bao `GdiplusStartup/Shutdown` bằng một đối tượng RAII khai báo **trước** `CuaSoNhaHang app`.
  - Bố cục nội dung: `X0 = 280` (thanh bên 248 + lề 32), lề phải 32, `Y0 = 124` (dưới đầu trang), chân trang cao 40; khoảng cách giữa thẻ 20; đệm trong thẻ 24.
  - `TrangDangNhap.cpp`: `trangDangNhap()`, `trangDangKy()`, `veManDangNhap(HDC)` — bố cục spec 4.1, cùng trường/ID/câu chữ/tiêu điểm ban đầu như bản cũ (`O_USER` cho đăng nhập, `O_TEN` cho đăng ký, Giới tính mặc định "Khác"). Xử lý nút `NUT_DANGNHAP`, `NUT_MO_DANGKY`, `NUT_DANGKY`, `NUT_VE_DANGNHAP`, `DANG_XUAT` và điều hướng trang chuyển nguyên văn logic của `xuLyNut` bản cũ sang `xuLyNut` mới (`CuaSo.cpp`).
  - `KiemThu/GiaoDien.psm1` (PowerShell 7, `Add-Type` C# gọi `user32`/`kernel32`): `Tao-HopCat [-TaiKhoanTrong] [-NguonDuLieu <dir>]` → thư mục mới `$env:TEMP\pbl_kiemthu\<yyyyMMdd-HHmmss-fff>` chứa exe + bản sao `DuLieu`; `Khoi-Dong $thuMuc` (chờ cửa sổ `NhaHangCuaSo`); `Dat-Chu $id $chu` (`WM_SETTEXT` + gửi `EN_CHANGE` cho cửa sổ chính); `Doc-Chu $id`; `Bam $id` (**`PostMessage`** `WM_COMMAND/BN_CLICKED`, để không kẹt khi nút mở hộp thoại); `Chon-Hop $id $viTri` (`CB_SETCURSEL` + post `CBN_SELCHANGE`); `Chon-Dong $id $dong` (tiêu điểm vào ListView rồi `VK_HOME` + `VK_DOWN`×dong); `Doc-Bang $id` (đọc ô qua `VirtualAllocEx`/`LVM_GETITEMTEXTW`/`ReadProcessMemory`); `Chon-Mon $id $viTri` (tin nhắn `LM_CHONMUC`, có từ Task 5); `Cho-HopThoai [-Bam Yes|No|OK] [-Giay 5]` → trả nội dung hộp thoại `#32770` của tiến trình và bấm nút; `Co-DieuKhien $id`; `Dang-Bat $id`; `Dang-Hien $id`; `Chup $ten` (`PrintWindow` → `$hopCat\anh\<ten>.png`); `Doi-KichThuoc $w $h`; `Dong-UngDung`; `Kiem $dieuKien $moTa` + `Ket-Thuc` (in `PASS n/n` hoặc danh sách trượt, `exit 1`). Hằng mật khẩu thử `MAT_KHAU_THU = 'KiemThu@2026'` (chỉ dùng trong hộp cát).

- [ ] **Step 1: Viết `KiemThu/GiaoDien.psm1`** theo danh sách hàm trên.
- [ ] **Step 2: Viết `KiemThu/KT2_DangNhap.ps1` (thất bại)** — hộp cát `-TaiKhoanTrong`:
  1. Mở app: `Co-DieuKhien O_CONFIRM` (chế độ thiết lập), không có `NUT_MO_DANGKY`.
  2. Thiết lập `ql_test` / `MAT_KHAU_THU` hai lần → hộp thoại chứa "Đã tạo tài khoản quản lý"; sau OK, `Doc-Chu O_USER` = `ql_test`.
  3. Sai mật khẩu lần 1 → hộp thoại chứa "Sai tài khoản"; `Doc-Chu O_PASS` rỗng.
  4. Bấm `NUT_MO_DANGKY` → có `O_EMAIL`, `O_GIOITINH` chọn mục 2; bấm `NUT_VE_DANGNHAP` → lại có `NUT_DANGNHAP`.
  5. Đăng ký `kh_test` (Họ tên `Khách Thử`, SĐT `0901234567`, email `khach.thu@example.com`) → hộp thoại "Đăng ký thành công" chứa `KH`; `Doc-Chu O_USER` = `kh_test`.
  6. Đăng nhập `ql_test` đúng → có `TRANG_CHU`, `KHACH_HANG`, `NHAN_VIEN`, `THUC_DON`, `BAO_CAO`, `TAI_KHOAN`, `MAT_KHAU`, `DANG_XUAT`; **không** có `BAN_HANG`, `DON_DAT`. `Chup dangnhap_ql`.
  7. Đăng xuất (Yes) → về màn đăng nhập. Sai 3 lần với `kh_test` → hộp thoại "Đăng nhập sai 3 lần", tiến trình thoát trong 5 giây.
  8. Mở lần hai khi lần một đang chạy → hộp thoại "Chương trình đang mở", tiến trình thứ hai thoát.
- [ ] **Step 3: Chạy** `.\build.ps1` → lỗi biên dịch/liên kết (chưa có mã). Ghi nhận.
- [ ] **Step 4: Viết `ChuDe`, `AnhMon`, `CuaSo.h`, `DieuKhien.cpp`, `CuaSo.cpp`, `TrangDangNhap.cpp`** theo Interfaces.
- [ ] **Step 5: Build** `.\build.ps1` → `Built: ...`, không có dòng `warning:`.
- [ ] **Step 6: Chạy** `pwsh KiemThu/KT2_DangNhap.ps1` → `PASS`. Mở ảnh `dangnhap_ql.png` và ảnh màn đăng nhập: kiểm icon không phải ô vuông, chữ không bị cắt, logo đúng tỉ lệ.

---

### Task 3: Tạo tài khoản, Đổi mật khẩu, Thông tin của tôi + bộ dữ liệu mẫu

**Files:**
- Create: `TrangCaNhan.cpp`; Modify: `CuaSo.cpp` (`case TAI_KHOAN`, `MAT_KHAU`, `HO_SO_CUA_TOI`; nút `NUT_TAOTAIKHOAN`, `NUT_DOIMATKHAU`; `CBN_SELCHANGE` của `O_ROLE`)
- Test: `KiemThu/TaoDuLieuMau.ps1`, `KiemThu/KT3_TaiKhoan.ps1`

**Interfaces:**
- Consumes: hàm dựng điều khiển Task 2.
- Produces: `void trangTaiKhoan()`, `void trangMatKhau()`, `void trangHoSoKhachHang()`, `void capNhatHoSoTaiKhoan()`, `string vaiTroDuocChon() const`, `string maLienKetDuocChon() const` — logic giống hệt bản cũ (`KhachHangCuaSo.cpp`, `BieuMau.cpp`). Thẻ biểu mẫu rộng 520. Hồ sơ khách: thẻ hai cột, mỗi trường vẽ là nhãn nhỏ + giá trị trên nền `O_CHI_DOC` (vẽ trong `veTrang`, **không** dùng EDIT), hạng thành viên vẽ bằng `veHuyHieu`, dòng "Liên hệ nhân viên nếu cần cập nhật thông tin.".
  `KiemThu/TaoDuLieuMau.ps1` → tạo (hoặc dùng lại nếu đã có) `$env:TEMP\pbl_kiemthu\mau\DuLieu`: chép `DuLieu` của dự án, xóa `taikhoan.txt`, chạy app: thiết lập `ql_test`; đăng nhập; Tạo tài khoản `nv_test` vai trò Nhân viên liên kết `NV01`; đăng xuất; Đăng ký `kh_test` như Task 2. Các task sau gọi `Tao-HopCat -NguonDuLieu $env:TEMP\pbl_kiemthu\mau\DuLieu`.

- [ ] **Step 1: Viết `KT3_TaiKhoan.ps1` (thất bại)** — từ dữ liệu mẫu:
  1. `ql_test`: mở `TAI_KHOAN`; `Chon-Hop O_ROLE 1` (Quản lý) → `O_NHANVIEN` bị vô hiệu, mục 0 là "Không cần liên kết hồ sơ"; `Chon-Hop O_ROLE 2` → số mục `O_NHANVIEN` = số khách trong `khachhang.txt`.
  2. Tạo tài khoản trùng `nv_test` → hộp thoại lỗi; tài khoản mới `nv_hai` liên kết NV02 → trạng thái "Đã tạo tài khoản thành công."; `taikhoan.txt` của hộp cát có dòng bắt đầu `nv_hai|NHANVIEN|NV02|`.
  3. `MAT_KHAU`: mật khẩu cũ sai → hộp thoại lỗi; đúng → "Đã đổi mật khẩu thành công.", các ô trống. Đăng xuất, đăng nhập bằng mật khẩu mới được.
  4. `kh_test`: mở `HO_SO_CUA_TOI` → không có `O_TEN` (không còn ô nhập); `Chup hoso_kh`. Mở `NHAN_VIEN` bằng `Bam` → hộp thoại "Tài khoản khách hàng không có quyền".
- [ ] **Step 2: Chạy** → trượt. **Step 3: Viết `TrangCaNhan.cpp` + sửa `CuaSo.cpp`.** **Step 4: Build sạch cảnh báo; chạy `TaoDuLieuMau.ps1` rồi `KT3_TaiKhoan.ps1`** → `PASS`.

---

### Task 4: Khách hàng và Nhân viên

**Files:**
- Create: `TrangDanhSach.cpp`; Modify: `CuaSo.cpp` (`case KHACH_HANG`, `NHAN_VIEN`; `EN_CHANGE` của `TIM_KIEM`; `LVN_ITEMCHANGED` của `BANG_CHINH` → `chonDong()`; nút `NUT_LUU`, `NUT_MOI`, `NUT_XOA`)
- Test: `KiemThu/KT4_DanhSach.ps1`

**Interfaces:**
- Produces: `void trangDanhSach()` (dùng cho KH/NV; Thực đơn ở Task 5), `void napBang()`, `void chonDong()`, `void luuBieuMau()`, `void xoaBanGhi()` — luật dữ liệu giống hệt bản cũ (`BieuMau.cpp`), tìm kiếm dùng `khopTuKhoa`. Bố cục: thẻ trái (ô `TIM_KIEM` có icon kính lúp vẽ trong khung, dòng hướng dẫn, `BANG_CHINH` cột như cũ); thẻ phải rộng 340 "Thông tin chi tiết" với các ô như cũ, nút `NUT_LUU` (Chính, icon Lưu), `NUT_XOA` (Nguy hiểm), `NUT_MOI` (Phụ, icon Thêm), ghi chú "Mã được giữ cố định khi sửa bản ghi.". Cột "Hạng VIP" là cột huy hiệu, hiển thị `tenHangVip`.

- [ ] **Step 1: Viết `KT4_DanhSach.ps1` (thất bại)** — `ql_test`:
  1. `KHACH_HANG`: `NUT_XOA` vô hiệu; `Dat-Chu TIM_KIEM 'đông'` → mọi dòng `Doc-Bang` chứa "Đông"/"đông"; xóa tìm kiếm → số dòng = số dòng `khachhang.txt`.
  2. Thêm `KH9001` "Nguyễn Văn Thử" `0911222333` điểm `120` email `thu@example.com` → `khachhang.txt` có dòng `KH9001|`; `O_MA` chỉ đọc; `NUT_XOA` bật.
  3. Điểm `abc` → `O_DIEM` không nhận chữ (ES_NUMBER) — `Dat-Chu` bỏ qua; điểm `2000000000` → hộp thoại "số không hợp lệ".
  4. Chọn dòng KH9001, `NUT_XOA` → hộp thoại hỏi, bấm No → còn; bấm lại, Yes → mất khỏi tệp.
  5. `NHAN_VIEN`: chọn dòng 0 → `O_LUONG` = `7000000`; `O_LUONG` gõ `1.2.3` → chỉ nhận một dấu chấm.
  6. `nv_test`: thanh bên không có `NHAN_VIEN`; `KHACH_HANG` dùng được. `Chup khachhang`.
- [ ] **Step 2: Chạy → trượt. Step 3: Viết mã. Step 4: Build sạch; chạy KT4** → `PASS`.

---

### Task 5: Lưới thẻ món và trang Thực đơn

**Files:**
- Create: `LuoiMon.h`, `LuoiMon.cpp`, `TrangThucDon.cpp`; Modify: `CuaSo.cpp` (`case THUC_DON`; `NUT_NHOM_THUCAN/NUOC`, `SAP_XEP`, `CON_HANG`, `O_LOAI`, `NUT_NHAPKHO`, `LBN_SELCHANGE` của `DANH_SACH_THUCDON`), `TrangDanhSach.cpp` (`luuBieuMau`/`xoaBanGhi`/`chonDong` nhánh Thực đơn)
- Test: `KiemThu/KT5_ThucDon.ps1`

**Interfaces:**
- Produces (`LuoiMon.h`):
  - `struct TheMon { int duLieu; wstring ten, gia, thongTin, huyHieu; COLORREF mauHuyHieu, nenHuyHieu; bool mo=false; const Mon* mon=nullptr; };`
  - `HWND taoLuoiMon(HWND cha, int id, RECT vung, const BoFont*, KhoAnh*)` — lớp cửa sổ `L"NhaHangLuoiMon"`, trạng thái riêng cấp phát ở `WM_NCCREATE`, giải phóng ở `WM_NCDESTROY`.
  - `void luoiMonDat(HWND, const vector<TheMon>&, int duLieuChon)` — giữ vị trí cuộn nếu có thể.
  - Tin nhắn: `LM_CHONMUC = WM_APP+101` (wParam = vị trí hiển thị; chọn và gửi `WM_COMMAND(MAKEWPARAM(id,LBN_SELCHANGE))` cho cha), `LM_LAYCHON = WM_APP+102` (trả `duLieu` của thẻ chọn hoặc −1), `LM_SOMUC = WM_APP+103`.
  - Hành vi: thẻ rộng tối thiểu 200, số cột = max(1, (rộng−16) / (200+16)), thẻ cao 236 (ảnh 128 bo góc trên); nhấp = chọn (+`LBN_SELCHANGE`), nhấp đúp = `LBN_DBLCLK`; con lăn + thanh cuộn dọc; rê chuột viền `CHINH_RE`, thẻ chọn viền 2 px `CHINH` nền `#FFF8F2`; `mo` → ảnh và chữ nhạt; danh sách rỗng → chữ giữa "Không có món phù hợp. Hãy thử tìm kiếm hoặc đổi bộ lọc."; vẽ qua bộ đệm đôi.
  - Trang: `void trangThucDon()`, `void napThucDonAnh()`, `int monThucDonDaChon() const`, `void doiLoaiMon()` — lọc/sắp xếp/đếm nhóm/thông tin thẻ như bản cũ (`ThucDonAnh.cpp`): thông tin = `Độ cay: n/5 · Món chay|Món mặn` hoặc `Size X`; quản lý: huy hiệu `Tồn kho: n` (`Hết hàng` đỏ khi 0); người khác: chỉ huy hiệu `Hết hàng` khi 0. Biểu mẫu quản lý (thẻ phải 340) giữ đủ trường/ID/luật bật-tắt của bản cũ. Nhóm dùng `KieuNut::TabChon/Tab`, chữ "Thức ăn (n)" / "Nước uống (n)".

- [ ] **Step 1: Viết `KT5_ThucDon.ps1` (thất bại)** — hộp cát có thêm `DuLieu/AnhMon/MA01.png` là 100 byte rác:
  1. `ql_test` mở `THUC_DON`: không lỗi; `Doc-Chu NUT_NHOM_THUCAN` = "Thức ăn (3)", `NUT_NHOM_NUOC` = "Nước uống (2)"; `SendMessage LM_SOMUC` = 3.
  2. `Chon-Hop SAP_XEP 3` (Giá giảm) → `LM_CHONMUC 0` → `Doc-Chu O_MA` = `MA01`; `O_MA` chỉ đọc; `NUT_NHAPKHO` bật; `O_SIZE` ẩn.
  3. `Bam NUT_NHOM_NUOC` → `LM_SOMUC` = 2; chọn món → `O_CAY` vô hiệu, `O_SIZE` hiện.
  4. Thêm món `MA09` "Phở bò" 50000 tồn 0 → `menu.txt` có `MONAN|MA09|`; bật `CON_HANG` → `LM_SOMUC` = 3 (MA09 bị ẩn); nhập kho `O_NHAPKHO=5` cho MA09 → `menu.txt` tồn = 5.
  5. Xóa MA09 (Yes) → không còn trong `menu.txt`.
  6. `kh_test` mở `THUC_DON`: không có `O_MA`, không có `NUT_LUU`. `Chup thucdon_kh`, `Chup thucdon_ql`.
- [ ] **Step 2–4:** trượt → viết mã → build sạch, `PASS`; xem ảnh: thẻ món có ảnh bo góc, giá màu cam, không chữ bị tràn.

---

### Task 6: Bán hàng, Đặt món, cửa sổ hóa đơn

**Files:**
- Create: `TrangBanHang.cpp`, `CuaSoHoaDon.cpp`; Modify: `CuaSo.cpp` (`case BAN_HANG`, `DAT_MON`; `NUT_THEM_GIO`, `NUT_XOA_GIO`, `NUT_HUY_GIO`, `NUT_THANHTOAN`, `NUT_GIAM_SL`, `NUT_TANG_SL`, `NUT_GUI_DON`, `O_KHACH` đổi, `LBN_SELCHANGE`/`LBN_DBLCLK` của `DANH_SACH_MON`)
- Test: `KiemThu/KT6_BanHang.ps1`

**Interfaces:**
- Consumes: `LuoiMon` (Task 5), `ganKieuBang`/`veDongBang` (Task 2).
- Produces: `void trangBanHang()`, `void trangDatMon()`, `void napMonDat()` (dùng cho cả hai trang, lưới `DANH_SACH_MON`), `int monDatDaChon() const`, `void chonMonDat()`, `void napGio()`, `void themVaoGio()`, `void thanhToan()`, `string khachDaChon() const`, `string nhanVienDaChon() const`, `void veTongGio(HDC)` (khung tổng trong thẻ giỏ: Tạm tính, Giảm giá, **Thanh toán/Dự kiến** chữ `so` màu `CHINH`; giỏ trống → minh họa + "Giỏ món đang trống.\nChọn món bên trái để bắt đầu."). Giá trị tổng cũng gán vào STATIC ẩn `O_TONG` (để kiểm thử đọc được) theo đúng chuỗi bản cũ.
  Luật giống hệt bản cũ (`BanHangCuaSo.cpp`, `ThucDonKhachHang.cpp`, `DatMonCuaSo.cpp:56-64`), cộng: Bán hàng dùng lưới + 2 tab nhóm (`nhomDatMon` chung), thẻ hiện `Còn n` (n = tồn − trong giỏ, đỏ "Hết hàng" khi ≤ 0, `mo=true`); Đặt món chỉ hiện "Đã hết món"; `NUT_GIAM_SL`/`NUT_TANG_SL` đổi `O_SOLUONG` trong [1, 999]; `NUT_THEM_GIO` vô hiệu khi chưa chọn hoặc hết (cả hai trang).
  `void xemHoaDon(HWND chu, const HoaDon&, const BoFont&)` (`CuaSoHoaDon.cpp`): lớp `L"NhaHangHoaDonMoi"`, modal như bản cũ (vô hiệu cửa sổ cha, vòng lặp riêng, chuyển tiếp `WM_QUIT`), tối thiểu 760 × 650, bố cục spec 4.3, Esc/Enter/nút `Đóng hóa đơn` đều đóng; khách/nhân viên đọc từ `toFile()` như bản cũ; bảng dùng `ganKieuBang(…, 46)` với cột Món vẽ hai dòng (tên + mã) — xử lý riêng trong thủ tục cửa sổ hóa đơn, còn lại gọi `veDongBang`.

- [ ] **Step 1: Viết `KT6_BanHang.ps1` (thất bại)** — `nv_test`:
  1. `BAN_HANG`: `O_MA` bắt đầu `HD`, chỉ đọc; `O_NGAY` = hôm nay; `O_NHANVIEN` = `NV01`; `NUT_THANHTOAN`, `NUT_XOA_GIO`, `NUT_HUY_GIO` vô hiệu; `NUT_THEM_GIO` vô hiệu.
  2. Chọn món `Nuoc cam` (nhóm nước) → `O_SIZE` hiện; `O_SOLUONG` = 3, `Bam NUT_TANG_SL` → 4; size L; thêm → `Doc-Bang BANG_GIO` = `Nuoc cam (L)`, `4`, `120.000 đ`.
  3. Thêm 155 nữa (tổng > 158 tồn) → hộp thoại "không vượt tồn kho"; thêm 1 cùng size → vẫn 1 dòng, SL 5.
  4. `Chon-Hop O_KHACH` chọn KH0001 (hạng có giảm) → `Doc-Chu O_TONG` chứa "Giảm:" khác `0 đ`.
  5. Chuyển `THUC_DON` rồi về `BAN_HANG` → `O_MA`, khách, giỏ giữ nguyên.
  6. `NUT_THANHTOAN` → hộp thoại "Xác nhận thanh toán", No → giỏ còn; lại, Yes → cửa sổ hóa đơn mở (`FindWindow NhaHangHoaDonMoi`), `Chup hoadon`, gửi Esc → đóng; `hoadon.txt` có dòng mới với mã cũ; `menu.txt` tồn NU02 giảm 5; `O_MA` đổi mã mới; giỏ trống.
  7. Đóng cửa sổ khi giỏ có món → hộp thoại "Giỏ hàng chưa thanh toán sẽ bị hủy", No → còn mở.
  8. `kh_test` `DAT_MON`: thẻ không có chữ "Còn"; nhấp đúp (gửi `LBN_DBLCLK`) thêm món; `NUT_GUI_DON` → hộp thoại tạm tính, Yes → trang chuyển sang `DON_DAT`, `dondatmon.txt` có đơn trạng thái `CHO`. `Chup datmon`.
- [ ] **Step 2–4:** trượt → viết mã → build sạch, `PASS`.

---

### Task 7: Đơn khách đặt / Đơn đặt của tôi

**Files:** Create `TrangDonDat.cpp`; Modify `CuaSo.cpp` (`case DON_DAT`; `NUT_LAMMOI_DON`, `NUT_HUY_DON`, `NUT_THANHTOAN_DON`; chọn dòng `BANG_CHINH` ở trang này → `chiTietDonDat()`). Test `KiemThu/KT7_DonDat.ps1`.

**Interfaces:** Produces `void trangDonDat()`, `void chiTietDonDat()`, `void xuLyDonDat(int id)` — logic bản cũ (`DatMonCuaSo.cpp`). Bảng đơn: cột "Trạng thái" là cột huy hiệu (`tenTrangThaiDon`); bảng chi tiết bên dưới trong thẻ riêng.

- [ ] **Step 1: Viết KT7 (thất bại):** `kh_test` gửi 2 đơn → `DON_DAT` không có `NUT_THANHTOAN_DON`; chọn đơn mới nhất (dòng 0) → `BANG_GIO` có món; hủy (Yes) → trạng thái `Đã hủy`, nút Hủy vô hiệu khi chọn lại. `nv_test` `DON_DAT`: chọn đơn còn chờ → `NUT_THANHTOAN_DON` bật → Yes → cửa sổ hóa đơn mở, đóng; dòng đó thành `Đã thanh toán`, cột Hóa đơn có mã; `dondatmon.txt` trạng thái `XONG`. Chọn đơn đã hủy → cả hai nút vô hiệu. `Chup dondat_nv`.
- [ ] **Step 2–4:** trượt → viết mã → build sạch, `PASS`.

---

### Task 8: Báo cáo và Hóa đơn của tôi

**Files:** Create `TrangBaoCao.cpp`; Modify `TrangCaNhan.cpp` (`trangLichSuKhachHang`), `CuaSo.cpp` (`case BAO_CAO`, `HOA_DON_CUA_TOI`; `NUT_BAOCAO`, `NUT_BC_HOMNAY/THANG/TATCA`, `NUT_HOADON`, `NM_DBLCLK` của `BANG_CHINH` ở hai trang này). Test `KiemThu/KT8_BaoCao.ps1`.

**Interfaces:** Produces `void trangBaoCao()`, `void napBaoCao()`, `void veThongKeBaoCao(HDC)`, `void trangLichSuKhachHang()` — logic bản cũ (`BaoCaoCuaSo.cpp`, `KhachHangCuaSo.cpp:53-67`). Chip lọc dùng `KieuNut::ChipChon/Chip`; dòng "Đang xem: " + `kyBaoCao`; 3 thẻ số liệu có icon tròn; bảng tồn kho: cột "Còn lại" là cột huy hiệu.

- [ ] **Step 1: Viết KT8 (thất bại):** `ql_test` `BAO_CAO`: `NUT_HOADON` vô hiệu; `Bam NUT_BC_HOMNAY` → `Doc-Chu LOC_BAO_CAO` = hôm nay, `NHAN_KY_BAOCAO` chứa "Ngày"; `Dat-Chu LOC_BAO_CAO '99/99'` + `NUT_BAOCAO` → hộp thoại lỗi và bảng giữ số dòng trước đó; `NUT_BC_TATCA` → số dòng = số hóa đơn trong `hoadon.txt` (hoặc 1 dòng "Chưa có hóa đơn"); chọn dòng 0 → `NUT_HOADON` bật → bấm → cửa sổ hóa đơn mở, đóng. Enter trong `LOC_BAO_CAO` (gửi `WM_KEYDOWN VK_RETURN` qua hàng đợi) áp dụng bộ lọc. `kh_test` `HOA_DON_CUA_TOI`: số dòng = số hóa đơn trong `hoadon.txt` mang mã khách liên kết với `kh_test` (đọc từ `taikhoan.txt`); `NUT_HOADON` bật khi có hóa đơn. `Chup baocao`.
- [ ] **Step 2–4:** trượt → viết mã → build sạch, `PASS`.

---

### Task 9: Tổng quan và biểu đồ doanh thu

**Files:** Create `TrangTongQuan.cpp`; Modify `CuaSo.cpp` (`case TRANG_CHU`; `LOC_NAM_DOANHTHU`; `WM_MOUSEMOVE`/`WM_MOUSELEAVE` cho biểu đồ). Test `KiemThu/KT9_TongQuan.ps1`.

**Interfaces:** Produces `void trangChu()`, `void trangChuKhachHang()`, `void veThongKeTongQuan(HDC)`, `void trangBieuDoDoanhThu()`, `void napBieuDoDoanhThu()`, `RECT vungBieuDo() const`, `void veBieuDoDoanhThu(HDC)`, `void diChuyenBieuDo(int x, int y)` — số liệu thẻ, nút thao tác nhanh, danh sách năm, ghi chú như bản cũ (`CuaSo.cpp:344-369`, `BieuMau.cpp:34-54`, `KhachHangCuaSo.cpp:16-32`, `BieuDoDoanhThu.cpp`). Biểu đồ: trục từ `tinhTrucBieuDo`, điểm tính bằng một hàm dùng chung cho vẽ và rê chuột (bắt điểm trong ±14 px), đường `DrawCurve` căng 0,35 bút 3 px `CHINH` (cắt theo vùng trục để không vẽ dưới đáy), vùng tô `LinearGradientBrush` từ `CHINH` alpha 70 xuống alpha 0, điểm tròn trắng viền cam, thẻ gợi ý nền `THANH_BEN` chữ trắng. Thẻ tổng quan khách: dải chào "Xin chào, <tên>" nền chuyển sắc.

- [ ] **Step 1: Viết KT9 (thất bại):** `ql_test`: có `LOC_NAM_DOANHTHU`, mục 0 = năm lớn nhất trong (năm có hóa đơn ∪ năm nay); không có `BANG_CHINH`; gửi `WM_MOUSEMOVE` tới tọa độ điểm tháng hiện tại (đọc lại từ ảnh chụp không cần — chỉ kiểm không lỗi). `nv_test`: có `BANG_CHINH` liệt kê các món tồn ≤ 5 (đối chiếu `menu.txt`). `kh_test`: có `DAT_MON`, `HO_SO_CUA_TOI`, `HOA_DON_CUA_TOI` dạng nút nhanh và `BANG_CHINH`. `Chup tongquan_ql`, `tongquan_nv`, `tongquan_kh`.
- [ ] **Step 2–4:** trượt → viết mã → build sạch, `PASS`; xem ảnh biểu đồ: đường mượt, nhãn trục, đơn vị, không tràn.

---

### Task 10: Hồi quy toàn bộ, kích thước, rò rỉ GDI, README

**Files:** Create `KiemThu/ChayTatCa.ps1`; Modify `README.md` (chỉ mục "Cấu trúc mã nguồn": thay dòng Module7 bằng danh sách tệp mới; thêm một dòng về thư mục `KiemThu`).

- [ ] **Step 1: Viết `ChayTatCa.ps1`** — chạy `ChayKiemThuDonVi.ps1`, `TaoDuLieuMau.ps1` (làm mới), KT2…KT9 lần lượt; dừng ở lỗi đầu tiên; cuối in tổng.
- [ ] **Step 2: Thêm kiểm kích thước** (trong `ChayTatCa.ps1`): với `nv_test` ở `BAN_HANG`, `ql_test` ở `BAO_CAO` và `THUC_DON`, `kh_test` ở `DAT_MON`: `Doi-KichThuoc 1120 760` rồi phóng to; sau 400 ms, mọi cửa sổ con hiển thị nằm trọn trong vùng khách và các nút `NUT_XOA_GIO`, `NUT_HUY_GIO`, `NUT_THANHTOAN`/`NUT_GUI_DON`, `NUT_THEM_GIO` không giao nhau với bảng/lưới. `Chup` mỗi trường hợp.
- [ ] **Step 3: Thêm kiểm rò rỉ GDI**: `ql_test` bấm lần lượt `TRANG_CHU, KHACH_HANG, NHAN_VIEN, THUC_DON, BAO_CAO, TAI_KHOAN` 10 vòng; `GetGuiResources(GR_GDIOBJECTS)` sau − trước ≤ 40.
- [ ] **Step 4: Sửa README**; **Step 5: Build sạch + chạy `pwsh KiemThu/ChayTatCa.ps1`** → mọi bộ `PASS`. Đối chiếu từng mục 1–22 của spec mục 6 với kịch bản đã chạy; mục nào chưa có khẳng định thì kiểm tay bằng kịch bản tạm và ghi kết quả vào báo cáo cuối.
- [ ] **Step 6: Xem lại toàn bộ ảnh chụp** trong các hộp cát (màn đăng nhập, mỗi trang mỗi vai trò, hóa đơn, các kích thước) để soát lỗi hiển thị; sửa nếu có, chạy lại `ChayTatCa.ps1`.
