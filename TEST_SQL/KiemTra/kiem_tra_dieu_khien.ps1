# Kiem tra kieu nut phu (nen trang) va tieu de cot bang (nen nga THANH_BEN) bang lay mau diem anh.
#   powershell -ExecutionPolicy Bypass -File KiemTra\kiem_tra_dieu_khien.ps1
param([string]$Exe = (Join-Path $PSScriptRoot '..\QuanLyNhaHangGUI.exe'))
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'WinApi.ps1')
$Exe = (Resolve-Path $Exe).Path
Get-Process QuanLyNhaHangGUI -ErrorAction SilentlyContinue | Stop-Process -Force
$loi = 0
function Can([string]$ten, [bool]$dat, [string]$chiTiet) {
    "{0,-44} {1,-4} {2}" -f $ten, $(if ($dat) { 'ok' } else { 'SAI' }), $chiTiet
    if (-not $dat) { $script:loi++ }
}
function VungClient([IntPtr]$h, [IntPtr]$con) {
    $r = New-Object KiemTraW+RECT; [KiemTraW]::GetWindowRect($con, [ref]$r) | Out-Null
    $g = New-Object KiemTraW+POINT; [KiemTraW]::ClientToScreen($h, [ref]$g) | Out-Null
    return @(($r.L - $g.X), ($r.T - $g.Y), ($r.R - $g.X), ($r.B - $g.Y))
}
function MauTrungBinh($bmp, $x1, $y1, $x2, $y2) {
    $R = 0; $G = 0; $B = 0; $n = 0
    for ($y = $y1; $y -lt $y2; $y++) { for ($x = $x1; $x -lt $x2; $x++) { $c = $bmp.GetPixel($x, $y); $R += $c.R; $G += $c.G; $B += $c.B; $n++ } }
    return @([int]($R / $n), [int]($G / $n), [int]($B / $n))
}
function Gan($mau, $chuan, $saiSo) {
    return ([Math]::Abs($mau[0] - $chuan[0]) -le $saiSo) -and ([Math]::Abs($mau[1] - $chuan[1]) -le $saiSo) -and ([Math]::Abs($mau[2] - $chuan[2]) -le $saiSo)
}

$p = Mo-App $Exe; $h = $p.MainWindowHandle
Dang-Nhap $h 'QUANLY' '123456789'

# Nut phu "Them moi / Lam trong" (id 401) tren trang Thuc don: nen trang.
Bam-Nut $h 103
$b = Chup-Client $h
$v = VungClient $h ([KiemTraW]::GetDlgItem($h, 401))
$giua = [int](($v[1] + $v[3]) / 2)
$m = MauTrungBinh $b ($v[0] + 6) ($giua - 4) ($v[0] + 18) ($giua + 4)
Can 'Nut phu: nen trang' (Gan $m @(255, 255, 255) 3) "mau TB = $($m -join ',')"
$b.Dispose()

# Tieu de cot bang Khach hang (id 210): nen THANH_BEN (240,238,230).
Bam-Nut $h 101
$b = Chup-Client $h
$bang = [KiemTraW]::GetDlgItem($h, 210)
$tieuDe = [KiemTraW]::SendMessage($bang, 0x101F, [IntPtr]::Zero, [IntPtr]::Zero)  # LVM_GETHEADER
$v = VungClient $h $tieuDe
$m = MauTrungBinh $b ($v[2] - 14) ($v[1] + 4) ($v[2] - 4) ($v[1] + 10)
Can 'Tieu de cot: nen nga thanh ben' (Gan $m @(240, 238, 230) 3) "mau TB = $($m -join ',')"
$b.Dispose()

# Dong dang chon (chon dong dau bang phim mui ten): nen PHU_NEN (245,233,226) de thay ro ca o mep phai.
[KiemTraW]::SendMessage($bang, 0x0100, [IntPtr]0x28, [IntPtr]::Zero) | Out-Null  # WM_KEYDOWN, VK_DOWN
[KiemTraW]::SendMessage($bang, 0x0101, [IntPtr]0x28, [IntPtr]::Zero) | Out-Null  # WM_KEYUP
$b = Chup-Client $h
$vb = VungClient $h $bang
$y = $v[3] + 20   # giua dong dau (dong cao 40px, ngay duoi tieu de cot)
# Khoang trong giua cot "Gioi tinh" va "Diem" (cach mep trai bang ~385px, xa vach nhan va chu).
$m = MauTrungBinh $b ($vb[0] + 385) ($y - 3) ($vb[0] + 405) ($y + 3)
Can 'Dong dang chon: nen PHU_NEN ro rang' (Gan $m @(245, 233, 226) 3) "mau TB = $($m -join ',')"
$b.Dispose()
Stop-Process -Id $p.Id -Force

if ($loi) { "THAT BAI ($loi loi)"; exit 1 } else { 'DAT (0 loi)' }
