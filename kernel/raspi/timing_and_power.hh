#pragma once
#include <mem.hh>
#include <raspi/mbox.hh>
#include <raspi/mmio.hh>
#include <stdint.h>

#define spinwhile(b) \
        while(b) cpu_relax()

#define SYSTMR_LO mem(MMIO_BASE + 0x00003004)
#define SYSTMR_HI mem(MMIO_BASE + 0x00003008)

#define PM_RSTC         mem(MMIO_BASE + 0x0010001c)
#define PM_RSTS         mem(MMIO_BASE + 0x00100020)
#define PM_WDOG         mem(MMIO_BASE + 0x00100024)
#define PM_WDOG_MAGIC   0x5a000000
#define PM_RSTC_FULLRST 0x00000020

namespace {
// this is copy-paste from linux
// (i have actually it looked up from arm and it seems fine to me)
#define cpu_relax() asm volatile("yield" ::: "memory")

inline void spincycles(uint64_t n) { spinwhile(n--); }

#define abort shutdown
void shutdown(bool restart = false) {
        if(!restart) {
                for(uint32_t i = 0; i < 16; i++) {
                        size_t idx = mbox_start(8);
                        mbox_cmd(idx, MBOX_TAG_SETPOWER, 2, i, 0);
                        mbox_end(idx);
                        mbox_call(MBOX_CH_PROP);
                }

                GPFSEL0 = 0;
                GPFSEL1 = 0;
                GPFSEL2 = 0;
                GPFSEL3 = 0;
                GPFSEL4 = 0;
                GPFSEL5 = 0;
                GPPUD   = 0;
                spincycles(150);
                GPPUDCLK0 = 0xffffffff;
                GPPUDCLK1 = 0xffffffff;
                spincycles(150);
                GPPUDCLK0 = 0;
                GPPUDCLK1 = 0;
        }

        uint64_t r = PM_RSTS & ~0xfffffaaa;
        if(!restart) r |= 0x555;
        PM_RSTS = PM_WDOG_MAGIC | r;
        PM_WDOG = PM_WDOG_MAGIC | 10;
        PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
}

inline uint64_t clock() { return ((uint64_t)SYSTMR_HI << 32) | SYSTMR_LO; }

inline void usleep(uint64_t n) {
        uint64_t t = clock() + n;
        spinwhile(clock() < t);
}
}
