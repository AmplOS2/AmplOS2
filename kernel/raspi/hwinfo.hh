#pragma once
#include <stdint.h>

namespace {

#define read_mrs(reg)                                      \
        (([]() {                                           \
                volatile uint64_t _mrs;                    \
                asm volatile("mrs %0, " reg : "=r"(_mrs)); \
                return _mrs;                               \
        })())

class MemoryModel {
private:
        uint64_t mmfr0, mmfr1;

public:
        inline MemoryModel() {
                mmfr0 = read_mrs("ID_AA64MMFR0_EL1");
                mmfr1 = read_mrs("ID_AA64MMFR1_EL1");
        }

        // https://developer.arm.com/documentation/ddi0595/2020-12/AArch64-Registers/ID-AA64MMFR0-EL1--AArch64-Memory-Model-Feature-Register-0

        inline bool ecv() const { return (mmfr0 >> 60) & 3; }
        inline bool fgt() const { return ((mmfr0 >> 56) & 0xf) == 1; }
        inline bool exs() const { return ((mmfr0 >> 44) & 0xf) == 1; }

        inline bool stage1_tgran4() const { return ((mmfr0 >> 28) & 0xe) == 0; }
        inline bool stage1_tgran16() const {
                uint32_t i = (mmfr0 >> 20) & 0xf;
                return i == 1 || i == 2;
        }
        inline bool stage1_tgran64() const { return ((mmfr0 >> 24) & 0xf) == 0; }

        inline bool stage2_tgran4() const {
                uint32_t i = (mmfr0 >> 40) & 0xf;
                if(i == 0) return stage1_tgran4();
                return (i & 2) == 2;
        }
        inline bool stage2_tgran16() const {
                uint32_t i = (mmfr0 >> 32) & 0xf;
                if(i == 0) return stage1_tgran16();
                return (i & 2) == 2;
        }
        inline bool stage2_tgran64() const {
                uint32_t i = (mmfr0 >> 36) & 0xf;
                if(i == 0) return stage1_tgran64();
                return i == 2;
        }

        inline bool mixed_endian() const { return ((mmfr0 >> 8) & 0xf) == 1; }
        inline bool sns_mem() const { return ((mmfr0 >> 12) & 0xf) == 1; }
        inline bool mixed_endian_el0() const {
                return mixed_endian() || ((mmfr0 >> 16) & 0xf) == 1;
        }
        inline int32_t asid_bits() const {
                uint32_t i = (mmfr0 >> 4) & 0xf;
                if(i == 0) return 8;
                if(i == 2) return 16;
                return -1;
        }
        inline int32_t pa_range() const {
                switch(mmfr0 & 0xf) {
                        case 0: return 32;
                        case 1: return 36;
                        case 2: return 40;
                        case 3: return 42;
                        case 4: return 44;
                        case 5: return 48;
                        case 6: return 52;
                        default: return -1;
                }
        }

        // TODO: other registers
};
/// reads CNTPCT, the physical counter; CNTVCT is the virtual one
inline uint64_t      rdtsc() { return read_mrs("CNTPCT_EL0"); }
inline uint_fast32_t cpufrequency() { return read_mrs("CNTFRQ_EL0") & 0xffffffff; }
inline uint_fast8_t  current_el() { return (read_mrs("CurrentEL") & 0b1100) >> 2; }
/// https://developer.arm.com/documentation/ddi0595/2020-12/AArch64-Registers/CPTR-EL2--Architectural-Feature-Trap-Register--EL2-
/// D13.2.31, p.5321
inline uint_fast16_t cptr_el2() { return (read_mrs("CPTR_EL2") & 0xffffffff) >> 16; }
#undef read_mrs
}
