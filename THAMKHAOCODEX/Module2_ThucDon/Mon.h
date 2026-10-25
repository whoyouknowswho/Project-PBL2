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
    void capNhatCoBan(const string& tenMon, double giaMon, int soLuongTon);
    bool truTonKho(int soLuong);
    void congTonKho(int soLuong);
    virtual string getLoai() const = 0;
    virtual string hienThiChiTiet() const = 0;
    // Moi lop con tu in hai cot dac trung cua minh (the hien da hinh).
    virtual void inChiTietRieng() const = 0;
    // Tao ten hien thi tren hoa don; NuocUong se hoi size va lua chon da khi ban.
    virtual string taoTenKhiBan() const = 0;
    virtual void capNhatChiTiet() = 0;
    virtual string toFile() const = 0;
};
#endif
