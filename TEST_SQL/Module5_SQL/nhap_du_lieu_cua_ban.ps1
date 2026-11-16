# Thay database QuanLyNhaHang_PBL2 tren may nay bang du lieu trong database.sql (thu muc goc repo).
# Cach chay (PowerShell, tai thu muc TEST_SQL):
#     powershell -ExecutionPolicy Bypass -File Module5_SQL\nhap_du_lieu_cua_ban.ps1
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
$Ten = 'QuanLyNhaHang_PBL2'
$Tam = 'QuanLyNhaHang_PBL2_DangNhap'

function Sql([string]$lenh, [string]$db = 'master') {
    $kq = & sqlcmd -S $Server -E -C -b -f 65001 -d $db -h -1 -W -Q "SET NOCOUNT ON; $lenh" 2>&1
    if ($LASTEXITCODE -ne 0) { throw "Lỗi SQL: $kq" }
    return $kq
}

$TepSql = (Resolve-Path $TepSql).Path
Write-Host "Nguồn dữ liệu: $TepSql"

# 1. Dong app
$app = Get-Process QuanLyNhaHangGUI -ErrorAction SilentlyContinue
if ($app) { Write-Host 'Đang đóng QuanLyNhaHangGUI.exe...'; $app | Stop-Process -Force; Start-Sleep -Milliseconds 500 }

# 2. Sao luu
$coSan = (Sql "SELECT COUNT(*) FROM sys.databases WHERE name = N'$Ten'").Trim()
if ($coSan -eq '1') {
    $thuMuc = (Sql "SELECT CAST(SERVERPROPERTY('InstanceDefaultBackupPath') AS nvarchar(400))").Trim()
    $tep = Join-Path $thuMuc ("{0}_{1:yyyyMMdd_HHmmss}.bak" -f $Ten, (Get-Date))
    Sql "BACKUP DATABASE [$Ten] TO DISK = N'$tep' WITH COPY_ONLY, INIT" | Out-Null
    Write-Host "Đã sao lưu database cũ: $tep"
}

# 3. Nap vao database tam (bo duong dan file .mdf/.ldf ghi cung theo may nguoi xuat)
$noiDung = Get-Content -LiteralPath $TepSql -Raw   # tu nhan UTF-16/UTF-8 co BOM
$noiDung = [regex]::Replace($noiDung, "(?s)(CREATE DATABASE \[$Ten\]).*?(\r?\nGO)", '$1$2')
$noiDung = [regex]::Replace($noiDung, "(?m)^ALTER DATABASE \[$Ten\] SET COMPATIBILITY_LEVEL = \d+\s*$", '')
$noiDung = $noiDung.Replace("[$Ten]", "[$Tam]")
$tepTam = Join-Path $env:TEMP 'pbl2_nhap_du_lieu.sql'
[IO.File]::WriteAllText($tepTam, $noiDung, (New-Object Text.UTF8Encoding $true))

Sql "IF DB_ID(N'$Tam') IS NOT NULL BEGIN ALTER DATABASE [$Tam] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; DROP DATABASE [$Tam]; END" | Out-Null
$kq = & sqlcmd -S $Server -E -C -b -f 65001 -i $tepTam 2>&1
if ($LASTEXITCODE -ne 0) {
    Sql "IF DB_ID(N'$Tam') IS NOT NULL BEGIN ALTER DATABASE [$Tam] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; DROP DATABASE [$Tam]; END" | Out-Null
    throw "Không nạp được database.sql, database cũ vẫn giữ nguyên.`n$kq"
}
Remove-Item $tepTam -ErrorAction SilentlyContinue

# 4. Thay database cu
Sql "IF DB_ID(N'$Ten') IS NOT NULL BEGIN ALTER DATABASE [$Ten] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; DROP DATABASE [$Ten]; END" | Out-Null
Sql "ALTER DATABASE [$Tam] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; ALTER DATABASE [$Tam] MODIFY NAME = [$Ten]; ALTER DATABASE [$Ten] SET MULTI_USER;" | Out-Null

Write-Host "Xong. Số dòng trong $Ten`:"
Sql "SELECT 'ThucDon', COUNT(*) FROM dbo.ThucDon UNION ALL SELECT 'KhachHang', COUNT(*) FROM dbo.KhachHang
     UNION ALL SELECT 'NhanVien', COUNT(*) FROM dbo.NhanVien UNION ALL SELECT 'HoaDon', COUNT(*) FROM dbo.HoaDon
     UNION ALL SELECT 'DonDat', COUNT(*) FROM dbo.DonDat UNION ALL SELECT 'TaiKhoan', COUNT(*) FROM dbo.TaiKhoan" $Ten
