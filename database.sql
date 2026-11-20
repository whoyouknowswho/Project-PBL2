-- Tao boi TEST_SQL/Module5_SQL/xuat_du_lieu.ps1. Khong sua tay; nap bang nhap_du_lieu_cua_ban.ps1.
USE [master]
GO
CREATE DATABASE [QuanLyNhaHang_PBL2]
GO
USE [QuanLyNhaHang_PBL2]
GO
CREATE TABLE [dbo].[DonDat] (
    [thuTu] bigint IDENTITY(1,1) NOT NULL,
    [noiDung] nvarchar(max) NULL,
    PRIMARY KEY ([thuTu])
)
GO
SET IDENTITY_INSERT [dbo].[DonDat] ON
INSERT INTO [dbo].[DonDat] ([thuTu], [noiDung]) VALUES (14, N'"XONG" "HD_DM1" "DM1|15/11/2026|KH0002||0.000000|MA0001,Mỳ cay ,3,10000" 1 "MA0001" 3 "" 0')
SET IDENTITY_INSERT [dbo].[DonDat] OFF
GO
CREATE TABLE [dbo].[HoaDon] (
    [ma] nvarchar(160) NOT NULL,
    [ngay] nvarchar(255) NULL,
    [maKhach] nvarchar(255) NULL,
    [maNhanVien] nvarchar(255) NULL,
    [giamGia] decimal(28, 6) NULL,
    [chiTiet] nvarchar(max) NULL,
    PRIMARY KEY ([ma])
)
GO
INSERT INTO [dbo].[HoaDon] ([ma], [ngay], [maKhach], [maNhanVien], [giamGia], [chiTiet]) VALUES (N'HD_DM1', N'15/11/2026', N'KH0002', N'NV0001', 0.000000, N'MA0001,Mỳ cay ,3,10000')
INSERT INTO [dbo].[HoaDon] ([ma], [ngay], [maKhach], [maNhanVien], [giamGia], [chiTiet]) VALUES (N'HD20261120-720211546', N'20/11/2026', N'KH0001', N'NV0001', 0.000000, N'MA0002,Mỳ Quảng ,3,30000;MA0001,Mỳ cay ,1,10000')
GO
CREATE TABLE [dbo].[KhachHang] (
    [ma] nvarchar(160) NOT NULL,
    [ten] nvarchar(255) NULL,
    [dienThoai] nvarchar(255) NULL,
    [gioiTinh] nvarchar(255) NULL,
    [diem] int NULL,
    [email] nvarchar(255) NULL,
    PRIMARY KEY ([ma])
)
GO
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0001', N'Tô Đông Phước', N'0905554790', N'Nam', 10, N'todongphuoc2008@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0002', N'Nguyễn Văn A', N'0905554798', N'Nam', 3, N'nguyenvana@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0003', N'Lê Thị Hồng', N'0935111203', N'Nữ', 120, N'lethihong03@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0004', N'Trần Minh Đức', N'0935111204', N'Nam', 250, N'tranminhduc04@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0005', N'Phạm Ngọc Ánh', N'0935111205', N'Nữ', 40, N'phamngocanh05@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0006', N'Ngô Văn Phúc', N'0935111206', N'Nam', 560, N'ngovanphuc06@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0007', N'Đỗ Thị Hạnh', N'0935111207', N'Nữ', 0, N'dothihanh07@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0008', N'Hoàng Gia Bảo', N'0935111208', N'Nam', 1050, N'hoanggiabao08@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0009', N'Vũ Thị Kim Chi', N'0935111209', N'Nữ', 75, N'vuthikimchi09@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0010', N'Phan Thanh Sơn', N'0935111210', N'Nam', 210, N'phanthanhson10@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0011', N'Trịnh Mỹ Linh', N'0935111211', N'Nữ', 15, N'trinhmylinh11@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0012', N'Mai Xuân Trường', N'0935111212', N'Nam', 330, N'maixuantruong12@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0013', N'Cao Thị Yến', N'0935111213', N'Nữ', 0, N'caothiyen13@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0014', N'Dương Văn Lực', N'0935111214', N'Nam', 640, N'duongvanluc14@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0015', N'Lâm Bảo Ngọc', N'0935111215', N'Nữ', 90, N'lambaongoc15@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0016', N'Tạ Quang Vinh', N'0935111216', N'Nam', 5, N'taquangvinh16@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0017', N'Châu Thị Diễm', N'0935111217', N'Nữ', 205, N'chauthidiem17@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0018', N'La Hữu Thắng', N'0935111218', N'Nam', 1200, N'lahuuthang18@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0019', N'Kiều Anh Thư', N'0935111219', N'Nữ', 60, N'kieuanhthu19@gmail.com')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0020', N'Quách Tuấn Kiệt', N'0935111220', N'Nam', 0, N'quachtuankiet20@gmail.com')
GO
CREATE TABLE [dbo].[NhanVien] (
    [ma] nvarchar(160) NOT NULL,
    [ten] nvarchar(255) NULL,
    [dienThoai] nvarchar(255) NULL,
    [gioiTinh] nvarchar(255) NULL,
    [chucVu] nvarchar(255) NULL,
    [luong] decimal(28, 6) NULL,
    [phuCap] decimal(28, 6) NULL,
    PRIMARY KEY ([ma])
)
GO
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0001', N'Lê Văn Tài', N'0795881745', N'Nam', N'Thu ngân', 7000000.000000, 500000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0002', N'Trần Thị Mai', N'0912345602', N'Nữ', N'Phục vụ', 6000000.000000, 400000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0003', N'Nguyễn Hoàng Nam', N'0912345603', N'Nam', N'Đầu bếp', 12000000.000000, 1000000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0004', N'Phạm Thu Hà', N'0912345604', N'Nữ', N'Pha chế', 7500000.000000, 500000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0005', N'Võ Minh Khang', N'0912345605', N'Nam', N'Phụ bếp', 6500000.000000, 400000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0006', N'Đặng Ngọc Lan', N'0912345606', N'Nữ', N'Thu ngân', 7000000.000000, 500000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0007', N'Bùi Quốc Bảo', N'0912345607', N'Nam', N'Phục vụ', 6000000.000000, 400000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0008', N'Huỳnh Thị Thảo', N'0912345608', N'Nữ', N'Phục vụ', 6000000.000000, 400000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0009', N'Lý Gia Huy', N'0912345609', N'Nam', N'Giám sát ca', 9000000.000000, 800000.000000)
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0010', N'Hồ Thanh Tâm', N'0912345610', N'Nữ', N'Pha chế', 7500000.000000, 500000.000000)
GO
CREATE TABLE [dbo].[TaiKhoan] (
    [tenDangNhap] varchar(50) NOT NULL,
    [matKhau] nvarchar(100) NULL,
    [vaiTro] varchar(20) NULL,
    [maLienKet] varchar(50) NULL,
    PRIMARY KEY ([tenDangNhap])
)
GO
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang01', N'123456789', N'KHACHHANG', N'KH0001')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang02', N'123456789', N'KHACHHANG', N'KH0002')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang03', N'123456789', N'KHACHHANG', N'KH0003')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang04', N'123456789', N'KHACHHANG', N'KH0004')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang05', N'123456789', N'KHACHHANG', N'KH0005')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang06', N'123456789', N'KHACHHANG', N'KH0006')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang07', N'123456789', N'KHACHHANG', N'KH0007')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang08', N'123456789', N'KHACHHANG', N'KH0008')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang09', N'123456789', N'KHACHHANG', N'KH0009')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang10', N'123456789', N'KHACHHANG', N'KH0010')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang11', N'123456789', N'KHACHHANG', N'KH0011')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang12', N'123456789', N'KHACHHANG', N'KH0012')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang13', N'123456789', N'KHACHHANG', N'KH0013')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang14', N'123456789', N'KHACHHANG', N'KH0014')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang15', N'123456789', N'KHACHHANG', N'KH0015')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang16', N'123456789', N'KHACHHANG', N'KH0016')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang17', N'123456789', N'KHACHHANG', N'KH0017')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang18', N'123456789', N'KHACHHANG', N'KH0018')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang19', N'123456789', N'KHACHHANG', N'KH0019')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'khachhang20', N'123456789', N'KHACHHANG', N'KH0020')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien01', N'123456789', N'NHANVIEN', N'NV0001')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien02', N'123456789', N'NHANVIEN', N'NV0002')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien03', N'123456789', N'NHANVIEN', N'NV0003')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien04', N'123456789', N'NHANVIEN', N'NV0004')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien05', N'123456789', N'NHANVIEN', N'NV0005')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien06', N'123456789', N'NHANVIEN', N'NV0006')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien07', N'123456789', N'NHANVIEN', N'NV0007')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien08', N'123456789', N'NHANVIEN', N'NV0008')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien09', N'123456789', N'NHANVIEN', N'NV0009')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'nhanvien10', N'123456789', N'NHANVIEN', N'NV0010')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'QUANLY', N'123456789', N'QUANLY', N'')
GO
CREATE TABLE [dbo].[ThucDon] (
    [loai] nvarchar(255) NULL,
    [ma] nvarchar(160) NOT NULL,
    [ten] nvarchar(255) NULL,
    [gia] decimal(28, 6) NULL,
    [tonKho] int NULL,
    [doCayHoacSize] nvarchar(255) NULL,
    [monChayHoacCoDa] int NULL,
    [hinhAnh] nvarchar(500) NULL,
    PRIMARY KEY ([ma])
)
GO
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0001', N'Mỳ Cay', 10000.000000, 37, N'5', 0, N'DuLieu/AnhMon/mi-kim-chi-thap-cam.png')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0002', N'Mỳ Quảng', 30000.000000, 22, N'3', 0, N'DuLieu/AnhMon/55334-mi-quang-ba-lu-da-nang-1.jpg')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0003', N'Phở bò tái', 45000.000000, 40, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0004', N'Phở gà', 40000.000000, 40, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0005', N'Bún bò Huế', 45000.000000, 35, N'3', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0006', N'Bún chả Hà Nội', 45000.000000, 30, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0007', N'Bún riêu cua', 40000.000000, 30, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0008', N'Bún thịt nướng', 40000.000000, 30, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0009', N'Bún mắm nêm', 40000.000000, 25, N'2', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0010', N'Hủ tiếu Nam Vang', 45000.000000, 30, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0011', N'Mì xào hải sản', 55000.000000, 25, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0012', N'Mì Quảng gà', 40000.000000, 30, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0013', N'Cao lầu', 45000.000000, 20, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0014', N'Cơm tấm sườn bì chả', 50000.000000, 40, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0015', N'Cơm gà xối mỡ', 45000.000000, 35, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0016', N'Cơm chiên dương châu', 40000.000000, 35, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0017', N'Cơm chiên hải sản', 55000.000000, 25, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0018', N'Cơm sườn rim', 45000.000000, 30, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0019', N'Cơm bò lúc lắc', 65000.000000, 20, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0020', N'Cơm chay thập cẩm', 35000.000000, 25, N'0', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0021', N'Bánh mì thịt nướng', 25000.000000, 50, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0022', N'Bánh mì ốp la', 20000.000000, 50, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0023', N'Bánh xèo miền Tây', 40000.000000, 25, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0024', N'Bánh cuốn nóng', 35000.000000, 30, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0025', N'Bánh canh cua', 50000.000000, 25, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0026', N'Bánh bèo chén', 30000.000000, 30, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0027', N'Gỏi cuốn tôm thịt', 30000.000000, 40, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0028', N'Gỏi cuốn chay', 25000.000000, 30, N'0', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0029', N'Chả giò rế', 35000.000000, 40, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0030', N'Nem lụi Huế', 45000.000000, 25, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0031', N'Gỏi ngó sen tôm thịt', 60000.000000, 20, N'2', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0032', N'Gỏi gà xé phay', 55000.000000, 20, N'2', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0033', N'Lẩu thái hải sản', 250000.000000, 15, N'4', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0034', N'Lẩu gà lá é', 220000.000000, 15, N'2', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0035', N'Lẩu nấm chay', 180000.000000, 15, N'0', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0036', N'Cá kho tộ', 60000.000000, 20, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0037', N'Thịt kho trứng', 55000.000000, 25, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0038', N'Canh chua cá lóc', 60000.000000, 20, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0039', N'Gà nướng muối ớt', 120000.000000, 15, N'3', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0040', N'Sườn nướng mật ong', 90000.000000, 20, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0041', N'Bò nướng lá lốt', 70000.000000, 25, N'1', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0042', N'Mực xào sa tế', 85000.000000, 20, N'4', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0043', N'Tôm rang muối', 95000.000000, 20, N'2', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0044', N'Ốc len xào dừa', 65000.000000, 20, N'2', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0045', N'Rau muống xào tỏi', 35000.000000, 40, N'0', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0046', N'Đậu hũ sốt cà chua', 35000.000000, 30, N'0', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0047', N'Đậu hũ tứ xuyên', 45000.000000, 25, N'5', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0048', N'Cháo lòng', 35000.000000, 25, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0049', N'Cháo gà hành', 35000.000000, 25, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0050', N'Xôi gà', 30000.000000, 30, N'0', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0051', N'Mì cay hải sản', 55000.000000, 30, N'5', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0052', N'Khoai tây chiên', 30000.000000, 50, N'0', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0001', N'Cà phê đen đá', 20000.000000, 80, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0002', N'Cà phê sữa đá', 25000.000000, 80, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0003', N'Bạc xỉu', 28000.000000, 60, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0004', N'Cà phê muối', 32000.000000, 50, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0005', N'Cà phê cốt dừa', 35000.000000, 40, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0006', N'Espresso', 30000.000000, 40, N'S', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0007', N'Cappuccino', 40000.000000, 40, N'M', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0008', N'Latte', 42000.000000, 40, N'M', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0009', N'Americano', 35000.000000, 40, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0010', N'Mocha', 45000.000000, 30, N'M', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0011', N'Trà đào cam sả', 35000.000000, 60, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0012', N'Trà vải', 35000.000000, 50, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0013', N'Trà chanh', 15000.000000, 80, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0014', N'Trà tắc', 15000.000000, 80, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0015', N'Trà sữa trân châu', 30000.000000, 60, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0016', N'Trà sữa matcha', 35000.000000, 50, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0017', N'Trà sữa khoai môn', 35000.000000, 40, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0018', N'Hồng trà sữa', 30000.000000, 50, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0019', N'Trà xanh nhài', 20000.000000, 50, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0020', N'Trà ô long', 25000.000000, 40, N'M', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0021', N'Nước cam ép', 30000.000000, 40, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0022', N'Nước ép dưa hấu', 30000.000000, 40, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0023', N'Nước ép táo', 35000.000000, 30, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0024', N'Nước ép ổi', 30000.000000, 30, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0025', N'Nước ép cà rốt', 30000.000000, 30, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0026', N'Nước ép thơm', 30000.000000, 30, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0027', N'Sinh tố bơ', 40000.000000, 30, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0028', N'Sinh tố xoài', 35000.000000, 30, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0029', N'Sinh tố dâu', 40000.000000, 30, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0030', N'Sinh tố mãng cầu', 40000.000000, 25, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0031', N'Sinh tố sapoche', 35000.000000, 25, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0032', N'Nước dừa tươi', 25000.000000, 40, N'L', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0033', N'Nước mía', 15000.000000, 60, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0034', N'Rau má đậu xanh', 25000.000000, 40, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0035', N'Sâm bí đao', 15000.000000, 50, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0036', N'Chanh dây', 25000.000000, 40, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0037', N'Soda chanh', 25000.000000, 40, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0038', N'Soda việt quất', 35000.000000, 30, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0039', N'Sữa chua đá', 25000.000000, 40, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0040', N'Yaourt trái cây', 35000.000000, 30, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0041', N'Cacao đá', 30000.000000, 40, N'M', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0042', N'Socola nóng', 35000.000000, 30, N'M', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0043', N'Matcha đá xay', 45000.000000, 30, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0044', N'Cookie đá xay', 45000.000000, 30, N'L', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0045', N'Nước suối', 10000.000000, 100, N'S', 0, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0046', N'Coca-Cola', 15000.000000, 100, N'S', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0047', N'Pepsi', 15000.000000, 100, N'S', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0048', N'7Up', 15000.000000, 100, N'S', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0049', N'Bia Huda', 20000.000000, 100, N'S', 1, N'')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'NUOC', N'NU0050', N'Bia Saigon', 20000.000000, 100, N'S', 1, N'')
GO
