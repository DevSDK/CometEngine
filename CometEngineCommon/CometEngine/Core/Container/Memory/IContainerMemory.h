#pragma once
#include "../../Memory/CometEngineMemory.h"
namespace CometEngine{
namespace Core {
namespace Container{
namespace Memory
{
	class IContainerMemory
	{
	protected:
		Core::Memory::BaseAllocator* Allocator;
		int AllowSize;
		int MemorySize;
	};


}}}}