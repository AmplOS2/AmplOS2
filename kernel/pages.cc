#include <memset.hh>
#include <pages.hh>

#define MEMORY_SIZE (256 * 1024 * 1024)
// TODO: get this from arch specific check
#define PAGE_COUNT     (MEMORY_SIZE / PAGE_SIZE)
#define FIRST_PAGE     ((0x10 * 0x100000) / PAGE_SIZE)
#define PAGE2ADDR(x)   ((void *)((x)*PAGE_SIZE))
#define ADDR2PAGE(x)   (((uintptr_t)(x)) / PAGE_SIZE)
#define PAGES2BYTES(x) ((x)*PAGE_SIZE)

struct {
        bool allocated = false, continued = false;
} pages[PAGE_COUNT];

void *k_page_alloc(size_t count) {
        for(size_t i = FIRST_PAGE; i < PAGE_COUNT - count; i++) {
                if(i + count >= PAGE_COUNT) continue;
                bool b = false;
                for(size_t j = i; j < i + count; j++) b |= pages[j].allocated;
                if(b) continue;
                for(size_t j = i; j < i + count; j++)
                        pages[j].allocated = pages[j].continued = true;
                pages[i + count - 1].continued = false;
                void *ptr                      = PAGE2ADDR(i);
                memset(ptr, 0, PAGES2BYTES(count));
                return ptr;
        }
        return NULL;
}

void k_page_free(void *ptr) {
        if((uintptr_t)ptr % PAGE_SIZE) return;
        size_t i = ADDR2PAGE(ptr);
        if(!pages[i--].allocated) return;
        while(pages[++i].continued) pages[i].allocated = pages[i].continued = false;
        pages[i].allocated = false;
}
