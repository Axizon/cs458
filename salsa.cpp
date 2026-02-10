#include <iostream>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <string>

uint32_t rotateleft(uint32_t x, int n) {
    uint32_t y = x << n;
    y = y | (x >> (32 - n));
    return y;
}

void quarterround(uint32_t &y0, uint32_t &y1, uint32_t &y2, uint32_t &y3) {
    uint32_t z0, z1, z2, z3;
    z1 = y1 ^ rotateleft((y0 + y3), 7);
    z2 = y2 ^ rotateleft((z1 + y0), 9);
    z3 = y3 ^ rotateleft((z2 + z1), 13);
    z0 = y0 ^ rotateleft((z3 + z2), 18);
    y0 = z0;
    y1 = z1;
    y2 = z2;
    y3 = z3;
}

int main(int argc, char *argv[]) {
    return 0;
}
