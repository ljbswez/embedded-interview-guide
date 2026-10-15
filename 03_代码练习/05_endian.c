#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

bool read_be32(const uint8_t *data, size_t size, size_t offset,
               uint32_t *out) {
    if (!data || !out || offset > size || size - offset < 4u)
        return false;
    const uint8_t *p = data + offset;
    *out = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
         | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
    return true;
}
bool write_be32(uint8_t *data, size_t size, size_t offset, uint32_t v) {
    if (!data || offset > size || size - offset < 4u) return false;
    data[offset] = (uint8_t)(v >> 24);
    data[offset + 1] = (uint8_t)(v >> 16);
    data[offset + 2] = (uint8_t)(v >> 8);
    data[offset + 3] = (uint8_t)v;
    return true;
}

int main(void) {
uint8_t bytes[10]={0};uint32_t out=77;
assert(write_be32(bytes,sizeof(bytes),3,0x89ABCDEFu));assert(bytes[3]==0x89&&bytes[4]==0xAB&&bytes[5]==0xCD&&bytes[6]==0xEF);
assert(read_be32(bytes,sizeof(bytes),3,&out)&&out==0x89ABCDEFu);
out=77;assert(!read_be32(bytes,10,7,&out)&&out==77);assert(!read_be32(bytes,10,SIZE_MAX,&out));
uint8_t old[10];memcpy(old,bytes,10);assert(!write_be32(bytes,10,8,0));assert(memcmp(old,bytes,10)==0);
for(uint32_t i=0;i<100000;i++){uint32_t v=i*104729u;assert(write_be32(bytes,10,1,v));assert(read_be32(bytes,10,1,&out)&&out==v);}
puts("PASS endian");
return 0;
}
