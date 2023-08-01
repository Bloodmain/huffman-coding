#include "compressor.h"
#include "consts.h"
#include "decompressor.h"
#include "fiwrapper.h"
#include "fowrapper.h"
#include "tree.h"

#include <tclap/CmdLine.h>

#include <chrono>
#include <iostream>
#include <string>

template <typename... Args>
static void status_info(bool print, Args... args) {
  if (print) {
    (std::cout << ... << args);
    std::cout << std::endl;
  }
}

int main(int argc, char** argv) {
  using namespace huffman;

  bool compress_flag, decompress_flag, status_flag;
  std::string input_file, output_file;

  try {
    TCLAP::CmdLine cmd("It's a tool for (de-)compressing files with Huffman algorithm.", ' ', HUF_TOOL_PROJECT_VERSION,
                       true);

    TCLAP::SwitchArg compressing("c", "compress", "Compress input file");
    TCLAP::SwitchArg decompressing("d", "decompress", "Decompress input file");
    cmd.xorAdd(compressing, decompressing); // only one of the options allowed

    TCLAP::SwitchArg status("s", "status", "Enable status output", cmd);

    TCLAP::ValueArg<std::string> input("i", "input", "Input file", true, "", "string", cmd);
    TCLAP::ValueArg<std::string> output("o", "output", "Output file", true, "", "string", cmd);

    cmd.parse(argc, argv);
    compress_flag = compressing.getValue();
    decompress_flag = decompressing.getValue();
    status_flag = status.getValue();
    input_file = input.getValue();
    output_file = output.getValue();

  } catch (TCLAP::ArgException& e) {
    std::cerr << "Error occurred while parsing command line arguments: \n";
    std::cerr << e.what();
    return consts::exit_codes::PO_ERROR;
  }

  auto start = std::chrono::high_resolution_clock::now();
  try {
    if (compress_flag) {
      status_info(status_flag, "Compressing...");

      fiwrapper input(input_file);
      fowrapper output(output_file);

      consts::huf_freq freq{};
      while (!input.eof()) {
        ++freq[static_cast<unsigned char>(input.next_char())];
      }
      tree hf(freq);

      auto end = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      status_info(status_flag, "Tree's building completed. Time taken: ", duration.count(), "ms.");

      input.seekg(0, std::ios_base::beg);
      compressor::compress_tree(hf.get_tree(), output);

      byte_stream code;
      while (!input.eof()) {
        hf.encode(input.next_char(), code);
        compressor::compress_code(code, output);
        code.clear();
      }
      code.ensure_last_word();
      compressor::compress_code(code, output);
      compressor::write_number_significant_bits(code, output);

      end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      status_info(status_flag, "Compressing completed. Time taken: ", duration.count(), "ms.");

    } else {
      status_info(status_flag, "Decompressing...");
      fiwrapper input(input_file);
      fowrapper output(output_file);

      std::uint8_t last = decompressor::get_last_significant_bits(input);
      consts::huf_tree tr = decompressor::decompress_tree(input);
      tree hf(tr);

      auto end = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      status_info(status_flag, "Tree's building completed. Time taken: ", duration.count(), "ms.");

      while (!input.last_char() && !input.eof()) {
        decompressor::decompress_code(input, output, hf, last);
      }
      if (!input.last_char()) {
        throw std::runtime_error("[Decompressor error] Decompressing corrupted file");
      }

      end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      status_info(status_flag, "Decompressing completed. Time taken: ", duration.count(), "ms.");
    }
  } catch (const std::runtime_error& ex) {
    std::cerr << "Error occured:\n";
    std::cerr << ex.what() << std::endl;
    return consts::exit_codes::HUFFMAN_ERROR;
  }
}
