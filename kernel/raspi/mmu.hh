#include <stdint.h>

namespace {
namespace MMU {
extern "C" void mmu_enable(void *ttbr0, uint64_t tcr, uint64_t mair);

// TODO: make it easy to create them
typedef uint64_t table_descriptor;
typedef uint64_t block_entry;
}
}
