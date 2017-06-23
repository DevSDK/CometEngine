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
	//Iteration �� �ʿ��� Node�� ����
	FreeBlockType* block = m_FreeBlock;
	FreeBlockType* preblock = nullptr;

	//�������� ��ġ�� ã������ ����.
	//First-Fit Find Allocation Position
	while (block != nullptr)
	{
		//������ ������ ���� ����ũ��
		Type::uint8 adjustment = Utils::alignForwardHeader(block, m_Align, sizeof(MemoryHeader));
		Type::size_t request_size = i_size + adjustment;
		
		//�Ҵ��� �� �ִ� ����� ã�´�. (��������)
		if (block->Size < request_size)
		{
			preblock = block;
			block = block->Next;
			continue;
		}
		
		//����ü�� Align�� �����ϰ� �ִ��� üũ.
		static_assert(sizeof(MemoryHeader) >= sizeof(FreeBlockType), "sizeof(AdjustmentHeader) >= sizeof(FreeBlockType)");


		//��û ��� ũ�Ⱑ, Ÿ��Ʈ �ϸ�
		if (block->Size - request_size <= sizeof(MemoryHeader))
		{
			//��ûũ�⸦ ����� ũ��� ������
			request_size = block->Size;
			if (preblock != nullptr)
				preblock->Next = block->Next;
			else
				m_FreeBlock = block->Next;
		}
		else
		{
			//����� ������ �ִٸ�,
			FreeBlockType* next = (FreeBlockType*)Utils::Add(block, (void*)request_size);
			//�޸� ����� �ڸ���
			next->Size = block->Size - request_size;
			//�� ����� ������ FreeList���� �����Ų��.
			next->Next = block->Next;

			if (preblock != nullptr)
				preblock->Next = next;
			else
				m_FreeBlock = next;
		}
		//�ش��� �ְ�, ���ĵ� �ּҰ��� �����ϱ����� adjustment ���� �ִ´�.
		Type::ptr adress = (Type::ptr)block + adjustment;
		//�ش��� ����
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
	//�Ҵ��ߴ� �޸𸮿��� �ش����� ����
	MemoryHeader* header = (MemoryHeader*)Utils::Sub(i_Adress, (void*)sizeof(MemoryHeader));


	//�޸� ���� ���� ������
	Type::ptr block_begin = ((Type::ptr)i_Adress) - header->Adjustment;
	Type::size_t block_size = header->Size;
	//�޸� ���� �� ������
	Type::ptr block_end = block_begin + block_size;

	//�ʱ�ȭ�� ������ Ȯ���ϱ� ���� 0����  �ʱ�ȭ
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
		//���ĵ� ����Ʈ�� �����ϱ� ���� ��ġ Ž��
		if ((Type::ptr)block >= block_end)
			break;
		preblock = block;
		block = block->Next;
	}

	if (preblock == nullptr)
	{
		//���� ����� ���� ���¶�� 
		//���� ����� ���� ����� ������ ����Ű�� �ѵ� ��������� ����������� �ٲ�.( �׳� �ڿ��ΰ� ����)
		preblock = (FreeBlockType*)block_begin;
		preblock->Size = block_size;
		preblock->Next = m_FreeBlock;
		m_FreeBlock = preblock;
	}
	else if ((Type::ptr) preblock + preblock->Size == block_begin)
	{
		//�� ����� �ٷ� ���������� ���������� �ִٸ�, ����� ����� Ű��.
		preblock->Size += block_size;
	}
	else
	{
		//��� ��带 ����
		FreeBlockType* freeblock = (FreeBlockType*)block_begin;
		freeblock->Size = block_size;
		//��� �߰��� ��������. preblock->freeblock-> preblock�� ����Ű�� ���
		freeblock->Next = preblock->Next;
		preblock->Next = freeblock;
		preblock = freeblock;
	}

	//�������� ��尡 ���� ������, �� ��带 ������
	if (block != nullptr && (Type::ptr)block == block_end)
	{
		preblock->Size += block->Size;
		preblock->Next = block->Next;
	}

	m_AllocCounter--;
	m_UsedMemory -= block_size;

}
