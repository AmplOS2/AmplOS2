#pragma once
#include <mem.hh>
#include <raspi/mmio.hh>
#include <stddef.h>

#define MBOX_REQUEST 0

#define MBOX_CH_POWER 0
#define MBOX_CH_FB    1
#define MBOX_CH_VUART 2
#define MBOX_CH_VCHIQ 3
#define MBOX_CH_LEDS  4
#define MBOX_CH_BTNS  5
#define MBOX_CH_TOUCH 6
#define MBOX_CH_COUNT 7
#define MBOX_CH_PROP  8

#define MBOX_TAG_GETSERIAL  0x10004
#define MBOX_TAG_SETCLKRATE 0x38002
#define MBOX_TAG_LAST       0

#define VIDEOCORE_MBOX      (MMIO_BASE + 0xB880)
#define MBOX_READ           mem(VIDEOCORE_MBOX + 0x00)
#define MBOX_POLL           mem(VIDEOCORE_MBOX + 0x10)
#define MBOX_SENDER         mem(VIDEOCORE_MBOX + 0x14)
#define MBOX_STATUS         mem(VIDEOCORE_MBOX + 0x18)
#define MBOX_CONFIG         mem(VIDEOCORE_MBOX + 0x1C)
#define MBOX_WRITE          mem(VIDEOCORE_MBOX + 0x20)
#define MBOX_RESPONSE       0x80000000
#define MBOX_FULL           0x80000000
#define MBOX_EMPTY          0x40000000
#define MBOX_TAG_SETPOWER   0x28001
#define MBOX_TAG_SETCLKRATE 0x38002
#define MBOX_TAG_LAST       0

extern volatile uint32_t mbox[36];

namespace {
inline bool mbox_call(const char ch) {
        // static_assert(ch < 0x10);
        // static_assert((((uintptr_t)&mbox) & 0xffffffff0000000f) == 0);
        uint32_t r = (((uint32_t)(uintptr_t)&mbox) | ch);
        // TODO: reorganise some code to allow using spinwhile here again
        while(MBOX_STATUS & MBOX_FULL)
                ;
        MBOX_WRITE = r;
        while(1) {
                while(MBOX_STATUS & MBOX_EMPTY)
                        ;
                if(r == MBOX_READ) return mbox[1] == MBOX_RESPONSE;
        }
}

constexpr inline size_t mbox_start(size_t len) {
        size_t idx  = 0;
        mbox[idx++] = len * sizeof(uint32_t);
        mbox[idx++] = MBOX_REQUEST;
        return idx;
}

constexpr inline void mbox_cmd(size_t &       idx,
                               const uint32_t cmd,
                               const uint32_t len,
                               const uint32_t arg1,
                               const uint32_t arg2 = 0) {
        // static_assert(len == 1 || len == 2);
        mbox[idx++] = cmd;
        mbox[idx++] = len * sizeof(uint32_t);
        mbox[idx++] = len * sizeof(uint32_t);
        mbox[idx++] = arg1;
        if(len == 2) mbox[idx++] = arg2;
}

constexpr inline void mbox_end(size_t &idx) { mbox[idx++] = MBOX_TAG_LAST; }
}
