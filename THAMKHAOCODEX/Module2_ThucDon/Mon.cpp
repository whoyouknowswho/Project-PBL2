#include "Mon.h"

#include <utility>
Mon::Mon(std::string m, std::string t, double g, int ton)
    : ma(std::move(m)), ten(std::move(t)), gia(g), tonKho(ton) {
}
const std::string& Mon::getMa() const {
    return ma;
}
const std::string& Mon::getTen() const {
    return ten;
}
double Mon::getGia() const {
    return gia;
}
int Mon::getTonKho() const {
    return tonKho;
}
void Mon::capNhatCoBan(const std::string& t, double g, int ton) {
    ten = t;
    gia = g;
    tonKho = ton;
}
bool Mon::truTonKho(int sl) {
    if (sl <= 0 || sl > tonKho)
        return false;
    tonKho -= sl;
    return true;
}
void Mon::congTonKho(int sl) {
    if (sl > 0)
        tonKho += sl;
}
