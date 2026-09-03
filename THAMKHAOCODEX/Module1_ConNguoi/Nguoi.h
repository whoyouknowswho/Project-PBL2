#ifndef NGUOI_H
#define NGUOI_H
#include <string>
class Nguoi {
   protected:
    std::string ma, ten, soDienThoai, gioiTinh;

   public:
    Nguoi() = default;
    Nguoi(std::string ma, std::string ten, std::string sdt, std::string gioiTinh);
    virtual ~Nguoi() = default;
    const std::string& getMa() const;
    const std::string& getTen() const;
    const std::string& getSoDienThoai() const;
    const std::string& getGioiTinh() const;
    void capNhatThongTin(const std::string&, const std::string&, const std::string&);
    virtual void hienThiThongTin() const = 0;
};
#endif
