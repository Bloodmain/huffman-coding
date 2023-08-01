#include "fiwrapper.h"

#include "../consts.h"

#include <stdexcept>

huffman::fiwrapper::fiwrapper(std::string filename) {
  file.open(filename, std::ios_base::in | std::ios_base::binary);
  if (file.fail()) {
    throw std::runtime_error(std::string("[Input wrapper] Unable to open file \"") + filename + "\"");
  }
}

huffman::fiwrapper::~fiwrapper() {
  file.close();
}

void huffman::fiwrapper::read_buffer() {
  file.read(buffer.data(), huffman::consts::BUF_SIZE);
  buffer_pos = 0;
  if (file.fail() && !file.eof()) {
    throw std::runtime_error("[Input wrapper] Error during reading input file");
  }
  buffer_available = file.gcount();
}

bool huffman::fiwrapper::eof() {
  if (buffer_pos >= buffer_available && !file.eof()) {
    read_buffer();
  }
  return buffer_pos >= buffer_available && file.eof();
}

char huffman::fiwrapper::next_char() {
  if (buffer_pos < buffer_available) {
    return buffer[buffer_pos++];
  } else {
    read_buffer();
    return buffer[buffer_pos++];
  }
}

void huffman::fiwrapper::seekg(std::ptrdiff_t off, std::ios_base::seekdir dir) {
  file.clear();
  file.seekg(off, dir);
  buffer_available = 0;
  buffer_pos = 0;
}

bool huffman::fiwrapper::last_char() {
  eof();
  return file.eof() && buffer_available - buffer_pos == 1;
}
