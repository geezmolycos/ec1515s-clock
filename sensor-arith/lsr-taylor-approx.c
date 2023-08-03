
#include <stdint.h>
#include <stdio.h>

// 1024/x - 1 then (t 0~63)
// for x<20, result is too big
// 1.3364691554162t^(2) + 1.1199998237482t - 1.064257549764

// use 6.10 for t

uint16_t sensor_light_approx_t(uint16_t x){
    // 0~1023 for x
    // calculate 1024/x - 1 and output t as 6.10
    uint32_t t = ((uint32_t)1024 << 10) / x;
    t -= (1 << 10);
    // 6.10
    uint32_t term1 = ((((t * t) >> 10) * 0x559) >> 10);
    uint32_t term2 = (t * 0x47b) >> 10;
    uint32_t term3 = 0x442;

    uint32_t res = term1 + term2 - term3;
    if (res & (1u << 31)){
        return 0;
    } else {
        return (res >> 6) & 0xffff; // 12.4
    }
}

// 149.29990369062475
// -0.36602994345730655 x
// + 0.0004635776216000975 x^2
// -0.00000026765531167816103 x^3
uint16_t sensor_temp_celsius(uint16_t x){
    // 10.22
    uint32_t term1 = 0x176d09 * x;
    uint32_t term2 = 0x79863 * (x * x >> 8);
    uint32_t term3 = 0x11f6 * (x * x * x >> 12);
    return (0x2553319f - term1 + term2 - term3) >> 16; // 10.6
}


int main(){
    for (int i=1; i<1024; i++){
        float r = (float)(int16_t)sensor_temp_celsius(i) / (1 << 6);
        printf("%d, %.2f\n", i, r);
        if (r == 0){
            break;
        }
    }
}