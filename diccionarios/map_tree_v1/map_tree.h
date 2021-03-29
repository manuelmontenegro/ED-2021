/*
 * ---------------------------------------------------
 *                ESTRUCTURAS DE DATOS
 * ---------------------------------------------------
 *              Manuel Montenegro Montes
 *              Facultad de Informática
 *         Universidad Complutense de Madrid
 * ---------------------------------------------------
 */

/*
 * Implementación del TAD Diccionario mediante árboles binarios
 * de búsqueda.
 *
 * Versión inicial
 */

#ifndef __MAP_TREE_H
#define __MAP_TREE_H

#include <cassert>
#include <iostream>
#include <tuple>
#include <utility>

template <typename K, typename V> class MapTree {
public:
  struct MapEntry {
    K key;
    V value;

    MapEntry(K key, V value) : key(key), value(value) {}
    MapEntry(K key) : key(key) {}
  };

  MapTree() : root_node(nullptr), num_elems(0) {}
  MapTree(const MapTree &other)
      : num_elems(other.num_elems), root_node(copy_nodes(other.root_node)) {}
  ~MapTree() { delete_nodes(root_node); }

  void insert(const MapEntry &entry) {
    auto [new_root, inserted] = insert(root_node, entry);
    this->root_node = new_root;
    if (inserted) {
      num_elems++;
    }
  }

  bool contains(const K &key) const {
    return search(root_node, key) != nullptr;
  }

  const V &at(const K &key) const {
    Node *result = search(root_node, key);
    assert(result != nullptr);
    return result->entry.value;
  }

  V &at(const K &key) {
    Node *result = search(root_node, key);
    assert(result != nullptr);
    return result->entry.value;
  }

  V &operator[](const K &key) {
    auto [inserted, new_root, found_node] = search_or_insert(root_node, key);
    this->root_node = new_root;
    if (inserted)
      num_elems++;
    return found_node->entry.value;
  }

  void erase(const K &key) {
    auto [new_root, erased] = erase(root_node, key);
    this->root_node = new_root;
    if (erased) {
      num_elems--;
    }
  }

  int size() const { return num_elems; }
  bool empty() const { return num_elems == 0; }

  MapTree &operator=(const MapTree &other) {
    if (this != &other) {
      num_elems = other.num_elems;
      delete_nodes(root_node);
      root_node = copy_nodes(other.root_node);
    }
    return *this;
  }

  void display(std::ostream &out) const {
    out << "{";
    display(root_node, out);
    out << "}";
  }

private:
  struct Node {
    MapEntry entry;
    Node *left, *right;

    Node(Node *left, const MapEntry &entry, Node *right)
        : left(left), entry(entry), right(right) {}
  };

  Node *root_node;
  int num_elems;

  // métodos auxiliares privados

  static Node *copy_nodes(const Node *node) {
    if (node == nullptr) {
      return nullptr;
    } else {
      return new Node(copy_nodes(node->left), node->entry,
                      copy_nodes(node->right));
    }
  }

  static void delete_nodes(const Node *node) {
    if (node != nullptr) {
      delete_nodes(node->left);
      delete_nodes(node->right);
      delete node;
    }
  }

  static std::pair<Node *, bool> insert(Node *root, const MapEntry &elem) {
    if (root == nullptr) {
      return {new Node(nullptr, elem, nullptr), true};
    } else if (elem.key < root->entry.key) {
      auto [new_root_left, inserted] = insert(root->left, elem);
      root->left = new_root_left;
      return {root, inserted};
    } else if (root->entry.key < elem.key) {
      auto [new_root_right, inserted] = insert(root->right, elem);
      root->right = new_root_right;
      return {root, inserted};
    } else {
      return {root, false};
    }
  }

  static Node *search(Node *root, const K &key) {
    if (root == nullptr) {
      return nullptr;
    } else if (key < root->entry.key) {
      return search(root->left, key);
    } else if (root->entry.key < key) {
      return search(root->right, key);
    } else {
      return root;
    }
  }

  static std::tuple<bool, Node *, Node *> search_or_insert(Node *root,
                                                           const K &key) {
    if (root == nullptr) {
      Node *new_node = new Node(nullptr, {key}, nullptr);
      return {true, new_node, new_node};
    } else if (key < root->entry.key) {
      auto [inserted, new_root, found_node] = search_or_insert(root->left, key);
      root->left = new_root;
      return {inserted, root, found_node};
    } else if (root->entry.key < key) {
      auto [inserted, new_root, found_node] =
          search_or_insert(root->right, key);
      root->right = new_root;
      return {inserted, root, found_node};
    } else {
      return {false, root, root};
    }
  }

  static std::pair<Node *, bool> erase(Node *root, const K &key) {
    if (root == nullptr) {
      return {root, false};
    } else if (key < root->entry.key) {
      auto [new_root_left, erased] = erase(root->left, key);
      root->left = new_root_left;
      return {root, erased};
    } else if (root->entry.key < key) {
      auto [new_root_right, erased] = erase(root->right, key);
      root->right = new_root_right;
      return {root, erased};
    } else {
      return {remove_root(root), true};
    }
  }

  static Node *remove_root(Node *root) {
    Node *left_child = root->left, *right_child = root->right;
    delete root;
    if (left_child == nullptr && right_child == nullptr) {
      return nullptr;
    } else if (left_child == nullptr) {
      return right_child;
    } else if (right_child == nullptr) {
      return left_child;
    } else {
      auto [lowest, new_right_root] = remove_lowest(right_child);
      lowest->left = left_child;
      lowest->right = new_right_root;
      return lowest;
    }
  }

  static std::pair<Node *, Node *> remove_lowest(Node *root) {
    assert(root != nullptr);
    if (root->left == nullptr) {
      return {root, root->right};
    } else {
      auto [removed_node, new_root_left] = remove_lowest(root->left);
      root->left = new_root_left;
      return {removed_node, root};
    }
  }

  static void display(Node *root, std::ostream &out) {
    if (root != nullptr) {
      if (root->left != nullptr) {
        display(root->left, out);
        out << ", ";
      }
      out << root->entry.key << " -> " << root->entry.value;
      if (root->right != nullptr) {
        out << ", ";
        display(root->right, out);
      }
    }
  }
};

template <typename K, typename V>
std::ostream &operator<<(std::ostream &out, const MapTree<K, V> &map) {
  map.display(out);
  return out;
}

#endif
