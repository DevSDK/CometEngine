#include "CometEngineMemory.h"
using namespace CometEngine;
using namespace Core;

const void * CometEngine::Core::Memory::Utils::alignForward(const void * i_base_adress, Type::ptr i_alignment)
{
	return (void*)(((Type::ptr)i_base_adress) + ((i_alignment - 1) & (~(i_alignment - 1))));
}

const Type::uint32 CometEngine::Core::Memory::Utils::alignForwardAdjustment(const void * i_adress, Type::ptr i_alignment)
{
	Type::uint8 adjustment = i_alignment - ((Type::ptr)(i_adress)) & ((Type::ptr) i_alignment - 1);
	if (i_alignment == adjustment)
		return 0;
	return adjustment;
}

const Type::uint32 CometEngine::Core::Memory::Utils::alignForwardHeader(const void * i_adress, Type::ptr i_alignment, Type::uint32 i_HeaderSize)
{
	Type::uint8 adjustment = alignForwardAdjustment(i_adress, i_alignment);
	Type::uint8 need = i_HeaderSize;
	if (adjustment < need)
	{
		need -= adjustment;
		adjustment += i_alignment* (need / i_alignment);
		if (need % i_alignment > 0)
			adjustment += i_alignment;
		return adjustment;
	}

}

const void * CometEngine::Core::Memory::Utils::Add(const void * A, const void * B)
{
	return (void*)((Type::ptr)A + (Type::ptr)B);
}

const void * CometEngine::Core::Memory::Utils::Sub(const void * A, const void * B)
{
	return (void*)((Type::ptr)A - (Type::ptr)B);
}

CometEngine::Core::Memory::BaseAllocator::BaseAllocator(void * i_MemoryBlock, Type::uint8 i_align, Type::size_t i_MemorySize, Type::size_t i_offset)
{
	m_Offset = i_offset;
	m_MemoryBlock = (void*)((Type::ptr)i_MemoryBlock + (Type::ptr)i_offset);
	m_TotalSize = i_MemorySize;
	m_Align = i_align;
	m_AllocCounter = 0;
	m_UsedMemory = 0;

}

CometEngine::Core::Memory::FreeListAllocator::FreeListAllocator(void * i_MemoryBlock, Type::uint8 i_align, Type::size_t i_MemorySize, Type::size_t i_offset)
	: BaseAllocator(i_MemoryBlock, i_align, i_MemorySize, i_offset)
{
	m_FreeBlock = (FreeBlockType*)m_MemoryBlock;
	m_FreeBlock->Next = nullptr;
	m_FreeBlock->Size = m_TotalSize;
}

void * CometEngine::Core::Memory::FreeListAllocator::alloc(Type::size_t i_size)
{
	//Iteration 에 필요한 Node를 대입
	FreeBlockType* block = m_FreeBlock;
	FreeBlockType* preblock = nullptr;

	//최초적합 위치를 찾기위한 루프.
	//First-Fit Find Allocation Position
	while (block != nullptr)
	{
		//데이터 정렬을 위한 공간크기
		Type::uint8 adjustment = Utils::alignForwardHeader(block, m_Align, sizeof(MemoryHeader));
		Type::size_t request_size = i_size + adjustment;
		
		//할당할 수 있는 블록을 찾는다. (최초적합)
		if (block->Size < request_size)
		{
			preblock = block;
			block = block->Next;
			continue;
		}
		
		//구조체가 Align을 유지하고 있는지 체크.
		static_assert(sizeof(MemoryHeader) >= sizeof(FreeBlockType), "sizeof(AdjustmentHeader) >= sizeof(FreeBlockType)");


		//요청 블록 크기가, 타이트 하면
		if (block->Size - request_size <= sizeof(MemoryHeader))
		{
			//요청크기를 블록의 크기로 설정함
			request_size = block->Size;
			if (preblock != nullptr)
				preblock->Next = block->Next;
			else
				m_FreeBlock = block->Next;
		}
		else
		{
			//충분한 공간이 있다면,
			FreeBlockType* next = (FreeBlockType*)Utils::Add(block, (void*)request_size);
			//메모리 블록을 자르고
			next->Size = block->Size - request_size;
			//이 블록을 제외한 FreeList끼리 연결시킨다.
			next->Next = block->Next;

			if (preblock != nullptr)
				preblock->Next = next;
			else
				m_FreeBlock = next;
		}
		//해더를 넣고, 정렬된 주소값을 유지하기위한 adjustment 값을 넣는다.
		Type::ptr adress = (Type::ptr)block + adjustment;
		//해더를 넣음
		MemoryHeader* header = (MemoryHeader*)(adress - sizeof(MemoryHeader));
		header->Size = request_size;
		header->Adjustment = adjustment;
		m_UsedMemory += request_size;
		m_AllocCounter++;

		CEAssert(Utils::alignForwardAdjustment((void*)adress, m_Align) == 0);

		return (void*)adress;
	}
#ifdef _DEBUG
	CEAssert(!"Not eonugh memory block");
#endif
	return nullptr;
}

void CometEngine::Core::Memory::FreeListAllocator::dealloc(void * i_Adress)
{
	CEAssert(i_Adress != nullptr);
	//할당했던 메모리에서 해더값을 추출
	MemoryHeader* header = (MemoryHeader*)Utils::Sub(i_Adress, (void*)sizeof(MemoryHeader));


	//메모리 블럭의 시작 포인터
	Type::ptr block_begin = ((Type::ptr)i_Adress) - header->Adjustment;
	Type::size_t block_size = header->Size;
	//메모리 블럭의 끝 포인터
	Type::ptr block_end = block_begin + block_size;

	//초기화를 눈으로 확인하기 위해 0으로  초기화
#ifdef _DEBUG
	for (Type::ptr i = (Type::ptr)i_Adress; i != block_end; i++)
	{
		(*((char*)i)) = 0;
	}
#endif
	FreeBlockType* block = m_FreeBlock;
	FreeBlockType* preblock = nullptr;

	while (block != nullptr)
	{
		//정렬된 리스트를 유지하기 위한 위치 탐색
		if ((Type::ptr)block >= block_end)
			break;
		preblock = block;
		block = block->Next;
	}

	if (preblock == nullptr)
	{
		//이전 블록이 없는 상태라면 
		//현제 블록을 기존 블록의 다음을 가르키게 한뒤 기존블록을 현제블록으로 바꿈.( 그냥 뒤에두고 연결)
		preblock = (FreeBlockType*)block_begin;
		preblock->Size = block_size;
		preblock->Next = m_FreeBlock;
		m_FreeBlock = preblock;
	}
	else if ((Type::ptr) preblock + preblock->Size == block_begin)
	{
		//두 블록이 바로 오른쪽으로 연속적으로 있다면, 블록의 사이즈를 키움.
		preblock->Size += block_size;
	}
	else
	{
		//블록 노드를 만듬
		FreeBlockType* freeblock = (FreeBlockType*)block_begin;
		freeblock->Size = block_size;
		//노드 중간에 끼워넣음. preblock->freeblock-> preblock이 가르키는 노드
		freeblock->Next = preblock->Next;
		preblock->Next = freeblock;
		preblock = freeblock;
	}

	//왼쪽으로 노드가 옆에 있으면, 두 노드를 연결함
	if (block != nullptr && (Type::ptr)block == block_end)
	{
		preblock->Size += block->Size;
		preblock->Next = block->Next;
	}

	m_AllocCounter--;
	m_UsedMemory -= block_size;

}
