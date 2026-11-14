# Task 3: Tao tai khoan, Doi mat khau, Thong tin cua toi.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mk = $MAT_KHAU_THU
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    $soKhach = @(Doc-BangSql $hop KhachHang | Where-Object { $_.Trim() }).Count
    Khoi-Dong $hop | Out-Null
    Dang-Nhap 'ql_test'

    # 1. Doi vai tro nap lai ho so lien ket
    Bam TAI_KHOAN
    Kiem (Co-DieuKhien O_ROLE) 'tao tai khoan: co o vai tro'
    Chon-Hop O_ROLE 1
    Kiem (-not (Dang-Bat O_NHANVIEN)) 'vai tro quan ly: vo hieu ho so lien ket'
    Kiem ((Chu-MucHop O_NHANVIEN 0) -eq 'Không cần liên kết hồ sơ') 'vai tro quan ly: muc khong lien ket'
    Chon-Hop O_ROLE 2
    Kiem (Dang-Bat O_NHANVIEN) 'vai tro khach: bat ho so lien ket'
    Kiem ((So-MucHop O_NHANVIEN) -eq $soKhach) "vai tro khach: $soKhach ho so (co $(So-MucHop O_NHANVIEN))"
    Chup 'taotaikhoan' | Out-Null

    # 2. Tao tai khoan trung va hop le
    Chon-Hop O_ROLE 0; Chon-Hop O_NHANVIEN 0
    Dat-Chu O_USER 'nv_test'; Dat-Chu O_PASS $mk; Dat-Chu O_CONFIRM $mk; Bam NUT_TAOTAIKHOAN
    $tb = Cho-HopThoai -Bam OK -Giay 6
    Kiem ($null -ne $tb) "tai khoan trung: bao loi ($tb)"
    Bam TAI_KHOAN
    Chon-Hop O_ROLE 0; Chon-Hop O_NHANVIEN 1
    Dat-Chu O_USER 'nv_hai'; Dat-Chu O_PASS $mk; Dat-Chu O_CONFIRM $mk; Bam NUT_TAOTAIKHOAN
    $tb = Cho-HopThoai -Bam OK -Giay 4
    Kiem ($null -eq $tb) "tai khoan moi: khong loi ($tb)"
    Kiem ((Doc-BangSql $hop TaiKhoan) -match '\|nv_hai\|NHANVIEN\|NV02\|') 'tai khoan moi: luu nv_hai lien ket NV02'
    Kiem ((Doc-Chu O_USER) -eq '') 'tai khoan moi: lam trong bieu mau'

    # 3. Doi mat khau
    Bam MAT_KHAU
    Dat-Chu O_OLDPASS 'saimatkhau'; Dat-Chu O_PASS 'KiemThu@2027'; Dat-Chu O_CONFIRM 'KiemThu@2027'
    Bam NUT_DOIMATKHAU
    $tb = Cho-HopThoai -Bam OK -Giay 6
    Kiem ($null -ne $tb) "doi mat khau sai mat khau cu: bao loi ($tb)"
    Dat-Chu O_OLDPASS $mk; Dat-Chu O_PASS 'KiemThu@2027'; Dat-Chu O_CONFIRM 'KiemThu@2027'
    Bam NUT_DOIMATKHAU
    $tb = Cho-HopThoai -Bam OK -Giay 4
    Kiem ($null -eq $tb) "doi mat khau dung: khong loi ($tb)"
    Kiem ((Doc-Chu O_OLDPASS) -eq '' -and (Doc-Chu O_PASS) -eq '') 'doi mat khau: lam trong o'
    Chup 'doimatkhau' | Out-Null
    Dang-Xuat
    Dang-Nhap 'ql_test' 'KiemThu@2027'
    $tb = Cho-HopThoai -Bam OK -Giay 3
    Kiem ($null -eq $tb -and (Co-DieuKhien DANG_XUAT)) 'dang nhap bang mat khau moi'
    Dang-Xuat

    # 4. Khach hang: ho so chi doc, khong vao trang quan tri
    Dang-Nhap 'kh_test'
    Bam HO_SO_CUA_TOI
    Kiem (-not (Co-DieuKhien O_TEN)) 'ho so khach: khong co o nhap'
    Chup 'hoso_kh' | Out-Null
    Bam NHAN_VIEN
    $tb = Cho-HopThoai -Bam OK -Giay 4
    Kiem ($tb -like '*không có quyền*') "khach mo Nhan vien: bi chan ($tb)"
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT3_TaiKhoan'
