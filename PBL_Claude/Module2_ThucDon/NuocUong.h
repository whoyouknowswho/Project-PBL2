#ifndef NUOCUONG_H
#define NUOCUONG_H
#include "Mon.h"

using namespace std;
class NuocUong : public Mon {
    string sizeLy;
    bool coDa = false;

   public:
    NuocUong() = default;
    NuocUong(string maMon, string tenMon, double giaMon, int soLuongTon,
             string kichCo, bool themDa);
    const string& getSizeLy() const;
    bool getCoDa() const;
    string getLoai() const override;
    string hienThiChiTiet() const override;
    string toFile() const override;
};
#endif
