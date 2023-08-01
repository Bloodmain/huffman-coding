#include "fiwrapper.h"
#include "fowrapper.h"
#include "test_utils.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(io_correctness, input) {
  std::string in_file = from_unit_test_data("in1.txt");
  std::string res = read(in_file);
  EXPECT_EQ(get_data_from_file(in_file), res);
}

TEST(io_correctness, input_empty) {
  std::string in_file = from_unit_test_data("in_empty.txt");
  std::string res = read(in_file);
  EXPECT_TRUE(res.empty());
}

TEST(io_correctness, input_nonexisting) {
  std::string in_file = from_unit_test_data("no_file.txt");
  EXPECT_THROW(read(in_file), std::runtime_error);
}

TEST(io_correctness, input_to_start) {
  constexpr int TIMES = 100;
  std::string in_file = from_unit_test_data("in1.txt");
  std::string expected = get_data_from_file(in_file);
  fiwrapper in(in_file);

  for (int i = 0; i < TIMES; ++i) {
    std::string res;
    while (!in.eof()) {
      res.push_back(in.next_char());
    }
    EXPECT_EQ(expected, res);
    in.seekg(0, std::ios_base::beg);
  }

  EXPECT_EQ(expected, get_data_from_file(in_file));
}

TEST(io_correctness, output) {
  std::string out_file = from_unit_test_data("out_test.txt");
  std::string str = "kljfsioudfgw8eycr8owegcugfnoewifhcioewuhyr9832y482374y823\tsdfs\nsdfjo";
  write(out_file, str);
  EXPECT_EQ(str, get_data_from_file(out_file));
}

TEST(io_correctness, output_empty) {
  std::string out_file = from_unit_test_data("out_empty.txt");
  std::string str = "";
  write(out_file, str);
  EXPECT_TRUE(get_data_from_file(out_file).empty());
}

TEST(io_correctness, io_equality) {
  std::string in_file = from_unit_test_data("in1.txt");
  std::string out_file = from_unit_test_data("out1.txt");
  std::string res = read(in_file);
  write(out_file, res);
  EXPECT_EQ(get_data_from_file(in_file), get_data_from_file(out_file));
}

TEST(io_correctness, io_chinese) {
  std::string in_file = from_unit_test_data("in_chinese.txt");
  std::string out_file = from_unit_test_data("out_chinese.txt");
  std::string res = read(in_file);
  write(out_file, res);
  EXPECT_EQ(get_data_from_file(in_file), get_data_from_file(out_file));
}

TEST(io_correctness, io_no_ext) {
  std::string in_file = from_unit_test_data("no_ext");
  std::string out_file = from_unit_test_data("out_no_ext");
  std::string res = read(in_file);
  write(out_file, res);
  EXPECT_EQ(get_data_from_file(in_file), get_data_from_file(out_file));
}

TEST(io_correctness, io_my_ext) {
  std::string in_file = from_unit_test_data("my_ext.hahahuffman");
  std::string out_file = from_unit_test_data("out_my_ext.hahahuffman");
  std::string res = read(in_file);
  write(out_file, res);
  EXPECT_EQ(get_data_from_file(in_file), get_data_from_file(out_file));
}

TEST(io_correctness, io_big) {
  std::string in_file = from_unit_test_data("big.txt");
  std::string out_file = from_unit_test_data("out_big.txt");
  std::string res = read(in_file);
  write(out_file, res);
  EXPECT_EQ(get_data_from_file(in_file), get_data_from_file(out_file));
}
