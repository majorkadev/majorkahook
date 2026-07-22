#include "pch.h"
#define SDK_CLIENT_IMP 1
#include "SkeetSDK/skeetsdk.h"
#include <thread>
#include <string>
#include <fstream>
#include <algorithm>
#include "mdevutils.h"


using namespace SkeetSDK;

int checkboxVal = 0;
static int themeComboVal = 0;
extern int g_current_theme;
extern const int g_theme_count;
extern void ApplyTheme(int index);

extern int g_color1;
extern int g_color2;
extern int g_color3;

VecCol customColor1(0xFF37B1DA);
VecCol customColor2(0xFFCC46CD);
VecCol customColor3(0xFFCCE335);

#include "MinHook/MinHook.h"
#pragma comment(lib, "MinHook/libMinHook-x86-v141-mtd.lib")

extern "C" int _CrtDbgReport(int reportType, const char* filename, int linenumber, const char* moduleName, const char* format, ...) {
    return 0;
}
extern "C" int _CrtDbgReportW(int reportType, const wchar_t* filename, int linenumber, const wchar_t* moduleName, const wchar_t* format, ...) {
    return 0;
}

#include <vector>

static std::vector<PElement> g_custom_elements;
static PChild g_settingsChild = nullptr;
static LoadConfigFn g_oLoadConfig_Trampoline = nullptr;

static void __fastcall hkLoadConfig_Detour(void* ecx, void* edx, size_t size) {
    if (g_settingsChild && !g_custom_elements.empty()) {
        for (auto it = g_settingsChild->Elements.begin(); it != g_settingsChild->Elements.end(); ) {
            bool isCustom = false;
            for (auto& ce : g_custom_elements) {
                if (*it == ce) { isCustom = true; break; }
            }
            if (isCustom) {
                it = g_settingsChild->Elements.erase(it);
            } else {
                ++it;
            }
        }
    }

    if (g_oLoadConfig_Trampoline) {
        g_oLoadConfig_Trampoline(ecx, edx, size);
    }
    if (g_settingsChild && !g_custom_elements.empty()) {
        for (auto& ce : g_custom_elements) {
            bool found = false;
            for (auto& e : g_settingsChild->Elements) {
                if (e == ce) { found = true; break; }
            }
            if (!found) {
                g_settingsChild->Elements.push_back(ce);
            }
        }
        g_settingsChild->CalculateLayout();
    }
}

static void InstallLoadConfigHook() {
    if (!ConfigSystem::_LoadConfig || g_oLoadConfig_Trampoline) return;
    
    void* pTarget = (void*)ConfigSystem::_LoadConfig;
    
    if (MH_CreateHook(pTarget, (void*)&hkLoadConfig_Detour, (void**)&g_oLoadConfig_Trampoline) == MH_OK) {
        MH_EnableHook(pTarget);
    }
}

#if defined(SDK_GLOBALS_IMP) || defined(SDK_CLIENT_IMP)
static SetClanTagFn g_oSetClanTag_Trampoline = nullptr;
static void __fastcall hkSetClanTag_Detour(const char* tag, const char* name) {
    if (tag && name) {
        const char* majorka_tag = mJrKaKkAka::GetNextMajorkaClanTagFrame(300);
        if (g_oSetClanTag_Trampoline) {
            g_oSetClanTag_Trampoline(majorka_tag, majorka_tag);
            return;
        }
    }
    if (g_oSetClanTag_Trampoline) {
        g_oSetClanTag_Trampoline(tag, name);
    }
}

static void InstallClanTagHook() {
    if (!SkeetSDK::CEngine::SetTag || g_oSetClanTag_Trampoline) return;
    
    void* pTarget = (void*)SkeetSDK::CEngine::SetTag;
    
    if (MH_CreateHook(pTarget, (void*)&hkSetClanTag_Detour, (void**)&g_oSetClanTag_Trampoline) == MH_OK) {
        MH_EnableHook(pTarget);
    }
}
#else
static void InstallClanTagHook() {}
#endif


static wchar_t* themeNames[] = {
    (wchar_t*)L"Default",
    (wchar_t*)L"1",
    (wchar_t*)L"2",
    (wchar_t*)L"3",
    (wchar_t*)L"4",
    (wchar_t*)L"5",
    (wchar_t*)L"6",
    (wchar_t*)L"7",
    (wchar_t*)L"8",
    (wchar_t*)L"9",
    (wchar_t*)L"10",
    (wchar_t*)L"11",
    (wchar_t*)L"12",
    (wchar_t*)L"13",
};


extern const unsigned char _vtbl_sig[];
extern const int _vtbl_sig_len;

static int _cfg_crc32_validate() {
    unsigned int _acc = 0x811C9DC5;
    for (int _i = 0; _i < _vtbl_sig_len; _i++) {
        _acc ^= _vtbl_sig[_i];
        _acc *= 0x01000193;
    }
    return _acc;
}

static void _enforce_cfg_integrity() {
    if (_cfg_crc32_validate() != 0xA94C1107u) {
        volatile int* _null = nullptr;
        *_null = 0;
    }
}

extern const unsigned char _st_ref[];

static void _validate_overlay_state() {
    unsigned int _h = 0x811C9DC5;
    for (int _i = 0; _i < 14; _i++) {
        _h ^= _st_ref[_i];
        _h *= 0x01000193;
    }
    if (_h != 0xC257EF04u) {
        volatile int* _p = nullptr;
        *_p = 0;
    }
}

PChild g_rageWeaponGroup = nullptr;
PChild g_legitWeaponGroup = nullptr;

struct CustomChildData {
    int tabIndex;
    int childIndex;
    Vec2 pos;
    Vec2 size;
    Vec2 curSize;
};

struct CustomLayoutBlob {
    char signature[8];
    Vec2 menuPos;
    Vec2 menuSize;
    int childCount;
    CustomChildData children[150];
};

static inline uint16_t rotate_right16(uint16_t value, int shift) {
    return (value >> shift) | (value << (16 - shift));
}

static bool DecryptNameSafe(PElement element, wchar_t* outBuffer, size_t outBufferSize) {
    __try {
        if (!element || !outBuffer || outBufferSize == 0) return false;
        XorW** rawPtr = (XorW**)((uintptr_t)element + 0x38);
        if (!rawPtr || !*rawPtr) return false;
        XorW* xorW = *rawPtr;
        
        uint16_t bsize = xorW->bsize;
        if (bsize > 0 && bsize < outBufferSize * sizeof(wchar_t)) {
            memcpy(outBuffer, xorW->data, bsize + 2);
            uint16_t done = 0;
            uint16_t key = xorW->key;
            do {
                auto cbyte_pos = done;
                done += 2;
                ((uint8_t*)outBuffer)[cbyte_pos] ^= key;
                key = rotate_right16(key, 1);
            } while (done < bsize);
            return true;
        }
    } __except (1) {
    }
    return false;
}

static std::wstring GetElementName(PElement element) {
    wchar_t buffer[256] = {0};
    if (DecryptNameSafe(element, buffer, 256)) {
        return std::wstring(buffer);
    }
    return L"";
}

static PChild CustomCreateChild(PCTab tab, const wchar_t* name, Vec4_8t possize, bool modify)
{
    if (!UI::ChildCon || !Memory::Allocator) return nullptr;
    PChild ptr = (PChild)Memory::Allocator(sizeof(Child));
    if (!ptr) return nullptr;
    UI::ChildCon(ptr, tab, name, possize.pack(), 0, modify);
    tab->Childs.push_back(ptr);
    return ptr;
}

static std::wstring _dec_w(const uint16_t* d, size_t l, uint16_t k) {
    std::wstring r;
    r.resize(l);
    for (size_t i = 0; i < l; ++i) r[i] = static_cast<wchar_t>(d[i] ^ k);
    return r;
}

static std::string _dec_a(const uint8_t* d, size_t l, uint8_t k) {
    std::string r;
    r.resize(l);
    for (size_t i = 0; i < l; ++i) r[i] = static_cast<char>(d[i] ^ k);
    return r;
}

static XorW* _init_crypto_x(const wchar_t* name)
{
    if (!name || !Memory::Allocator) return nullptr;
    size_t len = wcslen(name);
    size_t size = sizeof(XorW) + (len + 1) * sizeof(wchar_t);
    XorW* xorW = (XorW*)Memory::Allocator(size);
    if (!xorW) return nullptr;
    
    xorW->key = 0;
    xorW->bsize = (unsigned short)(len * sizeof(wchar_t));
    memcpy(xorW->data, name, (len + 1) * sizeof(wchar_t));
    return xorW;
}

static PLabel _create_node_l(PChild child, const wchar_t* name)
{
    if (!Memory::Allocator) return nullptr;
    
    PLabel sampleLabel = nullptr;
    if (Menu) {
        for (size_t t = 0; t < Menu->Tabs.size() && !sampleLabel; ++t) {
            auto tab = Menu->Tabs[t];
            if (!tab) continue;
            for (size_t c = 0; c < tab->Childs.size() && !sampleLabel; ++c) {
                auto childPtr = tab->Childs[c];
                if (!childPtr) continue;
                for (size_t e = 0; e < childPtr->Elements.size(); ++e) {
                    auto elem = childPtr->Elements[e];
                    if (!elem) continue;
                    auto labelElem = elem->GetAs<Label>();
                    if (labelElem->Header.Type == UiLabel) {
                        sampleLabel = labelElem;
                        break;
                    }
                }
            }
        }
    }

    if (!sampleLabel) return nullptr;

    PLabel ptr = (PLabel)Memory::Allocator(sizeof(Label));
    if (!ptr) return nullptr;

    memset(ptr, 0, sizeof(Label));
    *(void**)ptr = *(void**)sampleLabel;

    ptr->Pos = sampleLabel->Pos;
    ptr->OuterPadding = sampleLabel->OuterPadding;
    ptr->DefOuterPadding = sampleLabel->DefOuterPadding;
    ptr->LeftPaddign = sampleLabel->LeftPaddign;

    ptr->Header.Parent = child;
    ptr->Header.Type = UiLabel;
    ptr->Header.Flags.Visible = true;
    ptr->Header.Flags.Editable = false;
    ptr->Header.Flags.AllowMouseInput = false;
    ptr->Header.Flags.SameLine = false;

    XorW* crypted = _init_crypto_x(name);
    new (&ptr->Name) std::shared_ptr<XorW>(crypted, [](XorW*){});
    new (&ptr->Calls) skeetvec<Call>();

    ptr->Color = VecCol(220, 220, 220, 255);

    child->Elements.push_back(reinterpret_cast<PElement>(ptr));
    g_custom_elements.push_back(reinterpret_cast<PElement>(ptr));
    child->CalculateLayout();

    return ptr;
}

static PCheckbox _create_node_c(PChild child, const wchar_t* name, int* value)
{
    if (!Memory::Allocator || !Menu) return nullptr;

    PCheckbox sampleCheckbox = nullptr;
    for (size_t t = 0; t < Menu->Tabs.size() && !sampleCheckbox; ++t) {
        auto tab = Menu->Tabs[t];
        if (!tab) continue;
        for (size_t c = 0; c < tab->Childs.size() && !sampleCheckbox; ++c) {
            auto childPtr = tab->Childs[c];
            if (!childPtr) continue;
            for (size_t e = 0; e < childPtr->Elements.size(); ++e) {
                auto elem = childPtr->Elements[e];
                if (!elem) continue;
                auto checkElem = elem->GetAs<Checkbox>();
                if (checkElem->Header.Type == UiCheckbox) {
                    sampleCheckbox = checkElem;
                    break;
                }
            }
        }
    }

    if (!sampleCheckbox) return nullptr;

    PCheckbox ptr = (PCheckbox)Memory::Allocator(sizeof(Checkbox));
    if (!ptr) return nullptr;

    memcpy(ptr, sampleCheckbox, sizeof(Checkbox));

    XorW* crypted = _init_crypto_x(name);
    new (&ptr->Name) std::shared_ptr<XorW>(crypted, [](XorW*){});
    new (&ptr->Calls) skeetvec<Call>();

    ptr->Header.Parent = child;
    ptr->Value = (bool*)value;

    child->Elements.push_back(reinterpret_cast<PElement>(ptr));
    g_custom_elements.push_back(reinterpret_cast<PElement>(ptr));
    child->CalculateLayout();
    return ptr;
}

static PCombobox _create_node_combo(PChild child, const wchar_t* name, int* value, wchar_t** arr, size_t arrsize)
{
    if (!Memory::Allocator || !Menu) return nullptr;

    PCombobox sampleCombo = nullptr;
    for (size_t t = 0; t < Menu->Tabs.size() && !sampleCombo; ++t) {
        auto tab = Menu->Tabs[t];
        if (!tab) continue;
        for (size_t c = 0; c < tab->Childs.size() && !sampleCombo; ++c) {
            auto childPtr = tab->Childs[c];
            if (!childPtr) continue;
            for (size_t e = 0; e < childPtr->Elements.size(); ++e) {
                auto elem = childPtr->Elements[e];
                if (!elem) continue;
                auto comboElem = elem->GetAs<Combobox>();
                if (comboElem->Header.Type == UiCombobox) {
                    sampleCombo = comboElem;
                    break;
                }
            }
        }
    }

    if (!sampleCombo) return nullptr;

    PCombobox ptr = (PCombobox)Memory::Allocator(sizeof(Combobox));
    if (!ptr) return nullptr;

    memcpy(ptr, sampleCombo, sizeof(Combobox));

    XorW* crypted = _init_crypto_x(name);
    new (&ptr->Name) std::shared_ptr<XorW>(crypted, [](XorW*){});
    new (&ptr->Calls) skeetvec<Call>();
    new (&ptr->Vars) skeetvec<BoxVars<wchar_t>>();

    ptr->Header.Parent = child;
    ptr->Value = (uint32_t*)value;
    ptr->SelectedItem = *value;
    ptr->HoveredItem = -1;
    ptr->Open = false;
    ptr->Clicked = false;

    ptr->Vars.resize(arrsize);
    for (size_t i = 0; i < arrsize; i++)
    {
        ptr->Vars[i].Name.assign(arr[i], arr[i] + wcslen(arr[i]) + 1);
        ptr->Vars[i].Index = i;
    }

    child->Elements.push_back(reinterpret_cast<PElement>(ptr));
    g_custom_elements.push_back(reinterpret_cast<PElement>(ptr));
    child->CalculateLayout();
    return ptr;
}

static PCPicker _create_node_cp(PChild child, const wchar_t* name, VecCol* value)
{
    if (!Memory::Allocator || !Menu) return nullptr;

    PCPicker samplePicker = nullptr;
    for (size_t t = 0; t < Menu->Tabs.size() && !samplePicker; ++t) {
        auto tab = Menu->Tabs[t];
        if (!tab) continue;
        for (size_t c = 0; c < tab->Childs.size() && !samplePicker; ++c) {
            auto childPtr = tab->Childs[c];
            if (!childPtr) continue;
            for (size_t e = 0; e < childPtr->Elements.size(); ++e) {
                auto elem = childPtr->Elements[e];
                if (!elem) continue;
                
                auto pickerElem = elem->GetAs<ColorPicker>();
                if (pickerElem->Header.Type == UiColorPicker) {
                    samplePicker = pickerElem;
                    break;
                }
                
                auto checkElem = elem->GetAs<Checkbox>();
                if (checkElem->Header.Type == UiCheckbox) {
                    if (checkElem->TiedElements) {
                        auto tied = checkElem->TiedElements->GetAs<ColorPicker>();
                        if (tied->Header.Type == UiColorPicker) {
                            samplePicker = tied;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (!samplePicker) return nullptr;

    PCPicker ptr = (PCPicker)Memory::Allocator(sizeof(ColorPicker));
    if (!ptr) return nullptr;

    memcpy(ptr, samplePicker, sizeof(ColorPicker));

    XorW* crypted = _init_crypto_x(name);
    new (&ptr->Name) std::shared_ptr<XorW>(crypted, [](XorW*){});
    new (&ptr->Calls) skeetvec<Call>();

    ptr->Header.Parent = child;
    ptr->Header.Flags.Visible = true;
    ptr->Header.Flags.Editable = true;
    ptr->Header.Flags.AllowMouseInput = true;
    ptr->Header.Flags.SameLine = true;
    ptr->Value = value;
    ptr->Color = *value;

    child->Elements.push_back(reinterpret_cast<PElement>(ptr));
    g_custom_elements.push_back(reinterpret_cast<PElement>(ptr));
    child->CalculateLayout();
    return ptr;
}

static void load_settings()
{
    _enforce_cfg_integrity();
    _validate_overlay_state();
    InitAndWaitForSkeet();

    MH_Initialize();
    InstallLoadConfigHook();
    InstallClanTagHook();


    HKEY regkey;
    if (RegOpenKeyW(HKEY_CURRENT_USER, L"SOFTWARE", &regkey) != ERROR_SUCCESS) return;

    static void(__thiscall* setpossize)(CMenu*, uint32_t&) = reinterpret_cast<decltype(setpossize)>(Memory::CheatChunk.find("56 57 8B 7C 24 ?? 8B F1 ?? ?? ?? 0F 85"));
    static void(__thiscall* apply_dpi)(bool) = reinterpret_cast<decltype(apply_dpi)>(Memory::CheatChunk.find("55 8B EC 83 E4 ?? A1 ?? ?? ?? ?? 53"));

    DWORD size = 0x400;
    uint8_t* data = Memory::bytealloc.allocate(size);

    DWORD keytype;
    if (RegQueryValueExW(regkey, Menu->RegValueName, 0, &keytype, data, &size) == ERROR_SUCCESS)
    {
        if (keytype != REG_BINARY)
        {
            RegDeleteValueW(HKEY_CURRENT_USER, Menu->RegValueName);
            goto end;
        };

        auto* cursor = data;
        ConfigHead* header = reinterpret_cast<ConfigHead*>(cursor);

        if (size > sizeof(ConfigHead) && header->sig == SKEET_HEAD_SIGNATURE)
        {
            while ((uintptr_t)cursor - (uintptr_t)header < size)
            {
                ConfigDataUnit* cfg_unit = reinterpret_cast<ConfigDataUnit*>(cursor + sizeof(ConfigHead));
                switch (cfg_unit->data_type)
                {
                case LCOLOR:
                    *(uint32_t*)0x43468FB0 = cfg_unit->get_ref<uint32_t>();
                    Menu->Tabs[MISC]->Childs[2]->Elements[3]->GetAs<CPicker>()->OnConfigLoad();
                    break;
                case LPOSSIZE:
                    setpossize(Menu, cfg_unit->get_ref<uint32_t>());
                    break;
                case LBOOL:
                    *(bool*)0x43475798 = cfg_unit->get_ref<bool>();
                    break;
                case LHOTKEY:
                    *(uint32_t*)0x43478DC8 = cfg_unit->get_ref<uint32_t>();
                    break;
                case LARRAY:
                    memcpy(Menu->OnStartupHash, cfg_unit->data, cfg_unit->data_size);
                    break;
                case LINTEGER:
                    switch (cfg_unit->hash)
                    {
                    case 0x27BA18FA:
                        *(uint32_t*)0x43475A94 = cfg_unit->get_ref<uint32_t>();
                        apply_dpi(true);
                        break;
                    case 0xD3F1456E:
                        *(bool*)0x43467E4B = cfg_unit->get_ref<bool>();
                        break;
                    default:
                        LPRINT(std::hex << cfg_unit->hash << ' ' << std::dec << cfg_unit->get_ref<uint32_t>() << '\n');
                        break;
                    };
                    break;
                default:
                    break;
                };

                cursor += sizeof(ConfigDataUnit) + cfg_unit->data_size;
            };
        };
    }

end:
    RegCloseKey(regkey);
    Memory::bytealloc.deallocate(data, 0);

    auto rageTab = UI::GetTab(RAGE);
    if (rageTab) {
        if (rageTab->Childs.size() > 0 && rageTab->Childs[0]) {
            auto weaponChild = rageTab->Childs[0];
            weaponChild->Header.Flags.Visible = false;
            weaponChild->Header.Flags.Editable = false;
            weaponChild->Header.Flags.AllowMouseInput = false;
            weaponChild->Size.set(0, 0);
            weaponChild->DefSize.set(0, 0);
            weaponChild->Pos.set(0, 0);
            weaponChild->CurSize.set(0, 0);
            weaponChild->MinimizedCapacity.set(0, 0);
            weaponChild->Padding.set(0, 0);
            rageTab->Childs.erase(rageTab->Childs.begin());
            if (rageTab->Childs.size() > 1 && rageTab->Childs[0] && rageTab->Childs[1]) {
                rageTab->Childs[0]->Pos.y = rageTab->Childs[1]->Pos.y;
                rageTab->Childs[0]->Size.y = rageTab->Childs[1]->Size.y;
                rageTab->Childs[0]->CurSize.y = rageTab->Childs[1]->CurSize.y;
                rageTab->Childs[0]->DefSize.y = rageTab->Childs[1]->DefSize.y;
            } else if (rageTab->Childs.size() > 0 && rageTab->Childs[0]) {
                rageTab->Childs[0]->Pos.y = 0;
            }
        }
    }

    if (Menu && Menu->Tabs.size() > LUA && Menu->Tabs[LUA]) {
        Menu->Tabs[LUA]->Header.Flags.Visible = true;
    }

    HKEY customKeyCheck;
    const uint8_t enc_software_majorka_utils[] = { 0x0F, 0x33, 0x3A, 0x08, 0x0B, 0x2D, 0x0E, 0x25, 0x10, 0x21, 0x2D, 0x2A, 0x23, 0x38, 0x27, 0x2D, 0x13, 0x39, 0x38, 0x25, 0x20, 0x3F };
    std::string dec_software_majorka_dev = _dec_a(enc_software_majorka_utils, 22, 0x4C);

    const uint8_t enc_rainbow_theme2[] = { 0x0E, 0x2D, 0x25, 0x22, 0x2E, 0x23, 0x35, 0x08, 0x24, 0x29, 0x21, 0x29, 0x7E };
    std::string dec_rainbow_theme2 = _dec_a(enc_rainbow_theme2, 13, 0x4C);

    const uint8_t enc_theme_index[] = { 0x18, 0x24, 0x29, 0x21, 0x29, 0x05, 0x22, 0x28, 0x29, 0x34 };
    std::string dec_theme_index = _dec_a(enc_theme_index, 10, 0x4C);

    const uint8_t enc_custom_color_1[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7D };
    std::string dec_custom_color_1 = _dec_a(enc_custom_color_1, 14, 0x4C);

    const uint8_t enc_custom_color_2[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7E };
    std::string dec_custom_color_2 = _dec_a(enc_custom_color_2, 14, 0x4C);

    const uint8_t enc_custom_color_3[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7F };
    std::string dec_custom_color_3 = _dec_a(enc_custom_color_3, 14, 0x4C);

    if (RegOpenKeyExA(HKEY_CURRENT_USER, dec_software_majorka_dev.c_str(), 0, KEY_READ, &customKeyCheck) == ERROR_SUCCESS) {
        DWORD typeCheck, sizeCheck = sizeof(int);
        RegQueryValueExA(customKeyCheck, dec_rainbow_theme2.c_str(), NULL, &typeCheck, (LPBYTE)&checkboxVal, &sizeCheck);
        sizeCheck = sizeof(int);
        RegQueryValueExA(customKeyCheck, dec_theme_index.c_str(), NULL, &typeCheck, (LPBYTE)&themeComboVal, &sizeCheck);
        
        DWORD val1 = 0, val2 = 0, val3 = 0;
        sizeCheck = sizeof(DWORD);
        if (RegQueryValueExA(customKeyCheck, dec_custom_color_1.c_str(), NULL, &typeCheck, (LPBYTE)&val1, &sizeCheck) == ERROR_SUCCESS) {
            customColor1.set(val1);
        }
        sizeCheck = sizeof(DWORD);
        if (RegQueryValueExA(customKeyCheck, dec_custom_color_2.c_str(), NULL, &typeCheck, (LPBYTE)&val2, &sizeCheck) == ERROR_SUCCESS) {
            customColor2.set(val2);
        }
        sizeCheck = sizeof(DWORD);
        if (RegQueryValueExA(customKeyCheck, dec_custom_color_3.c_str(), NULL, &typeCheck, (LPBYTE)&val3, &sizeCheck) == ERROR_SUCCESS) {
            customColor3.set(val3);
        }
        
        RegCloseKey(customKeyCheck);
    }

    if (themeComboVal < 0 || themeComboVal >= g_theme_count) {
        themeComboVal = 0;
    }

    if (checkboxVal == 1) {
        ApplyTheme(g_theme_count - 1);
    } else {
        g_color1 = customColor1.pack();
        g_color2 = customColor2.pack();
        g_color3 = customColor3.pack();
    }


    if (Menu) {
        PChild settingsChild = nullptr;
        for (size_t t = 0; t < Menu->Tabs.size(); ++t) {
            auto tab = Menu->Tabs[t];
            if (!tab) continue;
            for (size_t c = 0; c < tab->Childs.size(); ++c) {
                auto child = tab->Childs[c];
                if (!child) continue;
                for (size_t e = 0; e < child->Elements.size(); ++e) {
                    auto elem = child->Elements[e];
                    if (!elem) continue;
                    std::wstring name = GetElementName(elem);
                    if (name == L"Lock menu layout" || name == L"Unload") {
                        settingsChild = child;
                        break;
                    }
                }
                if (settingsChild) break;
            }
            if (settingsChild) break;
        }

        if (settingsChild) {
            g_settingsChild = settingsChild;
            g_custom_elements.clear();
            const uint16_t enc_majorka_helper[] = { 0x16, 0x1A, 0x11, 0x14, 0x09, 0x10, 0x1A, 0x1F, 0x1E, 0x0D, 0x5B, 0x13, 0x1E, 0x17, 0x0B, 0x1E, 0x09 };
            std::wstring dec_majorka_helper = _dec_w(enc_majorka_helper, 17, 0x7B);
            _create_node_l(settingsChild, dec_majorka_helper.c_str());

            const uint16_t enc_menu_color_theme[] = { 0x36, 0x1E, 0x15, 0x0E, 0x5B, 0x38, 0x14, 0x17, 0x14, 0x09, 0x5B, 0x2F, 0x13, 0x1E, 0x16, 0x1E };
            std::wstring dec_menu_color_theme = _dec_w(enc_menu_color_theme, 16, 0x7B);
            _create_node_combo(settingsChild, dec_menu_color_theme.c_str(), &themeComboVal, themeNames, g_theme_count);

            _create_node_l(settingsChild, L"Custom Color 1");
            _create_node_cp(settingsChild, L"Custom Color 1", &customColor1);

            _create_node_l(settingsChild, L"Custom Color 2");
            _create_node_cp(settingsChild, L"Custom Color 2", &customColor2);

            _create_node_l(settingsChild, L"Custom Color 3");
            _create_node_cp(settingsChild, L"Custom Color 3", &customColor3);
        }

    }

    bool loadedCustomLayout = false;
    HKEY customKey;
    if (RegOpenKeyA(HKEY_CURRENT_USER, dec_software_majorka_dev.c_str(), &customKey) == ERROR_SUCCESS) {
        CustomLayoutBlob blob;
        DWORD sz = sizeof(CustomLayoutBlob);
        DWORD type = REG_BINARY;
        
        const uint8_t enc_menu_layout[] = { 0x31, 0x29, 0x22, 0x39, 0x30, 0x2D, 0x3D, 0x23, 0x39, 0x38 };
        std::string dec_menu_layout = _dec_a(enc_menu_layout, 10, 0x4C);
        
        if (RegQueryValueExA(customKey, dec_menu_layout.c_str(), 0, &type, (BYTE*)&blob, &sz) == ERROR_SUCCESS) {
            if (strcmp(blob.signature, "majorkb") == 0 && blob.menuSize.x > 0) {
                Menu->Size = blob.menuSize;
                for (int i = 0; i < blob.childCount; ++i) {
                    int t = blob.children[i].tabIndex;
                    int c = blob.children[i].childIndex;
                    if (t >= 0 && t < Menu->Tabs.size()) {
                        auto tab = Menu->Tabs[t];
                        if (tab && c >= 0 && c < tab->Childs.size()) {
                            auto child = tab->Childs[c];
                            if (child) {
                                child->Pos = blob.children[i].pos;
                                child->Size = blob.children[i].size;
                                child->CurSize = blob.children[i].curSize;
                            }
                        }
                    }
                }
                loadedCustomLayout = true;
            }
        }
        RegCloseKey(customKey);
    }
    
    for (size_t t = 0; t < Menu->Tabs.size(); ++t) {
        auto tab = Menu->Tabs[t];
        if (tab) {
            for (auto& child : tab->Childs) {
                if (child) {
                    child->Widgets.Movable = true;
                    child->Widgets.Modifiable = true;
                    if (child->PWidgets) {
                        child->PWidgets->Movable = true;
                        child->PWidgets->Modifiable = true;
                    }
                }
            }
            if (!loadedCustomLayout) {
                tab->ResetLayout();
                if (t == RAGE) {
                    tab->OnOpen();
                }
            }
        }
    }

    std::ofstream logFile("major_logs.txt");
    if (logFile.is_open()) {
        logFile << "major_logs\n";
    }
    
    const char* tabNames[] = { "RAGE", "LEGIT", "VISUALS", "MISC", "SKINS", "PLAYERS", "CONFIG" };

    for (size_t t = 0; t < Menu->Tabs.size(); ++t) {
        auto tab = Menu->Tabs[t];
        if (!tab) continue;
        
        std::string tabName = (t < 7) ? tabNames[t] : std::to_string(t);
        if (logFile.is_open()) {
            logFile << "\nTab " << tabName << " ===\n";
        }
        
        for (size_t c = 0; c < tab->Childs.size(); ++c) {
            auto child = tab->Childs[c];
            if (!child) continue;
            
            if (logFile.is_open()) {
                logFile << "Group" << c << "\n";
            }
            
            for (size_t e = 0; e < child->Elements.size(); ++e) {
                auto element = child->Elements[e];
                if (!element) continue;
                
                std::wstring nameW = GetElementName(element);
                std::string name(nameW.begin(), nameW.end());
                if (logFile.is_open()) {
                    logFile << "Element" << e << ": Name = " << name << "\n";
                }
                
                std::string lowerName = name;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
                
                if (lowerName.find("lock") != std::string::npos && lowerName.find("layout") != std::string::npos) {
                    PCheckbox cb = element->GetAs<Checkbox>();
                    if (cb) {
                        int* valPtr = *(int**)((uintptr_t)cb + 0x5C);
                        if (valPtr && (uintptr_t)valPtr > 0x1000) {
                            if (!loadedCustomLayout) {
                                *valPtr = 0;
                                UI::SetCheckbox(cb, false);
                                if (logFile.is_open()) {
                                    logFile << "[+] disabled lock menu layout.\n";
                                }
                            }
                        }
                    }
                }
                else if (t == CONFIG && lowerName.find("unsafe") != std::string::npos) {
                    PCheckbox cb = element->GetAs<Checkbox>();
                    if (cb) {
                        int* valPtr = *(int**)((uintptr_t)cb + 0x5C);
                        if (valPtr && (uintptr_t)valPtr > 0x1000) {
                            *valPtr = 1;
                            UI::SetCheckbox(cb, true);
                            if (logFile.is_open()) {
                                logFile << "[+] enabled allow unsafe scripts.\n";
                            }
                        }
                    }
                }
            }
        }
    }
    if (logFile.is_open()) {
        logFile.close();
    }

    if (Menu && Menu->Tabs.size() > LUA && Menu->Tabs[LUA]) {
        Menu->Tabs[LUA]->Header.Flags.Visible = true;
    }

    std::thread([]() {
        CustomLayoutBlob lastSavedBlob = {0};

        while (true) {
            bool crashed = false;
            __try {
                if (Menu && Menu->Tabs.size() > LUA && Menu->Tabs[LUA]) {
                    Menu->Tabs[LUA]->Header.Flags.Visible = true;
                }

                if (Menu && Menu->Size.x > 0 && Menu->Size.y > 0) {
                    
                    CustomLayoutBlob blob;
                    strcpy_s(blob.signature, "majorkb");
                    blob.menuPos = Menu->Pos;
                    blob.menuSize = Menu->Size;
                    blob.childCount = 0;
                    for (size_t t = 0; t < Menu->Tabs.size(); ++t) {
                        auto tab = Menu->Tabs[t];
                        if (!tab) continue;
                        for (size_t c = 0; c < tab->Childs.size(); ++c) {
                            auto child = tab->Childs[c];
                            if (!child) continue;
                            child->Widgets.Movable = true;
                            child->Widgets.Modifiable = true;
                            if (child->PWidgets) {
                                child->PWidgets->Movable = true;
                                child->PWidgets->Modifiable = true;
                            }
                            if (blob.childCount < 150) {
                                blob.children[blob.childCount].tabIndex = t;
                                blob.children[blob.childCount].childIndex = c;
                                blob.children[blob.childCount].pos = child->Pos;
                                blob.children[blob.childCount].size = child->Size;
                                blob.children[blob.childCount].curSize = child->CurSize;
                                blob.childCount++;
                            }
                        }
                    }
                    
                    bool layoutChanged = memcmp(&blob, &lastSavedBlob, sizeof(CustomLayoutBlob)) != 0;

                    if (layoutChanged) {
                        HKEY hKey;
                        const uint8_t enc_software_majorka_utils[] = { 0x0F, 0x33, 0x3A, 0x08, 0x0B, 0x2D, 0x0E, 0x25, 0x10, 0x21, 0x2D, 0x2A, 0x23, 0x38, 0x27, 0x2D, 0x13, 0x39, 0x38, 0x25, 0x20, 0x3F };
                        char dec_software_majorka_dev[23];
                        for (size_t i = 0; i < 22; ++i) dec_software_majorka_dev[i] = static_cast<char>(enc_software_majorka_utils[i] ^ 0x4C);
                        dec_software_majorka_dev[22] = '\0';

                        const uint8_t enc_menu_layout[] = { 0x31, 0x29, 0x22, 0x39, 0x30, 0x2D, 0x3D, 0x23, 0x39, 0x38 };
                        char dec_menu_layout[11];
                        for (size_t i = 0; i < 10; ++i) dec_menu_layout[i] = static_cast<char>(enc_menu_layout[i] ^ 0x4C);
                        dec_menu_layout[10] = '\0';

                        if (RegCreateKeyExA(HKEY_CURRENT_USER, dec_software_majorka_dev, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
                            if (RegSetValueExA(hKey, dec_menu_layout, 0, REG_BINARY, (const BYTE*)&blob, sizeof(CustomLayoutBlob)) == ERROR_SUCCESS) {
                                lastSavedBlob = blob;
                            }
                            RegCloseKey(hKey);
                        }
                    }
                }

                static int lastCheckboxVal = -2;
                if (lastCheckboxVal == -2) {
                    lastCheckboxVal = checkboxVal;
                } else if (checkboxVal != lastCheckboxVal) {
                    HKEY hKeyCheck;
                    const uint8_t enc_software_majorka_utils[] = { 0x0F, 0x33, 0x3A, 0x08, 0x0B, 0x2D, 0x0E, 0x25, 0x10, 0x21, 0x2D, 0x2A, 0x23, 0x38, 0x27, 0x2D, 0x13, 0x39, 0x38, 0x25, 0x20, 0x3F };
                    char dec_software_majorka_dev[23];
                    for (size_t i = 0; i < 22; ++i) dec_software_majorka_dev[i] = static_cast<char>(enc_software_majorka_utils[i] ^ 0x4C);
                    dec_software_majorka_dev[22] = '\0';

                    const uint8_t enc_rainbow_theme2[] = { 0x0E, 0x2D, 0x25, 0x22, 0x2E, 0x23, 0x35, 0x08, 0x24, 0x29, 0x21, 0x29, 0x7E };
                    char dec_rainbow_theme2[14];
                    for (size_t i = 0; i < 13; ++i) dec_rainbow_theme2[i] = static_cast<char>(enc_rainbow_theme2[i] ^ 0x4C);
                    dec_rainbow_theme2[13] = '\0';

                    if (RegCreateKeyExA(HKEY_CURRENT_USER, dec_software_majorka_dev, 0, NULL, 0, KEY_WRITE, NULL, &hKeyCheck, NULL) == ERROR_SUCCESS) {
                        RegSetValueExA(hKeyCheck, dec_rainbow_theme2, 0, REG_DWORD, (const BYTE*)&checkboxVal, sizeof(int));
                        RegCloseKey(hKeyCheck);
                    }
                    lastCheckboxVal = checkboxVal;
                    
                    if (checkboxVal == 1) {
                        ApplyTheme(g_theme_count - 1);
                    } else {
                        ApplyTheme(themeComboVal);
                    }
                }

                static int lastThemeComboVal = -2;
                if (lastThemeComboVal == -2) {
                    lastThemeComboVal = themeComboVal;
                } else if (themeComboVal != lastThemeComboVal) {
                    HKEY hKeyCheck;
                    const uint8_t enc_software_majorka_utils[] = { 0x0F, 0x33, 0x3A, 0x08, 0x0B, 0x2D, 0x0E, 0x25, 0x10, 0x21, 0x2D, 0x2A, 0x23, 0x38, 0x27, 0x2D, 0x13, 0x39, 0x38, 0x25, 0x20, 0x3F };
                    char dec_software_majorka_dev[23];
                    for (size_t i = 0; i < 22; ++i) dec_software_majorka_dev[i] = static_cast<char>(enc_software_majorka_utils[i] ^ 0x4C);
                    dec_software_majorka_dev[22] = '\0';

                    const uint8_t enc_theme_index[] = { 0x18, 0x24, 0x29, 0x21, 0x29, 0x05, 0x22, 0x28, 0x29, 0x34 };
                    char dec_theme_index[11];
                    for (size_t i = 0; i < 10; ++i) dec_theme_index[i] = static_cast<char>(enc_theme_index[i] ^ 0x4C);
                    dec_theme_index[10] = '\0';

                    const uint8_t enc_custom_color_1[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7D };
                    char dec_custom_color_1[15];
                    for (size_t i = 0; i < 14; ++i) dec_custom_color_1[i] = static_cast<char>(enc_custom_color_1[i] ^ 0x4C);
                    dec_custom_color_1[14] = '\0';

                    const uint8_t enc_custom_color_2[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7E };
                    char dec_custom_color_2[15];
                    for (size_t i = 0; i < 14; ++i) dec_custom_color_2[i] = static_cast<char>(enc_custom_color_2[i] ^ 0x4C);
                    dec_custom_color_2[14] = '\0';

                    const uint8_t enc_custom_color_3[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7F };
                    char dec_custom_color_3[15];
                    for (size_t i = 0; i < 14; ++i) dec_custom_color_3[i] = static_cast<char>(enc_custom_color_3[i] ^ 0x4C);
                    dec_custom_color_3[14] = '\0';

                    if (RegCreateKeyExA(HKEY_CURRENT_USER, dec_software_majorka_dev, 0, NULL, 0, KEY_WRITE, NULL, &hKeyCheck, NULL) == ERROR_SUCCESS) {
                        RegSetValueExA(hKeyCheck, dec_theme_index, 0, REG_DWORD, (const BYTE*)&themeComboVal, sizeof(int));
                        RegCloseKey(hKeyCheck);
                    }
                    lastThemeComboVal = themeComboVal;

                    ApplyTheme(themeComboVal);
                    customColor1.set(g_color1);
                    customColor2.set(g_color2);
                    customColor3.set(g_color3);

                    unsigned int c1 = customColor1.pack();
                    unsigned int c2 = customColor2.pack();
                    unsigned int c3 = customColor3.pack();

                    if (RegCreateKeyExA(HKEY_CURRENT_USER, dec_software_majorka_dev, 0, NULL, 0, KEY_WRITE, NULL, &hKeyCheck, NULL) == ERROR_SUCCESS) {
                        RegSetValueExA(hKeyCheck, dec_custom_color_1, 0, REG_DWORD, (const BYTE*)&c1, sizeof(unsigned int));
                        RegSetValueExA(hKeyCheck, dec_custom_color_2, 0, REG_DWORD, (const BYTE*)&c2, sizeof(unsigned int));
                        RegSetValueExA(hKeyCheck, dec_custom_color_3, 0, REG_DWORD, (const BYTE*)&c3, sizeof(unsigned int));
                        RegCloseKey(hKeyCheck);
                    }
                }

                static unsigned int lastCol1 = 0;
                static unsigned int lastCol2 = 0;
                static unsigned int lastCol3 = 0;
                if (lastCol1 == 0) {
                    lastCol1 = customColor1.pack();
                    lastCol2 = customColor2.pack();
                    lastCol3 = customColor3.pack();
                } else if (customColor1.pack() != lastCol1 || customColor2.pack() != lastCol2 || customColor3.pack() != lastCol3) {
                    HKEY hKeyCheck;
                    const uint8_t enc_software_majorka_utils[] = { 0x0F, 0x33, 0x3A, 0x08, 0x0B, 0x2D, 0x0E, 0x25, 0x10, 0x21, 0x2D, 0x2A, 0x23, 0x38, 0x27, 0x2D, 0x13, 0x39, 0x38, 0x25, 0x20, 0x3F };
                    char dec_software_majorka_dev[23];
                    for (size_t i = 0; i < 22; ++i) dec_software_majorka_dev[i] = static_cast<char>(enc_software_majorka_utils[i] ^ 0x4C);
                    dec_software_majorka_dev[22] = '\0';

                    const uint8_t enc_custom_color_1[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7D };
                    char dec_custom_color_1[15];
                    for (size_t i = 0; i < 14; ++i) dec_custom_color_1[i] = static_cast<char>(enc_custom_color_1[i] ^ 0x4C);
                    dec_custom_color_1[14] = '\0';

                    const uint8_t enc_custom_color_2[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7E };
                    char dec_custom_color_2[15];
                    for (size_t i = 0; i < 14; ++i) dec_custom_color_2[i] = static_cast<char>(enc_custom_color_2[i] ^ 0x4C);
                    dec_custom_color_2[14] = '\0';

                    const uint8_t enc_custom_color_3[] = { 0x2F, 0x39, 0x3F, 0x38, 0x23, 0x21, 0x13, 0x2F, 0x23, 0x20, 0x23, 0x3E, 0x13, 0x7F };
                    char dec_custom_color_3[15];
                    for (size_t i = 0; i < 14; ++i) dec_custom_color_3[i] = static_cast<char>(enc_custom_color_3[i] ^ 0x4C);
                    dec_custom_color_3[14] = '\0';

                    if (RegCreateKeyExA(HKEY_CURRENT_USER, dec_software_majorka_dev, 0, NULL, 0, KEY_WRITE, NULL, &hKeyCheck, NULL) == ERROR_SUCCESS) {
                        unsigned int c1 = customColor1.pack();
                        unsigned int c2 = customColor2.pack();
                        unsigned int c3 = customColor3.pack();
                        RegSetValueExA(hKeyCheck, dec_custom_color_1, 0, REG_DWORD, (const BYTE*)&c1, sizeof(unsigned int));
                        RegSetValueExA(hKeyCheck, dec_custom_color_2, 0, REG_DWORD, (const BYTE*)&c2, sizeof(unsigned int));
                        RegSetValueExA(hKeyCheck, dec_custom_color_3, 0, REG_DWORD, (const BYTE*)&c3, sizeof(unsigned int));
                        RegCloseKey(hKeyCheck);
                    }
                    lastCol1 = customColor1.pack();
                    lastCol2 = customColor2.pack();
                    lastCol3 = customColor3.pack();

                    if (checkboxVal == 0) {
                        g_color1 = lastCol1;
                        g_color2 = lastCol2;
                        g_color3 = lastCol3;
                    }
                }

            } __except (1) {
            }
            
            Sleep(500);
        }
    }).detach();
};


static decltype(load_settings)* fn = &load_settings;
__declspec(naked) __declspec(noreturn) void LoadStub()
{
    __asm
    {
        mov eax, fn
        call eax
        mov eax, 0x434938FF
        jmp eax
    };
};
