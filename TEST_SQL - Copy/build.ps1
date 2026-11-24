$ErrorActionPreference = "Stop"
Set-Location -LiteralPath $PSScriptRoot
Stop-Process -Name QuanLyNhaHangGUI -Force -ErrorAction SilentlyContinue
Start-Sleep -Milliseconds 200

$sources = @(Get-ChildItem main.cpp,Module*/*.cpp | ForEach-Object FullName)
$resObj = Join-Path $env:TEMP "pbl2-gui-res.o"
& windres -I Module7_CuaSo Module7_CuaSo/UngDung.rc -o $resObj
& g++ -std=c++14 -O2 -Wall -Wextra -static -mwindows @sources $resObj -lsqlite3 -lz -lcomctl32 -lcomdlg32 -lgdi32 -luxtheme -lgdiplus -lole32 -o QuanLyNhaHangGUI.exe
if ($LASTEXITCODE -ne 0) { throw "Build that bai." }
Remove-Item $resObj -ErrorAction SilentlyContinue
Write-Host "Build thanh cong: $PSScriptRoot\QuanLyNhaHangGUI.exe"
