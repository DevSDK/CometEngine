
#include <iostream>
#include "../CometEngine/Memory/CometEngineMemory.h"

using namespace CometEngine;
using namespace Core;

int main()
{
	char* MemoryBlock = new char[1024 * 1024 * 10];
	Memory::FreeListAllocator allocator = Memory::FreeListAllocator(MemoryBlock, 4, 1024 * 1024);
	
	int* ptarr[10];
	
	for (int i = 0; i < 10; i++)
	{
		ptarr[i] = (int*)allocator.alloc(sizeof(int) * 10);

	}

	for (int i = 0; i < 10; i++)
	{
		if (i & 1)
			allocator.dealloc(ptarr[i]);
	}



	getchar();
	delete MemoryBlock;

}