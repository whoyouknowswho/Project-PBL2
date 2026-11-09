# Thiết kế giao diện mới — PBL_Claude

Ngày: 09/11/2026 · Phạm vi: chỉ thư mục `PBL_Claude` (bản sao của `PBLmain`).

## 1. Mục tiêu

Viết lại toàn bộ phần giao diện Win32 của ứng dụng quản lý nhà hàng để đẹp, dễ nhìn hơn, **không đổi bất kỳ chức năng, quyền hay dữ liệu nào**.

Thành công khi:

- Mọi chức năng của 3 vai trò (Quản lý, Nhân viên, Khách hàng) hoạt động giống bản cũ (danh sách kiểm tra ở mục 6).
- Dữ liệu trong `DuLieu/` đọc/ghi y như cũ; không đổi định dạng tệp.
- Vẫn là C++14 + Win32 + GDI/GDI+, build bằng `build.ps1` ra một tệp `QuanLyNhaHangGUI.exe`, không thêm thư viện ngoài.
- Build không có cảnh báo mới với `-Wall -Wextra`.

Ngoài phạm vi: đổi nghiệp vụ, thêm chức năng mới, đổi định dạng dữ liệu, sửa `PBLmain`.

## 2. Giữ nguyên / xóa / viết mới

**Giữ nguyên, không sửa:** `main.cpp`, `build.ps1`, Module1–Module6, `Module7_CuaSo/DichVuNhaHang.h`, `DichVuNhaHang.cpp`, `DatMonDichVu.cpp`, `UngDung.rc`, `UngDung.manifest`, `Anh/*`.

**Xóa (code giao diện cũ):** `CuaSo.cpp`, `BieuMau.cpp`, `BanHangCuaSo.cpp`, `BaoCaoCuaSo.cpp`, `BieuDoDoanhThu.cpp`, `DangKyCuaSo.cpp`, `DangNhapCuaSo.cpp`, `DatMonCuaSo.cpp`, `HoaDonCuaSo.cpp`, `KhachHangCuaSo.cpp`, `ThucDonAnh.cpp`, `ThucDonKhachHang.cpp`, `Logo.cpp`, `Logo.h`; `CuaSo.h` được viết lại hoàn toàn (giữ tên vì `main.cpp` include nó và dùng `rong()`, `chayGiaoDienCuaSo()`).

**Viết mới (cùng thư mục `Module7_CuaSo`, để `build.ps1` tự nhận):**

| Tệp | Trách nhiệm |
| --- | --- |
| `CuaSo.h` | Lớp `CuaSoNhaHang`, mã điều khiển, khai báo tiện ích (`rong`, `hep`, `tien`, `so`, `homNay`, `maHoaDonMoi`, `xemHoaDon`, `chayGiaoDienCuaSo`). |
| `ChuDe.h/.cpp` | Bảng màu, font, hàm vẽ GDI+ khử răng cưa (thẻ bo góc, bóng đổ, huy hiệu, icon). |
| `TienIch.cpp` | Các hàm chuyển chuỗi/tiền/ngày — cùng kết quả như bản cũ. |
| `DieuKhien.cpp` | Tạo và vẽ điều khiển: ô nhập, nút, hộp chọn, ô đánh dấu, bảng. |
| `LuoiMon.h/.cpp` | Điều khiển lưới thẻ món có ảnh, tự cuộn, chọn, nhấp đúp. |
| `AnhMon.h/.cpp` | Nạp logo/ảnh món từ tài nguyên và `DuLieu/AnhMon/<ma>.png`. |
| `CuaSo.cpp` | Cửa sổ chính, vòng lặp tin nhắn, chuyển trang, thanh bên, đầu trang, chân trang. |
| `TrangDangNhap.cpp` | Đăng nhập, thiết lập lần đầu, đăng ký khách hàng. |
| `TrangTongQuan.cpp` | Tổng quan của 3 vai trò + biểu đồ doanh thu. |
| `TrangDanhSach.cpp` | Khách hàng, Nhân viên (bảng + biểu mẫu). |
| `TrangThucDon.cpp` | Thực đơn (lưới món + biểu mẫu của quản lý). |
| `TrangBanHang.cpp` | Bán hàng (nhân viên) và Đặt món (khách) — dùng chung giỏ hàng. |
| `TrangDonDat.cpp` | Đơn khách đặt / Đơn đặt của tôi. |
| `TrangBaoCao.cpp` | Báo cáo kinh doanh. |
| `TrangCaNhan.cpp` | Thông tin của tôi, Hóa đơn của tôi, Đổi mật khẩu, Tạo tài khoản. |
| `CuaSoHoaDon.cpp` | Cửa sổ xem hóa đơn. |

`README.md` chỉ sửa phần "Cấu trúc mã nguồn" cho khớp tên tệp mới.

## 3. Phong cách: "Ấm áp kiểu nhà hàng"

| Vai trò màu | Mã màu |
| --- | --- |
| Nền ứng dụng (kem) | `#FAF6F0` |
| Thẻ / bề mặt | `#FFFFFF`, viền `#EDE4D8`, bóng đổ mờ |
| Thanh bên (nâu espresso) | `#2B1D16`; mục rê chuột `#3D2A20`; mục đang chọn: nền cam đất, chữ trắng |
| Màu chính (cam đất) | `#D9622B`; rê chuột `#C4551F`; nhấn `#A84719` |
| Nút phụ | nền `#FBEDE3`, chữ `#8A3B12` |
| Nguy hiểm | chữ `#C0392B`, nền `#FDECEA` |
| Thành công | `#2E8B57`, nền `#E6F4EC` |
| Cảnh báo | `#B7791F`, nền `#FFF5DB` |
| Chữ chính / chữ phụ | `#2D2420` / `#8A7B70` |

- Font: Segoe UI (cỡ 14/15/17/26), icon: font hệ thống **Segoe MDL2 Assets** (có sẵn trên Windows 10/11; nếu thiếu thì ẩn icon, chữ vẫn hiển thị).
- Bo góc 10–14 px, vẽ bằng GDI+ khử răng cưa. Cửa sổ chính vẽ qua bộ đệm (double-buffer) để không nhấp nháy.

## 4. Bố cục

### 4.1 Màn đăng nhập / đăng ký / thiết lập lần đầu

- Nửa trái: nền chuyển sắc espresso → cam đất, vòng tròn trang trí mờ; thẻ trắng chứa 2 logo (Bách Khoa, CNTT) và tên trường/khoa như cũ; tiêu đề lớn "Hệ thống quản lý nhà hàng" và dòng mô tả.
- Nửa phải: thẻ biểu mẫu căn giữa, ô nhập phẳng bo góc có nhãn phía trên, viền đổi sang cam khi đang nhập; nút chính màu cam, nút phụ màu kem.
- Ba chế độ giữ nguyên nội dung và trường như bản cũ (đăng nhập; thiết lập quản lý đầu tiên có thêm "Nhập lại mật khẩu"; đăng ký khách có Họ tên, SĐT, Giới tính, Email, Tên đăng nhập, Mật khẩu, Nhập lại).

### 4.2 Khung sau khi đăng nhập

```
┌────────────┬──────────────────────────────────────────────┐
│  🍽 NHÀ HÀNG │  Tiêu đề trang                   [📅 09/11/2026] │
│  Không gian │  Mô tả ngắn                                   │
│            ├──────────────────────────────────────────────┤
│  ⌂ Tổng quan│                                              │
│  👥 Khách   │            Nội dung trang (các thẻ)            │
│  ...        │                                              │
│            │                                              │
│ ┌────────┐ │                                              │
│ │(A) tên │ │                                              │
│ │ vai trò│ ├──────────────────────────────────────────────┤
│ └────────┘ │  ⓘ Dòng trạng thái                             │
│  ⎋ Đăng xuất│                                              │
└────────────┴──────────────────────────────────────────────┘
```

- Thanh bên rộng 248 px: logo chữ + icon, danh sách mục có icon (cùng danh sách và thứ tự mục theo vai trò như bản cũ), thẻ người dùng (vòng tròn chữ cái đầu, tên đăng nhập, huy hiệu vai trò), nút Đăng xuất.
- Đầu trang: tiêu đề + mô tả (giữ nguyên câu chữ bản cũ), ô ngày bên phải.
- Chân trang: dòng trạng thái (giữ các thông báo như bản cũ) có icon.
- Cửa sổ tối thiểu 1120 × 760, mặc định 1320 × 880 (tự thu nhỏ theo màn hình), như cũ.

### 4.3 Các trang

- **Tổng quan:** 4 thẻ số liệu có icon tròn màu nhạt (nội dung thẻ theo vai trò giữ nguyên như bản cũ), hàng nút thao tác nhanh.
  - Quản lý: biểu đồ doanh thu 12 tháng — đường cong mượt màu cam, vùng tô chuyển sắc, lưới mờ, hộp chọn năm, rê chuột hiện thẻ số tiền, ghi chú hóa đơn bị bỏ qua.
  - Nhân viên: bảng "Tồn kho thấp" (huy hiệu đỏ "Hết hàng" / vàng "Sắp hết").
  - Khách: dải chào mừng + bảng lịch sử mua hàng.
- **Khách hàng / Nhân viên:** trái là thẻ chứa ô tìm kiếm + bảng; phải là thẻ "Thông tin chi tiết" với cùng trường và nút như bản cũ (Lưu thông tin, Xóa, Thêm mới / Làm trống).
- **Thực đơn:** ô tìm kiếm, hộp sắp xếp, 2 nút nhóm dạng "tab" có số đếm (Thức ăn (n) / Nước uống (n)), ô "Chỉ hiện món còn hàng"; bên dưới là **lưới thẻ món** (ảnh trên, tên, giá cam, dòng thông tin, huy hiệu Hết hàng / Tồn kho cho quản lý). Quản lý có thêm thẻ biểu mẫu bên phải (giữ đủ trường, Nhập kho).
- **Bán hàng (nhân viên):** hàng trên là 4 ô thông tin hóa đơn (Mã HĐ tự động, Ngày lập, Khách hàng, Nhân viên). Trái: tìm món + 2 tab nhóm + lưới thẻ món (hiện "Còn n"); dưới lưới là Số lượng (có nút − / +), Size (chỉ hiện với nước uống), nút "Thêm món đã chọn". Phải: thẻ Giỏ hàng (bảng), khung tổng tiền vẽ đẹp (Tạm tính, Giảm, **Thanh toán** chữ lớn), nút Xóa món / Hủy giỏ / Thanh toán & lưu hóa đơn.
  - *Thay đổi nhỏ có chủ ý:* danh sách món đổi từ bảng sang lưới thẻ có 2 tab nhóm (giống Đặt món), vẫn tìm theo tên/mã và vẫn hiện số còn lại.
- **Đặt món (khách):** cùng bố cục với Bán hàng nhưng không có hàng thông tin hóa đơn, không hiện tồn kho (chỉ "Đã hết món"), nút cuối là "Gửi đơn đặt món", có thông báo giỏ trống.
- **Đơn khách đặt / Đơn đặt của tôi:** thanh nút (Làm mới, Hủy đơn đã chọn, Thanh toán đơn — chỉ nhân viên); bảng đơn với **huy hiệu trạng thái màu** (Chờ xử lý vàng, Đã thanh toán xanh, Đã hủy xám); bảng chi tiết đơn bên dưới.
- **Báo cáo:** thẻ bộ lọc (ô ngày/tháng, Áp dụng, 3 nút lọc nhanh dạng "chip" — chip đang chọn tô cam), dòng "Đang xem", 3 thẻ số liệu, 2 bảng (Danh sách hóa đơn + nút Xem hóa đơn đã chọn; Cần nhập thêm với màu đỏ/vàng), dòng tổng kết.
- **Thông tin của tôi:** thẻ hồ sơ hai cột, chỉ đọc (các ô hiển thị dạng chữ trên nền kem nhạt, không phải ô nhập), có huy hiệu hạng thành viên.
- **Hóa đơn của tôi:** nút "Xem hóa đơn đã chọn" + bảng (Mã, Ngày, Số loại món, Thanh toán).
- **Đổi mật khẩu / Tạo tài khoản:** thẻ biểu mẫu rộng 520 px, cùng trường và gợi ý như bản cũ.
- **Cửa sổ hóa đơn:** kiểu "biên lai": nền kem, thẻ trắng; đầu có tên nhà hàng + huy hiệu "ĐÃ THANH TOÁN"; lưới 4 thông tin (Mã, Ngày, Khách, Nhân viên); bảng món (#, Món/Mã, SL, Đơn giá, Thành tiền); khối tổng với Tổng tiền chữ lớn màu cam; nút "Đóng hóa đơn"; Esc để đóng; cửa sổ chính bị khóa khi hóa đơn mở (như cũ).

### 4.4 Điều khiển dùng chung

- **Ô nhập:** EDIT không viền đặt giữa một khung bo góc do cửa sổ cha vẽ; khung đổi màu khi có tiêu điểm; giữ nguyên giới hạn độ dài, `ES_NUMBER`, bộ lọc ký tự số thực/ngày, ký tự che mật khẩu `*`, gợi ý (cue banner).
- **Nút:** owner-draw, có trạng thái rê chuột / nhấn / vô hiệu / tiêu điểm bàn phím. Kiểu: chính, phụ, nguy hiểm, mục thanh bên, tab/chip.
- **Hộp chọn:** `CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS`, vẽ phẳng cùng phong cách, cao 40 px.
- **Ô đánh dấu:** nút owner-draw tự lưu trạng thái, vẽ ô vuông bo góc có dấu ✓.
- **Bảng:** ListView báo cáo, dòng cao ~40 px, tiêu đề tự vẽ (chữ in hoa nhỏ, nền kem), dòng xen màu, dòng chọn màu cam nhạt, không kẻ lưới; ô trạng thái vẽ thành huy hiệu.
- **Lưới thẻ món (`LuoiMon`):** cửa sổ con tự vẽ; số cột tự tính theo chiều rộng (thẻ ≥ 200 px); thanh cuộn dọc + con lăn chuột; nhấp để chọn, nhấp đúp để thêm vào giỏ; gửi cho cửa sổ cha cùng thông báo `LBN_SELCHANGE` / `LBN_DBLCLK` như listbox cũ để giữ luồng xử lý; trạng thái rỗng "Không có món phù hợp".
- **Hộp thoại:** vẫn dùng `MessageBoxW` của Windows cho xác nhận/lỗi (ổn định, đúng chuẩn, tránh lỗi).

## 5. Kiến trúc và luồng dữ liệu

- Giữ mô hình bản cũ: `hienTrang(trang, giuNhap)` xóa điều khiển con rồi dựng trang mới; khi đổi kích thước cửa sổ thì dựng lại sau 180 ms và giữ giá trị đang nhập. Mô hình này đã chạy đúng, viết lại theo đúng cách đó để giảm rủi ro.
- Mọi thao tác nghiệp vụ gọi qua `DichVuNhaHang` với cùng tham số như bản cũ; giao diện không tự ghi tệp.
- Lỗi: mọi ngoại lệ trong thủ tục cửa sổ được bắt và hiện `MessageBoxW` "Vui lòng kiểm tra" như cũ.
- GDI+ khởi tạo một lần khi chạy và tắt khi thoát; mọi đối tượng GDI tạo ra đều được giải phóng.

## 6. Danh sách hành vi phải giữ (dùng để kiểm thử)

1. Khóa một phiên chạy cho mỗi thư mục dữ liệu; tự chuyển thư mục làm việc về cạnh exe.
2. Chưa có tài khoản → chế độ "Thiết lập lần đầu" tạo tài khoản Quản lý, sau đó điền sẵn tên đăng nhập.
3. Đăng nhập sai 3 lần liên tiếp → báo lỗi và đóng ứng dụng; sai dưới 3 lần → xóa ô mật khẩu, báo lỗi.
4. Enter ở màn đăng nhập/đăng ký kích hoạt nút đang có tiêu điểm hoặc nút chính (trừ khi hộp Giới tính đang mở).
5. Đăng ký khách: kiểm tra qua dịch vụ, báo mã khách mới, quay lại đăng nhập và điền sẵn tên.
6. Thanh bên và quyền truy cập trang theo vai trò giống hệt bản cũ (Quản lý không có Bán hàng/Đơn khách đặt; Khách chỉ có 7 trang của mình; Nhân viên không có Nhân viên/Báo cáo/Tạo tài khoản).
7. Đăng xuất và đóng cửa sổ: hỏi xác nhận; cảnh báo riêng khi giỏ hàng chưa trống; đăng xuất xóa giỏ và trạng thái tạm.
8. Khách hàng/Nhân viên: tìm theo mã/tên/SĐT (khách thêm email); chọn dòng nạp biểu mẫu; mã chỉ đọc khi sửa; Xóa chỉ bật khi đang sửa và hỏi xác nhận; Lưu xong giữ chế độ sửa; Thêm mới làm trống.
9. Thực đơn: tìm, sắp xếp (Mặc định, Tên A–Z, Giá tăng, Giá giảm), nhóm có số đếm, "Chỉ hiện món còn hàng"; quản lý thấy tồn kho, người khác chỉ thấy "Hết hàng"; đổi Loại bật/tắt Độ cay–Món chay hoặc Size; Nhập kho chỉ bật khi đang sửa.
10. Bán hàng: mã HĐ tự sinh chỉ đọc; ngày sửa được (chỉ số và `/`); khách mặc định "Khách lẻ"; mã nhân viên của phiên, chỉ đọc; các giá trị này giữ lại khi chuyển trang rồi quay lại.
11. Thêm vào giỏ: phải chọn món; số lượng 1 ≤ SL ≤ tồn − đã có trong giỏ; nước uống lấy size; gộp dòng cùng món + size; số còn lại hiển thị = tồn − trong giỏ.
12. Tổng giỏ: tạm tính, giảm theo % của khách đang chọn (khách đăng nhập khi Đặt món), thanh toán; đổi khách cập nhật tổng; các nút giỏ bị vô hiệu khi giỏ trống.
13. Thanh toán: xem trước → xác nhận số tiền → lưu → sinh mã mới → mở cửa sổ hóa đơn.
14. Đặt món: nút Thêm bị vô hiệu khi chưa chọn món hoặc món hết; nhấp đúp thêm món; đổi nhóm không mất giỏ; Gửi đơn xác nhận số tạm tính rồi chuyển sang trang Đơn đặt.
15. Đơn đặt: mới nhất ở đầu; chọn đơn hiện chi tiết; Hủy/Thanh toán chỉ bật với đơn "Chờ xử lý"; khách chỉ hủy; nhân viên thanh toán có xác nhận và mở hóa đơn.
16. Báo cáo: lọc `dd/mm/yyyy`, `mm/yyyy` hoặc trống; Enter trong ô lọc = Áp dụng; lọc lỗi giữ kết quả cũ; chip đang chọn được tô; hóa đơn mới nhất ở đầu; nhấp đúp hoặc nút để xem hóa đơn; bảng tồn kho ≤ 5 (đỏ Hết hàng, vàng còn ít); dòng tổng kết.
17. Biểu đồ: danh sách năm = các năm có hóa đơn + năm hiện tại, giảm dần; thang trục tự làm tròn và đổi đơn vị (đồng/nghìn/triệu/tỷ); rê chuột vào điểm hiện tháng + số tiền; ghi chú số hóa đơn bỏ qua; "Chưa có doanh thu trong năm này" khi trống.
18. Khách: Tổng quan hiện điểm, hạng (tên tiếng Việt), ưu đãi %, số hóa đơn; hồ sơ chỉ đọc; Hóa đơn của tôi xem chi tiết bằng nút hoặc nhấp đúp.
19. Tạo tài khoản: đổi vai trò nạp lại danh sách hồ sơ liên kết; vai trò Quản lý → "Không cần liên kết hồ sơ" và vô hiệu hộp chọn.
20. Đổi mật khẩu: gọi dịch vụ với mật khẩu cũ/mới/nhập lại, xong làm trống biểu mẫu.
21. Ảnh món: ưu tiên `DuLieu/AnhMon/<ma>.png` (mã chỉ gồm chữ, số, `_`, `-`); nếu không có thì cắt ảnh minh họa có sẵn theo tên món (cơm gà, rau xào, mì tôm, trà đào, nước cam, còn lại là ảnh mặc định).
22. Cửa sổ hóa đơn: khách trống → "Khách lẻ", nhân viên trống → "—"; dòng "n món · m phần"; giảm giá hiện dấu "−" khi > 0.

## 7. Kiểm thử

- Build bằng `build.ps1`, không có cảnh báo mới.
- Chạy thử trên **bản sao dữ liệu thử** trong thư mục tạm (chép exe + `DuLieu` với `taikhoan.txt` trống), không đụng dữ liệu thật trong `PBL_Claude/DuLieu`. Tự động hóa bằng PowerShell: điền ô qua `WM_SETTEXT`, bấm nút qua `WM_COMMAND` theo mã điều khiển, chụp màn hình bằng `PrintWindow`.
- Kịch bản: thiết lập quản lý → tạo nhân viên + khách → đi qua từng trang của mỗi vai trò → bán hàng, đặt món, thanh toán đơn, xem báo cáo/biểu đồ/hóa đơn → kiểm tra tệp dữ liệu sau thao tác đúng như kỳ vọng. Đối chiếu từng mục ở phần 6.
- Kiểm tra đổi kích thước cửa sổ (tối thiểu và phóng to) không vỡ bố cục.
