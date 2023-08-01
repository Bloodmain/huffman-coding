#include "test_utils.h"

#include "compressor.h"
#include "decompressor.h"
#include "fiwrapper.h"
#include "fowrapper.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace huffman;

tree build_huffman(std::string str) {
  consts::huf_freq freq{};
  for (char ch : str) {
    ++freq[static_cast<unsigned char>(ch)];
  }
  tree hf(freq);
  return hf;
}

void compress(const std::string& str, const std::string& filename = from_unit_test_data("tmp_.txt")) {
  fowrapper out(filename);
  tree hf = build_huffman(str);
  consts::huf_tree tr = hf.get_tree();
  compressor::compress_tree(tr, out);

  byte_stream code;
  for (char ch : str) {
    hf.encode(ch, code);
    compressor::compress_code(code, out);
    code.clear();
  }
  code.ensure_last_word();
  compressor::compress_code(code, out);
  compressor::write_number_significant_bits(code, out);
}

std::string decompress(const std::string& filename = from_unit_test_data("tmp_.txt")) {
  {
    fiwrapper in(filename);
    fowrapper out(from_unit_test_data("tmp2_.txt"));
    std::uint8_t last = decompressor::get_last_significant_bits(in);
    consts::huf_tree tr = decompressor::decompress_tree(in);
    tree hf(tr);

    while (!in.last_char() && !in.eof()) {
      decompressor::decompress_code(in, out, hf, last);
    }
    if (!in.last_char()) {
      throw std::runtime_error("[Test error] Decompressing corrupted file");
    }
  }
  return get_data_from_file(from_unit_test_data("tmp2_.txt"));
}

std::string compress_decompress(const std::string& str) {
  compress(str);
  return decompress();
}

std::string get_data_from_file(const std::string& filename) {
  std::ifstream file;
  file.open(filename, std::ios_base::in | std::ios_base::binary | std::ios::ate);
  std::size_t count = file.tellg();
  if (file.fail()) {
    throw std::runtime_error(std::string("[Test error] Can't open file \"") + filename + "\"");
  }
  std::string res;
  res.resize(count);
  file.seekg(0);
  file.read(&res[0], count);
  file.close();
  return res;
}

std::string from_unit_test_data(const std::string& path) {
  return std::string(CMAKE_CURRENT_SOURCE_DIR) + "/data/" + path;
}

std::string read(const std::string& filename) {
  fiwrapper in(filename);
  std::string res;
  while (!in.eof()) {
    res.push_back(in.next_char());
  }
  return res;
}

void write(const std::string& filename, const std::string& str) {
  fowrapper out(filename);
  for (char ch : str) {
    out.write(ch);
  }
}

bool ith(byte_stream& stream, std::size_t i) {
  return (stream.get_data()[i / consts::BITS] >> (i % consts::BITS)) & 1;
}
