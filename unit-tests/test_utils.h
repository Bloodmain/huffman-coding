#pragma once
#include "decompressor.h"
#include "tree.h"

#include <string>
using namespace huffman;

tree build_huffman(std::string str);
void compress(const std::string& filename, const std::string& str);
std::string decompress(const std::string& filename);
std::string compress_decompress(const std::string& str);
std::string get_data_from_file(const std::string& filename);
std::string from_unit_test_data(const std::string& path);
std::string read(const std::string& filename);
void write(const std::string& filename, const std::string& str);
bool ith(byte_stream& stream, std::size_t i);
