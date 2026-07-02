#pragma once
#include <cstdint>
#include <vector>

enum eoperation {
	ROL, DEC, NOT, BSWAP, ADD, XOR, ROR, INC, NEG, SUB
};

struct vm_operation_t {
	uint64_t addr;
	eoperation operation;
	uint64_t value;
};

struct vm_element_t {
	uint64_t addr;
	uint8_t size;
	uint64_t vip;
	uint64_t encryption_key;
	uint64_t value;
	std::vector<vm_operation_t> operations;
};