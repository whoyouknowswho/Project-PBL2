# Do thoi gian ve luoi mon khi cuon (trang Thuc don cua QUANLY).
#   powershell -ExecutionPolicy Bypass -File KiemTra\do_cuon_luoi.ps1 [-SoLan 60]
param([string]$Exe = (Join-Path $PSScriptRoot '..\QuanLyNhaHangGUI.exe'), [int]$SoLan = 60)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'WinApi.ps1')
$Exe = (Resolve-Path $Exe).Path
Get-Process QuanLyNhaHangGUI -ErrorAction SilentlyContinue | Stop-Process -Force
$p = Mo-App $Exe
$h = $p.MainWindowHandle
Dang-Nhap $h 'QUANLY' '123456789'
Bam-Nut $h 103
$luoi = [KiemTraW]::GetDlgItem($h, 216)
if ($luoi -eq [IntPtr]::Zero) { Stop-Process -Id $p.Id -Force; throw 'Khong tim thay luoi mon (id 216)' }
# Ve mot lan de nap anh vao bo nho truoc khi do.
[KiemTraW]::RedrawWindow($luoi, [IntPtr]::Zero, [IntPtr]::Zero, 0x0101) | Out-Null
$dongHo = [Diagnostics.Stopwatch]::StartNew()
for ($i = 0; $i -lt $SoLan; $i++) {
    [KiemTraW]::SendMessage($luoi, 0x0115, [IntPtr]1, [IntPtr]::Zero) | Out-Null   # WM_VSCROLL, SB_LINEDOWN
    [KiemTraW]::RedrawWindow($luoi, [IntPtr]::Zero, [IntPtr]::Zero, 0x0101) | Out-Null  # RDW_INVALIDATE | RDW_UPDATENOW
}
$dongHo.Stop()
Stop-Process -Id $p.Id -Force
"$SoLan lan cuon: $($dongHo.ElapsedMilliseconds) ms"
