-- Mo trong SSMS hoac VS Code (MSSQL), ket noi localhost\SQLEXPRESS.
-- Boi den tung cau lenh roi Execute de hoc tung buoc.
USE QuanLyNhaHang;
GO

-- 1. SELECT: xem cac cot cua bang Mon.
SELECT Ma, Ten, Gia, TonKho
FROM dbo.Mon;

-- 2. WHERE: chi lay mon co gia tu 30000 tro xuong.
SELECT Ma, Ten, Gia
FROM dbo.Mon
WHERE Gia <= 30000;

-- 3. LIKE: tim ten mon chua chu com. N giup viet chu tieng Viet.
SELECT Ma, Ten
FROM dbo.Mon
WHERE Ten LIKE N'%cơm%';

-- 4. ORDER BY: sap xep gia tu thap den cao.
SELECT Ma, Ten, Gia
FROM dbo.Mon
ORDER BY Gia ASC;

-- 5. COUNT: dem so mon; SUM: cong so luong ton kho.
SELECT COUNT(*) AS SoMon, SUM(TonKho) AS TongTonKho
FROM dbo.Mon;

-- 6. JOIN: ghep hoa don voi cac mon theo cung ma hoa don.
SELECT h.Ma, h.NgayGoc, c.TenMon, c.SoLuong, c.DonGia,
       c.SoLuong * c.DonGia AS ThanhTien
FROM dbo.HoaDon AS h
JOIN dbo.ChiTietHoaDon AS c ON h.Ma = c.MaHoaDon;

-- 7. GROUP BY: tinh tong tien hang cua tung hoa don (chua giam gia).
SELECT MaHoaDon, SUM(SoLuong * DonGia) AS TienHang
FROM dbo.ChiTietHoaDon
GROUP BY MaHoaDon;

-- CAC VI DU THAY DOI DU LIEU O DUOI DANG DUOC CHU THICH.
-- Dong ung dung truoc khi sua trong SQL, mo lai de doc du lieu moi.
-- Muon thuc hanh: boi den va chay rieng cau lenh can dung trong khoi /* ... */.
-- MA_HOC_SQL la ma vi du, doi thanh ma chua co khi them mon.
/*
-- 8. INSERT: them mot mon moi. Thu tu VALUES khop voi thu tu cot.
INSERT INTO dbo.Mon
    (Ma, Loai, Ten, Gia, TonKho, DoCay, MonChay, SizeLy, CoDa)
VALUES
    (N'MA_HOC_SQL', N'MONAN', N'Com ga hoc SQL', 35000, 20, 0, 0, N'', 0);

-- 9. UPDATE: sua gia dung mon co ma nay.
-- WHERE chi ro dong can sua; bo WHERE se sua tat ca cac dong.
UPDATE dbo.Mon
SET Gia = 40000
WHERE Ma = N'MA_HOC_SQL';

-- 10. DELETE: xoa mon vi du theo ma.
-- Kiem tra ma truoc khi xoa. Bo WHERE se xoa tat ca cac dong.
DELETE FROM dbo.Mon
WHERE Ma = N'MA_HOC_SQL';
*/
