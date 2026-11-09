# Chay toan bo kiem thu: don vi, tao du lieu mau, KT2..KT10. Dung o loi dau tien.
$ErrorActionPreference = 'Stop'
$cacBuoc = @(
    @('ChayKiemThuDonVi.ps1'), @('TaoDuLieuMau.ps1', '-LamMoi'),
    @('KT2_DangNhap.ps1'), @('KT3_TaiKhoan.ps1'), @('KT4_DanhSach.ps1'), @('KT5_ThucDon.ps1'),
    @('KT6_BanHang.ps1'), @('KT7_DonDat.ps1'), @('KT8_BaoCao.ps1'), @('KT9_TongQuan.ps1'), @('KT10_KichThuoc.ps1'), @('KT11_LoiGhi.ps1')
)
$ketQua = @()
foreach ($buoc in $cacBuoc) {
    $tep = Join-Path $PSScriptRoot $buoc[0]
    $raRa = & pwsh -NoProfile -File $tep @($buoc | Select-Object -Skip 1) 2>&1
    $dong = $raRa | Select-Object -Last 1
    $ketQua += "{0,-24} {1}" -f $buoc[0], $dong
    if ($LASTEXITCODE -ne 0) {
        $ketQua | ForEach-Object { Write-Host $_ }
        $raRa | Select-String 'TRUOT:' | ForEach-Object { Write-Host $_ }
        Write-Host "DUNG: $($buoc[0]) that bai"
        exit 1
    }
}
$ketQua | ForEach-Object { Write-Host $_ }
Write-Host "TAT CA PASS ($($cacBuoc.Count) buoc)"
exit 0
