#ifndef LUOIMON_NHAHANG_H
#define LUOIMON_NHAHANG_H

#include "ChuDe.h"
#include "AnhMon.h"
#include <vector>

// Tin nhan dieu khien luoi the mon (cung dung cho kiem thu tu dong).
const UINT LM_CHONMUC = WM_APP + 101; // wParam: vi tri hien thi; chon va bao LBN_SELCHANGE cho cua so cha
const UINT LM_LAYCHON = WM_APP + 102; // tra ve duLieu cua the dang chon hoac -1
const UINT LM_SOMUC = WM_APP + 103;   // so the dang hien
const UINT LM_CAOTHE = WM_APP + 104;  // chieu cao the hien tai (tu thu gon khi luoi thap)

struct TheMon {
    int duLieu = -1;
    wstring ten, gia, thongTin, huyHieu;
    COLORREF mauHuyHieu = Mau::CHU, nenHuyHieu = Mau::PHU_NEN;
    bool mo = false;
    // Chep thong tin can de ve anh, khong giu con tro toi Mon (thuc don co the duoc nap lai).
    string maMon, loaiMon, tenMon;
};

HWND taoLuoiMon(HWND cha, int id, RECT vung, const BoFont* font, KhoAnh* anh);
void luoiMonDat(HWND luoi, const vector<TheMon>& cacThe, int duLieuChon);

#endif
