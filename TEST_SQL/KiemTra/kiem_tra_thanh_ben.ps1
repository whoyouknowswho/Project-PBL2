# Kiem tra chu tren thanh ben sang doc duoc va cac khoi toi con giu nen toi (lay mau diem anh).
#   powershell -ExecutionPolicy Bypass -File KiemTra\kiem_tra_thanh_ben.ps1
param([string]$Exe = (Join-Path $PSScriptRoot '..\QuanLyNhaHangGUI.exe'))
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'WinApi.ps1')
$Exe = (Resolve-Path $Exe).Path
Get-Process QuanLyNhaHangGUI -ErrorAction SilentlyContinue | Stop-Process -Force
$loi = 0
function Can([string]$ten, [bool]$dat, [string]$chiTiet) {
    "{0,-48} {1,-4} {2}" -f $ten, $(if ($dat) { 'ok' } else { 'SAI' }), $chiTiet
    if (-not $dat) { $script:loi++ }
}

# Man dang nhap: panel trai phai la nen toi.
$p = Mo-App $Exe
$b = Chup-Client $p.MainWindowHandle
$s = DoSang-TrungBinh $b 10 10 60 60
Can 'Dang nhap: panel trai nen toi' ($s -lt 80) "do sang TB = $([int]$s)"
$b.Dispose(); Stop-Process -Id $p.Id -Force

# Quan ly, trang Tong quan: chu tren thanh ben phai toi (doc duoc tren nen sang).
$p = Mo-App $Exe; $h = $p.MainWindowHandle
Dang-Nhap $h 'QUANLY' '123456789'
Bam-Nut $h 100
$b = Chup-Client $h
$n = Dem-DiemToi $b 78 28 236 50
Can 'Thanh ben: chu "NHA HANG" toi' ($n -ge 40) "diem toi = $n"
$nut = [KiemTraW]::GetDlgItem($h, 100)
$r = New-Object KiemTraW+RECT; [KiemTraW]::GetWindowRect($nut, [ref]$r) | Out-Null
$goc = New-Object KiemTraW+POINT; [KiemTraW]::ClientToScreen($h, [ref]$goc) | Out-Null
$n = Dem-DiemToi $b ($r.L - $goc.X + 50) ($r.T - $goc.Y) ($r.R - $goc.X - 10) ($r.B - $goc.Y)
Can 'Thanh ben: chu muc dang chon toi' ($n -ge 20) "diem toi = $n"
$n = Dem-DiemToi $b 76 ($b.Height - 131) 238 ($b.Height - 109)
Can 'Thanh ben: ten nguoi dung toi' ($n -ge 15) "diem toi = $n"
$b.Dispose(); Stop-Process -Id $p.Id -Force

# Khach hang, Tong quan: banner chao giu nen toi.
$p = Mo-App $Exe; $h = $p.MainWindowHandle
Dang-Nhap $h 'khachhang01' '123456789'
Bam-Nut $h 100
$b = Chup-Client $h
$s = DoSang-TrungBinh $b 285 129 340 139
Can 'Banner chao khach nen toi' ($s -lt 80) "do sang TB = $([int]$s)"
$b.Dispose(); Stop-Process -Id $p.Id -Force

if ($loi) { "THAT BAI ($loi loi)"; exit 1 } else { 'DAT (0 loi)' }
