#pragma once

#include <cstdint>
#include <vector>

namespace huffman {
class byte_stream {
  std::vector<std::uint64_t> data;

  std::uint64_t now = 0;
  std::size_t sz = 0;
  std::uint8_t now_bit = 0;

  std::size_t reading_uint = 0;
  std::uint8_t reading_bit = 0;

public:
  byte_stream& push(std::uint64_t v, std::uint8_t size);
  byte_stream& push(byte_stream& other);
  byte_stream& pop();

  std::vector<std::uint64_t>& get_data();
  void ensure_last_word();
  void clear();
  bool get();

  std::size_t size() const;
  bool empty() const;
  std::uint8_t number_last_significant_bits() const;
};
} // namespace huffman
