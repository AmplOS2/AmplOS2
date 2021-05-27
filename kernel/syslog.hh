#pragma once
#include <stdarg.h>
#include <stdint.h>

// TODO: make portable
#include <raspi/uart0.hh>
#define putc uart_putc

namespace {
enum _syslog_int_format {
        syslog_dec,
        syslog_hex,
};

class _syslog_impl {
private:
        _syslog_int_format int_format;
        inline _syslog_impl(_syslog_int_format i_f) : int_format(i_f) {}

        inline _syslog_impl &putuint(uintmax_t i) {
                char  tmpstr[64];
                char *p = &tmpstr[63];
                *p      = '\0';
                if(int_format == syslog_dec) {
                        do {
                                *(--p) = '0' + (i % 10);
                                i /= 10;
                        } while(i && p > tmpstr);
                } else {
                        do {
                                char n = i & 0xf;
                                *(--p) = n + '0' + (n > 9 ? 7 : 0);
                                i >>= 4;
                        } while(i && p > tmpstr);
                }
                operator<<(p);
                return *this;
        }

public:
        inline _syslog_impl() : int_format(syslog_dec) {}

        inline _syslog_impl &operator<<(char *s) {
                if(!s) s = (char *)"(null)";
                while(*s) putc(*s++);
                return *this;
        }

        inline _syslog_impl &operator<<(const char *s) { return operator<<((char *)s); }

        inline _syslog_impl &operator<<(intmax_t i) {
                if(int_format == syslog_dec) {
                        if(i < 0) operator<<("-");
                        return putuint(i < 0 ? -i : i);
                } else {
                        return putuint((uintmax_t)i);
                }
        }

                inline _syslog_impl &operator<<(void *p) {
                        return operator<<(syslog_hex).putuint((uintptr_t)p);
                }

        inline _syslog_impl operator<<(_syslog_int_format i_f) { return _syslog_impl(i_f); }
};

_syslog_impl syslog;
}
