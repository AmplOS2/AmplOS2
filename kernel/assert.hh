#pragma once

#ifdef ANOS_NODEBUG
#define assert(expr)
#else
#include <abort.hh>
#include <syslog.hh>
namespace {
inline void _assert_fail(const char *ass, const char *file, unsigned line, const char *func) {
        syslog << "anOS: " << file << ":" << line << ": ";
        if(func) syslog << func << ": ";
        syslog << "Assertion '" << ass << "' failed.\n";
        abort();
}
}

#ifdef __PRETTY_FUNCTION__
// __PRETTY_FUNCTION__ is a GNU extension, which is a bit nicer than
// C's __func__.
#define ASSERT_FUNCTION __extension__ __PRETTY_FUNCTION__
#else
#define ASSERT_FUNCTION __func__
#endif

#define assert(expr)                         \
        (static_cast<bool>(expr) ? (void)(0) \
                                 : _assert_fail(#expr, __FILE__, __LINE__, ASSERT_FUNCTION))
#endif
