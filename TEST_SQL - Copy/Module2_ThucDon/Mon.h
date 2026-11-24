#ifndef MON_H
#define MON_H
#include <string>
#include <vector>

using namespace std;
class Mon {
   protected:
    string ma, ten, hinhAnh;
    double gia = 0;
    int tonKho = 0;

   public:
    Mon() = default;
    Mon(string maMon, string tenMon, double giaMon, int soLuongTon);
    virtual ~Mon() = default;
    const string& getMa() const;
    const string& getTen() const;
    const string& getHinhAnh() const;
    void setHinhAnh(const string& duongDan);
    double getGia() const;
    int getTonKho() const;
    bool truTonKho(int soLuong);
    void congTonKho(int soLuong);
    virtual string getLoai() const = 0;
    virtual string hienThiChiTiet() const = 0;
    virtual vector<string> cacCot() const = 0;
};
#endif
