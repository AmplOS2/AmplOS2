#pragma once
#include <stdint.h>

#define mem(loc) (*reinterpret_cast<volatile uint32_t *>(loc))
