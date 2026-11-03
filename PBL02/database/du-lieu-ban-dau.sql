-- DU LIEU BAN DAU: cac lenh INSERT cu the, co the sua truc tiep trong SSMS/VS Code.
-- Chon database QuanLyNhaHang va chay schema.sql truoc khi chay file nay.
-- Du lieu lay tu database hien co; giu nguyen lich su va mat khau BAM cua tai khoan.
-- Chi nap khi tat ca bang nghiep vu rong. Chay lai se bo qua, khong ghi de.
-- Dong ung dung nha hang khi thiet lap database. Khong can cong cu C++ nhap du lieu.
SET NOCOUNT ON;
SET XACT_ABORT ON;
IF DB_NAME() IN (N'master', N'model', N'msdb', N'tempdb')
    THROW 51000, N'Hay chon database QuanLyNhaHang truoc khi chay.', 1;
BEGIN TRY
    BEGIN TRANSACTION;
    IF EXISTS (SELECT 1 FROM dbo.[KhachHang])
       OR EXISTS (SELECT 1 FROM dbo.[NhanVien])
       OR EXISTS (SELECT 1 FROM dbo.[Mon])
       OR EXISTS (SELECT 1 FROM dbo.[TaiKhoan])
       OR EXISTS (SELECT 1 FROM dbo.[HoaDon])
       OR EXISTS (SELECT 1 FROM dbo.[DonDatMon])
       OR EXISTS (SELECT 1 FROM dbo.[ChiTietHoaDon])
       OR EXISTS (SELECT 1 FROM dbo.[ChiTietDuTinh])
       OR EXISTS (SELECT 1 FROM dbo.[ChiTietDonDat])
    BEGIN
        PRINT N'Database da co du lieu. Bo qua nap du lieu ban dau.';
        COMMIT TRANSACTION;
        RETURN;
    END;

    -- KhachHang : 6 ban ghi
    INSERT INTO dbo.[KhachHang] ([Ma], [HoTen], [DienThoai], [GioiTinh], [Diem])
    VALUES
        (N'0', N'To Dong Hai ', N'0934967814', N'Nam ', 200),
        (N'KH0001', N'Tô Đông Hải', N'0905995740', N'Nam', 266),
        (N'KH01', N'Le Van Tai', N'07e9ei39723', N'Nam', 0),
        (N'KH02', N'Tran Thi Binh', N'0912345678', N'Nu', 50),
        (N'KH03', N'To Dong Hai', N'0934967814', N'Nam', 210),
        (N'KH04', N'To Dong Thoan ', N'0905554790', N'Nam', 22);

    -- NhanVien : 3 ban ghi
    INSERT INTO dbo.[NhanVien] ([Ma], [HoTen], [DienThoai], [GioiTinh], [ChucVu], [Luong], [PhuCap])
    VALUES
        (N'NV01', N'Le Minh Chau', N'0987654321', N'Nam', N'Phuc vu', 7000000.0000, 500000.0000),
        (N'NV02', N'Pham Thu Dung', N'0976543210', N'Nu', N'Thu ngan', 8500000.0000, 700000.0000),
        (N'NV03', N'To Dong Hai', N'0934967814', N'Nam', N'Giam Doc', 200000.0000, 23000.0000);

    -- Mon : 5 ban ghi
    INSERT INTO dbo.[Mon] ([Ma], [Loai], [Ten], [Gia], [TonKho], [DoCay], [MonChay], [SizeLy], [CoDa])
    VALUES
        (N'MA01', N'MONAN', N'Com ga', 45000.0000, 1000000, 1, 0, N'', 0),
        (N'MA02', N'MONAN', N'Rau xao', 35000.0000, 1000298, 0, 1, N'', 0),
        (N'MA03', N'MONAN', N'Mi Tom', 15000.0000, 1000, 0, 0, N'', 0),
        (N'NU01', N'NUOC', N'Tra dao', 25000.0000, 205, 0, 0, N'M', 1),
        (N'NU02', N'NUOC', N'Nuoc cam', 30000.0000, 158, 0, 0, N'L', 0);

    -- TaiKhoan : 4 ban ghi
    INSERT INTO dbo.[TaiKhoan] ([TenDangNhap], [VaiTro], [MaLienKet], [Muoi], [MatKhauBam], [ThuatToan])
    VALUES
        (N'DongHaiTo', N'KHACHHANG', N'KH0001', N'7e4535df08ef787eac9a7dd04cd67d86', N'bc28135b8d583c5af460765db08967b3406a8164f88891ce80b425bfffa95cfc', N'PBKDF2-SHA256-600000'),
        (N'SeaSide', N'QUANLY', N'', N'ce0dc461a87f6efbb246cac83a4dfe1b', N'a53fcfc4d80c93d70e7ad59c5238301fb53ef76a37184f82e278d860fa3cc6b5', N'PBKDF2-SHA256-600000'),
        (N'seaside01', N'NHANVIEN', N'NV01', N'7052d82cda7ee7b855d4886fb3c0aceb', N'd67b6e47d8e92236b2f32d0a87fd8081ddcf9524d071029bd4d63618bb39b370', N'PBKDF2-SHA256-600000'),
        (N'seaside02', N'NHANVIEN', N'NV02', N'359a2ba3e974915cdb7f788e2a88dcd6', N'267f3fd2655bac3218b31972bfb5e49a3be13ef78921068e9827827ffb1c6709', N'PBKDF2-SHA256-600000');

    -- HoaDon : 17 ban ghi
    INSERT INTO dbo.[HoaDon] ([Ma], [NgayGoc], [MaKhachHang], [MaNhanVien], [GiamGia])
    VALUES
        (N'10202202', N'30303', N'3030', N'3030', 0.0000),
        (N'1022304', N'03 / 09 / 2025', N'KH03', N'NV09', 0.0000),
        (N'102250115', N'01 / 09 / 2026', N'KH03', N'NV01', 0.0000),
        (N'102250116', N'02 / 09 / 2026', N'', N'NV03', 0.0000),
        (N'102250117', N'02/09/2026', N'KH03', N'NV01', 0.0000),
        (N'102250118', N'2', N'3', N'2', 0.0000),
        (N'102250119', N'02 / 09 / 2026', N'KH01', N'NV02', 0.0000),
        (N'2', N'3', N'3', N'3', 0.0000),
        (N'4', N'4', N'4', N'NV01', 0.0000),
        (N'HD001', N'01/09/2026', N'KH01', N'NV02', 5.0000),
        (N'HD20261027-1065865656', N'27/10/2026', N'', N'NV03', 0.0000),
        (N'HD20261027-1067113437', N'27/10/2026', N'', N'NV01', 0.0000),
        (N'HD20261027-1072111703', N'27/10/2026', N'KH04', N'NV02', 0.0000),
        (N'HD20261027-1072160531', N'27/10/2026', N'KH04', N'NV02', 0.0000),
        (N'HD_DM1', N'27/10/2026', N'KH0001', N'NV02', 0.0000),
        (N'HD_DM2', N'27/10/2026', N'KH0001', N'NV02', 0.0000),
        (N'HD_DM3', N'30/10/2026', N'KH0001', N'NV02', 0.0000);

    -- DonDatMon : 3 ban ghi
    INSERT INTO dbo.[DonDatMon] ([Ma], [NgayGoc], [MaKhachHang], [GiamGia], [TrangThai], [MaHoaDon])
    VALUES
        (N'DM1', N'27/10/2026', N'KH0001', 0.0000, N'XONG', N'HD_DM1'),
        (N'DM2', N'27/10/2026', N'KH0001', 0.0000, N'XONG', N'HD_DM2'),
        (N'DM3', N'30/10/2026', N'KH0001', 0.0000, N'XONG', N'HD_DM3');

    -- ChiTietHoaDon : 34 ban ghi
    INSERT INTO dbo.[ChiTietHoaDon] ([MaHoaDon], [Dong], [MaMon], [TenMon], [SoLuong], [DonGia])
    VALUES
        (N'10202202', 1, N'MA03', N'Mi Tom', 1, 15000.0000),
        (N'1022304', 1, N'MA03', N'Mi Tom', 3, 15000.0000),
        (N'102250115', 1, N'MA01', N'Com ga', 3, 45000.0000),
        (N'102250116', 1, N'MA01', N'Com ga', 3, 45000.0000),
        (N'102250116', 2, N'NU01', N'Tra dao', 5, 25000.0000),
        (N'102250117', 1, N'MA01', N'Com ga', 23, 45000.0000),
        (N'102250117', 2, N'NU02', N'Nuoc cam', 23, 30000.0000),
        (N'102250118', 1, N'NU01', N'Tra dao', 2, 25000.0000),
        (N'102250119', 1, N'MA03', N'Mi Tom', 2, 15000.0000),
        (N'2', 1, N'MA03', N'Mi Tom', 1, 15000.0000),
        (N'4', 1, N'MA03', N'Mi Tom', 1, 15000.0000),
        (N'HD001', 1, N'MA01', N'Com ga', 2, 45000.0000),
        (N'HD001', 2, N'NU01', N'Tra dao', 2, 25000.0000),
        (N'HD20261027-1065865656', 1, N'MA01', N'Com ga', 1, 45000.0000),
        (N'HD20261027-1065865656', 2, N'MA02', N'Rau xao', 2, 35000.0000),
        (N'HD20261027-1067113437', 1, N'MA01', N'Com ga', 1, 45000.0000),
        (N'HD20261027-1067113437', 2, N'NU01', N'Tra dao (Size M, Co da)', 1, 25000.0000),
        (N'HD20261027-1067113437', 3, N'NU02', N'Nuoc cam (Size M, Co da)', 1, 30000.0000),
        (N'HD20261027-1072111703', 1, N'NU01', N'Tra dao (Size M, Khong da)', 1, 25000.0000),
        (N'HD20261027-1072111703', 2, N'NU02', N'Nuoc cam (Size M, Khong da)', 1, 30000.0000),
        (N'HD20261027-1072111703', 3, N'MA03', N'Mi Tom', 1, 15000.0000),
        (N'HD20261027-1072160531', 1, N'NU02', N'Nuoc cam (Size M, Co da)', 1, 30000.0000),
        (N'HD20261027-1072160531', 2, N'NU01', N'Tra dao (Size M, Co da)', 1, 25000.0000),
        (N'HD_DM1', 1, N'MA02', N'Rau xao', 1, 35000.0000),
        (N'HD_DM1', 2, N'NU02', N'Nuoc cam (Size M, Co da)', 2, 30000.0000),
        (N'HD_DM1', 3, N'NU01', N'Tra dao (Size M, Co da)', 1, 25000.0000),
        (N'HD_DM2', 1, N'MA01', N'Com ga', 1, 45000.0000),
        (N'HD_DM2', 2, N'MA03', N'Mi Tom', 4, 15000.0000),
        (N'HD_DM2', 3, N'NU01', N'Tra dao (Size M, Khong da)', 5, 25000.0000),
        (N'HD_DM3', 1, N'MA01', N'Com ga', 2, 45000.0000),
        (N'HD_DM3', 2, N'MA02', N'Rau xao', 3, 35000.0000),
        (N'HD_DM3', 3, N'MA03', N'Mi Tom', 7, 15000.0000),
        (N'HD_DM3', 4, N'NU01', N'Tra dao (Size L, Co da)', 36, 25000.0000),
        (N'HD_DM3', 5, N'NU02', N'Nuoc cam (Size L, Co da)', 37, 30000.0000);

    -- ChiTietDuTinh : 11 ban ghi
    INSERT INTO dbo.[ChiTietDuTinh] ([MaDon], [Dong], [MaMon], [TenMon], [SoLuong], [DonGia])
    VALUES
        (N'DM1', 1, N'MA02', N'Rau xao', 1, 35000.0000),
        (N'DM1', 2, N'NU02', N'Nuoc cam (Size M, Co da)', 2, 30000.0000),
        (N'DM1', 3, N'NU01', N'Tra dao (Size M, Co da)', 1, 25000.0000),
        (N'DM2', 1, N'MA01', N'Com ga', 1, 45000.0000),
        (N'DM2', 2, N'MA03', N'Mi Tom', 4, 15000.0000),
        (N'DM2', 3, N'NU01', N'Tra dao (Size M, Khong da)', 5, 25000.0000),
        (N'DM3', 1, N'MA01', N'Com ga', 2, 45000.0000),
        (N'DM3', 2, N'MA02', N'Rau xao', 3, 35000.0000),
        (N'DM3', 3, N'MA03', N'Mi Tom', 7, 15000.0000),
        (N'DM3', 4, N'NU01', N'Tra dao (Size L, Co da)', 36, 25000.0000),
        (N'DM3', 5, N'NU02', N'Nuoc cam (Size L, Co da)', 37, 30000.0000);

    -- ChiTietDonDat : 11 ban ghi
    INSERT INTO dbo.[ChiTietDonDat] ([MaDon], [Dong], [MaMon], [SoLuong], [SizeLy], [CoDa])
    VALUES
        (N'DM1', 1, N'MA02', 1, N'', 0),
        (N'DM1', 2, N'NU02', 2, N'M', 1),
        (N'DM1', 3, N'NU01', 1, N'M', 1),
        (N'DM2', 1, N'MA01', 1, N'', 0),
        (N'DM2', 2, N'MA03', 4, N'', 0),
        (N'DM2', 3, N'NU01', 5, N'M', 0),
        (N'DM3', 1, N'MA01', 2, N'', 0),
        (N'DM3', 2, N'MA02', 3, N'', 0),
        (N'DM3', 3, N'MA03', 7, N'', 0),
        (N'DM3', 4, N'NU01', 36, N'L', 1),
        (N'DM3', 5, N'NU02', 37, N'L', 1);

    COMMIT TRANSACTION;
    PRINT N'Da nap du lieu ban dau thanh cong.';
END TRY
BEGIN CATCH
    IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    THROW;
END CATCH;
