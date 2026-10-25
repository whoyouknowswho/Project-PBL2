#include "QuanLy.h"
#include "../Module5_GiaoDien/GiaoDien.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace GiaoDien;

bool QuanLy::laQuanLy() const {
    return taiKhoanHienTai.vaiTro == "QUANLY";
}

bool QuanLy::dangNhap() {
    taiKhoanHienTai = TaiKhoan();
    for (int lanThu = 1; lanThu <= 3; lanThu++) {
        tieuDe("DANG NHAP HE THONG");
        string ten = nhapChuoi("Ten dang nhap (0 de thoat): ");
        if (ten == "0") {
            return false;
        }
        string matKhau = nhapMatKhau("Mat khau: ");
        if (quanLyTaiKhoan.kiemTraMatKhau(ten, matKhau)) {
            const TaiKhoan* taiKhoan = quanLyTaiKhoan.tim(ten);
            // Tai khoan nhan vien chi hoat dong khi ho so nhan vien con ton tai.
            if (taiKhoan->vaiTro == "QUANLY" || timNhanVien(taiKhoan->maNhanVien) != nullptr) {
                taiKhoanHienTai = *taiKhoan;
                return true;
            }
        }
        cout << DO << "Sai tai khoan/mat khau hoac ho so nhan vien khong con ton tai.\n"
             << "So lan thu con lai: " << 3 - lanThu << RESET << '\n';
        dungManHinh();
    }
    return false;
}

bool QuanLy::taoTaiKhoan(bool lanDau) {
    // Chi cho phep khoi tao quan ly khi danh sach tai khoan chua duoc tao.
    if (!lanDau && !laQuanLy()) {
        cout << DO << "Ban khong co quyen tao tai khoan.\n" << RESET;
        return false;
    }
    tieuDe("TAO TAI KHOAN");
    if (lanDau) {
        cout << "Lan chay dau: hay tao tai khoan QUAN LY cua ban.\n";
    }
    string ten = nhapChuoi("Ten dang nhap (0 de huy): ");
    if (ten == "0") {
        return false;
    }
    string vaiTro = "QUANLY";
    string maNhanVien;
    if (!lanDau) {
        int luaChon = nhapInt("Vai tro (1-Quan ly, 2-Nhan vien): ", 1, 2);
        if (luaChon == 2) {
            vaiTro = "NHANVIEN";
            maNhanVien = nhapChuoi("Ma nhan vien da co trong danh sach: ");
            if (timNhanVien(maNhanVien) == nullptr) {
                cout << DO << "Khong tim thay nhan vien. Hay them ho so nhan vien truoc.\n" << RESET;
                return false;
            }
        }
    }
    string matKhau = nhapMatKhau("Mat khau (8-128 ky tu): ");
    string xacNhan = nhapMatKhau("Nhap lai mat khau: ");
    if (matKhau != xacNhan) {
        cout << DO << "Hai mat khau khong trung nhau.\n" << RESET;
        return false;
    }
    try {
        quanLyTaiKhoan.tao(ten, matKhau, vaiTro, maNhanVien);
        cout << XANH << "Da tao tai khoan thanh cong.\n" << RESET;
        return true;
    } catch (const exception& loi) {
        cout << DO << loi.what() << RESET << '\n';
        return false;
    }
}

void QuanLy::doiMatKhau() {
    tieuDe("DOI MAT KHAU");
    string matKhauCu = nhapMatKhau("Mat khau hien tai: ");
    if (!quanLyTaiKhoan.kiemTraMatKhau(taiKhoanHienTai.tenDangNhap, matKhauCu)) {
        cout << DO << "Mat khau hien tai khong dung.\n" << RESET;
        return;
    }
    string matKhauMoi = nhapMatKhau("Mat khau moi (8-128 ky tu): ");
    string xacNhan = nhapMatKhau("Nhap lai mat khau moi: ");
    if (matKhauMoi != xacNhan) {
        cout << DO << "Hai mat khau khong trung nhau.\n" << RESET;
        return;
    }
    try {
        quanLyTaiKhoan.doiMatKhau(taiKhoanHienTai.tenDangNhap, matKhauMoi);
        cout << XANH << "Da doi mat khau.\n" << RESET;
    } catch (const exception& loi) {
        cout << DO << loi.what() << RESET << '\n';
    }
}
