#ifndef __COMETENGINE_TYPES_H__
#define __COMETENGINE_TYPES_H__
#include <vcruntime.h>
#include <assert.h>
#define CEAssert(expression)  assert(expression)
namespace CometEngine
{
	namespace Type
	{

	
	#ifdef _WIN64 
		typedef unsigned long long ptr;
		typedef unsigned __int64 size_t;

	#else 
		typedef unsigned int size_t;
		typedef unsigned long ptr;
	#endif

		typedef __int8 int8;
		typedef __int32 int32;
		typedef __int64 int64;
	
		typedef unsigned __int8 uint8;
		typedef unsigned __int32 uint32;
		typedef unsigned int uint;
		typedef unsigned __int64 uint64;
			

	}

}

#endif