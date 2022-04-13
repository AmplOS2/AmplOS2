// https://sourceforge.net/p/predef/wiki/Architectures/
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) ||   \
        defined(_M_X64) || defined(_M_AMD64) || defined(i386) || defined(__i386) ||         \
        defined(__i386__) || defined(__IA32__) || defined(_M_I86) || defined(_M_IX86) ||    \
        defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || \
        defined(__INTEL__)
#define ANOS_X86_PC
#elif defined(__aarch64__)
#define ANOS_ARM64_RASPI
#else
// TODO: parse more archs
#error "Check the arch your're compiling for! It isn't AArch64 nor is it x86."
#endif

#include <kinclude.hh>

#ifdef ANOS_X86_PC
#error "Support for x86 is coming, but currently only RasPis are supported."
#endif

#ifdef ANOS_ARM64_RASPI
uint8_t f = 1;

extern "C" void kmain() {
        uint64_t start = clock();
        uart_init();

        // get the serial number with a mailbox call
        size_t idx = mbox_start(8);
        mbox_cmd(idx, MBOX_TAG_GETSERIAL, 2, 0, 0);
        mbox_end(idx);

        if(!mbox_call(MBOX_CH_PROP)) uart_puts("Unable to query serial!\n");

        srand();
        syslog << "Random number: " << syslog_hex << rand() << "\n";

        MemoryModel memmod;
        syslog << "physical address space: " << memmod.pa_range() << " bits\n";

        syslog << "4k granules " << (memmod.stage1_tgran4() ? "" : "not ") << "supported.\n";
        syslog << "16k granules " << (memmod.stage1_tgran16() ? "" : "not ") << "supported.\n";
        syslog << "64k granules " << (memmod.stage1_tgran64() ? "" : "not ") << "supported.\n";

        syslog << "CPU frequency: " << (cpufrequency() / 1000000) << "MHz\n";

        assert(current_el() == 2);
        syslog << "Kernel running at EL2, that is very good.\n";

        //  TODO: do we have to write to that?
        syslog << "CPTR_EL2: " << syslog_hex << cptr_el2() << "\n";

        // TODO: exception vectors (VBAR_ELx)

        constexpr uint64_t TT_S1_NORMAL_WBWA   = 0x00000000000000405; // Index = 1, AF=1
        constexpr uint64_t TT_S1_DEVICE_nGnRnE = 0x00600000000000409; // Index = 2, AF=1, PXN=1,

        uint64_t *tt = (uint64_t *)kalloc(4096);
        memset(tt, 0, 4096);
        tt[0] = TT_S1_NORMAL_WBWA | 0;
        tt[1] = TT_S1_DEVICE_nGnRnE | MMIO_BASE;

        // T0SZ=0b011001 Limits VA space to 39 bits, translation starts @ l1
        // IGRN0=0b01    Walks to TTBR0 are Inner WB/WA
        // OGRN0=0b01    Walks to TTBR0 are Outer WB/WA
        // SH0=0b11      Inner Shareable
        // TBI0=0b0      Top byte not ignored
        // TG0=0b00      4KB granule
        // IPS=0         32-bit PA space
        MMU::mmu_enable(tt, 0x19 | (1 << 8) | (1 << 10) | (3 << 12), 0x000000000000FF44);

        GPU gpu;
        assert(gpu.valid());

        gpu.draw_picture([](uint32_t x, uint32_t y, uint32_t) { return rgba(x, y, x ^ y, 0xff); });
        gpu.draw_text_unifont("AmplOS2 is the best operating system ever created by humanity.\n"
                              "It should be used by everyone sooner rather than later.\n"
                              "Hello, world!\n"
                              "Hello, Ludwig!",
                              rand() | rgba(0xf, 0xf, 0xf, 0xff),
                              25,
                              600);
        gpu.swap_buffers();

        uint64_t end = clock();

        syslog << "Initialization took " << (end - start) << " microseconds.\n";

        syslog << "Allocated 100B each @ " << kalloc(100) << " & " << kalloc(100) << "\n";

        for(;; f = (f << 1) | 1) {
                usleep(500000);
                gpu.draw_picture([](uint32_t x, uint32_t y, uint32_t) {
                        return rgba(x, y, x ^ y, 0xff) ^
                               rgba(rand() & f, rand() & f, rand() & f, 0);
                });
                for(uint32_t x = 0; x < gpu.width(); x++) {
                        uint32_t y =
                                sin(fmod(((double)x) / 100, 2 * PI)) * 200 - 100 + gpu.width() / 2;
                        for(uint32_t i = y - 5; i <= y + 5; i++) gpu.pixel(x, i) = 0xffffffff;
                }
                gpu.draw_text_unifont("AmplOS2 is the best operating system ever created by "
                                      "humanity.\n"
                                      "It should be used by everyone sooner rather than later.\n"
                                      "Hello, world!\n"
                                      "Hello, Ludwig!",
                                      rand() | rgba(0xf, 0xf, 0xf, 0xff),
                                      25,
                                      600);
                gpu.swap_buffers();
                if(f == 0xff) f = 1;
        }
}
#endif
