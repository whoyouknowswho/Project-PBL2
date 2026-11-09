# Ra soat cuoi: ghi du lieu that bai (menu.txt chi doc) -> bao loi than thien, ung dung khong sap, du lieu giu nguyen.
Import-Module (Join-Path $PSScriptRoot 'GiaoDien.psm1') -Force -WarningAction SilentlyContinue
$ErrorActionPreference = 'Stop'
$mau = Join-Path $env:TEMP 'pbl_kiemthu\mau\DuLieu'
$tepMenu = $null
try {
    $hop = Tao-HopCat -NguonDuLieu $mau
    $tepMenu = Join-Path $hop 'DuLieu\menu.txt'
    $menuGoc = Get-Content $tepMenu -Raw
    Khoi-Dong $hop | Out-Null
    Dang-Nhap 'ql_test'
    Bam THUC_DON
    Set-ItemProperty $tepMenu -Name IsReadOnly -Value $true
    foreach ($lan in 1..5) {
        Chon-Mon DANH_SACH_THUCDON 0
        Dat-Chu O_GIA '46000'
        Bam NUT_LUU
        $tb = Cho-HopThoai -Bam OK -Giay 6
        Kiem ($tb -like 'Không *') "lan $lan : bao loi ghi than thien ($tb)"
        Kiem (-not (Ung-Dung).Process.HasExited) "lan $lan : ung dung van chay"
        if ((Ung-Dung).Process.HasExited) { break }
        Kiem ((So-Mon DANH_SACH_THUCDON) -eq 3) "lan $lan : luoi van du 3 mon"
    }
    Set-ItemProperty $tepMenu -Name IsReadOnly -Value $false
    Kiem ((Get-Content $tepMenu -Raw) -eq $menuGoc) 'menu.txt giu nguyen'
    # Sau loi, chon mon va them vao gio van dung binh thuong.
    Dang-Xuat; Dang-Nhap 'nv_test'; Bam BAN_HANG
    Chon-Mon DANH_SACH_MON 0; Bam NUT_THEM_GIO
    Kiem ((So-Dong BANG_GIO) -eq 1) 'ban hang sau loi ghi: them mon binh thuong'
    Bam NUT_HUY_GIO; [void](Cho-HopThoai -Bam Yes)
} catch {
    Kiem $false "loi kich ban: $_ $($_.ScriptStackTrace)"
} finally {
    if ($tepMenu -and (Test-Path $tepMenu)) { Set-ItemProperty $tepMenu -Name IsReadOnly -Value $false }
}
Ket-Thuc 'KT11_LoiGhi'
