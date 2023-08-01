#pragma once

#include "../consts.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>

namespace huffman {
class fowrapper {
  std::ofstream file;
  std::array<char, huffman::consts::BUF_SIZE> buffer;
  std::streamsize buffer_pos = 0;

public:
  explicit fowrapper(std::string filename);
  ~fowrapper();

  void write(char ch);
};
} // namespace huffman
