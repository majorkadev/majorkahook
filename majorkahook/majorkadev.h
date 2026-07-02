#pragma once
#include <Windows.h>
#include <map>
#include <fstream>
#include "VM/recompilation.h"
#include "Json/json.hpp"

using u32 = unsigned int;

class skeet_t {
	u32 _base;
	u32 _size;
	void* page;
	char* skeet_bin;
	void* _stack;
	void* skeetFonts;
	static skeet_t* singleton;
	std::map<uint32_t, import_t> exports;
	using handler_t = std::vector<vm_element_t>;
	std::vector<handler_t> handlers;
public:
	skeet_t(HMODULE base);
	skeet_t(const skeet_t&) = delete;
	void operator=(const skeet_t&) = delete;
	bool map();
	bool fix_imports();
	bool extra();
	bool entry();
	void recompile();
	void* stack() const { return _stack; }
	u32 stack_size() const { return 0x100000; }
	u32 base() const { return _base; }
	u32 size() const { return _size; }
	void* menuFonts() { return skeetFonts; }

	static skeet_t* getInstance(HMODULE base = nullptr);
	static bool is_stack_range(u32 addr);
	static bool is_image_range(u32 addr);
	static bool is_exception(u32 addr);
};