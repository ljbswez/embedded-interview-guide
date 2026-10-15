#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

/* Accept only 1+ ASCII decimal digits. On failure, *out is unchanged. */
bool parse_u32(const char *s, size_t len, uint32_t *out) {
    if (!s || !out || !len) return false;
    uint32_t value = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = (unsigned char)s[i];
        if (c < '0' || c > '9') return false;
        uint32_t digit = (uint32_t)(c - '0');
        if (value > (UINT32_MAX - digit) / 10u) return false;
        value = value * 10u + digit;
    }
    *out = value;
    return true;
}

int main(void) {
uint32_t out=99;
assert(parse_u32("0",1,&out)&&out==0);assert(parse_u32("4294967295",10,&out)&&out==UINT32_MAX);
const char *bad[]={"","4294967296","-1","+1"," 1","1 ","12x"};
for(unsigned i=0;i<sizeof(bad)/sizeof(bad[0]);i++){out=123;assert(!parse_u32(bad[i],strlen(bad[i]),&out)&&out==123);}
char embedded[3]={'1',0,'2'};assert(!parse_u32(embedded,3,&out));assert(!parse_u32(NULL,1,&out));
for(uint32_t i=0;i<10000;i++){uint32_t v=i*429499u;char buf[32];int len=snprintf(buf,sizeof(buf),"%lu",(unsigned long)v);assert(len>0);assert(parse_u32(buf,(size_t)len,&out)&&out==v);}
puts("PASS parse");
return 0;
}
