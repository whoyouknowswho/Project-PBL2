// Chi bien dich voi -DNHAHANG_GUI_PREVIEW, su dung du lieu gia trong .build/gui-preview.
#include "../Module7_CuaSo/CuaSo.h"
#include <stdexcept>

class KiemTraCuaSo {
public:
    static void chuanBi(CuaSoNhaHang& app) {
        if(app.dichVu.khoiTao()) app.dichVu.taoTaiKhoan("qa_gui","TestOnly123","TestOnly123",true,"");
        if(!app.dichVu.dangNhap("qa_gui","TestOnly123")) throw runtime_error("Preview fixture login failed");
    }
};

int main() {
    wchar_t path[32768]; GetModuleFileNameW(nullptr,path,32768);
    wstring thuMuc=path; thuMuc=thuMuc.substr(0,thuMuc.find_last_of(L"\\/"));
    SetCurrentDirectoryW(thuMuc.c_str());
    if(GetFileAttributesW(L"fixture-gui-test")==INVALID_FILE_ATTRIBUTES) return 1;
    CuaSoNhaHang app; KiemTraCuaSo::chuanBi(app);
    return app.chay(GetModuleHandleW(nullptr),SW_SHOW);
}
