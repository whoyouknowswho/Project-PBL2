#include "Nguoi.h"

#include <utility>
Nguoi::Nguoi(std::string m, std::string t, std::string s, std::string g)
    : ma(std::move(m)), ten(std::move(t)), soDienThoai(std::move(s)), gioiTinh(std::move(g)) {
}
const std::string& Nguoi::getMa() const {
    return ma;
}
const std::string& Nguoi::getTen() const {
    return ten;
}
const std::string& Nguoi::getSoDienThoai() const {
    return soDienThoai;
}
const std::string& Nguoi::getGioiTinh() const {
    return gioiTinh;
}
void Nguoi::capNhatThongTin(const std::string& t, const std::string& s, const std::string& g) {
    ten = t;
    soDienThoai = s;
    gioiTinh = g;
}
