#include "Module7_CuaSo/DichVuNhaHang.h"
#include "Module5_CoSoDuLieu/SqlServer.h"
#include <iostream>
#include <stdexcept>
void check(bool ok) { if (!ok) throw std::runtime_error("check failed"); }
int main() {
 try {
  DichVuNhaHang dv;
  check(dv.khoiTao());
  dv.taoTaiKhoan("mergeadmin", "TestPass123!", "TestPass123!", "QUANLY", "");
  check(dv.dangNhap("mergeadmin", "TestPass123!"));
  dv.luuKhachHang(KhachHang("OLD", "Old Customer", "0912345678", "Nam", 0), true);
  dv.dangXuat();
  auto ma = dv.dangKyKhachHang("New Customer", "0987654321", "merge@example.com", "Nam", "mergetest", "TestPass123!", "TestPass123!");
  SqlServer::instance().taiLai();
  DichVuNhaHang loaded; check(!loaded.khoiTao());
  check(loaded.dangNhap("mergetest", "TestPass123!"));
  check(loaded.hoSoCuaToi().getEmail() == "merge@example.com");
  loaded.dangXuat();
  bool rejected=false;
  try { loaded.dangKyKhachHang("Duplicate", "0901111222", "MERGE@example.com", "Nam", "duplicate", "TestPass123!", "TestPass123!"); } catch (...) { rejected=true; }
  check(rejected);
  check(loaded.dangNhap("mergeadmin", "TestPass123!"));
  loaded.luuKhachHang(KhachHang("OLD", "Old Customer", "0912345678", "Nam", 10), false);
  SqlServer::instance().taiLai();
  check(SqlServer::instance().doc("KhachHang").find("merge@example.com") != std::string::npos);
  std::cout << "PASS: registration, SQL email reload, duplicate email, legacy customer update\n";
 } catch (const std::exception& e) { std::cerr << e.what(); return 1; }
}
