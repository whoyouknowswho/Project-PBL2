IF DB_ID('QuanLyNhaHang_PBL2') IS NULL
    CREATE DATABASE [QuanLyNhaHang_PBL2];
GO

USE [QuanLyNhaHang_PBL2];
GO

IF OBJECT_ID('dbo.ThucDon', 'U') IS NULL
CREATE TABLE [dbo].[ThucDon](
    [loai] [nvarchar](255) NULL,
    [ma] [nvarchar](160) NOT NULL PRIMARY KEY,
    [ten] [nvarchar](255) NULL,
    [gia] [decimal](28, 6) NULL,
    [tonKho] [int] NULL,
    [doCayHoacSize] [nvarchar](255) NULL,
    [monChayHoacCoDa] [int] NULL,
    [hinhAnh] [nvarchar](500) NULL
);
ELSE IF COL_LENGTH('dbo.ThucDon', 'hinhAnh') IS NULL
    ALTER TABLE [dbo].[ThucDon] ADD [hinhAnh] [nvarchar](500) NULL;
GO

IF OBJECT_ID('dbo.KhachHang', 'U') IS NULL
CREATE TABLE [dbo].[KhachHang](
    [ma] [nvarchar](160) NOT NULL PRIMARY KEY,
    [ten] [nvarchar](255) NULL,
    [dienThoai] [nvarchar](255) NULL,
    [gioiTinh] [nvarchar](255) NULL,
    [diem] [int] NULL,
    [email] [nvarchar](255) NULL
);
GO

IF OBJECT_ID('dbo.NhanVien', 'U') IS NULL
CREATE TABLE [dbo].[NhanVien](
    [ma] [nvarchar](160) NOT NULL PRIMARY KEY,
    [ten] [nvarchar](255) NULL,
    [dienThoai] [nvarchar](255) NULL,
    [gioiTinh] [nvarchar](255) NULL,
    [chucVu] [nvarchar](255) NULL,
    [luong] [decimal](28, 6) NULL,
    [phuCap] [decimal](28, 6) NULL
);
GO

IF OBJECT_ID('dbo.HoaDon', 'U') IS NULL
CREATE TABLE [dbo].[HoaDon](
    [ma] [nvarchar](160) NOT NULL PRIMARY KEY,
    [ngay] [nvarchar](255) NULL,
    [maKhach] [nvarchar](255) NULL,
    [maNhanVien] [nvarchar](255) NULL,
    [giamGia] [decimal](28, 6) NULL,
    [chiTiet] [nvarchar](max) NULL
);
GO

IF OBJECT_ID('dbo.DonDat', 'U') IS NULL
CREATE TABLE [dbo].[DonDat](
    [thuTu] [bigint] IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [noiDung] [nvarchar](max) NULL
);
GO

IF OBJECT_ID('dbo.TaiKhoan', 'U') IS NULL
CREATE TABLE [dbo].[TaiKhoan](
    [tenDangNhap] [varchar](50) NOT NULL PRIMARY KEY,
    [matKhau] [nvarchar](100) NULL,
    [vaiTro] [varchar](20) NULL,
    [maLienKet] [varchar](50) NULL
);
ELSE IF COL_LENGTH('dbo.TaiKhoan', 'maLienKet') IS NULL
    ALTER TABLE [dbo].[TaiKhoan] ADD [maLienKet] [varchar](50) NULL;
GO

DELETE FROM [dbo].[ThucDon];
DELETE FROM [dbo].[KhachHang];
DELETE FROM [dbo].[NhanVien];
DELETE FROM [dbo].[TaiKhoan];
GO

INSERT [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0001', N'Tô Đông Phước', N'0905554790', N'Nam', 0, N'todongphuoc2008@gmail.com ');
INSERT [dbo].[KhachHang] ([ma], [ten], [dienThoai], [gioiTinh], [diem], [email]) VALUES (N'KH0002', N'Nguyễn Văn A', N'0905554798', N'Nam', 0, N'HAI@124');
GO

INSERT [dbo].[NhanVien] ([ma], [ten], [dienThoai], [gioiTinh], [chucVu], [luong], [phuCap]) VALUES (N'NV0001', N'Lê Văn Tài ', N'0905995740', N'Nam', N'Nhân viên ', CAST(100.000000 AS Decimal(28, 6)), CAST(100.000000 AS Decimal(28, 6)));
GO

INSERT [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'DongHaiTo01', N'123456789', N'KHACHHANG', N'KH0002');
INSERT [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'SeaSide', N'HAItd2007', N'QUANLY', N'');
INSERT [dbo].[TaiKhoan] ([tenDangNhap], [matKhau], [vaiTro], [maLienKet]) VALUES (N'SeaSide01', N'123456789', N'NHANVIEN', N'NV0001');
GO

INSERT [dbo].[ThucDon] ([loai], [ma], [ten], [gia], [tonKho], [doCayHoacSize], [monChayHoacCoDa], [hinhAnh]) VALUES (N'MONAN', N'MA0001', N'Mỳ cay ', CAST(10000.000000 AS Decimal(28, 6)), 41, N'5', 0, N'DuLieu/AnhMon/mi-kim-chi-thap-cam.png');
GO
