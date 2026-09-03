#ifndef MON_H
#define MON_H
#include <string>
class Mon {
   protected:
    std::string ma, ten;
    double gia = 0;
    int tonKho = 0;

   public:
    Mon() = default;
    Mon(std::string, std::string, double, int);
    virtual ~Mon() = default;
    const std::string& getMa() const;
    const std::string& getTen() const;
    double getGia() const;
    int getTonKho() const;
    void capNhatCoBan(const std::string&, double, int);
    bool truTonKho(int);
    void congTonKho(int);
    virtual std::string getLoai() const = 0;
    virtual std::string hienThiChiTiet() const = 0;
    // Moi lop con tu in hai cot dac trung cua minh (the hien da hinh).
    virtual void inChiTietRieng() const = 0;
    // Tao ten hien thi tren hoa don; NuocUong se hoi size va lua chon da khi ban.
    virtual std::string taoTenKhiBan() const = 0;
    virtual void capNhatChiTiet() = 0;
    virtual std::string toFile() const = 0;
};
#endif
