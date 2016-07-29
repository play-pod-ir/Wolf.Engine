/*
	Project			 : Wolf Engine. Copyright(c) Pooya Eimandar (http://PooyaEimandar.com) . All rights reserved.
	Source			 : Please direct any bug to https://github.com/PooyaEimandar/Wolf.Engine/issues
	Website			 : http://WolfSource.io
	Name			 : w_memory.h
	Description		 : Linear memory manager
	Comment          :
*/

#ifndef __W_MEMORY_H__
#define __W_MEMORY_H__

#include "w_linear_allocator.h"
#include "w_system_dll.h"

#define MAX_PTR_ALLOCS 2048

namespace wolf
{
	namespace system
	{
		class w_memory
		{
		public:
			SYS_EXP w_memory();
			SYS_EXP ~w_memory();

			SYS_EXP bool	malloc(size_t pSize);
			SYS_EXP bool	allocate(size_t pSize, uint8_t pAlignment);
			SYS_EXP void*	read(size_t p_memory_address);
			SYS_EXP void	write(size_t p_memory_address, void* pValue);
			SYS_EXP size_t	get_address() const;
			SYS_EXP void	clear();
			SYS_EXP void	free();

			// _1KB = 10424 B, make sure change size of MAX_PTR_ALLOCS
			static const size_t _1KB() { return 1024; }
			// _1MB = 1048576 B, make sure change size of MAX_PTR_ALLOCS
			static const size_t _1MB() { return 1048576; }
			// _1GB = 1073741824 B, make sure change size of MAX_PTR_ALLOCS
			static const size_t _1GB() { return 1073741824; }

		private:
			//Prevent copying
			w_memory(w_memory const&);
			w_memory& operator= (w_memory const&);

			bool				_isReleased;
			void*				_mem;
			void*				_ptrAllocs[MAX_PTR_ALLOCS];
			w_linear_allocator*	_linearAllocator;
			size_t				_writeAddress;
		};
	}
}

#endif
