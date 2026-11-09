from pathlib import Path
p=Path('../PBLmain/README.md');s=p.read_text(encoding='utf-8');s=s.replace('Email được lưu cùng hồ sơ trong `DuLieu/khachhang.txt`; thông tin xác thực vẫn nằm trong `DuLieu/taikhoan.txt`.','Email được lưu trong cột `dbo.KhachHang.Email`; thông tin xác thực nằm trong bảng `dbo.TaiKhoan` của SQL Server.')
s=s.replace('## Chạy chương trình','Thư mục ứng dụng chính sau khi ghép nhánh là **PBLmain**. Với database cũ, chạy lại `database/schema.sql` một lần để bổ sung cột Email; dữ liệu hiện có được giữ nguyên.\n\n## Chạy chương trình',1);p.write_text(s,encoding='utf-8')
p=Path('../PBLmain/database/xem-du-lieu.sql');s=p.read_text(encoding='utf-8').replace('GioiTinh, Diem FROM','GioiTinh, Diem, Email FROM');p.write_text(s,encoding='utf-8')
