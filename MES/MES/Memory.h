#pragma once
#include"iostream"

struct AllocationMetrics
{
	uint32_t totalAllocated = 0;
	uint32_t totalFreed = 0;
	uint32_t currentUsage() { return totalAllocated - totalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

inline void* operator new(size_t size)
{
	s_AllocationMetrics.totalAllocated += size;
	return malloc(size);
}

inline void operator delete(void* memory, size_t size)
{
	s_AllocationMetrics.totalFreed += size;
	free(memory);
}

static void printMemoryUsage()
{
	auto mem = s_AllocationMetrics.currentUsage();
	int i = 0;
	while (mem >= 1024 && i < 3)
	{
		i++;
		mem /= 1024;
	}
	std::cout << "Memory Usage = " << mem;
	switch (i)
	{
	case 0:
		std::cout << " B\n";
		break;
	case 1:
		std::cout << " KB\n";
		break;
	case 2:
		std::cout << " MB\n";
		break;
	case 3:
		std::cout << " GB\n";
		break;
	}
}