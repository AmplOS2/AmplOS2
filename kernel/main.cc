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

        // send the message to the GPU and receive answer
        if(mbox_call(MBOX_CH_PROP)) printf("My serial number is: %8x%8x\n", mbox[6], mbox[5]);
        else
                uart_puts("Unable to query serial!\n");

        srand();
        printf("Random number: %8x\n", rand());

        MemoryModel memmod;
        printf("physical address space: %d bits\n", memmod.pa_range());

        printf("4k granules %ssupported.\n", memmod.stage1_tgran4() ? "" : "not ");
        printf("16k granules %ssupported.\n", memmod.stage1_tgran16() ? "" : "not ");
        printf("64k granules %ssupported.\n", memmod.stage1_tgran64() ? "" : "not ");

        printf("CPU frequency: %dMHz\n", cpufrequency() / 1000000);

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

        uint64_t end = clock();

        uart_puts("Initialization took ");
        printf("%d microseconds.\n", end - start);

        printf("Allocated 100B each @ %lx & %lx\n", kalloc(100), kalloc(100));

        for(;; f = (f << 1) | 1) {
                usleep(500000);
                gpu.draw_picture([](uint32_t x, uint32_t y, uint32_t) {
                        return rgba(x, y, x ^ y, 0xff) ^
                               rgba(rand() & f, rand() & f, rand() & f, 0);
                });
                gpu.draw_text_unifont("AmplOS2 is the best operating system ever created by "
                                      "humanity.\n"
                                      "It should be used by everyone sooner rather than later.\n"
                                      "Hello, world!\n"
                                      "Hello, Ludwig!\n"
                                      "Hä, wo kommt auf einmal dieser Unicode-Support her?!\n"
                                      "Je tu Pokémon, chtěl bych ho zničit, jo!\n"
                                      "私の父はケクです",
                                      rand() | rgba(0xf, 0xf, 0xf, 0xff),
                                      25,
                                      600);
                if(f == 0xff) f = 1;
        }
}
#endif
