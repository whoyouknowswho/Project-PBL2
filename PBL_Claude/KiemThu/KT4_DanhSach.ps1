# Task 4: Khach hang va Nhan vien (bang + bieu mau).
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
function Tim-Dong($bang, [string]$ma) { $rows = Doc-Bang $bang; for ($i = 0; $i -lt $rows.Count; $i++) { if ($rows[$i][0] -eq $ma) { return $i } }; -1 }
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    $tepKh = Join-Path $hop 'DuLieu\khachhang.txt'
    $soKhach = @(Get-Content $tepKh -Encoding UTF8 | Where-Object { $_.Trim() }).Count
    Khoi-Dong $hop | Out-Null
    Dang-Nhap 'ql_test'

    # 1. Tim kiem tieng Viet co dau
    Bam KHACH_HANG
    Kiem (-not (Dang-Bat NUT_XOA)) 'khach: Xoa vo hieu khi chua chon'
    Kiem ((So-Dong BANG_CHINH) -eq $soKhach) "khach: du $soKhach dong (co $(So-Dong BANG_CHINH))"
    Dat-Chu TIM_KIEM 'đông'
    $rows = Doc-Bang BANG_CHINH
    Kiem ($rows.Count -ge 1) 'tim "đông": co ket qua'
    Kiem (@($rows | Where-Object { ($_ -join ' ') -notmatch 'đông' }).Count -eq 0) 'tim "đông": moi dong deu khop'
    Dat-Chu TIM_KIEM ''
    Kiem ((So-Dong BANG_CHINH) -eq $soKhach) 'xoa tim kiem: hien lai tat ca'
    Chup 'khachhang' | Out-Null

    # 2. Them khach moi
    Dat-Chu O_MA 'KH9001'; Dat-Chu O_TEN 'Nguyễn Văn Thử'; Dat-Chu O_SDT '0911222333'
    Dat-Chu O_DIEM '120'; Dat-Chu O_EMAIL 'thu@example.com'
    Bam NUT_LUU
    $tb = Cho-HopThoai -Bam OK -Giay 3
    Kiem ($null -eq $tb) "luu khach moi: khong loi ($tb)"
    Kiem ([bool]((Get-Content $tepKh -Encoding UTF8) -match '^KH9001\|Nguyễn Văn Thử\|0911222333\|')) 'luu khach moi: ghi tep'
    Kiem (Chi-Doc O_MA) 'sau khi luu: ma chi doc'
    Kiem (Dang-Bat NUT_XOA) 'sau khi luu: bat nut Xoa'

    # 3. Kiem tra o so
    $truoc = Doc-Chu O_DIEM
    Go-Phim O_DIEM 'abc'
    Kiem ((Doc-Chu O_DIEM) -eq $truoc) 'o diem: khong nhan chu cai'
    Dat-Chu O_DIEM '2000000000'; Bam NUT_LUU
    $tb = Cho-HopThoai -Bam OK -Giay 4
    Kiem ($tb -like '*số không hợp lệ*') "diem qua lon: bao loi ($tb)"

    # 4. Xoa co xac nhan
    $i = Tim-Dong BANG_CHINH 'KH9001'
    Kiem ($i -ge 0) 'tim thay KH9001 trong bang'
    Chon-Dong BANG_CHINH $i
    Kiem ((Doc-Chu O_MA) -eq 'KH9001') 'chon dong: nap bieu mau'
    Bam NUT_XOA
    [void](Cho-HopThoai -Bam No)
    Kiem ([bool]((Get-Content $tepKh -Encoding UTF8) -match '^KH9001\|')) 'xoa -> No: van con'
    Bam NUT_XOA
    [void](Cho-HopThoai -Bam Yes)
    Kiem (-not ((Get-Content $tepKh -Encoding UTF8) -match '^KH9001\|')) 'xoa -> Yes: da xoa khoi tep'
    Kiem ((Doc-Chu O_MA) -eq '' -and -not (Dang-Bat NUT_XOA)) 'sau khi xoa: bieu mau trong'

    # 5. Nhan vien: chon dong va o so thuc
    Bam NHAN_VIEN
    Chon-Dong BANG_CHINH 0
    Kiem ((Doc-Chu O_LUONG) -eq '7000000') "nhan vien dong 0: luong 7000000 ($(Doc-Chu O_LUONG))"
    Kiem ((Doc-Chu O_MA) -eq 'NV01') 'nhan vien dong 0: ma NV01'
    Dat-Chu O_LUONG ''
    Go-Phim O_LUONG '1.2.3'
    Kiem ((Doc-Chu O_LUONG) -eq '1.23') "o luong: mot dau cham ($(Doc-Chu O_LUONG))"
    Chup 'nhanvien' | Out-Null
    Bam NUT_MOI
    Kiem ((Doc-Chu O_MA) -eq '' -and -not (Chi-Doc O_MA)) 'Them moi: lam trong va mo khoa ma'
    Dang-Xuat

    # 6. Nhan vien ban hang: khong co trang Nhan vien
    Dang-Nhap 'nv_test'
    Kiem (-not (Co-DieuKhien NHAN_VIEN)) 'nv_test: khong co muc Nhan vien'
    Bam KHACH_HANG
    Kiem (Co-DieuKhien BANG_CHINH) 'nv_test: mo duoc Khach hang'
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT4_DanhSach'
