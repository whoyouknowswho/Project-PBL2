@echo off
rem Mo ung dung voi database QuanLyNhaHang_PBL2_Ban (du lieu nhap tu database.sql cua ban).
rem Database cua minh (QuanLyNhaHang_PBL2) khong bi dong toi.
cd /d "%~dp0"
set "PBL2_SQL_CONNECTION=Driver={ODBC Driver 18 for SQL Server};Server=.\SQLEXPRESS;Database=QuanLyNhaHang_PBL2_Ban;Trusted_Connection=yes;TrustServerCertificate=yes;"
start "" "%~dp0QuanLyNhaHangGUI.exe"
