#define STR(...) #__VA_ARGS__
#ifdef COMPILER
const char *header = "#include <algorithm>\n" STR(
#endif

#ifndef COMPILER
#include <algorithm>
#endif

class turing_ptr {
	int* memory;
	int addr_begin,
	    addr_end;
	int ptr;
public:
	turing_ptr()
		: addr_begin(0)
		, addr_end(0)
		, ptr(0)
	{
		memory = new int[1];
		memory[0] = 0;
	}
	void resize_left()
	{
		int size  = addr_end - addr_begin + 1,
		    nsize = size*2;
		int* tmp = new int[nsize];
		std::copy(memory, memory+size, tmp+size);
		std::fill(tmp, tmp+size, 0);
		delete []memory;
		memory = tmp;
		addr_begin -= size;
	}
	void resize_right()
	{
		int size  = addr_end - addr_begin + 1,
		    nsize = size*2;
		int* tmp = new int[nsize];
		std::copy(memory, memory+size, tmp);
		std::fill(tmp+size, tmp+nsize, 0);
		delete []memory;
		memory = tmp;
		addr_end += size;
	}
	void fix()
	{
		while (ptr > addr_end)
			resize_right();
		while (ptr < addr_begin)
			resize_left();
	}
	void operator++()
	{
		++ptr;
		fix();
	}
	void operator+=(int offset)
	{
		ptr += offset;
		fix();
	}
	void operator--()
	{
		--ptr;
		fix();
	}
	void operator-=(int offset)
	{
		ptr -= offset;
		fix();
	}
	int& operator*()
	{
		return memory[ptr - addr_begin];
	}
};
#ifdef COMPILER
);
#endif
