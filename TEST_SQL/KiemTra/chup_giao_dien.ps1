# Chup cac trang chinh cua app de so sanh giao dien truoc/sau.
#   powershell -ExecutionPolicy Bypass -File KiemTra\chup_giao_dien.ps1 -Ra KiemTra\truoc [-PhongTo]
# Dung tai khoan mau (mat khau 123456789). Moi tai khoan mo app mot lan; app duoc dong sau khi chup.
param([string]$Exe = (Join-Path $PSScriptRoot '..\QuanLyNhaHangGUI.exe'), [string]$Ra, [switch]$PhongTo)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'WinApi.ps1')
$Exe = (Resolve-Path $Exe).Path
New-Item -ItemType Directory -Force $Ra | Out-Null
Get-Process QuanLyNhaHangGUI -ErrorAction SilentlyContinue | Stop-Process -Force

# Man dang nhap
$p = Mo-App $Exe -PhongTo:$PhongTo
Chup-CuaSo $p.MainWindowHandle (Join-Path $Ra 'dangnhap.png')
Stop-Process -Id $p.Id -Force

# Trang theo vai tro: 100 Tong quan, 101 Khach hang, 102 Nhan vien, 103 Thuc don, 104 Ban hang,
# 105 Bao cao, 106 Tao tai khoan, 109 Thong tin cua toi, 111 Dat mon, 112 Don khach dat
$kichBan = [ordered]@{ 'QUANLY' = @(100, 101, 102, 103, 105, 106); 'nhanvien01' = @(104, 112); 'khachhang01' = @(100, 111, 109) }
$dem = 1
foreach ($tk in $kichBan.Keys) {
    $p = Mo-App $Exe -PhongTo:$PhongTo
    $h = $p.MainWindowHandle
    Dang-Nhap $h $tk '123456789'
    foreach ($trang in $kichBan[$tk]) {
        Bam-Nut $h $trang
        Chup-CuaSo $h (Join-Path $Ra "$($tk)_$trang.png")
        $dem++
    }
    Stop-Process -Id $p.Id -Force
}
"Da chup $dem anh vao $Ra"
