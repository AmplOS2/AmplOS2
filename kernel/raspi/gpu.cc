#include <raspi/gpu.hh>
#include <raspi/mbox.hh>
#include <raspi/uart0.hh>

GPU::GPU(uint32_t width, uint32_t height) noexcept {
        this->_valid = false;

        size_t idx = mbox_start(35);
        mbox_cmd(idx, 0x48003, 2, width, height); // set physical width & height
        mbox_cmd(idx, 0x48004, 2, width, height); // set virtual width & height
        mbox_cmd(idx, 0x48009, 2, 0, 0);          // set virtual offset
        mbox_cmd(idx, 0x48005, 1, 32, 0);         // set depth
        mbox_cmd(idx, 0x48006, 1, 1, 0);          // set pixel order
        mbox_cmd(idx, 0x40001, 2, 4096, 0);       // get framebuffer & alignment (pointer, size)
        mbox_cmd(idx, 0x40008, 1, 0, 0);          // get pitch
        mbox_end(idx);

        if(mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28]) {
                this->_width  = mbox[5];
                this->_height = mbox[6];
                pitch         = mbox[33]; // number of bytes per line
                if(!mbox[24]) {
                        uart_puts("BGR is an error.\n");
                        return;
                }
                //                  GPU has different addrs, idk
                buffer       = (uint8_t *)((uintptr_t)mbox[28] & 0x3FFFFFFF);
                this->_valid = true;
        } else
                uart_puts("Can't set screen res\n");
}
