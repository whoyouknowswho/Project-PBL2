#ifndef NHANVIEN_H
#define NHANVIEN_H

#include "NGUOI.h"

class NHANVIEN : public NGUOI {
private:
    string chucvu;
    double luong = 0;
    double phucap = 0;

public:
    NHANVIEN();
    NHANVIEN(string, string, string, string, string, double, double);
    ~NHANVIEN();
    const string& getchucvu() const;
    double getluong() const;
    double getphucap() const;
    double tinhluong() const;
    void updateVIEC(const string&, double, double);
    void show() const override;
    string tofile() const;
};

#endif