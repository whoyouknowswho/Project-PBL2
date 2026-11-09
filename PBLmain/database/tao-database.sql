-- Buoc 1: chay file nay trong SSMS hoac VS Code.
USE master;
GO
IF DB_ID(N'QuanLyNhaHang') IS NULL
    CREATE DATABASE QuanLyNhaHang;
GO
USE QuanLyNhaHang;
GO
-- Buoc 2: chon QuanLyNhaHang va chay schema.sql.
-- Buoc 3: van chon QuanLyNhaHang, chay du-lieu-ban-dau.sql.
-- Buoc 4: chay xem-du-lieu.sql de kiem tra.
