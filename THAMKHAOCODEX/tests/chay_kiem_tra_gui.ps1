$ErrorActionPreference = "Stop"
Set-Location -LiteralPath (Split-Path -Parent $PSScriptRoot)
$sources = @(Get-ChildItem Module*/*.cpp | Where-Object { $_.Directory.Name -ne "Module7_CuaSo" } | ForEach-Object FullName)
$sources += (Join-Path $PWD "Module7_CuaSo/DichVuNhaHang.cpp")
New-Item -ItemType Directory -Force .build | Out-Null
& g++ -std=c++14 -Wall -Wextra @sources tests/kiem_tra_dich_vu_gui.cpp -lbcrypt -o .build/kiem_tra_dich_vu_gui.exe
if ($LASTEXITCODE -ne 0) { throw "Test build failed" }
@'
from pathlib import Path
import subprocess
import tempfile
root = Path.cwd()
with tempfile.TemporaryDirectory(dir=root / '.build') as temporary:
    folder = Path(temporary)
    (folder / 'fixture-gui-test').write_text('test-only')
    (folder / 'DuLieu').mkdir()
    for name in ['menu.txt', 'khachhang.txt', 'nhanvien.txt', 'hoadon.txt']:
        (folder / 'DuLieu' / name).write_text('')
    subprocess.run([str(root / '.build/kiem_tra_dich_vu_gui.exe')], cwd=folder, check=True)
'@ | python -
if ($LASTEXITCODE -ne 0) { throw "GUI service tests failed" }
