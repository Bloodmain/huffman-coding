#include "compressor.h"
#include "test_utils.h"
#include "tree.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace huffman;

TEST(hf_tree_correctness, encode) {
  std::string str = "beadbdddbaddaecbde";
  tree hf = build_huffman(str);
  byte_stream buf;
  for (auto ch : str) {
    hf.encode(ch, buf);
  }
  buf.ensure_last_word();
  EXPECT_EQ(40, buf.size());
}

TEST(hf_tree_correctness, decode) {
  std::string str = "beadbdddbaddaecbde";
  tree hf = build_huffman(str);

  std::unordered_map<char, byte_stream> coded;
  for (auto ch : str) {
    byte_stream code;
    hf.encode(ch, code);
    code.ensure_last_word();
    coded[ch] = code;
  }
  for (auto ch : str) {
    {
      fowrapper out(from_unit_test_data("tmp_.txt"));
      for (int i = 0; i < coded[static_cast<unsigned char>(ch)].size(); ++i) {
        hf.traverse_with_bit(ith(coded[ch], i), out);
      }
    }
    std::string s = get_data_from_file(from_unit_test_data("tmp_.txt"));
    EXPECT_EQ(ch, s[0]);
    EXPECT_EQ(1, s.size());
  }
}

TEST(hf_tree_correctness, traverse_empty) {
  std::string str = "";
  tree hf = build_huffman(str);
  fowrapper buf(from_unit_test_data("tmp_"));
  EXPECT_THROW(hf.traverse_with_bit(0, buf), std::runtime_error);
  EXPECT_THROW(hf.traverse_with_bit(1, buf), std::runtime_error);
}

TEST(hf_tree_correctness, traverse_single_char) {
  std::string str = "a";
  {
    tree hf = build_huffman(str);
    fowrapper buf(from_unit_test_data("tmp_"));
    byte_stream buf1;
    hf.encode('a', buf1);
    buf1.ensure_last_word();
    EXPECT_NO_THROW(hf.traverse_with_bit(ith(buf1, 0), buf));
  }
  std::string s = get_data_from_file(from_unit_test_data("tmp_"));
  EXPECT_EQ('a', s[0]);
  EXPECT_EQ(1, s.size());
}

TEST(hf_tree_correctness, single_char) {
  tree hf = build_huffman("a");

  byte_stream stream;
  hf.encode('a', stream);
  stream.ensure_last_word();
  EXPECT_EQ(1, stream.size());

  {
    fowrapper buf(from_unit_test_data("tmp_"));
    for (std::size_t i = 0; i < stream.size(); ++i) {
      hf.traverse_with_bit(ith(stream, i), buf);
    }
  }

  std::string s = get_data_from_file(from_unit_test_data("tmp_"));
  EXPECT_EQ('a', s[0]);
  EXPECT_EQ(1, s.size());
}

TEST(hf_tree_correctness, empty) {
  tree hf = build_huffman("");
  consts::huf_tree tr = hf.get_tree();
  for (auto& x : tr) {
    EXPECT_TRUE(x.empty());
  }
}

TEST(hf_tree_correctness, encode_decode_russian) {
  std::string str = "С другой стороны социально-экономическое развитие представляет собой интересный эксперимент "
                    "проверки всесторонне сбалансированных нововведений.";
  std::string decoded = compress_decompress(str);
  EXPECT_EQ(str, decoded);
}

TEST(hf_tree_correctness, encode_decode_english) {
  std::string str =
      "Hill gravity conduct from pleasure parish fail immediate linen out. Worth poor season in  feel uneasy heart  "
      "besides projecting lovers too door body rapturous. Apartments event except betrayed narrow rapid. If near "
      "vulgar differed might advantages precaution read week attempted seven breeding home insipidity diminution "
      "sensible real. Strongly knew folly above suitable greater dwelling raillery taken year agreeable any.";
  std::string decoded = compress_decompress(str);
  EXPECT_EQ(str, decoded);

  tree hf = build_huffman(str);
  byte_stream buf2;
  for (auto ch : str) {
    hf.encode(ch, buf2);
  }
  buf2.ensure_last_word();
  EXPECT_EQ(1805, buf2.size());
}

TEST(hf_tree_correctness, encode_decode_chinese) {
  std::string str = "在此期间，团队应该好好构建。尽管这些格式容易被人类阅读，但是它们不是结构化的。当然这个库需要被很好"
                    "的组织和管理，但是它确实起到了人员之间共享的作用。虽然您可以不使用这种方法，因为还存在许多其他的方"
                    "法可供使用，但是您应该使用某种优化方法或者结构化的程序。";
  std::string decoded = compress_decompress(str);
  EXPECT_EQ(str, decoded);
}

TEST(hf_tree_correctness, encode_non_existing) {
  std::string str = "beadbdddbaddaecbde";
  tree hf = build_huffman(str);
  byte_stream buf;
  EXPECT_THROW(hf.encode('m', buf), std::runtime_error);
  EXPECT_NO_THROW(hf.encode('a', buf));
}

TEST(hf_tree_correctness, load_empty) {
  tree hf = build_huffman("");
  consts::huf_tree tr = hf.get_tree();

  tree hf2(tr);
  consts::huf_tree tr2 = hf2.get_tree();
  for (auto& x : tr2) {
    EXPECT_TRUE(x.empty());
  }
  fowrapper buf(from_unit_test_data("tmp_"));
  EXPECT_THROW(hf2.traverse_with_bit(0, buf), std::runtime_error);
  EXPECT_THROW(hf2.traverse_with_bit(1, buf), std::runtime_error);
}

TEST(hf_tree_correctness, load_correct) {
  std::string str =
      "在此期间，团队应该好好构建。尽管这些格式容易被人类阅读，但是它们不是结构化的。当然这个库需要被很好";
  tree hf = build_huffman(str);
  consts::huf_tree tr = hf.get_tree();

  std::unordered_map<char, byte_stream> coded;
  for (auto ch : str) {
    byte_stream code;
    hf.encode(ch, code);
    code.ensure_last_word();
    coded[ch] = code;
  }

  tree hf2(tr);
  for (auto ch : str) {
    {
      fowrapper out(from_unit_test_data("tmp_.txt"));
      for (int i = 0; i < coded[static_cast<unsigned char>(ch)].size(); ++i) {
        hf.traverse_with_bit(ith(coded[ch], i), out);
      }
    }
    std::string s = get_data_from_file(from_unit_test_data("tmp_.txt"));
    EXPECT_EQ(ch, s[0]);
    EXPECT_EQ(1, s.size());
  }
}
