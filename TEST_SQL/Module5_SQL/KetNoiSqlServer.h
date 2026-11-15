#pragma once
#include <string>
#include <utility>
#include <vector>

namespace Sql {
enum Bang { ThucDon, KhachHang, NhanVien, HoaDon, DonDat, TaiKhoan };

using Dong = std::vector<std::string>;
using CacDong = std::vector<Dong>;

CacDong doc(Bang bang);
void luu(const std::vector<std::pair<Bang, CacDong>>& thayDoi);
}
