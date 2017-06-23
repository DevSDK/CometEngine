
#ifndef __COMMON_COMETENGINE_MEMORY__
#define __COMMON_COMETENGINE_MEMORY__

#include <iostream>
#include "../Definition/CometEngineTypes.h"

/*----------------------------------------------------

This is Comet Engine Memory Management System class
Singleton Class

Memory Pool Based Memory

The Memory Pool For Tite Loop Data.

First-Fit based Allocate.
Feee List Based Allocator.



A Large Memory use System Allocator.

-----------------------------------------------------*/



namespace CometEngine
{

	namespace Core
	{
		namespace Memory
		{

			namespace Utils
			{
				//Get Next Align Adress
				const void* alignForward(const void* i_base_adress, Type::ptr i_alignment);
				//Get AlignAdjustment
				const Type::uint32 alignForwardAdjustment(const void* i_adress, Type::ptr i_alignment);
				//Get AlignForwardHeader Size
				const Type::uint32 alignForwardHeader(const void* i_adress, Type::ptr i_alignment, Type::uint32 i_HeaderSize);
				//Add Pointer A, B
				const void* Add(const void* A, const void* B);
				//Sub Pointer A, B
				const void* Sub(const void* A, const void* B);
				
			}
			//The Custom Allocator Base Class
			class BaseAllocator
			{
			protected:
				void* m_MemoryBlock;
				Type::size_t m_Offset;
				Type::size_t m_AllocCounter;
				Type::size_t m_UsedMemory;
				Type::size_t m_TotalSize;
				Type::uint8 m_Align;
				BaseAllocator(void* i_MemoryBlock, Type::uint8 i_align, Type::size_t i_MemorySize, Type::size_t i_offset = 0);
			public:
				virtual void* alloc(Type::size_t i_byte) = 0;
				virtual void dealloc(void* i_Adress) = 0;
			};

			/**
			 * @brief Free-List�� ������� �ϴ� Ŀ���� �޸� �Ҵ����̴�.
			 * @details
			 * �Ҵ�� �޸� ����� ������, Free-List�� �����Ͽ�, ���� ����
			 * �޸� �Ҵ� ��ġ�� ã�� �Ҵ����ִ� ������ �Ѵ�.
			 *
			 * @author Dev SDK, k_sdk@naver.com
			 * @date 2017-06-23
			 * @version 0.0.1
			 */
			class FreeListAllocator : public BaseAllocator
			{
				struct FreeBlockType
				{
					FreeBlockType* Next;
					Type::size_t Size;
				};

				struct MemoryHeader
				{
					Type::size_t Size;
					Type::uint8 Adjustment;
				};

				FreeBlockType* m_FreeBlock;
			public:
				
				/**
				* @brief FreeListAllocator ������.
				* @param void* i_MemoryBlock ������ �޸� ����� �Է����� �޴´�. 16����Ʈ���� Ŀ���Ѵ�.
				* @param Type::uint8 i_align �޸� ���� ũ��
				* @param Type::size_t i_MemorySize ����� �޸��� ũ��. 
				* @param Type::size_t i_offset �޸��� ���� ��ġ�� �˸��� offset default=0
				*/
				FreeListAllocator(void* i_MemoryBlock, Type::uint8 i_align, Type::size_t i_MemorySize, Type::size_t i_offset = 0);


				/**
				* @brief i_byte ��ŭ �޸𸮸� �Ҵ��մϴ�.
				* @param Type::size_t i_byte �޸� �Ҵ� ũ��
				* @return �Ҵ�� �޸��� ���� �ּҸ� ��ȯ
				*/
				virtual void * alloc(Type::size_t i_byte) override;
				
				/**
				* @brief i_Adress�� ����Ű�� �ִ� �޸𸮸� �Ҵ� �����մϴ�.
				* @param void* i_Adress �Ҵ� ������ �޸� �ּ�
				*/
				virtual void dealloc(void * i_Adress) override;

			};


		}
	}
}



#endif	