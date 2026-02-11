#include <iostream>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <string>
#include <cstdio>

using namespace std;

uint32_t rotateleft(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
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

uint32_t littleendian(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    uint32_t a = (uint32_t)b0 | ((uint32_t)b1 << 8) | ((uint32_t)b2 << 16) | ((uint32_t)b3 << 24);
    return a;
}

void littleendianinverse(uint32_t a, uint8_t *b) {
    b[0] = a & 0xFF;
    b[1] = (a >> 8) & 0xFF;
    b[2] = (a >> 16) & 0xFF;
    b[3] = (a >> 24) & 0xFF;
}

void salsa20(uint8_t x[64]) {
    uint32_t z[16];
    uint32_t original[16];
    
    for (int i = 0; i < 16; i++) {
        z[i] = littleendian(x[4*i], x[4*i+1], x[4*i+2], x[4*i+3]);
        original[i] = z[i];
    }
    
    for (int i = 0; i < 4; i++) {
        doubleround(z);
    }
    
    for (int i = 0; i < 16; i++) {
        z[i] = z[i] + original[i];
        littleendianinverse(z[i], x + 4*i);
    }
}
void expand256(uint8_t k[32], uint8_t n[16], uint8_t state[64]) {
    uint8_t a[] = {101, 120, 112, 97};
    uint8_t b[] = {110, 100, 32, 51};
    uint8_t c[] = {50, 45, 98, 121};
    uint8_t d[] = {116, 101, 32, 107};
    
    for (int i = 0; i < 4; i++) {
        state[i] = a[i];
    }
    for (int i = 0; i < 16; i++) {
        state[4 + i] = k[i];
    }
    for (int i = 0; i < 4; i++) {
        state[20 + i] = b[i];
    }
    for (int i = 0; i < 16; i++) {
        state[24 + i] = n[i];
    }
    for (int i = 0; i < 4; i++) {
        state[40 + i] = c[i];
    }
    for (int i = 0; i < 16; i++) {
        state[44 + i] = k[16 + i];
    }
    for (int i = 0; i < 4; i++) {
        state[60 + i] = d[i];
    }
}

void expand128(uint8_t k[16], uint8_t n[16], uint8_t state[64]) {
    uint8_t a[] = {101, 120, 112, 97};
    uint8_t b[] = {110, 100, 32, 49};
    uint8_t c[] = {54, 45, 98, 121};
    uint8_t d[] = {116, 101, 32, 107};
    
    for (int i = 0; i < 4; i++) {
        state[i] = a[i];
    }
    for (int i = 0; i < 16; i++) {
        state[4 + i] = k[i];
    }
    for (int i = 0; i < 4; i++) {
        state[20 + i] = b[i];
    }
    for (int i = 0; i < 16; i++) {
        state[24 + i] = n[i];
    }
    for (int i = 0; i < 4; i++) {
        state[40 + i] = c[i];
    }
    for (int i = 0; i < 16; i++) {
        state[44 + i] = k[i];
    }
    for (int i = 0; i < 4; i++) {
        state[60 + i] = d[i];
    }
}

void expand64(uint8_t k[8], uint8_t n[16], uint8_t state[64]) {
    uint8_t a[] = {101, 120, 112, 97};
    uint8_t b[] = {110, 100, 32, 48};
    uint8_t c[] = {56, 45, 98, 121};
    uint8_t d[] = {116, 101, 32, 107};
    
    for (int i = 0; i < 4; i++) {
        state[i] = a[i];
    }
    for (int i = 0; i < 8; i++) {
        state[4 + i] = k[i];
    }
    for (int i = 0; i < 8; i++) {
        state[12 + i] = k[i];
    }
    for (int i = 0; i < 4; i++) {
        state[20 + i] = b[i];
    }
    for (int i = 0; i < 16; i++) {
        state[24 + i] = n[i];
    }
    for (int i = 0; i < 4; i++) {
        state[40 + i] = c[i];
    }
    for (int i = 0; i < 8; i++) {
        state[44 + i] = k[i];
    }
    for (int i = 0; i < 8; i++) {
        state[52 + i] = k[i];
    }
    for (int i = 0; i < 4; i++) {
        state[60 + i] = d[i];
    }
}
int hex_to_bytes(string hex, uint8_t *bytes) {
    for (int i = 0; i < (int)hex.length(); i += 2) {
        char c1 = hex[i];
        char c2 = hex[i+1];
        int n1, n2;
        
        if (c1 >= '0' && c1 <= '9') n1 = c1 - '0';
        else if (c1 >= 'a' && c1 <= 'f') n1 = c1 - 'a' + 10;
        else if (c1 >= 'A' && c1 <= 'F') n1 = c1 - 'A' + 10;
        else n1 = 0;
        
        if (c2 >= '0' && c2 <= '9') n2 = c2 - '0';
        else if (c2 >= 'a' && c2 <= 'f') n2 = c2 - 'a' + 10;
        else if (c2 >= 'A' && c2 <= 'F') n2 = c2 - 'A' + 10;
        else n2 = 0;
        
        bytes[i/2] = (n1 << 4) + n2;
    }
    return hex.length() / 2;
}

string bytes_to_hex(uint8_t *bytes, int len) {
    string result = "";
    for (int i = 0; i < len; i++) {
        char b[3];
        snprintf(b, 3, "%02x", bytes[i]);
        result += b;
    }
    return result;
}

void encrypt_decrypt(int keylen, uint8_t *key, uint8_t *nonce, uint8_t *input, int inputlen, uint8_t *output) {
    uint8_t state[64];
    uint8_t counter[16];
    
    for (int i = 0; i < 8; i++) {
        counter[i] = nonce[i];
    }
    
    int num = 0;
    int processed = 0;
    
    while (processed < inputlen) {
        for (int i = 0; i < 8; i++) {
            counter[8 + i] = (num >> (8 * i)) & 0xFF;
        }
        
        if (keylen == 256) {
            expand256(key, counter, state);
        } else if (keylen == 128) {
            expand128(key, counter, state);
        } else if (keylen == 64) {
            expand64(key, counter, state);
        }
        
        salsa20(state);
        
        int block_bytes;
        if ((inputlen - processed) < 64) {
            block_bytes = inputlen - processed;
        } else {
            block_bytes = 64;
        }
        
        for (int i = 0; i < block_bytes; i++) {
            output[processed + i] = input[processed + i] ^ state[i];
        }
        
        processed += block_bytes;
        num++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        return 1;
    }
    
    int keylen = stoi(argv[1]);
    string key_hex = argv[2];
    string nonce_hex = argv[3];
    string input_hex = argv[4];
    
    uint8_t key[32];
    uint8_t nonce[16];
    uint8_t input[1024];
    uint8_t output[1024];
    
    hex_to_bytes(key_hex, key);
    hex_to_bytes(nonce_hex, nonce);
    int inputlen = hex_to_bytes(input_hex, input);
    
    encrypt_decrypt(keylen, key, nonce, input, inputlen, output);
    
    string result = bytes_to_hex(output, inputlen);
    cout << "\"" << result << "\"" << endl;
    
    return 0;
}
