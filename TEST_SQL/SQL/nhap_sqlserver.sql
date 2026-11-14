SET NOCOUNT ON; SET XACT_ABORT ON;
IF DB_ID(N'QuanLyNhaHang_PBL2') IS NULL EXEC(N'CREATE DATABASE QuanLyNhaHang_PBL2');
GO
USE QuanLyNhaHang_PBL2;
GO
BEGIN TRAN;
IF OBJECT_ID(N'dbo.ThucDon', N'U') IS NULL
CREATE TABLE dbo.ThucDon (
    loai nvarchar(255),
    ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    ten nvarchar(255),
    gia decimal(28,6),
    tonKho int,
    doCayHoacSize nvarchar(255),
    monChayHoacCoDa int
);

IF OBJECT_ID(N'dbo.KhachHang', N'U') IS NULL
CREATE TABLE dbo.KhachHang (
    ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    ten nvarchar(255),
    dienThoai nvarchar(255),
    gioiTinh nvarchar(255),
    diem int,
    email nvarchar(255)
);

IF OBJECT_ID(N'dbo.NhanVien', N'U') IS NULL
CREATE TABLE dbo.NhanVien (
    ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    ten nvarchar(255),
    dienThoai nvarchar(255),
    gioiTinh nvarchar(255),
    chucVu nvarchar(255),
    luong decimal(28,6),
    phuCap decimal(28,6)
);

IF OBJECT_ID(N'dbo.HoaDon', N'U') IS NULL
CREATE TABLE dbo.HoaDon (
    ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    ngay nvarchar(255),
    maKhach nvarchar(255),
    maNhanVien nvarchar(255),
    giamGia decimal(28,6),
    chiTiet nvarchar(max)
);

IF OBJECT_ID(N'dbo.DonDat', N'U') IS NULL
CREATE TABLE dbo.DonDat (
    thuTu bigint IDENTITY PRIMARY KEY,
    noiDung nvarchar(max)
);

IF OBJECT_ID(N'dbo.TaiKhoan', N'U') IS NULL
CREATE TABLE dbo.TaiKhoan (
    thuatToan nvarchar(255),
    tenDangNhap nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    vaiTro nvarchar(255),
    maLienKet nvarchar(255),
    muoi nvarchar(255),
    matKhauBam nvarchar(255)
);

COMMIT;
