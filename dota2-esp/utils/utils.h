#pragma once

#include <cstdint>
#include <windows.h>

namespace utils {
  const unsigned char* FindPattern(const void* address, size_t size, const unsigned char* signature, const char* mask);
  const unsigned char* FindPattern(const char* module_name, const unsigned char* signature, const char* mask);
  const uint32_t* FindPattern(const char* module_name, const unsigned char* signature, const char* mask, int offset);
  uint32_t GetAbsoluteAddress(uint32_t relative_address);
  size_t GetModuleSize(const char* module_name);
  HMODULE GetModuleHandleSafe(const char* module_name );
  void Log(const char* format, ...);
}