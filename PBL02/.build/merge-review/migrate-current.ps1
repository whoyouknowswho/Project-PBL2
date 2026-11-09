$ErrorActionPreference='Stop'
$main=(Resolve-Path "$PSScriptRoot/../../../PBLmain").Path
$c=New-Object System.Data.SqlClient.SqlConnection 'Server=localhost\SQLEXPRESS;Database=QuanLyNhaHang;Integrated Security=True;TrustServerCertificate=True;Connect Timeout=5'
$c.Open()
try {
 $q=$c.CreateCommand()
 $counts="SELECT STRING_AGG(CONCAT(t.name,':',p.rows),',') WITHIN GROUP (ORDER BY t.name) FROM sys.tables t JOIN sys.partitions p ON t.object_id=p.object_id AND p.index_id IN (0,1) WHERE SCHEMA_NAME(t.schema_id)='dbo'"
 $q.CommandText=$counts; $before=$q.ExecuteScalar()
 $schema=Get-Content "$main/database/schema.sql" -Raw -Encoding UTF8
 foreach($batch in [regex]::Split($schema,'(?m)^GO\s*$')) {if($batch.Trim()) {$q.CommandText=$batch;$null=$q.ExecuteNonQuery()}}
 $q.CommandText=$counts; $after=$q.ExecuteScalar()
 if($before -ne $after){throw "Row counts changed: $before / $after"}
 Write-Host "PASS: existing database migrated; row counts unchanged: $after"
} finally {$c.Dispose()}
