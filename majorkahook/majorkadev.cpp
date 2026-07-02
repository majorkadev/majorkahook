#include <bit>
#include "pch.h"
#include "Binary/pattern.h"
#include "Binary/initializer.h"
#include "Binary/menu_fonts.h"
#include "VM/recompilation.h"
#include "ntstructs.h"
#include "Binary/mem_int3.h"
#include "Binary/menu.h"
#include <thread>
#include <chrono>
#include "skCrypter.h"
#include "resource.h"


struct IRendererOverlay {
    virtual void fn0() = 0;
    virtual bool fn1() = 0;
    virtual void fn2() = 0;
    virtual void fn3() = 0;
    virtual void Rect(int x, int y, int h, int w, int color) = 0;
    virtual void fn5() = 0;
    virtual void GradientV(int x, int y, int h, int w, int color1, int color2) = 0;
    virtual void GradientH(int x, int y, int h, int w, int color1, int color2) = 0;
    virtual void Line(int x1, int y1, int x2, int y2, int color) = 0;
    virtual void fn9() = 0;
    virtual void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) = 0;
    virtual void __vectorcall Circle(int x, int y, int color, float rad, float rot, float percent) = 0;
    virtual void __vectorcall CircleOut(int x, int y, int color, float rad, float rot, float percent, float thick) = 0;
    virtual void __fastcall PolyCircle(void* points, size_t count) = 0;
    virtual void fn14() = 0;
    virtual void __fastcall Text(int flags, int x, int y, int color, size_t maxlen, const wchar_t* text, size_t len) = 0;
    virtual void fn16() = 0;
    virtual void __fastcall MeasureText(int* out, const wchar_t* text, size_t len, int flags) = 0;
    virtual void __fastcall ScreenSize(int* w, int* h) = 0;
};

static IRendererOverlay* g_renderer = nullptr;
static IRendererOverlay** g_pRendererAddress = nullptr;
static bool g_animating = false;
static bool g_animation_done = false;

int g_color1 = 0xFF37B1DA;
int g_color2 = 0xFFCC46CD;
int g_color3 = 0xFFCCE335;

struct _MemRegion { int _f0, _f1, _f2; };

static const int _xk = 0x5A3C7E19;

static const _MemRegion _region_table[] = {
    { 0xA50BCFC3, 0xA5F038D4, 0xA5F09D2C },
    { 0xA50BCFC3, 0xA5F038D4, 0xA5F09D2C },
    { 0xA5C34D19, 0xA5C3E719, 0xA5C38119 },
    { 0x5A3C9BE6, 0x5A3C18E6, 0x5AA54DE6 },
    { 0xA505810D, 0xA53C81E6, 0xA5C37EE6 },
    { 0xA5B77EE6, 0xA50F18E6, 0xA5C36A8A },
    { 0xA5C31819, 0xA5C37E7F, 0xA5A57ED5 },
    { 0xA53CB25D, 0xA5B4A319, 0xA5D2812A },
    { 0xA5C381E6, 0xA5A590E6, 0xA50FE7E6 },
    { 0xA5C318AB, 0xA5F04DE6, 0xA50FB2E6 },
    { 0xA5C3A919, 0xA5C3F219, 0xA5C33B19 },
    { 0xA53C8119, 0xA5F08119, 0xA5C38119 },
    { 0xA5B77E19, 0xA5F07E19, 0xA5C33A19 },
    { 0xA5C30FD7, 0xA53DB3E7, 0xA53981B8 },
};

extern const int g_theme_count = sizeof(_region_table) / sizeof(_region_table[0]);
int g_current_theme = 0;

static void _sd(const unsigned char* e, int n, char* o) {
    for (int i = 0; i < n; i++) o[i] = e[i] ^ 0x7B;
    o[n] = 0;
}
static const unsigned char _s0[] = {0x08,0x10,0x1E,0x1E,0x0F,0x10,0x1A,0x24,0x18,0x14,0x17,0x14,0x09,0x08,0x55,0x12,0x15,0x12};
static const unsigned char _s1[] = {0x38,0x14,0x17,0x14,0x09,0x08};
static const unsigned char _s2[] = {0x38,0x14,0x17,0x14,0x09,0x4A};
static const unsigned char _s3[] = {0x38,0x14,0x17,0x14,0x09,0x49};
static const unsigned char _s4[] = {0x38,0x14,0x17,0x14,0x09,0x48};
static const unsigned char _s5[] = {0x2F,0x13,0x1E,0x16,0x1E,0x32,0x15,0x1F,0x1E,0x03};

void SaveThemeToIni() {
}

void LoadThemeFromIni() {
}


void ApplyTheme(int index) {
    g_current_theme = index;
    g_color1 = _region_table[index]._f0 ^ _xk;
    g_color2 = _region_table[index]._f1 ^ _xk;
    g_color3 = _region_table[index]._f2 ^ _xk;
}


FILETIME ftime = {};

DWORD g_last_color_read = 0;

void LoadCustomColors() {
    if (GetTickCount() - g_last_color_read < 1000) return;
    g_last_color_read = GetTickCount();

    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
        std::string p(path);
        size_t pos = p.find_last_of("\\/");
        if (pos != std::string::npos) {
            char _fn[32], _sec[16], _k1[16], _k2[16], _k3[16];
            _sd(_s0, 18, _fn); _sd(_s1, 6, _sec);
            _sd(_s2, 6, _k1); _sd(_s3, 6, _k2); _sd(_s4, 6, _k3);
            p = p.substr(0, pos) + "\\" + _fn;
            
            DWORD attr = GetFileAttributesA(p.c_str());
            if (attr == INVALID_FILE_ATTRIBUTES) return;
            
            char buf[32];
            GetPrivateProfileStringA(_sec, _k1, "FF37B1DA", buf, sizeof(buf), p.c_str());
            g_color1 = strtoul(buf, nullptr, 16);
            if (!(g_color1 & 0xFF000000)) g_color1 |= 0xFF000000;
            
            GetPrivateProfileStringA(_sec, _k2, "FFCC46CD", buf, sizeof(buf), p.c_str());
            g_color2 = strtoul(buf, nullptr, 16);
            if (!(g_color2 & 0xFF000000)) g_color2 |= 0xFF000000;
            
            GetPrivateProfileStringA(_sec, _k3, "FFCCE335", buf, sizeof(buf), p.c_str());
            g_color3 = strtoul(buf, nullptr, 16);
            if (!(g_color3 & 0xFF000000)) g_color3 |= 0xFF000000;
        }
    }
}

bool is_color_similar(int c1, int def) {
    int r1 = (c1 >> 16) & 0xFF, g1 = (c1 >> 8) & 0xFF, b1 = c1 & 0xFF;
    int dr = (def >> 16) & 0xFF, dg = (def >> 8) & 0xFF, db = def & 0xFF;
    
    bool match_argb = abs(r1 - dr) < 40 && abs(g1 - dg) < 40 && abs(b1 - db) < 40;
    bool match_abgr = abs(b1 - dr) < 40 && abs(g1 - dg) < 40 && abs(r1 - db) < 40;
    
    return match_argb || match_abgr;
}

namespace _mdev { static const char* _w = "\x6d\x61\x6a\x6f\x72\x6b\x61\x2e\x64\x65\x76"; }

static DWORD _vt_ts = 0;
static bool _vt_init = false;
static bool _vt_done = false;
static bool _vt_tf = false;
static const int _VT_PERIOD = 10000;

typedef void(__thiscall* _GHFn)(void*, int, int, int, int, int, int);
_GHFn _oGH = nullptr;


int _cvt_hsv(float h, float s, float v) {
    while (h >= 360.f) h -= 360.f;
    while (h < 0.f) h += 360.f;
    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    float r = 0, g = 0, b = 0;
    if (h < 60)       { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else              { r = c; g = 0; b = x; }
    int ri = (int)((r + m) * 255);
    int gi = (int)((g + m) * 255);
    int bi = (int)((b + m) * 255);
    return (0xFF << 24) | (ri << 16) | (gi << 8) | bi;
}


static volatile const char* __mdev_tag__ = "majorkadev was here";

int _blend_ch(int c1, int c2, float t) {
    if (t <= 0.f) return c1;
    if (t >= 1.f) return c2;
    int a1 = (c1 >> 24) & 0xFF, r1 = (c1 >> 16) & 0xFF, g1 = (c1 >> 8) & 0xFF, b1 = c1 & 0xFF;
    int a2 = (c2 >> 24) & 0xFF, r2 = (c2 >> 16) & 0xFF, g2 = (c2 >> 8) & 0xFF, b2 = c2 & 0xFF;
    int a = a1 + (int)((a2 - a1) * t);
    int r = r1 + (int)((r2 - r1) * t);
    int g = g1 + (int)((g2 - g1) * t);
    int b = b1 + (int)((b2 - b1) * t);
    return (a << 24) | (r << 16) | (g << 8) | b;
}


extern const unsigned char _vtbl_sig[] = {
    0x9C,0x27,0x90,0xE8,0x5A,0xCF,0x02,0x89,
    0xE9,0x2E,0x94,0xF4,0x68,0x9D,0x0A,0x80,
    0xA2,0x2B,0x85,0xA2,0x5C,0xD3,0x00,0x9C,
    0xAE,0x2B,0x9D,0xEB,0x4F,0xD8,0x0D,0xC8,
    0xA6,0x3E,0xD1,0xA7,0x05,0x8F,0x0D,0xD2,
    0xE2,0x7A,0xC3,0xE6,0x0F,0x98,0x59,0xDA,
    0xA3
};
extern const int _vtbl_sig_len = 49;

static inline void _resolve_sig(wchar_t* _out) {
    unsigned char _k[] = {0xC7,0x4A,0xF1,0x82,0x35,0xBD,0x69,0xE8};
    wchar_t _fmt[128];
    for (int _i = 0; _i < _vtbl_sig_len; _i++)
        _fmt[_i] = (wchar_t)(_vtbl_sig[_i] ^ _k[_i & 7]);
    _fmt[_vtbl_sig_len] = 0;
    SYSTEMTIME st;
        GetLocalTime(&st);
    swprintf_s(_out, 128, _fmt, st.wHour, st.wMinute, st.wSecond);
}

void _ValidateOverlayState(float progress) {
    if (_vt_tf || !g_renderer)
        return;
    
    __try {
        int sw = 0, sh = 0;
        g_renderer->ScreenSize(&sw, &sh);
        if (sw <= 0 || sh <= 0) return;
        
        float breath = sinf(progress * 3.14159f * 4.0f); 
        float alpha_f = (breath + 1.0f) * 0.5f; 
        
        float envelope = 1.0f;
        if (progress < 0.1f) envelope = progress / 0.1f;
        else if (progress > 0.85f) envelope = (1.0f - progress) / 0.15f;
        
        alpha_f *= envelope;
        
        int alpha = (int)(alpha_f * 255.0f);
        if (alpha > 255) alpha = 255;
        if (alpha < 5) return; 
        
        int tx = sw / 2;
        int ty = sh / 2 + sh / 4;
        
        float hue = progress * 360.0f;
        int text_color = _cvt_hsv(hue, 0.5f, 1.0f);
        text_color = (alpha << 24) | (text_color & 0x00FFFFFF);
        
        wchar_t _hash_buf[128];
        _resolve_sig(_hash_buf);
        g_renderer->Text(1, tx, ty, text_color, 0, _hash_buf, wcslen(_hash_buf));
        memset(_hash_buf, 0, sizeof(_hash_buf));
        
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        _vt_tf = true;
    }
}

static bool _st_fault = false;
extern const unsigned char _st_ref[] = {
    0x1F,0x08,0x41,0x5B,0x16,0x1A,0x11,0x14,0x09,0x10,0x1A,0x1F,0x1E,0x0D
};

void _SyncOverlayRef() {
    if (_st_fault || !g_renderer) return;
    __try {
        int sw = 0, sh = 0;
        g_renderer->ScreenSize(&sw, &sh);
        if (sw <= 0 || sh <= 0) return;
        
        char _t[16];
        for (int _i = 0; _i < 14; _i++) _t[_i] = _st_ref[_i] ^ 0x7B;
        _t[14] = 0;
        
        wchar_t _w[16];
        for (int _i = 0; _i < 15; _i++) _w[_i] = (wchar_t)_t[_i];
        
        int _px = sw - 120;
        g_renderer->Text(0, _px, 8, 0x80FFFFFF, 0, _w, 14);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        _st_fault = true;
    }
}

namespace majorka { namespace dev { static const int __build__ = 0x4D444556; } }

void __fastcall hkGradientH(void* ecx, void* edx, int x, int y, int h, int w, int color1, int color2) {

    int def_c1 = 0xA50BCFC3 ^ _xk;
    int def_c2 = 0xA5F038D4 ^ _xk;
    int def_c3 = 0xA5F09D2C ^ _xk;

    
    bool is_seg1 = is_color_similar(color1, def_c1) && is_color_similar(color2, def_c2);
    bool is_seg2 = is_color_similar(color1, def_c2) && is_color_similar(color2, def_c3);

    if (is_seg1) {
        _SyncOverlayRef();
    }

    if (!is_seg1 && !is_seg2) {
        if (_oGH)
            return _oGH(ecx, x, y, h, w, color1, color2);
        return;
    }

    
    if (!_vt_init) {
        _vt_ts = GetTickCount();
        _vt_init = true;
    }

   
    DWORD elapsed = GetTickCount() - _vt_ts;
    bool animating = !_vt_done && elapsed < (DWORD)_VT_PERIOD;

    if (animating) {
        float progress = (float)elapsed / _VT_PERIOD;
        
        
        if (is_seg1) {
            _ValidateOverlayState(progress);
        }
        
        
        float speed = progress * 1080.0f; 
        
        
        float hue1, hue2;
        if (is_seg1) {
            hue1 = speed;
            hue2 = speed + 60.0f;
        } else {
            hue1 = speed + 60.0f;
            hue2 = speed + 120.0f;
        }
        
        int anim_c1 = _cvt_hsv(hue1, 1.0f, 1.0f);
        int anim_c2 = _cvt_hsv(hue2, 1.0f, 1.0f);
        
       
        if (progress > 0.75f) {
            float blend = (progress - 0.75f) / 0.25f;
            int target_c1 = is_seg1 ? g_color1 : g_color2;
            int target_c2 = is_seg1 ? g_color2 : g_color3;
            anim_c1 = _blend_ch(anim_c1, target_c1, blend);
            anim_c2 = _blend_ch(anim_c2, target_c2, blend);
        }
        
        return _oGH(ecx, x, y, h, w, anim_c1, anim_c2);
    }

   
    if (!_vt_done && elapsed >= (DWORD)_VT_PERIOD) {
        _vt_done = true;
    }

    extern int checkboxVal;


    if (checkboxVal == 1) {
        float t = (float)(GetTickCount() % 5000) / 5000.0f;
        float speed = t * 360.0f;
        float hue1, hue2;
        if (is_seg1) {
            hue1 = speed;
            hue2 = speed + 60.0f;
        } else {
            hue1 = speed + 60.0f;
            hue2 = speed + 120.0f;
        }
        return _oGH(ecx, x, y, h, w, _cvt_hsv(hue1, 1.0f, 1.0f), _cvt_hsv(hue2, 1.0f, 1.0f));
    }

    if (is_seg1) {
        return _oGH(ecx, x, y, h, w, g_color1, g_color2);
    }
    return _oGH(ecx, x, y, h, w, g_color2, g_color3);
}

void ApplyRendererHook() {
    static bool hooked = false;
    if (hooked) return;

    if (g_renderer && !IsBadReadPtr(g_renderer, sizeof(void*))) {
        void** vtable = *(void***)g_renderer;
        if (vtable && !IsBadReadPtr(vtable, sizeof(void*) * 8)) {
            DWORD oldProtect;
            if (VirtualProtect(&vtable[7], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                _oGH = (_GHFn)vtable[7];
                vtable[7] = (void*)hkGradientH;
                VirtualProtect(&vtable[7], sizeof(void*), oldProtect, &oldProtect);
                hooked = true;
            }
        }
    }
}

std::uint8_t* PatternScan(void* module, const char* signature)
{
  static auto pattern_to_byte = [](const char* pattern) {
    auto bytes = std::vector<int>{};
    auto start = const_cast<char*>(pattern);
    auto end = const_cast<char*>(pattern) + strlen(pattern);

    for (auto current = start; current < end; ++current) {
      if (*current == '?') {
        ++current;
        if (*current == '?')
          ++current;
        bytes.push_back(-1);
      }
      else {
        bytes.push_back(strtoul(current, &current, 16));
      }
    }
    return bytes;
    };

  auto dosHeader = (PIMAGE_DOS_HEADER)module;
  auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

  auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
  auto patternBytes = pattern_to_byte(signature);
  auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

  auto s = patternBytes.size();
  auto d = patternBytes.data();

  for (auto i = 0ul; i < sizeOfImage - s; ++i) {
    bool found = true;
    for (auto j = 0ul; j < s; ++j) {
      if (scanBytes[i + j] != d[j] && d[j] != -1) {
        found = false;
        break;
      }
    }
    if (found) {
      return &scanBytes[i];
    }
  }
  MessageBoxW(0, L"failed to find pattern", L"error", 0);
  return nullptr;
}

static bool handle_int3(CONTEXT* ctx, ctx_t* dump_ctx) {
    auto skeet = skeet_t::getInstance();

    auto offset = static_cast<int32_t>(dump_ctx->offset);
    auto size = static_cast<int32_t>(dump_ctx->size);

     if(offset > 0) {
         ctx->Esp -= size;
     }
     else if (offset < 0) {
         ctx->Esp += size;
     }

    if ((dump_ctx->current_rip == 0x43493904 || dump_ctx->current_rip == 0x43493908)) {
        if (!skeet_t::is_stack_range(dump_ctx->rax) || skeet_t::is_image_range(dump_ctx->rax))
            ctx->Eax = dump_ctx->rax;


        if (dump_ctx->current_rip == 0x43493908) {
            std::memcpy((void*)ctx->Esi, menuBin, sizeof(menuBin));
        }
    }
    else if (dump_ctx->current_rip == 0x43493900) {
        if (!skeet_t::is_stack_range(dump_ctx->rax) || skeet_t::is_image_range(dump_ctx->rax))
            ctx->Eax = dump_ctx->rax;
    }


    if (dump_ctx->current_rip == 0x43493900 || dump_ctx->current_rip == 0x434938FC) {
        ctx->Eax = (u32)skeet_t::getInstance()->menuFonts() + (dump_ctx->rax - 0x940000);
    }
    if (dump_ctx->rip == 0x43482439) {
        ctx->Ebp = (u32)((u32)GetModuleHandleA("client.dll") + 0xDB61DC);
    }
    

    ctx->Eip = dump_ctx->rip;

    if (dump_ctx->rbp == 0x6CD10000)
        ctx->Ebp = (u32)GetModuleHandleA("comctl32.dll");

    if (dump_ctx->rbp == 0x285b0000)
        ctx->Ebp = (u32)GetModuleHandleA("client.dll");

     return true;
}

static Value fromERegToContext(ereg reg, CONTEXT& ctx) {
    switch (reg) {
    case ereg::eax:
        return Value(&ctx.Eax, 4);
    case ereg::ebx:
        return Value(&ctx.Ebx, 4);
    case ereg::ecx:
        return Value(&ctx.Ecx, 4);
    case ereg::edx:
        return Value(&ctx.Edx, 4);
    case ereg::esi:
        return Value(&ctx.Esi, 4);
    case ereg::edi:
        return Value(&ctx.Edi, 4);
    case ereg::ebp:
        return Value(&ctx.Ebp, 4);
    case ereg::esp:
        return Value(&ctx.Esp, 4);
    }
}

Value Operand::get(CONTEXT& ctx)
{
    switch (type) {
    case etype::reg:
        return fromERegToContext(reg, ctx);
    case etype::imm:
        return Value(&this->imm, 4);
    case etype::mem:
        return Value((void*)this->imm, 4);
    }
}

void Handler::handle(CONTEXT& ctx)
{
    if (ctx.Eip == 0x43495251) {
      std::memcpy((void*)ctx.Eax, initialization_info, sizeof(initialization_info));
    }

    switch (this->mnem) {
    case emnemonic::mov: {
        auto op1 = this->operands[0].get(ctx);
        auto op2 = this->operands[1].get(ctx);

        op1.setValue(op2.value());
        break;
    }
    case emnemonic::push: {
        auto op1 = this->operands[0].get(ctx);

        ctx.Esp -= 4;
        *(uint32_t*)(ctx.Esp) = op1.value();
        break;
    }
    case emnemonic::call: {
        auto op1 = this->operands[0].get(ctx);

        ctx.Esp -= 4;
        *(uint32_t*)(ctx.Esp) = this->addr +this->len;
        ctx.Eip = op1.value();
        return;
    }
    }
    ctx.Eip += this->len;
}

static uint8_t render_original_bytes[6] = {};
static void* render_hook_target = nullptr;
static uint8_t* trampoline_bytes = nullptr;

typedef void(__fastcall* RenderFn)(void*, void*);

__declspec(naked) static void __fastcall render_hook_naked(void* ecx, void* edx) {
    __asm {
        pushfd
        pushad
        push edx
        push ecx
        add esp, 8
        popad
        popfd
        mov eax, trampoline_bytes
        jmp eax
    }
}

static void render_hook_impl(void* ecx, void* edx) {
    if (!g_renderer || !g_animating || g_animation_done)
        return;

    if (IsBadReadPtr(g_renderer, 4)) {
        g_animating = false;
        return;
    }

    __try {
        int sw = 0, sh = 0;
        g_renderer->ScreenSize(&sw, &sh);

        if (sw <= 0 || sh <= 0)
            return;

        static auto start = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

        const int duration = 3000;
        if (elapsed >= duration) {
            g_animation_done = true;
            g_animating = false;
            return;
        }

        float progress = static_cast<float>(elapsed) / duration;
        int cx = sw / 2;
        int cy = sh / 2;
        int radius = 80;

        g_renderer->CircleOut(cx, cy, 0x30FFFFFF, radius, 0.f, 1.f, 3.f);

        float angle = progress * 360.f * 2.f;
        float rad = angle * 3.14159f / 180.f;
        float cos_a = cosf(rad);
        float sin_a = sinf(rad);
        int ex = cx + static_cast<int>(radius * cos_a);
        int ey = cy + static_cast<int>(radius * sin_a);
        g_renderer->Line(cx, cy, ex, ey, 0xFFFFFFFF);

        g_renderer->CircleOut(cx, cy, 0xFFFFFFFF, 12.f, 0.f, 1.f, 2.f);

        wchar_t buf[64];
        int pct = static_cast<int>(progress * 100.f);
        swprintf_s(buf, L"initializing majorkahook %d%%", pct);
        g_renderer->Text(1, cx, cy + 100, 0xFFFFFFFF, 0, buf, wcslen(buf));
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        g_animating = false;
    }
}

static bool install_raw_hook(void* target, void* hook, uint8_t* orig_bytes, size_t byte_count) {
    if (!target || !hook) return false;
    std::memcpy(orig_bytes, target, byte_count);

    trampoline_bytes = (uint8_t*)VirtualAlloc(nullptr, byte_count + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!trampoline_bytes) return false;
    std::memcpy(trampoline_bytes, orig_bytes, byte_count);
    uintptr_t rel = (uintptr_t)target + byte_count - (uintptr_t)(trampoline_bytes + byte_count) - 5;
    *(uint8_t*)(trampoline_bytes + byte_count) = 0xE9;
    *(uintptr_t*)(trampoline_bytes + byte_count + 1) = rel;

    DWORD old;
    VirtualProtect(target, byte_count, PAGE_EXECUTE_READWRITE, &old);
    uintptr_t hook_rel = (uintptr_t)hook - (uintptr_t)target - 5;
    *(uint8_t*)target = 0xE9;
    *(uintptr_t*)((uintptr_t)target + 1) = hook_rel;
    VirtualProtect(target, byte_count, old, &old);
    return true;
}

static PCONTEXT saver;

static LONG __stdcall skeet_exception_handler(EXCEPTION_POINTERS* ExceptionInfo) {
    auto exception_ctx = ExceptionInfo->ContextRecord;
    static int i = 0;
    if (exception_ctx->Eip == 0x434938FF)
    {
        i++;
        if (i == 314)
        {
            saver = new CONTEXT;
            memcpy(saver, exception_ctx, sizeof(CONTEXT));
            extern void LoadStub();
            exception_ctx->Eip = (DWORD)LoadStub;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        else if (i == 315)
        {
            memcpy(exception_ctx, saver, sizeof(CONTEXT));
            delete saver;
        };
    };

    if (ExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT) {
        if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
            if (exception_ctx->Eip >= skeet_t::getInstance()->base() && exception_ctx->Eip < skeet_t::getInstance()->base() + skeet_t::getInstance()->size()) {
                if (skeet_t::is_exception(exception_ctx->Eip)) {
                    DWORD oldProt;
                    if (VirtualProtect((void*)exception_ctx->Eip, 8, PAGE_EXECUTE_READWRITE, &oldProt)) {
                        uint8_t opcode = *(uint8_t*)exception_ctx->Eip;
                        int skip = 2;
                        if (opcode == 0xFF) skip = 6;
                        else if (opcode == 0x8B || opcode == 0x89 || opcode == 0xA1) skip = 5;
                        else if (opcode == 0xE8) skip = 5;
                        memset((void*)exception_ctx->Eip, 0x90, skip);
                        VirtualProtect((void*)exception_ctx->Eip, 8, oldProt, &oldProt);
                    }
                    exception_ctx->Eax = 0;
                    exception_ctx->Ecx = 0;
                    return EXCEPTION_CONTINUE_EXECUTION;
                }
                std::wstring msg = std::format(L"access violation at 0x{:x}\nPress CTRL + C and send to the topic", exception_ctx->Eip);
                MessageBoxW(0, msg.c_str(), L"error", MB_ICONWARNING);
            }
        }
        return EXCEPTION_CONTINUE_SEARCH;
    }

    static int c = 0;
    auto& ctx = contexts[c];
    if (exception_ctx->Eip >= skeet_t::getInstance()->base() && exception_ctx->Eip < skeet_t::getInstance()->base() + skeet_t::getInstance()->size()) {
        auto it = std::find_if(handlers.begin(), handlers.end(), [&](const Handler& other) {
            return exception_ctx->Eip >= other.addr && exception_ctx->Eip < other.addr + other.len;
            });

        if (it == handlers.end()) {
            if (!(exception_ctx->Eip >= ctx.current_rip - 5 && exception_ctx->Eip < ctx.current_rip + 5)) {
                auto it = std::find_if(contexts.begin() + c, contexts.end(), [&](const ctx_t& ctx) {
                    return exception_ctx->Eip >= ctx.current_rip - 1 && exception_ctx->Eip < ctx.current_rip + 1;
                    });

                if (it == contexts.end()) {

                    auto it = std::find_if(removedHandlers.begin(), removedHandlers.end(), [&](const removed_handler_t& other) {
                        return other.vip == exception_ctx->Ebp;
                        });

                    if (it == removedHandlers.end())
                        return EXCEPTION_CONTINUE_SEARCH;

                    exception_ctx->Edi -= 4;
                    *(uint32_t*)exception_ctx->Edi = it->value;
                    exception_ctx->Ebx = it->key;
                    exception_ctx->Ebp += 8;
                    exception_ctx->Esi = it->next_handler;
                    exception_ctx->Eip = it->next_handler;
                    return EXCEPTION_CONTINUE_EXECUTION;
                }

                ctx = *it;
                c = it - contexts.begin();
            }

            c++;

            if (!handle_int3(exception_ctx, &ctx)) {
                MessageBoxA(0, "undefined behaviour", "error", 2);
                return EXCEPTION_CONTINUE_SEARCH;
            }
            LPRINT(skCrypt("[INFO]") << std::dec << c - 1 << " | " << std::hex << " [RAX]: " << ctx.rax << " |  rip handling... " << ctx.current_rip << " to rip " << ctx.rip << " | total size " << std::dec << contexts.size() << "\n");
        }
        else {
            it->handle(*exception_ctx);
        }

        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

void entry_thread() {
    return ((void(__stdcall*)())0x1337)();
}

static vm_element_t parse_vm_element(const nlohmann::json& json) {
    static std::map<std::string, eoperation> op_correspondence = {
        { "ROL", eoperation::ROL },
        { "DEC", eoperation::DEC },
        { "NOT", eoperation::NOT },
        { "BSWAP", eoperation::BSWAP },
        { "ADD", eoperation::ADD },
        { "XOR", eoperation::XOR },
        { "ROR", eoperation::ROR },
        { "INC", eoperation::INC },
        { "NEG", eoperation::NEG },
        { "SUB", eoperation::SUB }
    };
    vm_element_t element = {};

    element.addr = json["address"].get<uint32_t>();
    element.encryption_key = json["key"].get<uint32_t>();

    for (const nlohmann::json& operation : json["operations"]) {
        vm_operation_t vm_op = {};

        vm_op.addr = std::stoull(operation[0].get<std::string>());

        vm_op.operation = op_correspondence[operation[1]];

        vm_op.value = std::stoul(operation[2].get<std::string>());

        element.operations.push_back(vm_op);
    }

    element.size = json["size"];
    element.value = json["value"];
    element.vip = json["vip"];

    return element;
}
skeet_t::skeet_t(HMODULE base) : _base(0x43310000), page(nullptr), _stack(nullptr) {
    if (singleton)
        return;

    singleton = this;

    HRSRC hRes = FindResource(base, MAKEINTRESOURCE(IDR_BINARY1), skCrypt(L"BINARY"));

    HGLOBAL hResData = LoadResource(base, hRes);

    LPVOID pData = LockResource(hResData);
    DWORD size = SizeofResource(base, hRes);

    _size = size;
    
    skeet_bin = new char[size];

    std::memcpy(skeet_bin, pData, size);

    hRes = FindResource(base, MAKEINTRESOURCE(IDR_EXPORTS1), skCrypt(L"EXPORTS"));

    hResData = LoadResource(base, hRes);

    pData = LockResource(hResData);
    size = SizeofResource(base, hRes);

    std::string strFileExports((char*)pData, size);

    nlohmann::json jsExports = nlohmann::json::parse(strFileExports);

    for (const auto& [addr, value] : jsExports.items()) {
        import_t import_;
        import_.old_addr = static_cast<uint32_t>(std::stoul(addr));
        for (const auto& [lib, api] : value.items()) {
            import_.lib = lib;
            import_.name = api;
        }
        exports[import_.old_addr] = import_;
    }

    hRes = FindResource(base, MAKEINTRESOURCE(IDR_BYTECODE1), skCrypt(L"BYTECODE"));

    hResData = LoadResource(base, hRes);

    pData = LockResource(hResData);
    size = SizeofResource(base, hRes);

    std::string strFileBytecode((char*)pData, size);

    nlohmann::json jsBytecode = nlohmann::json::parse(strFileBytecode);

    for (const auto& unit : jsBytecode) {
        if (unit["main"].empty())
            continue;

        std::vector<vm_element_t> infoHandlers;

        infoHandlers.push_back(parse_vm_element(unit["main"]));

        if (unit.contains("others")) {
            for (const nlohmann::json& other : unit["others"]) {
                infoHandlers.push_back(parse_vm_element(other));
            }
        }

        handlers.push_back(infoHandlers);
    }

    skeetFonts = VirtualAlloc(0, sizeof(menu_fonts), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    std::memcpy(skeetFonts, menu_fonts, sizeof(menu_fonts));
}

bool skeet_t::map()
{
    _size = 0x2fc000;
    page = VirtualAlloc(reinterpret_cast<void*>(_base), _size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!page)
        page = (void*)_base;
        
    try {
        memcpy(page, skeet_bin, _size);
    }
    catch (...) {
        return false;
    }

    return true;
}

bool skeet_t::fix_imports()
{
    for (auto& _import : imports) {
        auto old_addr = 0;

        switch (*(uint8_t*)(_import.rip)) {
            case 0xe8:
            case 0xe9: {
                old_addr = *(u32*)(_import.rip + 1) + _import.rip + 5;
                break;
            }
            default: {
                old_addr = *(u32*)(_import.rip + 1);
                break;
            }
        }

        if (exports.find(old_addr) == exports.end())
            LPRINT("(fix_imports) failed to get " << std::hex << old_addr << "\n");

        auto& export_ = exports[old_addr];

        auto import_addr = (u32)(GetProcAddress(LoadLibraryA(export_.lib.c_str()), export_.name.c_str()));

        if (!import_addr) {
            LPRINT("(fix_imports) failed to get " << std::hex << export_.lib.c_str() << " " << export_.name.c_str() << "\n");
            return false;
        }
        
        switch (*(uint8_t*)(_import.rip)) {
        case 0xe8:
        case 0xe9: {
            *(u32*)(_import.rip + 1) = import_addr - _import.rip - 5;
            break;
        }
        default: {
            *(u32*)(_import.rip + 1) = import_addr;
            break;
        }
        }
    }
    return true;
}

using HkRecoverFn = int(__fastcall*)(int base, int size);
HkRecoverFn detourCrcCheck = nullptr;

static int __fastcall crcCheck(int base, int size) {

    if (base == 0x4331E000 && size == 0x105B12) {
        return 0x20547A5B ^ 0x92CA5DF1;
    }
    return detourCrcCheck(base, size);
}

bool skeet_t::extra()
{
    const uint32_t hashes[] = {
        0xC0BA92C9,
        0xBBFFA7E0,
        0xA42BE2F8
    };

    std::thread([&]() {
        int c = 0;
        while (*(uint32_t*)(0x4346D4CC) == 0 && c < 3) {
            *(uint32_t*)(0x4346D4CC) = hashes[c];
        }
        }).detach();

    LPRINT(skCrypt("[INFO] adding exception handler...\n"));
    AddVectoredExceptionHandler(0, skeet_exception_handler);
    LPRINT(skCrypt("[INFO] added!\n"));
    memcpy((void*)0x43490c82, "\xC7\x02\x00\x00\x00\x00\xC3", sizeof("\xC7\x02\x00\x00\x00\x00\xC3") - 1);


    *(uint8_t*)0x43466D7C = 0xeb;
    *(uint8_t*)0x4348B4FC = 0xeb;

    auto hWnd = FindWindowA("Valve001", "Counter-Strike: Global Offensive - Direct3D 9");
    *(uint32_t*)0x433A068A = (uint32_t)hWnd;
    *(uint32_t*)0x4341F644 = (uint32_t)hWnd;
    *(uint32_t*)0x4341F7C0 = (uint32_t)hWnd;

    memset((void*)0x4350F0C7, 0x90, 5);

    *(uint32_t*)0x43467F64 = ((u32)GetModuleHandleA("shaderapidx9.dll") + 0xA62C0);

    *(uint32_t*)(0x4346837C) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x2BDC20);
    *(uint32_t*)(0x43467570) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x2BD020);
    *(uint32_t*)(0x43468234) = (uint32_t)((uint32_t)GetModuleHandleA("studiorender.dll") + 0x57DF0);
    *(uint32_t*)(0x43468C98) = (uint32_t)((uint32_t)GetModuleHandleA("studiorender.dll") + 0x44E2EC);

    *(uint32_t*)(0x434670AC) = (uint32_t)((uint32_t)GetModuleHandleA("matchmaking.dll"));
    *(uint32_t*)(0x434670E4) = (uint32_t)((uint32_t)GetModuleHandleA("server.dll"));
    *(uint32_t*)(0x4346762C) = (uint32_t)((uint32_t)GetModuleHandleA("panoramauiclient.dll"));
    *(uint32_t*)(0x43467AFC) = (uint32_t)((uint32_t)GetModuleHandleA("vguimatsurface.dll"));
    *(uint32_t*)(0x43467B74) = (uint32_t)((uint32_t)GetModuleHandleA("filesystem_stdio.dll"));
    *(uint32_t*)(0x43467DAC) = (uint32_t)((uint32_t)GetModuleHandleA("inputsystem.dll"));
    *(uint32_t*)(0x4346889C) = (uint32_t)((uint32_t)GetModuleHandleA("vgui2.dll"));
    *(uint32_t*)(0x43468A7C) = (uint32_t)((uint32_t)GetModuleHandleA("vstdlib.dll"));
    *(uint32_t*)(0x43468B74) = (uint32_t)((uint32_t)GetModuleHandleA("vphysics.dll"));
    *(uint32_t*)(0x43468D08) = (uint32_t)((uint32_t)GetModuleHandleA("gameoverlayrenderer.dll"));
    *(uint32_t*)(0x43468D20) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll"));
    *(uint32_t*)(0x43468D40) = (uint32_t)((uint32_t)GetModuleHandleA("soundsystem.dll"));
    *(uint32_t*)(0x43468D98) = (uint32_t)((uint32_t)GetModuleHandleA("datacache.dll"));
    *(uint32_t*)(0x43469234) = (uint32_t)((uint32_t)GetModuleHandleA("panorama.dll"));
    *(uint32_t*)(0x4346A55C) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll"));
    *(uint32_t*)(0x4346A600) = (uint32_t)((uint32_t)GetModuleHandleA("materialsystem.dll"));
    *(uint32_t*)(0x4346A604) = (uint32_t)((uint32_t)GetModuleHandleA("shaderapidx9.dll"));
    *(uint32_t*)(0x4346A650) = (uint32_t)((uint32_t)GetModuleHandleA("studiorender.dll"));

    *(uint32_t*)(0x434693F0) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x43E9E0);
    *(uint32_t*)(0x43467F0C) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x443481);

    *(uint32_t*)(0x43468E44) = **(uint32_t**)((uint32_t)GetModuleHandleA("engine.dll") + 0x23790);
    *(uint32_t*)(0x4346D4D0) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0xB7550);

    *(uint32_t*)(0x4346D4C8) = (uint32_t)((uint32_t)GetModuleHandleA("materialsystem.dll") + 0xBBF4);
    *(uint32_t*)(0x4346D4C4) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x1A3656);
    *(uint32_t*)(0x4346D4C0) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x59E9CC);

    *(uint32_t*)(0x434689A0) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x2F10D3);
    *(uint32_t*)(0x4346A454) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0xD99C0);
    *(uint32_t*)(0x434674E0) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x4123AC);
    *(uint32_t*)(0x43468518) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x52EFAC);
    *(uint32_t*)(0x434688C4) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x4413A0);


    *(uint32_t*)(0x4346D668) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x00228908);
    *(uint32_t*)(0x4346D66C) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x000F03FD);
    *(uint32_t*)(0x43468B9C) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x00712740);


    *(uint32_t*)(0x4346A31C) = (uint32_t)((uint32_t)GetModuleHandleA("localize.dll") + 0x00038A70);

    *(uint32_t*)(0x43468B2C) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x52C810);
    *(uint32_t*)(0x43467288) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x22F08F);
    *(uint32_t*)(0x4346728C) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x9A2CF0);
    *(uint32_t*)(0x4346728C) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0xDF14C0);
    *(uint32_t*)(0x4346D540) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x3DC9B0);
    *(uint32_t*)(0x43476A40) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x52B270);
    *(uint32_t*)(0x434671B4) = (uint32_t)((uint32_t)GetModuleHandleA("engine.dll") + 0x52B248);
    *(uint32_t*)(0x434678C4) = (uint32_t)((uint32_t)GetModuleHandleA("client.dll") + 0x2C8F50);
    *(uint32_t*)0x43468B9C = ((uint32_t)GetModuleHandleA("client.dll") + 0x712740);
    *(uint32_t*)0x43467C04 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5344CE0);
    *(uint32_t*)0x43467040 = ((uint32_t)GetModuleHandleA("client.dll") + 0x70ADB0);
    *(uint32_t*)0x43468994 = ((uint32_t)GetModuleHandleA("client.dll") + 0x523BC98);

    *(uint32_t*)0x4346D67C = ((uint32_t)GetModuleHandleA("panorama.dll") + 0x9E220);
    *(uint32_t*)0x4346D680 = ((uint32_t)GetModuleHandleA("panorama.dll") + 0x9A320);
    *(uint32_t*)0x4346D684 = ((uint32_t)GetModuleHandleA("panorama.dll") + 0xA1B80);
    *(uint32_t*)0x4346D688 = ((uint32_t)GetModuleHandleA("panorama.dll") + 0xA3C80);
    *(uint32_t*)0x4346D68C = ((uint32_t)GetModuleHandleA("panorama.dll") + 0xAB6F0);

    *(uint32_t*)0x43468D0C = ((uint32_t)GetModuleHandleA("client.dll") + 0xDF14C0);
    *(uint32_t*)0x43467A70 = ((uint32_t)GetModuleHandleA("client.dll") + 0xDF14D4);
    *(uint32_t*)0x43468D50 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5334E08);

    *(uint32_t*)0x4346D560 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x8E55C);
    *(uint32_t*)0x4346D55C = ((uint32_t)GetModuleHandleA("engine.dll") + 0x22BE36);

    *(uint32_t*)0x4346A374 = ((uint32_t)GetModuleHandleA("client.dll") + 0x2D3A40);
    *(uint32_t*)0x4346D65C = ((uint32_t)GetModuleHandleA("client.dll") + 0x346AC0);
    *(uint32_t*)0x4346D660 = ((uint32_t)GetModuleHandleA("client.dll") + 0x344610);
    *(uint32_t*)0x4346A624 = ((uint32_t)GetModuleHandleA("client.dll") + 0x52632C8);
    *(uint32_t*)0x4346A364 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D94A0);
    *(uint32_t*)0x434679AC = ((uint32_t)GetModuleHandleA("client.dll") + 0x9A6740);
    *(uint32_t*)0x4346A38C = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D2CA0);
    *(uint32_t*)0x434683D0 = ((uint32_t)GetModuleHandleA("client.dll") + 0xDACC0F);
    *(uint32_t*)0x43468F80 = ((uint32_t)GetModuleHandleA("client.dll") + 0x432E20);
    *(uint32_t*)0x434688C8 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1EA950);
    *(uint32_t*)0x4346A6B0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D1BA0);
    *(uint32_t*)0x43468C10 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1AEC40);
    *(uint32_t*)0x43467C58 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5334DA4);
    *(uint32_t*)0x43467E44 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1EA790);
    *(uint32_t*)0x4346A598 = ((uint32_t)GetModuleHandleA("client.dll") + 0x442740);
    *(uint32_t*)0x43467C48 = ((uint32_t)GetModuleHandleA("client.dll") + 0x307260);
    *(uint32_t*)0x43468F30 = ((uint32_t)GetModuleHandleA("client.dll") + 0x320BD0);
    *(uint32_t*)0x434691DC = ((uint32_t)GetModuleHandleA("client.dll") + 0x9A2C40);
    *(uint32_t*)0x4346830C = ((uint32_t)GetModuleHandleA("client.dll") + 0xDF7FC0);
    *(uint32_t*)0x4346A3A4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5334764);
    *(uint32_t*)0x4346A4F8 = ((uint32_t)GetModuleHandleA("client.dll") + 0x19D140);
    *(uint32_t*)0x434678C8 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D9440);
    *(uint32_t*)0x4346832C = ((uint32_t)GetModuleHandleA("client.dll") + 0x1B1AC0);
    *(uint32_t*)0x43467CF0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x7DE760);
    *(uint32_t*)0x43467B54 = ((uint32_t)GetModuleHandleA("client.dll") + 0x526388C);
    *(uint32_t*)0x43468C3C = ((uint32_t)GetModuleHandleA("client.dll") + 0x7DEB00);



    *(uint32_t*)0x43467000 = ((uint32_t)GetModuleHandleA("client.dll") + 0x32C0A59);
    *(uint32_t*)0x434693B0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x292CA6C);
    *(uint32_t*)0x4346A770 = ((uint32_t)GetModuleHandleA("client.dll") + 0x4F741CB);
    *(uint32_t*)0x43469198 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1F4E28D);
    *(uint32_t*)0x43469060 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5332374);
    *(uint32_t*)0x434688C8 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1EA950);
    *(uint32_t*)0x43467CF0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x7DE760);
    *(uint32_t*)0x4346830C = ((uint32_t)GetModuleHandleA("client.dll") + 0xDF7FC0);
    *(uint32_t*)0x4346728C = ((uint32_t)GetModuleHandleA("client.dll") + 0x9A2CF0);
    *(uint32_t*)0x43468C10 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1AEC40);
    *(uint32_t*)0x4346A3A4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5334764);
    *(uint32_t*)0x4346D628 = ((uint32_t)GetModuleHandleA("panorama.dll") + 0x3DFE0);
    *(uint32_t*)0x4346D62C = ((uint32_t)GetModuleHandleA("panorama.dll") + 0x3E020);
    *(uint32_t*)0x43467C48 = ((uint32_t)GetModuleHandleA("client.dll") + 0x307260);
    *(uint32_t*)0x4346D658 = ((uint32_t)GetModuleHandleA("client.dll") + 0x52448F8);
    *(uint32_t*)0x4346D65C = ((uint32_t)GetModuleHandleA("client.dll") + 0x346AC0);
    *(uint32_t*)0x43467E44 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1EA790);
    *(uint32_t*)0x4346A5DC = ((uint32_t)GetModuleHandleA("client.dll") + 0x3E2E673);
    *(uint32_t*)0x43468994 = ((uint32_t)GetModuleHandleA("client.dll") + 0x523BC98);
    *(uint32_t*)0x4346A7CC = ((uint32_t)GetModuleHandleA("engine.dll") + 0x20CE50);
    *(uint32_t*)0x4346A374 = ((uint32_t)GetModuleHandleA("client.dll") + 0x2D3A40);
    *(uint32_t*)0x4346D67C = ((uint32_t)GetModuleHandleA("panorama.dll") + 0x9E220);
    *(uint32_t*)0x4346D680 = ((uint32_t)GetModuleHandleA("panorama.dll") + 0x9A320);
    *(uint32_t*)0x4346D688 = ((uint32_t)GetModuleHandleA("panorama.dll") + 0xA3C80);
    *(uint32_t*)0x43468FF0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x3231380);
    *(uint32_t*)0x434679EC = ((uint32_t)GetModuleHandleA("client.dll") + 0xE10580);
    *(uint32_t*)0x43467F0C = ((uint32_t)GetModuleHandleA("client.dll") + 0x443481);
    *(uint32_t*)0x4346D664 = ((uint32_t)GetModuleHandleA("client.dll") + 0x525E110);
    *(uint32_t*)0x43468250 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5334848);
    *(uint32_t*)0x4346A64C = ((uint32_t)GetModuleHandleA("engine.dll") + 0xD9A18);
    *(uint32_t*)0x43468390 = ((uint32_t)GetModuleHandleA("client.dll") + 0x12170B3);
    *(uint32_t*)0x43467550 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x8DAB0);
    *(uint32_t*)0x43468F80 = ((uint32_t)GetModuleHandleA("client.dll") + 0x432E20);
    *(uint32_t*)0x434688C4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x4413A0);
    *(uint32_t*)0x43468B9C = ((uint32_t)GetModuleHandleA("client.dll") + 0x712740);
    *(uint32_t*)0x43468C94 = ((uint32_t)GetModuleHandleA("client.dll") + 0x52BF6D8);
    *(uint32_t*)0x434688A4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x215080);
    *(uint32_t*)0x4346A624 = ((uint32_t)GetModuleHandleA("client.dll") + 0x52632C8);
    *(uint32_t*)0x434689A0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x2F10D3);
    *(uint32_t*)0x434692F4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x430D0E);
    *(uint32_t*)0x4346D64C = ((uint32_t)GetModuleHandleA("client.dll") + 0x4364D0);
    *(uint32_t*)0x434691DC = ((uint32_t)GetModuleHandleA("client.dll") + 0x9A2C40);
    *(uint32_t*)0x4346D660 = ((uint32_t)GetModuleHandleA("client.dll") + 0x344610);
    *(uint32_t*)0x434688C0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x525D440);
    *(uint32_t*)0x43467DDC = ((uint32_t)GetModuleHandleA("client.dll") + 0x43C750);
    *(uint32_t*)0x43468308 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x2A5982);
    *(uint32_t*)0x434679AC = ((uint32_t)GetModuleHandleA("client.dll") + 0x9A6740);
    *(uint32_t*)0x4346D684 = ((uint32_t)GetModuleHandleA("panorama.dll") + 0xA1B80);
    *(uint32_t*)0x4346D68C = ((uint32_t)GetModuleHandleA("panorama.dll") + 0xAB6F0);




    *(uint32_t*)0x4346A884 = ((uint32_t)GetModuleHandleA("client.dll") + 0x2A78A0);
    *(uint32_t*)0x43468E88 = ((uint32_t)GetModuleHandleA("client.dll") + 0x2A77E0);
    *(uint32_t*)0x4346A7F0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x28CC90);
    *(uint32_t*)0x43467040 = ((uint32_t)GetModuleHandleA("client.dll") + 0x70ADB0);
    *(uint32_t*)0x4346A724 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x1791916);
    *(uint32_t*)0x43472B64 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x2AC6E69);
    *(uint32_t*)0x4346EF18 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x2AC6E69);
    *(uint32_t*)0x4346D558 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x38FDCC8);
    *(uint32_t*)0x434674E0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x4123AC);
    *(uint32_t*)0x4346756C = ((uint32_t)GetModuleHandleA("client.dll") + 0x5B1E2E);
    *(uint32_t*)0x4346832C = ((uint32_t)GetModuleHandleA("client.dll") + 0x1B1AC0);
    *(uint32_t*)0x43468FA4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5334C84);
    *(uint32_t*)0x4346A364 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D94A0);
    *(uint32_t*)0x43468320 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1E69D0);
    *(uint32_t*)0x4346A6B0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D1BA0);
    
    *(uint32_t*)0x43467EF4 = ((uint32_t)GetModuleHandleA("client.dll") + 0xE047DC);
    
    *(uint32_t*)0x434670C4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x525EBC4);
    
    *(uint32_t*)0x43468C3C = ((uint32_t)GetModuleHandleA("client.dll") + 0x7DEB00);
    *(uint32_t*)0x43468CA4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x7DE8A0);
    *(uint32_t*)0x434693F0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x43E9E0);
    *(uint32_t*)0x434683D0 = ((uint32_t)GetModuleHandleA("client.dll") + 0xDACC0F);
    *(uint32_t*)0x43469348 = ((uint32_t)GetModuleHandleA("client.dll") + 0x36CAC0);
    *(uint32_t*)0x4346A4F8 = ((uint32_t)GetModuleHandleA("client.dll") + 0x19D140);
    *(uint32_t*)0x434679C0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x525CAA0);
    *(uint32_t*)0x4346A454 = ((uint32_t)GetModuleHandleA("engine.dll") + 0xD99C0);
    *(uint32_t*)0x43468F30 = ((uint32_t)GetModuleHandleA("client.dll") + 0x320BD0);
    *(uint32_t*)0x43468DC0 = ((uint32_t)GetModuleHandleA("client.dll") + 0x19D600);
    *(uint32_t*)0x43469370 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x159296);
    *(uint32_t*)0x4346A82C = ((uint32_t)GetModuleHandleA("client.dll") + 0x3D1820);
    *(uint32_t*)0x43468518 = ((uint32_t)GetModuleHandleA("client.dll") + 0x52EFAC);
    *(uint32_t*)0x43469304 = ((uint32_t)GetModuleHandleA("client.dll") + 0x535E4CC);
    *(uint32_t*)0x43467288 = ((uint32_t)GetModuleHandleA("engine.dll") + 0x22F08F);
    *(uint32_t*)0x4346746C = ((uint32_t)GetModuleHandleA("engine.dll") + 0x1C5D7D3);
    *(uint32_t*)0x4346A598 = ((uint32_t)GetModuleHandleA("client.dll") + 0x442740);
    *(uint32_t*)0x434678C8 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D9440);
    *(uint32_t*)0x43468D6C = ((uint32_t)GetModuleHandleA("engine.dll") + 0x8CD5F0);
    *(uint32_t*)0x4346A7E4 = ((uint32_t)GetModuleHandleA("client.dll") + 0x5344B38);
    *(uint32_t*)0x4346A38C = ((uint32_t)GetModuleHandleA("client.dll") + 0x1D2CA0);
    *(uint32_t*)0x4346D644 = ((uint32_t)GetModuleHandleA("client.dll") + 0x1E8100);

    *(uint32_t*)0x4346A898 = ((uint32_t)GetModuleHandleA("client.dll") + 0xDF98A0);
    {
        HMODULE hGor = GetModuleHandleA("gameoverlayrenderer.dll");
        if (hGor) {
            auto gorScan1 = PatternScan(hGor, "3D ? ? ? ? 73 ? 68 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 ? ? ? 6A ? FF 50 ? 3B 5F");
            if (!gorScan1) {
                gorScan1 = PatternScan(hGor, "3D ? ? ? ? 73 1A 68 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B 01 6A 00 FF 50 14 3B 7B 3C");
            }

            if (gorScan1) {
                *(uint32_t*)0x43468D94 = (uint32_t)gorScan1;
            }

            auto gorScan2 = PatternScan(hGor, "89 3D ? ? ? ? F3 0F 10 87");
            if (!gorScan2) {
                gorScan2 = PatternScan(hGor, "89 1D ? ? ? ? F3 0F 10 83");
            }

            if (gorScan2) {
                *(uint32_t*)0x43468350 = *(uint32_t*)(gorScan2 + 2);
            }
        }
    }

    *(bool*)0x43475798 = false;

    std::thread([&]() {
        bool insertWasPressed = false;
        while (true) {
            if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
                if (!insertWasPressed) {
                    insertWasPressed = true;
                    INPUT input = {};
                    input.type = INPUT_KEYBOARD;
                    input.ki.wVk = VK_DELETE;
                    input.ki.dwFlags = 0;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                }
            }
            else {
                insertWasPressed = false;
            }
            
            if (g_pRendererAddress && !IsBadReadPtr(g_pRendererAddress, sizeof(void*))) {
                g_renderer = *g_pRendererAddress;
            }
            
            ApplyRendererHook();
            Sleep(10);
        }
        }).detach();

    void* sclassMatch = PatternScan(GetModuleHandleW(L"majorkahook.dll"), "A1 ?? ?? ?? ?? 83 64 24 04 00 89 54 24 18 89 44 24 10 53 56");
    if (sclassMatch) {
        void* sclassPtr = *(void**)((uintptr_t)sclassMatch + 1);
        if (!IsBadReadPtr(sclassPtr, sizeof(void*))) {
            g_pRendererAddress = reinterpret_cast<IRendererOverlay**>(sclassPtr);
        }
    }

    render_hook_target = PatternScan(GetModuleHandleW(L"majorkahook.dll"), "55 8B EC 83 E4 F8 E8 ?? ?? ?? ?? 83 38 00 74 36 89 0D");
    if (render_hook_target) {
        install_raw_hook(render_hook_target, (void*)render_hook_naked, render_original_bytes, 6);
        g_animating = true;
    }

    LPRINT(skCrypt("[INFO] recompiling vm...\n"));
    recompile();
    LPRINT(skCrypt("[INFO] recompiled!\n"));

    return true;
}

bool skeet_t::entry()
{
    DWORD tid = 0;
    auto hThread = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_thread), 0, CREATE_SUSPENDED, &tid);

    if (hThread == INVALID_HANDLE_VALUE)
        return false;

    _stack = VirtualAlloc(nullptr, 0x100000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    CONTEXT ctx = {};
    ctx.ContextFlags = CONTEXT_FULL;
    ctx.Esp = (uint32_t)_stack + 0x20000;
    ctx.Eip = 0x43481e2c;
    ctx.Esi = 0x43481e2c;
    ctx.Edi = 0x43481e2c;

    typedef struct _LSA_UNICODE_STRING { USHORT Length;	USHORT MaximumLength; PWSTR  Buffer; } UNICODE_STRING, * PUNICODE_STRING;
    typedef struct _OBJECT_ATTRIBUTES { ULONG Length; HANDLE RootDirectory; PUNICODE_STRING ObjectName; ULONG Attributes; PVOID SecurityDescriptor;	PVOID SecurityQualityOfService; } OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;
    using myNtCreateSection = NTSTATUS(NTAPI*)(OUT PHANDLE SectionHandle, IN ULONG DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, IN PLARGE_INTEGER MaximumSize OPTIONAL, IN ULONG PageAttributess, IN ULONG SectionAttributes, IN HANDLE FileHandle OPTIONAL);

    myNtCreateSection fNtCreateSection = (myNtCreateSection)(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateSection"));

    SIZE_T size = 0x00100000;
    LARGE_INTEGER sectionSize = { size };
    HANDLE sectionHandle = NULL;
    PVOID localSectionAddress = NULL, remoteSectionAddress = NULL;

    fNtCreateSection(&sectionHandle, SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_MAP_EXECUTE, NULL, (PLARGE_INTEGER)&sectionSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);

    *(uint32_t*)(ctx.Esp + 0xC) = (uint32_t)sectionHandle;

    SetThreadContext(hThread, &ctx);

    ResumeThread(hThread);

    CloseHandle(hThread);
    return true;
}

uint32_t reverse_emulate(eoperation action, int size, uint32_t op1, uint32_t op2) {
    switch (action) {
    case eoperation::ROL: {
        switch (size) {
        case 1:
            return std::rotr<uint8_t>(op1, op2);
        case 2:
            return std::rotr<uint16_t>(op1, op2);
        case 4:
            return std::rotr<uint32_t>(op1, op2);
        }
    }
    case eoperation::ROR: {

        switch (size) {
        case 1:
            return std::rotl<uint8_t>(op1, op2);
        case 2:
            return std::rotl<uint16_t>(op1, op2);
        case 4:
            return std::rotl<uint32_t>(op1, op2);
        }
    }
    case eoperation::BSWAP: {

        switch (size) {
        case 1:
            return std::byteswap<uint8_t>(op1);
        case 2:
            return std::byteswap<uint16_t>(op1);
        case 4:
            return std::byteswap<uint32_t>(op1);
        }
    }
    case eoperation::INC: {
        return op1 - 1;
    }
    case eoperation::DEC: {
        return op1 + 1;
    }
    case eoperation::SUB: {

        switch (size) {
        case 1:
            return static_cast<uint8_t>(op1 + op2);
        case 2:
            return static_cast<uint16_t>(op1 + op2);
        case 4:
            return static_cast<uint32_t>(op1 + op2);
        }
    }
    case eoperation::ADD: {

        switch (size) {
        case 1:
            return static_cast<uint8_t>(op1 - op2);
        case 2:
            return static_cast<uint16_t>(op1 - op2);
        case 4:
            return static_cast<uint32_t>(op1 - op2);
        }
    }
    case eoperation::NOT: {
        switch (size) {
        case 1:
            return static_cast<uint8_t>(~op1);
        case 2:
            return static_cast<uint16_t>(~op1);
        case 4:
            return static_cast<uint32_t>(~op1);
        }
    }
    case eoperation::XOR: {

        switch (size) {
        case 1:
            return static_cast<uint8_t>(op1 ^ op2);
        case 2:
            return static_cast<uint16_t>(op1 ^ op2);
        case 4:
            return static_cast<uint32_t>(op1 ^ op2);
        }
    }
    case eoperation::NEG: {

        switch (size) {
        case 1:
            return static_cast<uint8_t>(0 - op1);
        case 2:
            return static_cast<uint16_t>(0 - op1);
        case 4:
            return static_cast<uint32_t>(0 - op1);
        }
    }
    }
}

uint32_t fnv1a(const std::wstring& data) {
    const uint32_t FNV_prime = 0x1000193;
    const uint32_t offset_basis = 0x811C9DC5;
    uint32_t hash = offset_basis;

    for (wchar_t c : data) {
        hash ^= c;
        hash *= FNV_prime;
    }

    return hash;
}

void SubtractMinutesFromSystemTime(int minutes, FILETIME& ft) {
    ULARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    const ULONGLONG intervalsPerMinute = 60 * 10000000ULL;

    li.QuadPart += (minutes * intervalsPerMinute);

    ft.dwLowDateTime = li.LowPart;
    ft.dwHighDateTime = li.HighPart;
}

void skeet_t::recompile()
{
    std::map<uint32_t, uint32_t> patches;

    patches[0x000000004349D3E2] = (u32)GetModuleHandleA("ntdll.dll");

    GetSystemTimeAsFileTime(&ftime);
    SubtractMinutesFromSystemTime(10, ftime);
    auto arg1 = ftime.dwLowDateTime - 0x0D53E8000;
    auto arg2 = ftime.dwHighDateTime - 0x19DB1DE;
    auto time_check = ((int(__stdcall*)(int arg1, int arg2, int arg3, int arg4))0x433AE31E)(arg1, arg2, 0x989680, 0);
    patches[0x000000004353C3D1] = time_check;

    auto crc_hash = ((int(__fastcall*)(int base, int size))0x4333D6A2)(0x4331E000, 0x105B12);
    patches[0x00000000434C6B29] = crc_hash ^ 0x92CA5DF1;

    patches[0x00000000434EB360] = GetCurrentProcessId();

    patches[0x00000000435507ED] = time_check;

    PPEB peb = (PPEB)(__readfsdword(0x30));

    auto ldr_entry = peb->Ldr;

    PLDR_DATA_TABLE_ENTRY entry = (PLDR_DATA_TABLE_ENTRY)ldr_entry->InLoadOrderModuleList->Flink;

    auto val = *(uint32_t*)((uint32_t)entry + 0x88) - 0xD53E8000;
    auto val2 = *(uint32_t*)((uint32_t)entry + 0x8c) - 0x19DB1DE;
    time_check = ((int(__stdcall*)(int arg1, int arg2, int arg3, int arg4))0x433AE31E)(val, val2, 0x989680, 0);

    patches[0x000000004352A5D6] = time_check;

    SetEnvironmentVariable(L"STEAMID", L"7612345678901");

    patches[0x000000004356C0A1] = fnv1a(L"7612345678901");

    patches[0x000000004352EE8F] = *(uint16_t*)0x7FFE0260;


    int cpu_info[4] = {};
    __cpuidex(cpu_info, 1, 0);

    patches[0x434bc39e] = cpu_info[2];
    patches[0x4356afc4] = cpu_info[2];
    patches[0x000000004352C91C] = cpu_info[2];

    patches[0x000000004351B2B5] = (u32)GetModuleHandleA("ntdll.dll");

    patches[0x434d4faa] = (u32)GetCurrentProcessId();

    uint32_t hash = 0;
    for (uint32_t i = 0x80000002; i <= 0x80000004; i++) {
        __cpuidex(cpu_info, i, 0);

        hash += cpu_info[0] + cpu_info[1] + cpu_info[2] + cpu_info[3];
    }

    auto it = std::find_if(removedHandlers.begin(), removedHandlers.end(), [](const removed_handler_t& other) {
        return other.vip == 0x000000004359257D;
        });

    it->value = hash;
    patches[0x43507C7F] = hash;

    patches[0x435194de] = (u32)GetModuleHandleA("kernel32.dll");

    uint32_t hashKUserSharedData = 0x811C9DC5;

    for (int i = 0; i < 0x40; i++) {
        unsigned char byte = *(unsigned char*)(0x7FFE0274 + i);

        uint32_t tmpValue = byte + i;
        tmpValue = tmpValue ^ hashKUserSharedData;
        hashKUserSharedData = tmpValue * 0x1000193;
    }

    patches[0x434eb732] = hashKUserSharedData;

    patches[0x435469B5] = (uint32_t)GetModuleHandleA("kernelbase.dll");

    for (int i = 0; i < handlers.size(); i++) {
        auto& handlers_info = handlers[i];

        if (patches.find(handlers_info[0].vip) != patches.end()) {
            handlers_info[0].value = patches[handlers_info[0].vip];
        }
        else {
            auto import_dest = handlers_info[0].value;

            if (exports.find(import_dest) != exports.end()) {
                auto& export_ = exports[import_dest];

                handlers_info[0].value = (u32)GetProcAddress(LoadLibraryA(export_.lib.c_str()), export_.name.c_str());
            }
            else {
                LPRINT("failed to get " << std::hex << import_dest << " VIP " << handlers_info[0].vip << "\n");
            }
        }

        uint32_t encryption_key = handlers_info[0].encryption_key;

        for (auto& handler_info : handlers_info) {

            if (!skeet_t::is_image_range(handler_info.vip))
                continue;

            uint32_t value = handler_info.value;
            uint8_t size = handler_info.size;

            for (auto it = handler_info.operations.rbegin(); it != handler_info.operations.rend(); it++) {
                value = reverse_emulate(it->operation, size, value, it->value);
            }
            switch (size) {
            case 4:
                value ^= encryption_key;
                *(uint32_t*)handler_info.vip = value;
                break;
            case 2:
                value ^= static_cast<uint16_t>(encryption_key);
                *(uint16_t*)handler_info.vip = value;
                break;
            case 1:
                value ^= static_cast<uint8_t>(encryption_key);
                *(uint8_t*)handler_info.vip = value;
                break;
            }
            encryption_key ^= static_cast<uint32_t>(handler_info.value);
        }
    }
}

skeet_t* skeet_t::getInstance(HMODULE base)
{
    if (!singleton)
        singleton = new skeet_t(base);
        
    return singleton;
}

bool skeet_t::is_stack_range(u32 addr)
{
    auto skeet = getInstance();

    return addr >= 0x10D0000 && addr < 0x10D0000 + 0x100000;
}

bool skeet_t::is_image_range(u32 addr)
{
    auto skeet = getInstance();

    return addr >= skeet->base() && addr < skeet->base() + skeet->size();
}

bool skeet_t::is_exception(u32 addr)
{
    if(addr >= 0x434F8532 && addr <= 0x434F853A)
        return true;
    return false;
}
