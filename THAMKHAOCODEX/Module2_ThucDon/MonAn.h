#ifndef MONAN_H
#define MONAN_H
#include "Mon.h"
class MonAn : public Mon {
    int doCay = 0;
    bool monChay = false;

   public:
    MonAn() = default;
    MonAn(std::string, std::string, double, int, int, bool);
    int getDoCay() const;
    bool laMonChay() const;
    std::string getLoai() const override;
    std::string hienThiChiTiet() const override;
    void inChiTietRieng() const override;
    std::string taoTenKhiBan() const override;
    void capNhatChiTiet() override;
    std::string toFile() const override;
};
#endif
