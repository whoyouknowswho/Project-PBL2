# Task 9: Tong quan cua 3 vai tro va bieu do doanh thu.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    # Hai mon ton kho thap: MA03 con 3 (sap het), NU01 het hang.
    $tepMenu = Join-Path $hop 'DuLieu\menu.txt'
    $menu = Get-Content $tepMenu -Encoding UTF8 | ForEach-Object {
        $c = $_ -split '\|'
        if ($c[1] -eq 'MA03') { $c[4] = '3' }
        if ($c[1] -eq 'NU01') { $c[4] = '0' }
        $c -join '|'
    }
    [IO.File]::WriteAllLines($tepMenu, [string[]]$menu, (New-Object Text.UTF8Encoding $false))
    $namCo = @(Get-Content (Join-Path $hop 'DuLieu\hoadon.txt') -Encoding UTF8 | ForEach-Object {
        $n = ($_ -split '\|')[1]; if ($n -match '^\d\d/\d\d/(\d{4})$') { [int]$Matches[1] } })
    $namLonNhat = (@($namCo) + (Get-Date).Year | Measure-Object -Maximum).Maximum
    Khoi-Dong $hop | Out-Null

    # 1. Quan ly: bieu do doanh thu
    Dang-Nhap 'ql_test'
    Kiem (Co-DieuKhien LOC_NAM_DOANHTHU) 'quan ly: co chon nam doanh thu'
    Kiem ((Chu-MucHop LOC_NAM_DOANHTHU 0) -eq "$namLonNhat") "nam dau danh sach = $namLonNhat ($(Chu-MucHop LOC_NAM_DOANHTHU 0))"
    Kiem (-not (Co-DieuKhien BANG_CHINH)) 'quan ly: khong co bang ton kho'
    $c = Hinh-Con LOC_NAM_DOANHTHU
    for ($x = 300; $x -lt $c.R; $x += 25) {
        [void][KT.W32]::PostMessageW((Ung-Dung).Hwnd, 0x0200, [IntPtr]::Zero, [IntPtr]((($c.B + 220) -shl 16) -bor $x))
    }
    Cho-Ranh
    Kiem ($null -eq (Cho-HopThoai -Bam OK -Giay 1)) 're chuot tren bieu do: khong loi'
    Chon-Hop LOC_NAM_DOANHTHU 0
    Kiem ([KT.W32]::IsWindow((Ung-Dung).Hwnd)) 'doi nam: ung dung van chay'
    Chup 'tongquan_ql' | Out-Null
    Dang-Xuat

    # 2. Nhan vien: bang ton kho thap
    Dang-Nhap 'nv_test'
    $rows = Doc-Bang BANG_CHINH
    Kiem ($rows.Count -eq 2) "nhan vien: 2 mon ton thap ($($rows.Count))"
    $tt = @($rows | ForEach-Object { $_[4] })
    Kiem (($tt -contains 'Hết hàng') -and ($tt -contains 'Sắp hết')) "tinh trang: Het hang + Sap het ($($tt -join ','))"
    Kiem (Co-DieuKhien BAN_HANG) 'nhan vien: co nut Tao don ban hang'
    Chup 'tongquan_nv' | Out-Null
    Dang-Xuat

    # 3. Khach hang
    Dang-Nhap 'kh_test'
    foreach ($id in 'DAT_MON','HO_SO_CUA_TOI','HOA_DON_CUA_TOI','BANG_CHINH') { Kiem (Co-DieuKhien $id) "khach: co $id" }
    Chup 'tongquan_kh' | Out-Null
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT9_TongQuan'
