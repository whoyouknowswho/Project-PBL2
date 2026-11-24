-- Du lieu mau SQLite: khoi tao 6 bang (neu chua co), xoa tai khoan cu, tao QUANLY + 10 nhan vien + 20 khach hang,
-- them 50 mon an (MA0003-MA0052) va 50 nuoc uong (NU0001-NU0050), anh tai DuLieu/AnhMon/<ma>.jpg. Chay lai nhieu lan van an toan.
BEGIN TRANSACTION;

CREATE TABLE IF NOT EXISTS ThucDon (
    loai TEXT NOT NULL,
    ma TEXT PRIMARY KEY,
    ten TEXT NOT NULL,
    gia REAL NOT NULL DEFAULT 0,
    tonKho INTEGER NOT NULL DEFAULT 0,
    doCayHoacSize TEXT NOT NULL DEFAULT '',
    monChayHoacCoDa INTEGER NOT NULL DEFAULT 0,
    hinhAnh TEXT NOT NULL DEFAULT ''
);
CREATE TABLE IF NOT EXISTS KhachHang (
    ma TEXT PRIMARY KEY,
    ten TEXT NOT NULL,
    dienThoai TEXT NOT NULL DEFAULT '',
    gioiTinh TEXT NOT NULL DEFAULT '',
    diem INTEGER NOT NULL DEFAULT 0,
    email TEXT NOT NULL DEFAULT ''
);
CREATE TABLE IF NOT EXISTS NhanVien (
    ma TEXT PRIMARY KEY,
    ten TEXT NOT NULL,
    dienThoai TEXT NOT NULL DEFAULT '',
    gioiTinh TEXT NOT NULL DEFAULT '',
    chucVu TEXT NOT NULL DEFAULT '',
    luong REAL NOT NULL DEFAULT 0,
    phuCap REAL NOT NULL DEFAULT 0
);
CREATE TABLE IF NOT EXISTS HoaDon (
    ma TEXT PRIMARY KEY,
    ngay TEXT NOT NULL,
    maKhach TEXT NOT NULL DEFAULT '',
    maNhanVien TEXT NOT NULL DEFAULT '',
    giamGia REAL NOT NULL DEFAULT 0,
    chiTiet TEXT NOT NULL DEFAULT ''
);
CREATE TABLE IF NOT EXISTS DonDat (
    thuTu INTEGER PRIMARY KEY,
    noiDung TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS TaiKhoan (
    tenDangNhap TEXT PRIMARY KEY,
    matKhau TEXT NOT NULL,
    vaiTro TEXT NOT NULL,
    maLienKet TEXT NOT NULL DEFAULT ''
);

DELETE FROM TaiKhoan;
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('QUANLY', '123456789', 'QUANLY', '');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien01', '123456789', 'NHANVIEN', 'NV0001');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien02', '123456789', 'NHANVIEN', 'NV0002');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien03', '123456789', 'NHANVIEN', 'NV0003');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien04', '123456789', 'NHANVIEN', 'NV0004');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien05', '123456789', 'NHANVIEN', 'NV0005');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien06', '123456789', 'NHANVIEN', 'NV0006');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien07', '123456789', 'NHANVIEN', 'NV0007');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien08', '123456789', 'NHANVIEN', 'NV0008');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien09', '123456789', 'NHANVIEN', 'NV0009');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('nhanvien10', '123456789', 'NHANVIEN', 'NV0010');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang01', '123456789', 'KHACHHANG', 'KH0001');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang02', '123456789', 'KHACHHANG', 'KH0002');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang03', '123456789', 'KHACHHANG', 'KH0003');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang04', '123456789', 'KHACHHANG', 'KH0004');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang05', '123456789', 'KHACHHANG', 'KH0005');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang06', '123456789', 'KHACHHANG', 'KH0006');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang07', '123456789', 'KHACHHANG', 'KH0007');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang08', '123456789', 'KHACHHANG', 'KH0008');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang09', '123456789', 'KHACHHANG', 'KH0009');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang10', '123456789', 'KHACHHANG', 'KH0010');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang11', '123456789', 'KHACHHANG', 'KH0011');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang12', '123456789', 'KHACHHANG', 'KH0012');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang13', '123456789', 'KHACHHANG', 'KH0013');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang14', '123456789', 'KHACHHANG', 'KH0014');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang15', '123456789', 'KHACHHANG', 'KH0015');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang16', '123456789', 'KHACHHANG', 'KH0016');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang17', '123456789', 'KHACHHANG', 'KH0017');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang18', '123456789', 'KHACHHANG', 'KH0018');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang19', '123456789', 'KHACHHANG', 'KH0019');
INSERT INTO TaiKhoan (tenDangNhap, matKhau, vaiTro, maLienKet) VALUES ('khachhang20', '123456789', 'KHACHHANG', 'KH0020');

DELETE FROM NhanVien WHERE ma IN ('NV0001', 'NV0002', 'NV0003', 'NV0004', 'NV0005', 'NV0006', 'NV0007', 'NV0008', 'NV0009', 'NV0010');
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0001', 'Lê Văn Tài', '0795881745', 'Nam', 'Thu ngân', 7000000, 500000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0002', 'Trần Thị Mai', '0912345602', 'Nữ', 'Phục vụ', 6000000, 400000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0003', 'Nguyễn Hoàng Nam', '0912345603', 'Nam', 'Đầu bếp', 12000000, 1000000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0004', 'Phạm Thu Hà', '0912345604', 'Nữ', 'Pha chế', 7500000, 500000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0005', 'Võ Minh Khang', '0912345605', 'Nam', 'Phụ bếp', 6500000, 400000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0006', 'Đặng Ngọc Lan', '0912345606', 'Nữ', 'Thu ngân', 7000000, 500000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0007', 'Bùi Quốc Bảo', '0912345607', 'Nam', 'Phục vụ', 6000000, 400000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0008', 'Huỳnh Thị Thảo', '0912345608', 'Nữ', 'Phục vụ', 6000000, 400000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0009', 'Lý Gia Huy', '0912345609', 'Nam', 'Giám sát ca', 9000000, 800000);
INSERT INTO NhanVien (ma, ten, dienThoai, gioiTinh, chucVu, luong, phuCap) VALUES ('NV0010', 'Hồ Thanh Tâm', '0912345610', 'Nữ', 'Pha chế', 7500000, 500000);

DELETE FROM KhachHang WHERE ma IN ('KH0001', 'KH0002', 'KH0003', 'KH0004', 'KH0005', 'KH0006', 'KH0007', 'KH0008', 'KH0009', 'KH0010', 'KH0011', 'KH0012', 'KH0013', 'KH0014', 'KH0015', 'KH0016', 'KH0017', 'KH0018', 'KH0019', 'KH0020');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0001', 'Tô Đông Phước', '0905554790', 'Nam', 10, 'todongphuoc2008@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0002', 'Nguyễn Văn A', '0905554798', 'Nam', 3, 'nguyenvana@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0003', 'Lê Thị Hồng', '0935111203', 'Nữ', 120, 'lethihong03@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0004', 'Trần Minh Đức', '0935111204', 'Nam', 250, 'tranminhduc04@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0005', 'Phạm Ngọc Ánh', '0935111205', 'Nữ', 40, 'phamngocanh05@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0006', 'Ngô Văn Phúc', '0935111206', 'Nam', 560, 'ngovanphuc06@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0007', 'Đỗ Thị Hạnh', '0935111207', 'Nữ', 0, 'dothihanh07@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0008', 'Hoàng Gia Bảo', '0935111208', 'Nam', 1050, 'hoanggiabao08@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0009', 'Vũ Thị Kim Chi', '0935111209', 'Nữ', 75, 'vuthikimchi09@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0010', 'Phan Thanh Sơn', '0935111210', 'Nam', 210, 'phanthanhson10@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0011', 'Trịnh Mỹ Linh', '0935111211', 'Nữ', 15, 'trinhmylinh11@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0012', 'Mai Xuân Trường', '0935111212', 'Nam', 330, 'maixuantruong12@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0013', 'Cao Thị Yến', '0935111213', 'Nữ', 0, 'caothiyen13@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0014', 'Dương Văn Lực', '0935111214', 'Nam', 640, 'duongvanluc14@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0015', 'Lâm Bảo Ngọc', '0935111215', 'Nữ', 90, 'lambaongoc15@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0016', 'Tạ Quang Vinh', '0935111216', 'Nam', 5, 'taquangvinh16@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0017', 'Châu Thị Diễm', '0935111217', 'Nữ', 205, 'chauthidiem17@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0018', 'La Hữu Thắng', '0935111218', 'Nam', 1200, 'lahuuthang18@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0019', 'Kiều Anh Thư', '0935111219', 'Nữ', 60, 'kieuanhthu19@gmail.com');
INSERT INTO KhachHang (ma, ten, dienThoai, gioiTinh, diem, email) VALUES ('KH0020', 'Quách Tuấn Kiệt', '0935111220', 'Nam', 0, 'quachtuankiet20@gmail.com');

-- Bo khoang trang thua o ten 2 mon cu.
UPDATE ThucDon SET ten = TRIM(ten) WHERE ma IN ('MA0001', 'MA0002');
DELETE FROM ThucDon WHERE ma IN ('MA0003', 'MA0004', 'MA0005', 'MA0006', 'MA0007', 'MA0008', 'MA0009', 'MA0010', 'MA0011', 'MA0012', 'MA0013', 'MA0014', 'MA0015', 'MA0016', 'MA0017', 'MA0018', 'MA0019', 'MA0020', 'MA0021', 'MA0022', 'MA0023', 'MA0024', 'MA0025', 'MA0026', 'MA0027', 'MA0028', 'MA0029', 'MA0030', 'MA0031', 'MA0032', 'MA0033', 'MA0034', 'MA0035', 'MA0036', 'MA0037', 'MA0038', 'MA0039', 'MA0040', 'MA0041', 'MA0042', 'MA0043', 'MA0044', 'MA0045', 'MA0046', 'MA0047', 'MA0048', 'MA0049', 'MA0050', 'MA0051', 'MA0052', 'NU0001', 'NU0002', 'NU0003', 'NU0004', 'NU0005', 'NU0006', 'NU0007', 'NU0008', 'NU0009', 'NU0010', 'NU0011', 'NU0012', 'NU0013', 'NU0014', 'NU0015', 'NU0016', 'NU0017', 'NU0018', 'NU0019', 'NU0020', 'NU0021', 'NU0022', 'NU0023', 'NU0024', 'NU0025', 'NU0026', 'NU0027', 'NU0028', 'NU0029', 'NU0030', 'NU0031', 'NU0032', 'NU0033', 'NU0034', 'NU0035', 'NU0036', 'NU0037', 'NU0038', 'NU0039', 'NU0040', 'NU0041', 'NU0042', 'NU0043', 'NU0044', 'NU0045', 'NU0046', 'NU0047', 'NU0048', 'NU0049', 'NU0050');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0003', 'Phở bò tái', 45000, 40, '0', 0, 'DuLieu/AnhMon/MA0003.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0004', 'Phở gà', 40000, 40, '0', 0, 'DuLieu/AnhMon/MA0004.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0005', 'Bún bò Huế', 45000, 35, '3', 0, 'DuLieu/AnhMon/MA0005.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0006', 'Bún chả Hà Nội', 45000, 30, '1', 0, 'DuLieu/AnhMon/MA0006.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0007', 'Bún riêu cua', 40000, 30, '1', 0, 'DuLieu/AnhMon/MA0007.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0008', 'Bún thịt nướng', 40000, 30, '0', 0, 'DuLieu/AnhMon/MA0008.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0009', 'Bún mắm nêm', 40000, 25, '2', 0, 'DuLieu/AnhMon/MA0009.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0010', 'Hủ tiếu Nam Vang', 45000, 30, '0', 0, 'DuLieu/AnhMon/MA0010.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0011', 'Mì xào hải sản', 55000, 25, '1', 0, 'DuLieu/AnhMon/MA0011.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0012', 'Mì Quảng gà', 40000, 30, '1', 0, 'DuLieu/AnhMon/MA0012.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0013', 'Cao lầu', 45000, 20, '0', 0, 'DuLieu/AnhMon/MA0013.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0014', 'Cơm tấm sườn bì chả', 50000, 40, '0', 0, 'DuLieu/AnhMon/MA0014.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0015', 'Cơm gà xối mỡ', 45000, 35, '0', 0, 'DuLieu/AnhMon/MA0015.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0016', 'Cơm chiên dương châu', 40000, 35, '0', 0, 'DuLieu/AnhMon/MA0016.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0017', 'Cơm chiên hải sản', 55000, 25, '0', 0, 'DuLieu/AnhMon/MA0017.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0018', 'Cơm sườn rim', 45000, 30, '0', 0, 'DuLieu/AnhMon/MA0018.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0019', 'Cơm bò lúc lắc', 65000, 20, '1', 0, 'DuLieu/AnhMon/MA0019.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0020', 'Cơm chay thập cẩm', 35000, 25, '0', 1, 'DuLieu/AnhMon/MA0020.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0021', 'Bánh mì thịt nướng', 25000, 50, '1', 0, 'DuLieu/AnhMon/MA0021.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0022', 'Bánh mì ốp la', 20000, 50, '0', 0, 'DuLieu/AnhMon/MA0022.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0023', 'Bánh xèo miền Tây', 40000, 25, '0', 0, 'DuLieu/AnhMon/MA0023.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0024', 'Bánh cuốn nóng', 35000, 30, '0', 0, 'DuLieu/AnhMon/MA0024.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0025', 'Bánh canh cua', 50000, 25, '1', 0, 'DuLieu/AnhMon/MA0025.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0026', 'Bánh bèo chén', 30000, 30, '1', 0, 'DuLieu/AnhMon/MA0026.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0027', 'Gỏi cuốn tôm thịt', 30000, 40, '0', 0, 'DuLieu/AnhMon/MA0027.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0028', 'Gỏi cuốn chay', 25000, 30, '0', 1, 'DuLieu/AnhMon/MA0028.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0029', 'Chả giò rế', 35000, 40, '0', 0, 'DuLieu/AnhMon/MA0029.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0030', 'Nem lụi Huế', 45000, 25, '1', 0, 'DuLieu/AnhMon/MA0030.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0031', 'Gỏi ngó sen tôm thịt', 60000, 20, '2', 0, 'DuLieu/AnhMon/MA0031.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0032', 'Gỏi gà xé phay', 55000, 20, '2', 0, 'DuLieu/AnhMon/MA0032.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0033', 'Lẩu thái hải sản', 250000, 15, '4', 0, 'DuLieu/AnhMon/MA0033.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0034', 'Lẩu gà lá é', 220000, 15, '2', 0, 'DuLieu/AnhMon/MA0034.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0035', 'Lẩu nấm chay', 180000, 15, '0', 1, 'DuLieu/AnhMon/MA0035.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0036', 'Cá kho tộ', 60000, 20, '1', 0, 'DuLieu/AnhMon/MA0036.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0037', 'Thịt kho trứng', 55000, 25, '0', 0, 'DuLieu/AnhMon/MA0037.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0038', 'Canh chua cá lóc', 60000, 20, '1', 0, 'DuLieu/AnhMon/MA0038.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0039', 'Gà nướng muối ớt', 120000, 15, '3', 0, 'DuLieu/AnhMon/MA0039.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0040', 'Sườn nướng mật ong', 90000, 20, '0', 0, 'DuLieu/AnhMon/MA0040.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0041', 'Bò nướng lá lốt', 70000, 25, '1', 0, 'DuLieu/AnhMon/MA0041.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0042', 'Mực xào sa tế', 85000, 20, '4', 0, 'DuLieu/AnhMon/MA0042.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0043', 'Tôm rang muối', 95000, 20, '2', 0, 'DuLieu/AnhMon/MA0043.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0044', 'Ốc len xào dừa', 65000, 20, '2', 0, 'DuLieu/AnhMon/MA0044.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0045', 'Rau muống xào tỏi', 35000, 40, '0', 1, 'DuLieu/AnhMon/MA0045.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0046', 'Đậu hũ sốt cà chua', 35000, 30, '0', 1, 'DuLieu/AnhMon/MA0046.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0047', 'Đậu hũ tứ xuyên', 45000, 25, '5', 1, 'DuLieu/AnhMon/MA0047.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0048', 'Cháo lòng', 35000, 25, '0', 0, 'DuLieu/AnhMon/MA0048.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0049', 'Cháo gà hành', 35000, 25, '0', 0, 'DuLieu/AnhMon/MA0049.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0050', 'Xôi gà', 30000, 30, '0', 0, 'DuLieu/AnhMon/MA0050.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0051', 'Mì cay hải sản', 55000, 30, '5', 0, 'DuLieu/AnhMon/MA0051.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('MONAN', 'MA0052', 'Khoai tây chiên', 30000, 50, '0', 1, 'DuLieu/AnhMon/MA0052.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0001', 'Cà phê đen đá', 20000, 80, 'M', 1, 'DuLieu/AnhMon/NU0001.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0002', 'Cà phê sữa đá', 25000, 80, 'M', 1, 'DuLieu/AnhMon/NU0002.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0003', 'Bạc xỉu', 28000, 60, 'M', 1, 'DuLieu/AnhMon/NU0003.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0004', 'Cà phê muối', 32000, 50, 'M', 1, 'DuLieu/AnhMon/NU0004.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0005', 'Cà phê cốt dừa', 35000, 40, 'L', 1, 'DuLieu/AnhMon/NU0005.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0006', 'Espresso', 30000, 40, 'S', 0, 'DuLieu/AnhMon/NU0006.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0007', 'Cappuccino', 40000, 40, 'M', 0, 'DuLieu/AnhMon/NU0007.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0008', 'Latte', 42000, 40, 'M', 0, 'DuLieu/AnhMon/NU0008.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0009', 'Americano', 35000, 40, 'L', 1, 'DuLieu/AnhMon/NU0009.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0010', 'Mocha', 45000, 30, 'M', 0, 'DuLieu/AnhMon/NU0010.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0011', 'Trà đào cam sả', 35000, 60, 'L', 1, 'DuLieu/AnhMon/NU0011.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0012', 'Trà vải', 35000, 50, 'L', 1, 'DuLieu/AnhMon/NU0012.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0013', 'Trà chanh', 15000, 80, 'L', 1, 'DuLieu/AnhMon/NU0013.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0014', 'Trà tắc', 15000, 80, 'L', 1, 'DuLieu/AnhMon/NU0014.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0015', 'Trà sữa trân châu', 30000, 60, 'M', 1, 'DuLieu/AnhMon/NU0015.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0016', 'Trà sữa matcha', 35000, 50, 'M', 1, 'DuLieu/AnhMon/NU0016.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0017', 'Trà sữa khoai môn', 35000, 40, 'M', 1, 'DuLieu/AnhMon/NU0017.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0018', 'Hồng trà sữa', 30000, 50, 'M', 1, 'DuLieu/AnhMon/NU0018.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0019', 'Trà xanh nhài', 20000, 50, 'M', 1, 'DuLieu/AnhMon/NU0019.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0020', 'Trà ô long', 25000, 40, 'M', 0, 'DuLieu/AnhMon/NU0020.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0021', 'Nước cam ép', 30000, 40, 'M', 1, 'DuLieu/AnhMon/NU0021.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0022', 'Nước ép dưa hấu', 30000, 40, 'M', 1, 'DuLieu/AnhMon/NU0022.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0023', 'Nước ép táo', 35000, 30, 'M', 1, 'DuLieu/AnhMon/NU0023.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0024', 'Nước ép ổi', 30000, 30, 'M', 1, 'DuLieu/AnhMon/NU0024.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0025', 'Nước ép cà rốt', 30000, 30, 'M', 1, 'DuLieu/AnhMon/NU0025.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0026', 'Nước ép thơm', 30000, 30, 'M', 1, 'DuLieu/AnhMon/NU0026.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0027', 'Sinh tố bơ', 40000, 30, 'L', 1, 'DuLieu/AnhMon/NU0027.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0028', 'Sinh tố xoài', 35000, 30, 'L', 1, 'DuLieu/AnhMon/NU0028.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0029', 'Sinh tố dâu', 40000, 30, 'L', 1, 'DuLieu/AnhMon/NU0029.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0030', 'Sinh tố mãng cầu', 40000, 25, 'L', 1, 'DuLieu/AnhMon/NU0030.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0031', 'Sinh tố sapoche', 35000, 25, 'L', 1, 'DuLieu/AnhMon/NU0031.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0032', 'Nước dừa tươi', 25000, 40, 'L', 0, 'DuLieu/AnhMon/NU0032.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0033', 'Nước mía', 15000, 60, 'L', 1, 'DuLieu/AnhMon/NU0033.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0034', 'Rau má đậu xanh', 25000, 40, 'L', 1, 'DuLieu/AnhMon/NU0034.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0035', 'Sâm bí đao', 15000, 50, 'L', 1, 'DuLieu/AnhMon/NU0035.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0036', 'Chanh dây', 25000, 40, 'M', 1, 'DuLieu/AnhMon/NU0036.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0037', 'Soda chanh', 25000, 40, 'M', 1, 'DuLieu/AnhMon/NU0037.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0038', 'Soda việt quất', 35000, 30, 'M', 1, 'DuLieu/AnhMon/NU0038.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0039', 'Sữa chua đá', 25000, 40, 'M', 1, 'DuLieu/AnhMon/NU0039.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0040', 'Yaourt trái cây', 35000, 30, 'M', 1, 'DuLieu/AnhMon/NU0040.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0041', 'Cacao đá', 30000, 40, 'M', 1, 'DuLieu/AnhMon/NU0041.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0042', 'Socola nóng', 35000, 30, 'M', 0, 'DuLieu/AnhMon/NU0042.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0043', 'Matcha đá xay', 45000, 30, 'L', 1, 'DuLieu/AnhMon/NU0043.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0044', 'Cookie đá xay', 45000, 30, 'L', 1, 'DuLieu/AnhMon/NU0044.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0045', 'Nước suối', 10000, 100, 'S', 0, 'DuLieu/AnhMon/NU0045.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0046', 'Coca-Cola', 15000, 100, 'S', 1, 'DuLieu/AnhMon/NU0046.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0047', 'Pepsi', 15000, 100, 'S', 1, 'DuLieu/AnhMon/NU0047.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0048', '7Up', 15000, 100, 'S', 1, 'DuLieu/AnhMon/NU0048.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0049', 'Bia Huda', 20000, 100, 'S', 1, 'DuLieu/AnhMon/NU0049.jpg');
INSERT INTO ThucDon (loai, ma, ten, gia, tonKho, doCayHoacSize, monChayHoacCoDa, hinhAnh) VALUES ('NUOC', 'NU0050', 'Bia Saigon', 20000, 100, 'S', 1, 'DuLieu/AnhMon/NU0050.jpg');

COMMIT;
