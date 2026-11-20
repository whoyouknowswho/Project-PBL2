# Giao diện phong cách Claude — Kế hoạch thực hiện

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Đổi giao diện app sang phong cách Claude (nền ngà, nhấn đất nung, tiêu đề Georgia, thanh bên sáng, viền mảnh) và làm lưới món vẽ nhanh hơn, không đổi bố cục form.

**Architecture:** Mọi màu đi qua `namespace Mau` trong `ChuDe.h` nên đổi giá trị ở đó lan ra toàn app. Những chỗ đang giả định thanh bên tối (chữ trắng trên `THANH_BEN`) được sửa riêng. Lưới món thêm bộ đệm ảnh đã thu nhỏ trong `KhoAnh` và chỉ vẽ lại thẻ bị đổi khi rê chuột.

**Tech Stack:** C++14, Win32, GDI/GDI+, MinGW `g++`, PowerShell 5.1 (`System.Drawing`) cho kiểm tra.

**Spec:** `docs/superpowers/specs/2026-11-20-giao-dien-claude-design.md`

## Global Constraints

- Không dời vị trí/kích thước ô nhập, nút, bảng trong các trang.
- Không đổi nghiệp vụ, SQL, quyền; không thêm thư viện.
- Giữ nguyên tên các hằng trong `namespace Mau` (chỉ đổi giá trị, được thêm hằng mới).
- Build bằng `.\build.ps1` (trong `TEST_SQL`) phải không lỗi, không cảnh báo mới.
- Tương phản tối thiểu (WCAG): chữ thường ≥ 4.5:1, chữ khóa/mờ trên nền ≥ 3:1 (`CHU_KHOA` được miễn, vì là trạng thái vô hiệu).

## Review Focus

1. Chữ trắng còn sót trên nền đã chuyển sang sáng (thanh bên, banner chào khách, tooltip biểu đồ, màn đăng nhập) → phải đổi sang chữ tối hoặc nền tối riêng `NEN_TOI`. Kiểm bằng ảnh chụp Task 6.
2. Cửa sổ thấp hoặc hẹp (thẻ món co lại) → bộ đệm ảnh phải tạo lại theo kích thước mới, không kéo giãn ảnh cũ. Kiểm ở Task 5 bằng chụp sau khi đổi cỡ cửa sổ.
3. Thay ảnh món trên đĩa rồi chọn lại món → `xoaDemRieng()` phải xóa cả bộ đệm ảnh thu nhỏ. Kiểm ở Task 5.
4. Thẻ món "hết hàng" (`t.mo`) vẫn phủ mờ đúng sau khi dùng bộ đệm. Kiểm ở Task 5.
5. Nút vô hiệu, ô chỉ đọc, tab đang chọn vẫn phân biệt được với nút thường trên nền ngà. Kiểm ở Task 6.

---

### Task 1: Ảnh chụp "trước" và công cụ đo

**Files:**
- Create: `TEST_SQL/KiemTra/chup_giao_dien.ps1` — mở app, đăng nhập, chụp các trang ra thư mục chỉ định.
- Create: `TEST_SQL/KiemTra/do_cuon_luoi.ps1` — đo thời gian vẽ lưới món khi cuộn.

**Interfaces:**
- Produces: `chup_giao_dien.ps1 -Exe <exe> -Ra <thu_muc> [-PhongTo]` chụp: màn đăng nhập; `QUANLY` → Tổng quan(100), Khách hàng(101), Nhân viên(102), Thực đơn(103), Báo cáo(105), Tạo tài khoản(106); `nhanvien01` → Bán hàng(104), Đơn khách đặt(112); `khachhang01` → Tổng quan(100), Đặt món(111), Thông tin của tôi(109). Tên tệp `<tk>_<idTrang>.png`. Đăng nhập: `WM_SETTEXT` vào ô 350/351, `WM_COMMAND` nút 410; chuyển trang: `WM_COMMAND` id trang; chụp bằng `PrintWindow(h, dc, 2)`.
- Produces: `do_cuon_luoi.ps1 -Exe <exe>` đăng nhập `QUANLY`, mở Thực đơn, lấy lưới (`GetDlgItem(h, 216)`), gửi 60 lần `WM_VSCROLL SB_LINEDOWN` + `RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW)`, in tổng mili-giây.

- [ ] **Step 1:** Viết 2 script trên (dựa vào `chup_app.ps1` trong scratchpad đã chạy được ở phiên trước).
- [ ] **Step 2:** Chạy `chup_giao_dien.ps1 -Ra KiemTra\truoc` và `-PhongTo -Ra KiemTra\truoc_to`. Expected: 13 ảnh mỗi thư mục, mở được.
- [ ] **Step 3:** Chạy `do_cuon_luoi.ps1` 3 lần, ghi trung bình vào `KiemTra\ket_qua.txt` dòng `truoc: <ms>`.
- [ ] **Step 4:** Thêm `TEST_SQL/KiemTra/truoc*/`, `sau*/` vào `.gitignore` (ảnh không commit), commit 2 script.

### Task 2: Bảng màu, font, thẻ

**Files:**
- Modify: `TEST_SQL/Module7_CuaSo/ChuDe.h` (namespace `Mau`)
- Modify: `TEST_SQL/Module7_CuaSo/ChuDe.cpp` (`taoBoFont`, `veThe`)
- Create: `TEST_SQL/KiemTra/kiem_tra_tuong_phan.cpp`

**Interfaces:**
- Produces: hằng mới `Mau::NEN_TOI = RGB(38,37,34)`, `Mau::CHU_TREN_TOI = RGB(240,238,230)`, `Mau::CHU_TREN_TOI_MO = RGB(176,174,165)` cho các khối tối còn giữ lại.
- Values: đúng bảng màu trong spec (mục "Bảng màu"); lỗi/tốt/cảnh báo: `LOI=RGB(181,74,60)`, `LOI_NEN=RGB(250,236,232)`, `LOI_RE=RGB(245,218,211)`, `TOT=RGB(78,125,85)`, `TOT_NEN=RGB(234,242,234)`, `CANH_BAO=RGB(169,119,42)`, `CANH_BAO_NEN=RGB(250,242,222)`.

- [ ] **Step 1: Viết test tương phản** `kiem_tra_tuong_phan.cpp` (include `../Module7_CuaSo/ChuDe.h`, hàm `doTuongPhan(COLORREF, COLORREF)` theo WCAG). Assert:
  - `CHU/NEN ≥ 4.5`, `CHU_NHAN/NEN ≥ 4.5`, `CHU_MO/NEN ≥ 4.5`, `CHU_MO/THE ≥ 4.5`
  - `CHU_THANH_BEN/THANH_BEN ≥ 4.5`, `PHU_CHU/PHU_NEN ≥ 4.5`
  - `trắng/CHINH ≥ 3.0` (chữ nút đậm), `LOI/LOI_NEN ≥ 4.5`, `TOT/TOT_NEN ≥ 4.5`, `CANH_BAO/CANH_BAO_NEN ≥ 3.0`
  - `CHU_TREN_TOI/NEN_TOI ≥ 7.0`
- [ ] **Step 2:** `g++ -std=c++14 KiemTra/kiem_tra_tuong_phan.cpp -o %TEMP%\tp.exe && %TEMP%\tp.exe` → FAIL (chưa có `NEN_TOI`).
- [ ] **Step 3:** Đổi giá trị trong `ChuDe.h`, thêm 3 hằng mới.
- [ ] **Step 4:** `taoBoFont`: `tieuDe = taoFont(26, FW_SEMIBOLD, L"Georgia")`, `so = taoFont(24, FW_SEMIBOLD, L"Georgia")`. `veThe`: thay 3 lớp bóng bằng 1 lớp `{1, 2, 6}` màu `(20,20,19)`, giữ `veHinhBoGoc(..., Mau::THE, Mau::VIEN, 1.0f)`.
- [ ] **Step 5:** Chạy lại test → in `DAT`. Nếu `CHU_MO` thiếu tương phản, hạ độ sáng tới khi đạt (spec cho phép ± vài đơn vị).
- [ ] **Step 6:** `.\build.ps1` → `Built:`. Commit.

### Task 3: Thanh bên sáng và các khối tối

**Files:**
- Modify: `TEST_SQL/Module7_CuaSo/CuaSo.cpp:176-198` (`veThanhBen`)
- Modify: `TEST_SQL/Module7_CuaSo/DieuKhien.cpp:521-531` (nút `KieuNut::ThanhBen`)
- Modify: `TEST_SQL/Module7_CuaSo/TrangDangNhap.cpp:7,64-95` (panel trái màn đăng nhập)
- Modify: `TEST_SQL/Module7_CuaSo/TrangTongQuan.cpp:94-102, 265-268` (banner chào khách, hộp tooltip doanh thu)

**Interfaces:**
- Consumes: `Mau::NEN_TOI`, `Mau::CHU_TREN_TOI`, `Mau::CHU_TREN_TOI_MO` (Task 2).

- [ ] **Step 1:** `veThanhBen`: chữ "NHÀ HÀNG" → `Mau::CHU`; tên người dùng → `Mau::CHU`; thẻ người dùng nền `Mau::THE` viền `Mau::VIEN`; đường kẻ dùng `Mau::VIEN`; thêm đường kẻ dọc 1px `Mau::VIEN` ở mép phải thanh bên. Icon trong vòng tròn `CHINH` vẫn trắng.
- [ ] **Step 2:** Nút thanh bên: đang ở trang → nền `Mau::THE` + viền `Mau::VIEN`, vạch `Mau::CHINH` rộng 3px cao (chiều cao − 16) sát mép trái, chữ `Mau::CHU` font `nut`, icon `Mau::CHINH`; rê/nhấn → nền `Mau::THANH_BEN_RE`, chữ `Mau::CHU`; thường → chữ `Mau::CHU_THANH_BEN`.
- [ ] **Step 3:** Màn đăng nhập: `veChuyenSac(dc, trai, Mau::NEN_TOI, Mau::CHINH_NHAN)`; bỏ hằng `NAU_DO`; chữ trắng trên panel → `Mau::CHU_TREN_TOI`.
- [ ] **Step 4:** Tổng quan: banner chào khách và hộp tooltip doanh thu dùng `Mau::NEN_TOI` (banner chuyển sắc sang `Mau::CHINH_NHAN`), chữ phụ `Mau::CHU_TREN_TOI_MO`, chữ chính `Mau::CHU_TREN_TOI`.
- [ ] **Step 5:** `grep -n "THANH_BEN" Module7_CuaSo/*.cpp` → chỉ còn ở `CuaSo.cpp` (nền thanh bên, `BoCuc::THANH_BEN`) và `DieuKhien.cpp` (`mauNenTai`, nút thanh bên). Build. Commit.

### Task 4: Nút, ô nhập, hộp chọn, bảng

**Files:**
- Modify: `TEST_SQL/Module7_CuaSo/DieuKhien.cpp:463-481` (`veKhungNhap`), `533-577` (`veNut`), `580-` (`veHopChon`), `170-200` (vẽ dòng bảng), `80-100` (tiêu đề cột)

- [ ] **Step 1:** Nút `default` (phụ): nền `Mau::THE`, viền `Mau::VIEN_O`, chữ `Mau::CHU`; rê → nền `Mau::THANH_BEN`; nhấn → nền `Mau::THANH_BEN_RE`. Bo góc mọi nút/ô: 8 (chip giữ tròn).
- [ ] **Step 2:** `Tab`/`Chip`: rê → viền `Mau::CHU_MO` (không dùng `CHINH`). `TabChon`/`ChipChon` giữ nền `CHINH`.
- [ ] **Step 3:** Ô nhập & hộp chọn: bo 8; vòng sáng khi đang nhập giữ (`alpha 45`) nhưng bo 11.
- [ ] **Step 4:** Bảng: nền tiêu đề cột `Mau::THANH_BEN`, chữ `Mau::CHU_MO` font `nhoDam`; dòng chọn nền `Mau::CHINH_NHAT`, chữ `Mau::CHU`.
- [ ] **Step 5:** Build. Commit.

### Task 5: Bộ đệm ảnh lưới món

**Files:**
- Modify: `TEST_SQL/Module7_CuaSo/AnhMon.h`, `AnhMon.cpp`
- Modify: `TEST_SQL/Module7_CuaSo/LuoiMon.cpp` (`veLuoi`, `WM_MOUSEMOVE`, `WM_MOUSELEAVE`)

**Interfaces:**
- Produces: trong `KhoAnh` thêm `map<string, Gdiplus::Bitmap*> demThuNho;` khóa `ma|hinhAnh|rong|cao|banKinh`. `veAnhMon` giữ chữ ký cũ; bên trong: nếu có trong `demThuNho` thì `Graphics::DrawImage(bmp, x, y)` không co giãn; nếu chưa, tạo bitmap `PixelFormat32bppPARGB` cỡ đích, vẽ ảnh nguồn (hoặc ô minh họa) với `veCat` vào đó (nền trong suốt, clip bo góc), lưu rồi vẽ. `xoaDemRieng()` và `giaiPhong()` xóa cả `demThuNho`.
- Produces (LuoiMon.cpp): hàm nội bộ `void veLaiThe(HWND h, const TrangThaiLuoi& tt, int i)` gọi `InvalidateRect(h, &vungThe(...), FALSE)` khi `i >= 0`.

- [ ] **Step 1:** Cài bộ đệm như trên.
- [ ] **Step 2:** `WM_MOUSEMOVE`/`WM_MOUSELEAVE`: thay `InvalidateRect(h, nullptr, FALSE)` bằng `veLaiThe` cho thẻ cũ và thẻ mới (mở rộng vùng 3px để gồm viền rê).
- [ ] **Step 3:** Build. Chạy `do_cuon_luoi.ps1` 3 lần, ghi `sau: <ms>` vào `KiemTra\ket_qua.txt`. Expected: nhỏ hơn `truoc`.
- [ ] **Step 4 (Review Focus 2,3,4):** Chụp Thực đơn ở cỡ mặc định, rồi đổi cửa sổ sang 1000×700 (`SetWindowPos`) và chụp lại: ảnh món không méo/không lệch. Chụp trang Đặt món của `khachhang01` có bật "Chỉ hiện món còn hàng" tắt: thẻ hết hàng (nếu có) vẫn phủ mờ. Thay tạm `DuLieu/AnhMon/MA0003.jpg` bằng bản lật ngang, mở lại trang Thực đơn (gọi `xoaDemRieng` qua chọn lưu món) → ảnh mới hiện; trả ảnh cũ.
- [ ] **Step 5:** Commit.

### Task 6: Ảnh chụp "sau", so sánh, hoàn tất

- [ ] **Step 1:** Chạy `chup_giao_dien.ps1 -Ra KiemTra\sau` và `-PhongTo -Ra KiemTra\sau_to`.
- [ ] **Step 2:** Ghép từng cặp trước/sau thành ảnh cạnh nhau, xem từng trang. Đạt khi: không ô/nút/chữ nào bị che, lệch, mất; không còn chữ trắng trên nền sáng; nút vô hiệu, ô chỉ đọc, tab chọn phân biệt được (Review Focus 1, 5).
- [ ] **Step 3:** Sửa điểm hỏng (nếu có), build, chụp lại trang đó.
- [ ] **Step 4:** README: thêm 2–3 dòng mô tả bảng màu mới ở phần `Module7_CuaSo`. Commit spec + plan + README.
