#ifndef NUOCUONG_H
#define NUOCUONG_H
#include "Mon.h"
class NuocUong : public Mon {
    std::string sizeLy;
    bool coDa = false;

   public:
    NuocUong() = default;
    NuocUong(std::string, std::string, double, int, std::string, bool);
    const std::string& getSizeLy() const;
    bool getCoDa() const;
    std::string getLoai() const override;
    std::string hienThiChiTiet() const override;
    void inChiTietRieng() const override;
    std::string taoTenKhiBan() const override;
    void capNhatChiTiet() override;
    std::string toFile() const override;
};
#endif
