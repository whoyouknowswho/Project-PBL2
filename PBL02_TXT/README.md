# Quản lý nhà hàng — C++ / Win32 / File TXT

Ứng dụng cửa sổ viết bằng C++14, Windows Win32 API (GDI/GDI+) và lưu trữ dữ liệu trực tiếp trên các **tệp văn bản `.txt` (UTF-8)**. Không cần cài hệ quản trị cơ sở dữ liệu hay thư viện ngoài khi chạy.

## Chạy chương trình

Nhấp đúp **QuanLyNhaHangGUI.exe**, hoặc chạy trong PowerShell:

```powershell
.\QuanLyNhaHangGUI.exe
```

## Lưu trữ dữ liệu dạng tệp TXT

Toàn bộ dữ liệu được lưu trong thư mục `DuLieu/` dưới dạng 6 tệp `.txt` mã hóa UTF-8 (tự động khởi tạo khi mở ứng dụng lần đầu), các trường phân tách bởi dấu `|`:

- `DuLieu/ThucDon.txt`: `loai|ma|ten|gia|tonKho|doCayHoacSize|monChayHoacCoDa|hinhAnh`
- `DuLieu/KhachHang.txt`: `ma|ten|dienThoai|gioiTinh|diem|email`
- `DuLieu/NhanVien.txt`: `ma|ten|dienThoai|gioiTinh|chucVu|luong|phuCap`
- `DuLieu/HoaDon.txt`: `ma|ngay|maKhach|maNhanVien|giamGia|chiTiet`
- `DuLieu/DonDat.txt`: mỗi dòng lưu thông tin một đơn đặt món
- `DuLieu/TaiKhoan.txt`: `tenDangNhap|matKhau|vaiTro|maLienKet`

Tài khoản mẫu có sẵn trong `DuLieu/TaiKhoan.txt` gồm 1 quản lý (`QUANLY`), 10 nhân viên (`nhanvien01`–`nhanvien10`) và 20 khách hàng (`khachhang01`–`khachhang20`), mật khẩu chung `123456789`.

## Biên dịch

Cần MSYS2 / MinGW-w64 (`g++`, `windres`) trong `PATH`:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

## Cấu trúc mã nguồn

- **main.cpp**: Điểm vào chương trình và bắt lỗi khởi động.
- **Module1_ConNguoi**: Lớp cha `Nguoi`, `KhachHang` (điểm tích lũy, hạng VIP), `NhanVien` (chức vụ, lương).
- **Module2_ThucDon**: Lớp cha `Mon`, `MonAn` (độ cay, món chay), `NuocUong` (size, đá).
- **Module3_BanHang**: `HoaDon` và `ChiTietHoaDon` (tạm tính, giảm giá VIP, thành tiền).
- **Module4_QuanLy**: Quản lý danh sách đối tượng trong bộ nhớ và tìm kiếm theo mã.
- **Module5_LuuTru**: `LuuTruTxt` đọc/ghi 6 tệp dữ liệu `.txt` trong thư mục `DuLieu/`.
- **Module6_TaiKhoan**: Quản lý đăng nhập, phân quyền và đổi mật khẩu.
- **Module7_CuaSo**: Giao diện Win32/GDI+ và lớp dịch vụ nghiệp vụ `DichVuNhaHang`.
