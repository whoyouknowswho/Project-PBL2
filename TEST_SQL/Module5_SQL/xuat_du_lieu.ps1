# Xuat database QuanLyNhaHang_PBL2 (cau truc + du lieu) ra database.sql o thu muc goc repo.
# Thay cho buoc Generate Scripts trong SSMS. Ket qua sap theo khoa chinh nen chay lai
# ma du lieu khong doi thi file khong doi (git khong bao thay doi).
#
#   powershell -ExecutionPolicy Bypass -File TEST_SQL\Module5_SQL\xuat_du_lieu.ps1
#   powershell -ExecutionPolicy Bypass -File TEST_SQL\Module5_SQL\xuat_du_lieu.ps1 -DayLen
#       -DayLen: xuat xong thi tu commit database.sql + anh mon va push len GitHub.
param(
    [string]$Server = '.\SQLEXPRESS',
    [string]$TepRa = (Join-Path $PSScriptRoot '..\..\database.sql'),
    [switch]$DayLen
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'SqlTienIch.ps1')
$Ten = 'QuanLyNhaHang_PBL2'
$inv = [Globalization.CultureInfo]::InvariantCulture

function GiaTriSql($v) {
    if ($null -eq $v -or $v -is [DBNull]) { return 'NULL' }
    if ($v -is [string]) { return "N'" + $v.Replace("'", "''") + "'" }
    if ($v -is [bool]) { return [string][int]$v }
    if ($v -is [double] -or $v -is [single]) { return $v.ToString('R', $inv) }
    if ($v -is [datetime]) { return "'" + $v.ToString('yyyy-MM-ddTHH:mm:ss.fff', $inv) + "'" }
    if ($v -is [byte[]]) { return '0x' + (($v | ForEach-Object { $_.ToString('X2') }) -join '') }
    return ([IFormattable]$v).ToString($null, $inv)
}

function KieuCot($c) {
    $k = [string]$c.kieu
    if ($k -in 'nvarchar', 'nchar') { return "$k(" + $(if ($c.max_length -eq -1) { 'max' } else { $c.max_length / 2 }) + ')' }
    if ($k -in 'varchar', 'char', 'varbinary', 'binary') { return "$k(" + $(if ($c.max_length -eq -1) { 'max' } else { $c.max_length }) + ')' }
    if ($k -in 'decimal', 'numeric') { return "$k($($c.precision), $($c.scale))" }
    return $k
}

$cn = Mo-KetNoi $Server $Ten
$sb = New-Object System.Text.StringBuilder
function W([string]$s) { [void]$sb.AppendLine($s) }

W '-- Tao boi TEST_SQL/Module5_SQL/xuat_du_lieu.ps1. Khong sua tay; nap bang nhap_du_lieu_cua_ban.ps1.'
W 'USE [master]'
W 'GO'
W "CREATE DATABASE [$Ten]"
W 'GO'
W "USE [$Ten]"
W 'GO'

$cacBang = Lay-Bang $cn "SELECT name FROM sys.tables WHERE schema_id = SCHEMA_ID('dbo') ORDER BY name"
$tong = @()
foreach ($b in $cacBang.Rows) {
    $t = [string]$b.name
    $cot = Lay-Bang $cn @"
SELECT c.name, ty.name AS kieu, c.max_length, c.precision, c.scale, c.is_nullable, c.is_identity
FROM sys.columns c JOIN sys.types ty ON ty.user_type_id = c.user_type_id
WHERE c.object_id = OBJECT_ID(N'dbo.$t') ORDER BY c.column_id
"@
    $khoa = @((Lay-Bang $cn @"
SELECT c.name FROM sys.indexes i
JOIN sys.index_columns ic ON ic.object_id = i.object_id AND ic.index_id = i.index_id
JOIN sys.columns c ON c.object_id = ic.object_id AND c.column_id = ic.column_id
WHERE i.object_id = OBJECT_ID(N'dbo.$t') AND i.is_primary_key = 1 ORDER BY ic.key_ordinal
"@).Rows | ForEach-Object { "[$($_.name)]" })

    $dinhNghia = @(foreach ($c in $cot.Rows) {
        $s = "    [$($c.name)] $(KieuCot $c)"
        if ($c.is_identity) { $s += ' IDENTITY(1,1)' }
        $s + $(if ($c.is_nullable) { ' NULL' } else { ' NOT NULL' })
    })
    if ($khoa.Count) { $dinhNghia += '    PRIMARY KEY (' + ($khoa -join ', ') + ')' }
    W "CREATE TABLE [dbo].[$t] ("
    W ($dinhNghia -join ",`r`n")
    W ')'
    W 'GO'

    $tenCot = @($cot.Rows | ForEach-Object { [string]$_.name })
    $sapXep = if ($khoa.Count) { $khoa -join ', ' } else { ($tenCot | ForEach-Object { "[$_]" }) -join ', ' }
    $duLieu = Lay-Bang $cn "SELECT * FROM [dbo].[$t] ORDER BY $sapXep"
    if ($duLieu.Rows.Count) {
        $coIdentity = @($cot.Rows | Where-Object { $_.is_identity }).Count -gt 0
        if ($coIdentity) { W "SET IDENTITY_INSERT [dbo].[$t] ON" }
        $dsCot = ($tenCot | ForEach-Object { "[$_]" }) -join ', '
        foreach ($r in $duLieu.Rows) {
            $giaTri = foreach ($ten in $tenCot) { GiaTriSql $r[$ten] }
            W "INSERT INTO [dbo].[$t] ($dsCot) VALUES ($($giaTri -join ', '))"
        }
        if ($coIdentity) { W "SET IDENTITY_INSERT [dbo].[$t] OFF" }
        W 'GO'
    }
    $tong += "$t $($duLieu.Rows.Count)"
}
$cn.Close()

$TepRa = [IO.Path]::GetFullPath($TepRa)
[IO.File]::WriteAllText($TepRa, $sb.ToString(), (New-Object System.Text.UTF8Encoding $true))
Write-Host "Da xuat $TepRa ($($tong -join ', '))"

if ($DayLen) {
    $goc = (& git -C $PSScriptRoot rev-parse --show-toplevel).Trim()
    & git -C $goc add -- database.sql TEST_SQL/DuLieu/AnhMon
    & git -C $goc diff --cached --quiet -- database.sql TEST_SQL/DuLieu/AnhMon
    if ($LASTEXITCODE -eq 0) { Write-Host 'Du lieu khong doi, khong can day len.'; exit 0 }
    $env:PBL2_BO_QUA_HOOK = '1'   # vua xuat xong, khong de hook pre-commit xuat lan nua
    & git -C $goc commit -m "Cap nhat du lieu $(Get-Date -Format 'dd/MM/yyyy HH:mm')" -- database.sql TEST_SQL/DuLieu/AnhMon
    if ($LASTEXITCODE -ne 0) { throw 'Commit that bai.' }
    & git -C $goc pull --no-rebase --no-edit
    if ($LASTEXITCODE -ne 0) { throw 'Pull that bai (co the bi xung dot), hay xu ly roi push lai.' }
    & git -C $goc push
    if ($LASTEXITCODE -ne 0) { throw 'Push that bai.' }
    Write-Host 'Da day du lieu len GitHub.'
}
