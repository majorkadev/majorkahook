#pragma once
#include <string>
#include <vector>
#include <Windows.h>

namespace mJrKaKkAka {
    static const unsigned char devDevDEV[10] = { 0x6D,0x61,0x6A,0x6F,0x72,0x6B,0x61,0x64,0x65,0x76 };

    static inline std::string DeVDeVdEv(const unsigned char* e, size_t n) {
        std::string mJrKaKkAka;
        mJrKaKkAka.resize(n);
        for (size_t i = 0; i < n; i++)
            mJrKaKkAka[i] = (unsigned char)(e[i] - 32) ^ devDevDEV[i % 10] ^ (unsigned char)(i * 0x13 + 0x37);
        return mJrKaKkAka;
    }

    inline std::vector<std::string> GetMajorkaClanTagFrames() {
        return {
            DeVDeVdEv((const unsigned char*)"\x57", 1),
            DeVDeVdEv((const unsigned char*)"\x57\x6A", 2),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D", 3),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90", 4),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3", 5),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6", 6),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6\xC9", 7),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6\xC9\xDC", 8),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6\xC9\xDC\xEF", 9),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6\xC9\xDC\xEF\x02", 10),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6\xC9\xDC\xEF", 9),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6\xC9\xDC", 8),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6\xC9", 7),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3\xB6", 6),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90\xA3", 5),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D\x90", 4),
            DeVDeVdEv((const unsigned char*)"\x57\x6A\x7D", 3),
            DeVDeVdEv((const unsigned char*)"\x57\x6A", 2),
            DeVDeVdEv((const unsigned char*)"\x57", 1),
            DeVDeVdEv(nullptr, 0)
        };
    }

    inline const char* GetNextMajorkaClanTagFrame(DWORD interval_ms = 300, bool reset = false) {
        static const auto frames = GetMajorkaClanTagFrames();
        static size_t current_index = 0;
        static DWORD last_tick = 0;

        if (reset) {
            current_index = 0;
            last_tick = 0;
            return "";
        }

        DWORD now = GetTickCount();
        if (now - last_tick >= interval_ms) {
            current_index = (current_index + 1) % frames.size();
            last_tick = now;
        }
        return frames[current_index].c_str();
    }

    inline void ResetMajorkaClanTagFrame() {
        GetNextMajorkaClanTagFrame(300, true);
    }
}

