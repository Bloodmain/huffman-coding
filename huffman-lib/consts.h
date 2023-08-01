#pragma once
#include "Byte-stream/byte_stream.h"

#include <array>
#include <cstdint>
#include <string>

namespace huffman::consts {
constexpr int BUF_SIZE = 8192;
constexpr int UNIQUE_BYTES = 256;
constexpr std::uint8_t GROUP = 8;
static constexpr std::uint8_t BITS = sizeof(std::uint64_t) * GROUP;

using huf_freq = std::array<std::uint64_t, UNIQUE_BYTES>;
using huf_tree = std::array<byte_stream, consts::UNIQUE_BYTES>;

enum exit_codes {
  SUCCESS = 0,
  NO_TYPE_PROVIDED = 101,
  NO_FILE_PROVIDED = 102,
  PO_ERROR = 103,
  HUFFMAN_ERROR = 104
};
} // namespace huffman::consts
