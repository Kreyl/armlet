#include "kl_sprintf.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef MIN
#define MIN(a, b)   ( ((a)<(b))? (a) : (b) )
#endif

int32_t kl_vsprintf(ftVoidChar PPutChar, int32_t MaxLength, const char *format, va_list args) {
    int32_t CharCnt = 0, width = 0;
    bool zero_padded = false;
    // Print number n to buffer p in base base. If number is shorter
    // than width, it's prepended with spaces or zeros (if zero_padded
    // is set) from the left.
    void IPutUint(uint32_t n, uint32_t base) {
        char digits[10];
        int32_t len = 0;
        // Place digits to buffer
        do {
            int32_t digit = n % base;
            n /= base;
            digits[len++] = (digit < 10)? '0'+digit : 'A'+digit-10;
        } while(n > 0);
        // Add padding
        for(int32_t i = len; i < width; i++) {
            if(CharCnt >= MaxLength) return;
            if(zero_padded) PPutChar('0');
            else            PPutChar(' ');
            CharCnt++;
        }
        // Print digits
        int32_t Cnt = MIN(len, (MaxLength - CharCnt));
        CharCnt += Cnt;
        while(Cnt-- > 0) PPutChar(digits[--len]);
    } // IPutUint

    const char *f = format;
    char c;
    while((c = *f++) != 0) {
        if(c != '%') {
            if(CharCnt >= MaxLength) return CharCnt;
            PPutChar(c);
            CharCnt++;
        }
        else {
            // Here goes optional width specification.
            // If it starts with zero (zero_padded is true), it means we use '0' instead of ' ' as a filler.
            width = 0;
            zero_padded = false;
            while(true) {
                c = *f++;
                if(c < '0' || c > '9') break;
                if((width == 0) && (c == '0')) zero_padded = true;
                width *= 10;
                width += c-'0';
            }

            // Check if s, S, c, X, u, d, A
            if((c == 's') || (c == 'S')) {
                char *s = va_arg(args, char*);
                while(*s != 0) {
                    if(CharCnt >= MaxLength) return CharCnt;
                    PPutChar(*s++);
                    CharCnt++;
                }
            }
            else if(c == 'c') {
                if(CharCnt >= MaxLength) return CharCnt;
                PPutChar(va_arg(args, int));
                CharCnt++;
            }
            else if(c == 'X') IPutUint(va_arg(args, uint32_t), 16);
            else if(c == 'u') IPutUint(va_arg(args, uint32_t), 10);
            else if(c == 'd') {
                int32_t n = va_arg(args, int32_t);
                if(n < 0) {
                    if(CharCnt >= MaxLength) return CharCnt;
                    PPutChar('-');
                    CharCnt++;
                    n = -n;
                }
                IPutUint(n, 10);
            }
            else if(c == 'A') {
                uint8_t *arr = va_arg(args, uint8_t*);
                int32_t n = va_arg(args, int32_t);
                int32_t Delimiter = va_arg(args, int32_t);
                zero_padded = true; // Every byte consists of two characters.
                width = 2;          // ...01 02 0A etc.; not 1 2 A
                for(int32_t i = 0; i < n; i++) {
                    if((i > 0) && (Delimiter != 0)) {   // Do not place delimiter before or after array
                        if(CharCnt >= MaxLength) return CharCnt;
                        PPutChar((char)Delimiter);
                        CharCnt++;
                    }
                    IPutUint(arr[i], 16);
                    if(CharCnt >= MaxLength) return CharCnt;
                } // for i
            } // if 'A'
            if(CharCnt >= MaxLength) return CharCnt;
        } // if c != '%'
    } // while
    return CharCnt;
}
