#pragma once

#include <boost/throw_exception.hpp>
#include <map>

#include "process.h"

namespace addressretriever {

class AddressRetriever {
 public:
  AddressRetriever(std::string node_name);

  void LoadSignatures(const std::string& file_path);

  typedef std::map<std::string, uint32_t>::const_iterator AddressIterator;
  AddressIterator begin() {
    return addresses_.begin();
  }
  AddressIterator end() {
    return addresses_.end();
  }

  virtual uint32_t GetAddress(std::string name) {
    return addresses_[name];
  }

    protected:
  virtual uint32_t ProcessAddress(std::string module_name, Signature signature, std::map<std::string, int> commands) = 0;

   std::map<std::string, uint32_t> addresses_;
   std::string node_name_;
};

class ExternalAddressRetriever : public AddressRetriever {
public:
  ExternalAddressRetriever(std::string node_name, std::string process_name);
 protected:
  uint32_t ProcessAddress(std::string module_name, Signature signature, std::map<std::string, int> commands);


  std::string process_name_;
};

class InternalAddressRetriever : public AddressRetriever {
public:
  InternalAddressRetriever(std::string node_name) : AddressRetriever(node_name) {};
  protected:
  uint32_t ProcessAddress(std::string module_name, Signature signature, std::map<std::string, int> commands);
};


class DynamicExternalAddressRetriever : public ExternalAddressRetriever {
public:
  DynamicExternalAddressRetriever(std::string node_name, std::string process_name) : ExternalAddressRetriever(node_name, process_name) {};

  uint32_t GetDynamicSignature(std::string name) {
    Process process;
    process.Attach(process_name_.c_str());
    return process.ReadData<uint32_t>((const void*)addresses_[name]);
  }
};

class DynamicInternalAddressRetriever : public InternalAddressRetriever {
public:
  DynamicInternalAddressRetriever(std::string node_name) : InternalAddressRetriever(node_name) {};

  uint32_t GetDynamicSignature(std::string name) {
    return *(uint32_t*)(addresses_[name]);
  }
};

}