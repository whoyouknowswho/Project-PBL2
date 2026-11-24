# Quản lý nhà hàng — C++ / Win32 / SQLite

Ứng dụng cửa sổ viết bằng C++14, Windows Win32 API (GDI/GDI+) và cơ sở dữ liệu nhúng **SQLite**. Không cần cài SQL Server, trình duyệt hay thư viện ngoài khi chạy.

## Chạy chương trình

Nhấp đúp **QuanLyNhaHangGUI.exe**, hoặc chạy trong PowerShell:

```powershell
.\QuanLyNhaHangGUI.exe
```

## Cơ sở dữ liệu SQLite

Toàn bộ dữ liệu được lưu gọn trong tệp `DuLieu/QuanLyNhaHang.db` (tự tạo 6 bảng khi mở ứng dụng lần đầu).

- **Xem bảng & truy vấn**: Dùng tiện ích **SQLite Viewer** trong VS Code, **DB Browser for SQLite**, hoặc chạy lệnh:
  ```powershell
  sqlite3 DuLieu/QuanLyNhaHang.db "SELECT ma, ten, gia, tonKho FROM ThucDon LIMIT 10;"
  ```
- **Nạp lại dữ liệu mẫu** (1 quản lý `QUANLY`, 10 nhân viên `nhanvien01`–`nhanvien10`, 20 khách hàng `khachhang01`–`khachhang20`, 100 món ăn/nước uống — mật khẩu chung `123456789`):
  ```powershell
  Get-Content -Raw -Encoding UTF8 Module5_SQL/du_lieu_mau.sql | sqlite3 DuLieu/QuanLyNhaHang.db
  ```
  Danh sách tài khoản đầy đủ nằm trong [DanhSachTaiKhoan/TaiKhoan.txt](DanhSachTaiKhoan/TaiKhoan.txt).

## Biên dịch

Cần MSYS2 / MinGW-w64 (`g++`, `windres`, `libsqlite3`) trong `PATH`:

```powershell
.\build.ps1
```

## Cấu trúc mã nguồn

- **main.cpp**: Điểm vào chương trình và bắt lỗi khởi động.
- **Module1_ConNguoi**: Lớp cha `Nguoi`, `KhachHang` (điểm tích lũy, hạng VIP), `NhanVien` (chức vụ, lương).
- **Module2_ThucDon**: Lớp cha `Mon`, `MonAn` (độ cay, món chay), `NuocUong` (size, đá).
- **Module3_BanHang**: `HoaDon` và `ChiTietHoaDon` (tạm tính, giảm giá VIP, thành tiền).
- **Module4_QuanLy**: Quản lý danh sách đối tượng trong bộ nhớ và tìm kiếm theo mã.
- **Module5_SQL**: `KetNoiSqlite` đọc/ghi 6 bảng SQLite và tệp `du_lieu_mau.sql`.
- **Module6_TaiKhoan**: Quản lý đăng nhập, phân quyền và đổi mật khẩu.
- **Module7_CuaSo**: Giao diện Win32/GDI+ và lớp dịch vụ nghiệp vụ `DichVuNhaHang`.
