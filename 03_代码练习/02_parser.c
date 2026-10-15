#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

enum { WAIT_HEAD, READ_LEN, READ_DATA, READ_SUM };
typedef struct { unsigned state; uint8_t len, used, sum, data[64]; } Parser;
typedef struct { uint8_t len, data[64]; } Frame;
/* A5, length(0..64), payload, XOR(length and payload). */
int parser_feed(Parser *p, uint8_t byte, Frame *out) {
    if (!p || !out) return -1;
    switch (p->state) {
    case WAIT_HEAD:
        if (byte == 0xA5u) p->state = READ_LEN;
        return 0;
    case READ_LEN:
        if (byte > 64u) { p->state = WAIT_HEAD; return -1; }
        p->len = byte; p->used = 0; p->sum = byte;
        p->state = byte ? READ_DATA : READ_SUM;
        return 0;
    case READ_DATA:
        p->data[p->used++] = byte; p->sum ^= byte;
        if (p->used == p->len) p->state = READ_SUM;
        return 0;
    case READ_SUM:
        p->state = WAIT_HEAD;
        if (byte != p->sum) return -1;
        out->len = p->len;
        memcpy(out->data, p->data, p->len);
        return 1;
    default:
        p->state = WAIT_HEAD; return -1;
    }
}

int main(void) {
Parser p={0}; Frame f={0}; f.len=55;
uint8_t a[]={0x00,0xA5,3,0x11,0xA5,0x33,(uint8_t)(3^0x11^0xA5^0x33)};
for(size_t i=0;i<sizeof(a)-1;i++) assert(parser_feed(&p,a[i],&f)==0);
assert(f.len==55); assert(parser_feed(&p,a[sizeof(a)-1],&f)==1); assert(f.len==3 && f.data[1]==0xA5);
assert(parser_feed(&p,0xA5,&f)==0); assert(parser_feed(&p,0,&f)==0); assert(parser_feed(&p,0,&f)==1 && f.len==0);
assert(parser_feed(&p,0xA5,&f)==0); assert(parser_feed(&p,65,&f)==-1);
assert(parser_feed(&p,0xA5,&f)==0); assert(parser_feed(&p,1,&f)==0); assert(parser_feed(&p,7,&f)==0); assert(parser_feed(&p,99,&f)==-1 && f.len==0);
assert(parser_feed(&p,0xA5,&f)==0); assert(parser_feed(&p,64,&f)==0); uint8_t sum=64;
for(unsigned i=0;i<64;i++){sum^=(uint8_t)i;assert(parser_feed(&p,(uint8_t)i,&f)==0);}
assert(parser_feed(&p,sum,&f)==1 && f.len==64);
for(unsigned i=0;i<64;i++) assert(f.data[i]==i);
for(unsigned round=0;round<1000;round++){for(size_t i=0;i<sizeof(a);i++) {int result=parser_feed(&p,a[i],&f);assert(result==(i==sizeof(a)-1?1:0));}}
puts("PASS parser");
return 0;
}
