#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define POOL_N 8u
#define BLOCK_BYTES 32u
typedef struct { uint8_t slot; uint32_t generation; } Handle;
typedef struct {
    bool used[POOL_N]; uint32_t generation[POOL_N];
    uint8_t bytes[POOL_N][BLOCK_BYTES];
} Pool;
/* Byte-storage handles only; not a general malloc replacement. */
bool pool_valid(const Pool *p, Handle h) {
    return p && h.slot < POOL_N && p->used[h.slot] &&
           p->generation[h.slot] == h.generation;
}
bool pool_alloc(Pool *p, Handle *out) {
    if (!p || !out) return false;
    for (unsigned i = 0; i < POOL_N; ++i) {
        if (!p->used[i] && p->generation[i] != UINT32_MAX) {
            ++p->generation[i]; p->used[i] = true;
            memset(p->bytes[i], 0, BLOCK_BYTES);
            out->slot = (uint8_t)i; out->generation = p->generation[i];
            return true;
        }
    }
    return false;
}
bool pool_free(Pool *p, Handle h) {
    if (!pool_valid(p, h)) return false;
    p->used[h.slot] = false;
    return true;
}

int main(void) {
Pool p={0};Handle h[8],extra={99,123};
for(unsigned i=0;i<8;i++){assert(pool_alloc(&p,&h[i]));assert(pool_valid(&p,h[i]));}
assert(!pool_alloc(&p,&extra)&&extra.slot==99&&extra.generation==123);
Handle old=h[0];assert(pool_free(&p,old));assert(!pool_free(&p,old));assert(pool_alloc(&p,&h[0]));
assert(h[0].slot==old.slot&&h[0].generation!=old.generation);assert(!pool_free(&p,old));assert(pool_valid(&p,h[0]));
extra.slot=255;assert(!pool_valid(&p,extra));
assert(pool_free(&p,h[1]));p.generation[h[1].slot]=UINT32_MAX;assert(!pool_alloc(&p,&extra));
assert(pool_free(&p,h[2]));assert(pool_alloc(&p,&extra));assert(extra.slot==h[2].slot);
puts("PASS pool");
return 0;
}
