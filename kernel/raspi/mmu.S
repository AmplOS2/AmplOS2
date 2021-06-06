mmu_enable:
        // sets the page table addresses
        msr TTBR0_EL2, X0
        msr TTBR1_EL2, X1
        msr TCR_EL2, X2
        isb

        mrs X3, SCTLR_EL2
        // setting the lsb in SCTLR actually enables the MMU
        orr X3, X3, #1
        msr SCTLR_EL2, X3
        isb

        ret
