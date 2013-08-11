#include "process.h"
#include "utils\utils.h"

#include <boost/throw_exception.hpp>
#include <cassert>


Process::Process() : process_handle_(INVALID_HANDLE_VALUE) {
	rbuf.base = nullptr;
}

Process::~Process() {
	Detach();
}

void Process::Attach( HANDLE handle ) {
  if (handle == nullptr) {
    BOOST_THROW_EXCEPTION(std::runtime_error("handle is nullptr"));
  }
  process_handle_ = handle;

  if ( rbuf.base = ::VirtualAllocEx( process_handle_, nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE ) ) {
    rbuf.free = (char*)rbuf.base + 0x100;
  }
}

void Process::Attach(int pid) {
	process_id_ = pid;
	DWORD rights = PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION|PROCESS_SUSPEND_RESUME|SYNCHRONIZE;
	Attach(OpenProcess(rights, FALSE, pid));
}

void Process::Attach( const char* window ) {
  HWND hwnd = FindWindow(nullptr, window);
  if (hwnd == nullptr) {
    BOOST_THROW_EXCEPTION(std::runtime_error("FindWindowError"));
  }

	DWORD pid;
	DWORD tid = GetWindowThreadProcessId( hwnd, &pid );
	Attach(pid);
}
void Process::Detach() {
	VirtualFreeEx( process_handle_, rbuf.base, 0, MEM_RELEASE );
	CloseHandle( process_handle_ );
	process_handle_ = INVALID_HANDLE_VALUE;
	rbuf.free = rbuf.base = nullptr;
}

void Process::GetRemoteModule( const char* filename, MODULEENTRY32* lpme ) {
  HANDLE snapshot = ::CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, process_id_ );
	if ( snapshot == INVALID_HANDLE_VALUE ) {
    BOOST_THROW_EXCEPTION(std::runtime_error("CreateToolhelp32SnapshotError"));
  }

  MODULEENTRY32 me;
	if ( !lpme ) lpme = &me;

	lpme->dwSize = sizeof(MODULEENTRY32);
	if (Module32First( snapshot, lpme)) {
	  do {
		  if (!_stricmp( lpme->szModule, filename)) {
			  break;
		  }
		}
		while (Module32Next( snapshot, lpme ));
	}

	CloseHandle( snapshot );
}


void Process::ReadData( void* dest, const void* src, unsigned int bytes ) {
	SIZE_T n;
  if (ReadProcessMemory( process_handle_, src, dest, bytes, &n ) == false) {
    BOOST_THROW_EXCEPTION(std::runtime_error("ReadProcessMemoryError"));
  }
}
void Process::WriteData( void* dest, const void* src, unsigned int bytes ) {
	SIZE_T n;
  if (WriteProcessMemory( process_handle_, dest, src, bytes, &n ) == false) {
    BOOST_THROW_EXCEPTION(std::runtime_error("WriteProcessMemoryError"));
  }
}


void* Process::AddResource( const void* res, unsigned int bytes ) {
	void* rs = nullptr;

	if (WriteProcessMemory(process_handle_, rbuf.free, res, bytes, nullptr )) {
		rs = rbuf.free;
		((unsigned char*&)rbuf.free) += ((bytes-1)&~3)+4;
	}

	return rs;
}

int Process::Execute( void* addr, void* param ) {
  HANDLE thread = CreateRemoteThread( process_handle_, nullptr, 0, (LPTHREAD_START_ROUTINE)addr, param, 0, nullptr );
	if (thread) {
		WaitForSingleObject(thread, 5000);
		DWORD code;
		GetExitCodeThread(thread, &code);
		CloseHandle(thread);
		return code;
	}
	return -1;
}

Module::Module() : data_(nullptr) {
}
Module::~Module() {
	if (data_ != nullptr) {
    free(data_);
  }
}

void Module::Init( Process& process, const char* dllname ) {
	if (data_ != nullptr) {
    free( data_ );
  }
	data_ = nullptr;

	process.GetRemoteModule( dllname, &module_entry_ );
	data_ = malloc( module_entry_.modBaseSize );
  if (data_ == nullptr) {
    BOOST_THROW_EXCEPTION(std::runtime_error("mallocError"));
  }

	process.ReadData(data_, module_entry_.modBaseAddr, module_entry_.modBaseSize);
}

const unsigned char* Module::FindPattern(const unsigned char* signature, const char* mask) {
  if (data_ == nullptr) {
    BOOST_THROW_EXCEPTION(std::runtime_error("data_ == nullptr"));
  }

  const unsigned char* pattern = utils::FindPattern(data_, module_entry_.modBaseSize, signature, mask);
  if (pattern == nullptr) return nullptr;

  return (const unsigned char*)(pattern - (const unsigned char*)data_);
}

uint32_t Module::GetAbsoluteAddress(uint32_t relative_address) {
   return (utils::GetAbsoluteAddress((uint32_t)data_ + relative_address) - (uint32_t)data_);
 }