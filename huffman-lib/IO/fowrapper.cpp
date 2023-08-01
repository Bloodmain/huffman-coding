#include "fowrapper.h"

#include "../consts.h"

#include <stdexcept>

huffman::fowrapper::fowrapper(std::string filename) {
  file.open(filename, std::ios_base::out | std::ios_base::binary);
  if (file.fail()) {
    throw std::runtime_error(std::string("[Output wrapper] Error while opening/creating file \"") + filename + "\"");
  }
}

huffman::fowrapper::~fowrapper() {
  if (buffer_pos) {
    file.write(buffer.data(), buffer_pos);
  }
  file.close();
}

void huffman::fowrapper::write(char ch) {
  if (buffer_pos == huffman::consts::BUF_SIZE) {
    file.write(buffer.data(), buffer_pos);
    buffer_pos = 0;
    if (file.bad()) {
      throw std::runtime_error("[Output wrapper] Error while writing to the output file");
    }
    buffer[buffer_pos++] = ch;
    return;
  }
  buffer[buffer_pos++] = ch;
}
