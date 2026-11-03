USE QuanLyNhaHang;
GO
SELECT Ma, Loai, Ten, Gia, TonKho FROM dbo.Mon ORDER BY Ma;
SELECT Ma, HoTen, DienThoai, GioiTinh, Diem FROM dbo.KhachHang ORDER BY Ma;
SELECT Ma, HoTen, ChucVu, Luong, PhuCap FROM dbo.NhanVien ORDER BY Ma;
SELECT TenDangNhap, VaiTro, MaLienKet FROM dbo.TaiKhoan ORDER BY TenDangNhap;
SELECT * FROM dbo.vDoanhThu ORDER BY NgayLap DESC, Ma;  
SELECT * FROM dbo.ChiTietHoaDon ORDER BY MaHoaDon, Dong;
SELECT * FROM dbo.DonDatMon ORDER BY Ma;
SELECT * FROM dbo.ChiTietDonDat ORDER BY MaDon, Dong;
-- Cac ngay cu can doi chieu thu cong; truy van nay khong sua du lieu.
SELECT Ma, NgayGoc FROM dbo.HoaDon WHERE NgayLap IS NULL;
GO
