#include "precompiled_headers.h"

#include "netvar.h"
#include "global_instance_manager.h"

namespace sourcesdk {

#undef GetProp

int NetVarManager::GetOffsetFromTable(const char* class_name, const char* var_name, RecvTable* table) {
  int offset = 0;

  for( int i = 0; i < table->GetNumProps(); i++ ) {
    RecvProp* prop = table->GetProp(i);
    if( !prop ) continue;

    if( !strcmp( table->GetName(), class_name ) && !strcmp(prop->GetName(), var_name)) {
      offset += prop->GetOffset();
      break;
    }
  }

  return offset;
}

int NetVarManager::GetOffsetFromTableL(const char* class_name, const char* var_name, const char* var_name2, RecvTable* table) {
  int offset = 0;

  for( int i = 0; i < table->GetNumProps(); i++ ) {
    RecvProp* prop = table->GetProp(i);
    if( !prop ) continue;

    RecvTable* child = prop->GetDataTable();
    if(!strcmp( table->GetName(), class_name) && !strcmp(prop->GetName(), var_name))
    {
      RecvTable* child2 = prop->GetDataTable();
      if( !child2 ) continue;
      for( int j = 0; j < child2->GetNumProps(); j++ )
      {
        RecvProp* childProp = child->GetProp( j );
        if( !childProp ) continue;
        if( !strcmp( childProp->GetName(), var_name2 ) )
        {
          offset += childProp->GetOffset();
          break;
        }
      }
    }
  }

  return offset;
}

int NetVarManager::GetNetVarOffset(const char* class_name, const char* var_name) {
  std::string key(class_name);
  key.append(var_name);
  if (cache_[key]) return cache_[key];

  ClientClass* clientClass = GlobalInstanceManager::GetClient()->GetAllClasses();

  int offset = 0;

  for( ; clientClass; clientClass = clientClass->NextClass() )
  {
    RecvTable* table = clientClass->GetTable();

    if( !table ) continue;
    if( table->GetNumProps() <= 0 ) continue;
    if( !strstr( table->GetName(), "DT_" ) ) continue;

    offset = GetOffsetFromTable( class_name, var_name, table );
    if( offset != 0 ) break;
  }

  cache_[key] = offset;
  return offset;
}

int NetVarManager::GetNetVarOffsetL(const char* class_name, const char* var_name, const char* var_name2) {
  std::string key(class_name);
  key.append(var_name);
  key.append(var_name2);
  if (cache_[key]) return cache_[key];

  ClientClass* clientClass = GlobalInstanceManager::GetClient()->GetAllClasses();

  int offset = 0;

  for( ; clientClass; clientClass = clientClass->NextClass() )
  {
    RecvTable* table = clientClass->GetTable();

    if( !table ) continue;
    if( table->GetNumProps() <= 0 ) continue;
    if( !strstr( table->GetName(), "DT_" ) ) continue;

    offset = GetOffsetFromTableL( class_name, var_name, var_name2, table );
    if( offset != 0 ) break;
  }

  cache_[key] = offset;
  return offset;
}

}