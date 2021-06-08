#include <stdint.h>

namespace MMU {

// TODO: read ID_AA64MMFR0
const unsigned GRANULE_SIZE = 64 * 1024;

// TODO: make it easy to create them
typedef uint64_t table_descriptor;
typedef uint64_t block_entry;

extern "C" void mmu_enable(void *ttbr0, void *ttbr1, void *tcr);

};
