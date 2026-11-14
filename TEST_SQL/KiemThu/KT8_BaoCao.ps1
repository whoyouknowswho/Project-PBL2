# Task 8: Bao cao kinh doanh va Hoa don cua toi.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
function Dong-HoaDon {
    $h = Cho-CuaSo 'NhaHangHoaDonMoi'
    if ($h -eq [IntPtr]::Zero) { return $false }
    [void][KT.W32]::PostMessageW($h, 0x0111, [IntPtr]2, [IntPtr]::Zero)
    for ($k = 0; $k -lt 40 -and [KT.W32]::IsWindow($h); $k++) { Start-Sleep -Milliseconds 100 }
    Cho-Ranh
    $true
}
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    $soHd = @(Doc-BangSql $hop HoaDon | Where-Object { $_.Trim() }).Count
    $soTonThap = @(Doc-BangSql $hop ThucDon | Where-Object { $_.Trim() -and [int](($_ -split '\|')[4]) -le 5 }).Count
    Khoi-Dong $hop | Out-Null
    Dang-Nhap 'ql_test'

    # 1. Loc nhanh
    Bam BAO_CAO
    Kiem (-not (Dang-Bat NUT_HOADON)) 'chua chon: Xem hoa don vo hieu'
    Bam NUT_BC_HOMNAY
    Kiem ((Doc-Chu LOC_BAO_CAO) -eq (Get-Date -Format 'dd/MM/yyyy')) 'Hom nay: dien ngay'
    Kiem ((Doc-Chu NHAN_KY_BAOCAO) -like '*Ngày*') "Hom nay: dang xem Ngay ($(Doc-Chu NHAN_KY_BAOCAO))"
    Bam NUT_BC_TATCA
    Kiem ((Doc-Chu LOC_BAO_CAO) -eq '') 'Tat ca: o loc trong'
    Kiem ((So-Dong BANG_CHINH) -eq [Math]::Max(1, $soHd)) "Tat ca: $soHd hoa don (co $(So-Dong BANG_CHINH))"
    $tonExp = [Math]::Max(1, $soTonThap)
    Kiem ((So-Dong BANG_TON) -eq $tonExp) "ton kho thap: $tonExp dong (co $(So-Dong BANG_TON))"
    Chup 'baocao' | Out-Null

    # 2. Bo loc sai giu ket qua cu
    Dat-Chu LOC_BAO_CAO '99/99'; Bam NUT_BAOCAO
    $tb = Cho-HopThoai -Bam OK -Giay 4
    Kiem ($null -ne $tb) "loc sai: bao loi ($tb)"
    Kiem ((So-Dong BANG_CHINH) -eq [Math]::Max(1, $soHd)) 'loc sai: giu ket qua cu'

    # 3. Xem hoa don bang nut va nhap dup
    Chon-Dong BANG_CHINH 0
    Kiem (Dang-Bat NUT_HOADON) 'chon dong: bat Xem hoa don'
    Bam NUT_HOADON
    Kiem (Dong-HoaDon) 'nut Xem hoa don: mo cua so'
    NhapDup-Dong BANG_CHINH 0
    Kiem (Dong-HoaDon) 'nhap dup dong: mo cua so'

    # 4. Enter trong o loc = Ap dung
    Dat-Chu LOC_BAO_CAO (Get-Date -Format 'MM/yyyy')
    Nhan-Enter LOC_BAO_CAO
    Kiem ((Doc-Chu NHAN_KY_BAOCAO) -like '*Tháng*') "Enter: ap dung loc thang ($(Doc-Chu NHAN_KY_BAOCAO))"
    Dang-Xuat

    # 5. Khach: hoa don cua toi
    Dang-Nhap 'kh_test'
    Bam HOA_DON_CUA_TOI
    Kiem (Co-DieuKhien BANG_CHINH) 'hoa don cua toi: co bang'
    Kiem ((So-Dong BANG_CHINH) -eq 0 -and -not (Dang-Bat NUT_HOADON)) 'khach moi: chua co hoa don, nut vo hieu'
    Chup 'hoadon_kh' | Out-Null
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT8_BaoCao'
