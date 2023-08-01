#pragma once

#include "../Byte-stream/byte_stream.h"
#include "../IO/fowrapper.h"
#include "../consts.h"

namespace huffman::compressor {
void compress_tree(huffman::consts::huf_tree& tree, fowrapper& res);
void compress_code(byte_stream& code, fowrapper& res);
void write_number_significant_bits(byte_stream& code, fowrapper& res);
} // namespace huffman::compressor
