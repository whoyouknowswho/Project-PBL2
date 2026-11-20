# Kiem tra luoi mon cuon du nhanh: 60 lan cuon phai duoi nguong (mac dinh 1200 ms, ~ 20 ms/khung).
# Truoc khi co bo dem anh thu nho: ~2340 ms (xem KiemTra\ket_qua.txt).
#   powershell -ExecutionPolicy Bypass -File KiemTra\kiem_tra_toc_do_luoi.ps1
param([int]$NguongMs = 1200)
$ErrorActionPreference = 'Stop'
$ketQua = @()
for ($i = 0; $i -lt 3; $i++) {
    $dong = & powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $PSScriptRoot 'do_cuon_luoi.ps1')
    $ketQua += [int](($dong -replace '.*: (\d+) ms.*', '$1'))
}
$tb = [int](($ketQua | Measure-Object -Average).Average)
"3 lan do: $($ketQua -join ', ') ms; trung binh $tb ms (nguong $NguongMs ms)"
if ($tb -lt $NguongMs) { 'DAT (0 loi)' } else { 'THAT BAI (1 loi)'; exit 1 }
