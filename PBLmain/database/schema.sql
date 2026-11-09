-- Cach doc file nay:
-- CREATE TABLE: tao bang. Moi dong ben trong khai bao mot cot.
-- nvarchar: chuoi Unicode; int: so nguyen; decimal: so thap phan; bit: 0/1.
-- PRIMARY KEY: ma duy nhat. NOT NULL: bat buoc co gia tri.
-- CHECK: gioi han gia tri hop le. REFERENCES: lien ket den bang khac.
-- IF ... IS NULL: chi tao bang neu chua co, de co the chay lai.
-- BEGIN TRANSACTION / COMMIT: gom cac thay doi thanh mot giao dich.
-- dbo la ten schema (nhom chua cac bang); GO tach cac dot lenh trong SSMS.

-- SQL Server 2025 Express. Chay trong database QuanLyNhaHang.
-- Khong xoa bang hoac ghi de du lieu khi chay lai.
SET XACT_ABORT ON;
BEGIN TRANSACTION;

-- CauHinh: Thong tin phien ban database, ung dung tu su dung.
IF OBJECT_ID(N'dbo.CauHinh', N'U') IS NULL
BEGIN
    CREATE TABLE dbo.CauHinh (Id int PRIMARY KEY CHECK(Id=1),
    PhienBan int NOT NULL,
    DaNhap bit NOT NULL);
    INSERT dbo.CauHinh VALUES (1,1,0);
END;

-- KhachHang: Ho so khach hang va diem tich luy.
IF OBJECT_ID(N'dbo.KhachHang', N'U') IS NULL
CREATE TABLE dbo.KhachHang (
    Ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    HoTen nvarchar(160) NOT NULL,
    DienThoai nvarchar(160) NOT NULL,
    GioiTinh nvarchar(160) NOT NULL,
    Diem int NOT NULL CHECK(Diem>=0)
);
-- Bo sung email cho database cu, giu nguyen du lieu hien co.
IF COL_LENGTH(N'dbo.KhachHang', N'Email') IS NULL
    ALTER TABLE dbo.KhachHang ADD Email nvarchar(254) NOT NULL
        CONSTRAINT DF_KhachHang_Email DEFAULT N'';

-- NhanVien: Ho so nhan vien va luong.
IF OBJECT_ID(N'dbo.NhanVien', N'U') IS NULL
CREATE TABLE dbo.NhanVien (
    Ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    HoTen nvarchar(160) NOT NULL,
    DienThoai nvarchar(160) NOT NULL,
    GioiTinh nvarchar(160) NOT NULL,
    ChucVu nvarchar(160) NOT NULL,
    Luong decimal(19,4) NOT NULL CHECK(Luong>=0),
    PhuCap decimal(19,4) NOT NULL CHECK(PhuCap>=0)
);

-- Mon: Thuc don: MONAN la mon an, NUOC la nuoc uong.
IF OBJECT_ID(N'dbo.Mon', N'U') IS NULL
CREATE TABLE dbo.Mon (
    Ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    Loai nvarchar(10) NOT NULL CHECK(Loai IN ('MONAN','NUOC')),
    Ten nvarchar(160) NOT NULL,
    Gia decimal(19,4) NOT NULL CHECK(Gia>=0),
    TonKho int NOT NULL CHECK(TonKho>=0),
    DoCay int NOT NULL CHECK(DoCay BETWEEN 0 AND 5),
    MonChay bit NOT NULL,
    SizeLy nvarchar(1) NOT NULL CHECK(SizeLy IN ('','S','M','L')),
    CoDa bit NOT NULL
);
-- Ma lien ket va cac ma trong lich su duoc giu ngay ca khi ho so/mon bi xoa,
-- phu hop hanh vi ung dung cu. Dang nhap van kiem tra ho so con ton tai.

-- TaiKhoan: Tai khoan dang nhap; mat khau duoc bam boi ung dung.
IF OBJECT_ID(N'dbo.TaiKhoan', N'U') IS NULL
BEGIN
    CREATE TABLE dbo.TaiKhoan (
        TenDangNhap nvarchar(32) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
        VaiTro nvarchar(16) NOT NULL CHECK(VaiTro IN ('QUANLY','NHANVIEN','KHACHHANG')),
        MaLienKet nvarchar(160) COLLATE Latin1_General_100_BIN2 NOT NULL,
        Muoi varchar(32) NOT NULL CHECK(LEN(Muoi)=32 AND Muoi COLLATE Latin1_General_100_BIN2 NOT LIKE '%[^0-9a-f]%'),
        MatKhauBam varchar(64) NOT NULL CHECK(LEN(MatKhauBam)=64 AND MatKhauBam COLLATE Latin1_General_100_BIN2 NOT LIKE '%[^0-9a-f]%'),
        ThuatToan varchar(32) NOT NULL CHECK(ThuatToan='PBKDF2-SHA256-600000')
    );
    CREATE UNIQUE INDEX UX_TaiKhoan_KhachHang ON dbo.TaiKhoan(MaLienKet) WHERE VaiTro='KHACHHANG';
END;

-- HoaDon: Thong tin chung cua hoa don.
IF OBJECT_ID(N'dbo.HoaDon', N'U') IS NULL
CREATE TABLE dbo.HoaDon (
    Ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    NgayGoc nvarchar(160) NOT NULL,
    -- Ngay sai duoc giu nguyen de doi chieu, NgayLap se la NULL.
    NgayLap AS TRY_CONVERT(date, NgayGoc, 103),
    MaKhachHang nvarchar(160) NOT NULL,
    MaNhanVien nvarchar(160) NOT NULL,
    GiamGia decimal(7,4) NOT NULL CHECK(GiamGia BETWEEN 0 AND 100)
);

-- DonDatMon: Don dat: CHO, HUY hoac XONG.
IF OBJECT_ID(N'dbo.DonDatMon', N'U') IS NULL
CREATE TABLE dbo.DonDatMon (
    Ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY,
    NgayGoc nvarchar(160) NOT NULL,
    NgayDat AS TRY_CONVERT(date, NgayGoc, 103),
    MaKhachHang nvarchar(160) NOT NULL,
    GiamGia decimal(7,4) NOT NULL CHECK(GiamGia BETWEEN 0 AND 100),
    TrangThai varchar(8) NOT NULL CHECK(TrangThai IN ('CHO','HUY','XONG')),
    MaHoaDon nvarchar(160) NOT NULL
);

-- ChiTietHoaDon: Cac mon thuoc mot hoa don.
IF OBJECT_ID(N'dbo.ChiTietHoaDon', N'U') IS NULL
CREATE TABLE dbo.ChiTietHoaDon (
    MaHoaDon nvarchar(160) COLLATE Latin1_General_100_BIN2 NOT NULL REFERENCES dbo.HoaDon(Ma),
    Dong int NOT NULL CHECK(Dong>0),
    MaMon nvarchar(160) NOT NULL,
    TenMon nvarchar(240) NOT NULL,
    SoLuong int NOT NULL CHECK(SoLuong>0),
    DonGia decimal(19,4) NOT NULL CHECK(DonGia>=0),
    PRIMARY KEY(MaHoaDon,Dong)
);

-- ChiTietDuTinh: Cac mon va gia du tinh cua don dat.
IF OBJECT_ID(N'dbo.ChiTietDuTinh', N'U') IS NULL
CREATE TABLE dbo.ChiTietDuTinh (
    MaDon nvarchar(160) COLLATE Latin1_General_100_BIN2 NOT NULL REFERENCES dbo.DonDatMon(Ma),
    Dong int NOT NULL CHECK(Dong>0),
    MaMon nvarchar(160) NOT NULL,
    TenMon nvarchar(240) NOT NULL,
    SoLuong int NOT NULL CHECK(SoLuong>0),
    DonGia decimal(19,4) NOT NULL CHECK(DonGia>=0),
    PRIMARY KEY(MaDon,Dong)
);

-- ChiTietDonDat: So luong va size mon trong don dat.
IF OBJECT_ID(N'dbo.ChiTietDonDat', N'U') IS NULL
CREATE TABLE dbo.ChiTietDonDat (
    MaDon nvarchar(160) COLLATE Latin1_General_100_BIN2 NOT NULL REFERENCES dbo.DonDatMon(Ma),
    Dong int NOT NULL CHECK(Dong>0),
    MaMon nvarchar(160) NOT NULL,
    SoLuong int NOT NULL CHECK(SoLuong>0),
    SizeLy nvarchar(1) NOT NULL CHECK(SizeLy IN ('','S','M','L')),
    CoDa bit NOT NULL,
    PRIMARY KEY(MaDon,Dong)
);
COMMIT;
GO
CREATE OR ALTER VIEW dbo.vDoanhThu AS
SELECT h.Ma, h.NgayGoc, h.NgayLap, h.MaKhachHang, h.MaNhanVien, h.GiamGia,
       SUM(CONVERT(decimal(28,4),c.SoLuong)*c.DonGia) AS TamTinh,
       SUM(CONVERT(decimal(28,4),c.SoLuong)*c.DonGia)*(1-h.GiamGia/100) AS ThanhToan
FROM dbo.HoaDon h JOIN dbo.ChiTietHoaDon c ON c.MaHoaDon=h.Ma
GROUP BY h.Ma,h.NgayGoc,h.NgayLap,h.MaKhachHang,h.MaNhanVien,h.GiamGia;
GO
