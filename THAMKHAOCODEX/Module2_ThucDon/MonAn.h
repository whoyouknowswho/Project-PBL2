#ifndef MONAN_H
#define MONAN_H
#include "Mon.h"

using namespace std;
class MonAn : public Mon {
    int doCay = 0;
    bool monChay = false;

   public:
    MonAn() = default;
    MonAn(string maMon, string tenMon, double giaMon, int soLuongTon,
          int mucDoCay, bool laMonChay);
    int getDoCay() const;
    bool laMonChay() const;
    string getLoai() const override;
    string hienThiChiTiet() const override;
    void inChiTietRieng() const override;
    string taoTenKhiBan() const override;
    void capNhatChiTiet() override;
    string toFile() const override;
};
#endif
