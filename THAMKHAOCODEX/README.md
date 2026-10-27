# Quản lý nhà hàng — C++ / Win32

Ứng dụng cửa sổ viết bằng C++14 và Windows Win32 API. Giao diện và nghiệp vụ không cần trình duyệt, Qt hoặc Python.

## Chạy chương trình

Nhấp đúp **QuanLyNhaHangGUI.exe**, hoặc chạy trong PowerShell tại thư mục dự án:

```powershell
.\QuanLyNhaHangGUI.exe
```

Dùng tài khoản đã tạo trước đó. Nếu chưa có tệp tài khoản, chương trình hướng dẫn tạo quản lý đầu tiên. Mật khẩu hiển thị bằng dấu `*` và được lưu dưới dạng băm PBKDF2-HMAC-SHA256.

Thư mục **DuLieu** phải nằm cạnh exe. Ứng dụng tự tìm dữ liệu theo vị trí exe, kể cả khi mở bằng Explorer. Khi di chuyển ứng dụng, mang theo cả thư mục này.

## Chức năng

| Chức năng | Quản lý | Nhân viên |
| --- | --- | --- |
| Tổng quan, quản lý khách hàng | Có | Có |
| Xem, tìm kiếm, lọc và sắp xếp thực đơn | Có | Có |
| Thêm/sửa/xóa món, nhập kho | Có | Không |
| Quản lý hồ sơ, lương nhân viên | Có | Không |
| Giỏ hàng, thanh toán, xem hóa đơn | Có | Có |
| Báo cáo, doanh thu, lịch sử hóa đơn | Có | Không |
| Tạo tài khoản | Có | Không |
| Đổi mật khẩu của mình, đăng xuất | Có | Có |

- Chọn dòng trong bảng để sửa thông tin; **Thêm mới** làm trống biểu mẫu.
- Giỏ hàng chưa trừ tồn kho. Tồn kho, giảm giá VIP và điểm tích lũy được cập nhật khi xác nhận thanh toán.
- Hóa đơn hiển thị bằng bảng món, cột số lượng/đơn giá/thành tiền và khung tổng thanh toán.
- Nhân viên lập hóa đơn bằng mã gắn với tài khoản của mình.
- Báo cáo nhận bộ lọc `dd/mm/yyyy`, `mm/yyyy` hoặc để trống xem tất cả.
- Tài khoản nhân viên cần gắn với mã nhân viên đang tồn tại. Tài khoản này không đăng nhập được nếu hồ sơ nhân viên đã bị xóa.
- Đổi mật khẩu cần nhập đúng mật khẩu hiện tại. Sau ba lần đăng nhập sai liên tiếp, mở lại ứng dụng để thử lại.

## Biên dịch

Cần MinGW-w64 có `g++` và `windres` trong PATH. Đóng ứng dụng trước khi biên dịch lại:

```powershell
.\build.ps1
```

Trong VS Code, **Ctrl+Shift+B** biên dịch toàn bộ dự án; tác vụ **Nha hang: Chay giao dien** mở chương trình.

Build tạo một bản chạy chính là `QuanLyNhaHangGUI.exe`. Thư viện C++ được liên kết tĩnh. `bcrypt`, `comctl32`, `gdi32`, `uxtheme`, `gdiplus`, `ole32` là thư viện Windows. Tệp `.rc` và `.manifest` là tài nguyên hiển thị Windows, được nhúng vào exe. Hai logo gốc nằm trong `Module7_CuaSo/Anh` và cũng được nhúng vào exe; khi chạy không cần chép ảnh riêng.

## Cấu trúc mã nguồn

- **main.cpp:** mở giao diện cửa sổ và xử lý lỗi khởi động.
- **Module1_ConNguoi:** lớp cha Nguoi, KhachHang, NhanVien; thông tin cá nhân, VIP, lương. Hàm `toFile()` của mỗi lớp chuyển dữ liệu thành chuỗi để lưu.
- **Module2_ThucDon:** lớp cha Mon, MonAn, NuocUong; giá, tồn kho và thuộc tính riêng.
- **Module3_BanHang:** hóa đơn, chi tiết món, tổng tiền và giảm giá.
- **Module4_QuanLy:** danh sách trong bộ nhớ, tìm đối tượng theo mã và đọc dữ liệu.
- **Module6_TaiKhoan:** tài khoản, mật khẩu băm và lưu tài khoản.
- **Module7_CuaSo:** cửa sổ, biểu mẫu, bảng, nút bấm, giỏ hàng và hóa đơn. `DichVuNhaHang` nối giao diện với các lớp nghiệp vụ và kiểm tra quyền.

Chỉ giữ giao diện cửa sổ. Các menu, hàm nhập/xuất console và bản xem thử tạm đã được loại bỏ.

## Kiểm tra nghiệp vụ

```powershell
.\tests\chay_kiem_tra_gui.ps1
```

Bộ kiểm tra C++ chạy trên dữ liệu tạm, không sửa dữ liệu thật. Nội dung gồm đăng nhập, phân quyền, thêm/sửa/xóa, kiểm tra tồn kho, giảm giá, thanh toán, đọc lại hóa đơn, báo cáo, đổi mật khẩu và phục hồi khi ghi tệp thất bại.

Thư mục `.build` chỉ chứa sản phẩm biên dịch tạm; có thể tạo lại bằng script. Dữ liệu thực và tài khoản nằm trong `DuLieu`, không nằm trong `.build`.
