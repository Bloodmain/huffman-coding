#include "compressor.h"

static void convert_uint64_to_chars(std::uint64_t v, std::uint8_t size, huffman::fowrapper& res) {
  for (std::size_t i = 0; i < size; i += huffman::consts::GROUP) {
    char ch = v & ((1u << huffman::consts::GROUP) - 1);
    v >>= huffman::consts::GROUP;
    res.write(ch);
  }
}

void huffman::compressor::compress_code(huffman::byte_stream& code, huffman::fowrapper& res) {
  for (std::uint64_t v : code.get_data()) {
    convert_uint64_to_chars(v, consts::BITS, res);
  }
}

void huffman::compressor::compress_tree(huffman::consts::huf_tree& tree, huffman::fowrapper& res) {
  std::vector<std::pair<char, byte_stream&>> to_write;
  for (std::uint16_t i = 0; i < tree.size(); ++i) {
    if (!tree[i].empty()) {
      to_write.emplace_back(i, tree[i]);
    }
  }

  if (to_write.empty()) {
    res.write(1);
    return;
  }

  res.write(0);
  res.write(to_write.size() - 1);
  for (auto [ch, stream] : to_write) {
    res.write(ch);
    res.write(stream.size());
    for (std::size_t i = 0; i < stream.get_data().size() - 1; ++i) {
      convert_uint64_to_chars(stream.get_data()[i], consts::BITS, res);
    }
    convert_uint64_to_chars(stream.get_data().back(), stream.number_last_significant_bits(), res);
  }
}

void huffman::compressor::write_number_significant_bits(huffman::byte_stream& code, huffman::fowrapper& res) {
  res.write(code.number_last_significant_bits());
}
