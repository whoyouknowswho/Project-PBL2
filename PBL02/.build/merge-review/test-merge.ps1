$ErrorActionPreference='Stop'
$main=(Resolve-Path "$PSScriptRoot/../../../PBLmain").Path
$env:PATH='C:\msys64\ucrt64\bin;'+$env:PATH
$sources=@(Get-ChildItem "$main/Module[1-6]*/*.cpp" | ForEach-Object FullName)
& g++ -std=c++14 -O0 -static -I $main "$PSScriptRoot/smoke.cpp" @sources "$main/Module7_CuaSo/DichVuNhaHang.cpp" "$main/Module7_CuaSo/DatMonDichVu.cpp" -lodbc32 -lbcrypt -o "$PSScriptRoot/smoke.exe"
if ($LASTEXITCODE) {throw 'Smoke build failed'}
$db='PBL2_MergeTest_'+[guid]::NewGuid().ToString('N')
$conn=New-Object System.Data.SqlClient.SqlConnection 'Server=localhost\SQLEXPRESS;Database=master;Integrated Security=True;TrustServerCertificate=True;Connect Timeout=5'
$conn.Open()
$cmd=$conn.CreateCommand()
try {
 $cmd.CommandText="CREATE DATABASE [$db]"; $null=$cmd.ExecuteNonQuery()
 $conn.ChangeDatabase($db)
 $schema=Get-Content "$main/database/schema.sql" -Raw -Encoding UTF8
 # Simulate the older schema and a customer without Email before migration.
 $cmd.CommandText="CREATE TABLE dbo.KhachHang (Ma nvarchar(160) COLLATE Latin1_General_100_BIN2 PRIMARY KEY, HoTen nvarchar(160) NOT NULL, DienThoai nvarchar(160) NOT NULL, GioiTinh nvarchar(160) NOT NULL, Diem int NOT NULL CHECK(Diem>=0)); INSERT dbo.KhachHang VALUES(N'LEGACY',N'Legacy',N'0901234567',N'Nam',7)"; $null=$cmd.ExecuteNonQuery()
 foreach($iteration in 1..2) {
  foreach($batch in [regex]::Split($schema,'(?m)^GO\s*$')) {if($batch.Trim()) {$cmd.CommandText=$batch;$null=$cmd.ExecuteNonQuery()}}
 }
 $cmd.CommandText="SELECT COUNT(*) FROM dbo.KhachHang WHERE Ma=N'LEGACY' AND Diem=7 AND Email=N''"
 if($cmd.ExecuteScalar() -ne 1){throw 'Migration lost legacy data'}
 $run=Join-Path $PSScriptRoot 'smoke-run'; $null=New-Item -ItemType Directory -Force "$run/DuLieu"
 "[SQLServer]`nServer=localhost\SQLEXPRESS`nDatabase=$db`nDriver=ODBC Driver 18 for SQL Server`nTrustServerCertificate=Yes" | Set-Content "$run/DuLieu/sqlserver.ini"
 Push-Location $run
 try { & "$PSScriptRoot/smoke.exe"; if($LASTEXITCODE){throw 'Smoke tests failed'} } finally {Pop-Location}
 Write-Host 'PASS: schema migration is repeatable and preserves legacy data'
} finally {
 $conn.ChangeDatabase('master')
 $cmd.CommandText="ALTER DATABASE [$db] SET SINGLE_USER WITH ROLLBACK IMMEDIATE; DROP DATABASE [$db]"; $null=$cmd.ExecuteNonQuery()
 $conn.Dispose()
}
