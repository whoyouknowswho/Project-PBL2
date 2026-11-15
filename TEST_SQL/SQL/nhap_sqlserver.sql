IF DB_ID('QuanLyNhaHang_PBL2') IS NULL
    CREATE DATABASE QuanLyNhaHang_PBL2;
GO

USE QuanLyNhaHang_PBL2;
GO

IF OBJECT_ID('dbo.ThucDon', 'U') IS NULL
CREATE TABLE dbo.ThucDon (
    loai nvarchar(50),
    ma varchar(50) PRIMARY KEY,
    ten nvarchar(255),
    gia float,
    tonKho int,
    doCayHoacSize nvarchar(50),
    monChayHoacCoDa int
);

IF OBJECT_ID('dbo.KhachHang', 'U') IS NULL
CREATE TABLE dbo.KhachHang (
    ma varchar(50) PRIMARY KEY,
    ten nvarchar(255),
    dienThoai varchar(20),
    gioiTinh nvarchar(20),
    diem int,
    email varchar(255)
);

IF OBJECT_ID('dbo.NhanVien', 'U') IS NULL
CREATE TABLE dbo.NhanVien (
    ma varchar(50) PRIMARY KEY,
    ten nvarchar(255),
    dienThoai varchar(20),
    gioiTinh nvarchar(20),
    chucVu nvarchar(100),
    luong float,
    phuCap float
);

IF OBJECT_ID('dbo.HoaDon', 'U') IS NULL
CREATE TABLE dbo.HoaDon (
    ma varchar(50) PRIMARY KEY,
    ngay varchar(20),
    maKhach varchar(50),
    maNhanVien varchar(50),
    giamGia float,
    chiTiet nvarchar(max)
);

IF OBJECT_ID('dbo.DonDat', 'U') IS NULL
CREATE TABLE dbo.DonDat (
    thuTu int IDENTITY(1,1) PRIMARY KEY,
    noiDung nvarchar(max)
);

IF OBJECT_ID('dbo.TaiKhoan', 'U') IS NULL
CREATE TABLE dbo.TaiKhoan (
    tenDangNhap varchar(50) PRIMARY KEY,
    matKhau nvarchar(100),
    vaiTro varchar(20)
);
