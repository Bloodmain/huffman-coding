#pragma once

#include "../consts.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>

namespace huffman {
class fiwrapper {
  std::ifstream file;
  std::array<char, huffman::consts::BUF_SIZE> buffer;
  std::streamsize buffer_available = 0;
  std::size_t buffer_pos = 0;

  void read_buffer();

public:
  explicit fiwrapper(std::string filename);
  ~fiwrapper();

  bool eof();
  char next_char();
  void seekg(std::ptrdiff_t off, std::ios_base::seekdir dir);
  bool last_char();
};
} // namespace huffman
