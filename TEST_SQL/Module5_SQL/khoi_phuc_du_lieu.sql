-- Khoi phuc du lieu tu ban sao cu PBL_Claude/DuLieu (09/11/2026).
-- Chi nap vao bang dang TRONG; chay lai se bo qua, khong ghi de. Bang TaiKhoan khong dong cham.
-- Dong QuanLyNhaHangGUI.exe truoc khi chay.
USE QuanLyNhaHang_PBL2;
SET XACT_ABORT ON;
IF COL_LENGTH('dbo.ThucDon','hinhAnh') IS NULL ALTER TABLE dbo.ThucDon ADD hinhAnh nvarchar(500);
GO
BEGIN TRAN;
IF NOT EXISTS (SELECT 1 FROM dbo.ThucDon) BEGIN
  INSERT INTO dbo.ThucDon (loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa,hinhAnh) VALUES (N'MONAN',N'MA01',N'Com ga',N'45000',N'1000000',N'1',N'0',N'');
  INSERT INTO dbo.ThucDon (loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa,hinhAnh) VALUES (N'MONAN',N'MA02',N'Rau xao',N'35000',N'1000298',N'0',N'1',N'');
  INSERT INTO dbo.ThucDon (loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa,hinhAnh) VALUES (N'NUOC',N'NU01',N'Tra dao',N'25000',N'205',N'M',N'1',N'');
  INSERT INTO dbo.ThucDon (loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa,hinhAnh) VALUES (N'NUOC',N'NU02',N'Nuoc cam',N'30000',N'158',N'L',N'0',N'');
  INSERT INTO dbo.ThucDon (loai,ma,ten,gia,tonKho,doCayHoacSize,monChayHoacCoDa,hinhAnh) VALUES (N'MONAN',N'MA03',N'Mi Tom',N'15000',N'1000',N'0',N'0',N'');
END
IF NOT EXISTS (SELECT 1 FROM dbo.KhachHang) BEGIN
  INSERT INTO dbo.KhachHang (ma,ten,dienThoai,gioiTinh,diem,email) VALUES (N'KH02',N'Tran Thi Binh',N'0912345678',N'Nu',N'50',N'');
  INSERT INTO dbo.KhachHang (ma,ten,dienThoai,gioiTinh,diem,email) VALUES (N'KH03',N'To Dong Hai',N'0934967814',N'Nam',N'210',N'');
  INSERT INTO dbo.KhachHang (ma,ten,dienThoai,gioiTinh,diem,email) VALUES (N'KH04',N'To Dong Thoan ',N'0905554790',N'Nam',N'22',N'');
  INSERT INTO dbo.KhachHang (ma,ten,dienThoai,gioiTinh,diem,email) VALUES (N'KH01',N'Le Van Tai',N'07e9ei39723',N'Nam',N'0',N'');
  INSERT INTO dbo.KhachHang (ma,ten,dienThoai,gioiTinh,diem,email) VALUES (N'0',N'To Dong Hai ',N'0934967814',N'Nam ',N'200',N'');
  INSERT INTO dbo.KhachHang (ma,ten,dienThoai,gioiTinh,diem,email) VALUES (N'KH0001',N'Tô Đông Hải',N'0905995740',N'Nam',N'266',N'');
  INSERT INTO dbo.KhachHang (ma,ten,dienThoai,gioiTinh,diem,email) VALUES (N'KH0002',N'Lê Văn Tài',N'012345678',N'Nam',N'0',N'');
END
IF NOT EXISTS (SELECT 1 FROM dbo.NhanVien) BEGIN
  INSERT INTO dbo.NhanVien (ma,ten,dienThoai,gioiTinh,chucVu,luong,phuCap) VALUES (N'NV01',N'Le Minh Chau',N'0987654321',N'Nam',N'Phuc vu',N'7e+06',N'500000');
  INSERT INTO dbo.NhanVien (ma,ten,dienThoai,gioiTinh,chucVu,luong,phuCap) VALUES (N'NV02',N'Pham Thu Dung',N'0976543210',N'Nu',N'Thu ngan',N'8.5e+06',N'700000');
  INSERT INTO dbo.NhanVien (ma,ten,dienThoai,gioiTinh,chucVu,luong,phuCap) VALUES (N'NV03',N'To Dong Hai',N'0934967814',N'Nam',N'Giam Doc',N'200000',N'23000');
END
IF NOT EXISTS (SELECT 1 FROM dbo.HoaDon) BEGIN
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD001',N'01/09/2026',N'KH01',N'NV02',N'5',N'MA01,Com ga,2,45000;NU01,Tra dao,2,25000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'102250115',N'01 / 09 / 2026',N'KH03',N'NV01',N'0',N'MA01,Com ga,3,45000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'102250116',N'02 / 09 / 2026',N'',N'NV03',N'0',N'MA01,Com ga,3,45000;NU01,Tra dao,5,25000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'102250117',N'02/09/2026',N'KH03',N'NV01',N'0',N'MA01,Com ga,23,45000;NU02,Nuoc cam,23,30000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'102250118',N'2',N'3',N'2',N'0',N'NU01,Tra dao,2,25000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'102250119',N'02 / 09 / 2026',N'KH01',N'NV02',N'0',N'MA03,Mi Tom,2,15000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'1022304',N'03 / 09 / 2025',N'KH03',N'NV09',N'0',N'MA03,Mi Tom,3,15000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'10202202',N'30303',N'3030',N'3030',N'0',N'MA03,Mi Tom,1,15000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'2',N'3',N'3',N'3',N'0',N'MA03,Mi Tom,1,15000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'4',N'4',N'4',N'NV01',N'0',N'MA03,Mi Tom,1,15000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD20261027-1065865656',N'27/10/2026',N'',N'NV03',N'0',N'MA01,Com ga,1,45000;MA02,Rau xao,2,35000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD20261027-1067113437',N'27/10/2026',N'',N'NV01',N'0',N'MA01,Com ga,1,45000;NU01,Tra dao (Size M, Co da),1,25000;NU02,Nuoc cam (Size M, Co da),1,30000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD20261027-1072111703',N'27/10/2026',N'KH04',N'NV02',N'0',N'NU01,Tra dao (Size M, Khong da),1,25000;NU02,Nuoc cam (Size M, Khong da),1,30000;MA03,Mi Tom,1,15000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD20261027-1072160531',N'27/10/2026',N'KH04',N'NV02',N'0',N'NU02,Nuoc cam (Size M, Co da),1,30000;NU01,Tra dao (Size M, Co da),1,25000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD_DM1',N'27/10/2026',N'KH0001',N'NV02',N'0',N'MA02,Rau xao,1,35000;NU02,Nuoc cam (Size M, Co da),2,30000;NU01,Tra dao (Size M, Co da),1,25000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD_DM2',N'27/10/2026',N'KH0001',N'NV02',N'0',N'MA01,Com ga,1,45000;MA03,Mi Tom,4,15000;NU01,Tra dao (Size M, Khong da),5,25000');
  INSERT INTO dbo.HoaDon (ma,ngay,maKhach,maNhanVien,giamGia,chiTiet) VALUES (N'HD_DM3',N'30/10/2026',N'KH0001',N'NV02',N'0',N'MA01,Com ga,2,45000;MA02,Rau xao,3,35000;MA03,Mi Tom,7,15000;NU01,Tra dao (Size L, Co da),36,25000;NU02,Nuoc cam (Size L, Co da),37,30000');
END
IF NOT EXISTS (SELECT 1 FROM dbo.DonDat) BEGIN
  INSERT INTO dbo.DonDat (noiDung) VALUES (N'"XONG" "HD_DM1" "DM1|27/10/2026|KH0001||0|MA02,Rau xao,1,35000;NU02,Nuoc cam (Size M, Co da),2,30000;NU01,Tra dao (Size M, Co da),1,25000" 3 "MA02" 1 "" 0 "NU02" 2 "M" 1 "NU01" 1 "M" 1');
  INSERT INTO dbo.DonDat (noiDung) VALUES (N'"XONG" "HD_DM2" "DM2|27/10/2026|KH0001||0|MA01,Com ga,1,45000;MA03,Mi Tom,4,15000;NU01,Tra dao (Size M, Khong da),5,25000" 3 "MA01" 1 "" 0 "MA03" 4 "" 0 "NU01" 5 "M" 0');
  INSERT INTO dbo.DonDat (noiDung) VALUES (N'"XONG" "HD_DM3" "DM3|30/10/2026|KH0001||0|MA01,Com ga,2,45000;MA02,Rau xao,3,35000;MA03,Mi Tom,7,15000;NU01,Tra dao (Size L, Co da),36,25000;NU02,Nuoc cam (Size L, Co da),37,30000" 5 "MA01" 2 "" 0 "MA02" 3 "" 0 "MA03" 7 "" 0 "NU01" 36 "L" 1 "NU02" 37 "L" 1');
END
COMMIT;
GO
SELECT 'ThucDon' bang, COUNT(*) so_dong FROM dbo.ThucDon UNION ALL SELECT 'KhachHang', COUNT(*) FROM dbo.KhachHang UNION ALL SELECT 'NhanVien', COUNT(*) FROM dbo.NhanVien UNION ALL SELECT 'HoaDon', COUNT(*) FROM dbo.HoaDon UNION ALL SELECT 'DonDat', COUNT(*) FROM dbo.DonDat UNION ALL SELECT 'TaiKhoan', COUNT(*) FROM dbo.TaiKhoan;
