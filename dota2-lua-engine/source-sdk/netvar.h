// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include <unordered_map>
#include <string>


#include "source-sdk/source_sdk.h"

namespace sourcesdk {

class NetVarManager {
 public:
  static NetVarManager& GetInstance() {
    static NetVarManager  instance;

    return instance;
  }

  int GetNetVarOffset(const char* class_name, const char* var_name);
  int GetNetVarOffsetL(const char* class_name, const char* var_name,
                        const char* var_name2);
 private:
  NetVarManager() {}
  int GetOffsetFromTable(const char* class_name,
                          const char* var_name, RecvTable* table);
  int GetOffsetFromTableL(const char* class_name,
                          const char* var_name,
                          const char* var_name2, RecvTable* table);

  std::unordered_map<std::string, int> cache_;

  explicit NetVarManager(NetVarManager const&);
  void operator=(NetVarManager const&);
};

}  // namespace sourcesdk
