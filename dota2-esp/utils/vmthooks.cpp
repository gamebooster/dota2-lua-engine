#include "windows.h"
#include "vmthooks.h"

namespace utils {

	uint CountFuncs( void** vmt )
	{
		// Query memory and as long as the pointer lands in PAGE_EXECUTE_READ or PAGE_EXECUTE_READWRITE memory
		// assume this is a valid function pointer.
		MEMORY_BASIC_INFORMATION mem;
		int i = -1;
		do { i++; VirtualQuery( vmt[i], &mem, sizeof(MEMORY_BASIC_INFORMATION) ); }
		while( mem.Protect==PAGE_EXECUTE_READ || mem.Protect==PAGE_EXECUTE_READWRITE );
		return i;
	}
	uint CountFuncs( void* begin, void* end, void** vmt )
	{
		int i = -1;
		do i++; while ( begin<vmt[i] && vmt[i]<end );
		return i;
	}
	int FindFunc( void** vmt, void* func, uint vfuncs )
	{
		if ( !vfuncs ) vfuncs = CountFuncs( vmt );
		for ( uint i = 0; i<vfuncs; i++ )
		{
			if ( vmt[i]==func ) return i;
		}
		return -1;
	}





	// VMTManager
	VMTManager::VMTManager( void* inst, size_t offset, uint vfuncs )
	{
		_vftable = make_ptr<void***>( inst, offset );
		_oldvmt = *_vftable;
		// Count vfuncs ourself if needed
		if ( !vfuncs ) vfuncs = CountFuncs( _oldvmt );
		_vcount = vfuncs;
		// Allocate room for the new vtable
		_array = (void**)malloc( (vfuncs+3)*sizeof(void*) );
		// Initialize RTTI pointer (should there be one)
		_array[2] = _oldvmt[-1];	
		// Copy over the other vfuncs
		for ( uint i = 0; i<vfuncs; ++i ) _array[i+3] = _oldvmt[i];
		// Hook it
		*_vftable = _array+3;
	}
	VMTManager::~VMTManager()
	{
		if ( _vftable ) Unhook();
		free( _array );
	}






	// VMTHook

	VMTHook::~VMTHook()
	{
		Unhook();
		free( _backup );
	}
	void VMTHook::HookMethod( void* func, size_t index )
	{
		assert( index<_vcount );
		if ( Hooked() )
		{
			void* dest = &_vftable[index];
			DWORD dwOld;
			if ( VirtualProtect( dest, sizeof(void*), PAGE_EXECUTE_READWRITE, &dwOld ) )
			{
				_vftable[index] = func;
				VirtualProtect( dest, sizeof(void*), dwOld, &dwOld );
			}
		}
		else _backup[index] = func;
	}
	void VMTHook::UnhookMethod( size_t index )
	{
		assert( index<_vcount );
		if ( Hooked() )
		{
			void* dest = &_vftable[index];
			DWORD dwOld;
			if ( VirtualProtect( dest, sizeof(void*), PAGE_EXECUTE_READWRITE, &dwOld ) )
			{
				_vftable[index] = _backup[index];
				VirtualProtect( dest, sizeof(void*), dwOld, &dwOld );
			}
		}
		else _backup[index] = _vftable[index];
	}
	void VMTHook::_init( void** vmt, uint vfuncs )
	{
		_vftable = vmt;
		if ( !vfuncs ) vfuncs = CountFuncs( vmt );
		_vcount = vfuncs;
		_backup = (void**)malloc( _vcount*sizeof(void*) );
		for ( uint i = 0; i<_vcount; i++ ) _backup[i] = _vftable[i];
		_hooks = &_vftable;
		_orig = &_backup;
	}
	void VMTHook::_swap_tables()
	{
		DWORD dwOld;
		if ( VirtualProtect( _vftable, _vcount*sizeof(void*), PAGE_EXECUTE_READWRITE, &dwOld ) )
		{
			for ( uint i = 0; i<_vcount; i++ )
			{
				void* t = _vftable[i];
				_vftable[i] = _backup[i];
				_backup[i] = t;
			}
			VirtualProtect( _vftable, _vcount*sizeof(void*), dwOld, &dwOld );
		}
	}

};
