# Task 7: Don khach dat / Don dat cua toi.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
function Tim-Don([string]$ma) { $rows = Doc-Bang BANG_CHINH; for ($i = 0; $i -lt $rows.Count; $i++) { if ($rows[$i][0] -eq $ma) { return $i } }; -1 }
function Gui-Don([int]$viTri) {
    Bam DAT_MON; Chon-Mon DANH_SACH_MON $viTri; Bam NUT_THEM_GIO; Bam NUT_GUI_DON; [void](Cho-HopThoai -Bam Yes)
    (Doc-Bang BANG_CHINH)[0][0]
}
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    Khoi-Dong $hop | Out-Null

    # 1. Khach gui hai don va huy mot don
    Dang-Nhap 'kh_test'
    $don1 = Gui-Don 0
    $don2 = Gui-Don 1
    Kiem ($don1 -ne $don2 -and $don2) "gui hai don: $don1, $don2 (moi nhat o dau)"
    Kiem (-not (Co-DieuKhien NUT_THANHTOAN_DON)) 'khach: khong co nut Thanh toan don'
    Kiem (-not (Dang-Bat NUT_HUY_DON)) 'chua chon: Huy vo hieu'
    $rows = Doc-Bang BANG_CHINH
    Kiem (@($rows | Where-Object { $_[2] -ne $rows[0][2] }).Count -eq 0) 'khach chi thay don cua minh'
    Chon-Dong BANG_CHINH (Tim-Don $don2)
    Kiem ((So-Dong BANG_GIO) -ge 1) 'chon don: hien chi tiet mon'
    Kiem (Dang-Bat NUT_HUY_DON) 'don cho: bat Huy'
    Bam NUT_HUY_DON
    [void](Cho-HopThoai -Bam Yes)
    $i = Tim-Don $don2
    Kiem ((Doc-Bang BANG_CHINH)[$i][3] -eq 'Đã hủy') 'huy don: trang thai Da huy'
    Chon-Dong BANG_CHINH $i
    Kiem (-not (Dang-Bat NUT_HUY_DON)) 'don da huy: Huy vo hieu'
    Chup 'dondat_kh' | Out-Null
    Dang-Xuat

    # 2. Nhan vien thanh toan don con cho
    Dang-Nhap 'nv_test'
    Bam DON_DAT
    Kiem (Co-DieuKhien NUT_THANHTOAN_DON) 'nhan vien: co nut Thanh toan don'
    $i = Tim-Don $don1
    Chon-Dong BANG_CHINH $i
    Kiem (Dang-Bat NUT_THANHTOAN_DON -and (Dang-Bat NUT_HUY_DON)) 'don cho: bat Thanh toan va Huy'
    Bam NUT_THANHTOAN_DON
    $tb = Cho-HopThoai -Bam Yes
    Kiem ($tb -like '*Xác nhận thanh toán đơn*') "thanh toan don: hoi xac nhan ($tb)"
    $hdWin = Cho-CuaSo 'NhaHangHoaDonMoi'
    Kiem ($hdWin -ne [IntPtr]::Zero) 'thanh toan don: mo hoa don'
    if ($hdWin -ne [IntPtr]::Zero) {
        [void][KT.W32]::PostMessageW($hdWin, 0x0111, [IntPtr]2, [IntPtr]::Zero)
        for ($k = 0; $k -lt 40 -and [KT.W32]::IsWindow($hdWin); $k++) { Start-Sleep -Milliseconds 100 }
    }
    Cho-Ranh
    $i = Tim-Don $don1
    $dong = (Doc-Bang BANG_CHINH)[$i]
    Kiem ($dong[3] -eq 'Đã thanh toán' -and $dong[5]) "don da thanh toan: trang thai + ma hoa don ($($dong -join '/'))"
    Kiem ([bool]((Doc-BangSql $hop DonDat) -match ('^"XONG" .*"' + [regex]::Escape($don1) + '\|'))) 'DonDat: XONG'
    Chon-Dong BANG_CHINH (Tim-Don $don2)
    Kiem (-not (Dang-Bat NUT_THANHTOAN_DON) -and -not (Dang-Bat NUT_HUY_DON)) 'don da huy: ca hai nut vo hieu'
    Bam NUT_LAMMOI_DON
    Kiem ((So-Dong BANG_CHINH) -ge 2) 'lam moi: van hien don'
    Chup 'dondat_nv' | Out-Null
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT7_DonDat'
