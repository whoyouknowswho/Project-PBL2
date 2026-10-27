> Bản cửa sổ mới dùng Module7_CuaSo. Chạy `QuanLyNhaHangGUI.exe`; tài khoản cũ dùng được. Xem README.md ở thư mục gốc.

# Đăng nhập và phân quyền

## Chạy lần đầu

Chạy `QuanLyNhaHangDangNhap.exe` từ thư mục gốc dự án. Khi chưa có `DuLieu/taikhoan.txt`, chương trình yêu cầu bạn tự tạo tài khoản quản lý, rồi đăng nhập bằng tài khoản vừa tạo. Không có mật khẩu mặc định.

Tên đăng nhập gồm 1–32 chữ cái không dấu, chữ số, `_` hoặc `-`. Tên `0` dành cho thao tác hủy/thoát ở màn hình nhập tên. Mật khẩu dài 8–128 byte; mật khẩu ASCII tương ứng 8–128 ký tự. Khi nhập trong console Windows, mỗi ký tự mật khẩu hiện thành dấu `*`. Dùng Backspace để xóa và Enter để xác nhận.

## Quyền sử dụng

| Chức năng | Quản lý | Nhân viên |
| --- | --- | --- |
| Quản lý khách hàng | Có | Có |
| Quản lý hồ sơ nhân viên | Có | Không |
| Xem thực đơn | Có | Có |
| Thêm/sửa/xóa món, nhập kho | Có | Không |
| Lập hóa đơn | Có | Có |
| Thống kê doanh thu | Có | Không |
| Tạo tài khoản | Có | Không |
| Đổi mật khẩu của mình, đăng xuất | Có | Có |

Quản lý chọn **6** để tạo tài khoản quản lý khác hoặc tài khoản nhân viên. Tài khoản nhân viên phải gắn với một mã đã có trong danh sách nhân viên. Ví dụ, có thể dùng mã `NV01` nếu hồ sơ này còn tồn tại. Khi đăng nhập bằng tài khoản nhân viên, hóa đơn tự lấy mã này, không hỏi mã người bán.

Menu quản lý: chọn **7** để đổi mật khẩu, **8** để đăng xuất. Menu nhân viên: **1** quản lý khách hàng, **2** xem thực đơn, **3** lập hóa đơn, **4** đổi mật khẩu, **5** đăng xuất. Cả hai menu đều chọn **0** để lưu và thoát. Đổi mật khẩu yêu cầu nhập đúng mật khẩu hiện tại. Sau ba lần đăng nhập sai liên tiếp, chương trình kết thúc; đây không phải cơ chế khóa tài khoản lâu dài. Tài khoản nhân viên không đăng nhập được nếu hồ sơ nhân viên đã bị xóa.

## Tổ chức mã nguồn

- `TaiKhoan.h/.cpp`: đọc/ghi tài khoản, kiểm tra và băm mật khẩu.
- `Module4_QuanLy/DangNhap.cpp`: các bước đăng nhập, tạo tài khoản, đổi mật khẩu.
- `Module4_QuanLy/QuanLy.cpp`: kiểm tra quyền và điều hướng chức năng.
- `Module5_GiaoDien`: menu theo vai trò và nhập mật khẩu ẩn.

Mật khẩu được băm bằng PBKDF2-HMAC-SHA256 với 600.000 vòng, muối ngẫu nhiên riêng cho từng mật khẩu. Tệp tài khoản hỏng hoặc không đọc được khiến chương trình dừng, không tự tạo lại quản lý. Khi lưu, chương trình ghi tệp tạm rồi thay thế tệp cũ; thay đổi chỉ cập nhật trong bộ nhớ nếu lưu thành công.

Phần mật khẩu dùng thư viện BCrypt có sẵn trên Windows; khi biên dịch bằng g++ cần thêm `-lbcrypt` **sau danh sách mã nguồn**:

```powershell
.\build.ps1
.\QuanLyNhaHangGUI.exe --console
```

Kiểm tra tự động trên dữ liệu tạm, không sửa dữ liệu thật:

```powershell
python tests/kiem_tra_dang_nhap.py
```
