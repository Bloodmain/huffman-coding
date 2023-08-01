#include "compressor.h"
#include "decompressor.h"
#include "fiwrapper.h"
#include "fowrapper.h"
#include "test_utils.h"
#include "tree.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace huffman;

TEST(wrapper_correctness, wrap_unwrap_equality) {
  std::string str = "beadbdddbaddaecbde";
  auto res = compress_decompress(str);
  EXPECT_EQ(str, res);
}

TEST(wrapper_correctness, wrap_unwrap_empty) {
  std::string str = "";
  auto res = compress_decompress(str);
  EXPECT_EQ(str, res);
}

TEST(wrapper_correctness, wrap_unwrap_single_char) {
  std::string str = "f";
  auto res = compress_decompress(str);
  EXPECT_EQ(str, res);
}

TEST(wrapper_correctness, wrap_unwrap_chinese) {
  std::string str = "在此期间，团队应该好好构建。尽管这些格式容易被人类阅读，但是它们不是结构化的。当然这个库需要被很好"
                    "的组织和管理，但是它确实起到了人员之间共享的作用。虽然您可以不使用这种方法，因为还存在许多其他的方"
                    "法可供使用，但是您应该使用某种优化方法或者结构化的程序。";
  auto res = compress_decompress(str);
  EXPECT_EQ(str, res);
}

TEST(wrapper_correctness, corrupted_unwrap) {
  EXPECT_THROW(decompress(from_unit_test_data("in1.txt")), std::runtime_error);
}
