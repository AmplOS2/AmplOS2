#pragma once
#include <stdint.h>

namespace {
namespace graphics {
enum major {
        row_major,
        column_major,
};
enum pixel_order {
        ARGB,
        RGBA,
        ABGR,
        BGRA,
};
// TODO: support formats with not 32 bit per pixel (like 10-bit color)
template <major m, pixel_order o>
class buffer {
public:
        uint32_t *b;
        uint32_t  w, h;
        constexpr inline buffer(uint32_t *_b, uint32_t _w, uint32_t _h) : b(_b), w(_w), h(_h) {}
        constexpr inline buffer() : b(nullptr), w(0), h(0) {}

        constexpr inline uint32_t &pixel(uint32_t x, uint32_t y) const {
                if(m == row_major) return b[y * w + x];
                if(m == column_major) return b[x * h + y];
                return b[-1];
        }
};
}
}
