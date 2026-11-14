# Task 2: dang nhap, thiet lap lan dau, dang ky khach, gioi han 3 lan sai, khoa mot phien.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force
$ErrorActionPreference = 'Stop'
$mk = $MAT_KHAU_THU
try {
    $hop = Tao-HopCat -TaiKhoanTrong
    Khoi-Dong $hop | Out-Null

    # 1. Che do thiet lap lan dau
    Kiem (Co-DieuKhien O_CONFIRM) 'thiet lap: co o Nhap lai mat khau'
    Kiem (-not (Co-DieuKhien NUT_MO_DANGKY)) 'thiet lap: khong co nut dang ky'
    Chup 'thietlap' | Out-Null

    # 2. Tao quan ly dau tien
    Dat-Chu O_USER 'ql_test'; Dat-Chu O_PASS $mk; Dat-Chu O_CONFIRM $mk
    Bam NUT_DANGNHAP
    $tb = Cho-HopThoai -Bam OK -Giay 10
    Kiem ($tb -like '*Đã tạo tài khoản quản lý*') "thiet lap: thong bao tao quan ly ($tb)"
    Kiem ((Doc-Chu O_USER) -eq 'ql_test') 'thiet lap: dien san ten dang nhap'
    Kiem (-not (Co-DieuKhien O_CONFIRM)) 'sau thiet lap: man dang nhap thuong'
    Chup 'dangnhap' | Out-Null

    # 3. Sai mat khau lan 1 (bang phim Enter)
    Dat-Chu O_PASS 'saimatkhau1'
    Nhan-Enter O_PASS
    $tb = Cho-HopThoai -Bam OK -Giay 10
    Kiem ($tb -like '*Sai tài khoản*') "Enter + sai mat khau: bao loi ($tb)"
    Kiem ((Doc-Chu O_PASS) -eq '') 'sai mat khau: xoa o mat khau'

    # 4. Chuyen qua lai man dang ky
    Bam NUT_MO_DANGKY
    Kiem (Co-DieuKhien O_EMAIL) 'dang ky: co o email'
    Kiem ((Muc-Hop O_GIOITINH) -eq 2) 'dang ky: gioi tinh mac dinh Khac'
    Chup 'dangky' | Out-Null
    Bam NUT_VE_DANGNHAP
    Kiem (Co-DieuKhien NUT_DANGNHAP) 've dang nhap: co nut dang nhap'
    Kiem (-not (Co-DieuKhien O_EMAIL)) 've dang nhap: khong con o email'

    # 5. Dang ky khach hang
    Bam NUT_MO_DANGKY
    Dat-Chu O_TEN 'Khách Thử'; Dat-Chu O_SDT '0901234567'; Dat-Chu O_EMAIL 'khach.thu@example.com'
    Dat-Chu O_USER 'kh_test'; Dat-Chu O_PASS $mk; Dat-Chu O_CONFIRM $mk
    Bam NUT_DANGKY
    $tb = Cho-HopThoai -Bam OK -Giay 10
    Kiem ($tb -like '*Đăng ký thành công*') "dang ky: thanh cong ($tb)"
    Kiem ((Doc-Chu O_USER) -eq 'kh_test') 'dang ky: dien san ten dang nhap'
    Kiem ([bool]((Doc-BangSql $hop KhachHang) -match 'Khách Thử')) 'dang ky: luu ho so khach'

    # 6. Dang nhap quan ly: thanh ben dung quyen
    Dang-Nhap 'ql_test'
    foreach ($id in 'TRANG_CHU','KHACH_HANG','NHAN_VIEN','THUC_DON','BAO_CAO','TAI_KHOAN','MAT_KHAU','DANG_XUAT') {
        Kiem (Co-DieuKhien $id) "quan ly: co muc $id"
    }
    foreach ($id in 'BAN_HANG','DON_DAT','DAT_MON') { Kiem (-not (Co-DieuKhien $id)) "quan ly: khong co muc $id" }
    Chup 'dangnhap_ql' | Out-Null

    # 7. Dang xuat co hoi; sai 3 lan thi dong ung dung
    Bam DANG_XUAT
    $tb = Cho-HopThoai -Bam No
    Kiem ($tb -like '*đăng xuất*') "dang xuat: hoi xac nhan ($tb)"
    Kiem (Co-DieuKhien DANG_XUAT) 'dang xuat No: van dang nhap'
    Dang-Xuat
    Kiem (Co-DieuKhien NUT_DANGNHAP) 'dang xuat Yes: ve man dang nhap'
    Dang-Nhap 'ql_test'
    $tb = Cho-HopThoai -Bam OK -Giay 10
    Kiem ($null -eq $tb) 'dang nhap lai: khong co loi'
    Dang-Xuat
    $tb = $null
    foreach ($lan in 1..3) {
        Dat-Chu O_USER 'kh_test'; Dat-Chu O_PASS 'sai'; Bam NUT_DANGNHAP
        $tb = Cho-HopThoai -Bam OK -Giay 10
    }
    Kiem ($tb -like '*sai 3 lần*') "sai 3 lan: thong bao ($tb)"
    Kiem (Cho-Thoat 5000) 'sai 3 lan: ung dung dong'

    # 8. Khoa mot phien cho mot thu muc du lieu
    Khoi-Dong $hop | Out-Null
    $p2 = Start-Process (Join-Path $hop 'QuanLyNhaHangGUI.exe') -PassThru
    $tb2 = $null
    for ($i = 0; $i -lt 60 -and -not $tb2; $i++) {
        Start-Sleep -Milliseconds 100
        foreach ($h in [KT.W32]::TopWindows($p2.Id)) {
            if ([KT.W32]::ClassOf($h) -eq '#32770') {
                $tb2 = [KT.W32]::Text([KT.W32]::GetDlgItem($h, 0xFFFF))
                [void][KT.W32]::PostMessageW($h, 0x0111, [IntPtr]2, [IntPtr]::Zero)
            }
        }
    }
    Kiem ($tb2 -like '*Chương trình đang mở*') "phien thu hai: thong bao ($tb2)"
    Kiem ($p2.WaitForExit(5000)) 'phien thu hai: tu thoat'
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT2_DangNhap'
