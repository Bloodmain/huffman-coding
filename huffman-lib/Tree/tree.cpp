#include "tree.h"

#include <queue>

#include <functional>
#include <stdexcept>
#include <vector>

using namespace huffman;

void tree::encode(char ch, byte_stream& res) {
  byte_stream& code = codes[static_cast<unsigned char>(ch)];
  if (code.empty()) {
    throw std::runtime_error(std::string("[Huffman tree] Non-existing character \"") + ch + "\"");
  }
  res.push(code);
}

bool tree::is_leaf(tree::node* node) {
  return !node->l && !node->r;
}

void tree::traverse_with_bit(bool bit, fowrapper& res) {
  if (!decoding_state) {
    if (!root) {
      throw std::runtime_error("[Huffman tree] Trying to traverse an empty tree");
    }
    decoding_state = root;
  }
  if (bit == 0) {
    decoding_state = decoding_state->l;
  } else {
    decoding_state = decoding_state->r;
  }

  if (!decoding_state) {
    throw std::runtime_error("[Huffman tree] Can't traverse anymore");
  }

  if (is_leaf(decoding_state)) {
    res.write(decoding_state->ch);
    decoding_state = root;
  }
}

void huffman::tree::recursively_delete(tree::node* root) {
  if (!root) {
    return;
  }

  recursively_delete(root->l);
  recursively_delete(root->r);
  delete root;
}

void tree::build_tree() {
  struct node_comparison {
    bool operator()(const tree::node* l, const tree::node* r) const {
      return l->freq > r->freq;
    }
  };

  std::priority_queue<tree::node*, std::vector<tree::node*>, node_comparison> queue;
  std::size_t non_zero_freq = 0;

  for (std::size_t ch = 0; ch < frequencies.size(); ++ch) {
    if (frequencies[ch] == 0) {
      continue;
    }
    non_zero_freq++;
    try {
      auto* node = new tree::node(ch, frequencies[ch], nullptr, nullptr);
      queue.push(node);
    } catch (...) {
      delete_queue(queue);
      throw;
    }
  }

  while (queue.size() > 1) {
    tree::node* x = queue.top();
    queue.pop();
    tree::node* y = queue.top();
    queue.pop();

    try {
      auto xy = new tree::node(x->ch, x->freq + y->freq, x, y);
      queue.push(xy);
    } catch (...) {
      recursively_delete(x);
      recursively_delete(y);
      delete_queue(queue);
      throw;
    }
  }
  if (queue.size() == 1) {
    root = queue.top();
    if (non_zero_freq == 1) {
      root = new tree::node(root->ch, root->freq, root, nullptr);
    }
    make_tree_map(root);
  }
}

void tree::make_tree_map(tree::node* root) {
  if (!root) {
    return;
  }
  if (is_leaf(root)) {
    codes[static_cast<unsigned char>(root->ch)] = making_state;
    codes[static_cast<unsigned char>(root->ch)].ensure_last_word();
    return;
  }
  making_state.push(0, 1);
  make_tree_map(root->l);
  making_state.pop().push(1, 1);
  make_tree_map(root->r);
  making_state.pop();
}

consts::huf_tree& tree::get_tree() {
  return codes;
}

void tree::add_to_tree(tree::node* root, char ch, byte_stream stream) {
  for (std::size_t i = 0; i < stream.size(); ++i) {
    bool bit = stream.get();
    if (bit == 0) {
      if (!root->l) {
        root->l = new tree::node(ch, 0, nullptr, nullptr);
      }
      root = root->l;
    } else {
      if (!root->r) {
        root->r = new tree::node(ch, 0, nullptr, nullptr);
      }
      root = root->r;
    }
  }

  root->ch = ch;
}

void tree::load_tree(const consts::huf_tree& tr) {
  bool empty = true;
  for (std::size_t i = 0; i < tr.size(); ++i) {
    if (!tr[i].empty()) {
      if (empty) {
        root = new tree::node('\0', 0, nullptr, nullptr);
        root->l = new tree::node('\0', 0, nullptr, nullptr);
        root->r = new tree::node('\0', 0, nullptr, nullptr);
        empty = false;
      }
      add_to_tree(root, i, tr[i]);
    }
  }

  decoding_state = root;
}

tree::~tree() {
  recursively_delete(root);
}
