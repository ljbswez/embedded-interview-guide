#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define AVG_N 8u
typedef struct {
    int16_t values[AVG_N]; unsigned pos, count; int32_t sum;
} Average;
/* Single-thread model, initialized to all-zero. */
int16_t average_push(Average *a, int16_t sample) {
    if (a->count == AVG_N) a->sum -= a->values[a->pos];
    else ++a->count;
    a->values[a->pos] = sample;
    a->sum += sample;
    a->pos = (a->pos + 1u) % AVG_N;
    return (int16_t)(a->sum / (int32_t)a->count);
}

int main(void) {
Average a={0};assert(average_push(&a,100)==100);assert(average_push(&a,-100)==0);
memset(&a,0,sizeof(a));for(unsigned i=0;i<100;i++)assert(average_push(&a,INT16_MIN)==INT16_MIN);
memset(&a,0,sizeof(a));for(unsigned i=0;i<100;i++)assert(average_push(&a,INT16_MAX)==INT16_MAX);
memset(&a,0,sizeof(a));int16_t history[8]={0};unsigned count=0;
for(unsigned i=0;i<10000;i++){int16_t v=(int16_t)((int)(i*97u%60001u)-30000);history[i%8]=v;if(count<8)count++;int32_t sum=0;for(unsigned j=0;j<count;j++)sum+=history[j];assert(average_push(&a,v)==sum/(int32_t)count);}
puts("PASS average");
return 0;
}
