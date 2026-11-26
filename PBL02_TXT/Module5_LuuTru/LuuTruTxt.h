#ifndef LUUTRU_TXT_H
#define LUUTRU_TXT_H

#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace LuuTru {
enum Bang { ThucDon, KhachHang, NhanVien, HoaDon, DonDat, TaiKhoan };

using Dong = vector<string>;
using CacDong = vector<Dong>;

// Đọc toàn bộ các dòng từ file .txt tương ứng với bảng
CacDong doc(Bang bang);

// Ghi lại dữ liệu của các bảng ra file .txt
void luu(const vector<pair<Bang, CacDong>>& thayDoi);
}

#endif
