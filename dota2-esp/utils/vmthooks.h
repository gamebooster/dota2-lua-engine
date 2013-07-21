#pragma once

#include <cassert>

namespace utils {

typedef unsigned int uint;
typedef int int_ptr;
template< typename T > inline T make_ptr( void* ptr, int_ptr offset ) { return reinterpret_cast<T>( (size_t)ptr + offset ); }
inline void**& getvtable( void* inst, int_ptr offset = 0 ) { return *reinterpret_cast<void***>( (size_t)inst + offset ); }

// Find the number of vfuncs in a vtable
uint CountFuncs( void** pVMT );
uint CountFuncs( void* begin, void* end, void** pVMT );

// Find the index for a vfunc, result is negative if not found
int FindFunc( void** pVMT, void* pFunc, uint vfuncs = 0 );

// ----------------------------------------------------------------
// Class: VMTManager
// ----------------------------------------------------------------
// Hooks virtual functions by replacing the vtable pointer from an instance.
//
// Purpose:
//  Manages the virtual table of an object.
//
	class VMTManager {
		// Forbid copy constructing and assignment.
		VMTManager( const VMTManager& );
		VMTManager& operator= ( const VMTManager& );

	public:
		VMTManager( void* inst, size_t offset = 0, uint vfuncs = 0 );
		~VMTManager();

		// Hooks a function by index.
		inline void HookMethod( void* newfunc, size_t index )
		{
			assert( index<_vcount );
			_array[index+3] = newfunc;
		}
		// Unhooks a function by index.
		inline void UnhookMethod( size_t index )
		{
			assert( index<_vcount );
			_array[index+3] = _oldvmt[index];
		}

		// Manage the hooks.
		inline void Unhook() { *_vftable = _oldvmt; }
		inline void Rehook() { *_vftable = _array + 3; }
		inline bool Hooked() const { return *_vftable!=_oldvmt; }
		inline void EraseHooks() { for ( uint i = 0; i<_vcount; ++i ) _array[i+3] = _vftable[i]; }
		inline uint NumFuncs() const { return _vcount; }

		// If the instance is somehow destroyed before you get a chance to unhook it or destruct this hook object, call this.
		// It'll prevent the destructor from crashing.
		inline void Poof() { _vftable = NULL; }

		// Get the original function.
		// Use a function prototype for the template argument to make it very easy to call this function.
		// Example syntax: hook.GetMethod<bool (__thiscall*)( void*, int )>( 12 )( inst, arg );
		template< typename Fn >
		inline Fn GetMethod( size_t index ) const
		{
			assert( index<_vcount );
			return (Fn) _oldvmt[ index ];
		}

	protected:
		inline void _set_guard( size_t S ) { _array[1] = (void*) S; }
		inline size_t _get_guard() const { return (size_t) _array[1]; }
		inline void _set_backptr( void* ptr ) { _array[0] = ptr; }
		inline void* _get_backptr() const { return _array[0]; }

  private:
		void***	_vftable;
		void**	_oldvmt;
		void**	_array;
		uint	_vcount;
	};
	// ----------------------------------------------------------------
	// Class: DynManager
	// ----------------------------------------------------------------
	// Hooks virtual functions by replacing the vtable pointer from an instance.
	// Adds functionality to the basic VMTManager
	//
	// Purpose:
	//  Allows you to attach your own data to an instance.
	//  Hint: Hook the virtual destructor (usually the first entry in a vtable) if you allocated your userdata on the heap.
	//        Store a pointer back to the DynManager<T,S>(s) in your userdata.
	//
	// Warning:
	//  Do NOT attempt to unhook, as this will cause HookPresent and the others to fail (this may result in attempting to hook an already hooked object, which is BAD).
	//  Do NOT mix usage of multiple instances with a different template params (for the same reason as above).
	//
	template< typename T = int, size_t S = 0xDE4DB3EF >
	class DynManager : public VMTManager
	{
		// Forbid copy constructing and assignment.
		DynManager( const DynManager& );
		DynManager<T,S>& operator= ( const DynManager& );

	public:
		DynManager( void* inst, size_t offset = 0, uint vfuncs = 0 ) : VMTManager( inst, offset, vfuncs ), _userdata(NULL)
		{
			_set_guard( S );
			_set_backptr( make_ptr<void*>( this, S ) );
		}

		// Shouldn't be used.
		inline void Unhook() { assert( "DynManager::Unhook() Unhooking is not supported." ); }
		inline void Rehook() { assert( "DynManager::Rehook() Rehooking is not supported." ); }

		// Retrieve the userdata associated with this instance.
		inline T* UserData() const { return _userdata; }
		inline void UserData( T* data ) { _userdata = data; }

		// Returns true if this instance is already hooked.
		static inline bool HookPresent( void* inst, size_t offset = 0 )
		{
			void** vmt = getvtable( inst, offset );
			return vmt[-2]==(void*)S;
		}
		// Get the DynManager<T,S> for an object, returns NULL if it's not hooked.
		static inline DynManager<T,S>* GetHook( void* inst, size_t offset = 0 )
		{
			DynManager<T,S>* ptr = NULL;
			void** vmt = getvtable( inst, offset );
			if ( vmt[-2]==(void*)S ) ptr = make_ptr<DynManager<T,S>*>( vmt[-3], 0-S );
			return ptr;
		}
		// Get the DynManager<T,S> for an object. Creates a new hook if it doesn't exist yet.
		static DynManager<T,S>* GetOrCreateHook( void* inst, size_t offset = 0, size_t vfuncs = 0 )
		{
			void** vmt = getvtable( inst, offset );
			if ( vmt[-2]==(void*)S ) return make_ptr<DynManager<T,S>*>( vmt[-3], 0-S );
			else return new DynManager<T,S>( inst, offset, vfuncs );
		}

	private:
		T*		_userdata;
	};








	// ----------------------------------------------------------------
	// Class: VMTHook
	// ----------------------------------------------------------------
	// Hooks virtual functions by overwriting the function pointers in .rdata.
	//
	// Purpose:
	//  Hook virtual methods of all instances of a class.
	//
	class VMTHook
	{
		// Forbid copy constructing and assignment
		VMTHook( const VMTHook& );
		VMTHook& operator= ( const VMTHook& );

	public:
		VMTHook( void* inst, size_t offset = 0, uint vfuncs = 0 ) { _init( getvtable( inst, offset ), vfuncs ); }
		VMTHook( void** vmt, uint vfuncs = 0 ) { _init( vmt, vfuncs ); }
		~VMTHook();

		// Hooks a function
		void HookMethod( void* new_func, size_t index );
		void UnhookMethod( size_t index );

		inline void Unhook() { if ( Hooked() ) _swap_tables(); }
		inline void Rehook() { if ( !Hooked() ) _swap_tables(); }
		inline bool Hooked() const { return *_hooks==_vftable; }

		// Get the original function. Use a function prototype for the template argument to make it very easy to call this function.
		// Example syntax: hook.GetMethod<bool (__thiscall*)( void*, int )>( 0x5 )( inst, arg );
		template< typename Fn >
		inline Fn GetMethod( size_t index ) const
		{
			assert( index<_vcount ); 
			return (Fn) (*_orig)[ index ];
		}

	protected:
		void _init( void** vmt, uint vfuncs );
		void _swap_tables();

	private:
		void**	_vftable;
		void**	_backup;
		void***	_hooks;
		void***	_orig;
		uint	_vcount;
	};

};
