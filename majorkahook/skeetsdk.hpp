#ifndef SKEET_SDK_HPP

#define SKEET_SDK_HPP
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include <span>
#include <string_view>
#include <concepts>

#pragma comment(lib, "ntdll.lib")
extern "C"
{
	PVOID WINAPI RtlAllocateHeap(PVOID HeapHandle, ULONG Flags, SIZE_T Size);
	ULONG WINAPI RtlFreeHeap(PVOID HeapHandle, ULONG Flags, PVOID BaseAddress);
};

static_assert(sizeof(uintptr_t) == 4, "You should build solution in x86(32 bit) mode!");

#if !defined(SKEETSDK_ENABLE_DEBUG_LOGGING) && (defined(DEBUG) || defined(_DEBUG))
#define SKEETSDK_ENABLE_DEBUG_LOGGING
#endif // !SKEETSDK_ENABLE_DEBUG_LOGGING && (DEBUG || _DEBUG)

#define SKEETSDK_ASSERT_SIZE(_struct, _size) static_assert(sizeof(_struct) == _size, #_struct " should be " #_size " bytes long");

#define SKEETSDK_NOINLINE [[msvc::noinline]]
#define SKEETSDK_FORCEINLINE [[msvc::forceinline]]
#define SKEETSDK_FLATTEN [[msvc::flatten]]
#define SKEETSDK_EBCO __declspec(empty_bases)

#define SKEETSDK_DECLASSIGN(left, right) left = (decltype(left))right;

#define SKEETSDK_HANDLE_INTERFACE(i) \
protected: \
friend class skeetsdk::containers::lazy_ptr<i>; \
i(skeetsdk::GlobalContext& gctx); \
~i() = default; \
i(const i&) = delete; \
i& operator=(const i&) = delete; \
private:

#define SKEETSDK_GLOBAL_INTERFACE(i) \
SKEETSDK_HANDLE_INTERFACE(i) \
skeetsdk::GlobalContext& gctx;

#ifndef SKEETSDK_PUBLIC_ALL
#define SKEETSDK_PRIVATE_MEMBERS private
#else // SKEETSDK_PUBLIC_ALL
#define SKEETSDK_PRIVATE_MEMBERS public
#endif // !SKEETSDK_PUBLIC_ALL


#ifdef SKEETSDK_ENABLE_DEBUG_LOGGING
#if !defined(SKEETSDK_LOG) || !defined(SKEETSDK_ERROR)
#define SKEETSDK_LOG(...) fprintf(stdout, __VA_ARGS__)
#define SKEETSDK_ERROR(...) fprintf(stderr, __VA_ARGS__)
#endif // !SKEETSDK_LOG || !SKEETSDK_ERROR
#else // !SKEETSDKE_ENABLE_DEBUG_LOGGING
#define SKEETSDK_LOG(...)
#define SKEETSDK_ERROR(...)
#endif // SKEETSDKE_ENABLE_DEBUG_LOGGING

namespace skeetsdk
{
	class GlobalContext;


	using CFn = void* (__cdecl*)(void);
	using ThisFn = void(__thiscall*)(void* this_ptr);
	using ThisPtrFn = void(__thiscall*)(void* this_ptr, void* arg1);
	using Fast2PtrFn = void(__fastcall*)(void* arg0, void* arg1);
	using BoolThisFn = bool(__thiscall*)(void* this_ptr);
	using ThisIntFn = void(__thiscall*)(void* this_ptr, int arg1);


	namespace misc
	{
		template<typename T>
		struct Vec2
		{
		public:
			T x, y;

			Vec2 operator+(const Vec2& oth)
			{
				return Vec2(this->x + oth.x, this->y + oth.y);
			};
		};

		template<typename T>
		struct Vec4
		{
		public:
			T x, y, z, w;
		};

		struct RGBA
		{
			uint8_t r, g, b, a;

			constexpr RGBA(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {};

			constexpr RGBA(uint32_t hex)
			{
				r = hex & 0xFF;
				g = (hex >> 8) & 0xFF;
				b = (hex >> 16) & 0xFF;
				a = (hex >> 24) & 0xFF;
			}
		};

		struct MsgInfo
		{
			HWND hWnd;
			UINT uMsg;
			WPARAM wParam;
			LPARAM lParam;
		};

		struct Call
		{
			void* ecx;
			Fast2PtrFn function;
		};
	};	// namespace misc

	namespace mem
	{
		constexpr uintptr_t skeet_image_base = 0x43310000u;
		constexpr uint32_t skeet_image_size = 0x2FC000u;
		constexpr uintptr_t skeet_image_end = skeet_image_base + skeet_image_size;

#ifdef SKEETSDK_TARGET_CRACK
		namespace constants
		{
			constexpr uintptr_t menu_address = 0x434799AC;
			constexpr uintptr_t renderer_queue_address = 0x43479928;
			constexpr uintptr_t config_hash_address = 0x4347C9F8;
			constexpr uintptr_t get_config_data_address = 0x43359C2A;
			constexpr uintptr_t load_config_data_address = 0x43341D90;
			constexpr uintptr_t fetch_rage_tab_address = 0x4332F0AC;
			constexpr uintptr_t get_slua_address = 0x433CBCBB;
			constexpr uintptr_t lua_loader_address = 0x43475B00;
			constexpr uintptr_t is_lua_loaded_address = 0x43347065;
			constexpr uintptr_t update_visual_lua_info_address = 0x433F1A21;
			constexpr uintptr_t tab_constructor_address = 0x4348E41D;
			constexpr uintptr_t insert_tab_address = 0x43342275;
			constexpr uintptr_t child_constructor_address = 0x4348210C;
			constexpr uintptr_t ui_config_list_address = 0x434798BC;
			constexpr uintptr_t add_label_address = 0x4348DC73;
			constexpr uintptr_t add_button_address = 0x4347E840;
			constexpr uintptr_t checkbox_constructor_address = 0x433ADF35;
			constexpr uintptr_t textbox_constructor_address = 0x4336A72D;
			constexpr uintptr_t slider_constructor_address = 0x433CF6B2;
			constexpr uintptr_t colorpicker_constructor_address = 0x43406A63;
			constexpr uintptr_t hotkey_constructor_address = 0x4334DD58;
			constexpr uintptr_t multiselect_constructor_address = 0x433230AD;
			constexpr uintptr_t listbox_constructor_address = 0x433EDA39;
			constexpr uintptr_t combobox_constructor_address = 0x43399927;
#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			constexpr uintptr_t flat_weaponselector_constructor_address = 0x43484CFD;
			constexpr uintptr_t case_weaponselector_constructor_address = 0x4348E18F;
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS
		};	// namespace constants
#else	// !SKEETSDK_TARGET_CRACK
		namespace signatures
		{
			using signature_type = std::vector<BYTE>;

			inline const signature_type menu_signature = { 0x89, 0x35, 0xCC, 0xCC, 0xCC, 0xCC, 0x8B, 0x0D };
			inline const signature_type renderer_signature = { 0x83, 0x3D, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x57, 0x8B, 0xFA };
			inline const signature_type current_config_hash_signature = { 0xA3, 0x00, 0x00, 0x00, 0x00, 0x8B, 0xCC, 0x8D, 0x44, 0x24, 0x30, 0x89, 0x7C, 0x24, 0x10 };
			inline const signature_type get_config_data_signature = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x18, 0x56, 0x57, 0x89 };
			inline const signature_type load_config_siganture = { 0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x40, 0x56, 0x57, 0x8B, 0xFA };
			inline const signature_type after_tab_tage_signature = { 0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xCC, 0x83, 0xEC, 0xCC, 0x56, 0x8B, 0xF1, 0x57, 0x8B, 0x86 };
			inline const signature_type get_slua_signature = { 0xA1, 0xCC, 0xCC, 0xCC, 0xCC, 0x85, 0xC0, 0x74, 0x01, 0xC3, 0x83, 0xC0, 0x44, 0xC3 };
			inline const signature_type get_or_init_luastate_signature = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x2C, 0x56, 0x57, 0xE8 };
			inline const signature_type load_lua_signature = { 0x83, 0xEC, 0x3C, 0x53, 0x55, 0x33 };
			inline const signature_type is_lua_loaded_signature = { 0x51, 0x8B, 0xD1, 0x56, 0x8B };
			inline const signature_type update_visual_lua_info_signature = { 0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xCC, 0x81, 0xEC, 0xCC, 0xCC, 0xCC, 0xCC, 0x80, 0x3D, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x53, 0x56, 0x57, 0x8B, 0xF9, 0x89, 0x7C, 0x24, 0xCC, 0x0F, 0x84 };
			inline const signature_type tab_constructor_signature = { 0x56, 0x57, 0x33, 0xFF, 0x8B, 0xF1, 0x57, 0x6A, 0x01 };
			inline const signature_type insert_tab_signature = { 0x8B, 0x41, 0x58, 0x83, 0xC1 };
			inline const signature_type child_constructor_signature = { 0x55, 0x8B, 0xEC, 0x56, 0xFF, 0x75 };
			inline const signature_type ui_config_list_signature = { 0xBD, 0xCC, 0xCC, 0xCC, 0xCC, 0xEB, 0xCC, 0x57 };
			inline const signature_type ui_hotkey_list_signature = { 0x81, 0xFE, 0xCC, 0xCC, 0xCC, 0xCC, 0x74, 0xCC, 0x8B, 0x4E };
			inline const signature_type add_label_signature = { 0x56, 0x57, 0x8B, 0xF9, 0x6A, 0xCC, 0x59, 0xE8, 0xCC, 0xCC, 0xCC, 0xCC, 0x85, 0xC0, 0x74, 0xCC, 0xFF, 0x74, 0x24, 0xCC, 0x8B, 0xC8, 0x57 };
			inline const signature_type add_button_signature = { 0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xCC, 0x56, 0x57, 0x8B, 0xF9, 0x6A };
			inline const signature_type checkbox_constructor_signature = { 0x56, 0xFF, 0x74, 0x24, 0xCC, 0x8B, 0xF1, 0x6A };
			inline const signature_type textbox_constrcutor_signature = { 0x56, 0x57, 0x33, 0xFF, 0x8B, 0xF1, 0x57, 0x6A, 0xCC, 0x68 };
			inline const signature_type slider_constrcutor_signature = { 0x55, 0x8B, 0xEC, 0x53, 0x56, 0xFF, 0x75 };
			inline const signature_type colorpicker_constrcutor_signature = { 0x53, 0x56, 0x57, 0xFF, 0x74, 0x24, 0xCC, 0x8B, 0xD9, 0x6A };
			inline const signature_type hotkey_constrcutor_signature = { 0x53, 0x8B, 0x5C, 0x24, 0xCC, 0x56, 0x53 };
			inline const signature_type multiselect_constrcutor_signature = { 0x56, 0x57, 0xFF, 0x74, 0x24, 0xCC, 0x8B, 0xF1, 0x6A, 0xCC, 0xFF, 0x74, 0x24, 0xCC, 0xFF, 0x74, 0x24, 0xCC, 0xE8, 0xCC, 0xCC, 0xCC, 0xCC, 0x8A, 0x44, 0x24 };
			inline const signature_type listbox_constrcutor_signature = { 0x55, 0x8B, 0xEC, 0x53, 0x56, 0x57, 0xFF, 0x75, 0xCC, 0x8B, 0xF9 };
			inline const signature_type combobox_constrcutor_signature = { 0x56, 0x57, 0xFF, 0x74, 0x24, 0xCC, 0x8B, 0xF1, 0x6A, 0xCC, 0xFF, 0x74, 0x24, 0xCC, 0xFF, 0x74, 0x24, 0xCC, 0xE8, 0xCC, 0xCC, 0xCC, 0xCC, 0x83, 0x4E };
#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			inline const signature_type flat_weaponselector_constrcutor_signature = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0xCC, 0x53, 0x56, 0x6A };
			inline const signature_type case_weaponselector_constrcutor_signature = { 0x55, 0x8B, 0xEC, 0x81, 0xEC, 0xCC, 0xCC, 0xCC, 0xCC, 0x53, 0x56, 0x57, 0x6A };
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			inline const signature_type renderer_events_signature = { 0x8B, 0x35, 0xCC, 0xCC, 0xCC, 0xCC, 0x53 };
		};	// namepsace signatures

#endif	// SKEETSDK_TARGET_CRACK

		HMODULE WaitForSingleHandle(const char* module);

		bool IsExecutableAddress(PVOID Address);

		class SigFinder
		{
			PVOID lpBaseOfDll;
			DWORD SizeOfImage;

			_NODISCARD SKEETSDK_NOINLINE static std::vector<BYTE> signature_resolver(const char* signature, const char wildcard, const BYTE bytecard);

			SKEETSDK_NOINLINE static void generate_shift_table(BYTE out[256], const std::vector<BYTE>& pattern, const BYTE wildcard);
		public:
			SigFinder(const char* module) : lpBaseOfDll(nullptr), SizeOfImage(0)
			{
				PVOID pBase = GetModuleHandleA(module);
				if (pBase)
				{
					lpBaseOfDll = pBase;
					SizeOfImage = PIMAGE_NT_HEADERS((PBYTE)pBase + PIMAGE_DOS_HEADER(pBase)->e_lfanew)->OptionalHeader.SizeOfImage;
				};
			};

			constexpr SigFinder(LPVOID lpBaseAddress, DWORD SizeOfImage) : lpBaseOfDll(lpBaseAddress), SizeOfImage(SizeOfImage) {};

			_NODISCARD SKEETSDK_NOINLINE void* find(const std::vector<BYTE>& pattern, const BYTE wildcard) const;

			_NODISCARD void* find(const char* signature, int offset = 0, const char wildcard = '?', const BYTE bytecard = 0xCC) const;

			_NODISCARD SKEETSDK_NOINLINE void* findr32(const char* signature, const char wildcard = '?', const BYTE bytecard = 0xCC) const;
		};

		class Detour
		{
#pragma pack(push, 1)
			struct ControlBlock
			{
				BYTE Transit[5];
				SIZE_T OriginalSize;
				BYTE Code[];
			};
#pragma pack(pop)
		public:
			static void Hook(PVOID From, PVOID To, SIZE_T AllignCount, PVOID* TrampolineOut);

			static void UnHook(PVOID From);
		};

		template<typename T>
		class SkeetAllocator
		{
		public:
			using value_type = T;
			using pointer = T*;
			using const_pointer = const T*;
			using reference = T&;
			using const_reference = const T&;
			using size_type = size_t;
			using difference_type = ptrdiff_t;

			template<typename U>
			struct rebind {
				using other = SkeetAllocator<U>;
			};

			SkeetAllocator() = default;

			template <class U>
			SkeetAllocator(const SkeetAllocator<U>&) noexcept {};

			SkeetAllocator(const SkeetAllocator&) = default;

			SkeetAllocator& operator=(const SkeetAllocator&) = default;

			_NODISCARD SKEETSDK_FORCEINLINE pointer address(reference value) { return &value; };

			_NODISCARD SKEETSDK_FORCEINLINE static pointer allocate(size_type n)
			{
				return (pointer)_alloc_impl(n * sizeof(T));
			};

			static void deallocate(pointer ptr, size_type)
			{
				if (ptr)
					RtlFreeHeap(*(PVOID*)(__readfsdword(0x30) + 0x18), 0, (char*)ptr - *((DWORD*)ptr - 2) - 16);
			};

			template <typename U>
			bool operator==(const SkeetAllocator<U>&) noexcept { return true; }

			template <typename U>
			bool operator!=(const SkeetAllocator<U>&) noexcept { return false; }
		private:
			_NODISCARD SKEETSDK_NOINLINE static void* _alloc_impl(size_t byte_size)
			{
				if (!byte_size) return nullptr;
				
				size_t alloc_size = byte_size + 32;
				PVOID allocated = RtlAllocateHeap(*(PVOID*)(__readfsdword(0x30) + 0x18), HEAP_ZERO_MEMORY, alloc_size);
				if (allocated)
				{
					DWORD* header = (DWORD*)(((DWORD)allocated + 0xF) & ~0xF);
					*(header + 2) = (DWORD)header - (DWORD)allocated;
					allocated = (PVOID)((char*)header + 16);
					*(header + 3) = alloc_size;
				}
				return allocated;
			};
		};
	};	// namespace mem

	namespace mut
	{
		class FNV1aBase
		{
		protected:
			static constexpr uint32_t prime = 0x1000193;
			static constexpr uint32_t base = 0x811C9DC5;
			static constexpr uint32_t chaos = 0x92CA5DF1;
			uint32_t value;
		public:
			using utf8 = const char*;
			using utf16 = const wchar_t*;

			SKEETSDK_FORCEINLINE constexpr FNV1aBase(const FNV1aBase& oth) { value = oth.value; };
			SKEETSDK_FORCEINLINE constexpr FNV1aBase() : value(base) {};
			SKEETSDK_FORCEINLINE constexpr FNV1aBase(uint32_t val) : value(val) {};

			constexpr uint32_t hash() const { return value; };

			constexpr uint32_t hash_raw() const { return value ^ chaos; };

			constexpr FNV1aBase& operator=(uint32_t x) { value = x; return *this; };
			constexpr FNV1aBase& operator=(const FNV1aBase& oth) { value = oth.value; return *this; };

			constexpr uint32_t operator^(uint32_t x) const { return value ^ x; };
			constexpr uint32_t operator*(uint32_t x) const { return value * x; };
			constexpr FNV1aBase& operator^=(uint32_t x) { value ^= x; return *this; };
			constexpr FNV1aBase& operator*=(uint32_t x) { value *= x; return *this; };


			constexpr bool operator==(const FNV1aBase& oth) const { return value == oth.value; };
			constexpr bool operator!=(const FNV1aBase& oth) const { return false; };

			constexpr operator uint32_t() { return value; };
		};

		class FNV1a : public FNV1aBase
		{
		public:
			SKEETSDK_NOINLINE constexpr FNV1a(utf8 str) : FNV1aBase()
			{
				if (str)
				{
					while (*str)
					{
						uint8_t ch = static_cast<uint8_t>(*str++);
						value = prime * (value ^ ch);
					};
				};
				value ^= chaos;
			};

			SKEETSDK_NOINLINE constexpr FNV1a(utf16 str) : FNV1aBase()
			{
				if (str)
				{
					while (*str)
					{
						uint16_t ch = static_cast<uint16_t>(*str++);
						value = prime * (value ^ ch);
					};
				};
				value ^= chaos;
			};

			SKEETSDK_NOINLINE constexpr FNV1a(std::string_view str) : FNV1aBase()
			{
				for (uint8_t ch : str)
					value = prime * (value ^ ch);
				value ^= chaos;
			};

			SKEETSDK_NOINLINE constexpr FNV1a(std::wstring_view str) : FNV1aBase()
			{
				for (uint16_t ch : str)
					value = prime * (value ^ ch);
				value ^= chaos;
			};
		};

		class FNV1ai : public FNV1aBase
		{
		public:
			SKEETSDK_NOINLINE constexpr FNV1ai(utf8 str) : FNV1aBase()
			{
				if (str)
				{
					while (*str)
					{
						uint8_t ch = static_cast<uint8_t>(*str++);
						ch = ch - 'A' > 25 ? ch : ch + ' ';
						value = prime * (value ^ ch);
					};
				};
				value ^= chaos;
			};

			SKEETSDK_NOINLINE constexpr FNV1ai(utf16 str) : FNV1aBase()
			{
				if (str)
				{
					while (*str)
					{
						uint16_t ch = static_cast<uint16_t>(*str++);
						ch = ch - L'A' > 25 ? ch : ch + L' ';
						value = prime * (value ^ ch);
					};
				};
				value ^= chaos;
			};

			SKEETSDK_NOINLINE constexpr FNV1ai(std::string_view str) : FNV1aBase()
			{
				for (uint8_t ch : str)
				{
					ch = ch - 'A' > 25 ? ch : ch + ' ';
					value = prime * (value ^ ch);
				};
				value ^= chaos;
			};

			SKEETSDK_NOINLINE constexpr FNV1ai(std::wstring_view str) : FNV1aBase()
			{
				for (uint16_t ch : str)
				{
					ch = ch - L'A' > 25 ? ch : ch + L' ';
					value = prime * (value ^ ch);
				}
				value ^= chaos;
			};
		};
	};	// namespace mut

	namespace crypt
	{
		struct XorValue
		{
			unsigned short	key;
			unsigned short	bsize;
			unsigned char	data[];
		};

		class XorCryptor
		{
		public:
			static XorValue* Crypt(const void* data, size_t size, size_t elem_size);

			static size_t Decrypt(XorValue* data, size_t elem_size, void* buffer, size_t buffer_size);
		};
	};	// namespace crypt

	namespace containers
	{
		template <typename T>
		class lazy_ptr;
		template<typename T>
		class lazy_obj;

		template<typename _Ty>
		using skeetvec = std::vector<_Ty, mem::SkeetAllocator<_Ty>>;

		struct RBNodeBase
		{
			RBNodeBase* left;
			RBNodeBase* right;
			RBNodeBase* parent;
			bool color;
		};

		struct PairValue
		{
			skeetvec<uint8_t> data;
			char pad[0x4];
			uint8_t key;
		};

		struct NodePair
		{
			skeetvec<short> key;
			char pad[0x4];
			PairValue value;
		};

		struct RBMapNode
		{
			RBNodeBase base;
			NodePair pair;
		};

		struct RBTree
		{
			RBMapNode* leftmost;	// 0x58
			RBMapNode* root;		// 0x5C
			RBMapNode* rightmost;	// 0x60
			RBMapNode* Base;		// 0x64
			char		pad[0x4];
			size_t		count;		// 0x6C
		};

		template<typename KeyType, typename ValueType>
		struct UnorderedNode
		{
			KeyType key;
			ValueType value;
			UnorderedNode* next;
		};

		template<typename KeyType, typename ValueType>
		struct UnorderedSet
		{
			char pad[4];
			UnorderedNode<KeyType, ValueType>** buckets;
			uint32_t count;
		};

		enum LinkedType : uint16_t
		{
			LVOID = 0,
			LBOOL = 1,
			LINTEGER = 3,
			LCOLOR = 4,
			LARRAY = 6,
			LHOTKEY = 7,
			LPOSSIZE = 11,
			LVECTOR = 12,
		};

		struct LinkedNodeBase
		{
			LinkedNodeBase* next;		// 0x0
			LinkedNodeBase* prev;		// 0x4
			uint32_t		hash;		// 0x8
			uint16_t		data_size;	// 0xC
			LinkedType		data_type;	// 0xE
		};

		SKEETSDK_ASSERT_SIZE(LinkedNodeBase, 0x10);

		template<typename T>
		struct LinkedNode : LinkedNodeBase
		{
			T data;

			template<typename ...Args>
			LinkedNode(uint32_t hash, LinkedType type, Args&& ...args) : data(std::forward<Args>(args)...)
			{
				this->hash = hash;
				this->data_size = sizeof(T);
				this->data_type = type;
			};
		};

		// Dont use it as actuall container, used to mimic skeet config system
		template<template<typename> typename _Allocator = mem::SkeetAllocator>
		class SKEETSDK_EBCO LinkedList
		{
			LinkedNodeBase* Head;
			LinkedNodeBase* Tail;
		public:
			template<typename T, typename ...Args>
			LinkedNode<T>* emplace_back(uint32_t hash, LinkedType type, Args&&... args)
			{
				using Node = LinkedNode<T>;

				Node* node = _Allocator<Node>::allocate(1);
				std::construct_at(node, hash, type, std::forward<Args>(args)...);
				LinkedNodeBase* prev = Head ? Tail : Head;
				node->next = (LinkedNodeBase*)&Head;
				node->prev = prev;

				prev->next = node;
				Tail = node;
				return node;
			};
			template<typename T>
			LinkedNode<T>* insert_back(LinkedNode<T>* node)
			{
				LinkedNodeBase* prev = Head ? Tail : Head;
				node->next = (LinkedNodeBase*)&Head;
				node->prev = prev;

				prev->next = node;
				Tail = node;
				return node;
			};
		};

	};	// namespace containers

	namespace render
	{
		class FontAsset
		{
		public:
			struct GlyphInfo
			{
				float x0, y0;
				float x1, y1;
				float u0, v0;
				float u1, v1;
				LONG abcA;
				LONG abcC;
			};

			struct AtlasInfo
			{
				misc::Vec2<uint32_t> Size;
				void* Data;
				size_t DataSize;
			};

		private:
			virtual ~FontAsset() = 0; // deleted
		public:
			virtual bool BuildGlyphToAtlas(int symbol) = 0;
		private:
			virtual int fn2(uint16_t* a2) = 0;
		public:
			virtual int SetAtlasDirty() = 0;
			virtual bool IsAtlasDirty() = 0;
			virtual bool FindGlyph(uint16_t ch, GlyphInfo& out) = 0;
			virtual bool HasGlyph(uint16_t ch) = 0;
			virtual LONG GetHeight() = 0;
			virtual LONG GetInternalLeading() = 0;
			virtual LONG GetExternalLeading() = 0;
			virtual LONG GetAscent() = 0;
			virtual void GetAtlasInfo(AtlasInfo* out) = 0;
			virtual HANDLE& GetSRWLock() = 0;

		public:
			struct Mextrix
			{
				LONG Ascent;
				LONG ExternalLeading;
				LONG InternalLeading;
				LONG MaxCharWidth;
				LONG Height;
			};

			struct Atlas
			{
				misc::Vec2<uint32_t> Pen;
				misc::Vec2<uint32_t> Size;
				int IsDirty;
				containers::skeetvec<uint32_t> Data;
			};

			struct Glyph
			{
				float x0, y0;
				float x1, y1;
				LONG abcA;
				LONG abcC;
			};

			LONG Height;
			DWORD Flags;
			uint16_t Name[32];
			LONG MetrixHeight;
			Mextrix FontMetrix;
			HDC hDC;
			HFONT hFont;
			HBITMAP hDIB;
			void* pDIBBits;
			Atlas FontAtlas;
			char pad3[4];
			containers::UnorderedSet<wchar_t, Glyph> GlyphList;
			char pad4[20];
			HANDLE SWRLOCK;
			uint32_t bitmap[2048];
		};

		SKEETSDK_ASSERT_SIZE(FontAsset, 0x20BC);
		SKEETSDK_ASSERT_SIZE(FontAsset::GlyphInfo, 0x28);
		SKEETSDK_ASSERT_SIZE(FontAsset::AtlasInfo, 0x10);
		SKEETSDK_ASSERT_SIZE(FontAsset::Mextrix, 0x14);
		SKEETSDK_ASSERT_SIZE(FontAsset::Atlas, 0x20);
		SKEETSDK_ASSERT_SIZE(FontAsset::Glyph, 0x18);

		struct Vertex
		{
			float x, y, z;
			uint32_t color;
			float u, v;
		};

		class IRenderer
		{
		public:
			virtual ~IRenderer() = 0;
			virtual bool Begin() = 0;
			virtual void End() = 0;
		private:
			virtual void _guard_check_icall_nop3() = 0; // CFG ret 0 function
		public:
			virtual void Rect(int x, int y, int w, int h, int color) = 0;
			virtual void Gradient4(int x, int y, int w, int h, int ctop_left, int ctop_right, int cbot_right, int cbot_left) = 0;
			virtual void GradientV(int x, int y, int w, int h, int color1, int color2) = 0;
			virtual void GradientH(int x, int y, int w, int h, int color1, int color2) = 0;
			virtual void Line(int x1, int y1, int x2, int y2, int color) = 0;
			virtual void __fastcall LineStrip(Vertex* vertecies, UINT count, int col) = 0;
			virtual void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) = 0;
			virtual void __vectorcall Circle(int x, int y, int color, float rad, float rot, float percent) = 0;
			virtual void __vectorcall CircleOut(int x, int y, int color, float rad, float rot, float percent, float thick) = 0;
			virtual void __fastcall TriagnleList(Vertex* vertecies, UINT count) = 0;
			virtual void __fastcall TriagnleStrip(Vertex* vertecies, UINT count) = 0;
		public:
			virtual void __fastcall Text(int flags, int x, int y, int color, size_t maxlen, std::wstring_view text) = 0;
			virtual int* __fastcall MeasureText_i8(int* out, std::string_view text, uint32_t flags) = 0;
			virtual int* __fastcall MeasureText(int* out, std::wstring_view text, uint32_t flags) = 0;
			virtual void __fastcall ScreenSize(int* w, int* h) = 0;
			virtual int __fastcall AddTexture(unsigned char* data, int w, int h, size_t bytesize, int i0, uint32_t hash) = 0;
			virtual void __fastcall RebindTexture(int idx, int w, int h, unsigned char* data, size_t bytesize) = 0;
			virtual int __fastcall GetTextureIdByHash(uint32_t hash) = 0;
		private:
			virtual void _guard_check_icall_nop22() = 0; // CFG ret 0 function;
		public:
			virtual void __vectorcall Texture(int id, int x, int y, int color, int offset, int i0, int w, int h, int flag, float scale) = 0;
			virtual void __fastcall SetScissorsRect(RECT rect) = 0;
			virtual void __fastcall GetScissorsRect(RECT rect) = 0;
			virtual void __fastcall SetScissorsState(bool state) = 0;
			virtual bool GetScissorsState() = 0;
			virtual void __vectorcall SetAlpha(float alpha) = 0;
			virtual void __vectorcall SetZ(float z) = 0; // [0.0, 1.0] 0.0 = topmost, 1.0 = doesnt render
			virtual float GetZ() = 0; // [0.0, 1.0] 0.0 = topmost, 1.0 = doesnt render
			virtual void SetMonoGrey(bool state) = 0; // grey if enemy alive?
			virtual void __vectorcall SetScale(float scale) = 0;
			virtual float __vectorcall GetScale() = 0; // returns render scale aka dpi
		private:
			virtual int __fastcall fn34(int i) = 0; // returns render padding?
			virtual void fn35() = 0;
			virtual void fn36() = 0;
			virtual void fn37() = 0;
		public:
			/*
				custom ids for type
				D3DPT_POINTLIST = 0
				D3DPT_LINELIST  = 1
				D3DPT_TRIANGLELIST = 2
				D3DPT_TRIANGLESTRIP = 3 | 6
				D3DPT_LINESTRIP = 4
				max = 7
			*/
			virtual void __fastcall Vertecies(uint32_t type, Vertex* vertecies, UINT count) = 0;
			virtual float __vectorcall GetAlpha() = 0;
			virtual void __fastcall GetTextureSize(int idx, int* w, int* h) = 0;
			virtual void __fastcall RectOut(int thick, int x, int y, int w, int h, int color) = 0;
			virtual void __fastcall RectOutlined(int thick, int x, int y, int w, int h, int colorin, int colorout) = 0;
		};

		struct DXCore
		{
			struct FontInfo
			{
				containers::skeetvec<wchar_t>	Name;		// 0x0
				char							pad1[0x4];
				uint32_t						Width;		// 0x10
				uint32_t						Flags;		// 0x14
			};

			struct FontMesh
			{
				FontAsset* Asset;
				void* Texture;
				FontMesh* Next;
			};

			struct TextureMesh
			{
				uint32_t Hash;
				misc::Vec2<int> Size;
				float Scale;
				containers::skeetvec<uint32_t> Data;
				char pad[4];
				uint32_t Key;
				void* Texture;
				int Valid;
			};

			FontInfo							FontsInfos[7];						// 0x00
			HANDLE								SRWLock;							// 0xA8
			bool								StripTogether;						// 0xAC
			bool								SomeBool;							// 0xAD setted with monogray vfunc but not used
			bool								BufferOverflow;						// 0xAE
			char								pad1[1];
			UINT								MaxVertexCount;						// 0xB0
			UINT								VertexCount;						// 0xB4
			UINT								VertexCursor;						// 0xB8
			uint32_t							PrimitiveType;						// 0xBC
			float								RenderAlpha;						// 0xC0
			void*								LastTexture;						// 0xC4
			void*								Device;								// 0xC8
			void*								Texture;							// 0xCC
			containers::skeetvec<TextureMesh>	Textures;				// 0xD0
			char								pad2[4];
			bool								Bussy;								// 0xE0
			char								pad3[3];
			void*								VertexBuff;							// 0xE4
			Vertex*								LockedVertices;						// 0xE8
			float								RenderZ;							// 0xEC
			int									ScreenHeight;						// 0xF0
			int									ScreenWidth;						// 0xF4
			DWORD								ThreadId;							// 0xF8
			int									D3DFVF;								// 0xFC
			float								RenderScaleNew;						// 0x100
			float								RenderScale;						// 0x104
			FontMesh							DefaultFonts[7];					// 0x108
			FontMesh							DPIFonts[7];						// 0x15C
		};

		SKEETSDK_ASSERT_SIZE(DXCore::FontInfo, 0x18);
		SKEETSDK_ASSERT_SIZE(DXCore::FontMesh, 0xC);
		SKEETSDK_ASSERT_SIZE(DXCore::TextureMesh, 0x2C);

		class DXRenderer : public IRenderer
		{
		public:
			DXCore* Core;
		// FIXME: idk mb add virtuals realization? or just find a way to remove this ugly shit
		public:
			virtual ~DXRenderer() {};
			virtual bool Begin() override {};
			virtual void End() override {};
		private:
			virtual void _guard_check_icall_nop3() override {};
		public:
			virtual void Rect(int x, int y, int w, int h, int color) override {};
			virtual void Gradient4(int x, int y, int w, int h, int ctop_left, int ctop_right, int cbot_right, int cbot_left) override {};
			virtual void GradientV(int x, int y, int w, int h, int color1, int color2) override {};
			virtual void GradientH(int x, int y, int w, int h, int color1, int color2) override {};
			virtual void Line(int x1, int y1, int x2, int y2, int color) override {};
			virtual void __fastcall LineStrip(Vertex* vertecies, UINT count, int col) override {};
			virtual void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) override {};
			virtual void __vectorcall Circle(int x, int y, int color, float rad, float rot, float percent) override {};
			virtual void __vectorcall CircleOut(int x, int y, int color, float rad, float rot, float percent, float thick) override {};
			virtual void __fastcall TriagnleList(Vertex* vertecies, UINT count) override {};
			virtual void __fastcall TriagnleStrip(Vertex* vertecies, UINT count) override {};
		public:
			virtual void __fastcall Text(int flags, int x, int y, int color, size_t maxlen, std::wstring_view text) override {};
			virtual int* __fastcall MeasureText_i8(int* out, std::string_view text, uint32_t flags) override { return 0; };
			virtual int* __fastcall MeasureText(int* out, std::wstring_view text, uint32_t flags) override { return 0; };
			virtual void __fastcall ScreenSize(int* w, int* h) override {};
			virtual int __fastcall AddTexture(unsigned char* data, int w, int h, size_t bytesize, int i0, uint32_t hash) override { return 0; };
			virtual void __fastcall RebindTexture(int idx, int w, int h, unsigned char* data, size_t bytesize) override {};
			virtual int __fastcall GetTextureIdByHash(uint32_t hash) override {};
		private:
			virtual void _guard_check_icall_nop22() override {};
		public:
			virtual void __vectorcall Texture(int id, int x, int y, int color, int offset, int i0, int w, int h, int flag, float scale) override {};
			virtual void __fastcall SetScissorsRect(RECT rect) override {};
			virtual void __fastcall GetScissorsRect(RECT rect) override {};
			virtual void __fastcall SetScissorsState(bool state) override {};
			virtual bool GetScissorsState() override { return 0; };
			virtual void __vectorcall SetAlpha(float alpha) override {};
			virtual void __vectorcall SetZ(float z) override {};
			virtual float GetZ() override { return 0.f; };
			virtual void SetMonoGrey(bool state) override {};
			virtual void __vectorcall SetScale(float scale) override {};
			virtual float __vectorcall GetScale() override { 0.f; };
		private:
			virtual int __fastcall fn34(int i) override { return 0; };
			virtual void fn35() override {};
			virtual void fn36() override {};
			virtual void fn37() override {};
		public:
			virtual void __fastcall Vertecies(uint32_t type, Vertex* vertecies, UINT count) override {};
			virtual float __vectorcall GetAlpha() override { return 0.f; };
			virtual void __fastcall GetTextureSize(int idx, int* w, int* h) override {};
			virtual void __fastcall RectOut(int thick, int x, int y, int w, int h, int color) override {};
			virtual void __fastcall RectOutlined(int thick, int x, int y, int w, int h, int colorin, int colorout) override {};
		};

		class RendererQueue : public IRenderer
		{
		public:
			struct RenderQueue
			{
				int Flag;
				containers::skeetvec<void*> Forms;
				char pad[4];
			};

			DXRenderer						DirectRenderer;		// 0x4
			RenderQueue						Queues[4];			// 0xC
			std::shared_ptr<RenderQueue>	LastQueueShared;	// 0x5C
			RenderQueue*					CurrentQueue;		// 0x64
			bool							MonoGray;			// 0x68
			bool							ScissorsState;		// 0x69
			char							pad4[0x2];
			RECT							ScissorsRect;		// 0x6C
			float							Z;					// 0x7C
			float							Alpha;				// 0x80
		};

		SKEETSDK_ASSERT_SIZE(RendererQueue, 0x84);

		class InGameRenderEvent
		{
		public:
			virtual void __fastcall Event(RendererQueue& renderer, void* lplayer, void* someptr) = 0;
		};

		class RendererHandler
		{
			SKEETSDK_HANDLE_INTERFACE(RendererHandler);

			RendererQueue* _Renderer = nullptr;
			containers::skeetvec<InGameRenderEvent*>* _IngameEvents = nullptr;
		public:
			RendererQueue* operator->() { return _Renderer; };
			RendererQueue& operator*() { return *_Renderer; };

			inline void AddEvent(InGameRenderEvent& event) { _IngameEvents->emplace_back(&event); };
		};

	};	// namespace render

	namespace ui
	{
		enum UiType : uint8_t
		{
			UiNone = 0,
			UiTab,
			UiButton,
			UiCheckbox,
			UiCombobox,
			UiSlider,
			UiListbox,
			UiChild,
			UiLabel,
			UiHotkey,
			UiTextbox,
			UiColorPicker = 12,
			UiMultiselect,
		};

		class UiRoot { virtual ~UiRoot() = 0; };

		template <typename T>
		class UiBase : public UiRoot
		{
		public:
			virtual ~UiBase() = 0;
			virtual bool IsHovered(const misc::Vec2<int>& cursorpos) = 0;
			virtual bool HandleMouse(const misc::MsgInfo& msg, const misc::Vec2<int>& cursorpos) = 0;
			virtual bool HandleInput(const misc::MsgInfo& msg) = 0;
			virtual void Render(render::RendererQueue& renderer, const misc::Vec2<int>& base) = 0;
			virtual void OnConfigLoad() = 0;
			virtual void ResetInputState() = 0;
			virtual void Callback() = 0;
			virtual void ResetInputStateIfShould() = 0;
			virtual void HandleResize() = 0;
			virtual void HandleDPI() = 0;
		private:
			virtual void fn11() = 0;
			virtual void fn12() = 0;
		
		public:
			struct UiFlags
			{
				bool Editable;
				bool Visible;
				bool Hovered;
				bool AllowMouseInput;
				bool SameLine;
			};

			char								pad1[0x8];
			int									Index;		// 0xC	in CTab - tab index, in Childs - first element index, in Elements - element index
			UiBase<void>*						Parent;		// 0x10
			UiFlags								Flags;		// 0x14
			UiType								Type;		// 0x19
			char								pad2[0x2];
			uint32_t							TextFlags;	// 0x1C name text flags
			misc::Vec2<int>						Pos;		// 0x20
			misc::Vec2<int>						Size;		// 0x28
			misc::Vec2<int>						DefaultSize;// 0x30
			std::shared_ptr<crypt::XorValue>	Name;		// 0x38
			containers::skeetvec<misc::Call>	Calls;		// 0x40
			char								pad3[0x4];
			misc::RGBA							Color;		// 0x50
			misc::Vec2<int>						Paddign;	// 0x54
			T*									Value;		// 0x5C

		public:
			template<typename T>
			T& as()
			{
				return *reinterpret_cast<T*>(this);
			};

			void set_callback(misc::Call callback)
			{
				Calls.push_back(std::move(callback));
			};

			void set_visible(bool visible)
			{
				if (this->Flags.Visible != visible)
				{
					this->Flags.Visible = visible;
					Parent = this->Parent;
					if (Parent)
						Parent->ResetInputStateIfShould();
				};
			};
		};

		SKEETSDK_ASSERT_SIZE(UiBase<void>, 0x60);

		struct LuaLabelValue
		{
			int								OnStartup;
			containers::skeetvec<wchar_t>	Name;
		};

		SKEETSDK_ASSERT_SIZE(LuaLabelValue, 0x10);

		class Label : public UiBase<LuaLabelValue>
		{
		};

		SKEETSDK_ASSERT_SIZE(Label, 0x60);
		
		class Button : public UiBase<void>
		{
		public:
			int						HoldingOn;			// 0x60
		};

		SKEETSDK_ASSERT_SIZE(Button, 0x64);

		class Textbox : public UiBase<void>
		{
		public:
			char					pad4[0x4];
			int						Length;				// 0x64
			wchar_t					Text[0x40];			// 0x68
		};

		SKEETSDK_ASSERT_SIZE(Textbox, 0xE8);

		struct ListboxVar
		{
			static constexpr uint32_t ACTIVE_FLAG = 1u << 31;
		public:
			uint32_t						Index;
			containers::skeetvec<wchar_t>	Name;
			char							pad1[0x4];
		public:
			ListboxVar(const std::wstring_view& wstr, uint32_t idx) : Name(wstr.begin(), wstr.end()), Index(idx) {};
			void set_active()
			{
				Index |= ACTIVE_FLAG;
			};
		};

		SKEETSDK_ASSERT_SIZE(ListboxVar, 0x14);

		class Listbox : public UiBase<ListboxVar>
		{
		public:
			struct ListboxInfo
			{
				uint32_t							ScrollPosition;	// 0x84
				int									SelectedItem;	// 0x88
				char								pad2[0x4];
				containers::skeetvec<ListboxVar>	Items;			// 0x90
				char								pad3[0x4];
			};

			int									ElementSize;		// 0x60
			ListboxInfo*						PInfo;				// 0x64
			bool								ScrollbarEnabled;	// 0x68
			bool								ScrollbarActive;	// 0x69
			char								pad5[0x2];
			misc::Vec2<int>						ScrollbarPos;		// 0x6C
			uint32_t							VarsCount;			// 0x74
			char								pad6[0x4];
			bool								SearchPresent;		// 0x7C
			bool								SearchActive;		// 0x7D
			char								pad7[0x2];
			int									DisplayedCount;		// 0x80
			ListboxInfo							Info;				// 0x84
			containers::skeetvec<wchar_t>		Search;				// 0xA0
			char								pad8[0x4];
			containers::skeetvec<short>			SSpecs;				// 0xB0
			char								pad9[0x4];

		public:

		};

		SKEETSDK_ASSERT_SIZE(Listbox, 0xC0);
		SKEETSDK_ASSERT_SIZE(Listbox::ListboxInfo, 0x1C);

		typedef struct Checkbox : public UiBase<bool>
		{
			misc::Vec2<int>			Size;			// 0x60
			int						TextPadding;	// 0x68
			bool					HoldingOn;		// 0x6C
			char					pad5[0x3];
			UiBase*					TiedElements;	// 0x70 tied elements which visibility depends on checkbox value (points to the end of list)
			void*					SomePtr1;		// 0x74 refers on TiedElements?
		};

		SKEETSDK_ASSERT_SIZE(Checkbox, 0x78);

		class Popup : UiBase<void>
		{
		public:
			using PopupCallbackFn = void(__fastcall*)(UiBase* parent, Popup* this_ptr, uint32_t value);

			int										ElementHeight;	// 0x60
			bool									Opened;			// 0x64
			bool									Clicked;		// 0x65
			char									pad3[0x2];
			int										HoveredItem;	// 0x68 can be -1
			int										SelectedItem;	// 0x6C
			PopupCallbackFn							CallbackFunc;	// 0x70
			containers::skeetvec<std::wstring_view>	Variants;		// 0x74
			char									pad4[0x4];

		public:
			virtual ~Popup() {};
			virtual bool IsHovered(const misc::Vec2<int>& cursorpos) {};
			virtual bool HandleMouse(const misc::MsgInfo& msg, const misc::Vec2<int>& cursorpos) {};
			virtual bool HandleInput(const misc::MsgInfo& msg) {};
			virtual void Render(render::RendererQueue& renderer, const misc::Vec2<int>& base) {};
			virtual void OnConfigLoad() {};
			virtual void ResetInputState() {};
			virtual void Callback() {};
			virtual void ResetInputStateIfShould() {};
			virtual void HandleResize() {};
			virtual void HandleDPI() {};
		private:
			virtual void fn11() {};
			virtual void fn12() {};
		};

		SKEETSDK_ASSERT_SIZE(Popup, 0x84);

		struct HotkeyInfo
		{
			uint32_t			ToogleState : 2;
			uint32_t			Key : 30;
			uint32_t			Mode;

			constexpr HotkeyInfo(uint32_t vkey, uint32_t mode) : Key(vkey), Mode(mode), ToogleState(0) {};
		};

		class Hotkey : public UiBase<containers::LinkedNode<HotkeyInfo>>
		{
		public:

			enum HotkeyMode
			{
				Inherit = -1,
				AlwaysOn,
				OnHotkey,
				Toggle,
				OffHotkey
			};

			bool					SettingKey;				// 0x60
			char					pad3;
			wchar_t					KeyText[0x5];			// 0x62
			Popup*					UiPopup;				// 0x6C
		};

		SKEETSDK_ASSERT_SIZE(Hotkey, 0x70);

		class ColorPicker : public UiBase<misc::RGBA>
		{
		public:

			enum PickerStatus
			{
				PickerIdle,
				PickerClicked,
				PickerTint,
				PickerHue,
				PickerAlpha
			};

			struct ColorPopup
			{
				int				XAxis;			// 0x64
				misc::Vec2<int>	Size;			// 0x68
				misc::Vec2<int>	TintPos;		// 0x70
				misc::Vec2<int>	TintSize;		// 0x78
				misc::Vec2<int>	HuePos;			// 0x80
				misc::Vec2<int>	HueSize;		// 0x88
				misc::Vec2<int>	AlphaPos;		// 0x90
				misc::Vec2<int>	AlphaSize;		// 0x98
			};

			typedef struct HSV
			{
				float	Hue;
				float	Saturation;
				float	Value;
			};

			char					pad4[0x4];
			ColorPopup				Pallete;		// 0x64
			char					pad5[0x4];
			Popup					UiPopup;		// 0xA0
			PickerStatus			Status;			// 0x128
			HSV						ValueHSV;		// 0x12C
		};

		SKEETSDK_ASSERT_SIZE(ColorPicker, 0x138);

		template <typename T>
		struct BoxVars
		{
			containers::skeetvec<T>	Name;
			char					pad1[0x4];
			int						BitMask;
		};

		class Multiselect : public UiBase<uint32_t>
		{
		public:
			virtual void FormatStringValue() = 0;
		public:
			int									Height;				// 0x60
			int									YOffset;			// 0x64 mouse interaction offset
			bool								Popup;				// 0x68
			bool								Clicked;			// 0x69
			bool								NotNull;			// 0x6A
			char								pad3;
			int									HoveredItem;		// 0x6C
			containers::skeetvec<char>			StringValue;		// 0x70
			char								pad4[0x4];
			containers::skeetvec<BoxVars<char>>	Vars;				// 0x80
			char								pad5[0x4];
			BoxVars<char>						Varsi[0x20];		// 0x90
			char								pad6[0x1C];
		};

		SKEETSDK_ASSERT_SIZE(Multiselect, 0x32C);

		class Combobox : public UiBase<uint32_t>
		{
		public:
			int										DefHeight;			// 0x60
			int										Height;				// 0x64
			int										DefInteractOffset;	// 0x68
			int										InteractOffset;		// 0x6C
			bool									Open;				// 0x70
			bool									Clicked;			// 0x71
			char									pad3[0x2];
			int										HoveredItem;		// 0x74
			int										SelectedItem;		// 0x78
			containers::skeetvec<BoxVars<wchar_t>>	Vars;				// 0x7C
			char									pad4[0x20];
		};

		SKEETSDK_ASSERT_SIZE(Combobox, 0xA8);

#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
		class FlatWeaponSelector : public Combobox
		{
		public:
			int IconsGap;
			int IconsWidth;
		};

		SKEETSDK_ASSERT_SIZE(FlatWeaponSelector, 0xB0);

		class CaseWeaponSelector : public Combobox
		{
		public:
			char pad6[0x4];
			CaseWeaponSelector* pthis;
			misc::Vec2<int> somevec;
			int someint;
			Popup UiPopup;
		};

		SKEETSDK_ASSERT_SIZE(CaseWeaponSelector, 0x140);
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS

		class Slider : public UiBase<int>
		{
		public:
			struct Tooltip
			{
				int value;
				containers::skeetvec<wchar_t> tooltip;
			};

			struct TooltipString
			{
				wchar_t data[2];

				constexpr TooltipString(const wchar_t* str) { data[0] = *str++; data[1] = *str; };
			};

			misc::Vec2<int>			DefSize;			// 0x60
			bool					Active;				// 0x68
			bool					HasTooltip;			// 0x69
			char					pad3[0x6];
			Tooltip*				Tooltips;			// 0x70
			uint32_t				TooltipMod;			// 0x74 idk, if no tooltips provided it is 1 else 2
			size_t					TooltipsCount;		// 0x78
			float					SomeFloats[0x2];	// 0x7C
			int						SomeInt;			// 0x84
			char					pad4[0x4];
			misc::RGBA				SomeColor;			// 0x8C
			int						Max;				// 0x90
			int						Min;				// 0x94
			float					Step;				// 0x98
			int						MaxLen;				// 0x9C
			wchar_t					TooltipType[0x2];	// 0xA0
			wchar_t					TooltipStr[0x40];	// 0xA4
		};

		SKEETSDK_ASSERT_SIZE(Slider, 0x124);

		class DamageSlider : public Slider
		{
			containers::skeetvec<wchar_t> PlusHPStr;
			int pad;
		};

		SKEETSDK_ASSERT_SIZE(DamageSlider, 0x134);

		class Child : public UiBase<void>
		{
		public:

			enum ChildStatus
			{
				ChildIdle,
				ChildDragged,
				ChildResized
			};

			struct Widget
			{
				char		pad1[0x4];
				HANDLE		SRWLock;	// 0xAC
				bool		Modifiable;	// 0xB0
				bool		Movable;	// 0xB1
				char		pad2[0x2];
				misc::RGBA	NameCol;	// 0xB4
				misc::RGBA	BorderCol;	// 0xB8
				misc::RGBA	ShadowCol;	// 0xBC
				misc::RGBA	BackCol;	// 0xC0
				int			DrawName;	// 0xC4
			};

			char							pad2[0x8];
			misc::Vec2<int>					Padding;			// 0x68
			misc::Vec4<uint8_t>				PosSize;			// 0x70 block = minimum size child can be resized/moved by, x = blocks moved by X, y = blocks sized by X, z = blocks moved by Y, w = blocks sized by Y
			misc::Vec2<int>					MouseLastPos;		// 0x74
			ChildStatus						Status;				// 0x7C
			containers::skeetvec<UiBase*>	Elements;			// 0x80
			char							pad3[0x4];
			Widget*							PWidgets;			// 0x90
			char							pad4[0x4];
			misc::Vec2<int>					CurSize;			// 0x98
			misc::Vec2<int>					MinimizedCapacity;	// 0xA0
			Widget							Widgets;			// 0xA8
		public:
			template<typename T>
				requires std::derived_from<T, UiRoot>
			SKEETSDK_FORCEINLINE T& element(uint32_t element_id) { return Elements[element_id]->as<T>(); };
		};

		SKEETSDK_ASSERT_SIZE(Child, 0xC8);

		class Tab : public UiBase<void>
		{
		public:
			virtual void OnOpen() = 0;
			virtual void ResetLayout() = 0;

			struct TabActivies
			{
				int LastActiveChild;		// 0x64
				int LastActiveElementIndex;	// 0x68
				int LastActiveElementId;	// 0x6C
			};

			struct TabIcon
			{
				char			pad4[0x4];
				int				TextureId;		// 0x80
				int				TextureOffset;	// 0x84
				char			pad5[0x4];
				misc::Vec2<int>	Size;			// 0x8C
			};

			class Menu*						PMenu;				// 0x60
			TabActivies						InteractionInfo;	// 0x64
			containers::skeetvec<Child*>	Childs;				// 0x70
			TabIcon							Icon;				// 0x7C
			int								Padding;			// 0x94
		public:
			SKEETSDK_FORCEINLINE Child& child(uint32_t child_id) { return *Childs[child_id]; };
		};

		SKEETSDK_ASSERT_SIZE(Tab, 0x98);

		class ConfigTab : public Tab
		{
		public:
			containers::skeetvec<uint8_t>	Chunk;		// 0x98
			UiBase<void>*					CEs[0x5];	// 0xA4	childs and some ui elements. Idk how much.
		};

		enum Tab_t : uint32_t
		{
			RAGE,
			AA,
			LEGIT,
			VISUALS,
			MISC,
			SKINS,
			PLIST,
			CONFIG,
			LUA
		};

		class Menu
		{
		public:
			virtual ~Menu() = 0;
			virtual void SetupTabIcons(render::RendererQueue& renderer) = 0;
			virtual void RenderForm(render::RendererQueue& renderer, void* rect_mb) = 0;

			struct MouseInfo
			{
				enum MouseFlags
				{
					MouseIdle,
					LClick,
					RClick
				};

				uint32_t		MouseStatus;
				misc::Vec2<int>	Pos;
				misc::Vec2<int>	LastMenuPos;
			};

			enum FadeType
			{
				FadeNone,
				FadeOut,
				FadeIn
			};

			enum MenuFlags
			{
				MenuClosed = 0,
				MenuOpened = 1 << 0,
				MenuDragged = 1 << 2,
				MenuScaled = 1 << 3
			};

			Menu*						NextMenu;			// 0x4
			char						pad1[0x4];			// hWnd
			misc::Vec2<int>				Pos;				// 0xC
			misc::Vec2<int>				Size;				// 0x14
			misc::Vec2<int>				MinSize;			// 0x1C
			misc::Vec2<int>				TabStartPadding;	// 0x24
			misc::Vec2<int>				TabEndPadding;		// 0x2C
			uint32_t					MenuStatus;			// 0x34
			MouseInfo					Mouse;				// 0x38
			char						pad2[0x8];
			containers::skeetvec<Tab*>	Tabs;				// 0x54
			char						pad3[0x4];
			uint32_t					CurrentTab;			// 0x64
			char						pad4[0x20];
			unsigned int				MenuFadeTickCount;	// 0x88
			FadeType					MenuFadeType;		// 0x8C
			char						pad5[0x4];
			void*						SomeVT;				// 0x94
			int							SomeInt;			// 0x98
			char						pad6[0x14];
			float						MenuAlpha;			// 0xB0
			Tab*						TabsArr[0x9];		// 0xB4
			int							IconsTexutreId;		// 0xD8
			int							BackgroundTextureId;// 0xDC
			wchar_t						RegValueName[0xC];	// 0xE0
			char						pad8[0x28];			// 0xF8
			uint32_t					OnStartupHash[128]; // 0x120
		public:
			SKEETSDK_FORCEINLINE Tab& tab(uint32_t tab_id) { return *Tabs[tab_id]; };
		};

		SKEETSDK_ASSERT_SIZE(Menu, 0x320);

		class MenuHandler final
		{
			SKEETSDK_HANDLE_INTERFACE(MenuHandler);
			Menu* _Menu = nullptr;
		public:
			SKEETSDK_FORCEINLINE Menu* operator->() { return _Menu; };
			SKEETSDK_FORCEINLINE Menu& operator*() { return *_Menu; };
			SKEETSDK_FORCEINLINE operator Menu*() { return _Menu; };
			SKEETSDK_FORCEINLINE operator Menu&() { return *_Menu; };

			SKEETSDK_FORCEINLINE Tab& tab(uint32_t tab_id) { return _Menu->tab(tab_id); };

			Tab* create_tab(const wchar_t* name);

			Child* create_child(Tab& tab, const wchar_t* name, misc::Vec4<uint8_t> layout, uint32_t elements_capacity, bool modifiable);
		
			// FIXME: Make resetinputifshould call dedicated, so we can create all ui elements and call it once at the end.
			Label* create_label(Child& child, const wchar_t* name);
			Button* create_button(Child& child, const wchar_t* name, Fast2PtrFn callback, void* callback_arg);
			Textbox* create_textbox(Child& child);
			Checkbox* create_checkbox(Child& child, const wchar_t* name, bool initial_value);
			Slider* create_slider(Child& child, const wchar_t* name, int min, int max, int initial_value, bool tooltip, Slider::TooltipString tip, float step);
			ColorPicker* create_colorpicker(Child& child, const wchar_t* name, misc::RGBA initial_value);
			Hotkey* create_hotkey(Child& child, const wchar_t* name, uint32_t virtual_key, Hotkey::HotkeyMode mode, bool has_popup);
			Multiselect* create_multiselect(Child& child, const wchar_t* name, std::span<std::string_view> variables, uint32_t initial_value, bool always_valuable);
			Listbox* create_listbox(Child& child, const wchar_t* name, misc::Vec2<int> size, std::span<std::wstring_view> variables, uint32_t initial_value, bool has_search);
			Combobox* create_combobox(Child& child, const wchar_t* name, std::span<std::wstring_view> variables, uint32_t initial_value, bool sameline);
#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			FlatWeaponSelector* create_flat_weaponselector(Child& child, const wchar_t* name, uint32_t initial_value);
			CaseWeaponSelector* create_case_weaponselector(Child& child, const wchar_t* name, uint32_t initial_value);
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS
		private:
			uint32_t generate_ui_hash(Child& child, const wchar_t* name);
		private:
			using TabConstructorFn = Tab*(__thiscall*)(Tab* this_ptr, Menu* parent, const wchar_t* name, misc::Vec2<int>* icon_pos);
			using ChildConstructorFn = Child* (__thiscall*)(Child* this_ptr, Tab* parent, const wchar_t* name, misc::Vec4<uint8_t> layout, uint32_t elements_count, bool modifiable);

			using AddLabelFn = Label*(__thiscall*)(Child* this_ptr, const wchar_t* name);
			using AddButtonFn = Button*(__thiscall*)(Child* this_ptr, const wchar_t* name, void* callback_arg, Fast2PtrFn callback);
			
			using CheckboxConstructorFn = Checkbox*(__thiscall*)(Checkbox* this_ptr, Child* parent, const wchar_t* name, bool* value);
			using TextboxConstructorFn = Textbox*(__thiscall*)(Textbox* this_ptr, Child* parent);
			using SliderConstructorFn = Slider*(__thiscall*)(Slider* this_ptr, Child* parent, const wchar_t* name, int min, int max, int* value, bool tooltip, Slider::TooltipString tooltipstr, float step);
			using ColorPickerConstructorFn = ColorPicker*(__thiscall*)(ColorPicker* this_ptr, Child* parent, misc::RGBA* value);
			using HotkeyConstructorFn = Hotkey*(__thiscall*)(Hotkey* this_ptr, Child* parent, containers::LinkedNode<HotkeyInfo>* value, bool has_popup);
			using MultiselectConstructorFn = Multiselect*(__thiscall*)(Multiselect* this_ptr, Child* parent, const wchar_t* name, uint32_t* value, int unused, bool always_valuable);
			using ListboxConstructorFn = Listbox*(__thiscall*)(Listbox* this_ptr, Child* parent, const wchar_t* name, misc::Vec2<int> size, uint32_t* value, bool has_search);
			using ComboboxConstructorFn = Combobox*(__thiscall*)(Combobox* this_ptr, Child* parent, const wchar_t* name, uint32_t* value, bool sameline);
			
#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			using FlatWeaponSelectorConstructorFn = FlatWeaponSelector*(__thiscall*)(FlatWeaponSelector* this_ptr, Child* parent, const wchar_t* name, uint32_t* value, int unused);
			using CaseWeaponSelectorConstructorFn = CaseWeaponSelector*(__thiscall*)(CaseWeaponSelector* this_ptr, Child* parent, const wchar_t* name, uint32_t* value);
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			TabConstructorFn _TabCon = nullptr;
			ThisPtrFn _InsertTab = nullptr;

			ChildConstructorFn _ChildCon = nullptr;

			containers::LinkedList<>* _UiConfigList = nullptr;
			containers::LinkedList<>* _UiHotkeyList = nullptr;

			AddLabelFn _AddLabel = nullptr;
			AddButtonFn _AddButton = nullptr;

			CheckboxConstructorFn _CheckboxCon = nullptr;
			TextboxConstructorFn _TextboxCon = nullptr;
			SliderConstructorFn _SliderCon = nullptr;
			ColorPickerConstructorFn _ColorPickerCon = nullptr;
			HotkeyConstructorFn _HotkeyCon = nullptr;
			MultiselectConstructorFn _MultiselectCon = nullptr;
			ListboxConstructorFn  _ListboxCon = nullptr;
			ComboboxConstructorFn  _ComboboxCon = nullptr;
#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			FlatWeaponSelectorConstructorFn  _FlatWeaponSelectorCon = nullptr;
			CaseWeaponSelectorConstructorFn  _CaseWeaponSelectorCon = nullptr;
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS
		};

	};	// namespace ui

	namespace cfg
	{
		struct ConfigHead
		{
			uint32_t	sig;	// 0xDEC00D60
			uint32_t	unkn0;	// 0x00000002
			uint32_t	xkey;
		};

		struct ConfigDataUnit
		{
			uint16_t				data_size;
			containers::LinkedType	data_type;
			uint32_t				hash;
			uint8_t					data[];

			template<typename T>
			T& get_ref()
			{
				return *reinterpret_cast<T*>(&data);
			};
		};

		class ConfigSystem final
		{
			SKEETSDK_GLOBAL_INTERFACE(ConfigSystem);
		SKEETSDK_PRIVATE_MEMBERS:
			using GetConfigDataFn = bool(__fastcall*)(const char* config_name, containers::skeetvec<uint8_t>& vec_out);
			using LoadConfigFn = void(__fastcall*)(ui::Menu* menu, void* config_data, size_t data_size);
			using AfterTabRageFn = void(__thiscall*)(ui::Tab* rage_tab);


			GetConfigDataFn	_GetConfigData;
			LoadConfigFn	_LoadConfigData;
			AfterTabRageFn	_FetchRageTab;
			mut::FNV1aBase* _CurrentConfigHash;
		public:

			_NODISCARD const wchar_t* CurrentConfigName();

			SKEETSDK_FLATTEN bool LoadConfig(mut::FNV1a hash);

			SKEETSDK_FORCEINLINE bool LoadConfig(const char* name);
		};
	};	// namespace cfg

	namespace lua
	{
		struct LuaInfo
		{
			uint64_t						TimeStamp;
			int								OnStartup;
			containers::skeetvec<wchar_t>	Name;
			char							pad2[0x8];
		};

		SKEETSDK_ASSERT_SIZE(LuaInfo, 0x20);

		struct CloudLuaInfo
		{
			uint64_t						TimeStamp;
			int								Loaded;
			containers::skeetvec<char>		Name;
			char							pad[0x4];
			containers::skeetvec<uint8_t>	Data;
		};

		SKEETSDK_ASSERT_SIZE(CloudLuaInfo, 0x28);

		struct SLua
		{
			void* LuaState;		// 0x44
			char	pad2[0x10];
			containers::RBTree	LoadedLuaTree;	// 0x58
		};

		typedef struct CLua
		{
			char	pad1[0x44];
			SLua	state;
		} *PCLua, ** PCLuas;

		class LuaSystem final
		{
			SKEETSDK_GLOBAL_INTERFACE(LuaSystem);
		SKEETSDK_PRIVATE_MEMBERS:
			using LoadLuaFn = bool(__fastcall*)(const wchar_t* name);
			using UpdateVisualLuaInfoFn = void(__thiscall*)(ui::Tab* config_tab);

			CFn						_GetOrInitLuaState;
			CFn						_GetSLua;
			LoadLuaFn				_LoadLua;
			BoolThisFn				_IsLuaLoaded;
			UpdateVisualLuaInfoFn	_UpdateVisualLuaInfo;
			PCLuas					_LuaLoader_State;
		public:
			_NODISCARD size_t LuaCount();

			void* GetOrInitLuaState();

			_NODISCARD void* GetLuaState();

			_NODISCARD SLua* GetSLua();

			_NODISCARD PCLua GetLuaLoaderState();

			_NODISCARD const wchar_t* LuaName(mut::FNV1a hash);

			bool LoadScript(const wchar_t* name, bool on_startup);

			bool LoadScript(mut::FNV1a hash, bool on_startup);
		};
	};	// namespce lua

	namespace containers
	{
		class lazy_base
		{
		protected:
			static constexpr LONG BIT_INITALIZING = 1;
			static constexpr LONG BIT_INITALIZED = 2;
			volatile LONG status = 0;
		};

		template <typename T>
		class lazy_ptr : protected lazy_base
		{
			friend class skeetsdk::GlobalContext;

			T* ptr = nullptr;
		protected:
			~lazy_ptr()
			{
				if (InterlockedOr(&status, 0) & (1 << BIT_INITALIZED)) delete ptr;
			};
		public:
			constexpr lazy_ptr() = default;

			T* touch(skeetsdk::GlobalContext& gctx)
			{
				if ((InterlockedOr(&status, 0) & (1 << BIT_INITALIZED)))
					return ptr;

				if (!_interlockedbittestandset(&status, BIT_INITALIZING))
				{
					ptr = new T(gctx);

					std::atomic_thread_fence(std::memory_order_release);

					_interlockedbittestandset(&status, BIT_INITALIZED);

					return ptr;
				};

				while (!(InterlockedOr(&status, 0) & (1 << BIT_INITALIZED)))
					_mm_pause();

				std::atomic_thread_fence(std::memory_order_acquire);
				return ptr;
			};
		};

		template<typename T>
		class lazy_obj : protected lazy_base
		{
			friend class skeetsdk::GlobalContext;
			alignas(T) std::byte data[sizeof(T)] = {};
			SKEETSDK_FORCEINLINE constexpr T* as_object() { return reinterpret_cast<T*>(data); };
		protected:
			~lazy_obj()
			{
				if (InterlockedOr(&status, 0) & (1 << BIT_INITALIZED)) as_object()->~T();
			};
		public:
			constexpr lazy_obj() = default;

			T& touch(skeetsdk::GlobalContext& gctx)
			{
				if ((InterlockedOr(&status, 0) & (1 << BIT_INITALIZED)))
					return *as_object();

				if (!_interlockedbittestandset(&status, BIT_INITALIZING))
				{
					new (as_object()) T(gctx);

					std::atomic_thread_fence(std::memory_order_release);

					_interlockedbittestandset(&status, BIT_INITALIZED);

					return *as_object();
				};

				while (!(InterlockedOr(&status, 0) & (1 << BIT_INITALIZED)))
					_mm_pause();

				std::atomic_thread_fence(std::memory_order_acquire);
				return *as_object();
			};
		};
	}; // namespace containers

	class GlobalContext final
	{
	public:
		mem::SigFinder sf_cheat_chunk;
		containers::lazy_ptr<cfg::ConfigSystem> cfg_system;
		containers::lazy_ptr<lua::LuaSystem> lua_system;
		containers::lazy_ptr<ui::MenuHandler> h_menu;
		containers::lazy_ptr<render::RendererHandler> h_renderer;
	public:
		constexpr GlobalContext()
			: sf_cheat_chunk((void*)mem::skeet_image_base, mem::skeet_image_size)
		{
		};

		_NODISCARD SKEETSDK_FORCEINLINE void* find(const std::vector<BYTE>& pattern, const BYTE wildcard) const { return sf_cheat_chunk.find(pattern, wildcard); };

		_NODISCARD SKEETSDK_FORCEINLINE void* find(const char* signature, int offset = 0, const char wildcard = '?', const BYTE bytecard = 0xCC) const { return sf_cheat_chunk.find(signature, offset, wildcard, bytecard); };

		_NODISCARD SKEETSDK_FORCEINLINE void* findr32(const char* signature, const char wildcard = '?', const BYTE bytecard = 0xCC) const { return sf_cheat_chunk.findr32(signature, wildcard, bytecard); };
	

		SKEETSDK_FORCEINLINE cfg::ConfigSystem& Configs() { return *cfg_system.touch(*this); };

		SKEETSDK_FORCEINLINE lua::LuaSystem& Luas() { return *lua_system.touch(*this); };

		SKEETSDK_FORCEINLINE ui::MenuHandler& Menu() { return *h_menu.touch(*this); };

		SKEETSDK_FORCEINLINE render::RendererHandler& Renderer() { return *h_renderer.touch(*this); };
	};

#ifndef SKEETSDK_ILL_MANAGE_GLOBAL_CONTEXT
	inline GlobalContext g_ctx;
#endif	// !SKEETSDK_ILL_MANAGE_GLOBAL_CONTEXT

#ifdef SKEETSDK_IMPLEMENTATION
	namespace mem
	{
		HMODULE WaitForSingleHandle(const char* module)
		{
			HMODULE hmod = GetModuleHandleA(module);
			while (!hmod)
			{
				hmod = GetModuleHandleA(module);
				Sleep(0);
			};
			return hmod;
		};

		bool IsExecutableAddress(PVOID Address)
		{
			MEMORY_BASIC_INFORMATION mbi{ 0 };
			if (!VirtualQuery(Address, &mbi, sizeof(mbi))) return false;

			if (mbi.State != MEM_COMMIT) return false;

			switch (mbi.Protect & 0xFF)
			{
			case PAGE_EXECUTE:
			case PAGE_EXECUTE_READ:
			case PAGE_EXECUTE_READWRITE:
			case PAGE_EXECUTE_WRITECOPY:
				return true;
			default:
				return false;
			};
		};

		std::vector<BYTE> SigFinder::signature_resolver(const char* signature, const char wildcard, const BYTE bytecard)
		{
			size_t wildcard_width = 0;
			size_t bytes_count = *signature == ' ' ? 0 : 1;
			size_t bufsize = strlen(signature) + 1;
			char* m_sig = (char*)alloca(bufsize);
			strcpy_s(m_sig, bufsize, signature);

			char* s = m_sig;
			while (!(*s == '\0' || *s == wildcard)) s++;

			if (*s == wildcard)
				while (*s++ == wildcard) wildcard_width++;

			s = m_sig;
			while (*s)
			{
				if (*s == ' ')
				{
					bytes_count++;
					const char* src = s;
					while (*src == ' ') src++;
					memcpy(s, src, strlen(src) + 1);
				};
				s++;
			}

			std::vector<uint8_t> vec;
			vec.reserve(bytes_count);

			size_t i = 0;
			while (i < strlen(m_sig))
			{
				if (wildcard_width)
				{
					if (m_sig[i] == wildcard)
					{
						vec.push_back(bytecard);
						i += wildcard_width;
						continue;
					};
				};

				if (m_sig[i] >= 65 && m_sig[i] <= 90)
				{
					vec.push_back((m_sig[i] - 55) << 4);
				}
				else
				{
					vec.push_back((m_sig[i] - '0') << 4);
				};
				if (m_sig[i + 1] >= 65 && m_sig[i + 1] <= 90)
				{
					vec.back() |= (m_sig[i + 1] - 55);
				}
				else
				{
					vec.back() |= (m_sig[i + 1] - '0');
				};
				i += 2;
			};

			return vec;
		};

		void SigFinder::generate_shift_table(BYTE out[256], const std::vector<BYTE>& pattern, const BYTE wildcard)
		{
			size_t pattern_len = pattern.size() - 1;
			size_t i = pattern_len;

			for (; i; i--)
				if (pattern[i] == wildcard) break;

			size_t max_shift = pattern_len - i;
			if (pattern_len == i)
				max_shift = 1;

			memset(out, max_shift, 256);

			for (size_t j = pattern_len - max_shift; j < pattern_len; j++)
				out[pattern[j]] = pattern_len - j;
		};

		void* SigFinder::find(const std::vector<BYTE>& pattern, const BYTE wildcard) const
		{
			BYTE shift[256];
			generate_shift_table(shift, pattern, wildcard);
			size_t pattern_len = pattern.size() - 1;
			PBYTE base = (PBYTE)lpBaseOfDll;
			PBYTE bound = (PBYTE)lpBaseOfDll + SizeOfImage - pattern.size();
			while (base <= bound)
			{
				size_t i = pattern_len;
				while (true)
				{
					if (pattern[i] != wildcard && base[i] != pattern[i])
					{
						base += shift[base[pattern_len]];
						break;
					};

					if (!i) return base;
					i--;
				}
			};
			return nullptr;
		};

		void* SigFinder::find(const char* signature, int offset, const char wildcard, const BYTE bytecard) const
		{
			std::vector<BYTE> pattern = signature_resolver(signature, wildcard, bytecard);
			char* finded = (char*)find(pattern, bytecard);
			return finded ? finded + offset : nullptr;
		};

		void* SigFinder::findr32(const char* signature, const char wildcard, const BYTE bytecard) const
		{
			std::vector<BYTE> pattern = signature_resolver(signature, wildcard, bytecard);
			if (pattern[0] != 0xE9 && pattern[0] != 0xE8) return nullptr;

			PBYTE instruction = (PBYTE)find(pattern, bytecard);
			if (!instruction) return nullptr;

			return instruction + *(int*)(instruction + 1) + 5;
		};

		void Detour::Hook(PVOID From, PVOID To, SIZE_T AllignCount, PVOID* TrampolineOut)
		{
			if (!IsExecutableAddress(From) || !IsExecutableAddress(To)) return;

			bool has_tram = TrampolineOut != nullptr;
			size_t hook_size = 5 + AllignCount;
			size_t cblock_size = sizeof(ControlBlock) + hook_size;
			if (has_tram)
				cblock_size += 5;

			ControlBlock* cblock = (ControlBlock*)malloc(cblock_size);
			if (!cblock) return;

			cblock->OriginalSize = hook_size;

			DWORD oldProto = 0;
			VirtualProtect(cblock, cblock_size, PAGE_EXECUTE_READWRITE, &oldProto);

			if (VirtualProtect(From, hook_size, PAGE_EXECUTE_READWRITE, &oldProto))
			{
				memcpy(cblock->Code, From, hook_size);
				*(PBYTE)From = 0xE9;
				*(PLONG_PTR)((PBYTE)From + 1) = (LONG_PTR)cblock->Transit - (LONG_PTR)From - 5;

				*cblock->Transit = 0xE9;
				*(PLONG_PTR)(cblock->Transit + 1) = (LONG_PTR)To - (LONG_PTR)cblock->Transit - 5;

				if (has_tram)
				{
					cblock->Code[hook_size] = 0xE9;
					*(LONG_PTR*)(cblock->Code + hook_size + 1) = (LONG_PTR)((PBYTE)From + hook_size) - (LONG_PTR)(cblock->Code + hook_size) - 5;
					*TrampolineOut = cblock->Code;
				};

				VirtualProtect(From, hook_size, oldProto, &oldProto);
				FlushInstructionCache(GetCurrentProcess(), From, hook_size);
			};
		};

		void Detour::UnHook(PVOID From)
		{
			if (!IsExecutableAddress(From)) return;
			
			PBYTE bytes = (PBYTE)From;
			if (*bytes != 0xE9) return;

			ControlBlock* cblock = (ControlBlock*)((LONG_PTR)bytes + *(PLONG_PTR)(bytes + 1) + 5);
			if (!cblock) return;

			DWORD oldProto = 0;

			if (VirtualProtect(From, cblock->OriginalSize, PAGE_EXECUTE_READWRITE, &oldProto))
			{
				memcpy(From, cblock->Code, cblock->OriginalSize);

				VirtualProtect(From, cblock->OriginalSize, oldProto, &oldProto);
				FlushInstructionCache(GetCurrentProcess(), From, cblock->OriginalSize);
				free(cblock);
			};
		};
	};	// namespace mem

	namespace crypt
	{
		XorValue* XorCryptor::Crypt(const void* data, size_t size, size_t elem_size)
		{
			XorValue* xored = nullptr;//reinterpret_cast<XorValue*>(g_ctx.allocate(sizeof(XorValue) + size + elem_size));
			if (xored)
			{
				xored->bsize = size;
				xored->key = __rdtsc();
				if (size)
				{
					memcpy(xored->data, data, size);
					uint16_t key = xored->key;
					uint16_t len = 0;
					if (xored->bsize)
					{
						do
						{
							auto i = len;
							len += elem_size;
							*(xored->data + i) ^= key;
							key = _rotr16(key, 1);
						} while (len < xored->bsize);
					};
				};
				memset(xored->data + size, 0, elem_size);
			};
			return xored;
		};

		size_t XorCryptor::Decrypt(XorValue* data, size_t elem_size, void* buffer, size_t buffer_size)
		{
			uint16_t done = 0;
			size_t step = elem_size;
			if (buffer)
			{
				if (buffer_size)
				{
					memset(buffer, 0, buffer_size);
					if (data)
					{
						uint16_t bsize = data->bsize;
						if (buffer_size >= bsize + elem_size)
						{
							memcpy(buffer, data->data, bsize + elem_size);
							uint16_t key = data->key;
							if (bsize)
							{
								do
								{
									auto cbyte_pos = done;
									done += step;
									static_cast<uint8_t*>(buffer)[cbyte_pos] ^= key;
									key = _rotr16(key, 1);
								} while (done < bsize);
							}
							return bsize / step;
						}
					}
				}
			}
			return done;
		};
	};	// namespace crypt

	namespace cfg
	{
		ConfigSystem::ConfigSystem(skeetsdk::GlobalContext& gctx) : gctx(gctx)
		{
#ifndef SKEETSDK_TARGET_CRACK
			void* config_hash_base = gctx.find(mem::signatures::current_config_hash_signature, 0x00);
			if (config_hash_base)
			{
				_CurrentConfigHash = *(decltype(&_CurrentConfigHash))((char*)config_hash_base + 0x1);
				SKEETSDK_LOG("Found current config hash at %p.\n", _CurrentConfigHash);
			}
			else
				SKEETSDK_ERROR("Failed to find current config hash with: Invalid signature.\n");

			SKEETSDK_DECLASSIGN(_GetConfigData, gctx.find(mem::signatures::get_config_data_signature, 0xCC));
			if (_GetConfigData)
				SKEETSDK_LOG("Found get config data function at %p.\n", _GetConfigData);
			else
				SKEETSDK_ERROR("Failed to find get config data function with: Invalid signature.\n");

			SKEETSDK_DECLASSIGN(_LoadConfigData, gctx.find(mem::signatures::load_config_siganture, 0xCC));
			if (_LoadConfigData)
				SKEETSDK_LOG("Found load config data function at %p.\n", _LoadConfigData);
			else
				SKEETSDK_LOG("Failed to find load config data function with: Invalid signature.\n");

			SKEETSDK_DECLASSIGN(_FetchRageTab, gctx.find(mem::signatures::after_tab_tage_signature, 0xCC));
			if (_FetchRageTab)
				SKEETSDK_LOG("Found fetch rage tab function at %p.\n", _FetchRageTab);
			else
				SKEETSDK_ERROR("Failed to find fetch rage tab function with: Invalid signature.\n");
#else	// SKEETSDK_TARGET_CRACK
			SKEETSDK_DECLASSIGN(_CurrentConfigHash, mem::constants::config_hash_address);
			SKEETSDK_DECLASSIGN(_GetConfigData, mem::constants::get_config_data_address);
			SKEETSDK_DECLASSIGN(_LoadConfigData, mem::constants::load_config_data_address);
			SKEETSDK_DECLASSIGN(_FetchRageTab, mem::constants::fetch_rage_tab_address);
#endif	// !SKEETSDK_TARGET_CRACK
		};

		const wchar_t* ConfigSystem::CurrentConfigName()
		{
			if (_CurrentConfigHash->hash())
			{
				ui::Listbox& listbox = gctx.Menu().tab(ui::CONFIG).child(0).element<ui::Listbox>(0);
				for (auto& item : listbox.Info.Items)
				{
					if (mut::FNV1a(item.Name.data()) == *_CurrentConfigHash)
						return item.Name.data();
				};
			};
			return nullptr;
		};

		bool ConfigSystem::LoadConfig(const char* name)
		{
			containers::skeetvec<uint8_t> cfgchunk;
			_GetConfigData(name, cfgchunk);
			if (cfgchunk.size())
			{
				*_CurrentConfigHash = mut::FNV1a(name);
				ui::Menu& menu = *gctx.Menu();
				_LoadConfigData(&menu, cfgchunk.data(), cfgchunk.size());
				for (auto& cfg : menu.tab(ui::CONFIG).as<ui::ConfigTab>().CEs[3]->as<ui::Listbox>().Info.Items)
				{
					if (mut::FNV1a(cfg.Name.data()) == *_CurrentConfigHash)
						cfg.set_active();
				};
				_FetchRageTab(menu.Tabs[ui::RAGE]);
				return true;
			};
			return false;
		};

		bool ConfigSystem::LoadConfig(mut::FNV1a hash)
		{
			for (auto& cfg : gctx.Menu().tab(ui::CONFIG).as<ui::ConfigTab>().CEs[3]->as<ui::Listbox>().Info.Items)
			{
				if (mut::FNV1a(cfg.Name.data()) == *_CurrentConfigHash)
				{
					char u8name[_MAX_FNAME];
					int count = WideCharToMultiByte(CP_UTF8, 0, cfg.Name.data(), -1, u8name, sizeof(u8name), 0, 0);
					if (count > 0)
						return LoadConfig(u8name);
				}
			};
			return false;
		};
	};	// namespace cfg

	namespace lua
	{
		LuaSystem::LuaSystem(skeetsdk::GlobalContext& gctx) : gctx(gctx)
		{
#ifndef SKEETSDK_TARGET_CRACK
			SKEETSDK_DECLASSIGN(_GetSLua, gctx.find(mem::signatures::get_slua_signature, 0xCC));
			if (_GetSLua)
			{
				SKEETSDK_LOG("Found get slua function at %p.\n", _GetSLua);
				_LuaLoader_State = *(decltype(&_LuaLoader_State))((char*)_GetSLua + 1);
				if (_LuaLoader_State)
					SKEETSDK_LOG("Found lua loader state at %p.\n", _LuaLoader_State);
				else
					SKEETSDK_LOG("Failed to find lua loader state with: Lua loader is null.\n");
			}
			else
				SKEETSDK_LOG("Failed to find get slua function with: Invalid signature.\n");

			_GetOrInitLuaState = (decltype(_GetOrInitLuaState))gctx.find("55 8B EC 83 EC 2C 56 57 E8");

			SKEETSDK_DECLASSIGN(_LoadLua, gctx.find(mem::signatures::load_lua_signature, 0xCC));
			SKEETSDK_DECLASSIGN(_IsLuaLoaded, gctx.find(mem::signatures::is_lua_loaded_signature, 0xCC));
			SKEETSDK_DECLASSIGN(_UpdateVisualLuaInfo, gctx.find(mem::signatures::update_visual_lua_info_signature, 0xCC));
#else // SKEETSDK_TARGET_CRACK
			SKEETSDK_DECLASSIGN(_GetSLua, mem::constants::get_slua_address);
			SKEETSDK_DECLASSIGN(_LuaLoader_State, mem::constants::lua_loader_address);
			SKEETSDK_DECLASSIGN(_IsLuaLoaded, mem::constants::is_lua_loaded_address);
			SKEETSDK_DECLASSIGN(_UpdateVisualLuaInfo, mem::constants::update_visual_lua_info_address);
#endif // !SKEETSDK_TARGET_CRACK
		};

		size_t LuaSystem::LuaCount()
		{
			return gctx.Menu().tab(ui::CONFIG).as<ui::ConfigTab>().Chunk.size() / sizeof(LuaInfo);
		};

		void* LuaSystem::GetOrInitLuaState()
		{
			return _GetOrInitLuaState();
		}

		void* LuaSystem::GetLuaState()
		{
			return GetLuaLoaderState()->state.LuaState;
		};

		SLua* LuaSystem::GetSLua()
		{
			return reinterpret_cast<SLua*>((_GetSLua() == GetLuaLoaderState()) ? nullptr : _GetSLua());
		};

		PCLua LuaSystem::GetLuaLoaderState()
		{
			return *_LuaLoader_State;
		};

		const wchar_t* LuaSystem::LuaName(mut::FNV1a hash)
		{
			containers::skeetvec<LuaInfo>& luas = *(containers::skeetvec<LuaInfo>*)&gctx.Menu().tab(ui::CONFIG).as<ui::ConfigTab>().Chunk;

			for (auto& lua : luas)
			{
				if (mut::FNV1a(lua.Name.data()) == hash)
					return lua.Name.data();
			};

			return nullptr;
		};

		bool LuaSystem::LoadScript(const wchar_t* name, bool on_startup)
		{
			ui::ConfigTab& config = gctx.Menu().tab(ui::CONFIG).as<ui::ConfigTab>();
			ui::Listbox& scripts = config.child(1).element<ui::Listbox>(3);
			LuaInfo* chunk = (LuaInfo*)config.Chunk.data();
			ui::ListboxVar* var = nullptr;

			for (size_t i = 0; i < LuaCount(); i++)
			{
				if (!wcscmp(chunk[i].Name.data(), name))
				{
					chunk = &chunk[i];
					var = &scripts.Info.Items[i];
					break;
				};
			};

			if (var == nullptr) return false;

			bool result = _LoadLua(chunk->Name.data());
			if (result && GetLuaState() != nullptr && _IsLuaLoaded(&chunk->Name))
			{
				var->set_active();
				chunk->OnStartup = on_startup;
			}
			_UpdateVisualLuaInfo(&config);
			return result;
		};

		bool LuaSystem::LoadScript(mut::FNV1a hash, bool on_startup)
		{
			ui::ConfigTab& config = gctx.Menu().tab(ui::CONFIG).as<ui::ConfigTab>();
			ui::Listbox& scripts = config.child(1).element<ui::Listbox>(3);
			LuaInfo* chunk = (LuaInfo*)config.Chunk.data();
			ui::ListboxVar* var = nullptr;

			for (size_t i = 0; i < LuaCount(); i++)
			{
				if (mut::FNV1a(chunk[i].Name.data()) == hash)
				{
					chunk = &chunk[i];
					var = &scripts.Info.Items[i];
					break;
				};
			};

			if (var == nullptr) return false;

			bool result = _LoadLua(chunk->Name.data());
			if (result && GetLuaState() != nullptr && _IsLuaLoaded(&chunk->Name))
			{
				var->set_active();
				chunk->OnStartup = on_startup;
			}
			_UpdateVisualLuaInfo(&config);
			return result;
		}
	};	// namespace lua

	namespace render
	{
		RendererHandler::RendererHandler(skeetsdk::GlobalContext& gctx)
		{
#ifndef SKEETSDK_TARGET_CRACK
			void* base = gctx.find(mem::signatures::renderer_signature, 0xCC);
			if (base)
			{
				RendererQueue** prenderer_address = *(RendererQueue***)((char*)base + 2);
				if (prenderer_address)
				{
					_Renderer = *prenderer_address;
					SKEETSDK_LOG("Found renderer instance at %p.\n", _Renderer);

					{
						void* events_base = gctx.find(mem::signatures::renderer_events_signature, 0xCC);
						if (events_base)
						{
							_IngameEvents = *decltype(&_IngameEvents)((char*)events_base + 0x2);
							if (_IngameEvents)
								SKEETSDK_LOG("Found renderer ingame events at %p.\n", _IngameEvents);
							else
								SKEETSDK_ERROR("Failed to find renderer ingame events with: InGame Events is null.\n");
						}
						else
							SKEETSDK_ERROR("Failed to find renderer ingame events with: Invalid signature.\n");
					};

					return;
				};
				SKEETSDK_ERROR("Failed to find renderer queue with: Invalid pointer at base(%p) + 0x2.\n", base);
				return;
			};
			SKEETSDK_ERROR("Failed to find renderer queue with: Invalid signature.\n");
#else	// SKEETSDK_TARGET_CRACK
			SKEETSDK_DECLASSIGN(_Renderer, mem::constants::renderer_queue_address);
#endif	// !SKEETSDK_TARGET_CRACK
		};
	}; // namespace render

	namespace ui
	{
		MenuHandler::MenuHandler(skeetsdk::GlobalContext& gctx)
		{
#ifndef SKEETSDK_TARGET_CRACK
			void* base = gctx.find(mem::signatures::menu_signature, 0xCC);
			if (base)
			{
				Menu** pmenu_address = *(Menu***)((char*)base + 2);
				if (pmenu_address)
				{
					_Menu = *pmenu_address;
					if (_Menu)
						SKEETSDK_LOG("Found menu instance at %p.\n", _Menu);
					else
						SKEETSDK_ERROR("Failed to find menu instance with: Menu is null.\n");

					SKEETSDK_DECLASSIGN(_TabCon, gctx.find(mem::signatures::tab_constructor_signature, 0xCC));
					if (_TabCon)
						SKEETSDK_LOG("Found tab constructor at %p.\n", _TabCon);
					else
						SKEETSDK_ERROR("Failed to find tab constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_InsertTab, gctx.find(mem::signatures::insert_tab_signature, 0xCC));
					if (_InsertTab)
						SKEETSDK_LOG("Found insert tab function at %p.\n", _InsertTab);
					else
						SKEETSDK_ERROR("Failed to find insert tab function with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_ChildCon, gctx.find(mem::signatures::child_constructor_signature, 0xCC));
					if (_ChildCon)
						SKEETSDK_LOG("Found child constructor at %p.\n", _ChildCon);
					else
						SKEETSDK_ERROR("Failed to find child constructor with: Invalid signature.\n");
					
					{
						void* ui_config_list_base = gctx.find(mem::signatures::ui_config_list_signature, 0xCC);
						if (ui_config_list_base)
						{
								SKEETSDK_DECLASSIGN(_UiConfigList, *(void**)((char*)ui_config_list_base + 0x1));
								if (_UiConfigList)
									SKEETSDK_LOG("Found ui config list at %p.\n", _UiConfigList);
								else
									SKEETSDK_ERROR("Failed to find ui config list with: Ui config list is null.\n");	
						}
						else
							SKEETSDK_ERROR("Failed to find ui config list with: Invalid signature.\n");;
					};

					{
						void* ui_hotkey_list_base = gctx.find(mem::signatures::ui_hotkey_list_signature, 0xCC);
						if (ui_hotkey_list_base)
						{
							SKEETSDK_DECLASSIGN(_UiHotkeyList, *(void**)((char*)ui_hotkey_list_base + 0x2));
							if (_UiHotkeyList)
								SKEETSDK_LOG("Found ui hotkey list at %p.\n", _UiHotkeyList);
							else
								SKEETSDK_ERROR("Failed to find ui hotkey list with: Ui hotkey list is null.\n");
						}
						else
							SKEETSDK_ERROR("Failed to find ui hotkey list with: Invalid signature.\n");;
					};

					SKEETSDK_DECLASSIGN(_AddLabel, gctx.find(mem::signatures::add_label_signature, 0xCC));
					if (_AddLabel)
						SKEETSDK_LOG("Found add label function at %p.\n", _AddLabel);
					else
						SKEETSDK_ERROR("Failed to find add label function with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_AddButton, gctx.find(mem::signatures::add_button_signature, 0xCC));
					if (_AddButton)
						SKEETSDK_LOG("Found add button function at %p.\n", _AddButton);
					else
						SKEETSDK_ERROR("Failed to find add button function with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_CheckboxCon, gctx.find(mem::signatures::checkbox_constructor_signature, 0xCC));
					if (_CheckboxCon)
						SKEETSDK_LOG("Found checkbox constructor at %p.\n", _CheckboxCon);
					else
						SKEETSDK_ERROR("Failed to find checkbox constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_TextboxCon, gctx.find(mem::signatures::textbox_constrcutor_signature, 0xCC));
					if (_TextboxCon)
						SKEETSDK_LOG("Found textbox constructor at %p.\n", _TextboxCon);
					else
						SKEETSDK_ERROR("Failed to find textbox constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_SliderCon, gctx.find(mem::signatures::slider_constrcutor_signature, 0xCC));
					if (_SliderCon)
						SKEETSDK_LOG("Found slider constructor at %p.\n", _SliderCon);
					else
						SKEETSDK_ERROR("Failed to find slider constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_ColorPickerCon, gctx.find(mem::signatures::colorpicker_constrcutor_signature, 0xCC));
					if (_ColorPickerCon)
						SKEETSDK_LOG("Found colorpicker constructor at %p.\n", _ColorPickerCon);
					else
						SKEETSDK_ERROR("Failed to find colorpicker constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_HotkeyCon, gctx.find(mem::signatures::hotkey_constrcutor_signature, 0xCC));
					if (_HotkeyCon)
						SKEETSDK_LOG("Found hotkey constructor at %p.\n", _HotkeyCon);
					else
						SKEETSDK_ERROR("Failed to find hotkey constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_MultiselectCon, gctx.find(mem::signatures::multiselect_constrcutor_signature, 0xCC));
					if (_MultiselectCon)
						SKEETSDK_LOG("Found multiselect constructor at %p.\n", _MultiselectCon);
					else
						SKEETSDK_ERROR("Failed to find multiselect constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_ListboxCon, gctx.find(mem::signatures::listbox_constrcutor_signature, 0xCC));
					if (_ListboxCon)
						SKEETSDK_LOG("Found listbox constructor at %p.\n", _ListboxCon);
					else
						SKEETSDK_ERROR("Failed to find listbox constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_ComboboxCon, gctx.find(mem::signatures::combobox_constrcutor_signature, 0xCC));
					if (_ComboboxCon)
						SKEETSDK_LOG("Found combobox constructor at %p.\n", _ComboboxCon);
					else
						SKEETSDK_ERROR("Failed to find combobox constructor with: Invalid signature.\n");

#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
					SKEETSDK_DECLASSIGN(_FlatWeaponSelectorCon, gctx.find(mem::signatures::flat_weaponselector_constrcutor_signature, 0xCC));
					if (_FlatWeaponSelectorCon)
						SKEETSDK_LOG("Found flat weaponselector constructor at %p.\n", _FlatWeaponSelectorCon);
					else
						SKEETSDK_ERROR("Failed to find flat weaponselector constructor with: Invalid signature.\n");

					SKEETSDK_DECLASSIGN(_CaseWeaponSelectorCon, gctx.find(mem::signatures::case_weaponselector_constrcutor_signature, 0xCC));
					if (_CaseWeaponSelectorCon)
						SKEETSDK_LOG("Found case weaponselector constructor at %p.\n", _CaseWeaponSelectorCon);
					else
						SKEETSDK_ERROR("Failed to find case weaponselector constructor with: Invalid signature.\n");
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS

					return;
				};
				SKEETSDK_ERROR("Failed to find menu with: Invalid pointer at base(%p) + 0x2.\n", base);
				return;
			};
			SKEETSDK_ERROR("Failed to find menu with: Invalid signature.\n");
#else // SKEETSDK_TARGET_CRACK
			SKEETSDK_DECLASSIGN(_Menu, mem::constants::menu_address);
			SKEETSDK_DECLASSIGN(_TabCon, mem::constants::tab_constructor_address);
			SKEETSDK_DECLASSIGN(_InsertTab, mem::constants::insert_tab_address);
			SKEETSDK_DECLASSIGN(_ChildCon, mem::constants::child_constructor_address);
			SKEETSDK_DECLASSIGN(_UiConfigList, mem::constants::ui_config_list_address);
			SKEETSDK_DECLASSIGN(_AddLabel, mem::constants::add_label_address);
			SKEETSDK_DECLASSIGN(_AddButton, mem::constants::add_button_address);
			SKEETSDK_DECLASSIGN(_CheckboxCon, mem::constants::checkbox_constructor_address);
			SKEETSDK_DECLASSIGN(_TextboxCon, mem::constants::textbox_constructor_address);
			SKEETSDK_DECLASSIGN(_SliderCon, mem::constants::slider_constructor_address);
			SKEETSDK_DECLASSIGN(_ColorPickerCon, mem::constants::colorpicker_constructor_address);
			SKEETSDK_DECLASSIGN(_HotkeyCon, mem::constants::hotkey_constructor_address);
			SKEETSDK_DECLASSIGN(_MultiselectCon, mem::constants::multiselect_constructor_address);
			SKEETSDK_DECLASSIGN(_ListboxCon, mem::constants::listbox_constructor_address);
			SKEETSDK_DECLASSIGN(_ComboboxCon, mem::constants::combobox_constructor_address);
#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
			SKEETSDK_DECLASSIGN(_FlatWeaponSelectorCon, mem::constants::flat_weaponselector_constructor_address);
			SKEETSDK_DECLASSIGN(_CaseWeaponSelectorCon, mem::constants::case_weaponselector_constructor_address);
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS
#endif // !SKEETSDK_TARGET_CRACK
		};

		Tab* MenuHandler::create_tab(const wchar_t* name)
		{
			Tab* ptab = mem::SkeetAllocator<Tab>::allocate(1);
			if (ptab)
			{
				misc::Vec2<int> icon_pos = _Menu->Tabs.back()->Pos;
				icon_pos.y += 0x40;
				_TabCon(ptab, _Menu, name, &icon_pos);
				_InsertTab(_Menu, ptab);
			};
			return ptab;
		};

		Child* MenuHandler::create_child(Tab& tab, const wchar_t* name, misc::Vec4<uint8_t> layout, uint32_t elements_capacity, bool modifiable)
		{
			Child* cptr = mem::SkeetAllocator<Child>::allocate(1);
			if (cptr)
			{
				_ChildCon(cptr, &tab, name, layout, elements_capacity, modifiable);
				tab.Childs.push_back(cptr);
			};
			return cptr;
		};

		Label* MenuHandler::create_label(Child& child, const wchar_t* name)
		{
			Label* lptr = _AddLabel(&child, name);
			return lptr;
		};

		Button* MenuHandler::create_button(Child& child, const wchar_t* name, Fast2PtrFn callback, void* callback_arg)
		{
			Button* bptr = _AddButton(&child, name, callback_arg, callback);
			return bptr;
		};

		Textbox* MenuHandler::create_textbox(Child& child)
		{
			Textbox* tptr = mem::SkeetAllocator<Textbox>::allocate(1);
			if (tptr)
			{
				_TextboxCon(tptr, &child);
				child.Elements.push_back(tptr);
			};
			return tptr;
		};

		Checkbox* MenuHandler::create_checkbox(Child& child, const wchar_t* name, bool initial_value)
		{
			Checkbox* cptr = mem::SkeetAllocator<Checkbox>::allocate(1);
			if (cptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<bool>(hash, containers::LBOOL, initial_value);
				_CheckboxCon(cptr, &child, name, &node->data);
				child.Elements.push_back(&cptr->as<UiBase<void>>());
			};
			return cptr;
		};

		Slider* MenuHandler::create_slider(Child& child, const wchar_t* name, int min, int max, int initial_value, bool tooltip, Slider::TooltipString tip, float step)
		{
			Slider* sptr = mem::SkeetAllocator<Slider>::allocate(1);
			if (sptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<int>(hash, containers::LINTEGER, initial_value);
				_SliderCon(sptr, &child, name, min, max, &node->data, tooltip, tip, step);
				child.Elements.push_back(&sptr->as<UiBase<void>>());
			};
			return sptr;
		};

		ColorPicker* MenuHandler::create_colorpicker(Child& child, const wchar_t* name, misc::RGBA initial_value)
		{
			ColorPicker* cpptr = mem::SkeetAllocator<ColorPicker>::allocate(1);
			if (cpptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<misc::RGBA>(hash, containers::LCOLOR, initial_value);
				_ColorPickerCon(cpptr, &child, &node->data);
				child.Elements.push_back(&cpptr->as<UiBase<void>>());
			};
			return cpptr;
		};

		Hotkey* MenuHandler::create_hotkey(Child& child, const wchar_t* name, uint32_t virtual_key, Hotkey::HotkeyMode mode, bool has_popup)
		{
			Hotkey* hptr = mem::SkeetAllocator<Hotkey>::allocate(1);
			if (hptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<containers::LinkedNode<HotkeyInfo>>(hash, containers::LHOTKEY, 0, containers::LVOID, virtual_key, mode);
				_UiHotkeyList->insert_back(&node->data);
				_HotkeyCon(hptr, &child, &node->data, has_popup);
				child.Elements.push_back(&hptr->as<UiBase<void>>());
			};
			return hptr;
		};

		Multiselect* MenuHandler::create_multiselect(Child& child, const wchar_t* name, std::span<std::string_view> variables, uint32_t initial_value, bool always_valuable)
		{
			Multiselect* mptr = mem::SkeetAllocator<Multiselect>::allocate(1);
			if (mptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<uint32_t>(hash, containers::LINTEGER, initial_value);
				_MultiselectCon(mptr, &child, name, &node->data, 0, always_valuable);

				size_t cvars = variables.size() < 32 ? variables.size() : 32;
				mptr->Vars.resize(cvars);
				for (size_t i = 0; i < cvars; i++)
				{
					std::string_view& var = variables[i];
					mptr->Vars[i].Name.assign(var.begin(), var.end());
					mptr->Vars[i].BitMask = 1 << i;
				};

				child.Elements.push_back(&mptr->as<UiBase<void>>());
			};
			return mptr;
		};

		Listbox* MenuHandler::create_listbox(Child& child, const wchar_t* name, misc::Vec2<int> size, std::span<std::wstring_view> variables, uint32_t initial_value, bool has_search)
		{
			Listbox* lptr = mem::SkeetAllocator<Listbox>::allocate(1);
			if (lptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<uint32_t>(hash, containers::LINTEGER, initial_value);
				_ListboxCon(lptr, &child, name, size, &node->data, has_search);

				lptr->Info.SelectedItem = initial_value;

				size_t cvars = variables.size();
				lptr->VarsCount = cvars;

				lptr->Info.Items.reserve(cvars);
				lptr->SSpecs.reserve(cvars);
				for (uint32_t i = 0; i < cvars; i++)
				{
					std::wstring_view& var = variables[i];
					lptr->Info.Items.emplace_back(var, i);
					lptr->SSpecs.push_back(i);
				};

				child.Elements.push_back(&lptr->as<UiBase<void>>());
			};
			return lptr;
		};

		Combobox* MenuHandler::create_combobox(Child& child, const wchar_t* name, std::span<std::wstring_view> variables, uint32_t initial_value, bool sameline)
		{
			Combobox* cptr = mem::SkeetAllocator<Combobox>::allocate(1);
			if (cptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<uint32_t>(hash, containers::LINTEGER, initial_value);
				_ComboboxCon(cptr, &child, name, &node->data, sameline);

				cptr->SelectedItem = initial_value;

				size_t cvars = variables.size();
				cptr->Vars.resize(cvars);
				for (size_t i = 0; i < cvars; i++)
				{
					std::wstring_view& var = variables[i];
					cptr->Vars[i].Name.assign(var.begin(), var.end());
					cptr->Vars[i].BitMask = 1 << i;
				};

				child.Elements.push_back(&cptr->as<UiBase<void>>());
			};
			return cptr;
		};

#ifdef SKEETSDK_ENABLE_UI_WEAPONSELECTORS
		FlatWeaponSelector* MenuHandler::create_flat_weaponselector(Child& child, const wchar_t* name, uint32_t initial_value)
		{
			FlatWeaponSelector* fptr = mem::SkeetAllocator<FlatWeaponSelector>::allocate(1);
			if (fptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<uint32_t>(hash, containers::LINTEGER, initial_value);
				_FlatWeaponSelectorCon(fptr, &child, name, &node->data, 0);

				fptr->SelectedItem = initial_value;

				child.Elements.push_back(&fptr->as<UiBase<void>>());
			};
			return fptr;
		};

		CaseWeaponSelector* MenuHandler::create_case_weaponselector(Child& child, const wchar_t* name, uint32_t initial_value)
		{
			CaseWeaponSelector* cptr = mem::SkeetAllocator<CaseWeaponSelector>::allocate(1);
			if (cptr)
			{
				uint32_t hash = generate_ui_hash(child, name);
				auto* node = _UiConfigList->emplace_back<uint32_t>(hash, containers::LINTEGER, initial_value);
				_CaseWeaponSelectorCon(cptr, &child, name, &node->data);

				cptr->SelectedItem = initial_value;

				child.Elements.push_back(&cptr->as<UiBase<void>>());
			};
			return cptr;
		};
#endif	// SKEETSDK_ENABLE_UI_WEAPONSELECTORS

		uint32_t MenuHandler::generate_ui_hash(Child& child, const wchar_t* name)
		{
			wchar_t hash_buff[128];
			wchar_t tempbuff[64];
			std::shared_ptr child_name = child.Name;
			std::shared_ptr tab_name = child.Parent->as<Tab>().Name;

			memset(hash_buff, 0, sizeof(hash_buff));
			memset(tempbuff, 0, sizeof(tempbuff));

			crypt::XorCryptor::Decrypt(child_name.get(), sizeof(*tempbuff), tempbuff, sizeof(tempbuff));

			if (wcscat_s(hash_buff, sizeof(hash_buff) / sizeof(*hash_buff), tempbuff) != 0) return 0;

			memset(tempbuff, 0, sizeof(tempbuff));
			crypt::XorCryptor::Decrypt(tab_name.get(), sizeof(*tempbuff), tempbuff, sizeof(tempbuff));

			if (wcscat_s(hash_buff, sizeof(hash_buff) / sizeof(*hash_buff), tempbuff) != 0 || wcscat_s(hash_buff, sizeof(hash_buff) / sizeof(*hash_buff), name) != 0) return 0;

			return mut::FNV1a(hash_buff);
		};
	};	// namespace ui
#endif	// SKEETSDK_IMPLEMENTATION
};

#endif // SKEET_SDK_HPP
