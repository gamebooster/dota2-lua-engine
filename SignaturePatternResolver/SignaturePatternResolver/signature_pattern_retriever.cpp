#include "signature_pattern_retriever.h"

#include "yaml-cpp\yaml.h"
#include "utils\utils.h"

namespace addressretriever {

AddressRetriever::AddressRetriever(std::string node_name) : node_name_(node_name) {

}

void AddressRetriever::LoadSignatures(const std::string& file_path) {
  YAML::Node config = YAML::LoadFile(file_path);

  if (config[node_name_] == false) {
    BOOST_THROW_EXCEPTION(std::runtime_error("node_name does not exist"));
  }

  YAML::Node modules = config[node_name_];

  for(auto module_sequence : modules) {
    std::string module_name = module_sequence.first.as<std::string>();

    YAML::Node sequence = module_sequence.second;

    for(auto node : sequence) {
      if (node["name"] == false) {
        BOOST_THROW_EXCEPTION(std::runtime_error("pattern object without name")); 
      }
      if (node["signature"] == false) {
        BOOST_THROW_EXCEPTION(std::runtime_error("pattern object without signature")); 
      }

      std::map<std::string, int> command_map;
      if (node["commands"]) {
        YAML::Node commands = node["commands"];

        for(auto command : commands) {
          command_map[command.first.as<std::string>()] = command.second.as<int>();
        }
      }

      uint32_t address = ProcessAddress(module_name, Signature(node["signature"].as<std::string>()), command_map);
      if (address == 0) {
        BOOST_THROW_EXCEPTION(std::runtime_error("could not find address"));
      }
      addresses_[node["name"].as<std::string>()] = address;
    }
  }
}

ExternalAddressRetriever::ExternalAddressRetriever(std::string node_name, std::string process_name) : AddressRetriever(node_name), process_name_(process_name) {

}

uint32_t ExternalAddressRetriever::ProcessAddress(std::string module_name, Signature signature, std::map<std::string, int> commands) {
  Process process;
  process.Attach(process_name_.c_str());

  Module module;
  module.Init(process, module_name.c_str());

  uint32_t address = (uint32_t)module.FindPattern(signature);

  for (auto command : commands) {
    if (command.first == "offset") {
      address += command.second;
    }
    if (command.first == "relative") {
      address = module.GetAbsoluteAddress(address);
    }
  }

  return (uint32_t)module.GetAddress(address);
}
uint32_t InternalAddressRetriever::ProcessAddress(std::string module_name, Signature signature, std::map<std::string, int> commands) {
  uint32_t address = (uint32_t)utils::FindPattern(module_name.c_str(), reinterpret_cast<const unsigned char*>(signature.pattern.c_str()), signature.mask.c_str());

  if (address == 0) {
    BOOST_THROW_EXCEPTION(std::runtime_error("could not find pattern"));
  }

  for (auto command : commands) {
    if (command.first == "offset") {
      address += command.second;
    }
    if (command.first == "relative") {
      address = utils::GetAbsoluteAddress(address);
    }
  }
  return address;
}

}