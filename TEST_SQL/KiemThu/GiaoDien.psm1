# Bo cong cu dieu khien QuanLyNhaHangGUI.exe de kiem thu giao dien.
# Moi lan chay dung mot hop cat rieng trong %TEMP%\pbl_kiemthu, khong dung du lieu that.
Set-StrictMode -Version Latest

if (-not ('KT.W32' -as [type])) {
Add-Type -TypeDefinition @'
using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
namespace KT {
public struct RECT { public int L, T, R, B; }
public static class W32 {
    [DllImport("user32.dll", CharSet=CharSet.Unicode)] public static extern IntPtr SendMessageW(IntPtr h, uint m, IntPtr w, IntPtr l);
    [DllImport("user32.dll", CharSet=CharSet.Unicode, EntryPoint="SendMessageW")] public static extern IntPtr SendMessageStr(IntPtr h, uint m, IntPtr w, string l);
    [DllImport("user32.dll", CharSet=CharSet.Unicode, EntryPoint="SendMessageW")] public static extern IntPtr SendMessageSb(IntPtr h, uint m, IntPtr w, StringBuilder l);
    [DllImport("user32.dll")] public static extern IntPtr SendMessageTimeoutW(IntPtr h, uint m, IntPtr w, IntPtr l, uint f, uint t, out IntPtr r);
    [DllImport("user32.dll")] public static extern bool PostMessageW(IntPtr h, uint m, IntPtr w, IntPtr l);
    [DllImport("user32.dll")] public static extern IntPtr GetDlgItem(IntPtr h, int id);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr h);
    [DllImport("user32.dll")] public static extern bool IsWindowEnabled(IntPtr h);
    [DllImport("user32.dll")] public static extern bool IsWindow(IntPtr h);
    [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr h, out uint pid);
    [DllImport("user32.dll")] public static extern int GetDlgCtrlID(IntPtr h);
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
    [DllImport("user32.dll")] public static extern bool GetClientRect(IntPtr h, out RECT r);
    [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint f);
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int c);
    [DllImport("user32.dll")] public static extern bool MoveWindow(IntPtr h, int x, int y, int w, int hh, bool r);
    [DllImport("user32.dll")] public static extern int MapWindowPoints(IntPtr a, IntPtr b, ref RECT r, int n);
    [DllImport("user32.dll")] public static extern uint GetGuiResources(IntPtr p, uint f);
    [DllImport("user32.dll")] public static extern bool AttachThreadInput(uint a, uint b, bool f);
    [DllImport("user32.dll")] public static extern IntPtr SetFocus(IntPtr h);
    [DllImport("user32.dll")] public static extern IntPtr GetParent(IntPtr h);
    [DllImport("user32.dll")] public static extern IntPtr GetWindowLongPtrW(IntPtr h, int i);
    [DllImport("kernel32.dll")] public static extern uint GetCurrentThreadId();
    [DllImport("kernel32.dll")] public static extern IntPtr OpenProcess(uint a, bool i, uint pid);
    [DllImport("kernel32.dll")] public static extern bool CloseHandle(IntPtr h);
    [DllImport("kernel32.dll")] public static extern IntPtr VirtualAllocEx(IntPtr p, IntPtr a, IntPtr n, uint t, uint pr);
    [DllImport("kernel32.dll")] public static extern bool VirtualFreeEx(IntPtr p, IntPtr a, IntPtr n, uint t);
    [DllImport("kernel32.dll")] public static extern bool ReadProcessMemory(IntPtr p, IntPtr a, byte[] b, IntPtr n, out IntPtr r);
    [DllImport("kernel32.dll")] public static extern bool WriteProcessMemory(IntPtr p, IntPtr a, byte[] b, IntPtr n, out IntPtr r);
    delegate bool EnumProc(IntPtr h, IntPtr l);
    [DllImport("user32.dll")] static extern bool EnumWindows(EnumProc f, IntPtr l);
    [DllImport("user32.dll")] static extern bool EnumChildWindows(IntPtr p, EnumProc f, IntPtr l);
    [DllImport("user32.dll", CharSet=CharSet.Unicode)] static extern int GetClassNameW(IntPtr h, StringBuilder s, int n);

    public static string ClassOf(IntPtr h) { var s = new StringBuilder(256); GetClassNameW(h, s, 256); return s.ToString(); }
    public static List<IntPtr> TopWindows(uint pid) {
        var ds = new List<IntPtr>();
        EnumWindows((h, l) => { uint p; GetWindowThreadProcessId(h, out p); if (p == pid) ds.Add(h); return true; }, IntPtr.Zero);
        return ds;
    }
    public static List<IntPtr> Children(IntPtr cha) {
        var ds = new List<IntPtr>();
        EnumChildWindows(cha, (h, l) => { ds.Add(h); return true; }, IntPtr.Zero);
        return ds;
    }
    public static string Text(IntPtr h) {
        int n = (int)SendMessageW(h, 0x000E, IntPtr.Zero, IntPtr.Zero);
        var s = new StringBuilder(n + 2);
        SendMessageSb(h, 0x000D, (IntPtr)(n + 1), s);
        return s.ToString();
    }
    public static bool Idle(IntPtr h, uint ms) { IntPtr r; return SendMessageTimeoutW(h, 0, IntPtr.Zero, IntPtr.Zero, 0, ms, out r) != IntPtr.Zero; }

    const uint PROC = 0x0008 | 0x0010 | 0x0020 | 0x0400;
    // Doc toan bo o cua ListView trong tien trinh khac bang bo nho cap phat ben do.
    public static List<string[]> ReadListView(IntPtr lv) {
        uint pid; GetWindowThreadProcessId(lv, out pid);
        IntPtr proc = OpenProcess(PROC, false, pid);
        var kq = new List<string[]>();
        IntPtr header = SendMessageW(lv, 0x101F, IntPtr.Zero, IntPtr.Zero);
        int soCot = Math.Max(1, (int)SendMessageW(header, 0x1200, IntPtr.Zero, IntPtr.Zero));
        int soDong = (int)SendMessageW(lv, 0x1004, IntPtr.Zero, IntPtr.Zero);
        IntPtr mem = VirtualAllocEx(proc, IntPtr.Zero, (IntPtr)4096, 0x3000, 0x04);
        try {
            for (int d = 0; d < soDong; d++) {
                var dong = new string[soCot];
                for (int c = 0; c < soCot; c++) {
                    byte[] item = new byte[88];
                    BitConverter.GetBytes(c).CopyTo(item, 8);
                    BitConverter.GetBytes(mem.ToInt64() + 128).CopyTo(item, 24);
                    BitConverter.GetBytes(1000).CopyTo(item, 32);
                    IntPtr w; WriteProcessMemory(proc, mem, item, (IntPtr)88, out w);
                    int n = (int)SendMessageW(lv, 0x1073, (IntPtr)d, mem);
                    byte[] buf = new byte[2000]; IntPtr r;
                    ReadProcessMemory(proc, mem + 128, buf, (IntPtr)2000, out r);
                    dong[c] = Encoding.Unicode.GetString(buf, 0, Math.Max(0, n) * 2);
                }
                kq.Add(dong);
            }
        } finally { VirtualFreeEx(proc, mem, IntPtr.Zero, 0x8000); CloseHandle(proc); }
        return kq;
    }
    public static void SelectListViewRow(IntPtr lv, int row) {
        uint pid; GetWindowThreadProcessId(lv, out pid);
        IntPtr proc = OpenProcess(PROC, false, pid);
        IntPtr mem = VirtualAllocEx(proc, IntPtr.Zero, (IntPtr)128, 0x3000, 0x04);
        try {
            byte[] item = new byte[88]; IntPtr w;
            BitConverter.GetBytes(0).CopyTo(item, 12); BitConverter.GetBytes(3).CopyTo(item, 16);
            WriteProcessMemory(proc, mem, item, (IntPtr)88, out w);
            SendMessageW(lv, 0x102B, (IntPtr)(-1), mem);
            BitConverter.GetBytes(3).CopyTo(item, 12);
            WriteProcessMemory(proc, mem, item, (IntPtr)88, out w);
            SendMessageW(lv, 0x102B, (IntPtr)row, mem);
        } finally { VirtualFreeEx(proc, mem, IntPtr.Zero, 0x8000); CloseHandle(proc); }
    }
    // Windows chan WM_NOTIFY giua hai tien trinh, nen gui chuot nhap dup vao chinh dong cua ListView;
    // ListView tu bao NM_DBLCLK cho cua so cha.
    public static void PostDoubleClick(IntPtr lv, int row) {
        uint pid; GetWindowThreadProcessId(lv, out pid);
        IntPtr proc = OpenProcess(PROC, false, pid);
        IntPtr mem = VirtualAllocEx(proc, IntPtr.Zero, (IntPtr)64, 0x3000, 0x04);
        byte[] r = new byte[16]; IntPtr w;
        BitConverter.GetBytes(0).CopyTo(r, 0);   // LVIR_BOUNDS
        WriteProcessMemory(proc, mem, r, (IntPtr)16, out w);
        SendMessageW(lv, 0x100E, (IntPtr)row, mem); // LVM_GETITEMRECT
        ReadProcessMemory(proc, mem, r, (IntPtr)16, out w);
        VirtualFreeEx(proc, mem, IntPtr.Zero, 0x8000); CloseHandle(proc);
        int x = BitConverter.ToInt32(r, 0) + 20, y = (BitConverter.ToInt32(r, 4) + BitConverter.ToInt32(r, 12)) / 2;
        IntPtr toaDo = (IntPtr)((y << 16) | (x & 0xFFFF));
        PostMessageW(lv, 0x0201, (IntPtr)1, toaDo); PostMessageW(lv, 0x0202, IntPtr.Zero, toaDo);
        PostMessageW(lv, 0x0203, (IntPtr)1, toaDo); PostMessageW(lv, 0x0202, IntPtr.Zero, toaDo);
    }
    public static void Focus(IntPtr h) {
        uint pid; uint t = GetWindowThreadProcessId(h, out pid), me = GetCurrentThreadId();
        AttachThreadInput(me, t, true); SetFocus(h); AttachThreadInput(me, t, false);
    }
}
}
'@
}

$script:Ma = @{
    TRANG_CHU=100; KHACH_HANG=101; NHAN_VIEN=102; THUC_DON=103; BAN_HANG=104; BAO_CAO=105
    TAI_KHOAN=106; MAT_KHAU=107; DANG_XUAT=108; HO_SO_CUA_TOI=109; HOA_DON_CUA_TOI=110; DAT_MON=111; DON_DAT=112
    TIM_KIEM=200; LOC_LOAI=201; SAP_XEP=202; CON_HANG=203; LOC_BAO_CAO=204
    BANG_CHINH=210; BANG_GIO=211; BANG_TON=212; DANH_SACH_MON=214; LOC_NAM_DOANHTHU=215; DANH_SACH_THUCDON=216
    O_MA=300; O_TEN=301; O_SDT=302; O_GIOITINH=303; O_DIEM=304; O_CHUCVU=305; O_LUONG=306; O_PHUCAP=307
    O_LOAI=308; O_GIA=309; O_TON=310; O_CAY=311; O_CHAY=312; O_SIZE=313; O_NHAPKHO=314
    O_NGAY=330; O_KHACH=331; O_NHANVIEN=332; O_SOLUONG=333; O_TONG=334
    O_USER=350; O_PASS=351; O_CONFIRM=352; O_ROLE=353; O_OLDPASS=354; O_EMAIL=355
    O_HANG=360; O_UUDAI=361; NHAN_HOSO=362; NHAN_SIZE=363; NHAN_KY_BAOCAO=370; NHAN_MON_CHON=371; NHAN_GIO_TRONG=372
    NUT_LUU=400; NUT_MOI=401; NUT_XOA=402; NUT_NHAPKHO=403; NUT_THEM_GIO=404; NUT_XOA_GIO=405; NUT_HUY_GIO=406
    NUT_THANHTOAN=407; NUT_BAOCAO=408; NUT_HOADON=409; NUT_DANGNHAP=410; NUT_TAOTAIKHOAN=411; NUT_DOIMATKHAU=412
    NUT_MO_DANGKY=413; NUT_DANGKY=414; NUT_VE_DANGNHAP=415; NUT_GUI_DON=416; NUT_HUY_DON=417; NUT_THANHTOAN_DON=418
    NUT_LAMMOI_DON=419; NUT_BC_HOMNAY=420; NUT_BC_THANG=421; NUT_BC_TATCA=422; NUT_NHOM_THUCAN=423; NUT_NHOM_NUOC=424
    NUT_GIAM_SL=425; NUT_TANG_SL=426
}
$script:MAT_KHAU_THU = 'KiemThu@2026'
$script:App = $null
$script:Dat = 0
$script:Truot = [System.Collections.Generic.List[string]]::new()
$script:GocDuAn = Split-Path -Parent $PSScriptRoot

function MaDK($id) { if ($id -is [string]) { $script:Ma[$id] } else { [int]$id } }
function Hwnd($id) { [KT.W32]::GetDlgItem($script:App.Hwnd, (MaDK $id)) }

function Tao-HopCat {
    param([switch]$TaiKhoanTrong, [string]$NguonDuLieu)
    $goc = Join-Path $env:TEMP 'pbl_kiemthu'
    $thuMuc = Join-Path $goc (Get-Date -Format 'yyyyMMdd-HHmmss-fff')
    New-Item -ItemType Directory -Force $thuMuc, (Join-Path $thuMuc 'anh') | Out-Null
    Copy-Item (Join-Path $script:GocDuAn 'QuanLyNhaHangGUI.exe') $thuMuc
    $nguon = 'QuanLyNhaHang_PBL2'
    if ($NguonDuLieu) {
        $nguon = (Get-Content (Join-Path $NguonDuLieu 'database.id') -Raw).Trim()
        if ($nguon -notmatch '^PBL2_KT_[0-9a-f]{32}$') { throw 'Sai database kiem thu' }
    }
    $db = 'PBL2_KT_' + [guid]::NewGuid().ToString('N')
    Invoke-KiemThuSql master "CREATE DATABASE [$db]"
    $script:DatabaseThu = $db
    foreach ($bang in 'ThucDon','KhachHang','NhanVien','HoaDon','DonDat','TaiKhoan') {
        Invoke-KiemThuSql $db "SELECT * INTO dbo.$bang FROM [$nguon].dbo.$bang"
    }
    if ($TaiKhoanTrong) { Invoke-KiemThuSql $db 'DELETE FROM dbo.TaiKhoan' }
    New-Item -ItemType Directory -Force (Join-Path $thuMuc 'DuLieu') | Out-Null
    Set-Content (Join-Path $thuMuc 'DuLieu/database.id') $db
    $thuMuc
}

function Tim-CuaSoChinh([int]$pid_) {
    foreach ($h in [KT.W32]::TopWindows($pid_)) { if ([KT.W32]::ClassOf($h) -eq 'NhaHangCuaSo') { return $h } }
    [IntPtr]::Zero
}

function Khoi-Dong([string]$thuMuc) {
    $db = (Get-Content (Join-Path $thuMuc 'DuLieu/database.id') -Raw).Trim()
    if ($db -notmatch '^PBL2_KT_[0-9a-f]{32}$') { throw 'Sai database kiem thu' }
    $env:PBL2_SQL_CONNECTION = "Driver={ODBC Driver 18 for SQL Server};Server=.\SQLEXPRESS;Database=$db;Trusted_Connection=yes;TrustServerCertificate=yes"
    $p = Start-Process (Join-Path $thuMuc 'QuanLyNhaHangGUI.exe') -PassThru
    $h = [IntPtr]::Zero
    for ($i = 0; $i -lt 100 -and $h -eq [IntPtr]::Zero; $i++) { Start-Sleep -Milliseconds 100; $h = Tim-CuaSoChinh $p.Id }
    if ($h -eq [IntPtr]::Zero) { throw "Khong thay cua so chinh" }
    $script:App = [pscustomobject]@{ Process = $p; Hwnd = $h; ThuMuc = $thuMuc; Pid = $p.Id }
    Cho-Ranh
    $script:App
}

function Cho-Ranh([int]$ms = 120) {
    Start-Sleep -Milliseconds $ms
    if ($script:App -and [KT.W32]::IsWindow($script:App.Hwnd)) { [void][KT.W32]::Idle($script:App.Hwnd, 15000) }
}

function Dat-Chu($id, [string]$chu) { [void][KT.W32]::SendMessageStr((Hwnd $id), 0x000C, [IntPtr]::Zero, $chu); Cho-Ranh 30 }
function Doc-Chu($id) { [KT.W32]::Text((Hwnd $id)) }
function Go-Phim($id, [string]$chu) {
    $h = Hwnd $id
    foreach ($c in $chu.ToCharArray()) { [void][KT.W32]::PostMessageW($h, 0x0102, [IntPtr][int]$c, [IntPtr]1) }
    Cho-Ranh 80
}
function Bam($id) {
    $ma = MaDK $id
    $w = [IntPtr]([int]$ma)  # BN_CLICKED = 0
    [void][KT.W32]::PostMessageW($script:App.Hwnd, 0x0111, $w, (Hwnd $id))
    Cho-Ranh
}
function Gui-LenhCon($id, [int]$suKien) {
    $ma = MaDK $id
    $w = [IntPtr](($suKien -shl 16) -bor $ma)
    [void][KT.W32]::PostMessageW($script:App.Hwnd, 0x0111, $w, (Hwnd $id))
    Cho-Ranh
}
function Chon-Hop($id, [int]$viTri) {
    [void][KT.W32]::SendMessageW((Hwnd $id), 0x014E, [IntPtr]$viTri, [IntPtr]::Zero)
    Gui-LenhCon $id 1   # CBN_SELCHANGE
}
function So-MucHop($id) { [int][KT.W32]::SendMessageW((Hwnd $id), 0x0146, [IntPtr]::Zero, [IntPtr]::Zero) }
function Muc-Hop($id) { [int][KT.W32]::SendMessageW((Hwnd $id), 0x0147, [IntPtr]::Zero, [IntPtr]::Zero) }
function Chu-MucHop($id, [int]$viTri) {
    $h = Hwnd $id
    $n = [int][KT.W32]::SendMessageW($h, 0x0149, [IntPtr]$viTri, [IntPtr]::Zero)
    $s = [System.Text.StringBuilder]::new($n + 2)
    [void][KT.W32]::SendMessageSb($h, 0x0148, [IntPtr]$viTri, $s)
    $s.ToString()
}
function Chon-Dong($id, [int]$dong) { [KT.W32]::SelectListViewRow((Hwnd $id), $dong); Cho-Ranh }
function NhapDup-Dong($id, [int]$dong) { [KT.W32]::PostDoubleClick((Hwnd $id), $dong); Cho-Ranh }
function Doc-Bang($id) { ,([KT.W32]::ReadListView((Hwnd $id))) }
function So-Dong($id) { [int][KT.W32]::SendMessageW((Hwnd $id), 0x1004, [IntPtr]::Zero, [IntPtr]::Zero) }
function Chon-Mon($id, [int]$viTri) { [void][KT.W32]::SendMessageW((Hwnd $id), 0x8000 + 101, [IntPtr]$viTri, [IntPtr]::Zero); Cho-Ranh }
function Mon-Chon($id) { [int][KT.W32]::SendMessageW((Hwnd $id), 0x8000 + 102, [IntPtr]::Zero, [IntPtr]::Zero) }
function So-Mon($id) { [int][KT.W32]::SendMessageW((Hwnd $id), 0x8000 + 103, [IntPtr]::Zero, [IntPtr]::Zero) }
function Co-DieuKhien($id) { (Hwnd $id) -ne [IntPtr]::Zero }
function Dang-Bat($id) { $h = Hwnd $id; ($h -ne [IntPtr]::Zero) -and [KT.W32]::IsWindowEnabled($h) }
function Dang-Hien($id) { $h = Hwnd $id; ($h -ne [IntPtr]::Zero) -and [KT.W32]::IsWindowVisible($h) }
function Chi-Doc($id) { ([KT.W32]::GetWindowLongPtrW((Hwnd $id), -16).ToInt64() -band 0x0800) -ne 0 }
function Dat-TieuDiem($id) { [KT.W32]::Focus((Hwnd $id)); Cho-Ranh 30 }
function Nhan-Enter($id) {
    Dat-TieuDiem $id
    [void][KT.W32]::PostMessageW((Hwnd $id), 0x0100, [IntPtr]0x0D, [IntPtr]1)
    Cho-Ranh 200
}

function Tim-HopThoai {
    foreach ($h in [KT.W32]::TopWindows($script:App.Pid)) {
        if ([KT.W32]::ClassOf($h) -eq '#32770' -and [KT.W32]::IsWindowVisible($h)) { return $h }
    }
    [IntPtr]::Zero
}
function Cho-HopThoai {
    param([ValidateSet('Yes','No','OK','')][string]$Bam = 'OK', [double]$Giay = 6)
    $han = (Get-Date).AddSeconds($Giay); $h = [IntPtr]::Zero
    while ((Get-Date) -lt $han) { $h = Tim-HopThoai; if ($h -ne [IntPtr]::Zero) { break }; Start-Sleep -Milliseconds 80 }
    if ($h -eq [IntPtr]::Zero) { return $null }
    Start-Sleep -Milliseconds 80
    $chu = [KT.W32]::Text([KT.W32]::GetDlgItem($h, 0xFFFF))
    if ($Bam) {
        $nut = @{ OK = 1; Yes = 6; No = 7 }[$Bam]
        # Hop thoai MB_OK chi co mot nut mang ma IDCANCEL (2).
        if ($Bam -eq 'OK' -and [KT.W32]::GetDlgItem($h, 1) -eq [IntPtr]::Zero) { $nut = 2 }
        [void][KT.W32]::PostMessageW($h, 0x0111, [IntPtr]$nut, [KT.W32]::GetDlgItem($h, $nut))
        for ($i = 0; $i -lt 50 -and [KT.W32]::IsWindow($h); $i++) { Start-Sleep -Milliseconds 60 }
        Cho-Ranh
    }
    $chu
}
function Tim-CuaSo([string]$lop) {
    foreach ($h in [KT.W32]::TopWindows($script:App.Pid)) { if ([KT.W32]::ClassOf($h) -eq $lop -and [KT.W32]::IsWindowVisible($h)) { return $h } }
    [IntPtr]::Zero
}
function Cho-CuaSo([string]$lop, [double]$Giay = 6) {
    $han = (Get-Date).AddSeconds($Giay)
    while ((Get-Date) -lt $han) { $h = Tim-CuaSo $lop; if ($h -ne [IntPtr]::Zero) { return $h }; Start-Sleep -Milliseconds 80 }
    [IntPtr]::Zero
}

function Chup([string]$ten, [IntPtr]$h = [IntPtr]::Zero) {
    Add-Type -AssemblyName System.Drawing
    if ($h -eq [IntPtr]::Zero) { $h = $script:App.Hwnd }
    Start-Sleep -Milliseconds 150
    $r = New-Object KT.RECT; [void][KT.W32]::GetWindowRect($h, [ref]$r)
    $bmp = New-Object System.Drawing.Bitmap ([Math]::Max(1, $r.R - $r.L)), ([Math]::Max(1, $r.B - $r.T))
    $g = [System.Drawing.Graphics]::FromImage($bmp); $dc = $g.GetHdc()
    [void][KT.W32]::PrintWindow($h, $dc, 2); $g.ReleaseHdc($dc); $g.Dispose()
    $tep = Join-Path $script:App.ThuMuc "anh\$ten.png"
    $bmp.Save($tep); $bmp.Dispose()
    $tep
}
function Doi-KichThuoc([int]$w, [int]$h) {
    [void][KT.W32]::ShowWindow($script:App.Hwnd, 1)
    [void][KT.W32]::MoveWindow($script:App.Hwnd, 20, 20, $w, $h, $true)
    Start-Sleep -Milliseconds 400; Cho-Ranh
}
function Phong-To { [void][KT.W32]::ShowWindow($script:App.Hwnd, 3); Start-Sleep -Milliseconds 400; Cho-Ranh }
function So-GDI { [int][KT.W32]::GetGuiResources($script:App.Process.Handle, 0) }
function Hinh-Con($id) {
    $r = New-Object KT.RECT; $h = Hwnd $id
    [void][KT.W32]::GetWindowRect($h, [ref]$r)
    [void][KT.W32]::MapWindowPoints([IntPtr]::Zero, $script:App.Hwnd, [ref]$r, 2)
    $r
}
function Cac-ConHien {
    foreach ($h in [KT.W32]::Children($script:App.Hwnd)) {
        if ([KT.W32]::GetParent($h) -eq $script:App.Hwnd -and [KT.W32]::IsWindowVisible($h)) { $h }
    }
}

function Ung-Dung { $script:App }
function Cho-Thoat([int]$ms = 5000) {
    [void]$script:App.Process.WaitForExit($ms)
    $script:App.Process.HasExited
}

function Dong-UngDung {
    if (-not $script:App) { return }
    $p = $script:App.Process
    if (-not $p.HasExited) {
        [void][KT.W32]::PostMessageW($script:App.Hwnd, 0x0010, [IntPtr]::Zero, [IntPtr]::Zero)
        for ($i = 0; $i -lt 30 -and -not $p.HasExited; $i++) {
            if ((Tim-HopThoai) -ne [IntPtr]::Zero) { [void](Cho-HopThoai -Bam Yes -Giay 1) }
            Start-Sleep -Milliseconds 100
        }
        if (-not $p.HasExited) { $p.Kill() }
    }
    $script:App = $null
}

function Dang-Nhap([string]$ten, [string]$matKhau = $script:MAT_KHAU_THU) {
    Dat-Chu O_USER $ten; Dat-Chu O_PASS $matKhau; Bam NUT_DANGNHAP; Cho-Ranh 300
}
function Dang-Xuat { Bam DANG_XUAT; [void](Cho-HopThoai -Bam Yes) }

function Kiem([bool]$dieuKien, [string]$moTa) {
    if ($dieuKien) { $script:Dat++ } else { $script:Truot.Add($moTa); Write-Host "  TRUOT: $moTa" -ForegroundColor Red }
}
function Ket-Thuc([string]$ten) {
    Dong-UngDung
    if ($script:DatabaseThu -match '^PBL2_KT_[0-9a-f]{32}$') {
        Invoke-KiemThuSql master "DROP DATABASE [$script:DatabaseThu]"
    }
    $tong = $script:Dat + $script:Truot.Count
    if ($script:Truot.Count -gt 0) { Write-Host "$ten TRUOT $($script:Truot.Count)/$tong"; exit 1 }
    Write-Host "$ten PASS $($script:Dat)/$tong"; exit 0
}

function Invoke-KiemThuSql($database, $sql) {
    $kn = [System.Data.SqlClient.SqlConnection]::new("Server=.\SQLEXPRESS;Database=$database;Integrated Security=True;Pooling=False;TrustServerCertificate=True")
    try {
        $kn.Open()
        $lenh = $kn.CreateCommand()
        $lenh.CommandText = $sql
        $doc = $lenh.ExecuteReader()
        while ($doc.Read()) {
            $cot = for ($i = 0; $i -lt $doc.FieldCount; $i++) { $doc.GetValue($i) }
            $cot -join '|'
        }
    } finally { $kn.Dispose() }
}

function Doc-BangSql($hop, $bang) {
    $db = (Get-Content (Join-Path $hop 'DuLieu/database.id') -Raw).Trim()
    $cot = if ($bang -eq 'DonDat') { 'noiDung' } else { '*' }
    Invoke-KiemThuSql $db "SELECT $cot FROM dbo.$bang"
}

Export-ModuleMember -Function * -Variable MAT_KHAU_THU, Ma
