#include "precompiled_headers.h"

#include "global_address_retriever.hpp"

GlobalAddressRetriever& GlobalAddressRetriever::GetInstance() {
  static GlobalAddressRetriever  instance;
  return instance;
}