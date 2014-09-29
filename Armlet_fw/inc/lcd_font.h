/*
 * File:   lcd_font.h
 * Author: Kreyl Laurelindo
 *
 * Created on 27 Сентябрь 2010 г., 23:36
 */

#ifndef LCD_FONT_H
#define	LCD_FONT_H

#include <inttypes.h>

const char Font_6x8_Data[256*6] = {
0x08, 0x06, 0x06, 0x00, 0x00, 0x00,   // 0x00 - columns, rows, num_bytes_per_char
0x00, 0x7c, 0xa2, 0x8a, 0xa2, 0x7c,   // 0x01 - ���� � �������.
0x00, 0x7c, 0xd6, 0xf6, 0xd6, 0x7c,   // 0x02 - ���� � ������� �����������.
0x00, 0x38, 0x7c, 0x3e, 0x7c, 0x38,   // 0x03 - ����� (������).
0x00, 0x18, 0x3c, 0x7e, 0x3c, 0x18,   // 0x04 - �����.
0x00, 0x0c, 0x6c, 0xfe, 0x6c, 0x0c,   // 0x05 - ������.
0x00, 0x18, 0x3a, 0x7e, 0x3a, 0x18,   // 0x06 - ����.
0x00, 0x00, 0x18, 0x18, 0x00, 0x00,   // 0x07 - ����������� ���� �� ������.
0xff, 0xff, 0xe7, 0xe7, 0xff, 0xff,   // 0x08 - ����������� ���� �� ������ � ��������.
0x00, 0x3c, 0x24, 0x24, 0x3c, 0x00,   // 0x09 - ������������� ���� �� ������.
0xff, 0xc3, 0xdb, 0xdb, 0xc3, 0xff,   // 0x0a - ������������� ���� �� ������ � �������� (������).
0x00, 0x0c, 0x12, 0x52, 0x6c, 0x70,   // 0x0b - ������� ������ (���� �� �������� �����).
0x00, 0x60, 0x94, 0x9e, 0x94, 0x60,   // 0x0c - ������� ������ (���� � ������� �����).
0x00, 0x06, 0x0e, 0xfc, 0x40, 0x20,   // 0x0d - ���� I.
0x00, 0x06, 0x7e, 0x50, 0xac, 0xfc,  // 0x0e - ���� II.
0x00, 0x54, 0x38, 0x6c, 0x38, 0x54, // 0x0f - ������ (���� � ������������� ������).
0x00, 0x00, 0xfe, 0x7c, 0x38, 0x10, // 0x10 - ������� ������� ������.
0x00, 0x10, 0x38, 0x7c, 0xfe, 0x00,  // 0x11 - ������� ������� �����.
0x00, 0x28, 0x6c, 0xfe, 0x6c, 0x28, // 0x12 - ������ ������� �����-����.
0x00, 0x00, 0xfa, 0x00, 0xfa, 0x00,  // 0x13 - ��� ��������������� �����.
0x00, 0x60, 0x90, 0xfe, 0x80, 0xfe, // 0x14 - ������ "��".
0x00, 0x44, 0xb2, 0xaa, 0x9a, 0x44, // 0x15 - ������ ���������.
0x00, 0x06, 0x06, 0x06, 0x06, 0x00, // 0x16 - ������� �������������.
0x00, 0x28, 0x6d, 0xff, 0x6d, 0x28, // 0x17 - ������ ������� �����-���� � ��������������.
0x00, 0x20, 0x60, 0xfe, 0x60, 0x20, // 0x18 - ������ ������� �����.
0x00, 0x08, 0x0c, 0xfe, 0x0c, 0x08, // 0x19 - ������ ������� ����.
0x00, 0x10, 0x10, 0x7c, 0x38, 0x10, // 0x1a - ������ ������� ������.
0x00, 0x10, 0x38, 0x7c, 0x10, 0x10, // 0x1b - ������ ������� �����.
0x00, 0x1e, 0x02, 0x02, 0x02, 0x02, // 0x1c - ������ �������.
0x00, 0x10, 0x7c, 0x10, 0x7c, 0x10, // 0x1d - ������ ������� �����-������.
0x00, 0x0c, 0x3c, 0xfc, 0x3c, 0x0c, // 0x1e - ������� ������� �����.
0x00, 0xc0, 0xf0, 0xfc, 0xf0, 0xc0, // 0x1f - ������� ������� ����.
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x20 - ������ (������ ����������).
0x00, 0x00, 0x60, 0xfa, 0x60, 0x00, // 0x21 - ��������������� ����.
0x00, 0xe0, 0xc0, 0x00, 0xe0, 0xc0, // 0x22 - ������� �������.
0x00, 0x24, 0x7e, 0x24, 0x7e, 0x24, // 0x23 - �������.
0x00, 0x24, 0xd4, 0x56, 0x48, 0x00, // 0x24 - ������.
0x00, 0xc6, 0xc8, 0x10, 0x26, 0xc6, // 0x25 - �������.
0x00, 0x6c, 0x92, 0x6a, 0x04, 0x0a, // 0x26 - ���������.
0x00, 0x00, 0xe0, 0xc0, 0x00, 0x00, // 0x27 - ��������.
0x00, 0x00, 0x7c, 0x82, 0x00, 0x00, // 0x28 - ����������� ������.
0x00, 0x00, 0x82, 0x7c, 0x00, 0x00, // 0x29 - ����������� ������.
0x00, 0x10, 0x7c, 0x38, 0x7c, 0x10, // 0x2a - ��������� (���������).
0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, // 0x2b - ����.
0x00, 0x00, 0x07, 0x06, 0x00, 0x00, // 0x2c - �������.
0x00, 0x10, 0x10, 0x10, 0x10, 0x10, // 0x2d - ����.
0x00, 0x00, 0x06, 0x06, 0x00, 0x00, // 0x2e - �����.
0x00, 0x04, 0x08, 0x10, 0x20, 0x40, // 0x2f - ���� �����-������� ('/').
0x00, 0x7c, 0x82, 0x82, 0x82, 0x7c, // 0x30 - '0'.
0x00, 0x00, 0x42, 0xfe, 0x02, 0x00, // 0x31 - '1'.
0x00, 0x46, 0x8a, 0x92, 0x92, 0x62, // 0x32 - '2'.
0x00, 0x44, 0x92, 0x92, 0x92, 0x6c, // 0x33 - '3'.
0x00, 0x18, 0x28, 0x48, 0xfe, 0x08, // 0x34 - '4'.
0x00, 0xf4, 0x92, 0x92, 0x92, 0x8c, // 0x35 - '5'.
0x00, 0x3c, 0x52, 0x92, 0x92, 0x0c, // 0x36 - '6'.

0x00, 0x80, 0x8e, 0x90, 0xa0, 0xc0, // 0x37 - '7'.

0x00, 0x6c, 0x92, 0x92, 0x92, 0x6c, // 0x38 - '8'.
// 0x39 - '9'.
0x00, 0x60, 0x92, 0x92, 0x94, 0x78,
// 0x3a - ���������.
0x00, 0x00, 0x36, 0x36, 0x00, 0x00,
// 0x3b - ����� � �������.
0x00, 0x00, 0x37, 0x36, 0x00, 0x00,
// 0x3c - ������.
0x00, 0x10, 0x28, 0x44, 0x82, 0x00,
// 0x3d - �����.
0x00, 0x24, 0x24, 0x24, 0x24, 0x24,
// 0x3e - ������.
0x00, 0x00, 0x82, 0x44, 0x28, 0x10,
// 0x3f - �������������� ����.
0x00, 0x40, 0x80, 0x9a, 0x90, 0x60,
// 0x40 - "������" ('@').
0x00, 0x7c, 0x82, 0xba, 0xaa, 0x78,
// 0x41 - 'A'.
0x00, 0x7e, 0x88, 0x88, 0x88, 0x7e,
// 0x42 - 'B'.
0x00, 0xfe, 0x92, 0x92, 0x92, 0x6c,
// 0x43 - 'C'.
0x00, 0x7c, 0x82, 0x82, 0x82, 0x44,
// 0x44 - 'D'.
0x00, 0xfe, 0x82, 0x82, 0x82, 0x7c,
// 0x45 - 'E'.
0x00, 0xfe, 0x92, 0x92, 0x92, 0x82,
// 0x46 - 'F'.
0x00, 0xfe, 0x90, 0x90, 0x90, 0x80,
// 0x47 - 'G'.
0x00, 0x7c, 0x82, 0x92, 0x92, 0x5e,
// 0x48 - 'H'.
0x00, 0xfe, 0x10, 0x10, 0x10, 0xfe,
// 0x49 - 'I'.
0x00, 0x00, 0x82, 0xfe, 0x82, 0x00,
// 0x4a - 'J'.
0x00, 0x0c, 0x02, 0x02, 0x02, 0xfc,
// 0x4b - 'K'.
0x00, 0xfe, 0x10, 0x28, 0x44, 0x82,
// 0x4c - 'L'.
0x00, 0xfe, 0x02, 0x02, 0x02, 0x02,
// 0x4d - 'M'.
0x00, 0xfe, 0x40, 0x20, 0x40, 0xfe,
// 0x4e - 'N'.
0x00, 0xfe, 0x40, 0x20, 0x10, 0xfe,
// 0x4f - 'O'.
0x00, 0x7c, 0x82, 0x82, 0x82, 0x7c,
// 0x50 - 'P'.
0x00, 0xfe, 0x90, 0x90, 0x90, 0x60,
// 0x51 - 'Q'.
0x00, 0x7c, 0x82, 0x8a, 0x84, 0x7a,
// 0x52 - 'R'.
0x00, 0xfe, 0x90, 0x90, 0x98, 0x66,
// 0x53 - 'S'.
0x00, 0x64, 0x92, 0x92, 0x92, 0x4c,
// 0x54 - 'T'.
0x00, 0x80, 0x80, 0xfe, 0x80, 0x80,
// 0x55 - 'U'.
0x00, 0xfc, 0x02, 0x02, 0x02, 0xfc,
// 0x56 - 'V'.
0x00, 0xf8, 0x04, 0x02, 0x04, 0xf8,
// 0x57 - 'W'.
0x00, 0xfc, 0x02, 0x3c, 0x02, 0xfc,
// 0x58 - 'X'.
0x00, 0xc6, 0x28, 0x10, 0x28, 0xc6,
// 0x59 - 'Y'.
0x00, 0xe0, 0x10, 0x0e, 0x10, 0xe0,
// 0x5a - 'Z'.
0x00, 0x8e, 0x92, 0xa2, 0xc2, 0x00,
// 0x5b - '['.
0x00, 0x00, 0xfe, 0x82, 0x82, 0x00,
// 0x5c - '\'.
0x00, 0x40, 0x20, 0x10, 0x08, 0x04,
// 0x5d - ']'.
0x00, 0x00, 0x82, 0x82, 0xfe, 0x00,
// 0x5e - '^'.
0x00, 0x20, 0x40, 0x80, 0x40, 0x20,
// 0x5f - '_'.
0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
// 0x60 - �������� ��������.
0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00,
// 0x61 - 'a'.
0x00, 0x04, 0x2a, 0x2a, 0x2a, 0x1e,
// 0x62 - 'b'.
0x00, 0xfe, 0x22, 0x22, 0x22, 0x1c,
// 0x63 - 'c'.
0x00, 0x1c, 0x22, 0x22, 0x22, 0x14,
// 0x64 - 'd'.
0x00, 0x1c, 0x22, 0x22, 0x22, 0xfe,
// 0x65 - 'e'.
0x00, 0x1c, 0x2a, 0x2a, 0x2a, 0x10,
// 0x66 - 'f'.
0x00, 0x10, 0x7e, 0x90, 0x90, 0x00,
// 0x67 - 'g'.
0x00, 0x18, 0x25, 0x25, 0x25, 0x3e,
// 0x68 - 'h'.
0x00, 0xfe, 0x20, 0x20, 0x1e, 0x00,
// 0x69 - 'i'.
0x00, 0x00, 0x00, 0xbe, 0x02, 0x00,
// 0x6a - 'j'.
0x00, 0x02, 0x01, 0x21, 0xbe, 0x00,
// 0x6b - 'k'.
0x00, 0xfe, 0x08, 0x14, 0x22, 0x00,
// 0x6c - 'l'.
0x00, 0x00, 0x00, 0xfe, 0x02, 0x00,
// 0x6d - 'm'.
0x00, 0x3e, 0x20, 0x18, 0x20, 0x1e,
// 0x6e - 'n'.
0x00, 0x3e, 0x20, 0x20, 0x1e, 0x00,
// 0x6f - 'o'.
0x00, 0x1c, 0x22, 0x22, 0x22, 0x1c,
// 0x70 - 'p'.
0x00, 0x3f, 0x22, 0x22, 0x22, 0x1c,
// 0x71 - 'q'.
0x00, 0x1c, 0x22, 0x22, 0x22, 0x3f,
// 0x72 - 'r'.
0x00, 0x22, 0x1e, 0x22, 0x20, 0x10,
// 0x73 - 's'.
0x00, 0x10, 0x2a, 0x2a, 0x2a, 0x04,
// 0x74 - 't'.
0x00, 0x20, 0x7c, 0x22, 0x24, 0x00,
// 0x75 - 'u'.
0x00, 0x3c, 0x02, 0x04, 0x3e, 0x00,
// 0x76 - 'v'.
0x00, 0x38, 0x04, 0x02, 0x04, 0x38,
// 0x77 - 'w'.
0x00, 0x3c, 0x06, 0x0c, 0x06, 0x3c,
// 0x78 - 'x'.
0x00, 0x36, 0x08, 0x08, 0x36, 0x00,
// 0x79 - 'y'.
0x00, 0x39, 0x05, 0x06, 0x3c, 0x00,
// 0x7a - 'z'.
0x00, 0x26, 0x2a, 0x2a, 0x32, 0x00,
// 0x7b - '{'.
0x00, 0x10, 0x7c, 0x82, 0x82, 0x00,
// 0x7c - '|'.
0x00, 0x00, 0x00, 0xee, 0x00, 0x00,
// 0x7d - '}'.
0x00, 0x00, 0x82, 0x82, 0x7c, 0x10,
// 0x7e - '~'.
0x00, 0x40, 0x80, 0x40, 0x80, 0x00,
// 0x7f - "�����".
0x00, 0x3c, 0x64, 0xc4, 0x64, 0x3c,
// 0x80 - ����� �� ����� �����������.
0x22, 0x88, 0x22, 0x88, 0x22, 0x88,
// 0x81 - ����� �� ����� ������.
0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa,
// 0x82 - ����� �� ��������.
0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77,
// 0x83 - ������� ����� ������������� - ������������ �����.
0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
// 0x84 - ������������� - ������������ ����� � ������� �� ������ �����.
0x10, 0x10, 0x10, 0xff, 0x00, 0x00,
// 0x85 - ������������� - ������������ ����� � ������� ������� �� ������ �����.
0x50, 0x50, 0x50, 0xff, 0x00, 0x00,
// 0x86 - ������������� - ������� ������������ ����� � ������� �� ������ �����.
0x10, 0xff, 0x00, 0xff, 0x00, 0x00,
// 0x87 - ������������� - ������� ������ ���� � ������� ������������ ������.
0x10, 0x1f, 0x10, 0x1f, 0x00, 0x00,
// 0x88 - ������������� - ������� ������ ���� � ������� �������������� ������.
0x50, 0x50, 0x50, 0x7f, 0x00, 0x00,
// 0x89 - ������������� - ������� ������������ ����� � ������� ������� �� ������ �����.
0x50, 0xdf, 0x00, 0xff, 0x00, 0x00,
// 0x8a - ������������� - ������� ������������ �����.
0x00, 0xff, 0x00, 0xff, 0x00, 0x00,
// 0x8b - ������������� - ������� ������� ������ ����.
0x50, 0x5f, 0x40, 0x7f, 0x00, 0x00,
// 0x8c - ������������� - ������� ������ ������ ����.
0x50, 0xd0, 0x10, 0xf0, 0x00, 0x00,
// 0x8d - ������������� - ������ ������ ���� � ������� ������������ ������.
0x10, 0xf0, 0x10, 0xf0, 0x00, 0x00,
// 0x8e - ������������� - ������ ������ ���� � ������� �������������� ������.
0x50, 0x50, 0x50, 0xf0, 0x00, 0x00,
// 0x8f - ������������� - ������� ������ ����.
0x10, 0x10, 0x10, 0x1f, 0x00, 0x00,
// 0x90 - ������������� - ������ ����� ����.
0x00, 0x00, 0x00, 0xf0, 0x10, 0x10,
// 0x91 - ������������� - �������������� ����� � ������� �� ������ �����.
0x10, 0x10, 0x10, 0xf0, 0x10, 0x10,
// 0x92 - ������������� - �������������� ����� � ������� �� ������ ����.
0x10, 0x10, 0x10, 0x1f, 0x10, 0x10,
// 0x93 - ������������� - ������������ ����� � ������� �� ������ ������.
0x00, 0x00, 0x00, 0xff, 0x10, 0x10,
// 0x94 - ������������� - �������������� ����� �� ������.
0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
// 0x95 - ������������� - �����������.
0x10, 0x10, 0x10, 0xff, 0x10, 0x10,
// 0x96 - ������������� - ������������ ����� � ������� ������� �� ������ ������.
0x00, 0x00, 0x00, 0xff, 0x50, 0x50,
// 0x97 - ������������� - ������� ������������ ����� � ������� �� ������ ������.
0x00, 0xff, 0x00, 0xff, 0x10, 0x10,
// 0x98 - ������������� - ������� ������ ����� ����.
0x00, 0xf0, 0x10, 0xd0, 0x50, 0x50,
// 0x99 - ������������� - ������� ������� ����� ����.
0x00, 0x7f, 0x40, 0x5f, 0x50, 0x50,
// 0x9a - ������������� - ������� �������������� ����� � ������� ������� �� ������ �����.
0x50, 0xd0, 0x10, 0xd0, 0x50, 0x50,
// 0x9b - ������������� - ������� �������������� ����� � ������� ������� �� ������ ����.
0x50, 0x5f, 0x40, 0x5f, 0x50, 0x50,
// 0x9c - ������������� - ������� ������������ ����� � ������� ������� �� ������ ������.
0x00, 0xff, 0x00, 0xdf, 0x50, 0x50,
// 0x9d - ������������� - ������� �������������� ����� �� ������.
0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
// 0x9e - ������������� - ������� �����������.
0x50, 0xdf, 0x00, 0xdf, 0x50, 0x50,
// 0x9f - ������������� - ������� �������������� ����� � ������� �� ������ �����.
0x50, 0x50, 0x50, 0xd0, 0x50, 0x50,
// 0xa0 - ������������� - �������������� ����� � ������� ������� �� ������ �����.
0x10, 0xf0, 0x10, 0xf0, 0x10, 0x10,
// 0xa1 - ������������� - ������� �������������� ����� � ������� �� ������ ����.
0x50, 0x50, 0x50, 0x5f, 0x50, 0x50,
// 0xa2 - ������������� - �������������� ����� � ������� ������� �� ������ ����.
0x10, 0x1f, 0x10, 0x1f, 0x10, 0x10,
// 0xa3 - ������������� - ������ ����� ����, ������� ������������ �����.
0x00, 0xf0, 0x10, 0xf0, 0x10, 0x10,
// 0xa4 - ������������� - ������ ����� ����, ������� �������������� �����.
0x00, 0x00, 0x00, 0xf0, 0x50, 0x50,
// 0xa5 - ������������� - ������� ����� ����, ������� �������������� �����.
0x00, 0x00, 0x00, 0x7f, 0x50, 0x50,
// 0xa6 - ������������� - ������� ����� ����, ������� ������������ �����.
0x00, 0x1f, 0x10, 0x1f, 0x10, 0x10,
// 0xa7 - ������������� - ������� ������������ ����� �� ������ � �������� ������ � �����.
0x10, 0xff, 0x10, 0xff, 0x10, 0x10,
// 0xa8 - '�'.
0x00, 0x7e, 0xd2, 0x52, 0xd2, 0x42,
// 0xa9 - ������������� - ������� �������������� ����� �� ������ � �������� ����� � ����.
0x50, 0x50, 0x50, 0xff, 0x50, 0x50,
// 0xaa - ������������� - ������ ������ ����.
0x10, 0x10, 0x10, 0xf0, 0x00, 0x00,
// 0xab - ������������� - ������� ����� ����.
0x00, 0x00, 0x00, 0x1f, 0x10, 0x10,
// 0xac - ������������� - ����������� ����������.
0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
// 0xad - ������������� - ����������� ������ �������� ����������.
0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
// 0xae - ������������� - ����������� ����� �������� ����������.
0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
// 0xaf - ������������� - ����������� ������ �������� ����������.
0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
// 0xb0 - ������������� - ����������� ������� �������� ����������.
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
// 0xb1 - ���������� '�'.
0x00, 0x7c, 0x92, 0x92, 0x82, 0x44,
// 0xb2 - ���������� '�'.
0x00, 0x1c, 0x2a, 0x2a, 0x22, 0x14,
// 0xb3 - 'I' � ����� ������� ������.
0x00, 0x80, 0x02, 0x7e, 0x02, 0x80,
// 0xb4 - 'i' � ����� ������� ������.
0x00, 0x00, 0x80, 0x3e, 0x82, 0x00,
// 0xb5 - 'Y' � ������� ������ ('~').
0x00, 0xe4, 0x12, 0xd2, 0x12, 0xfc,
// 0xb6 - 'y' � ������� ������ ('~').
0x00, 0xb9, 0x45, 0x46, 0xbc, 0x00,
// 0xb7 - ��������� ������ ������.
0x00, 0x60, 0x90, 0x90, 0x60, 0x00,
// 0xb8 - '�'.
0x00, 0x1c, 0xaa, 0x2a, 0xaa, 0x10,
// 0xb9 - ������� ����������� ���� �� ������.
0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
// 0xba - ��������� ����������� ���� �� ������.
0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
// 0xbb - ������ ����� �����������.
0x00, 0x0c, 0x02, 0x7c, 0x40, 0x40,
// 0xbc - '�'.
0xfe, 0x60, 0x18, 0xfe, 0xc8, 0xc8,
// 0xbd - "������".
0x54, 0x7c, 0x28, 0x28, 0x7c, 0x54,
// 0xbe - ����������� ������� �� ������.
0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00,
// 0xbf - ������ ����������.
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0xc0 - '�'.
0x00, 0x7e, 0x88, 0x88, 0x88, 0x7e,
// 0xc1 - '�'.
0x00, 0xfe, 0x92, 0x92, 0x92, 0x8c,
// 0xc2 - '�'.
0x00, 0xfe, 0x92, 0x92, 0x92, 0x6c,
// 0xc3 - '�'.
0x00, 0xfe, 0x80, 0x80, 0x80, 0x80,
// 0xc4 - '�'.
0x03, 0x7e, 0x82, 0x82, 0xfe, 0x03,
// 0xc5 - '�'.
0x00, 0xfe, 0x92, 0x92, 0x92, 0x82,
// 0xc6 - '�'.
0x00, 0xee, 0x10, 0xfe, 0x10, 0xee,
// 0xc7 - '�'.
0x44, 0x92, 0x92, 0x92, 0x6c, 0x00,
// 0xc8 - '�'.
0x00, 0xfe, 0x04, 0x08, 0x10, 0xfe,
// 0xc9 - '�'.
0x00, 0x7e, 0x84, 0x88, 0x90, 0x7e,
// 0xca - '�'.
0x00, 0xfe, 0x10, 0x28, 0x44, 0x82,
// 0xcb - '�'.
0x00, 0x02, 0x7e, 0x80, 0x80, 0xfe,
// 0xcc - '�'.
0x00, 0xfe, 0x40, 0x20, 0x40, 0xfe,
// 0xcd - '�'.
0x00, 0xfe, 0x10, 0x10, 0x10, 0xfe,
// 0xce - '�'.
0x00, 0x7c, 0x82, 0x82, 0x82, 0x7c,
// 0xcf - '�'.
0x00, 0xfe, 0x80, 0x80, 0x80, 0xfe,
// 0xd0 - '�'.
0x00, 0xfe, 0x90, 0x90, 0x90, 0x60,
// 0xd1 - '�'.
0x00, 0x7c, 0x82, 0x82, 0x82, 0x44,
// 0xd2 - '�'.
0x00, 0x80, 0x80, 0xfe, 0x80, 0x80,
// 0xd3 - '�'.
0x00, 0xe4, 0x12, 0x12, 0x12, 0xfc,
// 0xd4 - '�'.
0x00, 0x70, 0x88, 0xfe, 0x88, 0x70,
// 0xd5 - '�'.
0x00, 0xc6, 0x28, 0x10, 0x28, 0xc6,
// 0xd6 - '�'.
0x00, 0xfe, 0x02, 0x02, 0xfe, 0x03,
// 0xd7 - '�'.
0x00, 0xe0, 0x10, 0x10, 0x10, 0xfe,
// 0xd8 - '�'.
0x00, 0xfe, 0x02, 0xfe, 0x02, 0xfe,
// 0xd9 - '�'.
0x00, 0xfe, 0x02, 0xfe, 0x02, 0xff,
// 0xda - '�'.
0xc0, 0x80, 0xfe, 0x12, 0x12, 0x0c,
// 0xdb - '�'.
0x00, 0xfe, 0x12, 0x12, 0x0c, 0xfe,
// 0xdc - '�'.
0x00, 0xfe, 0x12, 0x12, 0x12, 0x0c,
// 0xdd - '�'.
0x00, 0x44, 0x82, 0x92, 0x92, 0x7c,
// 0xde - '�'.
0x00, 0xfe, 0x10, 0x7c, 0x82, 0x7c,
// 0xdf - '�'.
0x00, 0x66, 0x98, 0x90, 0x90, 0xfe,
// 0xe0 - '�'.
0x00, 0x04, 0x2a, 0x2a, 0x2a, 0x1e,
// 0xe1 - '�'.
0x00, 0x3c, 0x52, 0x52, 0x52, 0x8c,
// 0xe2 - '�'.
0x00, 0x3e, 0x2a, 0x2a, 0x2a, 0x14,
// 0xe3 - '�'.
0x00, 0x3e, 0x20, 0x20, 0x30, 0x00,
// 0xe4 - '�'.
0x03, 0x1e, 0x22, 0x22, 0x3e, 0x03,
// 0xe5 - '�'.
0x00, 0x1c, 0x2a, 0x2a, 0x2a, 0x10,
// 0xe6 - '�'.
0x00, 0x36, 0x08, 0x3e, 0x08, 0x36,
// 0xe7 - '�'.
0x00, 0x14, 0x22, 0x2a, 0x2a, 0x14,
// 0xe8 - '�'.
0x00, 0x3e, 0x04, 0x08, 0x10, 0x3e,
// 0xe9 - '�'.
0x00, 0x3e, 0x04, 0x48, 0x50, 0x3e,
// 0xea - '�'.
0x00, 0x3e, 0x08, 0x14, 0x22, 0x00,
// 0xeb - '�'.
0x02, 0x1c, 0x20, 0x20, 0x3e, 0x00,
// 0xec - '�'.
0x00, 0x3e, 0x10, 0x08, 0x10, 0x3e,
// 0xed - '�'.
0x00, 0x3e, 0x08, 0x08, 0x08, 0x3e,
// 0xee - '�'.
0x00, 0x1c, 0x22, 0x22, 0x22, 0x1c,
// 0xef - '�'.
0x00, 0x3e, 0x20, 0x20, 0x20, 0x3e,
// 0xf0 - '�'.
0x00, 0x3f, 0x22, 0x22, 0x22, 0x1c,

0x00, 0x1c, 0x22, 0x22, 0x22, 0x14, // 0xf1 - '�'.

0x00, 0x20, 0x20, 0x3e, 0x20, 0x20, // 0xf2 - '�'.

0x00, 0x39, 0x05, 0x06, 0x3c, 0x00, // 0xf3 - '�'.

0x00, 0x18, 0x24, 0x3e, 0x24, 0x18, // 0xf4 - '�'.

0x00, 0x36, 0x08, 0x08, 0x36, 0x00, // 0xf5 - '�'.

0x00, 0x3e, 0x02, 0x02, 0x3e, 0x03, // 0xf6 - '�'.

0x00, 0x30, 0x08, 0x08, 0x08, 0x3e, // 0xf7 - '�'.

0x00, 0x3e, 0x02, 0x3e, 0x02, 0x3e, // 0xf8 - '�'.

0x00, 0x3e, 0x02, 0x3e, 0x02, 0x3f, // 0xf9 - '�'.

0x30, 0x20, 0x3e, 0x0a, 0x0a, 0x04, // 0xfa - '�'.

0x00, 0x3e, 0x0a, 0x0a, 0x04, 0x3e, // 0xfb - '�'.

0x00, 0x3e, 0x0a, 0x0a, 0x0a, 0x04, // 0xfc - '�'.

0x00, 0x14, 0x22, 0x2a, 0x2a, 0x1c, // 0xfd - '�'.

0x00, 0x3e, 0x08, 0x1c, 0x22, 0x1c, // 0xfe - '�'.

0x00, 0x12, 0x2c, 0x28, 0x28, 0x3e // 0xff - '�'.
};



#endif	/* LCD_FONT_H */

