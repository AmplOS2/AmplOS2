#pragma once
#include <fonts/unifont.psf.h>
#include <printf.hh>
#include <psf.hh>
#include <stdint.h>
#include <utf8.hh>

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

        inline void draw_glyph(uint32_t   c,
                               uint32_t   color,
                               uint32_t   x,
                               uint32_t   y,
                               psf::font &font) {
                uint8_t *glyph = font.glyph(c);
                for(unsigned i = 0; i < font.height; i++) {
                        for(unsigned k = 0; k * 8 < font.width; k++) {
                                unsigned r = glyph[i * font.glyphsize / font.height + k];
                                for(unsigned j = 0; j < font.width - k * 8; j++)
                                        if(r & (0x80 >> j)) pixel(x + k * 8 + j, y + i) = color;
                        }
                }
        }

        inline void draw_text(const char *str,
                              uint32_t    color,
                              uint32_t    x,
                              uint32_t    y,
                              psf::font & font) {
                x -= font.width;
                auto ox = x;
                while(*str) {
                        uint32_t c;
                        int      e;
                        str = (const char *)utf8_decode((const uint8_t *)str, c, e);
                        if(e) {
                                printf("utf8 error: %d\n", e);
                                return;
                        }
                        if(c == '\n') y += font.height, x = ox;
                        else
                                draw_glyph(c, color, x += font.width, y, font);
                }
        }

        inline void draw_text_unifont(const char *str, uint32_t color, uint32_t x, uint32_t y) {
                auto unifont = psf::font((uint8_t *)unifont_psf);
                unifont.parse();
                draw_text(str, color, x, y, unifont);
        }

        GPU(uint32_t width = 1024, uint32_t height = 768);
};

constexpr static inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return r | (g << 8) | (b << 16) | (a << 24);
}
