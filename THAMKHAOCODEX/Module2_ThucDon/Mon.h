#ifndef MON_H
#define MON_H
#include <string>

using namespace std;
class Mon {
   protected:
    string ma, ten;
    double gia = 0;
    int tonKho = 0;

   public:
    Mon() = default;
    Mon(string maMon, string tenMon, double giaMon, int soLuongTon);
    virtual ~Mon() = default;
    const string& getMa() const;
    const string& getTen() const;
    double getGia() const;
    int getTonKho() const;
    bool truTonKho(int soLuong);
    void congTonKho(int soLuong);
    virtual string getLoai() const = 0;
    virtual string hienThiChiTiet() const = 0;
    virtual string toFile() const = 0;
};
#endif
