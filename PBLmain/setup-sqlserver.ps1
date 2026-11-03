param([string]$Server = 'localhost\SQLEXPRESS', [string]$Database = 'QuanLyNhaHang')
$ErrorActionPreference = 'Stop'
Set-Location -LiteralPath $PSScriptRoot
if ($Database -notmatch '^[A-Za-z][A-Za-z0-9_]{0,100}$') { throw 'Invalid database name' }
$builder = New-Object System.Data.SqlClient.SqlConnectionStringBuilder
$builder['Data Source'] = $Server
$builder['Initial Catalog'] = 'master'
$builder['Integrated Security'] = $true
$builder['TrustServerCertificate'] = $true
$builder['Connect Timeout'] = 5
$connection = New-Object System.Data.SqlClient.SqlConnection $builder.ConnectionString
try {
    $connection.Open()
    $command = $connection.CreateCommand()
    $command.CommandText = "IF DB_ID(N'$Database') IS NULL CREATE DATABASE [$Database]"
    $command.ExecuteNonQuery() | Out-Null
    $connection.ChangeDatabase($Database)
    $schema = Get-Content -LiteralPath 'database/schema.sql' -Raw -Encoding UTF8
    foreach ($batch in [regex]::Split($schema, '(?m)^GO\s*$')) {
        if ($batch.Trim()) { $command.CommandText = $batch; $command.ExecuteNonQuery() | Out-Null }
    }
    # Du lieu ban dau la cac lenh INSERT trong file SQL, khong can cong cu C++.
    # File tu bo qua neu database da co du lieu.
    $command.CommandText = Get-Content -LiteralPath 'database/du-lieu-ban-dau.sql' -Raw -Encoding UTF8
    $command.ExecuteNonQuery() | Out-Null
} finally { $connection.Dispose() }

New-Item -ItemType Directory -Force 'DuLieu' | Out-Null
$ini = "[SQLServer]`r`nServer=$Server`r`nDatabase=$Database`r`nDriver=ODBC Driver 18 for SQL Server`r`nTrustServerCertificate=Yes`r`n"
[System.IO.File]::WriteAllText((Join-Path $PSScriptRoot 'DuLieu/sqlserver.ini'), $ini)
# Ket noi va schema da duoc kiem tra o tren; khong can bien dich cong cu rieng.
Write-Host "Database ready: $Server / $Database"
