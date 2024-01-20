#pragma once


String hexify_byte(uint8 b) {
    if (b < 16) return "0" + String(b, HEX);
    return String(b, HEX);
}

uint8 unhexify_byte(char* b, int offset) {
    uint8 res = 0;
    
    uint8 half1 = b[0 + offset];
    uint8 half2 = b[1 + offset];

    half1 -= '0';
    half2 -= '0';

    if (half1 > 9) half1 -= ('A' - '0') - 10;
    if (half2 > 9) half2 -= ('A' - '0') - 10;

    return half2 | (half1 << 4);
}

uint8 unhexify_byte(String b, int offset) {
    uint8 res = 0;
    
    b.toUpperCase();
    uint8 half1 = b[0 + offset];
    uint8 half2 = b[1 + offset];

    half1 -= '0';
    half2 -= '0';

    if (half1 > 9) half1 -= ('A' - '0') - 10;
    if (half2 > 9) half2 -= ('A' - '0') - 10;

    return half2 | (half1 << 4);
}

uint8 unhexify_byte(String b) {
    return unhexify_byte(b, 0);
}

uint64 unhexify_int(String b) {
    uint64 res = 0;
    for (int i = 0; i < b.length(); i += 2) {
        res = res << 8;
        res |= unhexify_byte(b, i);
    } 
    return res;
}

uint64 unhexify_int(byte* b, int length) {
    uint64 res = 0;
    for (int i = 0; i < length; i += 2) {
        res = res << 8;
        res |= unhexify_byte((char*)b, i);
    } 
    return res;
}