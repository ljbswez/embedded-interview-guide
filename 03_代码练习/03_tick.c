#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

/* Preconditions: monotonic 32-bit ticks; start is in the past;
   elapsed real ticks since start < 2^32; delay <= INT32_MAX. */
bool expired32(uint32_t now, uint32_t start, uint32_t delay) {
    if (delay > INT32_MAX) return false;
    return (uint32_t)(now - start) >= delay;
}

int main(void) {
uint32_t start=UINT32_MAX-15u;
assert(!expired32(start+5u,start,32)); assert(!expired32(0,start,32));
assert(expired32(16,start,32)); assert(expired32(17,start,32));
assert(expired32(0,0,0)); assert(!expired32(0,0,UINT32_MAX));
for(uint32_t i=0;i<100000;i++){uint32_t a=i*7919u;for(uint32_t d=0;d<100;d++){assert(expired32(a+d,a,d));if(d)assert(!expired32(a+d-1u,a,d));}}
puts("PASS tick");
return 0;
}
