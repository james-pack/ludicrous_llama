#pragma once

#include <filesystem>
#include <fstream>

#include "google/protobuf/message_lite.h"
#include "google/protobuf/text_format.h"

namespace pack::proto {

template <typename MessageT, typename = std::enable_if_t<std::is_base_of_v<google::protobuf::MessageLite, MessageT>>>
MessageT load_proto(const std::filesystem::path& filename) {
  MessageT result{};
  std::ifstream file{filename};
  result.ParseFromIstream(&file);
  return result;
}

template <typename MessageT, typename = std::enable_if_t<std::is_base_of_v<google::protobuf::MessageLite, MessageT>>>
void save_proto(const std::filesystem::path& filename, const MessageT& msg) {
  std::ofstream file{filename};
  msg.SerializeToOstream(&file);
}

template <typename MessageT, typename = std::enable_if_t<std::is_base_of_v<google::protobuf::MessageLite, MessageT>>>
MessageT load_text_proto(const std::filesystem::path& filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  const auto size = file.tellg();
  file.seekg(0);
  std::string buffer(size, '\0');
  file.read(buffer.data(), size);
  MessageT result{};
  google::protobuf::TextFormat::ParseFromString(buffer, &result);
  return result;
}

template <typename MessageT, typename = std::enable_if_t<std::is_base_of_v<google::protobuf::MessageLite, MessageT>>>
void save_text_proto(const std::filesystem::path& filename, const MessageT& msg) {
  std::string buffer{};
  google::protobuf::TextFormat::PrintToString(msg, &buffer);
  std::ofstream file(filename, std::ios::binary);
  file.write(buffer.data(), buffer.length());
}

}  // namespace pack::proto
