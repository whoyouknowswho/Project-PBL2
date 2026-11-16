# Bat dong bo du lieu tu dong qua git, moi may chay MOT lan:
#   May nguoi nhap du lieu:  powershell -ExecutionPolicy Bypass -File TEST_SQL\Module5_SQL\cai_dong_bo.ps1 -VaiTro nguon
#   May nguoi nhan du lieu:  powershell -ExecutionPolicy Bypass -File TEST_SQL\Module5_SQL\cai_dong_bo.ps1 -VaiTro nhan
#   Tat dong bo:             powershell -ExecutionPolicy Bypass -File TEST_SQL\Module5_SQL\cai_dong_bo.ps1 -VaiTro tat
#
# nguon: moi lan commit, database.sql duoc xuat lai tu SQL Server va dua vao commit.
#        Day nhanh ma khong can commit code: xuat_du_lieu.ps1 -DayLen
# nhan:  moi lan pull/sync ma database.sql thay doi, du lieu duoc nap vao QuanLyNhaHang_PBL2
#        (sao luu .bak truoc khi de).
param([Parameter(Mandatory = $true)][ValidateSet('nguon', 'nhan', 'tat')][string]$VaiTro)
$ErrorActionPreference = 'Stop'
$goc = (& git -C $PSScriptRoot rev-parse --show-toplevel).Trim()
if ($VaiTro -eq 'tat') {
    & git -C $goc config --unset core.hooksPath
    & git -C $goc config --unset pbl2.dongbo
    Write-Host 'Da tat dong bo du lieu tu dong.'
    exit 0
}
& git -C $goc config core.hooksPath .githooks
& git -C $goc config pbl2.dongbo $VaiTro
Write-Host "Da bat dong bo tu dong, may nay la: $VaiTro"
