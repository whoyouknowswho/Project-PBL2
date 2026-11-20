# Ghep anh truoc/sau cung ten thanh cac bang so sanh (moi bang 3 cap, trai = truoc, phai = sau).
#   powershell -ExecutionPolicy Bypass -File KiemTra\ghep_truoc_sau.ps1 -Truoc KiemTra\truoc -Sau KiemTra\sau -Ra KiemTra\so_sanh
param([string]$Truoc, [string]$Sau, [string]$Ra, [int]$Rong = 760)
Add-Type -AssemblyName System.Drawing
New-Item -ItemType Directory -Force $Ra | Out-Null
$ten = Get-ChildItem $Truoc -Filter *.png | Sort-Object Name | ForEach-Object Name
$font = New-Object System.Drawing.Font 'Segoe UI', 11, ([System.Drawing.FontStyle]::Bold)
$soBang = 0
for ($i = 0; $i -lt $ten.Count; $i += 3) {
    $nhom = @($ten[$i..([Math]::Min($i + 2, $ten.Count - 1))])
    $mau = [System.Drawing.Image]::FromFile((Resolve-Path (Join-Path $Truoc $nhom[0])))
    $cao = [int]($Rong * $mau.Height / $mau.Width); $mau.Dispose()
    $bmp = New-Object System.Drawing.Bitmap ($Rong * 2 + 10), (($cao + 26) * $nhom.Count)
    $g = [System.Drawing.Graphics]::FromImage($bmp); $g.Clear([System.Drawing.Color]::Black); $g.InterpolationMode = 'HighQualityBicubic'
    for ($k = 0; $k -lt $nhom.Count; $k++) {
        $y = $k * ($cao + 26)
        $g.DrawString($nhom[$k], $font, [System.Drawing.Brushes]::White, 4, $y + 2)
        foreach ($cot in 0, 1) {
            $tep = Join-Path $(if ($cot) { $Sau } else { $Truoc }) $nhom[$k]
            if (-not (Test-Path $tep)) { continue }
            $a = [System.Drawing.Image]::FromFile((Resolve-Path $tep))
            $g.DrawImage($a, $cot * ($Rong + 10), $y + 26, $Rong, $cao); $a.Dispose()
        }
    }
    $soBang++
    $bmp.Save((Join-Path $Ra "so_sanh_$soBang.png")); $g.Dispose(); $bmp.Dispose()
}
"Da tao $soBang bang so sanh"
