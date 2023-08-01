#include "byte_stream.h"

#include "../consts.h"

using namespace huffman;

byte_stream& byte_stream::push(std::uint64_t v, std::uint8_t size) {
  if (size + now_bit <= huffman::consts::BITS) {
    now |= v << now_bit;
    now_bit += size;
    if (now_bit == huffman::consts::BITS) {
      now_bit = 0;
      data.push_back(now);
      now = 0;
    }
  } else {
    std::size_t rest = huffman::consts::BITS - now_bit;
    now |= (v & ((1 << rest) - 1)) << now_bit;
    data.push_back(now);

    now_bit = size - rest;
    now = v >> rest;
  }

  sz += size;
  return *this;
}

byte_stream& byte_stream::push(byte_stream& other) {
  std::vector<std::uint64_t>& oth_data = other.get_data();
  for (int i = 0; i < oth_data.size() - 1; ++i) {
    push(oth_data[i], huffman::consts::BITS);
  }
  push(oth_data.back(), other.number_last_significant_bits());
  return *this;
}

byte_stream& byte_stream::pop() {
  if (now_bit == 0) {
    now_bit = huffman::consts::BITS - 1;
    now = data.back();
    data.pop_back();
  } else {
    --now_bit;
  }
  now &= ~(1ull << now_bit);
  --sz;
  return *this;
}

std::vector<std::uint64_t>& byte_stream::get_data() {
  return data;
}

void byte_stream::clear() {
  data.clear();
  sz = 0;
}

bool byte_stream::empty() const {
  return data.empty() && now_bit == 0;
}

std::size_t byte_stream::size() const {
  return sz;
}

void byte_stream::ensure_last_word() {
  if (now_bit != 0) {
    data.push_back(now);
    now = 0;
  }
}

std::uint8_t byte_stream::number_last_significant_bits() const {
  return now_bit == 0 ? huffman::consts::BITS : now_bit;
}

bool byte_stream::get() {
  bool res = data[reading_uint] & (1 << reading_bit);
  ++reading_bit;
  if (reading_bit == consts::BITS) {
    reading_bit = 0;
    ++reading_uint;
  }
  return res;
}
