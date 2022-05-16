// "I don't understand any of this, but it's from OSDev Wiki and I
// trust it." – chrissx, after they copy-pasted non-working code
// "I don't understand any of this, but it's from jsandler18 and I
// trust it." – chrissx, after they copy-pasted working code
// "This is horrible, ARM 32 <-> 64 bit differences are a fucking
// mess, I should go back to using AArch64 code" – chrissx, after they
// discovered, how bad jsandler18's code is for RPI3+.
// "ICH HAB N OBJECT FILE!" – chrissx, after fixing OSDev Wiki's code

// there are many ways of achieving the same thing in the boot code,
// but this seems to work in qemu. (but not all ways of achieving a
// successful boot have html in them)

.align 3

.section ".text.funkyentry"

.global _funkyentry
_funkyentry:
    b _start
    .incbin "sourcelist/dist/index.html"

.section ".text.boot"

.global _start
_start:
        // read cpu id, stop secondary cores
        mrs x1, mpidr_el1
        and x1, x1, #3
        cbz x1, 2f
        // cpu id > 0 ⇒ stop
1:      wfi
        b 1b
2:      // cpu id = 0

        // the stack grows from the entry point downwards
        ldr x1, =_start
        mov sp, x1

        // clear bss
        ldr x1, =__bss_start
        ldr w2, =__bss_size
3:      cbz w2, 4f
        str xzr, [x1], #8
        sub w2, w2, #1
        cbnz w2, 3b

4:      bl kmain
        b 1b
