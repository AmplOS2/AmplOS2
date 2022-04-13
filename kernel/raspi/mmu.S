.align 3
.global mmu_enable

mmu_enable:
        msr TTBR0_EL2, x0
        msr TCR_EL2, x1
        msr MAIR_EL2, x2
        isb

        // Invalidate TLBs
        tlbi ALLE2
        dsb SY
        isb

        mov      x3, #(1 << 0)        // M=1    Enable MMU
        orr      x3, x3, #(1 << 2)    // C=1    Enable data and unified caches
        orr      x3, x3, #(1 << 12)   // I=1    Enable instruction fetches to allocate into unified caches
                                      // A=0    Strict alignment checking disabled
                                      // SA=0   Stack alignment checking disabled
                                      // WXN=0  W^X disabled
                                      // EE=0   Little Endian accesses
        msr SCTLR_EL2, x3
        isb

        ret
