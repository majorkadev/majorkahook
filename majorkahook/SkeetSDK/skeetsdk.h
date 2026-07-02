

#ifndef SKEET_H
#define SKEET_H
#include <Windows.h>
#include <psapi.h>
#include <vector>
#include <string_view>
#include <memory>

#pragma push_macro("max")
#undef max;

#define SIZEASSERT(_struct, _size) static_assert(sizeof(_struct) == _size, #_struct " should be " #_size " bytes long");

#ifdef INCLUDE_NLOHMANN_JSON_HPP_
#define HAS_JSON 1
#else
#define HAS_JSON 0
#endif 

#if defined(__GNUC__) || defined(__GNUG__)
static_assert(false, "GCC not supported!");
#endif 

#define FORCECALL [[msvc::noinline]]

#ifdef _MSC_VER
#define FLATT [[msvc::flatten]]
#else
#define FLATT [[gnu::flatten]]
#endif _MSC_VER

#define LBOX_ACTIVE_FLAG 1u << 31


#ifdef _SDK_PUBLIC_UNSAFE_MEMBERS
#define MEMBERS_PRIVATE public
#define MEMBERS_PUBLIC public
#else
#define MEMBERS_PRIVATE private
#define MEMBERS_PUBLIC	public
#endif 

#ifdef SDK_RENDERER_IMP
#define TEXT_FLAG_DPI_SCALED		0x0
#define TEXT_FLAG_BOLD				0x1
#define TEXT_FLAG_LARGE				0x2
#define TEXT_FLAG_SMALL				0x4
#define TEXT_FLAG_CENTRED			0x10
#define TEXT_FLAG_RIGHT_ALIGNED		0x80
#define TEXT_FLAG_DEFAULT			0x100

#define TEXTURE_FLAG_AUTO			0x0
#define TEXTURE_FLAG_FILLED			0x1
#define TEXTURE_FLAG_REPEAT			0x2
#endif 

#define SKEET_HEAD_SIGNATURE 0xDEC00D60
#define SKEET_XOR_KEY 0x92CA5DF1
#define SKEET_FNV1A_PRIME 0x1000193
#define SKEET_FNV1A_BASE 0x811C9DC5

namespace SkeetSDK {

	
	typedef void* (__cdecl* CFn)(void);	
	typedef void(__thiscall* ThisFn)(void*);
	typedef	void(__thiscall* T2PFn)(void*, void*); 
	typedef void(__fastcall* F2PFn)(void*, void*);	
	typedef bool(__thiscall* BTFn)(void*); 
	typedef void(__thiscall* ThisIntFn)(void*, int);
	
	typedef void* (__thiscall* AllocatorFn)(int);
	
	typedef bool(__thiscall* LoadLuaFn)(const wchar_t*);
	typedef void* (__fastcall* CryptFn)(const wchar_t*, size_t, int);
	typedef int(__fastcall* DecryptFn)(void*, int, wchar_t*, int);
	typedef bool(__fastcall* GetConfigDataFn)(const char*, void*);
	typedef void(__fastcall* LoadConfigFn)(void*, void*, size_t);
	
	typedef int(__fastcall* SetKeyFn)(void*, unsigned int, unsigned int);
	typedef void(__fastcall* SetModeFn)(void*, void*, int);
	typedef void(__thiscall* SetCheckFn)(void*, bool);
	typedef void(__thiscall* HideUiFn)(void*, bool);
	typedef void(__thiscall* InsertFn)(void*, bool, void*);
	typedef void* (__thiscall* AddLabelFn)(void*, bool, const wchar_t*);
	typedef void* (__thiscall* AddButtonFn)(void*, void*, const wchar_t*, void*, void*);
	typedef void* (__thiscall* TBConFn)(void*, void*); 
	typedef void* (__thiscall* CPConFn)(void*, void*, void*); 
	typedef void* (__thiscall* LConFn)(void*, void*, const wchar_t*); 
	typedef void* (__thiscall* TCConFn)(void*, void*, const wchar_t*, int*); 
	typedef void* (__thiscall* SConFn)(void*, void*, const wchar_t*, int, int, int*, bool, unsigned int, float); 
	typedef void* (__thiscall* HKConFn)(void*, void*, void*, bool); 
	typedef void* (__thiscall* BConFn)(void*, void*, const wchar_t*, int, void*, void*, int); 
	typedef void* (__thiscall* CBConFn)(void*, void*, const wchar_t*, int*, bool); 
	typedef void* (__thiscall* MConFn)(void*, void*, const wchar_t*, int*, int, bool); 
	typedef void* (__thiscall* LBConFn)(void*, void*, const wchar_t*, int, int, int*, bool); 
	typedef void* (__thiscall* CHConFn)(void*, void*, const wchar_t*, int, int, bool); 
	typedef size_t(__thiscall* GetTBoxTextFn)(void*, wchar_t*);
	
	typedef void(__fastcall* RenderFn)(void*, void*);
	typedef void(__cdecl* RenderEventListenerFn)(void);
	typedef void* (__fastcall* ReadFileFn)(void*, const char*);
	typedef bool(__fastcall* LoadTextureFn)(int, const unsigned char*, int, int*, int*, void*);
	typedef int(__fastcall* LoadSvgFormFileFn)(const char*, int, int, int);
	typedef void(__thiscall* RenderIndicatorFn)(int*, void*, const char*, int, unsigned int, int, float);
	typedef bool(__fastcall* WorldToScreenFn)(float*, int*, int*);
	typedef void(__thiscall* RenderBlurFn)(void*, int, int, int, int);
	
	typedef void(__thiscall* LogFn)(char*);
	typedef void(__cdecl* LogErrorFn)(const char*, ...);
	typedef void(__cdecl* LogColorFn)(void*, int*, char*, ...);
	typedef void(__thiscall* ScreenLogFn)(const char*, int);
	typedef void(__fastcall* SetClanTagFn)(const char*, const char*);

	enum ETab : uint32_t
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

	enum MenuFlags
	{
		MenuClosed = 0,
		MenuOpened = 1,
		MenuDragged = 4,
		MenuScaled = 8
	};

	enum FadeType
	{
		FadeNone,
		FadeOut,
		FadeIn
	};

	enum ChildStatus
	{
		ChildIdle,
		ChildDragged,
		ChildResized
	};

	enum MouseStatus
	{
		MouseIdle,
		LClick,
		RClick
	};

	enum PickerStatus
	{
		PickerIdle,
		PickerClicked,
		PickerTint,
		PickerHue,
		PickerAlpha
	};

	enum LinkedType : uint16_t
	{
		LBOOL = 1,
		LINTEGER = 3,
		LCOLOR = 4,
		LARRAY = 6,
		LHOTKEY = 7,
		LPOSSIZE = 11,
		LVECTOR = 12,
	};

	enum UiType : unsigned char
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

	enum HotkeyMode
	{
		Inherit = -1,
		AlwaysOn,
		OnHotkey,
		Toggle,
		OffHotkey
	};

	enum TextureType
	{
		TEXTURE_RGBA = -1,
		TEXTURE_SVG,
		TEXTURE_PNG,
		TEXTURE_JPG
	};

	enum RenderEventType
	{
		REVENT_RENDER,
		REVENT_MENU,
		REVENT_FINAL
	};


	struct Vec2f_t
	{
		float x;
		float y;
		Vec2f_t(float x, float y) : x(x), y(y) {};
		Vec2f_t operator-(float min)
		{
			return Vec2f_t(this->x - min, this->y - min);
		};
		void clamp(float min, float max = 3.402823466e+38f)
		{
			if (this->x > max) this->x = max;
			if (this->x < min) this->x = min;
			if (this->y > max) this->y = max;
			if (this->y < min) this->y = min;
		};
	};

	struct Vec3f_t
	{
		float x;
		float y;
		float z;
		Vec3f_t() = default;
		float* array()
		{
			return reinterpret_cast<float*>(this);
		};
	};

	struct Vec2
	{
		int x;
		int y;
		Vec2() {};
		Vec2(int x, int y)
		{
			set(x, y);
		};

		inline Vec2 operator+(Vec2 vec)
		{
			return Vec2(this->x + vec.x, this->y + vec.y);
		};

		inline Vec2 operator+(int sum)
		{
			return Vec2(this->x + sum, this->y + sum);
		};

		inline Vec2 operator-(Vec2 vec)
		{
			return Vec2(this->x - vec.x, this->y - vec.y);
		};

		inline Vec2 operator-(int sub)
		{
			return Vec2(this->x - sub, this->y - sub);
		};

		inline Vec2 operator*(float mul)
		{
			return Vec2(this->x * mul, this->y * mul);
		};

		inline Vec2 operator*(Vec2f_t mul)
		{
			return Vec2((int)((float)this->x * mul.x), (int)((float)this->y * mul.y));
		};

		inline Vec2& operator+=(const Vec2& vec)
		{
			this->x += vec.x;
			this->y += vec.y;
			return *this;
		}

		inline void set(int x, int y)
		{
			this->x = x;
			this->y = y;
		};

		inline int* array()
		{
			return reinterpret_cast<int*>(this);
		};

		inline int& operator[](size_t idx)
		{
			return array()[idx];
		};

#if HAS_JSON
		friend inline void to_json(nlohmann::json& j, const Vec2& vec)
		{
			j = nlohmann::json::array({vec.x, vec.y});
		};

		friend inline void from_json(const nlohmann::json& j, Vec2& vec)
		{
			vec.x = j[0].get<int>();
			vec.y = j[1].get<int>();
		};
#endif
	};

	class VecCol
	{
		using col_t = unsigned char;
	public:
		col_t r;
		col_t g;
		col_t b;
		col_t a;
		VecCol() {};

		VecCol(unsigned int hex)
		{
			set(hex);
		};

		VecCol(unsigned int r, unsigned int g, unsigned int b, unsigned int a) 
		{
			set(r, g, b, a);
		};

		inline void set(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		};

		inline void set(unsigned int hex)
		{
			*reinterpret_cast<int*>(this) = hex;
		};

		inline unsigned int pack()
		{
			return *reinterpret_cast<int*>(this);
		};

		inline VecCol negated()
		{
			return VecCol(255 - this->r, 255 - this->g, 255 - this->b, this->a);
		};

		inline void invert()
		{
			*this = this->negated();
		};

		inline col_t* array()
		{
			return reinterpret_cast<col_t*>(this);
		};

		inline col_t& operator[](size_t idx)
		{
			return array()[idx];
		};

#if HAS_JSON
		friend inline void to_json(nlohmann::json& j, const VecCol& col)
		{
			j = nlohmann::json::array({ col.r, col.g, col.b, col.a });
		};

		friend inline void from_json(const nlohmann::json& j, VecCol& col)
		{
			col.set(j[0].get<uint32_t>(), j[1].get<uint32_t>(), j[2].get<uint32_t>(), j[3].get<uint32_t>());
		};
#endif
	};

	struct Vec4_8t
	{
		unsigned char x;
		unsigned char y;
		unsigned char z;
		unsigned char w;
		int pack()
		{
			return (this->x | this->y << 8 | this->z << 16 | this->w << 24);
		};
	};
	
	template<typename T>
	class sVec
	{
		T* buffer;
		T* endbuff;
		size_t length;
	public:
		sVec(size_t size) : length(0)
		{
			this->buffer = (T*)malloc(sizeof(T) * size);
			this->endbuff = this->buffer + size;
		}

		~sVec()
		{
			free(this->buffer);
		};

		void insert(T elem, size_t index)
		{
			size_t csize = size();
			T* newbuff = (T*)malloc(sizeof(T) * csize + 1);
			if (!newbuff) return;
			memcpy(newbuff, this->buffer, index * sizeof(T));
			memcpy(newbuff + index + 1, this->buffer + index, (csize - index) * sizeof(T));
			newbuff[index] = elem;
			free(this->buffer);
			this->buffer = newbuff;
			this->endbuff = this->buffer + ++csize;
			this->length++;
		};

		void push_back(T elem)
		{
			if (this->length >= size())
				resize(size() * 2);
			this->buffer[this->length++] = elem;
		};

		T pop(size_t index = 0)
		{
			if (this->length == 0) return T{ 0 };
			if (index >= this->length) return T{ 0 };
			T elem = this->buffer[index];
			remove(index);
			return elem;
		};

		T pop_back()
		{
			return pop(this->length - 1);
		};

		void fill(T val, size_t size)
		{
			if (size > this->size())
				resize(size);
			memset(this->buffer, val, size * sizeof(T));
		};

		void clear()
		{
			memset(this->buffer, 0, bsize());
			this->length = 0;
		};

		void remove(size_t index)
		{
			memcpy(buffer + index, buffer + index + 1, (this->length-- - index) * sizeof(T));
		};

		void removef(size_t index)
		{
			free(buffer[index]);
			memcpy(buffer + index, buffer + index + 1, (this->length-- - index) * sizeof(T));
		};

		void resize(size_t size)
		{
			T* newbuff = (T*)realloc(this->buffer, size);
			if (!newbuff) return;
			if (this->length >= size)
				this->length = size - 1;
			this->buffer = newbuff;
			this->endbuff = this->buffer + size;
		};

		size_t len()
		{
			return this->length;
		};

		size_t bsize()
		{
			return (uintptr_t)this->endbuff - (uintptr_t)this->buffer;
		};

		size_t size()
		{
			return bsize() / sizeof(T);
		};

		size_t cappacity()
		{
			return (uintptr_t)this->endbuff - (uintptr_t)(this->buffer + this->length);
		};

		T& last()
		{
			return this->buffer[this->length - 1];
		};

		T* begin()
		{
			return this->buffer;
		};

		T* end()
		{
			return this->endbuff;
		};

		T& operator[](size_t i)
		{
			return this->buffer[i];
		};

		void operator<<(T elem)
		{
			push_back(elem);
		};

		void operator<<=(T val)
		{
			last() = val;
		};

		void operator>>(T& out)
		{
			out = pop_back();
		};

		void operator >>=(T& out)
		{
			out = last();
		};

	};

	namespace Memory {

		class SigFinder
		{
			MODULEINFO info;
			static sVec<short> ida_sig_resolver(const char* sig);
		public:
			SigFinder(const char* module);
			SigFinder(LPVOID lpBaseAdress, DWORD SizeOfImage);
			FORCECALL void* find(const char* sig, int offset = 0);
			FORCECALL void* findr32(const char* sig);
		};

		class CHooked
		{
			bool Status;
			void* Address;
			unsigned char* OriginalBytes;
			size_t BytesSize;
			void* naked;
		public:
			CHooked(void* func, unsigned char* ogbytes, size_t size);
			~CHooked();
			void* Naked();
			void Unhook();
		};

		class DetourHook
		{
			static sVec<CHooked*> Hooked;
		public:
			enum JuncType : unsigned char
			{
				CALL = 0xE8,
				JUMP
			};

			static CHooked* Hook(void* Dst, void* Src, size_t size = 5, JuncType junc = JUMP);
			static void UnhookAll();
		};
		static SigFinder	CheatChunk((LPVOID)0x43310000, 0x2FC000u);
		static AllocatorFn	Allocator;
		static ThisFn		Deallocator; 

		template<typename T>
		class SkeetAllocator
		{
		public:
			using value_type = T;
			using pointer = T*;
			using const_pointer = const T*;
			using reference = T&;
			using const_reference = const T&;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			template<typename U>
			struct rebind {
				using other = SkeetAllocator<U>;
			};

			SkeetAllocator() = default;

			template <class U>
			SkeetAllocator(const SkeetAllocator<U>&) noexcept {};

			SkeetAllocator(const SkeetAllocator&) = default;

			SkeetAllocator& operator=(const SkeetAllocator&) = default;

			pointer address(reference value) const { return &value; };

			pointer allocate(size_type n)
			{
				if (auto ptr = static_cast<pointer>(Allocator(n * sizeof(value_type))))
					return ptr;
				throw std::bad_alloc();
			};

			void deallocate(pointer ptr, size_type) noexcept
			{
				Deallocator(ptr);
			};

			template<typename U, typename... Args>
			void construct(U* ptr, Args&&... args) { new(ptr) U(std::forward<Args>(args)...); };

			template<typename U>
			void destroy(U* ptr) { ptr->~U(); };

			size_type max_size() const noexcept { return std::numeric_limits<size_type>::max() / sizeof(value_type); };
		};

		template <typename T, typename U>
		bool operator==(const SkeetAllocator<T>&, const SkeetAllocator<U>&) noexcept { return true; }

		template <typename T, typename U>
		bool operator!=(const SkeetAllocator<T>&, const SkeetAllocator<U>&) noexcept { return false; }
		
		static SkeetAllocator<uint8_t> bytealloc;
	}
	
	template<typename _Ty>
	using skeetvec = std::vector<_Ty, Memory::SkeetAllocator<_Ty>>;


#pragma pack(push, 1)

	struct ConfigHead
	{
		uint32_t	sig;
		uint32_t	unkn0;	
		uint32_t	xkey;
	};

	struct ConfigDataUnit
	{
		uint16_t	data_size;
		LinkedType	data_type;
		uint32_t	hash;
		uint8_t		data[];
		
		template<typename T>
		T& get_ref()
		{
			return *reinterpret_cast<T*>(&data);
		};
	};

	struct LinkedNodeBase
	{
		LinkedNodeBase* next;
		LinkedNodeBase* prev;
		uint32_t		hash;
		uint16_t		data_size;
		LinkedType		data_type;
	};

	typedef struct Child* PChild;
	typedef struct CTab* PCTab;
	typedef struct Element* PElement;
	typedef struct CMenu* PCMenu;
	typedef struct Hotkey* PHotkey;
	struct MouseInfo
	{
		MouseStatus Status;
		Vec2		Pos;
		Vec2		LastMenuPos;
	};

	struct Flags_t
	{
		bool Editable;
		bool Visible;
		bool Hovered;
		bool AllowMouseInput;
		bool SameLine;
	};

	typedef struct XorW
	{
		unsigned short	key;
		unsigned short	bsize;
		wchar_t			data[];
	};

	typedef struct Call
	{
		void*	ecx;
		F2PFn	function;	
		Call() {};
		Call(void* ecx, F2PFn function) : ecx(ecx), function(function) {};
	};

	template <typename T>
	struct BoxVars
	{
		skeetvec<T>	Name;
		char		pad1[0x4];
		int			Index;
	};

	template <typename T>
	struct Header
	{
		char	pad1[0x8];
		int		Index;		
		T*		Parent;		
		Flags_t	Flags;		
		UiType	Type;		
		char	pad2[0x2];
		int		Font;		
	};

	struct MsgInfo
	{
		HWND hWnd;
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
	};

	
	class __declspec(novtable) IElement
	{
	public:
		virtual void Decay(bool free) = 0;
		virtual bool IsHovered(const Vec2& cursorpos) = 0;
		virtual bool HandleMouse(const MsgInfo& msg, const Vec2& cursorpos) = 0;
		virtual bool HandleInput(const MsgInfo& msg) = 0;
	private:
		virtual void Render() = 0;
	public:
		virtual void OnConfigLoad() = 0;
		virtual void ResetInputState() = 0;
		virtual void OnValueSet() = 0;
		virtual void ResetInputStateIfShould() = 0;
		virtual void HandleResize() = 0;
		virtual void HandleDPI() = 0;
	private:
		virtual void fn11() = 0;
		virtual void fn12() = 0;
	};

	
	typedef struct Slider : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;			
		Vec2					Size;			
		char					pad1[0x8];
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;			
		char					pad3[0x4];
		VecCol					Color;			
		int						LeftPaddign;	
		char					pad4[0x4];
		int*					Value;			
		Vec2					DefSize;		
		bool					Active;			
		bool					Tooltip;		
		char					pad5[0x26];
		int						Max;			
		int						Min;			
		float					Step;			
		int						MaxLen;			
		char					pad6[0x84];
	} *PSlider;

	
	typedef struct Checkbox : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;			
		Vec2					ActivateSize;	
		Vec2					DefActivateSize;
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;			
		char					pad3[0x4];
		VecCol					Color;			
		int						LeftPaddign;	
		char					pad4[0x4];
		bool*					Value;			
		Vec2					Size;			
		int						TextPadding;	
		bool					HoldingOn;		
		char					pad5[0x3];
		PElement				TiedElements;	
		void*					SomePtr1;		
	} *PCheckbox;

	SIZEASSERT(Checkbox, 0x78);

	
	typedef struct Multiselect : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;				
		Vec2					Size;				
		Vec2					OuterPadding;		
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;				
		char					pad2[0x4];
		VecCol					Color;				
		int						LeftPaddign;		
		char					pad3[0x4];
		int*					Value;				
		int						heigth;				
		char					pad4[0x4];
		bool					Popup;				
		bool					Clicked;			
		bool					NotNull;			
		char					pad5;
		int						HoveredItem;		
		skeetvec<char>			StringValue;		
		char					pad6[0x4];
		skeetvec<BoxVars<char>>	Vars;				
		char					pad7[0x4];
		BoxVars<char>			Varsi[];			
	} *PMultiselect;

	struct ListboxVar
	{
		unsigned int		Index;
		skeetvec<wchar_t>	Name;
		char				pad2[0x4];
		ListboxVar(size_t Index, skeetvec<wchar_t>& Name) : Index(Index), Name(Name) {};
	};

	typedef struct HotkeyPopup : IElement
	{
		Header<Hotkey>		Header;
		Vec4_8t				XAxis;			
		Vec4_8t				YAxis;			
		Vec2				Size;			
		char				pad3[0x20];
		VecCol				Color;			
		char				pad4[0x14];
		int					HoveredItem;	
		int					SelectedItem;	
		SetModeFn			SetMode;		
		std::wstring_view	Variants[4];	
	} HotkeyPop, *PHotkeyPop;

	
	typedef struct HotkeyNode
	{
		LinkedNodeBase		base;
		uint32_t			ToogleState : 2; 
		uint32_t			Key : 30;
		HotkeyMode			Mode;		
	} HotkeyInfo, * PHotkeyInfo;

	
	struct Hotkey : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;					
		Vec2					ActivateSize;			
		Vec2					DefaultActivateSize;	
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;					
		char					pad2[0x4];
		VecCol					Color;					
		int						LeftPaddign;			
		char					pad3[0x4];
		PHotkeyInfo				Info;					
		bool					SettingKey;				
		char					pad4;
		wchar_t					KeyText[0x5];			
		PHotkeyPop				Popup;					
	};

	
	typedef struct Button : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;				
		Vec2					Size;				
		Vec2					DefSize;			
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;				
		char					pad2[0x4];
		VecCol					Color;				
		int						LeftPaddign;		
		char					pad3[0xC];
	} *PButton;

	struct ColorPopup
	{
		int		XAxis;			
		Vec2	Size;			
		Vec2	TintPos;		
		Vec2	TintSize;		
		Vec2	HuePos;			
		Vec2	HueSize;		
		Vec2	AlphaPos;		
		Vec2	AlphaSize;		
	};

	typedef struct
	{
		float	Hue;
		float	Saturation;
		float	Value;
	} HSV_t, HSB;

	
	typedef struct ColorPicker : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;			
		Vec2					Size;			
		Vec2					DefSize;		
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;			
		char					pad2[0x4];
		VecCol					Color;			
		int						LeftPaddign;	
		char					pad3[0x4];
		VecCol*					Value;			
		char					pad4[0x4];
		ColorPopup				Popup;			
		char					pad5[0x88];
		PickerStatus			Status;			
		HSB						HSV;			
	} CPicker, *PCPicker;

	
	typedef struct Combobox : IElement
	{
		Header<Child>				Header;
		Vec2						Pos;				
		Vec2						InnerPadding;		
		Vec2						OuterPadding;		
		std::shared_ptr<XorW>		Name;				
		skeetvec<Call>				Calls;				
		char						pad2[0x4];
		VecCol						Color;				
		Vec2						BoxPos;				
		uint32_t*					Value;				
		int							Defheigth;			
		int							heigth;				
		int							DefInteractOffset;	
		int							InteractOffset;		
		bool						Open;				
		bool						Clicked;			
		char						pad3[0x2];
		int							HoveredItem;		
		int							SelectedItem;		
		skeetvec<BoxVars<wchar_t>>	Vars;				
		char						pad4[0x20];
	} *PCombobox;

	struct LuaLabelValue
	{
		int					OnStartup;
		skeetvec<wchar_t>	Name;
	};

	
	typedef struct Label : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;				
		Vec2					OuterPadding;		
		Vec2					DefOuterPadding;	
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;				
		char					pad2[0x4];
		VecCol					Color;				
		int						LeftPaddign;		
		char					pad3[0x4];
		LuaLabelValue* Value;				
	} *PLabel;

	SIZEASSERT(Label, 0x60);

	typedef struct ListboxInfo
	{
		size_t				ScrollPosition;	
		int					SelectedItem;	
		char				pad2[0x4];
		skeetvec<ListboxVar>Items;			
		char				pad3[0x4];
	} ListInfo, *PListInfo;

	
	typedef struct Listbox : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;				
		Vec2					Size;				
		char					pad1[0x8];
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;				
		char					pad3[0x4];
		VecCol					Color;				
		int						LeftPaddign;		
		char					pad4[0x4];
		ListboxVar*				Vlaue;				
		int						ElementSize;		
		PListInfo				PInfo;				
		bool					ScrollbarEnabled;	
		bool					ScrollbarActive;	
		char					pad5[0x2];
		Vec2					ScrollbarPos;		
		int						AbsoluteHeight;		
		char					pad6[0x4];
		bool					SearchPresent;		
		bool					SearchActive;		
		char					pad7[0x2];
		int						DisplayedCount;		
		ListboxInfo				Info;				
		skeetvec<wchar_t>		Search;				
		char					pad8[0x4];
		skeetvec<short>			SSpecs;				
		char					pad9[0x4];
	} *PListbox;

	
	typedef struct Textbox : IElement
	{
		Header<Child>			Header;
		Vec2					Pos;				
		Vec2					Size;				
		char					pad1[0x8];
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;				
		char					pad3[0x4];
		VecCol					Color;				
		int						LeftPaddign;		
		char					pad4[0xC];
		int						Length;				
		wchar_t					Text[0x40];			
	} *PTextbox;

	typedef struct Inspector
	{
		void**					Vtable;			
		char					pad1[0x15];
		UiType					Type;			
		Vec2					Pos;			
		Vec2					Size;			
		Vec2					Padding;		
		std::shared_ptr<XorW>	Name;			
		skeetvec<Call>			Calls;			
		char					pad4[0x10];
		void*					value;			
	} *PInspector;

	typedef struct CWidg
	{
		char	pad1[0x4];
		HANDLE	SWRLOCK;	
		bool	Modifiable;	
		bool	Movable;	
		char	pad2[0x2];
		VecCol	NameCol;	
		VecCol	BorderCol;	
		VecCol	ShadowCol;	
		VecCol	BackCol;	
		int		DrawName;	
	} *PCWidg;

	class __declspec(novtable) IChild
	{
		virtual void fn0() = 0;
		virtual void fn1() = 0;
		virtual void fn2() = 0;
		virtual void fn3() = 0;
		virtual void fn4() = 0;
		virtual void OnConfigLoad() = 0;
		virtual void fn6() = 0;
		virtual void fn7() = 0;
	public:
		virtual void CalculateLayout() = 0;
	};

	
	struct Child : public IChild
	{
		Header<CTab>			Header;
		Vec2					Pos;				
		Vec2					Size;				
		Vec2					DefSize;			
		std::shared_ptr<XorW>	Name;				
		skeetvec<Call>			Calls;				
		char					pad1[0x4];
		VecCol					Color;				
		char					pad2[0x14];
		Vec2					Padding;			
		Vec4_8t					PosSize;			
		Vec2					MouseLastPos;		
		ChildStatus				Status;				
		skeetvec<PElement>		Elements;			
		char					pad3[0x4];
		PCWidg					PWidgets;			
		char					pad4[0x4];
		Vec2					CurSize;			
		Vec2					MinimizedCapacity;	
		CWidg					Widgets;			
	};

	struct Element
	{
		template<typename T>
		T* GetAs() { return reinterpret_cast<T*>(this); };
	};

	typedef struct
	{
		char			pad4[0x4];
		int				TextureId;		
		int				TextureOffset;	
		char			pad5[0x4];
		Vec2			Size;			
	} TabIcon;

	class __declspec(novtable) ITab : public IElement
	{
	public:
		virtual void OnOpen() = 0;
		virtual void ResetLayout() = 0;
	};

	typedef struct TabActivies
	{
		int LastActiveChild;		
		int LastActiveElementIndex;	
		int LastActiveElementId;	
	};

	struct CTab : public ITab
	{
		Header<void>			Header;
		Vec2					Pos;				
		Vec2					Size;				
		Vec2					DefSize;			
		std::shared_ptr<XorW>	Name;				
		char					pad1[0x10];
		VecCol					Color;				
		char					pad2[0xC];
		PCMenu					Menu;				
		TabActivies				InteractionInfo;	
		skeetvec<PChild>		Childs;				
		TabIcon					Icon;				
		int						Padding;			
		skeetvec<uint8_t>		Chunk;				
		PElement				CEs[0x20];			
	};

	
	typedef struct
	{
		uint64_t			TimeStamp;	
		int					OnStartup;	
		skeetvec<wchar_t>	Name;		
		char				pad2[0x8];
	} LuaInfo;

	struct CloudLua
	{
		uint64_t			TimeStamp;	
		int					Loaded;		
		skeetvec<char>		Name;		
		char				pad[0x4];
		skeetvec<uint8_t>	Data;		
	};

	struct CMenu
	{
		void**			Vtable;				
		PCMenu			NextMenu;			
		char			pad1[0x4];			
		Vec2			Pos;				
		Vec2			Size;				
		Vec2			MinSize;			
		Vec2			TabStartPadding;	
		Vec2			TabEndPadding;		
		uint32_t		MenuStatus;			
		MouseInfo		Mouse;				
		char			pad2[0x8];
		skeetvec<PCTab>	Tabs;				
		char			pad3[0x4];
		uint32_t		CurrentTab;			
		char			pad4[0x20];
		unsigned int	MenuFadeTickCount;	
		FadeType		MenuFadeType;		
		char			pad5[0x4];
		void*			SomeVT;				
		int				SomeInt;			
		char			pad6[0x14];
		float			MenuAlpha;			
		PCTab			TabsArr[9];			
		char			pad7[0x4];
		int				BackgroundTextureId;
		wchar_t			RegValueName[0xC];	
		char			pad8[0x28];			
		uint32_t		OnStartupHash[128]; 
	};

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
		skeetvec<char> key;
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
		RBMapNode* leftmost;	
		RBMapNode* root;		
		RBMapNode* rightmost;	
		RBMapNode* header;		
		char		pad[0x4];
		size_t		count;		
	};

	struct SLua
	{
		void*	LuaState;		
		char	pad2[0x10];
		RBTree	LoadedLuaTree;	
	};

	typedef struct CLua
	{
		char	pad1[0x44];
		SLua	state;
	} *PCLua, **PCLuas;

#pragma pack(pop)

	class IRenderer
	{
	private:
		virtual void fn0() = 0;
		virtual bool fn1() = 0; 
		virtual void fn2() = 0; 
		virtual void _guard_check_icall_nop3() = 0; 
	public:
		virtual void Rect(int x, int y, int h, int w, int color) = 0;
	private:
		virtual void fn5() = 0;
	public:
		virtual void GradientV(int x, int y, int h, int w, int color1, int color2) = 0;
		virtual void GradientH(int x, int y, int h, int w, int color1, int color2) = 0;
		virtual void Line(int x1, int y1, int x2, int y2, int color) = 0;
	private:
		virtual void fn9() = 0;
	public:
		virtual void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) = 0;
		virtual void __vectorcall Circle(int x, int y, int color, float rad, float rot, float percent) = 0;
		virtual void __vectorcall CircleOut(int x, int y, int color, float rad, float rot, float percent, float thick) = 0;
	private:
		
		virtual void __fastcall PolyCircle(void* points, size_t count) = 0;
		virtual void fn14() = 0;
	public:
		virtual void __fastcall Text(int flags, int x, int y, int color, size_t maxlen, const wchar_t* text, size_t len) = 0;
	private:
		virtual void fn16() = 0;
	public:
		virtual void __fastcall MeasureText(int* out, const wchar_t* text, size_t len, int flags) = 0;
		virtual void __fastcall ScreenSize(int* w, int* h) = 0;
		virtual int __fastcall AddTexture(unsigned char* data, int w, int h, size_t bytesize, int i0, int i1) = 0;
	private:
		virtual void fn20() = 0;
		virtual void fn21() = 0;
		virtual void _guard_check_icall_nop22() = 0; 
	public:
		virtual void __vectorcall Texture(int id, int x, int y, int color, int offset, int i0, int w, int h, int flag, float scale) = 0;
		virtual void __fastcall SetBounds(int bounds[4]) = 0;
		virtual void __fastcall GetBounds(int bounds[4]) = 0;
		virtual void __fastcall ResetBounds(bool reset) = 0;
		virtual bool IsBoundsPresent() = 0;	
		virtual void __vectorcall SetAlpha(float alpha) = 0;
		virtual void __vectorcall SetZBuff(float z) = 0; 
		virtual float GetZBuff() = 0; 
	private:
		virtual void Monochrome(bool state) = 0; 
		virtual void fn32() = 0;
		virtual float __vectorcall GetScale() = 0; 
		virtual int __fastcall fn34(int i) = 0; 
		virtual void fn35() = 0;
		virtual void fn36() = 0;
		virtual void fn37() = 0;
		virtual void __fastcall PolyCircle_Q(int precision_Q, void* points, size_t count) = 0;
		virtual float __vectorcall fn39() = 0;
		virtual void __fastcall fn40(int, int*, int*) = 0;
	public:
		virtual void __fastcall RectOut(int thick, int x, int y, int w, int h, int color) = 0;
		virtual void __fastcall RectOutlined(int thick, int x, int y, int w, int h, int colorin, int colorout) = 0;
	};
	struct SkeetClass_
	{
		IRenderer* IRenderer;
		void* DefSubclassProc;
		void* RemoveWindowSubclass;
		void* SetWindowSubclass;
		PCMenu Menu;
		void* ThreadInMainThread;
	};


	static SkeetClass_* SCLASS = nullptr;
	static PCMenu Menu = nullptr;

	class Hasher final
	{
	public:
		using hash_t = uint32_t;
		using u8str = const char*;
		using u16str = const wchar_t*;

		static constexpr hash_t __fastcall FNV1a(u8str str);
		static constexpr hash_t __fastcall FNV1a(u16str str);
		static constexpr hash_t __fastcall FNV1a(u8str str, size_t len);

		static constexpr hash_t __fastcall FNV1al(u8str str);
		static constexpr hash_t __fastcall FNV1al(u16str str);
		static constexpr hash_t __fastcall FNV1al(u8str str, size_t len);
		static constexpr hash_t __fastcall FNV1al(u16str str, size_t len);
		static constexpr hash_t __fastcall FNV1al_s(u16str str, size_t len);
	};

	class ConfigSystem final
	{
		friend void InitAndWaitForSkeet();
	MEMBERS_PRIVATE:
		static GetConfigDataFn	GetConfigData;
		static ThisFn			AfterTabRage;
		static Hasher::hash_t*	CurrentConfigHash;
	MEMBERS_PUBLIC:
		static LoadConfigFn		_LoadConfig;
		static const wchar_t* CurrentConfigName();
		static bool LoadConfig(const char* name);
		static FLATT bool LoadConfig(Hasher::hash_t hash);
	};

	class LuaSystem final
	{
		friend void InitAndWaitForSkeet();
	MEMBERS_PRIVATE:
		static CFn			_GetOrInitLuaState;
		static CFn			_GetSLua;
		static LoadLuaFn	LoadLua;
		static BTFn			IsLuaLoaded;
		static ThisFn		UpdateVisualLuaInfo;
		static PCLuas		ll_State;
	MEMBERS_PUBLIC:
		static size_t	LuaCount();
		static void*	GetOrInitLuaState();
		static void*	GetLuaState();
		static SLua*	GetSLua();
		static PCLua	GetLuaLoaderState();

		static const wchar_t* LuaName(Hasher::hash_t hash);
		static bool		LoadScript(const wchar_t* name, bool on_startup);
		static bool		LoadScript(Hasher::hash_t hash, bool on_startup);
	};

	class Utils final
	{
		friend void InitAndWaitForSkeet();
		friend class UI;
	MEMBERS_PRIVATE:
		static DecryptFn		Decrypt;
		static CryptFn			Crypt;
		static ThisFn			Callback;
	MEMBERS_PUBLIC:
		static void			AllowUnsafe(int value);
		static XorW*		CryptName(const wchar_t* name);
		static void			ForEach(PChild child, void(*func)(PElement), UiType T = UiNone);
		static void			InitConfig();
		static unsigned int ChildsCount(PCTab tab);
		static unsigned int ElementsCount(PChild child);
	};

	class UI final
	{
		friend void InitAndWaitForSkeet();
		friend void load_settings();
	MEMBERS_PRIVATE:
		static ThisIntFn		TabSwitch;
		static ThisIntFn		SetList;
		static ThisFn			ChildLayout;
		static ThisFn			KeyInit;
		static SetKeyFn			SetKey;
		static SetCheckFn		SetCheck;
		static HideUiFn			HideUi;
		static ThisFn			DeleteUi;
		static T2PFn			InsertTab;
		static T2PFn			InsertChildr;
		static InsertFn			InsertElem;
		static AddLabelFn		AddLabel;
		static AddButtonFn		AddButton;
		static LConFn			LabelCon;
		static BConFn			ButtonCon;
		static TBConFn			TextboxCon;
		static SConFn			SliderCon;
		static HKConFn			HotkeyCon;
		static CPConFn			CPickerCon;
		static CBConFn			ComboboxCon;
		static MConFn			MultiCon;
		static LBConFn			ListboxCon;
		static TCConFn			TabCon;
		static GetTBoxTextFn	GetTBoxText;
		static T2PFn			SetListboxItems;
	MEMBERS_PUBLIC:
		static TCConFn			CheckboxCon;
		static CHConFn			ChildCon;
		static void			ResetLayout();
		static void			SetTab(uint32_t idx);
		static PCTab		GetTab(ETab tab);
		static uint32_t		GetActiveTab();
		static PCTab		GetActiveCTab();
		static void			SetVisible(PElement element, bool value);
		static VecCol		GetMenuCol();
		static void			SetMenuCol(VecCol col);
		static void			SetInput(PElement elemen, bool val);
		static void*		TieValue(PElement element, void* ptr);
		static void			SetCallback(PElement elem, F2PFn func, void* ecx = NULL);
		static void			DeleteElement(PElement element);
		static void			SetCheckbox(PCheckbox checkbox, bool value);
		static void			SetColorRGBA(PCPicker picker, VecCol Color);
		static void			SetColorHSV(PCPicker picker, HSB Color);
		static void			SetSlider(PSlider slider, int value);
		static void			SetCombobox(PCombobox combobox, uint32_t value);
		static void			SetListbox(PListbox listbox, int value);
		static void			SetHotkey(PHotkey hotkey, int key, HotkeyMode mode = Inherit);
		static wchar_t*		GetName(PElement element);
		static size_t		GetTextbotText(PTextbox tbox, wchar_t* buffer);
		static void			InsertElement(PChild child, void* element);
		static PLabel		CreateLabel(PChild child, const wchar_t* name);
		static PTextbox		CreateTextbox(PChild child, bool sameline = true, const wchar_t* name = NULL);
		static PCheckbox	CreateCheckbox(PChild child, const wchar_t* name, int* value);
		static PSlider		CreateSlider(PChild child, const wchar_t* name, int min, int max, int* value, bool tooltip = true, wchar_t* tip = NULL, float step = 1.f);
		static PHotkey		CreateHotkey(PChild child, const wchar_t* name, bool sameline = false, int vkey = 0x00, HotkeyMode mode = OnHotkey);
		static PButton		CreateButton(PChild child, const wchar_t* name, F2PFn function, void* ecx = Menu);
		static PCPicker		CreateColorPicker(PChild child, VecCol* value, bool sameline = true, wchar_t* name = NULL);
		static PCombobox	CreateCombobox(PChild child, const wchar_t* name, int* value, wchar_t** arr, size_t arrsize, bool sameline = false);
		static PMultiselect CreateMultiselect(PChild child, const wchar_t* name, int* value, char** arr, size_t arrsize, bool sameline = false);
		static PListbox		CreateListbox(PChild child, const wchar_t* name, int* value, wchar_t** arr, size_t arrsize, bool searchbox = true);
		static void			InsertChild(PCTab tab, void* child);
		static PChild		CreateChild(PCTab tab, const wchar_t* name, Vec4_8t possize, bool modify);
		static PCTab		CreateTab(const wchar_t* name, Vec2 pos);

		static FORCECALL PElement		GetByName(PChild child, const wchar_t* name);
		static FORCECALL PChild			GetChild(ETab tab, size_t index);
		template<typename T> static T*	GetElement(PChild, size_t index);

		
		static [[deprecated("Wrong implementation")]] void	RenameElement(PElement element, const wchar_t* name, XorW* oldname);
		static [[deprecated("Wrong implementation")]] void	AddListboxVar(PListbox list, const wchar_t* elem, size_t bsize = 0);
		static [[deprecated("Wrong implementation")]] void	RemoveListboxVar(PListbox list, size_t index);
	};

	class EventListener
	{
		size_t index;
		RenderEventType type;
		bool status;
	public:
		EventListener(size_t index, RenderEventType type) : index(index), type(type), status(true) {};
		~EventListener() { remove(); };
		void remove();
	};

	struct CTexture
	{
		int id;
		TextureType type;
		Vec2 Size;
		CTexture() = default;
		CTexture(int id, TextureType type, int width, int heigth) : id(id), type(type), Size(width, heigth) {};
	};

	class Renderer final
	{
	MEMBERS_PRIVATE:
		static sVec<RenderEventListenerFn> RenderEvents;
		static sVec<RenderEventListenerFn> MenuEvents;
		static sVec<RenderEventListenerFn> FinalEvents;
		static Memory::CHooked* RenderHook;
		static Memory::CHooked* MenuRenderHook;
		static Memory::CHooked* FinalRenderHook;
		static ReadFileFn ExReadFile;
		static LoadTextureFn LoadTexture;
		static LoadSvgFormFileFn LoadSvgFromFile;
		static RenderIndicatorFn RenderIndicator;
		static WorldToScreenFn WorldToScreen;
		static RenderBlurFn RenderBlur;
		static void* RenderBlurCtx;
		static void __fastcall RenderListener(void* ecx, void* edx);
		static void __fastcall MenuRenderListener(void* ecx, void* edx);
		static void __fastcall RenderFinalListener(void* ecx, void* edx);
		static void RemoveEvent(size_t index, RenderEventType type);
		static void LoadTextureFromFile(CTexture& texture, const char* filename, TextureType type);
		friend class EventListener;
	MEMBERS_PUBLIC:
		static void Init();
		static void Term();
		static EventListener* AddEvent(RenderEventType type, RenderEventListenerFn event);
		static Vec2 ScreenSize();
		static int ScreenWidth();
		static int ScreenHeigth();
		static Vec2 MeasureText(const wchar_t* text, unsigned int flags);
		static void ToScreen(Vec3f_t pos, int* x, int* y);
		static void Text(Vec2 pos, VecCol color, const wchar_t* text, int flags, unsigned int maxlen = 0);
		static void Line(Vec2 p1, Vec2 p2, VecCol color);
		static void Rect(Vec2 pos, Vec2 size, VecCol color);
		static void RectOut(Vec2 pos, Vec2 size, VecCol color, int thickness);
		static void OutlinedRect(Vec2 pos, Vec2 size, VecCol colorin, VecCol colorout, int thickness);
		static void Gradient(Vec2 pos, Vec2 size, VecCol color1, VecCol color2, bool horizontal = true);
		static void Blur(Vec2 pos, Vec2 size);
		static void Triangle(Vec2 p1, Vec2 p2, Vec2 p3, VecCol color);
		static void Indicator(int y, VecCol color, const char* text, int texureid = -1, float circlepercent = -1.f);
		static void __vectorcall Circle(Vec2 pos, VecCol color, float radius, float rotation = 0.f, float percentage = 1.f);
		static void __vectorcall CircleOut(Vec2 pos, VecCol color, float radius, float rotation = 0.f, float percentage = 1.f, float thickness = 1.f);
		static void Texture(int id, Vec2 pos, Vec2 size, VecCol color, int flag, int offset = 0, float scale = 1.f);
		static void LoadSVGTextureFromFile(CTexture& texture, const char* filename, int width = 0, int heigth = 0);
		static FORCECALL void LoadPNGTextureFromFile(CTexture& texture, const char* filename);
		static FORCECALL void LoadJPGTextureFromFile(CTexture& texture, const char* filename);
		static FORCECALL void LoadTextureFromMemory(CTexture& texture, const unsigned char* data, size_t size, TextureType type, int width, int heigth);
	};

#if defined(SDK_GLOBALS_IMP) || defined(SDK_CLIENT_IMP)
	struct CommandsInfo
	{
		unsigned int	LastOutCommand;	
		unsigned int	ChokedCommands;	
		unsigned int	OldCommandAck;	
		char			pad1[0x4];
		unsigned int	CommandAck;		
	};

	class CEngine final
	{
		friend void InitAndWaitForSkeet();
		friend class Globals;
		friend class Client;
		static void**** EngineCtx;
		static CommandsInfo** CommandsCtx;
		static SetClanTagFn SetTag;
		static unsigned int* PredComm;
		static bool IsInGame();
		static char* MapName();
		static void ExecCommand(const char* command);
		static void SetClanTag(const char* tag);
	};
#endif
	struct GlobalsInfo
	{
		float			RealTime;		
		int				FrameCount;		
		float			AbsFrameTime;	
		char			pad1[0x4];
		float			CurrentTime;	
		float			FrameTime;		
		int				MaxPlayers;		
		unsigned int	TickCount;		
		float			TickInterval;	
	};

	class Globals final
	{
		friend void InitAndWaitForSkeet();
	MEMBERS_PRIVATE:
		static GlobalsInfo** GlobalsCtx;
	MEMBERS_PUBLIC:
		static bool			IsInGame();
		static char*		MapName();
		static float		RealTime();
		static float		FrameTime(bool abs = false);
		static int			FrameCount();
		static float		CurrentTime();
		static float		TickInterval();
		static unsigned int	TickCount();
		static int			MaxPlayers();
		static unsigned int	LastOutCommand();
		static unsigned int	ChokedCommands();
		static unsigned int	OldCommandAck();
		static unsigned int	CommandAck();
	};

	class Client final
	{
		friend void InitAndWaitForSkeet();
	MEMBERS_PRIVATE:
		static LogFn Logger;
		static LogErrorFn LoggerError;
		static void*** LoggerCtx;
		static ScreenLogFn ScreenLogger;
	MEMBERS_PUBLIC:
		static void Exec(const char* cmd);
		static void SetClanTag(const char* tag);
		static void LogScreen(const char* text, VecCol color);
		static FORCECALL void Log(const char* fmt, ...);
		static FORCECALL void LogColor(VecCol Color, const char* fmt, ...);
		static FORCECALL void LogError(const char* fmt, ...);
	};	
} 



namespace SkeetSDK
{
	
	namespace Memory
	{
		template <typename T>
		T GetChunkAs(PCTab tab)
		{
			return reinterpret_cast<T>(tab->Chunk.data());
		};

		template <typename T>
		T GetVFunc(PElement ptr, size_t index)
		{
			return reinterpret_cast<T>(ptr->GetAs<Inspector>()->Vtable[index]);
		};

		template <typename T>
		T VtableBind(void** table, size_t index)
		{
			return reinterpret_cast<T>(table[index]);
		};

		HMODULE WaitForSingleHandle(const char* module)
		{
			HMODULE hmod = GetModuleHandleA(module);
			while (!hmod)
			{
				hmod = GetModuleHandleA(module);
				Sleep(100);
			};
			return hmod;
		}

		
		SigFinder::SigFinder(const char* module)
		{
			info = { 0 };
			GetModuleInformation(GetCurrentProcess(), WaitForSingleHandle(module), &info, sizeof(MODULEINFO));
		};

		SigFinder::SigFinder(LPVOID lpBaseAdress, DWORD SizeOfImage)
		{
			info = { 0 };
			info.lpBaseOfDll = lpBaseAdress;
			info.SizeOfImage = SizeOfImage;
		};

		sVec<short> SigFinder::ida_sig_resolver(const char* sig)
		{
			size_t siglen = strlen(sig);
			sVec<short> vec((siglen + 1) / 3);
			for (size_t i = 0; i < siglen; i += 3)
			{
				if (sig[i] == '?' && sig[i + 1] == '?')
				{
					vec.push_back(-1);
					continue;
				};
				if (sig[i] >= 65 && sig[i] <= 90)
				{
					vec.push_back((sig[i] - 55) << 4);
				}
				else
				{
					vec.push_back((sig[i] - '0') << 4);
				};
				if (sig[i + 1] >= 65 && sig[i + 1] <= 90)
				{
					vec.last() |= (sig[i + 1] - 55);
				}
				else
				{
					vec.last() |= (sig[i + 1] - '0');
				};
			};
			return vec;
		};

		void* SigFinder::find(const char* sig, int offset)
		{
			sVec<short> pattern = ida_sig_resolver(sig);
			for (size_t i = 0; i < info.SizeOfImage; i++)
			{
				for (size_t j = 0; j < pattern.len(); j++)
				{
					if (pattern[j] != -1 && pattern[j] != reinterpret_cast<unsigned char*>(info.lpBaseOfDll)[i + j]) break;
					if (j + 1 == pattern.len())
					{
						return reinterpret_cast<unsigned char*>(info.lpBaseOfDll) + i + offset;
					};
				};
			}
			return NULL;
		};

		void* SigFinder::findr32(const char* sig)
		{
			sVec<short> pattern = ida_sig_resolver(sig);
			if (pattern[0] != 0xE8 && pattern[0] != 0xE9) return NULL;
			for (size_t i = 0; i < info.SizeOfImage; i++)
			{
				for (size_t j = 0; j < pattern.len(); j++)
				{
					if (pattern[j] != -1 && pattern[j] != reinterpret_cast<unsigned char*>(info.lpBaseOfDll)[i + j]) break;
					if (j + 1 == pattern.len())
					{
						return reinterpret_cast<unsigned char*>(info.lpBaseOfDll) + i + 5 + *(int*)((unsigned char*)info.lpBaseOfDll + i + 1);
					};
				};
			}
			return NULL;
		};

		
		CHooked::CHooked(void* func, unsigned char* ogbytes, size_t size)
		{
			Address = func;

			OriginalBytes = ogbytes;

			BytesSize = size;
			Status = true;

			void* heap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BytesSize + 5);
			DWORD OldProto{ 0 };
			VirtualProtect(heap, BytesSize + 5, PAGE_EXECUTE_READWRITE, &OldProto);
			memcpy(heap, OriginalBytes, BytesSize);
			*(char*)((char*)heap + BytesSize) = 0xE9;
			*(int*)((char*)heap + BytesSize + 1) = (int)Address - ((int)heap + (int)BytesSize);
			naked = heap;
		};

		CHooked::~CHooked()
		{
			Unhook();
			free(OriginalBytes);
			HeapFree(GetProcessHeap(), 0, naked);
		};

		void* CHooked::Naked()
		{
			return naked;
		};

		void CHooked::Unhook()
		{
			if (!Status) return;
			memcpy(Address, OriginalBytes, BytesSize);
			Status = false;
		};

		
		CHooked* DetourHook::Hook(void* Dst, void* Src, size_t size, JuncType junc)
		{
			ptrdiff_t rel32 = (int)Src - (int)Dst - 5;
			DWORD OldProto{ 0 };
			unsigned char* ogbytes = (unsigned char*)malloc(size);
			VirtualProtect(Dst, size, PAGE_EXECUTE_READWRITE, &OldProto);
			memcpy(ogbytes, Dst, size);
			*(BYTE*)Dst = junc;
			*(int*)((BYTE*)Dst + 1) = rel32;
			if (size - 5 > 0)
				memset((char*)Dst + 5, 0x90, size - 5);
			VirtualProtect(Dst, size, OldProto, &OldProto);
			CHooked* hook = new CHooked(Dst, ogbytes, size);
			Hooked.push_back(hook);
			return hook;
		};

		void DetourHook::UnhookAll()
		{
			for (size_t i = 0; i < Hooked.len(); i++)
				Hooked[i]->Unhook();
		};

		sVec<CHooked*> DetourHook::Hooked = { 20 };
	}; 

	
	constexpr Hasher::hash_t Hasher::FNV1a(u8str str)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str)
		{
			while (1)
			{
				char ch = *str;
				if (!*str)
					break;
				++str;
				hash = SKEET_FNV1A_PRIME * (hash ^ ch);
			}
		}
		return hash ^ SKEET_XOR_KEY;
	}

	constexpr Hasher::hash_t Hasher::FNV1a(u16str str)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str)
		{
			wchar_t ch = *str;
			if (*str)
			{
				do
				{
					++str;
					hash = SKEET_FNV1A_PRIME * (hash ^ ch);
					ch = *str;
				} while (*str);
			}
		}
		return hash ^ SKEET_XOR_KEY;
	};

	constexpr Hasher::hash_t Hasher::FNV1a(u8str str, size_t len)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str && len)
		{
			do
			{
				char ch = *str++;
				hash = SKEET_FNV1A_PRIME * (hash ^ ch);
				--len;
			} while (len);
		}
		return hash ^ SKEET_XOR_KEY;
	};

	constexpr Hasher::hash_t Hasher::FNV1al(u8str str)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str)
		{
			while (1)
			{
				char ch = tolower(*str);
				if (!*str)
					break;
				++str;
				hash = SKEET_FNV1A_PRIME * (hash ^ ch);
			}
		}
		return hash ^ SKEET_XOR_KEY;
	};

	constexpr Hasher::hash_t Hasher::FNV1al(u16str str)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str)
		{
			wchar_t ch = *str;
			if (*str)
			{
				do
				{
					++str;
					ch = towlower(ch);
					hash = SKEET_FNV1A_PRIME * (hash ^ ch);
					ch = *str;
				} while (*str);
			}
		}
		return hash ^ SKEET_XOR_KEY;
	};

	constexpr Hasher::hash_t Hasher::FNV1al(u8str str, size_t len)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str)
		{
			while (len)
			{
				char ch = *str;
				--len;
				++str;
				ch = tolower(ch);
				hash = SKEET_FNV1A_PRIME * (hash ^ ch);
			}
		}
		return hash ^ SKEET_XOR_KEY;
	};

	constexpr Hasher::hash_t Hasher::FNV1al(u16str str, size_t len)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str && len)
		{
			do
			{
				wchar_t ch = towlower(*str++);
				hash = SKEET_FNV1A_PRIME * (hash ^ ch);
				--len;
			} while (len);
		}
		return hash ^ SKEET_XOR_KEY;
	};

	constexpr Hasher::hash_t Hasher::FNV1al_s(u16str str, size_t len)
	{
		hash_t hash = SKEET_FNV1A_BASE;
		if (str)
		{
			while (1)
			{
				wchar_t ch = towlower(*str);
				if (!*str)
					break;
				if (!len--)
					break;
				++str;
				hash = SKEET_FNV1A_PRIME * (hash ^ ch);
			}
		}
		return hash ^ SKEET_XOR_KEY;
	};

	
	GetConfigDataFn ConfigSystem::GetConfigData		= nullptr;
	LoadConfigFn	ConfigSystem::_LoadConfig		= nullptr;
	ThisFn			ConfigSystem::AfterTabRage		= nullptr;
	Hasher::hash_t*	ConfigSystem::CurrentConfigHash = nullptr;

	const wchar_t* ConfigSystem::CurrentConfigName()
	{
		if (!*CurrentConfigHash) return nullptr;
		PListbox ptr = Menu->Tabs[CONFIG]->Childs[0]->Elements[0]->GetAs<Listbox>();
		for (auto& item : ptr->Info.Items)
		{
			if (Hasher::FNV1a(item.Name.data()) == *CurrentConfigHash)
				return item.Name.data();
		}
	};

	bool ConfigSystem::LoadConfig(const char* name)
	{
		skeetvec<unsigned char> cfgchunk;
		GetConfigData(name, &cfgchunk);
		if (cfgchunk.size())
		{
			*CurrentConfigHash = Hasher::FNV1a(name);
			_LoadConfig(Menu, cfgchunk.data(), cfgchunk.size());
			for (auto& cfg : Menu->TabsArr[CONFIG]->CEs[3]->GetAs<Listbox>()->Info.Items)
			{
				if (Hasher::FNV1a(cfg.Name.data()) == *CurrentConfigHash)
					cfg.Index |= LBOX_ACTIVE_FLAG;
			};
			AfterTabRage(UI::GetTab(RAGE));
			return true;
		};
		return false;
	};

	bool ConfigSystem::LoadConfig(Hasher::hash_t hash)
	{
		for (auto& cfg : Menu->TabsArr[CONFIG]->CEs[3]->GetAs<Listbox>()->Info.Items)
		{
			if (Hasher::FNV1a(cfg.Name.data()) == *CurrentConfigHash)
			{
				char u8name[_MAX_FNAME];
				int count = WideCharToMultiByte(CP_UTF8, 0, cfg.Name.data(), -1, u8name, sizeof(u8name), 0, 0);
				if (count > 0)
					return LoadConfig(u8name);
			}
		};
		return false;
	};

	
	CFn			LuaSystem::_GetOrInitLuaState	= nullptr;
	CFn			LuaSystem::_GetSLua				= nullptr;
	LoadLuaFn	LuaSystem::LoadLua				= nullptr;
	BTFn		LuaSystem::IsLuaLoaded			= nullptr;
	ThisFn		LuaSystem::UpdateVisualLuaInfo	= nullptr;
	PCLuas		LuaSystem::ll_State				= nullptr;

	size_t LuaSystem::LuaCount()
	{
		return Menu->Tabs[CONFIG]->Chunk.size() / sizeof(LuaInfo);
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
		return *ll_State;
	};

	const wchar_t* LuaSystem::LuaName(Hasher::hash_t hash)
	{
		LuaInfo* luas = Memory::GetChunkAs<LuaInfo*>(Menu->Tabs[CONFIG]);

		for (int i = 0; i < LuaSystem::LuaCount(); i++)
		{
			if (Hasher::FNV1a(luas[i].Name.data()) == hash)
				return luas[i].Name.data();
		};

		return nullptr;
	};

	bool LuaSystem::LoadScript(const wchar_t* name, bool on_startup)
	{
		PCTab config = UI::GetTab(CONFIG);
		PListbox scripts = UI::GetChild(CONFIG, 1)->Elements[3]->GetAs<Listbox>();
		LuaInfo* chunk = Memory::GetChunkAs<LuaInfo*>(config);
		ListboxVar* var = nullptr;

		for (size_t i = 0; i < LuaCount(); i++)
		{
			if (!wcscmp(chunk[i].Name.data(), name))
			{
				chunk = &chunk[i];
				var = &scripts->Info.Items[i];
				break;
			};
		};

		if (var == nullptr) return false;

		bool result = LoadLua(chunk->Name.data());
		if (result && GetLuaState() != nullptr && IsLuaLoaded(&chunk->Name))
		{
			var->Index |= LBOX_ACTIVE_FLAG;
			chunk->OnStartup = on_startup;
		}
		UpdateVisualLuaInfo(config);
		return result;
	};

	bool LuaSystem::LoadScript(Hasher::hash_t hash, bool on_startup)
	{
		PCTab config = UI::GetTab(CONFIG);
		PListbox scripts = UI::GetChild(CONFIG, 1)->Elements[3]->GetAs<Listbox>();
		LuaInfo* chunk = Memory::GetChunkAs<LuaInfo*>(config);
		ListboxVar* var = nullptr;

		for (size_t i = 0; i < LuaCount(); i++)
		{
			if (Hasher::FNV1a(chunk[i].Name.data()) == hash)
			{
				chunk = &chunk[i];
				var = &scripts->Info.Items[i];
				break;
			};
		};

		if (var == nullptr) return false;

		bool result = LoadLua(chunk->Name.data());
		if (result && GetLuaState() != nullptr && IsLuaLoaded(&chunk->Name))
		{
			var->Index |= LBOX_ACTIVE_FLAG;
			chunk->OnStartup = on_startup;
		}
		UpdateVisualLuaInfo(config);
		return result;
	}

	
	DecryptFn		Utils::Decrypt			= nullptr;
	CryptFn			Utils::Crypt			= nullptr;
	ThisFn			Utils::Callback			= nullptr;


	unsigned int Utils::ChildsCount(PCTab tab)
	{
		return tab->Childs.size();
	};

	unsigned int Utils::ElementsCount(PChild child)
	{
		return child->Elements.size();
	};

	void Utils::InitConfig()
	{
		Menu->Tabs[CONFIG]->OnOpen();
	};

	void Utils::AllowUnsafe(int value)
	{
		UI::SetCheckbox(Menu->Tabs[CONFIG]->Childs[1]->Elements[1]->GetAs<Checkbox>(), value);
	};

	XorW* Utils::CryptName(const wchar_t* name)
	{
		return (XorW*)Crypt(name, (wcslen(name) + 1) * sizeof(wchar_t), 2);
	};


	void Utils::ForEach(PChild child, void(*func)(PElement), UiType T)
	{
		
		for (auto& element : child->Elements)
		{
			if (T == UiNone || element->GetAs<Header<void>>()->Type == T)
			{
				func(element);
			}
		}
	};

	
	ThisIntFn		UI::TabSwitch		= nullptr;
	ThisIntFn		UI::SetList			= nullptr;
	ThisFn			UI::ChildLayout		= nullptr;
	ThisFn			UI::KeyInit			= nullptr;
	SetKeyFn		UI::SetKey			= nullptr;
	SetCheckFn		UI::SetCheck		= nullptr;
	HideUiFn		UI::HideUi			= nullptr;
	ThisFn			UI::DeleteUi		= nullptr;
	T2PFn			UI::InsertTab		= nullptr;
	T2PFn			UI::InsertChildr	= nullptr;
	InsertFn		UI::InsertElem		= nullptr;
	AddLabelFn		UI::AddLabel		= nullptr;
	AddButtonFn		UI::AddButton		= nullptr;
	LConFn			UI::LabelCon		= nullptr;
	BConFn			UI::ButtonCon		= nullptr;
	TBConFn			UI::TextboxCon		= nullptr;
	TCConFn			UI::CheckboxCon		= nullptr;
	SConFn			UI::SliderCon		= nullptr;
	HKConFn			UI::HotkeyCon		= nullptr;
	CPConFn			UI::CPickerCon		= nullptr;
	CBConFn			UI::ComboboxCon		= nullptr;
	MConFn			UI::MultiCon		= nullptr;
	LBConFn			UI::ListboxCon		= nullptr;
	CHConFn			UI::ChildCon		= nullptr;
	TCConFn			UI::TabCon			= nullptr;
	GetTBoxTextFn	UI::GetTBoxText		= nullptr;
	T2PFn			UI::SetListboxItems	= nullptr;

	void UI::ResetLayout()
	{
		Button* b = GetChild(MISC, 2)->Elements[11]->GetAs<Button>();
		b->Calls[0].function(b->Calls[0].ecx, b);
	};

	void UI::SetTab(uint32_t idx)
	{
		if (Menu->Tabs.size() > idx)
			TabSwitch(Menu, idx);
	};

	PCTab UI::GetTab(ETab tab)
	{
		return Menu->Tabs[tab];
	};

	uint32_t UI::GetActiveTab()
	{
		return Menu->CurrentTab;
	}

	PCTab UI::GetActiveCTab()
	{
		return Menu->Tabs[Menu->CurrentTab];
	}

	void UI::SetVisible(PElement element, bool value)
	{
		HideUi(element, value);
	};

	VecCol UI::GetMenuCol()
	{
		return GetChild(MISC, 2)->Elements[3]->GetAs<CPicker>()->Value[0];
	};

	void UI::SetMenuCol(VecCol col)
	{
		PCPicker menucol = GetChild(MISC, 2)->Elements[3]->GetAs<CPicker>();
		*menucol->Value = col;
		menucol->OnValueSet();
	};

	void UI::SetInput(PElement elemen, bool val)
	{
		Header<Child>* head = elemen->GetAs<Header<Child>>();
		head->Flags.AllowMouseInput = val;
		head->Flags.Editable = val;
	};

	void* UI::TieValue(PElement element, void* ptr)
	{
		void* oldptr = element->GetAs<Inspector>()->value;
		element->GetAs<Inspector>()->value = ptr;
		return oldptr;
	};

	void UI::SetCallback(PElement elem, F2PFn func, void* ecx)
	{
		PInspector ptr = elem->GetAs<Inspector>();
		if (ptr->Calls.empty())
		{
			ptr->Calls.reserve(4);
		};
		ptr->Calls.emplace_back(ecx, func);
	}

	void UI::DeleteElement(PElement element)
	{
		DeleteUi(element);
	};

	void UI::SetCheckbox(PCheckbox checkbox, bool value)
	{
		SetCheck(checkbox, value);
	};

	void UI::SetColorRGBA(PCPicker picker, VecCol Color)
	{
		picker->Value[0] = Color;
		Utils::Callback(picker);
	};

	void UI::SetColorHSV(PCPicker picker, HSB Color)
	{
		picker->HSV = Color;
		Utils::Callback(picker);
	};

	void UI::SetSlider(PSlider slider, int value)
	{
		slider->Value[0] = value;
		Utils::Callback(slider);
	};

	void UI::SetCombobox(PCombobox combobox, uint32_t value)
	{
		*combobox->Value = value;
		combobox->OnValueSet();
	};

	void UI::SetListbox(PListbox listbox, int value)
	{
		SetList(listbox, value);
		Utils::Callback(listbox);
	};

	void UI::SetHotkey(PHotkey hotkey, int key, HotkeyMode mode)
	{
		SetKey(hotkey, key, 0);
		if (mode != Inherit)
			hotkey->Popup->SetMode(hotkey, hotkey->Popup, mode);
	};

	void UI::RenameElement(PElement element, const wchar_t* name, XorW* oldname)
	{
		if (oldname)
			oldname = element->GetAs<Inspector>()->Name.get();
		element->GetAs<Inspector>()->Name = std::shared_ptr<XorW>(Utils::CryptName(name));
	};

	wchar_t* UI::GetName(PElement element)
	{
		static wchar_t name[128];
		memset(name, 0, sizeof(name));
		if (!element || !element->GetAs<Slider>()->Name) return name;
		
		XorW* xname = element->GetAs<Slider>()->Name.get();
		if (!xname) return name;
		
		unsigned short key = xname->key;
		size_t count = xname->bsize / 2;
		if (count > 127) count = 127;
		
		for (size_t i = 0; i < count; ++i)
		{
			wchar_t dec = xname->data[i] ^ key;
			if (dec == L'\0') break;
			name[i] = dec;
		}
		return name;
	};

	size_t UI::GetTextbotText(PTextbox tbox, wchar_t* buffer)
	{
		return GetTBoxText(tbox, buffer);
	};

	PElement UI::GetByName(PChild child, const wchar_t* name)
	{
		for (auto& element : child->Elements)
		{
			wchar_t* elementName = GetName(element);
			if (wcscmp(elementName, name) == 0)
				return element;
		}
		return NULL;
	};

	PChild UI::GetChild(ETab tab, size_t index)
	{
		PCTab tabPtr = GetTab(tab);
		if (index < 0 || index >= Utils::ChildsCount(tabPtr)) return NULL;
		return tabPtr->Childs[index];
	};

	void UI::InsertElement(PChild child, void* element)
	{
		InsertElem(child, 0, element);
		child->CalculateLayout();
	};

	PLabel UI::CreateLabel(PChild child, const wchar_t* name)
	{
		PLabel ptr = (PLabel)AddLabel(child, 0, name);
		child->CalculateLayout();
		return ptr;
	};

	PTextbox UI::CreateTextbox(PChild child, bool sameline, const wchar_t* name)
	{
		if (!sameline && name)
			CreateLabel(child, name);
		PTextbox ptr = (PTextbox)Memory::Allocator(sizeof(Textbox));
		TextboxCon(ptr, child);
		InsertElement(child, ptr);
		return ptr;
	};

	PCheckbox UI::CreateCheckbox(PChild child, const wchar_t* name, int* value)
	{
		PCheckbox ptr = (PCheckbox)Memory::Allocator(sizeof(Checkbox));
		CheckboxCon(ptr, child, name, value);
		InsertElement(child, ptr);
		return ptr;
	};

	PSlider UI::CreateSlider(PChild child, const wchar_t* name, int min, int max, int* value, bool tooltip, wchar_t* tip, float step)
	{
		PSlider ptr = (PSlider)Memory::Allocator(sizeof(Slider));
		unsigned int tipch = 0;
		if (tip != NULL)
			tipch = tip[0] | tip[1] << 16;
		SliderCon(ptr, child, name, min, max, value, tooltip, tipch, step);
		InsertElement(child, ptr);
		return ptr;
	};

	PHotkey UI::CreateHotkey(PChild child, const wchar_t* name, bool sameline, int vkey, HotkeyMode mode)
	{
		if (!sameline)
			CreateLabel(child, name);
		PHotkeyInfo info = (PHotkeyInfo)Memory::Allocator(sizeof(HotkeyInfo));
		KeyInit(info);
		PHotkey ptr = (PHotkey)Memory::Allocator(sizeof(Hotkey));
		HotkeyCon(ptr, child, info, true);
		SetHotkey(ptr, vkey, mode);
		InsertElement(child, ptr);
		return ptr;
	};

	
	PButton UI::CreateButton(PChild child, const wchar_t* name, F2PFn function, void* ecx)
	{
		PButton ptr = (PButton)AddButton(child, NULL, name, ecx, function);
		child->CalculateLayout();
		return ptr;
	};

	PCPicker UI::CreateColorPicker(PChild child, VecCol* value, bool sameline, wchar_t* name)
	{
		if (!sameline && name != NULL)
			CreateLabel(child, name);
		PCPicker ptr = (PCPicker)Memory::Allocator(sizeof(ColorPicker));
		CPickerCon(ptr, child, value);
		InsertElement(child, ptr);
		return ptr;
	};

	PCombobox UI::CreateCombobox(PChild child, const wchar_t* name, int* value, wchar_t** arr, size_t arrsize, bool sameline)
	{
		if (!ComboboxCon) return nullptr;
		PCombobox ptr = (PCombobox)Memory::Allocator(sizeof(Combobox));
		ComboboxCon(ptr, child, name, value, sameline);
		ptr->Vars.resize(arrsize);
		for (size_t i = 0; i < arrsize; i++)
		{
			ptr->Vars[i].Name.assign(arr[i], arr[i] + wcslen(arr[i]) + 1);
			ptr->Vars[i].Index = i;
		}
		InsertElement(child, ptr);
		return ptr;
	};

	PMultiselect UI::CreateMultiselect(PChild child, const wchar_t* name, int* value, char** arr, size_t arrsize, bool sameline)
	{
		PMultiselect ptr = (PMultiselect)Memory::Allocator(sizeof(Multiselect));
		MultiCon(ptr, child, name, value, 0, sameline);
		ptr->Vars.resize(arrsize);
		for (size_t i = 0; i < arrsize; i++)
		{
			ptr->Vars[i].Name.assign(arr[i], arr[i] + strlen(arr[i]) + 1);
			ptr->Vars[i].Index = 1 << i;
		}
		InsertElement(child, ptr);
		return ptr;
	};

	PListbox UI::CreateListbox(PChild child, const wchar_t* name, int* value, wchar_t** arr, size_t arrsize, bool searchbox)
	{
		PListbox ptr = (PListbox)Memory::Allocator(sizeof(Listbox));
		ListboxCon(ptr, child, name, 158, 300, value, searchbox);

		ptr->Info.Items.reserve(arrsize);
		ptr->SSpecs.reserve(arrsize);
		ptr->SSpecs.push_back(0);
		for (size_t i = 0; i < arrsize;)
		{
			skeetvec<wchar_t> namevec(arr[i], arr[i] + wcslen(arr[i]) + 1);
			ptr->Info.Items.emplace_back(i++, namevec);
			ptr->SSpecs.push_back(i);
		}
		ptr->AbsoluteHeight = arrsize;
		InsertElement(child, ptr);
		return ptr;
	};

	void UI::AddListboxVar(PListbox list, const wchar_t* elem, size_t bsize)
	{
		bsize = bsize ? bsize : wcslen(elem) * sizeof(wchar_t);
		skeetvec<ListboxVar> newitems(list->Info.Items);
		skeetvec<wchar_t> name(elem, elem + bsize);
		newitems.emplace_back(list->Info.Items.back().Index + 1, name);
		SetListboxItems(list, &newitems);
	};

	void UI::RemoveListboxVar(PListbox list, size_t index)
	{
		if (!list->AbsoluteHeight || index >= list->AbsoluteHeight) return;
		list->SSpecs.resize(list->SSpecs.size() - 1);
		list->Info.Items.erase(std::next(list->Info.Items.begin(), index));
		list->Info.SelectedItem = --list->AbsoluteHeight - 1;

		for (size_t i = index; i < list->AbsoluteHeight; i++)
		{
			list->Info.Items[i].Index--;
		};

	};

	void UI::InsertChild(PCTab tab, void* child)
	{
		InsertChildr(tab, child);
		tab->ResetLayout();
	};

	PChild UI::CreateChild(PCTab tab, const wchar_t* name, Vec4_8t possize, bool modify)
	{
		PChild ptr = (PChild)Memory::Allocator(sizeof(Child));
		ChildCon(ptr, tab, name, possize.pack(), 0, modify);
		InsertChild(tab, ptr);
		return ptr;
	};
	
	PCTab UI::CreateTab(const wchar_t* name, Vec2 pos)
	{
		PCTab ptr = (PCTab)Memory::Allocator(sizeof(CTab));
		TabCon(ptr, Menu, name, pos.array());
		InsertTab(Menu, ptr);
		return ptr;
	};

	template <typename T>
	T* UI::GetElement(PChild child, size_t index)
	{
		return child->Elements[index]->GetAs<T>();
	};

#ifdef SDK_RENDERER_IMP
	Memory::CHooked*	Renderer::RenderHook		= nullptr;
	Memory::CHooked*	Renderer::MenuRenderHook	= nullptr;
	Memory::CHooked*	Renderer::FinalRenderHook	= nullptr;
	ReadFileFn			Renderer::ExReadFile		= nullptr;
	LoadTextureFn		Renderer::LoadTexture		= nullptr;
	LoadSvgFormFileFn	Renderer::LoadSvgFromFile	= nullptr;
	RenderIndicatorFn	Renderer::RenderIndicator	= nullptr;
	WorldToScreenFn		Renderer::WorldToScreen		= nullptr;
	RenderBlurFn		Renderer::RenderBlur		= nullptr;
	void*				Renderer::RenderBlurCtx		= nullptr;
	sVec<RenderEventListenerFn> Renderer::RenderEvents(40);
	sVec<RenderEventListenerFn> Renderer::MenuEvents(40);
	sVec<RenderEventListenerFn> Renderer::FinalEvents(40);

	
	void EventListener::remove()
	{
		if (!status) return;
		Renderer::RemoveEvent(index, type);
		status = false;
	};

	
	void Renderer::RenderListener(void* ecx, void* edx)
	{
		for (size_t i = 0; i < RenderEvents.len(); i++)
		{
			RenderEvents[i]();
		};
		reinterpret_cast<RenderFn>(RenderHook->Naked())(ecx, edx);
	};

	void Renderer::MenuRenderListener(void* ecx, void* edx)
	{
		for (size_t i = 0; i < MenuEvents.len(); i++)
		{
			MenuEvents[i]();
		};
		reinterpret_cast<RenderFn>(MenuRenderHook->Naked())(ecx, edx);
	};

	void Renderer::RenderFinalListener(void* ecx, void* edx)
	{
		
		SCLASS->IRenderer->SetAlpha(1.f);
		SCLASS->IRenderer->SetZBuff(0.0f);
		for (size_t i = 0; i < FinalEvents.len(); i++)
		{
			FinalEvents[i]();
		};
		reinterpret_cast<RenderFn>(FinalRenderHook->Naked())(ecx, edx);
	};

	void Renderer::Init()
	{
		if (RenderHook != NULL) return;
		LoadTexture		= (LoadTextureFn)Memory::CheatChunk.findr32("E8 ?? ?? ?? ?? 84 C0 74 76 8B 4C 24 14");
		LoadSvgFromFile = (LoadSvgFormFileFn)Memory::CheatChunk.find("55 8B EC 83 E4 F8 83 EC 28 56 83 CE FF 83 3D ?? ?? ?? ?? 00 57 8B FA");
		RenderIndicator = (RenderIndicatorFn)Memory::CheatChunk.find("55 8B EC 83 E4 F8 81 EC 48 40");
		WorldToScreen	= (WorldToScreenFn)Memory::CheatChunk.find("55 8B EC 83 E4 F8 F3 0F 10 71");
		RenderBlur		= (RenderBlurFn)Memory::CheatChunk.find("56 8D 71 28 8B 4E 04 8B C1 2B 06 C1 F8 03 3D 00 20 00 00");
		RenderBlurCtx	= **(void***)Memory::CheatChunk.find("8B 0D ?? ?? ?? ?? 83 C4 20 85 C9 74 09 50 55 53 57 E8", 0x2);
		ExReadFile		= (ReadFileFn)Memory::CheatChunk.find("55 8B EC 83 E4 F8 83 EC 14 53 56 57 8B F9 B8");
		RenderHook		= Memory::DetourHook::Hook(Memory::CheatChunk.find("55 8B EC 83 E4 F8 E8 ?? ?? ?? ?? 83 38 00 74 36 89 0D"), RenderListener, 6);
		MenuRenderHook	= Memory::DetourHook::Hook(Memory::CheatChunk.find("56 FF 74 24 08 8B F1 E8"), MenuRenderListener);
		FinalRenderHook = Memory::DetourHook::Hook(Memory::CheatChunk.find("53 56 57 8B F9 8B 57 08 8D 82 A8 00 00 00 F7 DA 1B D2 23 D0 52"), RenderFinalListener); 
	};

	void Renderer::Term()
	{
		if (RenderHook == nullptr) return;
		RenderEvents.~sVec();
		MenuEvents.~sVec();
		FinalEvents.~sVec();
		RenderHook->Unhook();
		MenuRenderHook->Unhook();
		FinalRenderHook->Unhook();
	};

	EventListener* Renderer::AddEvent(RenderEventType type, RenderEventListenerFn event)
	{
		auto& Events = (type == REVENT_RENDER ? RenderEvents : type == REVENT_MENU ? MenuEvents : FinalEvents);
		Events.push_back(event);
		return new EventListener(Events.len() - 1, type);
	};

	void Renderer::RemoveEvent(size_t index, RenderEventType type)
	{
		(type == REVENT_RENDER ? RenderEvents : type == REVENT_MENU ? MenuEvents : FinalEvents).remove(index);
	};

	Vec2 Renderer::ScreenSize()
	{
		Vec2 size{};
		SCLASS->IRenderer->ScreenSize(&size.x, &size.y);
		return size;
	};

	int Renderer::ScreenWidth()
	{
		int w = 0;
		SCLASS->IRenderer->ScreenSize(&w, NULL);
		return w;
	};

	int Renderer::ScreenHeigth()
	{
		int h = 0;
		SCLASS->IRenderer->ScreenSize(NULL, &h);
		return h;
	};

	Vec2 Renderer::MeasureText(const wchar_t* text, unsigned int flags)
	{
		Vec2 vec{};
		SCLASS->IRenderer->MeasureText(reinterpret_cast<int*>(&vec), text, wcslen(text), flags);
		return vec;
	};

	void Renderer::ToScreen(Vec3f_t pos, int* x, int* y)
	{
		WorldToScreen(pos.array(), x, y);
	};

	void Renderer::Text(Vec2 pos, VecCol color, const wchar_t* text, int flags, unsigned int maxlen)
	{
		SCLASS->IRenderer->Text(flags, pos.x, pos.y, color.pack(), 0x8 * maxlen, text, wcslen(text));
	};

	void Renderer::Line(Vec2 p1, Vec2 p2, VecCol color)
	{
		SCLASS->IRenderer->Line(p1.x, p1.y, p2.x, p2.y, color.pack());
	};

	void Renderer::Rect(Vec2 pos, Vec2 size, VecCol color)
	{
		SCLASS->IRenderer->Rect(pos.x, pos.y, size.x, size.y, color.pack());

	};

	void Renderer::RectOut(Vec2 pos, Vec2 size, VecCol color, int thickness)
	{
		SCLASS->IRenderer->RectOut(thickness, pos.x, pos.y, size.x, size.y, color.pack());
	};

	void Renderer::OutlinedRect(Vec2 pos, Vec2 size, VecCol colorin, VecCol colorout, int thickness)
	{
		SCLASS->IRenderer->RectOutlined(thickness, pos.x, pos.y, size.x, size.y, colorin.pack(), colorout.pack());
	};

	void Renderer::Gradient(Vec2 pos, Vec2 size, VecCol color1, VecCol color2, bool horizontal)
	{
		if (horizontal)
			SCLASS->IRenderer->GradientH(pos.x, pos.y, size.x, size.y, color1.pack(), color2.pack());
		else
			SCLASS->IRenderer->GradientV(pos.x, pos.y, size.x, size.y, color1.pack(), color2.pack());
	};

	void Renderer::Blur(Vec2 pos, Vec2 size)
	{
		RenderBlur(RenderBlurCtx, pos.x, pos.y, size.x, size.y);
	};

	void Renderer::Triangle(Vec2 p1, Vec2 p2, Vec2 p3, VecCol color)
	{
		SCLASS->IRenderer->Triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.pack());
	};

	void Renderer::Indicator(int y, VecCol color, const char* text, int texureid, float circlepercent)
	{
		RenderIndicator(&y, SCLASS->IRenderer, text, strlen(text), color.pack(), texureid, circlepercent);
	};

	void Renderer::Circle(Vec2 pos, VecCol color, float radius, float rotation, float percentage)
	{
		SCLASS->IRenderer->Circle(pos.x, pos.y, color.pack(), radius, rotation, percentage);

	};

	void Renderer::CircleOut(Vec2 pos, VecCol color, float radius, float rotation, float percentage, float thickness)
	{
		SCLASS->IRenderer->CircleOut(pos.x, pos.y, color.pack(), radius, rotation, percentage, thickness);
	};

	void Renderer::Texture(int id, Vec2 pos, Vec2 size, VecCol color, int flag, int offset, float scale)
	{
		SCLASS->IRenderer->Texture(id, pos.x, pos.y, color.pack(), offset, 0, size.x, size.y, flag, scale);
	};

	void Renderer::LoadSVGTextureFromFile(CTexture& texture, const char* filename, int width, int heigth)
	{
		int id = LoadSvgFromFile(filename, width, heigth, -1);
		if (id > 0)
		{
			texture.type = TEXTURE_SVG;
			texture.id = id;
			texture.Size = { width, heigth };
		};
	};

	void Renderer::LoadTextureFromFile(CTexture& texture, const char* filename, TextureType type)
	{
		skeetvec<unsigned char> textureChunk{ 0 };
		skeetvec<unsigned char> rawContent{ 0 };
		int width, heigth;
		ExReadFile(&textureChunk, filename);
		if (textureChunk.empty()) return;
		if (Renderer::LoadTexture(type, textureChunk.data(), textureChunk.size(), &width, &heigth, &rawContent))
		{
			int pxielSize = rawContent.size() / 4;
			if (pxielSize != 0) {
				unsigned int* ptr = (unsigned int*)rawContent.data();
				for (int i = 0; i < pxielSize; ++i)
					ptr[i] = _rotr(_byteswap_ulong(ptr[i]), 8);
			}
			int id = SCLASS->IRenderer->AddTexture(rawContent.data(), width, heigth, width * heigth * 4, 0, 0);
			if (id > 0)
			{
				texture.type = type;
				texture.id = id;
				texture.Size = { width, heigth };
			}
		}
	};

	void Renderer::LoadPNGTextureFromFile(CTexture& texture, const char* filename)
	{
		LoadTextureFromFile(texture, filename, TEXTURE_PNG);
	};

	void Renderer::LoadJPGTextureFromFile(CTexture& texture, const char* filename)
	{
		LoadTextureFromFile(texture, filename, TEXTURE_JPG);
	};

	void Renderer::LoadTextureFromMemory(CTexture& texture, const unsigned char* data, size_t size, TextureType type, int width, int heigth)
	{
		if (size == 0) return;
		int pxielSize;
		skeetvec<unsigned char> rawContent;
		if (type == TEXTURE_RGBA)
		{
			if (width * heigth * 4 != size) return;
			rawContent.assign(data, data+size);
			pxielSize = width * heigth;
		}
		else
		{
			if (!Renderer::LoadTexture(type, data, size, &width, &heigth, &rawContent)) return;
			pxielSize = rawContent.size() / 4;
		}

		if (pxielSize == 0) return;
		unsigned int* ptr = (unsigned int*)rawContent.data();
		for (int i = 0; i < pxielSize; ++i)
			ptr[i] = _rotr(_byteswap_ulong(ptr[i]), 8);
		int id = SCLASS->IRenderer->AddTexture(rawContent.data(), width, heigth, width * heigth * 4, 0, 0);
		if (id > 0)
		{
			texture.type = type;
			texture.id = id;
			texture.Size = { width, heigth };
		}
	};
#endif 
#if defined(SDK_GLOBALS_IMP) || defined(SDK_CLIENT_IMP)
	void**** CEngine::EngineCtx				= nullptr;
	CommandsInfo** CEngine::CommandsCtx		= nullptr;
	SetClanTagFn CEngine::SetTag			= nullptr;
	unsigned int* CEngine::PredComm			= nullptr;

	bool CEngine::IsInGame()
	{
		return Memory::VtableBind<bool(__cdecl*)()>(**EngineCtx, 0x1A)();
	};

	char* CEngine::MapName()
	{
		if (!IsInGame())
			return NULL;
		return Memory::VtableBind<char* (__cdecl*)()>(**EngineCtx, 0x35)();
	};

	void CEngine::ExecCommand(const char* command)
	{
		Memory::VtableBind<void(__stdcall*)(const char*)>(**EngineCtx, 0x6C)(command);
	};

	void CEngine::SetClanTag(const char* tag)
	{
		if (!Memory::VtableBind<bool(__cdecl*)()>(**EngineCtx, 0x52)() && !Memory::VtableBind<bool(__cdecl*)()>(**EngineCtx, 0x5D)())
		{
			if ((*CEngine::CommandsCtx)->LastOutCommand != *PredComm)
			{
				*PredComm = (*CEngine::CommandsCtx)->LastOutCommand;
				SetTag(tag, tag);
			};
		};
	};
#endif 

#ifdef SDK_GLOBALS_IMP
	GlobalsInfo** Globals::GlobalsCtx = nullptr;
	bool Globals::IsInGame()
	{
		return CEngine::IsInGame();
	};

	char* Globals::MapName()
	{
		return CEngine::MapName();
	};

	float Globals::RealTime()
	{
		return (*GlobalsCtx)->RealTime;
	};

	float Globals::FrameTime(bool abs)
	{
		if (abs)
			return (*GlobalsCtx)->AbsFrameTime;
		else
			return (*GlobalsCtx)->FrameTime;
	};

	int Globals::FrameCount()
	{
		return (*GlobalsCtx)->FrameCount;
	};

	float Globals::CurrentTime()
	{
		return (*GlobalsCtx)->CurrentTime;
	};

	float Globals::TickInterval()
	{
		return (*GlobalsCtx)->TickInterval;
	};

	unsigned int Globals::TickCount()
	{
		return (*GlobalsCtx)->TickCount;
	};

	int Globals::MaxPlayers()
	{
		return (*GlobalsCtx)->MaxPlayers;
	};

	unsigned int Globals::LastOutCommand()
	{
		return (*CEngine::CommandsCtx)->LastOutCommand;
	};

	unsigned int Globals::ChokedCommands()
	{
		return (*CEngine::CommandsCtx)->ChokedCommands;
	};

	unsigned int Globals::OldCommandAck()
	{
		return (*CEngine::CommandsCtx)->OldCommandAck;
	};

	unsigned int Globals::CommandAck()
	{
		return (*CEngine::CommandsCtx)->CommandAck;
	};
#endif 

#ifdef SDK_CLIENT_IMP
	LogFn		Client::Logger			= nullptr;
	LogErrorFn	Client::LoggerError		= nullptr;
	void***		Client::LoggerCtx		= nullptr;
	ScreenLogFn	Client::ScreenLogger	= nullptr;

	void Client::Exec(const char* cmd)
	{
		CEngine::ExecCommand(cmd);
	};

	void Client::SetClanTag(const char* tag)
	{
		CEngine::SetClanTag(tag);
	};

	void Client::LogScreen(const char* text, VecCol color)
	{
		ScreenLogger(text, color.pack());
	};

	void Client::Log(const char* fmt, ...)
	{
		char buffer[975];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);
		Logger(buffer);
	};
	void Client::LogColor(VecCol Color, const char* fmt, ...)
	{
		int col = Color.pack();
		va_list vargs;
		va_start(vargs, fmt);
		char msg[989];
		vsprintf(msg, fmt, vargs);
		va_end(vargs);
		((LogColorFn)LoggerCtx[0][0x19])(LoggerCtx, &col, msg);
	};
	void Client::LogError(const char* fmt, ...)
	{
		va_list vargs;
		va_start(vargs, fmt);
		char msg[975];
		vsprintf(msg, fmt, vargs);
		va_end(vargs);
		LoggerError(msg);
	};
#endif 
	void WaitForTabs()
	{
		while (Menu->Tabs.data() == nullptr || Menu->Tabs[LUA] == nullptr || Menu->Tabs[LUA]->Childs[1] == nullptr) Sleep(0);
	};

#define DECLASSIG(left, right) left = reinterpret_cast<decltype(left)>(right) 
	void InitAndWaitForSkeet()
	{
		using namespace Memory;

		
		
		
		
		
		
		
		
		
		
		
		

		
		
			DECLASSIG(Allocator, CheatChunk.find("56 8B F1 33 C0 85"));
		
		

		DECLASSIG(Deallocator, CheatChunk.find("85 C9 74 17 8D 41 F0 2B 40 08 50 64 A1 30 00 00 00 6A 00 FF 70 18 E8 ?? ?? ?? ?? C3"));

		SCLASS = *(SkeetClass_**)CheatChunk.find("A1 ?? ?? ?? ?? 83 64 24 04 00 89 54 24 18 89 44 24 10 53 56", 0x1);
		Menu = SCLASS->Menu;
		
		
		
		
		

		LuaSystem::ll_State = *(PCLuas*)CheatChunk.find("A1 ?? ?? ?? ?? 85 C0 75 01 C3 83 C0 44 C3", 0x1);
		ConfigSystem::CurrentConfigHash = *(decltype(ConfigSystem::CurrentConfigHash)*)CheatChunk.find("A3 ?? ?? ?? ?? 8B CC 8D 44 24 30 89 7C 24 10", 0x1);
		
		
		
		DECLASSIG(LuaSystem::LoadLua, CheatChunk.find("83 EC 4C 53 55 33"));
		DECLASSIG(LuaSystem::IsLuaLoaded, Memory::CheatChunk.find("51 8B D1 56 8B"));
		DECLASSIG(LuaSystem::UpdateVisualLuaInfo, Memory::CheatChunk.find("55 8B EC 83 E4 F8 81 EC 04 01 00 00 80"));
		
		
		
		DECLASSIG(ConfigSystem::GetConfigData, CheatChunk.find("55 8B EC 83 EC 18 56 57 89"));
		DECLASSIG(ConfigSystem::_LoadConfig, CheatChunk.find("55 8B EC 83 E4 F8 83 EC 40 56 57 8B FA"));
		DECLASSIG(ConfigSystem::AfterTabRage, Memory::CheatChunk.find("55 8B EC 83 E4 ?? 83 EC ?? 56 8B F1 57 8B 86"));
		DECLASSIG(UI::TabSwitch, CheatChunk.find("56 8B F1 57 8B 7C 24 0C 8B 4E 64"));
		
		
		
		DECLASSIG(UI::SetKey, CheatChunk.find("81 EC 08 02 00 00 53"));
		DECLASSIG(UI::SetCheck, CheatChunk.find("8B 41 5C 53 85"));
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		DECLASSIG(UI::SetListboxItems, CheatChunk.find("83 EC ?? 53 55 56 57 8B F9 E8"));
#if defined(SDK_GLOBALS_IMP) || defined(SDK_CLIENT_IMP)
		CEngine::EngineCtx = *(void*****)CheatChunk.find("8B 0D ?? ?? ?? ?? 8D 94 24 B0 02 00 00 52 57", 0x2);
		CEngine::CommandsCtx = *(CommandsInfo***)CheatChunk.find("8B 3D ?? ?? ?? ?? 8B 48 04 3B 0F 7E 07", 0x2);
		CEngine::SetTag = **(SetClanTagFn**)CheatChunk.find("C6 41 0F 00 FF 15 ?? ?? ?? ?? 66 C7 46 20 01 00 EB 4D", 0x6);
		CEngine::PredComm = *(unsigned int**)CheatChunk.find("A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 59 85 C0 7E 1D 8B 4D 08 8D 85 00 F0 FF FF", 0x1);
#endif 
#ifdef SDK_GLOBALS_IMP
		Globals::GlobalsCtx = *(GlobalsInfo***)CheatChunk.find("8B 0D ?? ?? ?? ?? 8B 54 24 20 D8 71 20", 0x2);
#endif
#ifdef SDK_CLIENT_IMP
		Client::LoggerCtx = **(void*****)CheatChunk.find("A1 ?? ?? ?? ?? 83 C4 0C 8B 08 56 68 ?? ?? ?? ?? 50 FF 51 68", 0x1);

		DECLASSIG(Client::LoggerError, CheatChunk.find("55 8B EC 83 E4 F8 81 EC 40 20"));
		DECLASSIG(Client::Logger, CheatChunk.find("55 8B EC 83 E4 F8 83 EC 20 80"));
		DECLASSIG(Client::ScreenLogger, CheatChunk.find("56 8B F1 8B 0D CC"));
#endif
	};
#undef DECLASSIG
#pragma pop_macro("max")
};
#endif 