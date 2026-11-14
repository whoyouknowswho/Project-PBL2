# Ra soat cuoi: ghi du lieu that bai (chan ghi tren bang thu nghiem) -> bao loi than thien, ung dung khong sap, du lieu giu nguyen.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
$db = $null
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    $db = (Get-Content (Join-Path $hop 'DuLieu/database.id') -Raw).Trim()
    Khoi-Dong $hop | Out-Null
    $menuGoc = ((Doc-BangSql $hop ThucDon) -join ';')
    Dang-Nhap 'ql_test'
    Bam THUC_DON
    Invoke-KiemThuSql $db "CREATE TRIGGER dbo.KiemThuLoi ON dbo.ThucDon AFTER DELETE AS THROW 51000, 'Kiem thu loi ghi', 1;"
    foreach ($lan in 1..5) {
        Chon-Mon DANH_SACH_THUCDON 0
        Dat-Chu O_GIA '46000'
        Bam NUT_LUU
        $tb = Cho-HopThoai -Bam OK -Giay 6
        Kiem ($tb -like '*SQL Server*') "lan $lan : bao loi ghi than thien ($tb)"
        Kiem (-not (Ung-Dung).Process.HasExited) "lan $lan : ung dung van chay"
        if ((Ung-Dung).Process.HasExited) { break }
        Kiem ((So-Mon DANH_SACH_THUCDON) -eq 3) "lan $lan : luoi van du 3 mon"
    }
    Invoke-KiemThuSql $db 'DROP TRIGGER IF EXISTS dbo.KiemThuLoi'
    Kiem (((Doc-BangSql $hop ThucDon) -join ';') -eq $menuGoc) 'ThucDon giu nguyen'
    # Sau loi, chon mon va them vao gio van dung binh thuong.
    Dang-Xuat; Dang-Nhap 'nv_test'; Bam BAN_HANG
    Chon-Mon DANH_SACH_MON 0; Bam NUT_THEM_GIO
    Kiem ((So-Dong BANG_GIO) -eq 1) 'ban hang sau loi ghi: them mon binh thuong'
    Bam NUT_HUY_GIO; [void](Cho-HopThoai -Bam Yes)
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
} finally {
    if ($db) { Invoke-KiemThuSql $db 'DROP TRIGGER IF EXISTS dbo.KiemThuLoi' }
}
Ket-Thuc 'KT11_LoiGhi'
