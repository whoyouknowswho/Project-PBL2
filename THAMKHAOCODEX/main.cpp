#include "Module4_QuanLy/QuanLy.h"
#include "Module5_GiaoDien/GiaoDien.h"
#include "Module7_CuaSo/CuaSo.h"
#include <iostream>
#include <exception>

using namespace std;


int main(int argc, char* argv[]) {
    if (argc < 2 || string(argv[1]) != "--console") {
        try {
            return chayGiaoDienCuaSo();
        } catch (const exception& loi) {
            MessageBoxW(nullptr, rong(loi.what()).c_str(), L"Không mở được ứng dụng", MB_OK | MB_ICONERROR);
            return 1;
        }
    }
    GiaoDien::khoiTao();
    QuanLy quanLy;
    try {
        quanLy.chay();
    } catch (const exception& loi) {
        cerr << loi.what() << '\n';
        return 1;
    }
    return 0;
}
