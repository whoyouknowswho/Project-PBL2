📁 QuanLyNhaHang/
│
├── 📜 main.cpp                      ─── [Điểm bắt đầu chạy chương trình]
│
├── 📂 [MODULE 1: QUẢN LÝ CON NGƯỜI]
│   ├── 📄 Nguoi.h      &  Nguoi.cpp       ─── (Lớp cha: Ma, Ten, SDT, GioiTinh)
│   ├── 📄 KhachHang.h  &  KhachHang.cpp   ─── (Kế thừa Nguoi: Tích điểm, Hạng VIP)
│   └── 📄 NhanVien.h   &  NhanVien.cpp    ─── (Kế thừa Nguoi: Chức vụ, Bảng lương)
│
├── 📂 [MODULE 2: QUẢN LÝ THỰC ĐƠN]
│   ├── 📄 Mon.h        &  Mon.cpp         ─── (Lớp cha trừu tượng: Mã, Tên, Giá, Tồn kho)
│   ├── 📄 MonAn.h      &  MonAn.cpp       ─── (Kế thừa Mon: Độ cay, Món chay)
│   └── 📄 NuocUong.h   &  NuocUong.cpp     ─── (Kế thừa Mon: Size ly, Có đá/Không đá)
│
├── 📂 [MODULE 3: BÁN HÀNG & HÓA ĐƠN]
│   └── 📄 HoaDon.h     &  HoaDon.cpp      ─── (Xử lý giỏ hàng, giảm giá, in hóa đơn)
│
├── 📂 [MODULE 4: TRUNG TÂM ĐIỀU KHIỂN & CSDL]
│   └── 📄 QuanLy.h     &  QuanLy.cpp      ─── (Menu hệ thống, kết nối logic và đọc/ghi file)
│
└── 📂 [CƠ SỞ DỮ LIỆU FILE .TXT]
    ├── 📝 menu.txt                       ─── (Lưu trữ dữ liệu: Món ăn & Nước uống)
    ├── 📝 khachhang.txt                  ─── (Lưu trữ dữ liệu: Khách hàng & Điểm số)
    ├── 📝 nhanvien.txt                   ─── (Lưu trữ dữ liệu: Nhân viên & Lương)
    └── 📝 hoadon.txt                     ─── (Lưu trữ dữ liệu: Lịch sử hóa đơn đã bán)