# Quản lý nhà hàng — C++ / Win32 / SQL Server

Ứng dụng cửa sổ viết bằng C++14 và Windows Win32 API. Giao diện và nghiệp vụ không cần trình duyệt, Qt hoặc Python.

## Chạy chương trình

Nhấp đúp **QuanLyNhaHangGUI.exe**, hoặc chạy trong PowerShell tại thư mục dự án:

```powershell
.\QuanLyNhaHangGUI.exe
```

Dùng tài khoản đã tạo trước đó. Nếu bảng tài khoản còn trống, chương trình hướng dẫn tạo quản lý đầu tiên. Mật khẩu hiển thị bằng dấu `*` và được lưu dưới dạng băm PBKDF2-HMAC-SHA256.

Dữ liệu nghiệp vụ nằm trong **SQL Server**, mặc định `localhost\SQLEXPRESS`, database `QuanLyNhaHang`. File **DuLieu/sqlserver.ini** nằm cạnh exe để cấu hình kết nối. Ứng dụng dùng Windows Authentication và Microsoft ODBC Driver 18; không cần lưu mật khẩu SQL trong mã nguồn.

## Thiết lập và nhập dữ liệu bằng SQL

Dữ liệu ban đầu được viết thành các lệnh `INSERT INTO ... VALUES ...` trong **`database/du-lieu-ban-dau.sql`**. Bạn có thể sửa trực tiếp tên món, giá, tồn kho, hồ sơ và các dữ liệu khác trong file này. Không dùng file TXT hoặc công cụ C++ để nhập dữ liệu ban đầu.

Trên máy đã cài SQL Server Express, mở SSMS hoặc extension MSSQL trong VS Code và kết nối `localhost\SQLEXPRESS` bằng Windows Authentication. Chạy lần lượt:

1. `database/tao-database.sql`: tạo database `QuanLyNhaHang` nếu chưa có.
2. Chọn **QuanLyNhaHang** trong ô database của cửa sổ truy vấn, chạy `database/schema.sql`: tạo các bảng và ràng buộc.
3. Vẫn chọn **QuanLyNhaHang**, chạy `database/du-lieu-ban-dau.sql`: thêm dữ liệu bằng các lệnh INSERT cụ thể.
4. Chạy `database/xem-du-lieu.sql`: xem dữ liệu đã thêm.

File dữ liệu ban đầu là bản chụp dữ liệu đang có khi chuyển sang cách thiết lập bằng SQL, gồm cả lịch sử hóa đơn. Các ngày nhập sai trước đây được giữ nguyên để đối chiếu. Tài khoản giữ nguyên muối và mật khẩu băm; không gõ mật khẩu thường vào cột `MatKhauBam`.

**Database hiện tại của bạn đã có dữ liệu nên không cần nhập lại.** File `du-lieu-ban-dau.sql` chỉ nạp khi tất cả bảng nghiệp vụ còn trống; nếu đã có bất kỳ dữ liệu nào, toàn bộ bước nạp được bỏ qua. Nếu một lệnh INSERT lỗi, giao dịch rollback. Không xóa dữ liệu thật để chạy lại file mẫu.

### Thêm dữ liệu mới bằng câu lệnh SQL

Đóng ứng dụng nhà hàng, mở **New Query**, chọn database `QuanLyNhaHang` và chạy một câu INSERT mới, ví dụ:

```sql
INSERT INTO dbo.Mon
    (Ma, Loai, Ten, Gia, TonKho, DoCay, MonChay, SizeLy, CoDa)
VALUES
    (N'MA04', N'MONAN', N'Cơm sườn', 45000, 50, 1, 0, N'', 0);
```

Chỉ dùng mã chưa tồn tại. Mở lại ứng dụng để tải dữ liệu mới. Sửa file SQL mà chưa Execute sẽ không thay đổi database; sửa file dữ liệu ban đầu cũng không cập nhật các bản ghi đã tồn tại.

### Thiết lập nhanh bằng PowerShell (tùy chọn)

Nếu không muốn chạy từng file trong SSMS, có thể dùng:

```powershell
.\setup-sqlserver.ps1
.\build.ps1
.\QuanLyNhaHangGUI.exe
```

Script PowerShell chỉ chạy các file SQL tạo bảng và thêm dữ liệu, sau đó ghi cấu hình kết nối. Không biên dịch hay gọi công cụ nhập dữ liệu C++. Để dùng tên database khác:

```powershell
.\setup-sqlserver.ps1 -Server 'localhost\SQLEXPRESS' -Database 'QuanLyNhaHang'
```

Ứng dụng vẫn cần Microsoft ODBC Driver 18 for SQL Server và lớp C++ `SqlServer` để đọc/ghi dữ liệu trong quá trình sử dụng. `TrustServerCertificate=Yes` trong cấu hình dành cho máy cục bộ dùng chứng chỉ tự ký; với máy chủ khác, dùng chứng chỉ hợp lệ và đặt `No`.

## Cách lưu dữ liệu

- `Module5_CoSoDuLieu` dùng ODBC Unicode và tham số SQL để giữ tiếng Việt, dấu nháy và dữ liệu tài khoản chính xác.
- Dữ liệu được lưu theo cột trong `KhachHang`, `NhanVien`, `Mon`, `TaiKhoan`, `HoaDon`, `ChiTietHoaDon`, `DonDatMon`, `ChiTietDuTinh`, `ChiTietDonDat`. `CauHinh` theo dõi phiên bản schema.
- Các hàm `toDuLieu()` chuyển đổi đối tượng thành dữ liệu trong bộ nhớ cho kho SQL, không đọc/ghi tệp. Kho dữ liệu so sánh bản đã đọc với bản mới, chỉ INSERT/UPDATE/DELETE những dòng thay đổi.
- Thanh toán lưu hóa đơn, trừ kho, cộng điểm và cập nhật đơn trong **một transaction**. Nếu một câu SQL lỗi, toàn bộ lần lưu rollback và bộ nhớ được khôi phục.
- Tiền trong SQL dùng `DECIMAL`, văn bản dùng `NVARCHAR`. Hóa đơn lưu tên và đơn giá tại thời điểm bán. `NgayGoc` giữ ngày cũ; cột tính toán `NgayLap`/`NgayDat` là ngày chuẩn hoặc NULL nếu không chuyển đổi được. Báo cáo giao diện vẫn áp dụng kiểm tra ngày của chương trình.
- Khóa ngoại nối các bảng chi tiết với hóa đơn/đơn đặt. Mã khách, nhân viên, món trong lịch sử được giữ ngay cả khi hồ sơ bị xóa, phù hợp hành vi cũ; không tự xóa lịch sử theo hồ sơ.
- **Một cửa sổ ứng dụng cho mỗi database**: khóa SQL `sp_getapplock` ngăn mở đồng thời từ bản sao/thư mục khác. Nếu sửa dữ liệu bằng SSMS khi ứng dụng đang mở, lần lưu tiếp theo phát hiện thay đổi và hủy; đóng/mở ứng dụng để tải lại. Đây chưa phải bản hỗ trợ nhiều quầy bán hàng cùng lúc.
- Khi chuyển máy, cần sao lưu/khôi phục database SQL Server và cập nhật file kết nối; chỉ chép `DuLieu` không mang theo dữ liệu nghiệp vụ mới.

## Kiểm tra tích hợp

```powershell
.\tests\run-sqlserver-tests.ps1
```

Bộ kiểm tra tạo database tạm tên `PBL2_SQL_Test_<GUID>`, kiểm tra CRUD, tiếng Việt/dấu nháy, quyền, đăng ký, đổi mật khẩu, đặt/hủy/thanh toán đơn, tồn kho, điểm, thanh toán trùng, thiếu kho, khôi phục khi lỗi SQL, xung đột từ SSMS, tải lại và báo cáo. Database tạm được xóa khi kết thúc; dữ liệu thật không bị sửa.


## Chức năng

Tài khoản **Quản lý** phụ trách khách hàng, nhân viên, thực đơn và tồn kho, tài khoản, báo cáo và xem hóa đơn. Tài khoản **Nhân viên** phụ trách bán hàng, xử lý đơn khách đặt và thanh toán. Quản lý không có mục Bán hàng hay Đơn khách đặt.

Trang **Tổng quan** của quản lý hiển thị biểu đồ đường doanh thu từ tháng 1 đến tháng 12 thay cho bảng tồn kho thấp. Chọn **Năm doanh thu** để xem các năm có hóa đơn hoặc năm hiện tại; rê chuột lên điểm để xem số tiền tháng đó. Doanh thu lấy từ hóa đơn đã thanh toán sau giảm giá; tháng không có hóa đơn có doanh thu 0. Hóa đơn có ngày hoặc số tiền không hợp lệ được bỏ qua và ghi rõ số lượng bên dưới biểu đồ. Phần tồn kho thấp vẫn xem được trong **Báo cáo**.

| Chức năng | Quản lý | Nhân viên | Khách hàng |
| --- | --- | --- | --- |
| Quản lý danh sách khách hàng | Có | Có | Không |
| Xem, tìm kiếm, lọc và sắp xếp thực đơn | Có | Có | Có |
| Thêm/sửa/xóa món, nhập kho | Có | Không | Không |
| Quản lý hồ sơ, lương nhân viên | Có | Không | Không |
| Lập và thanh toán hóa đơn | Không | Có | Không |
| Gửi đơn đặt món, xem và hủy đơn chờ của mình | Không | Không | Có |
| Xem, hủy và thanh toán đơn khách đặt | Không | Có | Không |
| Báo cáo, doanh thu, lịch sử toàn bộ hóa đơn | Có | Không | Không |
| Xem điểm, hạng thành viên và hồ sơ của mình | Không | Không | Có |
| Xem hóa đơn của mình | Không | Không | Có |
| Tạo tài khoản | Có | Không | Không |
| Đổi mật khẩu của mình, đăng xuất | Có | Có | Có |

- Chọn dòng trong bảng để sửa thông tin; **Thêm mới** làm trống biểu mẫu.
- Trang **Thực đơn** có hai mục riêng **Thức ăn** và **Nước uống**, mỗi mục hiển thị số món của nhóm. Các thẻ món có ảnh, tên, giá và thông tin riêng (độ cay/món chay hoặc size/đá). Tìm kiếm trong nhóm, chỉ hiện món còn hàng và sắp xếp tên/giá vẫn dùng được. Quản lý chọn thẻ món để sửa thông tin hoặc nhập kho; khách không thấy số lượng tồn kho.
- Giỏ hàng chưa trừ tồn kho. Tồn kho, giảm giá VIP và điểm tích lũy được cập nhật khi xác nhận thanh toán.
- Hóa đơn hiển thị bằng bảng món, cột số lượng/đơn giá/thành tiền và khung tổng thanh toán.
- Nhân viên lập hóa đơn bằng mã gắn với tài khoản của mình.
- Báo cáo có nút lọc nhanh **Hôm nay**, **Tháng này**, **Tất cả**. Có thể nhập `dd/mm/yyyy` hoặc `mm/yyyy` rồi nhấn **Áp dụng** (hoặc Enter). Ba ô thống kê hiển thị doanh thu sau giảm giá, số hóa đơn đã thanh toán và món bán chạy theo kỳ đang xem. Bảng tồn kho bên phải luôn phản ánh tồn kho hiện tại; màu đỏ là hết hàng, màu vàng là sắp hết.
- Tài khoản nhân viên và khách hàng cần gắn với hồ sơ đang tồn tại. Nếu hồ sơ bị xóa, tài khoản tương ứng không đăng nhập được.
- Đổi mật khẩu cần nhập đúng mật khẩu hiện tại. Sau ba lần đăng nhập sai liên tiếp, mở lại ứng dụng để thử lại.

## Biên dịch

Cần MinGW-w64 có `g++` và `windres` trong PATH. Đóng ứng dụng trước khi biên dịch lại:

```powershell
.\build.ps1
```

Trong VS Code, **Ctrl+Shift+B** biên dịch toàn bộ dự án; tác vụ **Nha hang: Chay giao dien** mở chương trình.

Build tạo một bản chạy chính là `QuanLyNhaHangGUI.exe`. Thư viện C++ được liên kết tĩnh. `odbc32`, `bcrypt`, `comctl32`, `gdi32`, `uxtheme`, `gdiplus`, `ole32` là thư viện Windows. Tệp `.rc` và `.manifest` là tài nguyên hiển thị Windows, được nhúng vào exe. Hai logo gốc nằm trong `Module7_CuaSo/Anh` và cũng được nhúng vào exe; khi chạy không cần chép ảnh riêng.

## Cấu trúc mã nguồn

- **main.cpp:** mở giao diện cửa sổ và xử lý lỗi khởi động.
- **Module1_ConNguoi:** lớp cha Nguoi, KhachHang, NhanVien; thông tin cá nhân, VIP, lương. Hàm `toDuLieu()` của mỗi lớp chuyển dữ liệu thành chuỗi để lưu.
- **Module2_ThucDon:** lớp cha Mon, MonAn, NuocUong; giá, tồn kho và thuộc tính riêng.
- **Module3_BanHang:** hóa đơn, chi tiết món, tổng tiền và giảm giá.
- **Module4_QuanLy:** danh sách trong bộ nhớ, tìm đối tượng theo mã và đọc dữ liệu.
- **Module5_CoSoDuLieu:** kết nối SQL Server, bộ chuyển đổi dữ liệu, transaction và cập nhật dữ liệu.
- **Module6_TaiKhoan:** tài khoản, mật khẩu băm và lưu tài khoản vào SQL Server.
- **Module7_CuaSo:** cửa sổ, biểu mẫu, bảng, nút bấm, giỏ hàng và hóa đơn. `DichVuNhaHang` nối giao diện với các lớp nghiệp vụ và kiểm tra quyền.

Chỉ giữ giao diện cửa sổ. Các menu, hàm nhập/xuất console và bản xem thử tạm đã được loại bỏ.

## Tài khoản khách hàng

Khách hàng mới có thể tự đăng ký ngay tại màn hình đăng nhập:

1. Nhấn **Đăng ký khách hàng**.
2. Nhập họ tên, số điện thoại gồm 9–15 chữ số, giới tính, tên đăng nhập và mật khẩu từ 8–128 ký tự; nhập lại mật khẩu để xác nhận.
3. Nhấn **Đăng ký**, sau đó đăng nhập bằng tài khoản vừa tạo.

Chương trình tự tạo mã khách hàng và hồ sơ hạng **Thường**, bắt đầu với **0 điểm**. Tài khoản tự đăng ký chỉ có quyền khách hàng. Tên đăng nhập và số điện thoại không được trùng dữ liệu đã có. Nếu đã có hồ sơ tại nhà hàng, liên hệ nhân viên hoặc quản lý để được cấp tài khoản liên kết đúng hồ sơ cũ.

Quản lý vẫn có thể cấp tài khoản cho khách đã có hồ sơ:

1. Quản lý đăng nhập, mở **Khách hàng** và tạo hồ sơ nếu chưa có.
2. Mở **Tạo tài khoản**, chọn vai trò **Khách hàng**.
3. Chọn đúng mã/họ tên trong **Hồ sơ liên kết**, nhập tên đăng nhập và mật khẩu, rồi nhấn **Tạo tài khoản**.
4. Đăng xuất. Khách đăng nhập bằng tài khoản vừa được cấp.

Khách có các trang **Tổng quan**, **Thực đơn**, **Đặt món**, **Đơn đặt của tôi**, **Thông tin của tôi**, **Hóa đơn của tôi**, **Đổi mật khẩu**. Một hồ sơ khách chỉ được gắn với một tài khoản khách hàng. Khách chưa có hóa đơn sẽ thấy danh sách trống; khi nhân viên thanh toán đơn đặt hoặc bán hàng chọn đúng mã khách, hóa đơn sẽ xuất hiện trong tài khoản của khách.

Điểm, hạng thành viên và thông tin cá nhân do nhà hàng quản lý, khách không tự sửa. Khách liên hệ nhân viên để cập nhật hồ sơ. Tài khoản cũ vẫn dùng được, không cần tạo lại hay đổi tệp dữ liệu. Không có mật khẩu mặc định cho tài khoản khách hàng.

## Đặt món cho khách hàng

1. Đăng nhập tài khoản khách hàng, mở **Đặt món**.
2. Chọn nhóm **Thức ăn** hoặc **Nước uống**. Mỗi món có ảnh, tên và giá; món hết hàng có thông báo riêng, không hiển thị số lượng tồn cho khách. Có thể tìm theo tên hoặc mã trong nhóm đang xem và cuộn danh sách để xem thêm.
3. Chọn món, nhập số lượng; với nước uống chọn size và có đá/không đá, rồi nhấn **Thêm món đã chọn** (hoặc nhấp đúp món). Có thể xóa từng món hoặc hủy giỏ trước khi gửi. Chuyển nhóm không làm mất giỏ hàng; món hết hàng không thêm được.
4. Nhấn **Gửi đơn đặt món** và xác nhận số tiền tạm tính. Đơn được gắn với khách đang đăng nhập, bắt đầu ở trạng thái **Chờ xử lý**.
5. Mở **Đơn đặt của tôi** để xem chi tiết và trạng thái. Khách chỉ xem được đơn của mình và có thể hủy đơn còn chờ.
6. Nhân viên đăng nhập, mở **Đơn khách đặt**, chọn đơn. Nhấn **Thanh toán đơn** để xác nhận tổng tiền và lưu hóa đơn, hoặc **Hủy đơn đã chọn** nếu không phục vụ được.

Đơn chờ chưa giữ hoặc trừ tồn kho, chưa cộng điểm và chưa tính doanh thu. Giá, ưu đãi thành viên và tồn kho được kiểm tra lại khi thanh toán; nếu thiếu món, không thanh toán được đơn. Thanh toán thành công tạo hóa đơn, trừ kho, cộng điểm và chuyển đơn sang **Đã thanh toán** trong cùng lần lưu. Đơn đã hủy hoặc đã thanh toán không được xử lý lần nữa.

Đơn đặt được lưu trong SQL Server. Mỗi database chỉ mở một cửa sổ chương trình; đăng xuất để chuyển giữa tài khoản khách và nhân viên. Chưa có giao diện đặt món qua Internet.

### Ảnh món ăn

Ảnh minh họa cơm gà, rau xào, mì tôm, trà đào và nước cam được tạo bằng công cụ imagegen, lưu ở `Module7_CuaSo/Anh/anh_mon_an.png` và nhúng trong exe. Món chưa có ảnh riêng dùng ảnh đĩa/bộ đồ ăn trung tính.

Có thể dùng ảnh thật của nhà hàng bằng cách đặt tệp PNG tại `DuLieu/AnhMon/<ma_mon>.png`, ví dụ `DuLieu/AnhMon/MA01.png`. Ảnh riêng được ưu tiên; đóng rồi mở lại chương trình sau khi thay ảnh. Ảnh thiếu hoặc không đọc được sẽ dùng ảnh minh họa có sẵn. Cả phần hiển thị ảnh và giao diện đều dùng C++/Win32/GDI+, không cần trình duyệt hay thư viện giao diện mới.

Thư mục `.build` được tự tạo khi build hoặc chạy kiểm thử, chỉ chứa tệp tạm và có thể xóa. Dữ liệu thực và tài khoản nằm trong SQL Server; `DuLieu` chứa cấu hình và ảnh tùy chọn.

## Thứ tự đọc mã nguồn

1. Đọc `Nguoi`, `KhachHang`, `NhanVien`, `Mon`, `MonAn`, `NuocUong` để hiểu thuộc tính, kế thừa và cách tính điểm/giá.
2. Đọc `HoaDon` để hiểu giỏ hàng, tổng tiền và giảm giá.
3. Đọc `QuanLy`: các vector giữ đối tượng trong bộ nhớ; mỗi hàm `docThucDon`, `docKhachHang`, `docNhanVien`, `docHoaDon` đọc một nhóm dữ liệu SQL.
4. Đọc `DichVuNhaHang::thanhToan`: kiểm tra quyền và giỏ hàng → lập hóa đơn → trừ kho → cộng điểm → gọi `luuThayDoi`.
5. Đọc `SqlServer::luu`: kiểm tra dữ liệu có bị sửa từ ngoài không → gọi `luuCacThayDoi` → COMMIT; có lỗi thì ROLLBACK.
6. Đọc các hàm tạo trang trong `Module7_CuaSo` và phần xử lý sự kiện trong `CuaSo.cpp` sau cùng.

Trong `SqlServer.cpp`, các hàm `themDongSQL`, `suaDongSQL`, `xoaDongSQL` tạo câu lệnh INSERT, UPDATE, DELETE. Dấu `?` là chỗ dành cho tham số, được truyền qua ODBC. `DongDuLieu` là vector các cột; `BangDuLieu` dùng mã làm khóa tra cứu; `DuLieuSQL` chứa các bảng theo tên. Các chuỗi chuyển đổi chỉ tồn tại trong bộ nhớ, dữ liệu vẫn lưu hoàn toàn trong SQL Server.

Các lớp nghiệp vụ dùng hàm và vòng lặp thông thường. `unique_ptr` trong danh sách món tự giải phóng bộ nhớ cho `MonAn`/`NuocUong`; các hàm ODBC và Win32 là phần gọi thư viện cần thiết cho kết nối SQL và giao diện Windows.
