// Copyright 2013 Karl Skomski - GPL v3
#include "utils/global_address_retriever.h"

GlobalAddressRetriever& GlobalAddressRetriever::GetInstance() {
  static GlobalAddressRetriever  instance;
  return instance;
}
