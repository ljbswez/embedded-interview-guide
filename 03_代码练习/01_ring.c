#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define RB_CAP 16u
typedef struct { uint8_t data[RB_CAP]; size_t head, tail; } Ring;
/* Single-thread use, or protect each operation externally. */
bool rb_push(Ring *r, uint8_t value) {
    if (!r) return false;
    size_t next = (r->head + 1u) % RB_CAP;
    if (next == r->tail) return false;
    r->data[r->head] = value;
    r->head = next;
    return true;
}
bool rb_pop(Ring *r, uint8_t *out) {
    if (!r || !out || r->head == r->tail) return false;
    *out = r->data[r->tail];
    r->tail = (r->tail + 1u) % RB_CAP;
    return true;
}

int main(void) {
Ring r = {0}; uint8_t out = 99;
assert(!rb_pop(&r, &out) && out == 99);
for (unsigned i=0;i<15;i++) assert(rb_push(&r,(uint8_t)i));
assert(!rb_push(&r,200));
for (unsigned i=0;i<15;i++) { assert(rb_pop(&r,&out)); assert(out==i); }
assert(!rb_pop(&r,&out));
for (unsigned i=0;i<10000;i++) { assert(rb_push(&r,(uint8_t)i)); assert(rb_pop(&r,&out)); assert(out==(uint8_t)i); }
assert(!rb_pop(NULL,&out)); assert(!rb_pop(&r,NULL));
puts("PASS ring");
return 0;
}
