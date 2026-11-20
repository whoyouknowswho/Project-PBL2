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
  public struct RECT { public int L, T, R, B; }
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

function Chup-CuaSo([IntPtr]$h, [string]$Ra) {
    Start-Sleep -Milliseconds 800
    $r = New-Object KiemTraW+RECT; [KiemTraW]::GetWindowRect($h, [ref]$r) | Out-Null
    $bmp = New-Object System.Drawing.Bitmap ($r.R - $r.L), ($r.B - $r.T)
    $g = [System.Drawing.Graphics]::FromImage($bmp); $dc = $g.GetHdc()
    [KiemTraW]::PrintWindow($h, $dc, 2) | Out-Null; $g.ReleaseHdc($dc); $g.Dispose()
    $bmp.Save($Ra, [System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
}
