#pragma once
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace Sql {
enum Bang { ThucDon, KhachHang, NhanVien, HoaDon, DonDat, TaiKhoan };

using Dong = vector<string>;
using CacDong = vector<Dong>;

CacDong doc(Bang bang);
void luu(const vector<pair<Bang, CacDong>>& thayDoi);
}
