"""Kiem tra dang nhap tren ban sao du lieu, khong sua DuLieu cua du an.
Chay: python tests/kiem_tra_dang_nhap.py
"""
from pathlib import Path
import hashlib
import shutil
import subprocess
import tempfile
import sys

ROOT = Path(__file__).resolve().parents[1]
EXE = ROOT / 'QuanLyNhaHangGUI.exe'
if len(sys.argv) > 1:
    EXE = Path(sys.argv[1]).resolve()


def chay(thu_muc, cau_tra_loi, thanh_cong=True):
    ket_qua = subprocess.run(
        [str(EXE), '--console'], cwd=thu_muc, input=cau_tra_loi.encode(),
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30)
    if thanh_cong:
        assert ket_qua.returncode == 0, ket_qua.stderr.decode(errors='replace')
    else:
        assert ket_qua.returncode != 0
    return ket_qua.stdout.decode(errors='replace') + ket_qua.stderr.decode(errors='replace')


with tempfile.TemporaryDirectory(dir=ROOT / "tests") as tam:
    thu_muc = Path(tam)
    shutil.copytree(ROOT / 'DuLieu', thu_muc / 'DuLieu')
    tep_tai_khoan = thu_muc / 'DuLieu/taikhoan.txt'
    if tep_tai_khoan.exists():
        tep_tai_khoan.unlink()
    # Fixture rieng, khong phu thuoc du lieu ma nguoi dung da sua.
    (thu_muc / 'DuLieu/nhanvien.txt').write_text('NV01|Nhan vien thu|090123|Nam|Thu ngan|100|0\n')
    (thu_muc / 'DuLieu/menu.txt').write_text('MONAN|MA01|Com thu|45000|10|1|0\n')
    (thu_muc / 'DuLieu/hoadon.txt').write_text('')

    # Khoi tao quan ly, dang nhap, tao nhan vien va dang xuat.
    noi_dung = chay(thu_muc, 'admin_test\nAdminPass123\nAdminPass123\n\nadmin_test\nAdminPass123\n6\nstaff_test\n2\nNV01\nStaffPass123\nStaffPass123\n\n8\n0\n')
    assert noi_dung.count('Da tao tai khoan thanh cong.') == 2
    du_lieu = tep_tai_khoan.read_text()
    assert 'AdminPass123' not in du_lieu and 'StaffPass123' not in du_lieu
    for dong in du_lieu.splitlines():
        phien_ban, ten, vai_tro, ma, muoi, bam = dong.split('|')
        mat_khau = 'AdminPass123' if ten == 'admin_test' else 'StaffPass123'
        assert hashlib.pbkdf2_hmac('sha256', mat_khau.encode(), muoi.encode(), 600000).hex() == bam
    print('PASS: khoi tao, tao nhan vien, bam PBKDF2 va luu tai khoan')

    noi_dung = chay(thu_muc, 'staff_test\nsai\n\nstaff_test\nStaffPass123\n6\n7\n8\n2\n\n3\nHD_TEST\n25/10/2026\n\nMA01\n1\n0\n\n0\n')
    assert 'Sai tai khoan/mat khau' in noi_dung
    assert noi_dung.count('Vui long nhap so hop le!') == 3
    assert '2.  Xem thuc don' in noi_dung
    assert '3.  Ban hang - Lap hoa don' in noi_dung
    assert '4.  Doi mat khau' in noi_dung
    assert '5.  Dang xuat' in noi_dung
    assert 'Quan ly nhan vien' not in noi_dung
    assert 'Thong ke va bao cao' not in noi_dung
    assert 'Tao tai khoan' not in noi_dung
    assert 'Ma nhan vien:' not in noi_dung
    assert '1.Them  2.Sua  3.Xoa' not in noi_dung
    assert 'HD_TEST|25/10/2026||NV01|' in (thu_muc / 'DuLieu/hoadon.txt').read_text()
    print('PASS: sai mat khau, phan quyen, xem thuc don, hoa don gan dung nhan vien')

    noi_dung = chay(thu_muc, 'staff_test\nStaffPass123\n4\nwrong\n\n4\nStaffPass123\nNewStaff123\nDifferent123\n\n4\nStaffPass123\nNewStaff123\nNewStaff123\n\n5\nstaff_test\nStaffPass123\n\nstaff_test\nNewStaff123\n0\n')
    assert 'Mat khau hien tai khong dung.' in noi_dung
    assert 'Hai mat khau khong trung nhau.' in noi_dung
    assert 'Da doi mat khau.' in noi_dung
    assert 'Sai tai khoan/mat khau' in noi_dung
    print('PASS: doi mat khau, dang xuat, tu choi mat khau cu')

    noi_dung = chay(thu_muc, 'staff_test\nNewStaff123\n1\n0\n5\nadmin_test\nAdminPass123\n2\n0\n5\n\n\n0\n')
    assert 'DANH SACH KHACH HANG' in noi_dung
    assert 'DANH SACH NHAN VIEN' in noi_dung
    assert 'THONG KE - BAO CAO KINH DOANH' in noi_dung
    print('PASS: muc 1 khach hang, muc 5 dang xuat va menu quan ly sau chuyen tai khoan')

    truoc = tep_tai_khoan.read_bytes()
    noi_dung = chay(thu_muc, 'admin_test\nAdminPass123\n6\nstaff_test\n2\nNV01\nAnother123\nAnother123\n\n6\nmissing_staff\n2\nNO_SUCH_ID\n\n0\n')
    assert 'Ten dang nhap da ton tai.' in noi_dung
    assert 'Khong tim thay nhan vien.' in noi_dung
    assert tep_tai_khoan.read_bytes() == truoc
    print('PASS: tu choi ten trung va ma nhan vien khong ton tai')

    (thu_muc / 'DuLieu/nhanvien.txt').write_text('')
    noi_dung = chay(thu_muc, 'staff_test\nNewStaff123\n\n0\n')
    assert 'Sai tai khoan/mat khau' in noi_dung
    print('PASS: tu choi nhan vien da bi xoa')

    noi_dung = chay(thu_muc, 'admin_test\nx\n\nadmin_test\nx\n\nadmin_test\nx\n\n')
    assert noi_dung.count('Sai tai khoan/mat khau') == 3
    assert 'Lua chon cua ban' not in noi_dung
    print('PASS: dung sau 3 lan dang nhap sai')

    tep_tai_khoan.write_text('du lieu hong\n')
    noi_dung = chay(thu_muc, '', thanh_cong=False)
    assert 'Tep tai khoan khong hop le' in noi_dung
    assert tep_tai_khoan.read_text() == 'du lieu hong\n'
    print('PASS: tep hong khong mo quyen truy cap hay tao lai quan ly')

    tep_tai_khoan.unlink()
    noi_dung = chay(thu_muc, '', thanh_cong=False)
    assert not tep_tai_khoan.exists()
    print('PASS: EOF khong treo chuong trinh hoac tao tai khoan')

