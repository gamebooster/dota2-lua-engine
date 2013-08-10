#include "precompiled_headers.h"

#include "utils.h"

#include <stdint.h>
#include <string>
#include <cassert>

__forceinline bool FindPatternCompare(const unsigned char* address, const unsigned char* signature, const char* mask) {
  while(*mask) {
    if(*reinterpret_cast<const uint32_t*>(mask) == 'xxxx') {
      if(*reinterpret_cast<const uint32_t*>(address) != *reinterpret_cast<const uint32_t*>(signature)) {
        return false;
      }

      address += 4;
      signature += 4;
      mask += 4;
      continue;
    } else if(*reinterpret_cast<const uint16_t*>(mask) == 'xx') {
      if(*reinterpret_cast<const uint16_t*>(address) != *reinterpret_cast<const uint16_t*>(signature)) {
        return false;
      }

      address += 2;
      signature += 2;
      mask += 2;
      continue;		
    } else if(*mask == 'x' && *address != *signature) {
      return false;
    }

    ++address;
    ++signature;
    ++mask;
  }

  return *mask == 0;
}

const unsigned char* FindPatternInternal(const unsigned char* address, size_t size, const unsigned char* signature, const char* mask) {
  for(size_t i = 0; i < size; i++) {
    if(FindPatternCompare(address + i, signature, mask)) {
      return address + i;
    }
  }

  return nullptr;
}

const unsigned char* utils::FindPattern(const void* address, size_t size, const unsigned char* signature, const char* mask) {
  assert(address != nullptr);
  assert(size != 0);
  assert(signature != nullptr);
  assert(mask != nullptr);

  size_t mask_length = strlen(mask);

  assert(mask_length != 0);
  assert(mask_length < size);

  return FindPatternInternal(reinterpret_cast<const unsigned char*>(address), size - mask_length, signature, mask);
}

size_t utils::GetModuleSize(const char* module_name) {
  HMODULE hModule = GetModuleHandle(module_name);
  if(hModule == NULL) return 0;
  IMAGE_DOS_HEADER* pDOSHeader = (IMAGE_DOS_HEADER*)hModule;
  IMAGE_NT_HEADERS* pNTHeaders =(IMAGE_NT_HEADERS*)((BYTE*)pDOSHeader + pDOSHeader->e_lfanew);
  return pNTHeaders->OptionalHeader.SizeOfImage;
}

const unsigned char* utils::FindPattern(const char* module_name, const unsigned char* signature, const char* mask) {
  HMODULE module = GetModuleHandle(module_name);
  if(module == NULL) return nullptr;
  return utils::FindPattern(reinterpret_cast<void*>(module), GetModuleSize(module_name), signature, mask);
}

const uint32_t* utils::FindPattern(const char* module_name, const unsigned char* signature, const char* mask, int offset) {
  HMODULE module = GetModuleHandle(module_name);
  if(module == NULL) return nullptr;
  const unsigned char* pattern_address = utils::FindPattern(reinterpret_cast<void*>(module), GetModuleSize(module_name), signature, mask);
  if (pattern_address == nullptr) return nullptr;
  return reinterpret_cast<uint32_t*>(*(uint32_t*)(pattern_address + offset));
}

uint32_t utils::GetAbsoluteAddress(uint32_t relative_address) {
  uint32_t destination = (uint32_t)(relative_address + 5 + *(int*)(relative_address + 1));
  return destination;
}

HMODULE utils::GetModuleHandleSafe(const char* module_name) {
  HMODULE hmModuleHandle = NULL;

  do {
    hmModuleHandle = GetModuleHandle(module_name);
    Sleep( 1 );
  }
  while(hmModuleHandle == NULL);

  return hmModuleHandle;
}

void utils::Log(const char* format, ...) {
  char buffer[MAX_PATH];

  va_list args; 
  va_start(args, format);
  vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer), format, args);
  va_end(args);
  OutputDebugString(buffer);
}

const std::wstring utils::ConvertToWide(const char *text) {
  const size_t size = strlen(text)+1;
  std::wstring wtext( size, L'#' );
  mbstowcs( &wtext[0], text, size );
  return wtext;
}

const std::string utils::ConvertToANSI(const wchar_t *text) {
  const size_t size = wcslen(text)+1;
  std::string wtext( size, '#' );
  WideCharToMultiByte(CP_UTF8, 0, text, -1, &wtext[0], size, NULL, NULL);
  return wtext;
}

int utils::ConvertANSIToUnicode(const char *ansi, wchar_t *unicode, int unicodeBufferSizeInBytes) {
  int chars = MultiByteToWideChar(CP_UTF8, 0, ansi, -1, unicode, unicodeBufferSizeInBytes / sizeof(wchar_t));
  unicode[(unicodeBufferSizeInBytes / sizeof(wchar_t)) - 1] = 0;
  return chars;
}

int utils::ConvertUnicodeToANSI(const wchar_t *unicode, char *ansi, int ansiBufferSize) {
  int result = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, ansi, ansiBufferSize, NULL, NULL);
  ansi[ansiBufferSize - 1] = 0;
  return result;
}

std::string utils::GetModulePath(HMODULE module) {
  char path[MAX_PATH];
  GetModuleFileName(module, path, MAX_PATH);
  return std::string(path);
}

 extern "C" IMAGE_DOS_HEADER __ImageBase;

HMODULE utils::GetCurrentModule() {
 return (HINSTANCE)&__ImageBase;
}

std::string utils::GetModuleDirectory() {
  std::string module_path_with_filename = utils::GetModulePath(utils::GetCurrentModule());
  char module_drive[_MAX_DRIVE];
  char module_dir[_MAX_DIR];

  _splitpath(module_path_with_filename.c_str(), module_drive, module_dir, NULL, NULL);

  std::string module_path(module_drive);
  module_path += module_dir;

  return module_path;
}