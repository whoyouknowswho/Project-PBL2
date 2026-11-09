# Bien dich va chay kiem thu don vi cho cac ham tien ich (khong dung exe chinh).
$ErrorActionPreference = "Stop"
$goc = Split-Path -Parent $PSScriptRoot
$ra = Join-Path $env:TEMP "pbl_kiemthu"
New-Item -ItemType Directory -Force $ra | Out-Null
$exe = Join-Path $ra "kt_tienich.exe"
& g++ -std=c++14 -Wall -Wextra -finput-charset=UTF-8 "$goc/KiemThu/KiemThuTienIch.cpp" "$goc/Module7_CuaSo/TienIch.cpp" -o $exe
if ($LASTEXITCODE -ne 0) { Write-Host "Bien dich kiem thu that bai"; exit 1 }
& $exe
exit $LASTEXITCODE
