#include "global_address_retriever.h"

GlobalAddressRetriever& GlobalAddressRetriever::GetInstance() {
  static GlobalAddressRetriever  instance;
  return instance;
}
