#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <algorithm>

class Signature {
public:
  Signature(std::string pattern, std::string mask) : pattern(pattern), mask(mask) {}
  Signature(std::string idapattern) {
    idapattern.erase(std::remove(idapattern.begin(), idapattern.end(), ' '), idapattern.end());
    for (auto it = idapattern.cbegin(); it != idapattern.cend();) {
      if (*it == wildcard) {
        pattern += '\0';
        mask += wildcard;
        it++;
        continue;
      }
      std::string cur(it, it + 2);

      std::stringstream converter(cur);
      uint16_t current = 0;
      converter >> std::hex >> current >> std::dec;
      char real_current = static_cast<char>(current);
      pattern += real_current;
      mask += 'x';
      it += 2;
    }
  }

  static const char wildcard = '?';
  std::string pattern;
  std::string mask;
};

class Process {
public:
	Process();
	~Process();

	// Various ways to attach to a process
	void Attach( HANDLE process );
	void Attach( int pid );
	void Attach( const char* window );
	void Detach();
	
	// Find module information in the specified process
  void GetRemoteModule( const char* filename, MODULEENTRY32* lpme );

	// Read some information
	void ReadData( void* dest, const void* src, unsigned int bytes );
	template< typename T >
	inline void ReadData( T& dest, const void* src ) {
		ReadData( &dest, src, sizeof(T) );
	}
	template< typename T >
	inline T ReadData( const void* src ) {
		T t;
		ReadData( &t, src, sizeof(T) ); // Not checking for failure...
		return t;
	}

	// Write some data back
	void WriteData( void* dest, const void* src, unsigned int bytes );
	template< typename T >
	inline void WriteData( void* dest, const T& src ) {
		WriteData( dest, &src, sizeof(T) );
	}

	// Remote buffer management
	void* AddResource( const void* res, unsigned int bytes );
	//int Invoke( unsigned ms = INFINITE );
	int Execute( void* addr, void* param );

private:
	int process_id_;
	HANDLE process_handle_;

	struct rbuf_t
	{
		void* base;
		void* free;
	} rbuf;
};

class Module {
 public:
	Module();
	~Module();

  void Init(Process& process, const char* dllname);

	template< typename T >
	inline T& ReadData( int offset ) const {
		return *(T*)((char*)data_ + offset );
	}

	inline void* GetAddress( uint32_t offset = 0 ) const {
		return module_entry_.modBaseAddr + offset;
	}
	inline uint32_t GetOffset( void* ptr ) const {
		return (char*)ptr - (char*)module_entry_.modBaseAddr;
	}
	inline const char* GetFileName() const {
		return module_entry_.szModule;
	}
  const unsigned char* Module::FindPattern(Signature signature) {
    return FindPattern(reinterpret_cast<const unsigned char*>(signature.pattern.c_str()), signature.mask.c_str());
  }
  const unsigned char* Module::FindPattern(const unsigned char* signature, const char* mask);
  uint32_t Module::GetAbsoluteAddress(uint32_t relative_address);

 private:
	MODULEENTRY32 module_entry_;
	void* data_;
};