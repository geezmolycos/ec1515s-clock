#include <stdint.h>
#include <stdio.h>

uint16_t sqrt_1616(uint16_t v) {
    uint16_t t, q, b, r;
    r = v;
    b = 0x4000;
    q = 0;
    while( b > 0x4 )
    {
        t = q + b;
        if( r >= t )
        {
            r -= t;
            q = t + b; // equivalent to q += 2*b
        }
        r <<= 1;
        b >>= 1;
    }
    q >>= 4;
    return q;
}

uint16_t isqrt(uint16_t n){
    uint16_t x = n;
    uint16_t y = (x + 0x100) / 2;
    while (y < x){
        x = y;
        y = (x + ((int)n << 8) / x) / 2;
    }
    return x;
}

uint16_t sqrt_10_6(uint16_t n){
    uint16_t x = n; // 10.6
    uint16_t y = (x + (1 << 6)) / 2; // 10.6
    while (y < x){
        x = y;
        y = (x + (n / ((x >> 3) + ((x & (1<<2)) >> 2)) << 3)) / 2; // 10.6 / 10.3 = 10.3
    }
    return x;
}

uint16_t pow_10_6(uint16_t n, uint8_t p){
    uint16_t prod = 1 << 6;
    for (uint8_t i=0; i<8; i++){
        n = sqrt_10_6(n);
        printf("i:%d, n:%x\n", i, n);
        if (p >> 7){
            prod = ((n >> 3) + ((n & (1<<2)) >> 2)) * ((prod >> 3) + ((prod & (1<<2)) >> 2));
        }
        p <<= 1;
    }
    return prod;
}

int main(){
    printf("%x", (int)pow_10_6(0x2000, 0xff));
    return 0;
}