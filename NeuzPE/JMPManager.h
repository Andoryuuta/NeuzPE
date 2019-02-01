#pragma once
#include <map>

class JMP {
public:
	JMP(void* from, void* to);
	void Activate();
	void Restore();

private:
	void make_writable();
	void restore_perms();

	bool active = false;
	void* from = nullptr;
	void* to = nullptr;
	uint64_t original_data;
	uint32_t original_perms;
};

