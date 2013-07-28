#ifndef TINY_SPRINTF_H
#define TINY_SPRINTF_H

#include <stdarg.h>
#include <stdint.h>

// Supported format specifiers:
//   %s - string
//   %c - char
//   %d - int
//   %[0][<width>]u - uint
//   %[0][<width>]X - uint as hex
//   %A - pair (uint8_t *arr, int len) as hex array


// *sprintf calls return number of characters printed (not counting trailing zero).
// For the sake of simplicity no overflow checks are performed.

int tiny_vsprintf(char *buf, int32_t BufSz, const char *format, va_list args);
//int tiny_sprintf(char *buf, const char *format, ...);

#endif
