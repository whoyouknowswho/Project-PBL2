#include "Module4_QuanLy/QuanLy.h"
#include "Module5_GiaoDien/GiaoDien.h"
#include <iostream>
#include <exception>

using namespace std;


int main() {
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
