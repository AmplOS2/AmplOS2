#pragma once
#include <stdint.h>
#include <term_font.h>

class GPU {
private:
        bool     _valid;
        uint32_t _width, _height, pitch;
        uint8_t *buffer;

public:
        constexpr inline bool     valid() const { return _valid; }
        constexpr inline uint32_t width() const { return _width; }
        constexpr inline uint32_t height() const { return _height; }
        inline uint32_t &         pixel(uint32_t x, uint32_t y) const {
                return *(uint32_t *)&buffer[y * pitch + x * 4];
        }
        inline void draw_picture(uint32_t (*f)(uint32_t, uint32_t, uint32_t),
                                 uint32_t width    = 0,
                                 uint32_t height   = 0,
                                 uint32_t offset_x = 0,
                                 uint32_t offset_y = 0) {
                if(width == 0) width = _width;
                if(height == 0) height = _height;
                for(uint32_t y = 0; y < height; y++) {
                        for(uint32_t x = 0; x < width; x++) {
                                auto &p = pixel(x + offset_x, y + offset_y);
                                p       = f(x, y, p);
                        }
                }
        }

        inline void draw_glyph(char c, uint32_t color, uint32_t x, uint32_t y) {
                for(int i = 0; i < 11; i++) {
                        int r = term_font[c * 11 + i];
                        if(r & 0x80) pixel(x + 0, y + i) = color;
                        if(r & 0x40) pixel(x + 1, y + i) = color;
                        if(r & 0x20) pixel(x + 2, y + i) = color;
                        if(r & 0x10) pixel(x + 3, y + i) = color;
                        if(r & 0x08) pixel(x + 4, y + i) = color;
                        if(r & 0x04) pixel(x + 5, y + i) = color;
                        if(r & 0x02) pixel(x + 6, y + i) = color;
                        if(r & 0x01) pixel(x + 7, y + i) = color;
                }
        }

        void draw_text(const char *str, uint32_t color, uint32_t x, uint32_t y) {
                x -= 8;
                auto ox = x;
                while(*str) {
                        if(*str == '\n') y += 11, x = ox;
                        else
                                draw_glyph(*str, color, x += 8, y);
                        str++;
                }
        }

        GPU(uint32_t width = 1024, uint32_t height = 768) noexcept;
};

constexpr inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return r | (g << 8) | (b << 16) | (a << 24);
}
