USE QuanLyNhaHang_PBL2;

-- 1. Xem thực đơn theo giá từ thấp đến cao
SELECT ma, ten, gia, tonKho
FROM ThucDon
ORDER BY gia, ten;

-- 2. Tìm các món sắp hết hàng (còn tối đa 5 phần)
SELECT ten, tonKho
FROM ThucDon
WHERE tonKho <= 5
ORDER BY tonKho;

-- 3. Xem danh sách hóa đơn kèm tên khách hàng
SELECT HoaDon.ma, HoaDon.ngay, KhachHang.ten
FROM HoaDon
LEFT JOIN KhachHang ON KhachHang.ma = HoaDon.maKhach;
