#pragma once
#include <string>

namespace mJrKaKkAka {
    static const unsigned char devDevDEV[10] = { 0x6D,0x61,0x6A,0x6F,0x72,0x6B,0x61,0x64,0x65,0x76 };

    static inline std::string DeVDeVdEv(const unsigned char* e, size_t n) {
        std::string mJrKaKkAka;
        mJrKaKkAka.resize(n);
        for (size_t i = 0; i < n; i++)
            mJrKaKkAka[i] = (unsigned char)(e[i] - 32) ^ devDevDEV[i % 10] ^ (unsigned char)(i * 0x13 + 0x37);
        return mJrKaKkAka;
    }
}
