#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

typedef struct { bool stable, candidate; uint32_t since; } Debounce;
/* Initialize with the actual input level and initial tick. */
void debounce_init(Debounce *d, bool level, uint32_t now) {
    d->stable = d->candidate = level; d->since = now;
}
/* Active-high key; period between checks and total delay < one wrap.
   Return +1 press, -1 release, 0 no accepted edge. */
int debounce_update(Debounce *d, bool raw, uint32_t now) {
    if (raw != d->candidate) {
        d->candidate = raw; d->since = now;
    }
    if (d->candidate != d->stable &&
        (uint32_t)(now - d->since) >= 20u) {
        d->stable = d->candidate;
        return d->stable ? 1 : -1;
    }
    return 0;
}

int main(void) {
Debounce d;debounce_init(&d,false,0);
assert(debounce_update(&d,true,1)==0);assert(debounce_update(&d,false,5)==0);assert(debounce_update(&d,true,8)==0);
assert(debounce_update(&d,true,27)==0);assert(debounce_update(&d,true,28)==1);assert(debounce_update(&d,true,900)==0);
assert(debounce_update(&d,false,901)==0);assert(debounce_update(&d,false,921)==-1);assert(debounce_update(&d,false,940)==0);
debounce_init(&d,false,UINT32_MAX-10u);assert(debounce_update(&d,true,UINT32_MAX-10u)==0);assert(debounce_update(&d,true,9)==1);
debounce_init(&d,true,0);assert(debounce_update(&d,true,100)==0);
puts("PASS debounce");
return 0;
}
