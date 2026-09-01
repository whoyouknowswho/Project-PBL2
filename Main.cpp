#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Bao gồm thư viện cục bộ cho Web Server và JSON
#include "httplib.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// Cấu trúc dữ liệu cho PBL2 Food App
struct MenuItem {
    int id;
    string name;
    double price;
    string category;
    string image;
    string description;
};

// Cơ sở dữ liệu mẫu (Mock Database)
vector<MenuItem> menu_db = {
    { 1, "Phở Bò Tái Nạm", 45000, "Món Nước", "https://images.unsplash.com/photo-1582878826629-29b7ad1cb438?w=500&q=80", "Phở bò truyền thống, nước dùng thanh ngọt thơm mùi hồi quế." },
    { 2, "Bún Chả Hà Nội", 40000, "Món Khô", "https://images.unsplash.com/photo-1617064379963-7186dfc79dcb?w=500&q=80", "Bún chả thịt nướng than hoa, kèm chả nem rán giòn rụm." },
    { 3, "Cơm Gà Xối Mỡ", 35000, "Cơm", "https://images.unsplash.com/photo-1604908176997-125f25cc6f3d?w=500&q=80", "Đùi gà xối mỡ giòn tan, ăn kèm cơm chiên dương châu." },
    { 4, "Trà Sữa Trân Châu", 25000, "Đồ Uống", "https://images.unsplash.com/photo-1558857563-b37102e976db?w=500&q=80", "Hồng trà sữa truyền thống, topping trân châu đen dai giòn." },
    { 5, "Cà Phê Sữa Đá", 20000, "Đồ Uống", "https://images.unsplash.com/photo-1517701550927-30cfcb64db10?w=500&q=80", "Cà phê phin đậm đà sữa đặc, đánh thức mọi giác quan." },
    { 6, "Gỏi Cuốn Tôm Thịt", 30000, "Ăn Vặt", "https://images.unsplash.com/photo-1632778149955-f50626a570c9?w=500&q=80", "Tôm nhảy, thịt ba chỉ luộc, chấm cùng tương đen hoặc mắm nêm." }
};

int main() {
    cout << "========================================================\n";
    cout << "  🚀 KHOI DONG HE THONG PBL2 (Food Ordering System)\n";
    cout << "  Sinh vien: To Dong Hai & Le Van Tai (25T_DT3)\n";
    cout << "========================================================\n";
    cout << "[!] Dang mo trinh duyet (http://localhost:8080) ...\n";
    
    // Tự động mở URL trên hệ điều hành Windows
    system("start http://localhost:8080");

    httplib::Server svr;

    // 1. API: Phục vụ file giao diện Frontend (index.html)
    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        ifstream file("index.html");
        if (file.is_open()) {
            string html((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            res.set_content(html, "text/html; charset=utf-8");
        } else {
            res.status = 404;
            res.set_content("Lỗi: Không tìm thấy file index.html!", "text/plain");
        }
    });

    // 2. API: Lấy danh sách thực đơn (GET /api/menu)
    svr.Get("/api/menu", [](const httplib::Request& req, httplib::Response& res) {
        json j_array = json::array();
        for (const auto& item : menu_db) {
            j_array.push_back({
                {"id", item.id},
                {"name", item.name},
                {"price", item.price},
                {"category", item.category},
                {"image", item.image},
                {"description", item.description}
            });
        }
        res.set_content(j_array.dump(), "application/json; charset=utf-8");
    });

    // 3. API: Xử lý Đặt hàng và Thanh toán (POST /api/checkout)
    svr.Post("/api/checkout", [](const httplib::Request& req, httplib::Response& res) {
        try {
            // Parse dữ liệu JSON từ Frontend gửi lên
            json order_data = json::parse(req.body);
            
            // Log thông tin đơn hàng ra màn hình Console C++
            cout << "\n[+] NHAN DUOC DON HANG MOI:\n";
            double total = order_data["total"].get<double>();
            
            for (const auto& item : order_data["items"]) {
                cout << "  - " << item["name"].get<string>() 
                     << " (x" << item["quantity"].get<int>() << ")\n";
            }
            cout << "  => Tong tien thanh toan: " << total << " VND\n";

            // Tạo phản hồi (Response) xác nhận thành công
            json response_data = {
                {"status", "Thành công"},
                {"order_id", rand() % 90000 + 10000}, // Sinh mã đơn hàng ngẫu nhiên
                {"message", "Đơn hàng của bạn đã được C++ Backend xử lý thành công!"},
                {"total", total}
            };
            
            res.set_content(response_data.dump(), "application/json; charset=utf-8");
        } catch (const exception& e) {
            cout << "[-] LOI KHI XU LY DON HANG: " << e.what() << "\n";
            res.status = 400;
            res.set_content("{\"status\":\"Lỗi dữ liệu đầu vào\"}", "application/json; charset=utf-8");
        }
    });

    cout << "[!] C++ Web Server dang chay tai cong 8080...\n";
    cout << "[!] Nhan Ctrl + C de dung may chu.\n";
    
    // Lắng nghe ở cổng 8080 (Localhost)
    svr.listen("localhost", 8080);

    return 0;
}