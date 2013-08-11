#pragma once

#include "source-sdk/source_sdk.h"
#include <string>
#include <unordered_map>

namespace sourcesdk {

class NetVarManager {
  public:

    static NetVarManager& GetInstance() {
      static NetVarManager  instance;

      return instance;
    }

    int GetNetVarOffset(const char* class_name, const char* var_name);
    int GetNetVarOffsetL(const char* class_name, const char* var_name, const char* var_name2);
  private:
    NetVarManager() {};
    int GetOffsetFromTable(const char* class_name, const char* var_name, RecvTable* table);
    int GetOffsetFromTableL(const char* class_name, const char* var_name, const char* var_name2, RecvTable* table);

    std::unordered_map<std::string, int> cache_;

    NetVarManager(NetVarManager const&);
    void operator=(NetVarManager const&);
};
}