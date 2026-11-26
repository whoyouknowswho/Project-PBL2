#include "LuuTruTxt.h"
#include <direct.h>
#include <fstream>
#include <stdexcept>

using namespace std;

namespace {
// Đường dẫn file .txt và số cột tương ứng với từng bảng trong enum Bang
const char* TEN_TEP[] = {
    "DuLieu/ThucDon.txt",
    "DuLieu/KhachHang.txt",
    "DuLieu/NhanVien.txt",
    "DuLieu/HoaDon.txt",
    "DuLieu/DonDat.txt",
    "DuLieu/TaiKhoan.txt"
};

const size_t SO_COT[] = {8, 6, 7, 6, 1, 4};

// Tách 1 dòng văn bản thành các cột ngăn cách bởi dấu '|'
LuuTru::Dong tachCot(const string& dong, size_t soCot) {
    if (soCot == 1) return {dong};

    LuuTru::Dong cacCot;
    string o;
    for (char c : dong) {
        if (c == '|') {
            cacCot.push_back(o);
            o.clear();
        } else {
            o += c;
        }
    }
    cacCot.push_back(o); // thêm cột cuối cùng

    while (cacCot.size() < soCot) {
        cacCot.push_back("");
    }
    return cacCot;
}
} // namespace

LuuTru::CacDong LuuTru::doc(Bang bang) {
    _mkdir("DuLieu");
    ifstream tep(TEN_TEP[bang]);
    if (!tep.is_open()) {
        ofstream taoMoi(TEN_TEP[bang]); // tự tạo file rỗng nếu chưa có
        return {};
    }

    CacDong ketQua;
    string dong;
    while (getline(tep, dong)) {
        if (!dong.empty() && dong.back() == '\r') dong.pop_back();
        if (dong.empty()) continue;
        ketQua.push_back(tachCot(dong, SO_COT[bang]));
    }
    return ketQua;
}

void LuuTru::luu(const vector<pair<Bang, CacDong>>& thayDoi) {
    _mkdir("DuLieu");
    for (const auto& muc : thayDoi) {
        Bang bang = muc.first;
        const CacDong& cacDong = muc.second;
        size_t soCot = SO_COT[bang];

        ofstream tep(TEN_TEP[bang]);
        if (!tep.is_open()) {
            throw runtime_error(string("Không mở được file để ghi: ") + TEN_TEP[bang]);
        }

        for (const Dong& dong : cacDong) {
            for (size_t i = 0; i < soCot; i++) {
                if (i > 0) tep << '|';
                if (i < dong.size()) tep << dong[i];
            }
            tep << '\n';
        }
    }
}
