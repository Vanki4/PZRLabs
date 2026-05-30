#include "BST.h"
#include <limits>
#include <iostream>

BinarySearchTree::Node::Node(Key key, Value value, bool color,Node *parent, Node *left, Node *right) : keyValuePair(key, value),color(color), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other) : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (other.left)
    {
        left = new Node(*other.left);
        left->parent = this;
    }

    if (other.right)
    {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const {
	return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree(int level) const {
	if (left) left->output_node_tree(level+1);
	if (this->keyValuePair.first == std::numeric_limits<Key>::max()) return;
	std::cout << "(" << this->keyValuePair.first << "," << this->keyValuePair.second << ")" << std::endl;
	if (right) right->output_node_tree(level+1);
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value,Node** root) {
	if (!root) return;
	if (key <= keyValuePair.first) {
		if (left) {
			left->insert(key, value, root);
			return;
		}
		left = new Node(key, value, true, this);
		left->insertRebalance(root);
	}
	else {
		if (right) {
			right->insert(key, value, root);
			return;
		}
		right = new Node(key, value, true, this);
		right->insertRebalance(root);
	}
}

void BinarySearchTree::Node::erase(const Key &key,Node** root) {
	/*if (key < keyValuePair.first)
	{
		if (left!=nullptr) left->erase(key);
		return;
	}
	if (key > keyValuePair.first)
	{
		if (right) right->erase(key);
		return;
	}*/
	if (!root) return;
	if (key != keyValuePair.first) {
		if (left && key <= keyValuePair.first) left->erase(key, root);
		else if (right) right->erase(key, root);
		return;
	}
	if (left && right) {
		Node* current = right;
		while (current->left) current = current->left;
		this->keyValuePair = current->keyValuePair;
		current->erase(current->keyValuePair.first, root);
		return;
	}
	if (color) {
		if (parent->left == this) parent->left = nullptr;
		else parent->right = nullptr;
		delete this;
		return;
	}
	Node* ptr = nullptr;
	if (left) ptr = left;
	else ptr = right;
	if (!ptr) {
		this->color = 0;
		this->eraseRebalance(root);
		if (parent) {
			if (parent->left == this) parent->left = ptr;
			else parent->right = ptr;
		}
		if (this == *root) *root = ptr;
		delete this;
		return;
	}
	if (parent) {
		if (parent->left == this) parent->left = ptr;
		else parent->right = ptr;
	}
	ptr->parent = parent;
	if (ptr->color) {
		ptr->color = false;
	} 
	else ptr->eraseRebalance(root);
	if (this == *root) *root = ptr;
	delete this;
	return;	
}

void BinarySearchTree::Node::rotateLeft() {
	Node* pivot = this->right;
	pivot->parent = this->parent;
	if (this->parent) {
		if (this->parent->left == this) {
			this->parent->left = pivot;
		}
		else {
			this->parent->right = pivot;
		}
	}
	this->right = pivot->left;
	if (pivot->left) pivot->left->parent = this;
	this->parent = pivot;
	pivot->left = this;
}

void BinarySearchTree::Node::rotateRight() {
	Node* pivot = this->left;
	pivot->parent = this->parent;
	if (this->parent) {
		if (this->parent->left == this) {
			this->parent->left = pivot;
		} 
		else {
			this->parent->right = pivot;
		}
	}
	this->left = pivot->right;
	if (pivot->right) pivot->right->parent = this;
	this->parent = pivot;
	pivot->right = this;
}

void BinarySearchTree::Node::insertRebalance(Node** root) {
	if (!parent) {
		color = 0;
		*root = this;
		return;
	}
	if (parent->color == false) return;
	Node* uncle = parent->parent->left == parent ? parent->parent->right : parent->parent->left;
	if (uncle && uncle->color) {
		parent->color = false;
		uncle->color = false;
		parent->parent->color = true;
		parent->parent->insertRebalance(root);
		return;
	} 
	Node* n = this;
	if (n == parent->right && parent == parent->parent->left) {
		parent->rotateLeft();
		n = left;
	} 
	else if (n == parent->left && parent == parent->parent->right) {
		parent->rotateRight();
		n = right;
	}
	n->parent->color = false;
	n->parent->parent->color = true;
	if ((n == n->parent->left) && (n->parent == n->parent->parent->left)) {
		if (n->parent->parent == *root) *root = n->parent->parent->left;
		n->parent->parent->rotateRight();
	} 
	else {
		if (n->parent->parent == *root) *root = n->parent->parent->right;
		n->parent->parent->rotateLeft();
	}
}

void BinarySearchTree::Node::eraseRebalance(Node** root) {
	if (!parent) return;
	Node* sibling;
	if (parent->left == this) sibling = parent->right;
	else sibling = parent->left;
	if (sibling->color) {
		parent->color = 1;
		sibling->color = 0;
		if (parent->left == this) {
			if (parent == *root) *root = parent->right;
			parent->rotateLeft();
		} 
		else {
			if (parent == *root) *root = parent->left;
			parent->rotateRight();
		}
	}
	if (!parent->color && !sibling->color && (!sibling->left || !sibling->left->color) && (!sibling->right || !sibling->right->color)) {
		sibling->color = 1;
		parent->eraseRebalance(root);
		return;
	}
	if (parent->color && !sibling->color && (!sibling->left || !sibling->left->color) && (!sibling->right || !sibling->right->color)) {
		sibling->color = 1;
		parent->color = 0;
		return;
	}
	if (!sibling->color) {
		if (this == parent->left && (!sibling->right || !sibling->right->color) && (sibling->left && sibling->left->color)) {
			sibling->color = 1;
			sibling->left->color = 0;
			sibling->rotateRight();
		} 
		else if (this == parent->right && (sibling->right && sibling->right->color) && (!sibling->left || !sibling->left->color)) {
			sibling->color = 1;
			sibling->right->color = 0;
			sibling->rotateLeft();
		}
		return;
	}
	sibling->color = parent->color;
	parent->color = 0;
	if (this == parent->left) {
		if (sibling->right) sibling->right->color = 0;
		if (parent == *root) *root = parent->right;
		parent->rotateLeft();
	} 
	else {
		if (sibling->left) sibling->left->color = 0;
		if (parent == *root) *root = parent->left;
		parent->rotateRight();
	}
}

size_t BinarySearchTree::Node::getMaxHeight() const {
	if (this->keyValuePair.first == std::numeric_limits<Key>::max()) return 0;
	size_t lh = 0, rh = 0;
	if (left) lh = left->getMaxHeight();
	if (right) rh = right->getMaxHeight();
	return 1 + std::max(lh, rh);
}

void BinarySearchTree::freeSubtree(Node* node) {
	if (!node) return;
	freeSubtree(node->left);
	freeSubtree(node->right);
	delete node;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) : _size(other._size) {
	if (other._root!=nullptr) _root = new Node(*other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other) {
	if (this == &other) return *this;
	freeSubtree(_root);
	_root = nullptr;
	_size = 0;
	if (other._root!=nullptr) _root = new Node(*other._root);
	_size = other._size;
	return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
	_root = other._root;
	_size = other._size;
	other._root = nullptr;
	other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
	if (this == &other) return *this;
	_root = other._root;
	_size = other._size;
	other._root = nullptr;
	other._size = 0;
	return *this;
}

BinarySearchTree::~BinarySearchTree() {
	freeSubtree(_root);
}

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {};

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
	if (_node==nullptr) throw std::runtime_error("Null pointer"); 
	return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	if (_node->right!=nullptr)
	{
		_node = _node->right;
		while (_node->left!=nullptr)
		{
			_node = _node->left;
		}
		return *this;
	}
	while (_node->parent!=nullptr)
	{
		if (_node == _node->parent->left)
		{
			_node = _node->parent;
			return *this;
		}
		_node = _node->parent;
	}
	throw std::runtime_error("end of the iterator!");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
	Iterator temp(*this);
	++(*this);
	return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
          if (_node->left!=nullptr)
          {
                  _node = _node->left;
                  while (_node->right!=nullptr)
                  {
                          _node = _node->right;
                  }
                  return *this;
          }
          while (_node->parent!=nullptr)
          {
                  if (_node == _node->parent->right)
                  {
                          _node = _node->parent;
                          return *this;
                  }
                  _node = _node->parent;
          }
          throw std::runtime_error("begin of the iterator!");	
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
	Iterator temp(*this);
	--(*this);
	return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
	return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
	return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
          if (_node->right!=nullptr)
          {
                  _node = _node->right;
                  while (_node->left!=nullptr)
                  {
                          _node = _node->left;
                  }
                  return *this;
          }
          while (_node->parent!=nullptr)
          {
                  if (_node == _node->parent->left)
                  {
                          _node = _node->parent;
                          return *this;
                  }
                  _node = _node->parent;
          }
          throw std::runtime_error("end of the iterator!");	
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
	ConstIterator temp(*this);
	++(*this);
	return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
          if (_node->left!=nullptr)
          {
                  _node = _node->left;
                  while (_node->right!=nullptr)
                  {
                          _node = _node->right;
                  }
                  return *this;
          }
          while (_node->parent!=nullptr)
          {
                  if (_node == _node->parent->right)
                  {
                          _node = _node->parent;
                          return *this;
                  }
                  _node = _node->parent;
          }
          throw std::runtime_error("begin of the iterator!");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
	ConstIterator temp(*this);
	--(*this);
	return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
	return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
	return _node != other._node;
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
	if (_root==nullptr) {
		_root = new Node(key, value);
		_root->right = new Node(std::numeric_limits<Key>::max(),value,_root);
	}
	else _root->insert(key, value);
	_size++;
}

void BinarySearchTree::erase(const Key &key) {
	if (_root==nullptr) return;
	Iterator tmp(_root);
	while ((tmp = find(key)) != end())
	{
		tmp._node->erase(key);
		_size--;
	}		
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
	Node *tmp = _root;
	while (tmp!=nullptr)
	{
		if (key < tmp->keyValuePair.first) tmp = tmp->left;
		else if (key > tmp->keyValuePair.first) tmp = tmp->right;
		else return ConstIterator(tmp);
	}
	return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
	Node *tmp = _root;
	while (tmp!=nullptr)
	{
		if (key < tmp->keyValuePair.first) tmp = tmp->left;
		else if (key > tmp->keyValuePair.first) tmp = tmp->right;
		else return Iterator(tmp);
	}
	return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key) {
	Iterator first = find(key);
	if (first == end()) return {end(), end()};
	Iterator second = first;
	while (second != end() && (*second).first == key) ++second;
	return {first, second};
}

std::pair<BinarySearchTree::ConstIterator,BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const {
	ConstIterator first = find(key);
	if (first == cend()) return {cend(), cend()};
	ConstIterator second = first;
	while (second != cend() && (*second).first == key) ++second;
	return {first, second};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
	Node *tmp = _root;
	if (tmp==nullptr) return cend();
	while (tmp->left) tmp = tmp->left;
	return ConstIterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
	if (_root==nullptr) return cend();
	return --cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
	std::pair<ConstIterator,ConstIterator> tmp = equalRange(key);
	if (tmp.first==cend()) return cend();
	ConstIterator mini = tmp.first;
	for (auto i = tmp.first;i!=tmp.second;i++)
	{
		if ((*mini).second > (*i).second) mini=i;
	}
	return mini;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
	std::pair<ConstIterator,ConstIterator> tmp = equalRange(key);
        if (tmp.first==cend()) return cend();
        ConstIterator maxi = tmp.first;
        for (auto i = tmp.first;i!=tmp.second;i++)
        { 
                if ((*maxi).second < (*i).second) maxi=i;
        } 
        return maxi;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
	Node *tmp = _root;
	while (tmp->left!=nullptr) tmp = tmp->left;
	return Iterator(tmp);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
	Node *tmp = _root;
        while (tmp->right!=nullptr) tmp = tmp->right;
        return Iterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
	Node *tmp = _root;
	while (tmp->left) tmp = tmp->left;
	return ConstIterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
	Node *tmp = _root;    
        while (tmp->right!=nullptr) tmp = tmp->right;
        return ConstIterator(tmp);
}

size_t BinarySearchTree::size() const {
	return _size;
}

void BinarySearchTree::output_tree() {
	if (_root) _root->output_node_tree();
}
