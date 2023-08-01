#include "decompressor.h"

#include <array>

static std::uint64_t read_uint64(huffman::fiwrapper& from) {
  std::uint64_t v = 0;
  for (std::size_t j = 0; j < huffman::consts::GROUP; ++j) {
    auto ch = static_cast<unsigned char>(from.next_char());
    v |= static_cast<std::uint64_t>(ch) << (j * huffman::consts::GROUP);
  }
  return v;
}

std::uint8_t huffman::decompressor::get_last_significant_bits(huffman::fiwrapper& from) {
  // (the last char in compressed file stores the number of significant bits in the last uint64)

  from.seekg(-1, std::ios_base::end);
  std::uint8_t res = from.next_char();
  from.seekg(0, std::ios_base::beg);
  return res;
}

huffman::consts::huf_tree huffman::decompressor::decompress_tree(huffman::fiwrapper& from) {
  huffman::consts::huf_tree tree{};

  bool empty = from.next_char(); // the first byte is 1 if the tree is empty, 0 otherwise
  if (empty) {
    return tree;
  }

  std::size_t size =
      static_cast<unsigned char>(from.next_char()) + 1; // next byte is the tree's size - 1 (to fit in one char)

  for (std::size_t i = 0; i < size; ++i) {
    unsigned char ch = from.next_char();       // the char itself
    std::int16_t code_size = from.next_char(); // the size of char's code
    while (code_size > 0) {
      unsigned char code_part = from.next_char();
      tree[ch].push(code_part, std::min(static_cast<std::int16_t>(consts::GROUP), code_size));
      code_size -= consts::GROUP;
    }
    tree[ch].ensure_last_word();
  }

  return tree;
}

void huffman::decompressor::decompress_code(huffman::fiwrapper& from, huffman::fowrapper& res, tree& hf,
                                            std::uint8_t last_significant_bits) {
  std::uint64_t next = read_uint64(from);
  std::uint8_t sz = from.last_char() ? last_significant_bits : huffman::consts::BITS;
  for (std::uint16_t i = 0; i < sz; ++i) {
    hf.traverse_with_bit(next & 1, res);
    next >>= 1;
  }
}
