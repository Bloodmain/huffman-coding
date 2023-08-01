#pragma once

#include "../Byte-stream/byte_stream.h"
#include "../IO/fiwrapper.h"
#include "../IO/fowrapper.h"
#include "../Tree/tree.h"
#include "../consts.h"

namespace huffman::decompressor {
std::uint8_t get_last_significant_bits(huffman::fiwrapper& from);
huffman::consts::huf_tree decompress_tree(fiwrapper& from);
void decompress_code(fiwrapper& from, fowrapper& res, tree& hf, std::uint8_t last_significant_bits);
} // namespace huffman::decompressor
