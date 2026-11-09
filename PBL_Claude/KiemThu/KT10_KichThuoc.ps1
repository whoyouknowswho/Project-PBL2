# Task 10: bo cuc o kich thuoc nho nhat va khi phong to; ro ri GDI khi chuyen trang.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'

function Giao($a, $b) { $a.L -lt $b.R -and $b.L -lt $a.R -and $a.T -lt $b.B -and $b.T -lt $a.B }
function Kiem-BoCuc([string]$ten) {
    $kh = New-Object KT.RECT; [void][KT.W32]::GetClientRect((Ung-Dung).Hwnd, [ref]$kh)
    $ds = @(Cac-ConHien | ForEach-Object {
        $r = New-Object KT.RECT; [void][KT.W32]::GetWindowRect($_, [ref]$r)
        [void][KT.W32]::MapWindowPoints([IntPtr]::Zero, (Ung-Dung).Hwnd, [ref]$r, 2)
        [pscustomobject]@{ Id = [KT.W32]::GetDlgCtrlID($_); Lop = [KT.W32]::ClassOf($_); R = $r }
    })
    $tran = @($ds | Where-Object { $_.R.L -lt 0 -or $_.R.T -lt 0 -or $_.R.R -gt $kh.R -or $_.R.B -gt $kh.B })
    Kiem ($tran.Count -eq 0) "$ten : dieu khien tran ra ngoai ($(($tran | ForEach-Object { $_.Id }) -join ','))"
    # O nhap/nut/hop chon khong duoc bi ep qua hep; o danh dau phai doc duoc chu.
    $hep = @($ds | Where-Object {
        $w = $_.R.R - $_.R.L
        ($_.Lop -eq 'Edit' -and $w -lt 40) -or ($_.Lop -eq 'Button' -and $w -lt 36) -or
        ($_.Lop -eq 'ComboBox' -and $w -lt 60) -or ($_.Id -eq 203 -and $w -lt 180) })
    Kiem ($hep.Count -eq 0) "$ten : dieu khien qua hep ($(($hep | ForEach-Object { "$($_.Id)=$($_.R.R - $_.R.L)" }) -join ','))"
    # Luoi mon phai hien tron it nhat mot the (the co the thu gon nhung >= 150 px).
    foreach ($luoi in 214, 216) {
        $h = [KT.W32]::GetDlgItem((Ung-Dung).Hwnd, $luoi)
        if ($h -eq [IntPtr]::Zero -or -not [KT.W32]::IsWindowVisible($h)) { continue }
        $r = New-Object KT.RECT; [void][KT.W32]::GetClientRect($h, [ref]$r)
        $caoThe = [int][KT.W32]::SendMessageW($h, 0x8000 + 104, [IntPtr]::Zero, [IntPtr]::Zero)
        Kiem ($caoThe -ge 150 -and $caoThe -le $r.B - 8) "$ten : luoi $luoi cao $($r.B) hien tron the cao $caoThe"
    }
    $chong = @()
    for ($i = 0; $i -lt $ds.Count; $i++) { for ($j = $i + 1; $j -lt $ds.Count; $j++) {
        if (Giao $ds[$i].R $ds[$j].R) { $chong += "$($ds[$i].Id)/$($ds[$j].Id)" } } }
    Kiem ($chong.Count -eq 0) "$ten : dieu khien chong nhau ($($chong -join ' '))"
}
function Kiem-HaiCo([string]$ten) {
    Doi-KichThuoc 1120 760; Kiem-BoCuc "$ten 1120x760"; Chup "$ten-nho" | Out-Null
    Phong-To; Kiem-BoCuc "$ten phong to"; Chup "$ten-to" | Out-Null
    [void][KT.W32]::ShowWindow((Ung-Dung).Hwnd, 9); Start-Sleep -Milliseconds 400; Cho-Ranh
}

try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    Khoi-Dong $hop | Out-Null
    Kiem-HaiCo 'dangnhap'
    Bam NUT_MO_DANGKY; Kiem-HaiCo 'dangky'; Bam NUT_VE_DANGNHAP

    Dang-Nhap 'ql_test'
    foreach ($trang in 'TRANG_CHU','KHACH_HANG','NHAN_VIEN','THUC_DON','BAO_CAO','TAI_KHOAN','MAT_KHAU') {
        Bam $trang; Kiem-HaiCo "ql-$trang"
    }
    # Ro ri GDI: chuyen trang lien tuc.
    Bam TRANG_CHU
    $truoc = So-GDI
    foreach ($vong in 1..10) { foreach ($trang in 'TRANG_CHU','KHACH_HANG','NHAN_VIEN','THUC_DON','BAO_CAO','TAI_KHOAN') { Bam $trang } }
    Bam TRANG_CHU
    $sau = So-GDI
    Kiem (($sau - $truoc) -le 40) "GDI sau 60 lan chuyen trang: $truoc -> $sau"
    Dang-Xuat

    Dang-Nhap 'nv_test'
    Bam BAN_HANG; Bam NUT_NHOM_NUOC; Chon-Mon DANH_SACH_MON 0; Bam NUT_THEM_GIO
    Kiem-HaiCo 'nv-BAN_HANG'
    Bam NUT_HUY_GIO; [void](Cho-HopThoai -Bam Yes)
    foreach ($trang in 'TRANG_CHU','DON_DAT') { Bam $trang; Kiem-HaiCo "nv-$trang" }
    Dang-Xuat

    Dang-Nhap 'kh_test'
    foreach ($trang in 'TRANG_CHU','THUC_DON','DAT_MON','DON_DAT','HO_SO_CUA_TOI','HOA_DON_CUA_TOI') { Bam $trang; Kiem-HaiCo "kh-$trang" }
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT10_KichThuoc'
