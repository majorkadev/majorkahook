#include "pch.h"
#include "skCrypter.h"
#include "mdevutils.h"
#include <iomanip>
#include <ctime>

static void LogInfo(const char* message) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::cout << "[" << std::put_time(&tm, "%H:%M:%S") << "][" << GetCurrentProcessId() << "][info] " << message << "\n";
}

struct ValveColor {
    unsigned char r, g, b, a;
    ValveColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
};

typedef void(__cdecl* ConColorMsgFn)(const ValveColor&, const char*, ...);

static void PrintToConsole() {
    typedef void* (*CreateInterfaceFn)(const char*, int*);
    CreateInterfaceFn engineFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(skCrypt("engine.dll")), skCrypt("CreateInterface"));
    if (!engineFactory) return;

    void* engine = engineFactory(skCrypt("VEngineClient014"), nullptr);
    if (!engine) return;

    typedef void(__thiscall* ClientCmdFn)(void*, const char*, bool);
    void** vtable = *(void***)engine;
    if (vtable && vtable[114]) {
        ClientCmdFn clientCmd = (ClientCmdFn)vtable[114];
        clientCmd(engine, skCrypt("clear"), false);
        clientCmd(engine, skCrypt("showconsole"), false);
    }

    Sleep(100);

    HMODULE tier0 = GetModuleHandleA(skCrypt("tier0.dll"));
    if (!tier0) return;
    ConColorMsgFn ConColorMsg = (ConColorMsgFn)GetProcAddress(tier0, skCrypt("?ConColorMsg@@YAXABVColor@@PBDZZ"));
    if (!ConColorMsg) return;

    {
        const unsigned char _0[] = { 154,43,55,95,241,253,8,24,170,212,216,105,113,129,51,63,70,94,232,22,30,167,179,7,213,59,132,156,46,40,92,229,253,5,15,187,194,210,108,106,130,59,39,95,73,229,0,16,162,172,192,97,121,137,139,39,62,78,248,246,6,31,187,203,197,97,124,148,38,32,68,93,229,13,7,163,186,202,100,98,138,131,63,87,65,237,248,8,26,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_0, sizeof(_0)).c_str());
        const unsigned char _1[] = { 154,43,55,95,241,253,8,24,170,212,216,105,113,129,51,63,70,94,232,22,30,167,179,250,213,73,132,156,46,40,92,229,253,5,15,187,194,210,108,106,130,59,39,95,73,229,0,16,162,172,192,97,121,137,139,39,62,78,248,246,6,31,187,203,197,97,124,148,38,32,68,93,229,13,7,163,186,202,100,98,138,131,63,87,65,237,248,8,26,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_1, sizeof(_1)).c_str());
        const unsigned char _2[] = { 154,43,55,95,241,253,8,24,170,212,216,105,113,129,51,63,70,94,232,22,30,167,179,195,205,121,132,156,46,40,92,229,253,5,15,187,194,210,108,106,130,59,39,95,73,229,0,16,162,172,192,97,121,137,139,39,62,78,248,246,6,31,187,203,197,97,124,148,38,32,68,93,229,13,7,163,186,202,100,98,138,131,63,87,65,237,248,8,26,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_2, sizeof(_2)).c_str());
        const unsigned char _3[] = { 130,51,47,71,233,191,6,220,178,204,26,103,113,129,43,39,94,70,170,24,30,167,171,219,213,97,132,146,106,48,68,167,243,5,23,163,218,22,116,114,154,59,39,95,73,229,24,8,186,172,216,121,97,145,73,41,62,78,248,246,30,7,249,197,197,97,114,80,62,56,134,83,229,13,9,231,162,210,38,108,138,131,63,87,65,237,224,16,2,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_3, sizeof(_3)).c_str());
        const unsigned char _4[] = { 130,51,47,95,243,229,16,0,170,214,192,113,51,129,51,63,70,92,240,14,220,167,177,219,213,97,64,132,54,42,94,253,229,199,23,163,218,226,110,106,130,59,39,95,73,235,24,8,18,172,192,97,121,139,147,63,124,78,248,246,6,29,163,211,7,37,100,140,36,34,92,69,167,201,31,187,184,200,124,122,72,131,63,87,65,227,224,16,170,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_4, sizeof(_4)).c_str());
        const unsigned char _5[] = { 130,51,47,95,241,229,16,0,170,212,192,113,105,143,119,39,94,70,240,14,6,167,179,219,213,97,156,132,54,40,92,253,229,29,23,163,218,210,108,106,130,59,39,95,81,253,24,187,162,162,4,121,97,145,147,63,38,78,248,246,6,31,163,211,221,121,100,140,38,32,92,69,253,21,31,187,186,202,124,122,146,131,63,87,89,245,224,163,26,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_5, sizeof(_5)).c_str());
        const unsigned char _6[] = { 130,51,47,95,241,229,16,0,170,212,192,113,105,153,43,39,70,94,240,14,6,167,179,219,213,97,61,132,54,38,82,253,229,181,23,163,218,210,108,106,130,59,39,95,73,231,24,8,224,180,216,121,121,137,147,63,38,78,248,246,6,31,163,211,221,88,100,140,40,46,92,69,213,180,31,187,180,196,124,122,58,131,63,87,65,239,224,16,216,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_6, sizeof(_6)).c_str());
        const unsigned char _7[] = { 130,51,47,95,241,229,16,0,170,212,192,113,105,131,138,39,94,70,240,14,6,167,179,219,213,97,132,158,151,48,68,213,255,5,23,163,218,210,108,106,130,59,39,95,73,229,24,8,186,174,249,121,97,145,147,63,38,78,248,246,6,31,163,211,221,97,126,45,62,56,116,95,229,13,5,26,162,210,84,96,138,131,63,87,65,237,224,16,2,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_7, sizeof(_7)).c_str());
        const unsigned char _8[] = { 154,43,55,95,241,253,8,24,170,212,216,105,113,129,51,63,70,94,232,22,30,167,179,219,213,97,132,156,46,40,92,229,253,5,15,187,194,210,108,106,130,35,63,71,73,229,0,16,162,172,192,97,121,137,139,39,62,86,224,238,6,31,187,203,197,97,124,148,38,32,68,93,229,13,7,163,186,202,100,98,138,155,39,79,65,237,248,8,26,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_8, sizeof(_8)).c_str());
        const unsigned char _9[] = { 154,43,55,95,241,253,8,24,170,212,216,105,113,129,51,63,70,94,232,22,30,167,247,219,213,73,132,156,46,40,92,229,253,5,15,187,194,210,108,106,130,35,63,71,73,229,0,16,162,172,192,97,121,137,139,39,62,86,224,238,6,31,187,203,197,97,124,148,38,32,68,93,229,13,7,163,186,202,100,98,138,155,39,79,65,237,248,8,26,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_9, sizeof(_9)).c_str());
        const unsigned char _10[] = { 154,43,55,95,241,253,8,24,170,212,216,105,113,129,51,63,70,94,232,22,6,191,171,243,207,121,132,156,46,40,92,229,253,5,15,187,194,210,108,106,130,35,63,71,73,229,0,16,162,172,192,97,121,137,139,39,62,86,224,238,6,31,187,203,197,97,124,148,38,32,68,93,229,13,7,163,186,202,100,98,138,155,39,79,65,237,248,8,26,164,162 };
        ConColorMsg(ValveColor(255, 120, 120), mJrKaKkAka::DeVDeVdEv(_10, sizeof(_10)).c_str());
        const unsigned char _11[] = {112,65};
        ConColorMsg(ValveColor(255, 255, 255), mJrKaKkAka::DeVDeVdEv(_11, sizeof(_11)).c_str());
        const unsigned char _12[] = {33,102,118,149,190,175,195,217,226,27,23,34,76,129};
        ConColorMsg(ValveColor(153, 204, 50), mJrKaKkAka::DeVDeVdEv(_12, sizeof(_12)).c_str());
        const unsigned char _13[] = {91,103,123,95,161,188,220,219,226,17,11,105,48,81,99,115,143,155,172,252};
        ConColorMsg(ValveColor(255, 255, 255), mJrKaKkAka::DeVDeVdEv(_13, sizeof(_13)).c_str());
        const unsigned char _14[] = {33,108,118,146,180,174,205,214,249,17,229,105};
        ConColorMsg(ValveColor(153, 204, 50), mJrKaKkAka::DeVDeVdEv(_14, sizeof(_14)).c_str());
        const unsigned char _15[] = {82,126,112,154,241,169,192,217,228,31,11,105,55,83,124,114,70,147,169,220,209,245,248,2,9,60,82,156,122,103,92,160,174,206,219,187,4,29,62,106,64,98,113,137,128,171,199,16,245,229,19,41,119,99,97};
        ConColorMsg(ValveColor(255, 255, 255), mJrKaKkAka::DeVDeVdEv(_15, sizeof(_15)).c_str());
    }
}
static void thread(HMODULE base) {
    AllocConsole();
    const unsigned char _title[] = { 87,106,125,144,163,182,201,208,229,27,19 };
    SetConsoleTitleA(mJrKaKkAka::DeVDeVdEv(_title, sizeof(_title)).c_str());
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 160, 30 };
    SetConsoleScreenBufferSize(hConsole, coord);
    SMALL_RECT rect = { 0, 0, 159, 29 };
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
    freopen(skCrypt("CONOUT$"), "w", stdout);
    system("cls");

    {
        const unsigned char _[] = { 83,101,126,139,184,188,196,209,240,29,22,46,127,143,61 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }
    skeet_t* skeet = skeet_t::getInstance(base);
    if (!skeet->map())
        return;

    {
        const unsigned char _[] = { 95,120,99,148,241,191,193,214,235,6,1,105,60,64,99,111,131,154,232,197,203,228,240,6,30,42,66,73,98,100,133,229,245,21,215,169,228,241,124,122,146,35,125,158,157,160,211,16,227,248,192,113,33,157,152,52,47,70,240,254,22,22,181 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }

    {
        const unsigned char _[] = { 72,110,100,144,189,171,193,214,237,212,17,36,33,78,97,107,149,94,169,216,218,167,225,6,15,44,77,80,106,97,146,162,253,172,194,235,13,0,56,106,35,103,123,149,140,182,211,16,22,237,2,45,60,137,131,142,159,98,233,224,8,17 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }
    skeet->fix_imports();
    {
        const unsigned char _[] = { 51,138,131,95,163,184,202,205,227,24,12,105,34,84,112,124,131,141,187,208,203,235,255,26,204,121,37,80,98,40,120,201,209,5,198,246,18,29,62,62,81,35,109,130,154,170,204,198,231,232,206 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }

    {
        const unsigned char _[] = { 91,123,103,147,168,180,198,223,170,25,29,36,62,83,106,63,150,159,188,213,214,226,224,195,12,55,64,156,102,103,147,174,180,203,200,187,230,234,117,101,39,109,120,142,135,160,0,192,237,229,14,53,60,91,88,41,48,88 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }
    skeet->extra();
    {
        const unsigned char _[] = { 87,110,122,144,163,164,8,200,235,0,27,33,52,82,51,126,136,154,232,211,208,224,250,13,8,121,76,83,97,99,143,229,188,213,223,247,11,23,40,106,81,118,124,132,140,182,211,214,247,224,12,56,119 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }

    {
        const unsigned char _[] = { 53,128,57 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }
    {
        const unsigned char _[] = { 33,108,118,146,180,174,205,214,249,17,229,105,57,84,116,122,70,138,160,215,208,236,224,195,11,43,75,81,46,101,157,175,178,215,196,250,6,23,58,106,86,108,63,130,154,174,212,16,228,227,18,97,59,72,69,105,119,152,167,238,209,214,232,3,203 };
        LogInfo(mJrKaKkAka::DeVDeVdEv(_, sizeof(_)).c_str());
    }
    std::cout.flush();

    skeet->entry();

    Sleep(500);
    PrintToConsole();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(thread), hModule, 0, 0));
        break;
    }
    return TRUE;
}