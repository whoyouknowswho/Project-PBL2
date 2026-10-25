# Module giao diện console

- `GiaoDien.h`: khai báo các hàm giao diện trong namespace `GiaoDien`.
- `GiaoDien.cpp`: khởi tạo console UTF-8, màu sắc, nhập liệu dùng chung, căn lề, menu chính, bảng khách hàng/nhân viên/thực đơn và báo cáo.
- `HienThiDoiTuong.cpp`: phần triển khai các hàm hiển thị của `KhachHang`, `NhanVien`, `MonAn`, `NuocUong`, `HoaDon`. Giữ nguyên khai báo trong các lớp để các lời gọi và hàm ảo hiện có tiếp tục hoạt động.

`Module4_QuanLy` vẫn xử lý điều hướng, tìm kiếm, lọc/sắp xếp, tính thống kê, bán hàng và đọc/ghi dữ liệu. Các lời nhắc và thông báo gắn với thao tác nghiệp vụ vẫn nằm tại nơi xử lý thao tác đó. Đây là phần giao diện console được tách riêng, chưa phải thư viện GUI cửa sổ.

Muốn đổi khung bảng, màu hoặc bố cục, sửa các tệp `.cpp` trong thư mục này.

## Biên dịch bằng PowerShell

Chạy từ thư mục gốc dự án, cần g++ hỗ trợ C++14 trở lên:

```powershell
$sources = @(Get-ChildItem main.cpp,Module*/*.cpp | ForEach-Object FullName)
g++ -std=c++14 -Wall -Wextra @sources -lbcrypt -o QuanLyNhaHangDangNhap.exe
.\QuanLyNhaHangDangNhap.exe
```

Nếu dùng IDE, thêm cả `GiaoDien.cpp` và `HienThiDoiTuong.cpp` vào danh sách tệp biên dịch. Chạy chương trình từ thư mục chứa `DuLieu`.
