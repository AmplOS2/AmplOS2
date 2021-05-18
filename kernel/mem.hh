#pragma once
#include <stdint.h>

namespace {
static inline volatile uint32_t &mem(const uintptr_t loc) {
        return *reinterpret_cast<volatile uint32_t *>(loc);
}
}
