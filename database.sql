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
INSERT INTO [dbo].[DonDat] ([thuTu], [noiDung]) VALUES (7, N'"XONG" "HD_DM1" "DM1|15/11/2026|KH0002||0.000000|MA0001,Mỳ cay ,3,10000" 1 "MA0001" 3 "" 0')
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
INSERT INTO [dbo].[HoaDon] ([ma], [ngay], [maKhach], [maNhanVien], [giamGia], [chiTiet]) VALUES (N'HD20261120-720211546', N'20/11/2026', N'KH0001', N'NV0001', 0.000000, N'MA0002,Mỳ Quảng ,3,30000;MA0001,Mỳ cay ,1,10000')
INSERT INTO [dbo].[HoaDon] ([ma], [ngay], [maKhach], [maNhanVien], [giamGia], [chiTiet]) VALUES (N'HD_DM1', N'15/11/2026', N'KH0002', N'NV0001', 0.000000, N'MA0001,Mỳ cay ,3,10000')
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
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0001', N'Tô Đông Phước', N'0905554790', N'Nam', 10, N'todongphuoc2008@gmail.com ')
INSERT INTO [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0002', N'Nguyễn Văn A', N'0905554798', N'Nam', 3, N'HAI@124')
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
INSERT INTO [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0001', N'Lê Văn Tài ', N'0905995740', N'Nam', N'Nhân viên ', 100.000000, 100.000000)
GO
CREATE TABLE [dbo].[TaiKhoan] (
    [tenDangNhap] varchar(50) NOT NULL,
    [matKhau] nvarchar(100) NULL,
    [vaiTro] varchar(20) NULL,
    [maLienKet] varchar(50) NULL,
    PRIMARY KEY ([tenDangNhap])
)
GO
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'DongHaiTo01', N'123456789', N'KHACHHANG', N'KH0002')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'SeaSide', N'HAItd2007', N'QUANLY', N'')
INSERT INTO [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'SeaSide01', N'123456789', N'NHANVIEN', N'NV0001')
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
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0001', N'Mỳ cay ', 10000.000000, 37, N'5', 0, N'DuLieu/AnhMon/mi-kim-chi-thap-cam.png')
INSERT INTO [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0002', N'Mỳ Quảng ', 30000.000000, 22, N'3', 0, N'DuLieu/AnhMon/55334-mi-quang-ba-lu-da-nang-1.jpg')
GO
