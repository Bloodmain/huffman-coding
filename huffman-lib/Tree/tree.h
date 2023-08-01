#pragma once
#include "../Byte-stream/byte_stream.h"
#include "../IO/fowrapper.h"
#include "../consts.h"

#include <queue>

#include <cstdint>

namespace huffman {
class tree {
  struct node {
    node(char ch, uint64_t freq, node* l, node* r) : ch(ch), freq(freq), l(l), r(r) {}

    char ch;
    std::uint64_t freq;
    node* l;
    node* r;
  };

  node* root;
  consts::huf_freq frequencies;
  consts::huf_tree codes;

  byte_stream making_state;
  node* decoding_state;

  void recursively_delete(tree::node* root);

  template <typename T, typename C>
  void delete_queue(std::priority_queue<T*, std::vector<T*>, C> queue) {
    while (!queue.empty()) {
      recursively_delete(queue.top());
      queue.pop();
    }
  }

  void make_tree_map(tree::node* root);
  bool is_leaf(node* node);
  void add_to_tree(tree::node* root, char ch, byte_stream stream);
  void load_tree(const consts::huf_tree& tree);
  void build_tree();

  tree() : frequencies{}, codes{}, root(nullptr), decoding_state(nullptr) {}

public:
  explicit tree(consts::huf_freq& frequencies) : tree() {
    this->frequencies = frequencies;
    build_tree();
  }

  explicit tree(consts::huf_tree& tr) : tree() {
    load_tree(tr);
  }

  void encode(char ch, byte_stream& res);

  void traverse_with_bit(bool bit, fowrapper& res);
  consts::huf_tree& get_tree();

  ~tree();
};
} // namespace huffman
