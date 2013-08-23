// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include <boost/exception/all.hpp>
#include <string>

#include "address-retriever/address-retriever/signature_pattern_retriever.h"
#include "utils/utils.h"

class GlobalAddressRetriever {
 public:
  static GlobalAddressRetriever& GetInstance();

  uint32_t GetStaticAddress(std::string name) {
    return static_addresses_.GetAddress(name);
  }

  uint32_t GetDynamicAddress(std::string name) {
    return dynamic_addresses_.GetDynamicSignature(name);
  }

 private:
  GlobalAddressRetriever() : static_addresses_("static"),
                             dynamic_addresses_("dynamic") {
    try {
      static_addresses_.LoadSignatures(
        utils::GetModuleDirectory() + "config.yaml");
    } catch(const boost::exception &e) {
      Warning("hook: GlobalAddressRetrieverError %s \n",
        boost::diagnostic_information(e));
    } catch(...) {
      Warning("hook: GlobalAddressRetrieverError");
    }

    for (auto address : static_addresses_) {
      Warning("hook: static: %s 0x%x \n",
                 address.first.c_str(),
                 address.second);
    }

    try {
      dynamic_addresses_.LoadSignatures(
        utils::GetModuleDirectory() + "config.yaml");
    } catch(const boost::exception &e) {
      Warning("hook: GlobalAddressRetrieverError %s \n",
        boost::diagnostic_information(e));
    } catch(...) {
      Warning("hook: GlobalAddressRetrieverError");
    }

    for (auto address : dynamic_addresses_) {
      Warning("hook: dynamic: %s 0x%x \n",
                 address.first.c_str(),
                 address.second);
    }
  }

  addressretriever::InternalAddressRetriever static_addresses_;
  addressretriever::DynamicInternalAddressRetriever dynamic_addresses_;
};
