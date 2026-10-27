$ErrorActionPreference = "Stop"
Set-Location -LiteralPath $PSScriptRoot
$sources = @(Get-ChildItem main.cpp,Module*/*.cpp | ForEach-Object FullName)
New-Item -ItemType Directory -Force .build | Out-Null
& windres -I Module7_CuaSo Module7_CuaSo/UngDung.rc -o .build/gui-res.o
if ($LASTEXITCODE -ne 0) { throw "Windows resources build failed" }
& g++ -std=c++14 -O2 -Wall -Wextra -static -mwindows @sources .build/gui-res.o -lbcrypt -lcomctl32 -lgdi32 -luxtheme -o QuanLyNhaHangGUI.exe
if ($LASTEXITCODE -ne 0) {
    throw "Build failed. Close QuanLyNhaHangGUI.exe if it is running, then retry."
}
Write-Host "Built: $PSScriptRoot\QuanLyNhaHangGUI.exe"
Write-Host "Run: .\QuanLyNhaHangGUI.exe"
