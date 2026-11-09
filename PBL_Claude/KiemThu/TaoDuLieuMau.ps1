# Tao bo du lieu mau dung chung cho cac kich ban: ql_test (quan ly), nv_test (NV01), kh_test (khach moi).
# Ket qua: %TEMP%\pbl_kiemthu\mau\DuLieu. Dung -LamMoi de tao lai.
param([switch]$LamMoi)
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau'
$tk = Join-Path $mau 'DuLieu\taikhoan.txt'
if ((Test-Path $tk) -and -not $LamMoi) { Write-Host "TaoDuLieuMau: dung lai $mau"; exit 0 }
Remove-Item $mau -Recurse -Force -ErrorAction SilentlyContinue
$mk = $MAT_KHAU_THU
try {
    $hop = Tao-HopCat -TaiKhoanTrong
    Khoi-Dong $hop | Out-Null
    Dat-Chu O_USER 'ql_test'; Dat-Chu O_PASS $mk; Dat-Chu O_CONFIRM $mk; Bam NUT_DANGNHAP
    [void](Cho-HopThoai -Bam OK -Giay 10)
    Dang-Nhap 'ql_test'
    Bam TAI_KHOAN
    Dat-Chu O_USER 'nv_test'; Chon-Hop O_ROLE 0; Chon-Hop O_NHANVIEN 0
    Dat-Chu O_PASS $mk; Dat-Chu O_CONFIRM $mk; Bam NUT_TAOTAIKHOAN
    $loi = Cho-HopThoai -Bam OK -Giay 3
    if ($loi) { throw "Tao nv_test loi: $loi" }
    Dang-Xuat
    Bam NUT_MO_DANGKY
    Dat-Chu O_TEN 'Khách Thử'; Dat-Chu O_SDT '0901234567'; Dat-Chu O_EMAIL 'khach.thu@example.com'
    Dat-Chu O_USER 'kh_test'; Dat-Chu O_PASS $mk; Dat-Chu O_CONFIRM $mk; Bam NUT_DANGKY
    $tb = Cho-HopThoai -Bam OK -Giay 10
    if ($tb -notlike '*Đăng ký thành công*') { throw "Dang ky kh_test loi: $tb" }
    Dong-UngDung
    $noiDung = Get-Content (Join-Path $hop 'DuLieu\taikhoan.txt') -Raw
    if ($noiDung -notmatch '\|nv_test\|NHANVIEN\|NV01\|' -or $noiDung -notmatch '\|kh_test\|KHACHHANG\|') { throw 'Thieu tai khoan mau' }
    New-Item -ItemType Directory -Force $mau | Out-Null
    Copy-Item (Join-Path $hop 'DuLieu') (Join-Path $mau 'DuLieu') -Recurse
    Write-Host "TaoDuLieuMau: xong $mau"
    exit 0
} catch {
    Dong-UngDung
    Write-Host "TaoDuLieuMau LOI: $_"
    exit 1
}
