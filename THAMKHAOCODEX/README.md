# Quản lý nhà hàng — giao diện cửa sổ C++

Mở **`QuanLyNhaHangGUI.exe`** bằng cách nhấp đúp, hoặc trong PowerShell:

```powershell
.\QuanLyNhaHangGUI.exe
```

Ứng dụng dùng **C++14 và Windows Win32 API**. Toàn bộ giao diện, sự kiện nút bấm và nghiệp vụ đều viết bằng C++; không dùng trình duyệt, HTML, JavaScript, Python hay Qt để chạy ứng dụng. Tệp `.rc`/`.manifest` là tài nguyên cấu hình hiển thị của Windows, được nhúng vào tệp exe.

## Bắt đầu

Nếu đã có tài khoản, dùng tên đăng nhập và mật khẩu cũ. Nếu chưa có `DuLieu/taikhoan.txt`, màn hình thiết lập sẽ cho bạn tạo quản lý đầu tiên. Mật khẩu hiển thị bằng `*`.

Sau khi đăng nhập, chọn chức năng ở thanh bên trái:

- **Tổng quan:** số khách, số món, tồn kho thấp; quản lý còn thấy doanh thu hôm nay.
- **Khách hàng:** tìm kiếm, chọn dòng để sửa, thêm mới, lưu và xóa có xác nhận.
- **Nhân viên:** hồ sơ, chức vụ, lương và phụ cấp; chỉ quản lý được truy cập.
- **Thực đơn:** tìm/lọc/sắp xếp món. Quản lý được thêm/sửa/xóa và nhập kho; nhân viên chỉ xem.
- **Bán hàng:** chọn khách và người bán, chọn món, số lượng, size/đá, thêm/xóa khỏi giỏ, thanh toán và xem hóa đơn. Tồn kho chỉ bị trừ khi xác nhận thanh toán; giảm giá VIP và điểm được tính tự động. Nhân viên được gắn đúng mã đang đăng nhập.
- **Báo cáo:** lọc ngày/tháng, doanh thu, món bán chạy, lịch sử hóa đơn và tồn kho thấp; chỉ quản lý được truy cập. Chọn hóa đơn rồi nhấn **Xem hóa đơn đã chọn**, hoặc nhấp đúp vào dòng.
- **Tạo tài khoản:** quản lý cấp tài khoản cho quản lý khác hoặc mã nhân viên đang tồn tại.
- **Đổi mật khẩu**, **Đăng xuất**; nút **X** đóng ứng dụng. Giỏ chưa thanh toán có xác nhận trước khi hủy khi thoát/đăng xuất.

Mọi thay đổi thành công được lưu ngay vào các tệp trong `DuLieu` cạnh exe. Khi di chuyển ứng dụng, mang theo cả thư mục `DuLieu`. Bản GUI tự xác định thư mục này từ vị trí exe, nên mở bằng Explorer cũng đọc đúng dữ liệu.

## Biên dịch trên Windows

Cần MinGW-w64 (`g++`, `windres` đã có trên máy hiện tại):

```powershell
.\build.ps1
```

Trong VS Code, **Ctrl+Shift+B** cũng biên dịch toàn bộ dự án; tác vụ **Nha hang: Chay giao dien** mở bản GUI đã biên dịch.

Script tạo exe có thư viện C++ được liên kết tĩnh, không cần cài Qt hay thư viện GUI ngoài. Các thư viện `bcrypt`, `comctl32`, `gdi32`, `uxtheme` thuộc Windows. Đóng bản GUI đang chạy trước khi biên dịch đè lên exe.

Nếu muốn sử dụng bản console để học/đối chiếu:

```powershell
.\QuanLyNhaHangGUI.exe --console
```

Chế độ console vẫn cần thư mục làm việc là thư mục dự án chứa `DuLieu`.

## Mã nguồn giao diện

`Module7_CuaSo` gồm:

- `DichVuNhaHang.h/.cpp`: nối các lớp nghiệp vụ hiện có với giao diện, kiểm tra dữ liệu và quyền, lưu thay đổi, lập hóa đơn. Không chứa nút hay cửa sổ Win32.
- `CuaSo.h/.cpp`: khung cửa sổ, màu sắc, thanh bên, nút, bảng, xử lý thông điệp Windows.
- `BieuMau.cpp`: biểu mẫu khách hàng/nhân viên/thực đơn, đăng nhập và tài khoản.
- `BanHangCuaSo.cpp`: giỏ hàng, thanh toán và báo cáo.
- `HoaDonCuaSo.cpp`: cửa sổ đọc hóa đơn.

## Kiểm tra

Các kiểm tra dùng dữ liệu tạm và không sửa dữ liệu thật:

```powershell
.\tests\chay_kiem_tra_gui.ps1
python tests/kiem_tra_dang_nhap.py .\QuanLyNhaHangGUI.exe
```

Python chỉ dùng cho bộ kiểm tra, không dùng để xây dựng hoặc chạy giao diện. `tests/xem_thu_cua_so.cpp` là bản xem thử riêng, chỉ biên dịch khi bật `NHAHANG_GUI_PREVIEW`, dùng dữ liệu giả trong `.build/gui-preview`; bản phát hành không có chế độ đăng nhập thử này.
