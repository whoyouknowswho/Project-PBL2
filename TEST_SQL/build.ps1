$ErrorActionPreference = "Stop"
Set-Location -LiteralPath $PSScriptRoot
Stop-Process -Name QuanLyNhaHangGUI -Force -ErrorAction SilentlyContinue
Start-Sleep -Milliseconds 200
$sources = @(Get-ChildItem main.cpp,Module*/*.cpp | ForEach-Object FullName)
$resObj = Join-Path $env:TEMP "pbl2-gui-res.o"
if (-not (Test-Path $resObj)) {
    & windres -I Module7_CuaSo Module7_CuaSo/UngDung.rc -o $resObj
    if ($LASTEXITCODE -ne 0) { throw "Windows resources build failed" }
}
& g++ -std=c++14 -O2 -Wall -Wextra -static -mwindows @sources $resObj -lodbc32 -lcomctl32 -lcomdlg32 -lgdi32 -luxtheme -lgdiplus -lole32 -o QuanLyNhaHangGUI.exe
if ($LASTEXITCODE -ne 0) {
    throw "Build failed. Close QuanLyNhaHangGUI.exe if it is running, then retry."
}
Write-Host "Built: $PSScriptRoot\QuanLyNhaHangGUI.exe"
Write-Host "Run: .\QuanLyNhaHangGUI.exe"
