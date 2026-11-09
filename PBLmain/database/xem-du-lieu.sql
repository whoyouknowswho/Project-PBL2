-- Chay file nay de XEM du lieu hien tai. File khong sua du lieu.
USE QuanLyNhaHang;
GO

-- 1. Thuc don: gia ban va so luong con trong kho.
SELECT Ma, Loai, Ten, Gia, TonKho
FROM dbo.Mon
ORDER BY Ma;

-- 2. Ho so khach hang.
SELECT Ma, HoTen, DienThoai, GioiTinh, Diem, Email
FROM dbo.KhachHang
ORDER BY Ma;

-- 3. Nhan vien.
SELECT Ma, HoTen, ChucVu, Luong, PhuCap
FROM dbo.NhanVien
ORDER BY Ma;

-- 4. Tai khoan: chi xem ten va vai tro.
SELECT TenDangNhap, VaiTro, MaLienKet
FROM dbo.TaiKhoan
ORDER BY TenDangNhap;

-- 5. Hoa don va tong tien. vDoanhThu la truy van da duoc dat ten (VIEW).
SELECT *
FROM dbo.vDoanhThu
ORDER BY NgayLap DESC, Ma;

-- 6. Cac mon trong moi hoa don.
SELECT * FROM dbo.ChiTietHoaDon ORDER BY MaHoaDon, Dong;

-- 7. Don dat va cac mon trong don.
SELECT * FROM dbo.DonDatMon ORDER BY Ma;
SELECT * FROM dbo.ChiTietDonDat ORDER BY MaDon, Dong;

-- 8. Doi chieu nhung ngay cu nhap sai, chua chuyen duoc sang kieu date.
SELECT Ma, NgayGoc
FROM dbo.HoaDon
WHERE NgayLap IS NULL;
GO
