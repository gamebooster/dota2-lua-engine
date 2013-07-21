#include "SDK.h"

namespace sourcesdk {

class NetVarManager {
  public:
    NetVarManager(CHLClient* client);

    int GetNetVarOffset(const char* class_name, const char* var_name);
    int GetNetVarOffsetL(const char* class_name, const char* var_name, const char* var_name2);
  private:
    int GetOffsetFromTable(const char* class_name, const char* var_name, RecvTable* table);
    int GetOffsetFromTableL(const char* class_name, const char* var_name, const char* var_name2, RecvTable* table);
    CHLClient* client_;
};
}