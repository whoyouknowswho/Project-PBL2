-- Sua du lieu truc tiep trong SQL Server (SSMS).
-- QUAN TRONG: dong QuanLyNhaHangGUI.exe truoc khi sua. Ung dung chi doc SQL luc mo,
-- va moi lan luu se xoa roi ghi lai toan bo bang tu bo nho -> sua luc app dang mo se bi mat.
-- Sua xong, mo lai app de thay du lieu moi.
--
-- Boi den 1 cau lenh roi nhan F5 de chay rieng cau do.

USE QuanLyNhaHang_PBL2;
GO

-- ===== Xem du lieu =====
SELECT * FROM dbo.ThucDon   ORDER BY ma;
SELECT * FROM dbo.KhachHang ORDER BY ma;
SELECT * FROM dbo.NhanVien  ORDER BY ma;
SELECT * FROM dbo.HoaDon    ORDER BY ma;
SELECT * FROM dbo.DonDat    ORDER BY thuTu;
SELECT tenDangNhap, vaiTro, maLienKet FROM dbo.TaiKhoan ORDER BY tenDangNhap;

-- ===== Vi du sua (bo dau -- o dau dong de dung) =====

-- Thuc don. loai = 'MONAN' hoac 'NUOC'.
--   MONAN: doCayHoacSize = do cay (so 0..), monChayHoacCoDa = 1 neu mon chay, 0 neu khong
--   NUOC : doCayHoacSize = size 'S'/'M'/'L', monChayHoacCoDa = 1 neu co da, 0 neu khong
-- INSERT INTO dbo.ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh)
-- VALUES ('MONAN', N'MA01', N'Cơm gà', 35000, 20, N'1', 0, N''),
--        ('NUOC',  N'NU01', N'Trà đào', 25000, 30, N'M', 1, N'');
-- UPDATE dbo.ThucDon SET gia = 40000, tonKho = 50 WHERE ma = 'MA01';
-- DELETE FROM dbo.ThucDon WHERE ma = 'MA01';

-- Khach hang. gioiTinh: N'Nam' / N'Nữ' / N'Khác'
-- INSERT INTO dbo.KhachHang (ma, ten, dienThoai, gioiTinh, diem, email)
-- VALUES ('KH01', N'Nguyễn Văn A', '0901234567', N'Nam', 0, 'a@example.com');
-- UPDATE dbo.KhachHang SET diem = 100 WHERE ma = 'KH01';

-- Nhan vien
-- INSERT INTO dbo.NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap)
-- VALUES ('NV01', N'Trần Thị B', '0912345678', N'Nữ', N'Thu ngân', 7000000, 500000);
-- UPDATE dbo.NhanVien SET luong = 8000000 WHERE ma = 'NV01';

-- Tai khoan: KHONG them/sua mat khau bang SQL (mat khau duoc bam PBKDF2).
-- Hay tao tai khoan trong ung dung (muc "Tao tai khoan").
