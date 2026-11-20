# Ham Win32 dung chung cho cac script kiem tra giao dien (chup man hinh, do toc do ve).
Add-Type -AssemblyName System.Drawing
if (-not ('KiemTraW' -as [type])) {
Add-Type @"
using System; using System.Runtime.InteropServices;
public static class KiemTraW {
  [DllImport("user32.dll")] public static extern IntPtr GetDlgItem(IntPtr h, int id);
  [DllImport("user32.dll", CharSet=CharSet.Unicode)] public static extern IntPtr SendMessage(IntPtr h, int m, IntPtr w, string l);
  [DllImport("user32.dll")] public static extern IntPtr SendMessage(IntPtr h, int m, IntPtr w, IntPtr l);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint f);
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int c);
  [DllImport("user32.dll")] public static extern bool SetWindowPos(IntPtr h, IntPtr a, int x, int y, int cx, int cy, uint f);
  [DllImport("user32.dll")] public static extern bool RedrawWindow(IntPtr h, IntPtr r, IntPtr rgn, uint f);
  [DllImport("user32.dll")] public static extern bool GetClientRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
  public struct RECT { public int L, T, R, B; }
  public struct POINT { public int X, Y; }
}
"@
}

function Mo-App([string]$Exe, [switch]$PhongTo) {
    $p = Start-Process $Exe -WorkingDirectory (Split-Path $Exe) -PassThru
    for ($i = 0; $i -lt 40 -and $p.MainWindowHandle -eq 0; $i++) { Start-Sleep -Milliseconds 250; $p.Refresh() }
    if ($PhongTo) { [KiemTraW]::ShowWindow($p.MainWindowHandle, 3) | Out-Null; Start-Sleep -Milliseconds 500 }
    return $p
}

function Bam-Nut([IntPtr]$h, [int]$id) {
    [KiemTraW]::SendMessage($h, 0x0111, [IntPtr]$id, [KiemTraW]::GetDlgItem($h, $id)) | Out-Null
    Start-Sleep -Milliseconds 700
}

function Dang-Nhap([IntPtr]$h, [string]$Tk, [string]$Mk) {
    [KiemTraW]::SendMessage([KiemTraW]::GetDlgItem($h, 350), 0x000C, [IntPtr]::Zero, $Tk) | Out-Null
    [KiemTraW]::SendMessage([KiemTraW]::GetDlgItem($h, 351), 0x000C, [IntPtr]::Zero, $Mk) | Out-Null
    Bam-Nut $h 410
}

# Chup vung client cua cua so thanh Bitmap (toa do anh = toa do client).
function Chup-Client([IntPtr]$h) {
    Start-Sleep -Milliseconds 800
    $w = New-Object KiemTraW+RECT; [KiemTraW]::GetWindowRect($h, [ref]$w) | Out-Null
    $c = New-Object KiemTraW+RECT; [KiemTraW]::GetClientRect($h, [ref]$c) | Out-Null
    $p = New-Object KiemTraW+POINT; [KiemTraW]::ClientToScreen($h, [ref]$p) | Out-Null
    $toan = New-Object System.Drawing.Bitmap ($w.R - $w.L), ($w.B - $w.T)
    $g = [System.Drawing.Graphics]::FromImage($toan); $dc = $g.GetHdc()
    [KiemTraW]::PrintWindow($h, $dc, 2) | Out-Null; $g.ReleaseHdc($dc); $g.Dispose()
    $vung = New-Object System.Drawing.Rectangle ($p.X - $w.L), ($p.Y - $w.T), $c.R, $c.B
    $kq = $toan.Clone($vung, $toan.PixelFormat); $toan.Dispose()
    return $kq
}

# Do sang 0-255 theo cong thuc Rec. 709.
function Do-Sang($mau) { return 0.2126 * $mau.R + 0.7152 * $mau.G + 0.0722 * $mau.B }

# Dem diem anh toi (do sang < $nguong) trong vung client [x1,y1,x2,y2).
function Dem-DiemToi($bmp, [int]$x1, [int]$y1, [int]$x2, [int]$y2, [double]$nguong = 90) {
    $n = 0
    for ($y = $y1; $y -lt $y2; $y++) { for ($x = $x1; $x -lt $x2; $x++) { if ((Do-Sang $bmp.GetPixel($x, $y)) -lt $nguong) { $n++ } } }
    return $n
}

function DoSang-TrungBinh($bmp, [int]$x1, [int]$y1, [int]$x2, [int]$y2) {
    $tong = 0.0; $n = 0
    for ($y = $y1; $y -lt $y2; $y++) { for ($x = $x1; $x -lt $x2; $x++) { $tong += Do-Sang $bmp.GetPixel($x, $y); $n++ } }
    return $tong / [Math]::Max(1, $n)
}

function Chup-CuaSo([IntPtr]$h, [string]$Ra) {
    Start-Sleep -Milliseconds 800
    $r = New-Object KiemTraW+RECT; [KiemTraW]::GetWindowRect($h, [ref]$r) | Out-Null
    $bmp = New-Object System.Drawing.Bitmap ($r.R - $r.L), ($r.B - $r.T)
    $g = [System.Drawing.Graphics]::FromImage($bmp); $dc = $g.GetHdc()
    [KiemTraW]::PrintWindow($h, $dc, 2) | Out-Null; $g.ReleaseHdc($dc); $g.Dispose()
    $bmp.Save($Ra, [System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
}
