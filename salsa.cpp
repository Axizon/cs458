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

void rowround(uint32_t y[16]) {
    quarterround(y[0], y[1], y[2], y[3]);
    quarterround(y[5], y[6], y[7], y[4]);
    quarterround(y[10], y[11], y[8], y[9]);
    quarterround(y[15], y[12], y[13], y[14]);
}

void columnround(uint32_t x[16]) {
    quarterround(x[0], x[4], x[8], x[12]);
    quarterround(x[5], x[9], x[13], x[1]);
    quarterround(x[10], x[14], x[2], x[6]);
    quarterround(x[15], x[3], x[7], x[11]);
}

void doubleround(uint32_t x[16]) {
    columnround(x);
    rowround(x);
}

int main(int argc, char *argv[]) {
    return 0;
}
