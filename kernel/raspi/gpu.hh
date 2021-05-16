#pragma once
#include <stdint.h>
#include <term_font.h>
#include <unifont.h>

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

        inline void draw_glyph(int         c,
                               uint32_t    color,
                               uint32_t    x,
                               uint32_t    y,
                               const char *font,
                               unsigned    glyph_width,
                               unsigned    glyph_height,
                               unsigned    bytes_per_glyph) {
                for(unsigned i = 0; i < glyph_height; i++) {
                        for(unsigned k = 0; k * 8 < glyph_width; k++) {
                                unsigned r = font[bytes_per_glyph * c +
                                                  i * bytes_per_glyph / glyph_height + k];
                                for(unsigned j = 0; j < glyph_width - k * 8; j++)
                                        if(r & (0x80 >> j)) pixel(x + k * 8 + j, y + i) = color;
                        }
                }
        }

        inline void draw_text(const char *str,
                              uint32_t    color,
                              uint32_t    x,
                              uint32_t    y,
                              const char *font,
                              unsigned    glyph_width,
                              unsigned    glyph_height,
                              unsigned    bytes_per_glyph) {
                x -= glyph_width;
                auto ox = x;
                while(*str) {
                        if(*str == '\n') y += glyph_height, x = ox;
                        else
                                draw_glyph(*str,
                                           color,
                                           x += glyph_width,
                                           y,
                                           font,
                                           glyph_width,
                                           glyph_height,
                                           bytes_per_glyph);
                        str++;
                }
        }

        inline void draw_text_term_font(const char *str, uint32_t color, uint32_t x, uint32_t y) {
                draw_text(str,
                          color,
                          x,
                          y,
                          term_font,
                          term_font_glyph_width,
                          term_font_glyph_height,
                          term_font_bytes_per_glyph);
        }

        inline void draw_text_unifont(const char *str, uint32_t color, uint32_t x, uint32_t y) {
                draw_text(str,
                          color,
                          x,
                          y,
                          unifont,
                          unifont_glyph_width,
                          unifont_glyph_height,
                          unifont_bytes_per_glyph);
        }

        GPU(uint32_t width = 1024, uint32_t height = 768) noexcept;
};

constexpr inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return r | (g << 8) | (b << 16) | (a << 24);
}
