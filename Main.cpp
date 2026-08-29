#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string>
#include <sstream>
#include <iomanip>

// Control IDs
#define IDC_EDIT_A      1001
#define IDC_EDIT_B      1002
#define IDC_BTN_ADD     1003
#define IDC_BTN_SUB     1004
#define IDC_BTN_MUL     1005
#define IDC_BTN_DIV     1006
#define IDC_BTN_CLEAR   1007

// Colors (Modern Dark Theme - Slate & Indigo Palette)
#define COLOR_BG            RGB(15, 23, 42)      // Slate 900
#define COLOR_CARD_BG       RGB(30, 41, 59)      // Slate 800
#define COLOR_CARD_BORDER   RGB(51, 65, 85)      // Slate 700
#define COLOR_INPUT_BG      RGB(51, 65, 85)      // Slate 700
#define COLOR_TEXT_PRIMARY  RGB(248, 250, 252)   // Slate 50
#define COLOR_TEXT_MUTED    RGB(148, 163, 184)   // Slate 400
#define COLOR_ACCENT        RGB(99, 102, 241)    // Indigo 500
#define COLOR_RESULT_BG     RGB(16, 185, 129)    // Emerald 500
#define COLOR_BADGE_BG      RGB(37, 99, 235)     // Blue 600

// Global variables
HWND hEditA, hEditB;
HWND hBtnAdd, hBtnSub, hBtnMul, hBtnDiv, hBtnClear;
HFONT hFontTitle, hFontSub, hFontBody, hFontBold, hFontResult, hFontSmall;
HBRUSH hBrBg, hBrCard, hBrInput;

std::wstring currentResult = L"Chưa có kết quả";
std::wstring calculationDetail = L"Nhập 2 số a, b rồi nhấn nút phép tính";

// Helper for drawing rounded rectangle
void DrawRoundedCard(HDC hdc, RECT rect, COLORREF bgCol, COLORREF borderCol, int radius = 12) {
    HBRUSH hBrush = CreateSolidBrush(bgCol);
    HPEN hPen = CreatePen(PS_SOLID, 1, borderCol);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, radius, radius);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

// Custom Draw Button
void DrawCustomButton(LPDRAWITEMSTRUCT pDIS, const wchar_t* text, COLORREF bgCol, COLORREF textCol, HFONT hFont) {
    HDC hdc = pDIS->hDC;
    RECT rc = pDIS->rcItem;
    bool isPressed = (pDIS->itemState & ODS_SELECTED);

    // Adjust color on click
    if (isPressed) {
        bgCol = RGB((BYTE)(GetRValue(bgCol) * 0.75), (BYTE)(GetGValue(bgCol) * 0.75), (BYTE)(GetBValue(bgCol) * 0.75));
    }

    HBRUSH hBrush = CreateSolidBrush(bgCol);
    HPEN hPen = CreatePen(PS_SOLID, 1, bgCol);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 8, 8);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);

    // Draw text
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textCol);
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    DrawTextW(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);
}

// Perform calculation
void Calculate(char op, HWND hwnd) {
    wchar_t bufA[64] = {0}, bufB[64] = {0};
    GetWindowTextW(hEditA, bufA, 64);
    GetWindowTextW(hEditB, bufB, 64);

    if (wcslen(bufA) == 0 || wcslen(bufB) == 0) {
        currentResult = L"Lỗi: Thiếu dữ liệu";
        calculationDetail = L"Vui lòng nhập đầy đủ cả 2 số a và b!";
        InvalidateRect(hwnd, NULL, TRUE);
        return;
    }

    try {
        double a = std::stod(bufA);
        double b = std::stod(bufB);
        double res = 0;
        std::wstringstream ssDetail, ssResult;

        if (op == '+') {
            res = a + b;
            ssDetail << L"Phép cộng: " << a << L" + " << b;
        } else if (op == '-') {
            res = a - b;
            ssDetail << L"Phép trừ: " << a << L" - " << b;
        } else if (op == '*') {
            res = a * b;
            ssDetail << L"Phép nhân: " << a << L" × " << b;
        } else if (op == '/') {
            if (b == 0) {
                currentResult = L"Lỗi: Chia cho 0";
                calculationDetail = L"Mẫu số b phải khác 0!";
                InvalidateRect(hwnd, NULL, TRUE);
                return;
            }
            res = a / b;
            ssDetail << L"Phép chia: " << a << L" ÷ " << b;
        }

        ssResult << L"= " << res;
        currentResult = ssResult.str();
        calculationDetail = ssDetail.str();
    } catch (...) {
        currentResult = L"Lỗi định dạng";
        calculationDetail = L"Giá trị nhập vào không hợp lệ!";
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Setup fonts (Segoe UI)
            hFontTitle  = CreateFontW(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            hFontSub    = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            hFontBody   = CreateFontW(15, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            hFontBold   = CreateFontW(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            hFontResult = CreateFontW(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            hFontSmall  = CreateFontW(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

            // Setup brushes
            hBrBg    = CreateSolidBrush(COLOR_BG);
            hBrCard  = CreateSolidBrush(COLOR_CARD_BG);
            hBrInput = CreateSolidBrush(COLOR_INPUT_BG);

            // Input A
            hEditA = CreateWindowExW(0, L"EDIT", L"10", 
                WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_CENTER,
                55, 140, 160, 36, hwnd, (HMENU)IDC_EDIT_A, NULL, NULL);
            SendMessage(hEditA, WM_SETFONT, (WPARAM)hFontBold, TRUE);

            // Input B
            hEditB = CreateWindowExW(0, L"EDIT", L"5", 
                WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_CENTER,
                245, 140, 160, 36, hwnd, (HMENU)IDC_EDIT_B, NULL, NULL);
            SendMessage(hEditB, WM_SETFONT, (WPARAM)hFontBold, TRUE);

            // Action Buttons
            hBtnAdd = CreateWindowW(L"BUTTON", L"+ Cộng", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                55, 195, 80, 38, hwnd, (HMENU)IDC_BTN_ADD, NULL, NULL);
            hBtnSub = CreateWindowW(L"BUTTON", L"- Trừ", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                145, 195, 80, 38, hwnd, (HMENU)IDC_BTN_SUB, NULL, NULL);
            hBtnMul = CreateWindowW(L"BUTTON", L"× Nhân", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                235, 195, 80, 38, hwnd, (HMENU)IDC_BTN_MUL, NULL, NULL);
            hBtnDiv = CreateWindowW(L"BUTTON", L"÷ Chia", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                325, 195, 80, 38, hwnd, (HMENU)IDC_BTN_DIV, NULL, NULL);

            // Clear Button
            hBtnClear = CreateWindowW(L"BUTTON", L"↺ Nhập lại", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                55, 365, 350, 32, hwnd, (HMENU)IDC_BTN_CLEAR, NULL, NULL);

            break;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, COLOR_TEXT_PRIMARY);
            SetBkColor(hdcEdit, COLOR_INPUT_BG);
            return (LRESULT)hBrInput;
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
            if (pDIS->CtlID == IDC_BTN_ADD || pDIS->CtlID == IDC_BTN_SUB || 
                pDIS->CtlID == IDC_BTN_MUL || pDIS->CtlID == IDC_BTN_DIV) {
                const wchar_t* btnText = L"";
                if (pDIS->CtlID == IDC_BTN_ADD) btnText = L"+ Cộng";
                if (pDIS->CtlID == IDC_BTN_SUB) btnText = L"- Trừ";
                if (pDIS->CtlID == IDC_BTN_MUL) btnText = L"× Nhân";
                if (pDIS->CtlID == IDC_BTN_DIV) btnText = L"÷ Chia";
                DrawCustomButton(pDIS, btnText, COLOR_ACCENT, COLOR_TEXT_PRIMARY, hFontBold);
                return TRUE;
            }
            if (pDIS->CtlID == IDC_BTN_CLEAR) {
                DrawCustomButton(pDIS, L"↺ Nhập lại từ đầu", RGB(71, 85, 105), COLOR_TEXT_PRIMARY, hFontSub);
                return TRUE;
            }
            break;
        }

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case IDC_BTN_ADD: Calculate('+', hwnd); break;
                case IDC_BTN_SUB: Calculate('-', hwnd); break;
                case IDC_BTN_MUL: Calculate('*', hwnd); break;
                case IDC_BTN_DIV: Calculate('/', hwnd); break;
                case IDC_BTN_CLEAR:
                    SetWindowTextW(hEditA, L"");
                    SetWindowTextW(hEditB, L"");
                    currentResult = L"Chưa có kết quả";
                    calculationDetail = L"Nhập 2 số a, b rồi nhấn nút phép tính";
                    InvalidateRect(hwnd, NULL, TRUE);
                    SetFocus(hEditA);
                    break;
            }
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Double Buffering to eliminate flicker completely
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            // 1. Fill Background
            FillRect(memDC, &clientRect, hBrBg);

            // 2. Header Area
            SetBkMode(memDC, TRANSPARENT);
            
            // PBL2 Badge
            RECT rcBadge = { 35, 20, 160, 42 };
            DrawRoundedCard(memDC, rcBadge, COLOR_BADGE_BG, COLOR_BADGE_BG, 8);
            SetTextColor(memDC, RGB(255, 255, 255));
            SelectObject(memDC, hFontSmall);
            DrawTextW(memDC, L"★ PBL2 C++20 APP", -1, &rcBadge, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Title
            RECT rcTitle = { 35, 48, 430, 78 };
            SetTextColor(memDC, COLOR_TEXT_PRIMARY);
            SelectObject(memDC, hFontTitle);
            DrawTextW(memDC, L"Bộ Tính Toán & Đặt Món", -1, &rcTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            // Subtitle
            RECT rcSub = { 35, 78, 430, 98 };
            SetTextColor(memDC, COLOR_TEXT_MUTED);
            SelectObject(memDC, hFontSub);
            DrawTextW(memDC, L"ĐH Bách Khoa - ĐH Đà Nẵng | Nhóm 25T_DT3", -1, &rcSub, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            // 3. Input Card
            RECT rcCardInputs = { 35, 105, 425, 245 };
            DrawRoundedCard(memDC, rcCardInputs, COLOR_CARD_BG, COLOR_CARD_BORDER, 14);

            // Labels
            SetTextColor(memDC, COLOR_TEXT_MUTED);
            SelectObject(memDC, hFontBody);
            RECT rcLblA = { 55, 118, 215, 138 };
            DrawTextW(memDC, L"Số thứ nhất (a):", -1, &rcLblA, DT_LEFT | DT_SINGLELINE);
            RECT rcLblB = { 245, 118, 405, 138 };
            DrawTextW(memDC, L"Số thứ hai (b):", -1, &rcLblB, DT_LEFT | DT_SINGLELINE);

            // 4. Result Card
            RECT rcCardResult = { 35, 255, 425, 350 };
            DrawRoundedCard(memDC, rcCardResult, COLOR_CARD_BG, COLOR_CARD_BORDER, 14);

            RECT rcDetail = { 50, 268, 410, 290 };
            SetTextColor(memDC, COLOR_TEXT_MUTED);
            SelectObject(memDC, hFontSub);
            DrawTextW(memDC, calculationDetail.c_str(), -1, &rcDetail, DT_CENTER | DT_SINGLELINE);

            RECT rcResVal = { 50, 295, 410, 335 };
            SetTextColor(memDC, COLOR_RESULT_BG);
            SelectObject(memDC, hFontResult);
            DrawTextW(memDC, currentResult.c_str(), -1, &rcResVal, DT_CENTER | DT_SINGLELINE);

            // 5. Footer (Authors)
            RECT rcFooter = { 35, 410, 425, 435 };
            SetTextColor(memDC, COLOR_TEXT_MUTED);
            SelectObject(memDC, hFontSmall);
            DrawTextW(memDC, L"Thành viên: Tô Đông Hải & Lê Văn Tài (25T_DT3)", -1, &rcFooter, DT_CENTER | DT_SINGLELINE);

            // Blit to screen
            BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

            // Cleanup memory DC
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_DESTROY: {
            DeleteObject(hFontTitle);
            DeleteObject(hFontSub);
            DeleteObject(hFontBody);
            DeleteObject(hFontBold);
            DeleteObject(hFontResult);
            DeleteObject(hFontSmall);
            DeleteObject(hBrBg);
            DeleteObject(hBrCard);
            DeleteObject(hBrInput);
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Support both standard main and WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    const wchar_t CLASS_NAME[] = L"PBL2_ModernApp";

    WNDCLASSW wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    RECT wr = { 0, 0, 460, 450 };
    AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"PBL2 - Ứng Dụng Đồ Họa C++20",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

int main() {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}