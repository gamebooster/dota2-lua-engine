// Copyright 2013 Karl Skomski - GPL v3
#include "global_address_retriever.h"

GlobalAddressRetriever& GlobalAddressRetriever::GetInstance() {
  static GlobalAddressRetriever  instance;
  return instance;
}
