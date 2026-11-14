USE QuanLyNhaHang_PBL2;

-- Xem thực đơn theo giá từ thấp đến cao.
SELECT ma, ten, gia, tonKho
FROM ThucDon
ORDER BY gia, ten;

-- Tìm các món cần nhập thêm (còn tối đa 5 phần).
SELECT ten, tonKho
FROM ThucDon
WHERE tonKho <= 5
ORDER BY tonKho;

-- Ghép hóa đơn với tên khách. Hóa đơn của khách vãng lai vẫn xuất hiện.
SELECT hd.ma, hd.ngay, COALESCE(kh.ten, N'Khách vãng lai') AS tenKhach
FROM HoaDon AS hd
LEFT JOIN KhachHang AS kh ON kh.ma = hd.maKhach;
