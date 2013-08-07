#include <iostream>
#include <fstream>

#include "signature_pattern_retriever.h"
#include "process.h"

#include <boost/exception/diagnostic_information.hpp> 

int main() {

  ExternalAddressRetriever address_retriever("static", "Dota 2");
  DynamicExternalAddressRetriever dynamic_retriever("dynamic", "Dota 2");

  try {
    address_retriever.LoadSignatures("config.yaml");
    for (auto address : address_retriever) {
      std::cout << std::hex << address.first << ": " << address.second << std::endl;
    }
  } catch( boost::exception &e) {
    std::cerr << boost::diagnostic_information(e) << std::endl;
  }

  try {
    dynamic_retriever.LoadSignatures("config.yaml");
    for (auto address : dynamic_retriever) {
      std::cout << std::hex << address.first << ": " << address.second << std::endl;
    }
  } catch( boost::exception &e) {
    std::cerr << boost::diagnostic_information(e) << std::endl;
  }

  system("pause");
  return 0;
}