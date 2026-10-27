#ifndef NGUOI_H
#define NGUOI_H
#include <string>

using namespace std;
class Nguoi {
   protected:
    string ma, ten, soDienThoai, gioiTinh;

   public:
    Nguoi() = default;
    Nguoi(string ma, string ten, string sdt, string gioiTinh);
    virtual ~Nguoi() = default;
    const string& getMa() const;
    const string& getTen() const;
    const string& getSoDienThoai() const;
    const string& getGioiTinh() const;
    virtual string toFile() const = 0;
};
#endif
