# Task 5: luoi the mon va trang Thuc don.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
$LUOI = 'DANH_SACH_THUCDON'
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    $tepMenu = Join-Path $hop 'DuLieu\menu.txt'
    # Anh rieng hong: phai dung anh minh hoa, khong loi.
    New-Item -ItemType Directory -Force (Join-Path $hop 'DuLieu\AnhMon') | Out-Null
    [IO.File]::WriteAllBytes((Join-Path $hop 'DuLieu\AnhMon\MA01.png'), [byte[]](1..100))
    Khoi-Dong $hop | Out-Null
    Dang-Nhap 'ql_test'

    # 1. Nhom va so dem
    Bam THUC_DON
    Kiem ($null -eq (Cho-HopThoai -Bam OK -Giay 1)) 'mo thuc don: khong loi du anh hong'
    Kiem ((Doc-Chu NUT_NHOM_THUCAN) -eq 'Thức ăn (3)') "nut nhom thuc an ($(Doc-Chu NUT_NHOM_THUCAN))"
    Kiem ((Doc-Chu NUT_NHOM_NUOC) -eq 'Nước uống (2)') "nut nhom nuoc ($(Doc-Chu NUT_NHOM_NUOC))"
    Kiem ((So-Mon $LUOI) -eq 3) "luoi thuc an: 3 the ($(So-Mon $LUOI))"
    Chup 'thucdon_ql' | Out-Null

    # 2. Sap xep gia giam, chon the nap bieu mau
    Chon-Hop SAP_XEP 3
    Chon-Mon $LUOI 0
    Kiem ((Doc-Chu O_MA) -eq 'MA01') "gia giam: the dau la MA01 ($(Doc-Chu O_MA))"
    Kiem (Chi-Doc O_MA) 'chon mon: ma chi doc'
    Kiem (Dang-Bat NUT_NHAPKHO) 'chon mon: bat Nhap kho'
    Kiem (-not (Dang-Hien O_SIZE)) 'mon an: an Size'

    # 3. Nhom nuoc uong
    Bam NUT_NHOM_NUOC
    Kiem ((So-Mon $LUOI) -eq 2) 'luoi nuoc: 2 the'
    Chon-Mon $LUOI 0
    Kiem (-not (Dang-Bat O_CAY)) 'nuoc uong: vo hieu Do cay'
    Kiem (Dang-Hien O_SIZE) 'nuoc uong: hien Size'

    # 4. Them mon, loc con hang, nhap kho
    Bam NUT_MOI
    Kiem ((Doc-Chu O_MA) -eq '' -and -not (Dang-Bat NUT_NHAPKHO)) 'Them moi: trong va vo hieu Nhap kho'
    Chon-Hop O_LOAI 0
    Dat-Chu O_MA 'MA09'; Dat-Chu O_TEN 'Phở bò'; Dat-Chu O_GIA '50000'; Dat-Chu O_TON '0'; Dat-Chu O_CAY '0'
    Bam NUT_LUU
    $tb = Cho-HopThoai -Bam OK -Giay 3
    Kiem ($null -eq $tb) "luu mon moi: khong loi ($tb)"
    Kiem ([bool]((Get-Content $tepMenu -Encoding UTF8) -match '^MONAN\|MA09\|Phở bò\|')) 'luu mon moi: ghi menu.txt'
    Bam NUT_NHOM_THUCAN
    Kiem ((So-Mon $LUOI) -eq 4) 'thuc an sau khi them: 4 the'
    Bam CON_HANG
    Kiem ((So-Mon $LUOI) -eq 3) 'chi hien con hang: an MA09'
    Bam CON_HANG
    Dat-Chu O_NHAPKHO '5'; Bam NUT_NHAPKHO
    Kiem ([bool]((Get-Content $tepMenu -Encoding UTF8) -match '^MONAN\|MA09\|[^|]*\|[^|]*\|5\|')) 'nhap kho: ton MA09 = 5'

    # 5. Xoa mon
    Chon-Hop SAP_XEP 0
    Dat-Chu TIM_KIEM 'MA09'
    Chon-Mon $LUOI 0
    Kiem ((Doc-Chu O_MA) -eq 'MA09') 'tim MA09 va chon'
    Bam NUT_XOA
    [void](Cho-HopThoai -Bam Yes)
    Kiem (-not ((Get-Content $tepMenu -Encoding UTF8) -match '^MONAN\|MA09\|')) 'xoa MA09 khoi menu.txt'
    Dang-Xuat

    # 6. Khach chi xem
    Dang-Nhap 'kh_test'
    Bam THUC_DON
    Kiem (-not (Co-DieuKhien O_MA) -and -not (Co-DieuKhien NUT_LUU)) 'khach: khong co bieu mau'
    Kiem ((So-Mon $LUOI) -eq 3) 'khach: thay 3 mon an'
    Chup 'thucdon_kh' | Out-Null
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
}
Ket-Thuc 'KT5_ThucDon'
