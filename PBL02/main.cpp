#include "Module7_CuaSo/CuaSo.h"
#include <exception>

using namespace std;

int main() {
    try {
        return chayGiaoDienCuaSo();
    } catch (const exception& loi) {
        MessageBoxW(nullptr, rong(loi.what()).c_str(), L"Không mở được ứng dụng", MB_OK | MB_ICONERROR);
        return 1;
    }
}
