#include "NGUOI.H"

NGUOI::NGUOI(string a, string b, string c, string d) : ma(a), hoten(b), SDT(c), SEX(d) {
}
NGUOI::~NGUOI() {};
const string& NGUOI::getma() const {
    return this->ma;
}
const string& NGUOI::getten() const {
    return this->hoten;
}
const string& NGUOI::getSDT() const {
    return this->SDT;
}
const string& NGUOI::getSEX() const {
    return this->SEX;
}
void NGUOI::update(const string& a, const string& b, const string& c) {
    this->ma = a;
    this->hoten = b;
    this->SEX = c;
}
