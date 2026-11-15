#include "Mon.h"

using namespace std;

Mon::Mon(string maMon, string tenMon, double giaMon, int soLuongTon)
    : ma(maMon), ten(tenMon), gia(giaMon), tonKho(soLuongTon) {
}

const string& Mon::getMa() const {
    return ma;
}

const string& Mon::getTen() const {
    return ten;
}

const string& Mon::getHinhAnh() const {
    return hinhAnh;
}

void Mon::setHinhAnh(const string& duongDan) {
    hinhAnh = duongDan;
}

double Mon::getGia() const {
    return gia;
}

int Mon::getTonKho() const {
    return tonKho;
}

bool Mon::truTonKho(int soLuong) {
    if (soLuong <= 0 || soLuong > tonKho)
        return false;
    tonKho -= soLuong;
    return true;
}

void Mon::congTonKho(int soLuong) {
    if (soLuong > 0)
        tonKho += soLuong;
}
