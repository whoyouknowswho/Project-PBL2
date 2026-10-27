> Giao diện cửa sổ mới: xem README.md và chạy QuanLyNhaHangGUI.exe. Module7_CuaSo chứa giao diện Win32 viết bằng C++. main.cpp mặc định mở cửa sổ; tham số --console chạy luồng menu cũ.

# Cách đọc mã nguồn

Đọc theo thứ tự: `main.cpp` → `QuanLy::chay()` → hàm xử lý menu bạn muốn tìm hiểu.

## Vai trò của từng module

- `Module1_ConNguoi`: thông tin chung của người, khách hàng, nhân viên; tính hạng VIP và lương.
- `Module2_ThucDon`: thông tin món ăn, nước uống và tồn kho.
- `Module3_BanHang`: chi tiết hóa đơn, tiền hàng, giảm giá và lưu/đọc hóa đơn.
- `Module4_QuanLy`: quản lý danh sách, xử lý lựa chọn menu và đọc/ghi dữ liệu.
- `Module5_GiaoDien`: nhập liệu dùng chung, màu sắc, khung bảng, báo cáo và in hóa đơn.

- `Module6_TaiKhoan`: lưu tài khoản và kiểm tra mật khẩu; xem `Module6_TaiKhoan/README.md` để sử dụng đăng nhập.

## Các cú pháp còn dùng trong chương trình

- `vector<KhachHang>`: danh sách khách hàng, có thể thêm hoặc xóa phần tử.
- `for (const KhachHang& khachHang : dsKhachHang)`: lần lượt đọc từng khách hàng. `&` tránh tạo bản sao; `const` không cho sửa đối tượng qua biến này.
- `KhachHang*`: con trỏ tới khách hàng trong danh sách. `nullptr` nghĩa là không tìm thấy; `->` dùng để gọi hàm qua con trỏ.
- `unique_ptr<Mon>`: giữ một món và tự giải phóng bộ nhớ khi món bị xóa. Nhờ lớp cha `Mon`, danh sách chứa được cả `MonAn` và `NuocUong`.
- `make_unique<MonAn>(...)`: tạo một món ăn được quản lý bởi `unique_ptr`.
- `virtual` và `override`: cho phép gọi đúng hàm của món ăn hoặc nước uống qua lớp cha.
- `map<string, ThongKeMon>`: tra cứu thống kê theo mã món. Khi duyệt map, `first` là mã món, `second` là thông tin thống kê.
- `static_cast<int>(...)`: đổi giá trị sang kiểu số nguyên một cách tường minh.
- `try/catch`: xử lý dòng dữ liệu không hợp lệ khi đọc tệp.
- Danh sách sau dấu `:` trong hàm khởi tạo dùng để khởi tạo lớp cha và các thuộc tính.

Các lambda đã được thay bằng hàm có tên hoặc vòng lặp. Biến dùng kiểu dữ liệu tường minh thay cho `auto`; các biểu thức `?:` được viết thành `if/else`. Mỗi bước nhập thông tin được thực hiện trước khi gọi hàm thêm hoặc cập nhật, giúp thứ tự câu hỏi rõ ràng.

## Biên dịch và chạy trong PowerShell

Chạy từ thư mục THAMKHAOCODEX:

```powershell
.\build.ps1
.\QuanLyNhaHangGUI.exe
```

Dùng `.\QuanLyNhaHangGUI.exe --console` nếu muốn xem bản console cũ.
