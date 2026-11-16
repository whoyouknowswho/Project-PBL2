# Thay database QuanLyNhaHang_PBL2 tren may nay bang du lieu trong database.sql (thu muc goc repo).
# Nhan ca file do xuat_du_lieu.ps1 tao lan file Generate Scripts cua SSMS.
#
#   powershell -ExecutionPolicy Bypass -File TEST_SQL\Module5_SQL\nhap_du_lieu_cua_ban.ps1
#
# Cac buoc:
#   1. Dong QuanLyNhaHangGUI.exe neu dang mo (app ghi lai toan bo bang khi luu, se de mat du lieu moi).
#   2. Sao luu database hien tai ra file .bak trong thu muc Backup cua SQL Server.
#   3. Nap database.sql vao mot database tam; neu loi thi dung lai, database cu khong bi dong toi.
#   4. Xoa database cu, doi ten database tam thanh QuanLyNhaHang_PBL2.
param(
    [string]$Server = '.\SQLEXPRESS',
    [string]$TepSql = (Join-Path $PSScriptRoot '..\..\database.sql')
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'SqlTienIch.ps1')
$Ten = 'QuanLyNhaHang_PBL2'
# Ten tam doi theo thoi gian: file .mdf/.ldf mang ten database luc tao, doi ten database khong doi ten file.
$Tam = 'QuanLyNhaHang_PBL2_Nap{0:yyyyMMddHHmmss}' -f (Get-Date)
$xoaTam = "IF DB_ID(N'$Tam') IS NOT NULL BEGIN ALTER DATABASE [$Tam] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; DROP DATABASE [$Tam]; END"

$TepSql = (Resolve-Path $TepSql).Path
Write-Host "Nguon du lieu: $TepSql"

# 1. Dong app
$app = Get-Process QuanLyNhaHangGUI -ErrorAction SilentlyContinue
if ($app) { Write-Host 'Dang dong QuanLyNhaHangGUI.exe...'; $app | Stop-Process -Force; Start-Sleep -Milliseconds 500 }

$cn = Mo-KetNoi $Server

# 2. Sao luu
if ([int](Lay-GiaTri $cn "SELECT COUNT(*) FROM sys.databases WHERE name = N'$Ten'") -eq 1) {
    $thuMuc = [string](Lay-GiaTri $cn "SELECT CAST(SERVERPROPERTY('InstanceDefaultBackupPath') AS nvarchar(400))")
    $tep = Join-Path $thuMuc ('{0}_{1:yyyyMMdd_HHmmss}.bak' -f $Ten, (Get-Date))
    Chay-Sql $cn "BACKUP DATABASE [$Ten] TO DISK = N'$tep' WITH COPY_ONLY, INIT"
    Write-Host "Da sao luu database cu: $tep"
}

# 3. Nap vao database tam (bo duong dan .mdf/.ldf ma SSMS ghi cung theo may nguoi xuat)
$noiDung = [IO.File]::ReadAllText($TepSql)   # tu nhan BOM UTF-8/UTF-16, mac dinh UTF-8
if ($noiDung -notmatch "CREATE DATABASE \[$Ten\]") { throw "File khong co lenh CREATE DATABASE [$Ten]." }
$noiDung = [regex]::Replace($noiDung, "(?s)(CREATE DATABASE \[$Ten\]).*?(\r?\nGO)", '$1$2')
$noiDung = [regex]::Replace($noiDung, "(?m)^ALTER DATABASE \[$Ten\] SET COMPATIBILITY_LEVEL = \d+\s*$", '')
$noiDung = $noiDung.Replace("[$Ten]", "[$Tam]")

Chay-Sql $cn $xoaTam
try {
    $i = 0
    foreach ($lo in (Tach-Lo $noiDung)) { $i++; Chay-Sql $cn $lo }
    Chay-Sql $cn 'USE [master]'
} catch {
    try { Chay-Sql $cn 'USE [master]'; Chay-Sql $cn $xoaTam } catch {}
    throw "Khong nap duoc database.sql (lo lenh thu $i), database cu van giu nguyen.`n$($_.Exception.Message)"
}
$cn.Close()
[System.Data.SqlClient.SqlConnection]::ClearAllPools()

# 4. Thay database cu
$cn = Mo-KetNoi $Server
Chay-Sql $cn "IF DB_ID(N'$Ten') IS NOT NULL BEGIN ALTER DATABASE [$Ten] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; DROP DATABASE [$Ten]; END"
Chay-Sql $cn "ALTER DATABASE [$Tam] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; ALTER DATABASE [$Tam] MODIFY NAME = [$Ten]; ALTER DATABASE [$Ten] SET MULTI_USER;"
$dem = Lay-Bang $cn @"
SELECT 'ThucDon' AS bang, COUNT(*) AS so FROM [$Ten].dbo.ThucDon UNION ALL SELECT 'KhachHang', COUNT(*) FROM [$Ten].dbo.KhachHang
UNION ALL SELECT 'NhanVien', COUNT(*) FROM [$Ten].dbo.NhanVien UNION ALL SELECT 'HoaDon', COUNT(*) FROM [$Ten].dbo.HoaDon
UNION ALL SELECT 'DonDat', COUNT(*) FROM [$Ten].dbo.DonDat UNION ALL SELECT 'TaiKhoan', COUNT(*) FROM [$Ten].dbo.TaiKhoan
"@
$cn.Close()
Write-Host ("Xong. $Ten co: " + (($dem.Rows | ForEach-Object { "$($_.bang) $($_.so)" }) -join ', '))
