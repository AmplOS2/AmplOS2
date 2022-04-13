#pragma once
#include <fonts/unifont.psf.h>
#include <graphics.hh>
#include <kalloc.hh>
#include <psf.hh>
#include <raspi/mbox.hh>
#include <raspi/uart0.hh>
#include <stdint.h>
#include <syslog.hh>
#include <utf8.hh>

namespace {
class GPU {
private:
        bool                                                  _valid;
        graphics::buffer<graphics::row_major, graphics::RGBA> front_buffer, back_buffer;

public:
        constexpr inline bool     valid() const { return _valid; }
        constexpr inline uint32_t width() const { return front_buffer.w; }
        constexpr inline uint32_t height() const { return front_buffer.h; }
        inline uint32_t &pixel(uint32_t x, uint32_t y) const { return back_buffer.pixel(x, y); }
        inline void      draw_picture(uint32_t (*f)(uint32_t, uint32_t, uint32_t),
                                      uint32_t width    = 0,
                                      uint32_t height   = 0,
                                      uint32_t offset_x = 0,
                                      uint32_t offset_y = 0) {
                if(width == 0) width = this->width();
                if(height == 0) height = this->height();
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
                uint8_t *glyph = font[c];
                for(unsigned i = 0; i < font.height; i++) {
                        for(unsigned k = 0; k * 8 < font.width; k++) {
                                unsigned r = glyph[i * font.bytesperglyph / font.height + k];
                                for(unsigned j = 0; j < font.width - k * 8; j++)
                                        if(r & (0x80 >> j)) pixel(x + k * 8 + j, y + i) = color;
                        }
                }
        }

        inline void draw_text(const char *str,
                              uint32_t    color,
                              uint32_t    x,
                              uint32_t    y,
                              psf::font  &font) {
                x -= font.width;
                auto ox = x;
                while(*str) {
                        uint32_t c;
                        int      e;
                        str = (const char *)utf8_decode((const uint8_t *)str, c, e);
                        if(e) {
                                syslog << "utf8 error: " << e << "\n";
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

        // currently this just copies, which is not at all how you *should* do it
        inline void swap_buffers() { back_buffer.copy_to(front_buffer); }

        GPU(uint32_t width = 1024, uint32_t height = 768) {
                _valid = false;

                size_t idx = mbox_start(35);
                mbox_cmd(idx, 0x48003, 2, width, height); // set physical width & height
                mbox_cmd(idx, 0x48004, 2, width, height); // set virtual width & height
                mbox_cmd(idx, 0x48009, 2, 0, 0);          // set virtual offset
                mbox_cmd(idx, 0x48005, 1, 32);            // set depth
                mbox_cmd(idx, 0x48006, 1, 1);             // set pixel order
                mbox_cmd(idx, 0x40001, 2, 4096, 0); // get framebuffer & alignment (pointer, size)
                mbox_end(idx);

                if(mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28]) {
                        if(!mbox[24]) {
                                uart_puts("BGR is an error.\n");
                                return;
                        }
                        uint32_t w = mbox[5];
                        uint32_t h = mbox[6];
                        //                  GPU has different addrs, idk
                        uint32_t *b = (uint32_t *)((uintptr_t)mbox[28] & 0x3FFFFFFF);
                        front_buffer =
                                graphics::buffer<graphics::row_major, graphics::RGBA>(b, w, h);
                        back_buffer =
                                graphics::buffer<graphics::row_major,
                                                 graphics::RGBA>((uint32_t *)kalloc(
                                                                         w * h * sizeof(uint32_t)),
                                                                 w,
                                                                 h);
                        _valid = true;
                } else
                        uart_puts("Can't set screen res\n");
        }
};

constexpr static inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return r | g << 8 | b << 16 | a << 24;
}
}
