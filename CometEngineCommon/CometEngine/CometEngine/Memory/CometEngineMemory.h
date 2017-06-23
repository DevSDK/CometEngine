
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
			 * @brief Free-List를 기반으로 하는 커스텀 메모리 할당자이다.
			 * @details
			 * 할당된 메모리 블록을 가지고, Free-List를 구성하여, 최초 적합
			 * 메모리 할당 위치를 찾아 할당해주는 역할을 한다.
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
				* @brief FreeListAllocator 생성자.
				* @param void* i_MemoryBlock 생성한 메모리 블록을 입력으로 받는다. 16바이트보다 커야한다.
				* @param Type::uint8 i_align 메모리 정렬 크기
				* @param Type::size_t i_MemorySize 사용할 메모리의 크기. 
				* @param Type::size_t i_offset 메모리의 시작 위치를 알리는 offset default=0
				*/
				FreeListAllocator(void* i_MemoryBlock, Type::uint8 i_align, Type::size_t i_MemorySize, Type::size_t i_offset = 0);


				/**
				* @brief i_byte 만큼 메모리를 할당합니다.
				* @param Type::size_t i_byte 메모리 할당 크기
				* @return 할당된 메모리의 시작 주소를 반환
				*/
				virtual void * alloc(Type::size_t i_byte) override;
				
				/**
				* @brief i_Adress가 가르키고 있는 메모리를 할당 헤제합니다.
				* @param void* i_Adress 할당 헤제할 메모리 주소
				*/
				virtual void dealloc(void * i_Adress) override;

			};


		}
	}
}



#endif	