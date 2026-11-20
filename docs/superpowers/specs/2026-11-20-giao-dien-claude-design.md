# Thiết kế lại giao diện theo phong cách Claude

Ngày: 20/11/2026 · Phạm vi: `TEST_SQL/Module7_CuaSo` (Win32 + GDI/GDI+)

## Mục đích

Người dùng muốn giao diện app Quản lý nhà hàng "ổn nhất" theo ngôn ngữ thiết kế của Claude, kèm tối ưu hóa. Người dùng giao toàn bộ quyết định thiết kế cho Claude, không nêu thêm lý do. Các quyết định dưới đây là lựa chọn của Claude.

## Mục tiêu

- Toàn app mang một cảm giác thống nhất: nền ngà ấm, chữ gần đen, một màu nhấn đất nung, tiêu đề chữ có chân, viền mảnh thay cho bóng đổ đậm, nhiều khoảng thở.
- Thanh bên chuyển từ nâu đậm sang màu sáng cùng tông với nền.
- Lưới món cuộn và rê chuột mượt hơn.

## Không làm

- Không dời vị trí hay đổi kích thước các ô nhập, nút, bảng trong từng trang (bố cục giữ nguyên).
- Không đổi nghiệp vụ, dữ liệu, SQL, quyền.
- Không đổi công nghệ (vẫn Win32/GDI+, không thêm thư viện).

## Phải tránh

- Form nào đang nhập được mà sau khi đổi bị che, lệch, mất chữ là thất bại, dù màu đẹp hơn.
- Chữ kém tương phản (chữ phụ trên nền ngà phải đọc rõ).
- Mất các trạng thái: chọn, rê chuột, vô hiệu, lỗi, cảnh báo, thành công phải vẫn phân biệt được.

## Các cách đã cân nhắc

| Cách | Nội dung | Đánh đổi |
| --- | --- | --- |
| 1. Chỉ đổi bảng màu và font | Sửa `ChuDe.h/.cpp` | Nhanh, an toàn; nhưng thanh bên, đầu trang, nút vẫn vẽ kiểu cũ nên trông nửa vời |
| **2. Bảng màu + khung chung + tối ưu lưới món (đã chọn)** | Cách 1, cộng vẽ lại thanh bên, đầu trang, thẻ, nút, ô nhập, bảng; lưu đệm ảnh món đã thu nhỏ | Đổi cảm giác toàn app mà không đụng bố cục form |
| 3. Làm lại bố cục từng trang | Sửa khoảng 3.500 dòng ở 10 trang | Rủi ro lệch form cao, lợi ích thêm so với cách 2 nhỏ |

Claude chọn cách 2.

## Thiết kế

### Bảng màu (`namespace Mau` trong `ChuDe.h`)

Giữ nguyên **tên** các hằng để mọi trang tự đổi theo; chỉ đổi giá trị.

| Vai trò | Hằng | Giá trị mới |
| --- | --- | --- |
| Nền trang | `NEN` | `#FAF9F5` (250,249,245) |
| Bề mặt thẻ | `THE` | `#FFFFFF` |
| Viền thẻ / ô | `VIEN` / `VIEN_O` | `#E8E6DC` / `#DAD7CC` |
| Thanh bên | `THANH_BEN` | `#F0EEE6` |
| Mục thanh bên khi rê / được chọn | `THANH_BEN_RE` | `#E8E6DC` |
| Chữ thanh bên | `CHU_THANH_BEN` | `#3D3D3A` |
| Màu nhấn (đất nung) | `CHINH` / `CHINH_RE` / `CHINH_NHAN` | `#D97757` / `#C6613F` / `#B05530` |
| Nền nhấn nhạt | `CHINH_NHAT` / `PHU_NEN` / `PHU_RE` / `PHU_NHAN` | `#FBF3EF` / `#F5E9E2` / `#F0DDD2` / `#E8CDBE` |
| Chữ trên nền nhấn nhạt | `PHU_CHU` | `#9A4A2C` |
| Chữ chính / nhãn / mờ / khóa | `CHU` / `CHU_NHAN` / `CHU_MO` / `CHU_KHOA` | `#141413` / `#3D3D3A` / `#6B6A64` / `#B0AEA5` |
| Lỗi / thành công / cảnh báo | `LOI`, `TOT`, `CANH_BAO` (+ nền) | Giảm độ gắt: `#B54A3C`, `#4E7D55`, `#A9772A`; nền nhạt tương ứng |
| Nền phụ, ô chỉ đọc, nút khóa, dòng xen | `XAM_NEN`, `O_CHI_DOC`, `NUT_KHOA`, `DONG_XEN` | Các sắc ngà: `#F0EEE6`, `#F7F6F1`, `#EEECE5`, `#FBFAF7` |

Vì thanh bên chuyển sang màu sáng, chỗ nào đang vẽ chữ/biểu tượng màu trắng trên `THANH_BEN` (logo, mục được chọn, thẻ người dùng) phải đổi sang chữ tối. Danh sách: `CuaSo.cpp` (khoảng dòng 177–200), `DieuKhien.cpp`, `TrangDangNhap.cpp`, `TrangTongQuan.cpp`.

### Chữ (`taoBoFont`)

- Tiêu đề trang (`tieuDe`) và số liệu lớn (`so`): **Georgia** (có sẵn trên Windows), đậm vừa (`FW_SEMIBOLD`), giữ cỡ chữ để không vỡ chỗ đặt.
- Còn lại giữ Segoe UI, cỡ giữ nguyên.

### Hình khối

- Thẻ (`veThe`): bỏ 3 lớp bóng nâu; thay bằng viền 1px `VIEN` và một lớp bóng rất nhẹ (alpha ≤ 6). Bo góc giữ tham số người gọi truyền vào.
- Nút chính: nền `CHINH`, chữ trắng. Nút phụ: nền trắng, viền `VIEN_O`, chữ `CHU`; rê chuột nền `THANH_BEN`.
- Mục thanh bên được chọn: nền trắng, chữ `CHU` đậm, vạch nhấn `CHINH` 3px bên trái (thay khối màu cam đặc).
- Ô nhập: nền trắng, viền `VIEN_O`, khi đang nhập viền `CHINH`.
- Bảng: tiêu đề cột chữ `CHU_MO` trên nền `THANH_BEN`; dòng xen `DONG_XEN`; dòng chọn `CHINH_NHAT`.

### Tối ưu lưới món (`LuoiMon.cpp`, `AnhMon.cpp`)

- Hiện tại mỗi lần vẽ (cuộn, rê chuột) đều thu nhỏ ảnh 480×360 bằng nội suy bicubic cho mọi thẻ đang hiện. Thêm bộ đệm: với mỗi (ảnh, kích thước đích) giữ sẵn một bitmap đã thu nhỏ và bo góc; chỉ tạo lại khi kích thước thẻ đổi. Đệm bị xóa khi `xoaDemRieng()` được gọi.
- Rê chuột chỉ vẽ lại hai thẻ (thẻ cũ và thẻ mới) thay cho cả lưới.

## Kiểm tra

- Build không lỗi, không cảnh báo.
- Chụp màn hình tự động (đăng nhập bằng tài khoản mẫu) các trang: Đăng nhập, Tổng quan, Khách hàng, Nhân viên, Thực đơn, Bán hàng, Báo cáo, Tạo tài khoản, Đặt món (khách), Hóa đơn — ở cỡ mặc định và phóng to. So với ảnh trước khi đổi: không có ô/nút/chữ bị che hoặc lệch.
- Kiểm tra trạng thái: chọn thẻ, rê chuột, nút vô hiệu, thông báo lỗi.

## Để người làm tự quyết

- Giá trị màu cụ thể có thể chỉnh ±vài đơn vị nếu chữ không đủ tương phản khi nhìn thực tế.
- Cách cài đặt bộ đệm ảnh (khóa, nơi giữ) miễn đúng hành vi trên.
