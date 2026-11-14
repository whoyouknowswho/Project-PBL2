# Task 6: Ban hang (nhan vien), Dat mon (khach), cua so hoa don.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
$LUOI = 'DANH_SACH_MON'
function Ton-Kho($hop, $ma) { foreach ($d in Doc-BangSql $hop ThucDon) { $c = $d -split '\|'; if ($c[1] -eq $ma) { return [int]$c[4] } }; -1 }
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    Khoi-Dong $hop | Out-Null
    Dang-Nhap 'nv_test'

    # 1. Thong tin hoa don ban dau
    Bam BAN_HANG
    $maHd = Doc-Chu O_MA
    Kiem ($maHd -like 'HD*') "ma hoa don tu dong ($maHd)"
    Kiem (Chi-Doc O_MA) 'ma hoa don chi doc'
    Kiem ((Doc-Chu O_NGAY) -eq (Get-Date -Format 'dd/MM/yyyy')) 'ngay lap = hom nay'
    Kiem ((Doc-Chu O_NHANVIEN) -eq 'NV01' -and (Chi-Doc O_NHANVIEN)) 'nhan vien NV01 chi doc'
    foreach ($id in 'NUT_THANHTOAN','NUT_XOA_GIO','NUT_HUY_GIO','NUT_THEM_GIO') { Kiem (-not (Dang-Bat $id)) "gio trong: $id vo hieu" }
    Chup 'banhang_trong' | Out-Null

    # 2. Them nuoc uong co size
    Bam NUT_NHOM_NUOC
    Chon-Mon $LUOI 1
    Kiem (Dang-Hien O_SIZE) 'chon nuoc: hien size'
    Kiem (Dang-Bat NUT_THEM_GIO) 'chon mon: bat nut them'
    Dat-Chu O_SOLUONG '3'; Bam NUT_TANG_SL
    Kiem ((Doc-Chu O_SOLUONG) -eq '4') "nut + : so luong 4 ($(Doc-Chu O_SOLUONG))"
    Chon-Hop O_SIZE 2
    Bam NUT_THEM_GIO
    $gio = Doc-Bang BANG_GIO
    Kiem ($gio.Count -eq 1 -and $gio[0][0] -eq 'Nuoc cam (L)' -and $gio[0][1] -eq '4' -and $gio[0][2] -eq '120.000 đ') "gio: Nuoc cam (L) x4 = 120.000 ($($gio | ForEach-Object { $_ -join '/' }))"

    # 3. Kiem tra ton kho va gop dong
    Dat-Chu O_SOLUONG '155'; Bam NUT_THEM_GIO
    $tb = Cho-HopThoai -Bam OK -Giay 4
    Kiem ($tb -like '*không vượt tồn kho*') "vuot ton kho: bao loi ($tb)"
    Dat-Chu O_SOLUONG '1'; Bam NUT_THEM_GIO
    $gio = Doc-Bang BANG_GIO
    Kiem ($gio.Count -eq 1 -and $gio[0][1] -eq '5') 'cung mon cung size: gop dong (SL 5)'

    # 4. Khach co uu dai
    $viTri = -1
    for ($i = 0; $i -lt (So-MucHop O_KHACH); $i++) { if ((Chu-MucHop O_KHACH $i) -like 'KH0001 *') { $viTri = $i } }
    Chon-Hop O_KHACH $viTri
    $tong = Doc-Chu O_TONG
    Kiem ($tong -match 'Giảm: ' -and $tong -notmatch 'Giảm: 0 đ') "khach hang Bac: co giam gia ($tong)"
    Chup 'banhang_gio' | Out-Null

    # 5. Chuyen trang roi quay lai giu nguyen
    Bam THUC_DON; Bam BAN_HANG
    Kiem ((Doc-Chu O_MA) -eq $maHd) 'quay lai: giu ma hoa don'
    Kiem ((Muc-Hop O_KHACH) -eq $viTri) 'quay lai: giu khach hang'
    Kiem ((So-Dong BANG_GIO) -eq 1) 'quay lai: giu gio hang'

    # 6. Thanh toan
    $tonTruoc = Ton-Kho $hop 'NU02'
    Bam NUT_THANHTOAN
    $tb = Cho-HopThoai -Bam No
    Kiem ($tb -like '*Xác nhận thanh toán*') "thanh toan: hoi xac nhan ($tb)"
    Kiem ((So-Dong BANG_GIO) -eq 1) 'thanh toan No: gio con'
    Bam NUT_THANHTOAN
    [void](Cho-HopThoai -Bam Yes)
    $hdWin = Cho-CuaSo 'NhaHangHoaDonMoi'
    Kiem ($hdWin -ne [IntPtr]::Zero) 'thanh toan: mo cua so hoa don'
    if ($hdWin -ne [IntPtr]::Zero) {
        Chup 'hoadon' $hdWin | Out-Null
        [void][KT.W32]::PostMessageW($hdWin, 0x0111, [IntPtr]2, [IntPtr]::Zero)
        for ($i = 0; $i -lt 40 -and [KT.W32]::IsWindow($hdWin); $i++) { Start-Sleep -Milliseconds 100 }
        Kiem (-not [KT.W32]::IsWindow($hdWin)) 'Esc/Huy: dong cua so hoa don'
    }
    Cho-Ranh
    Kiem ([bool]((Doc-BangSql $hop HoaDon) -match [regex]::Escape($maHd))) 'luu hoa don vao HoaDon'
    Kiem ((Ton-Kho $hop 'NU02') -eq $tonTruoc - 5) "tru ton kho NU02: $tonTruoc -> $(Ton-Kho $hop 'NU02')"
    Kiem ((Doc-Chu O_MA) -ne $maHd) 'sinh ma hoa don moi'
    Kiem ((So-Dong BANG_GIO) -eq 0) 'gio trong sau thanh toan'

    # 7. Dong cua so khi gio con mon
    Bam NUT_NHOM_THUCAN; Chon-Mon $LUOI 0; Dat-Chu O_SOLUONG '1'; Bam NUT_THEM_GIO
    [void][KT.W32]::PostMessageW((Ung-Dung).Hwnd, 0x0010, [IntPtr]::Zero, [IntPtr]::Zero)
    $tb = Cho-HopThoai -Bam No
    Kiem ($tb -like '*Giỏ hàng chưa thanh toán sẽ bị hủy*') "dong cua so: canh bao gio ($tb)"
    Kiem ([KT.W32]::IsWindow((Ung-Dung).Hwnd)) 'dong -> No: van mo'
    Bam NUT_HUY_GIO
    [void](Cho-HopThoai -Bam Yes)
    Kiem ((So-Dong BANG_GIO) -eq 0) 'huy gio: gio trong'
    Dang-Xuat

    # 8. Khach dat mon
    $soDon = @(Doc-BangSql $hop DonDat | Where-Object { $_.Trim() }).Count
    Dang-Nhap 'kh_test'
    Bam DAT_MON
    Kiem (-not (Co-DieuKhien O_MA)) 'dat mon: khong co thong tin hoa don'
    Kiem ((So-Mon $LUOI) -eq 3) 'dat mon: 3 mon an'
    Kiem (-not (Dang-Bat NUT_GUI_DON)) 'gio trong: vo hieu Gui don'
    Chon-Mon $LUOI 0
    Gui-LenhCon $LUOI 2   # LBN_DBLCLK
    Kiem ((So-Dong BANG_GIO) -eq 1) 'nhap dup: them mon vao gio'
    Chup 'datmon' | Out-Null
    Bam NUT_GUI_DON
    $tb = Cho-HopThoai -Bam Yes
    Kiem ($tb -like '*tạm tính*') "gui don: hoi xac nhan ($tb)"
    Kiem (@(Doc-BangSql $hop DonDat | Where-Object { $_.Trim() }).Count -gt $soDon) 'gui don: luu DonDat'
    Kiem ((So-Dong BANG_GIO) -eq 0 -and -not (Co-DieuKhien NUT_GUI_DON)) 'gui don: chuyen sang trang Don dat'
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT6_BanHang'
