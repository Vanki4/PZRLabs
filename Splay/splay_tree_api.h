#pragma once
#include <cstddef>

/**
 * @brief Класс SplayTree — самобалансирующееся дерево поиска (splay tree).
 * 
 * @tparam Key Тип ключа (должен поддерживать сравнение).
 * @tparam Value Тип значения.
 */
template<typename Key, typename Value>
class SplayTree {
public:
    /**
     * @brief Конструктор по умолчанию. Создаёт пустое дерево.
     */
    SplayTree() : _root(nullptr), _size(0) {}

    /**
     * @brief Деструктор. Очищает все ресурсы, связанные с деревом.
     */
    ~SplayTree() {
        clear(_root);
    }

    /**
     * @brief Вставляет пару (key, value) в дерево.
     * Если ключ уже существует, его значение обновляется.
     * После вставки/обновления соответствующий узел становится корнем (splay).
     * 
     * @param key Ключ для вставки.
     * @param value Значение для вставки.
     */
    void insert(const Key& key, const Value& value) {
        if (!_root) {
            _root = new Node(key, value);
            _size++;
            return;
        }
        
        Node* current = _root;
        Node* parent = nullptr;
        
        while (current) {
            parent = current;
            if (key < current->key) current = current->left;
            else if (current->key < key) current = current->right;
            else {
                current->value = value;
                splay(current);
                return;
            }
        }
        
        Node* newNode = new Node(key, value);
        newNode->parent = parent;
        
        if (key < parent->key) parent->left = newNode;
        else parent->right = newNode;
        
        _size++;
        splay(newNode);
    }

    /**
     * @brief Удаляет узел с заданным ключом из дерева.
     * 
     * @param key Ключ для удаления.
     * @return true если элемент был найден и удалён, false если такого ключа нет.
     */
    bool remove(const Key& key) {
        Node* node = findNode(key);
        if (!node) return false;
        
        splay(node);
        
        Node* leftSubtree = node->left;
        Node* rightSubtree = node->right;
        
        if (leftSubtree) leftSubtree->parent = nullptr;
        if (rightSubtree) rightSubtree->parent = nullptr;
        
        delete node;
        _size--;
        
        if (!leftSubtree) {
            _root = rightSubtree;
        } else {
            _root = leftSubtree;
            Node* maxNode = leftSubtree;
            while (maxNode->right) maxNode = maxNode->right;
            splay(maxNode);
            _root->right = rightSubtree;
            if (rightSubtree) rightSubtree->parent = _root;
        }
        
        return true;
    }

    /**
     * @brief Ищет элемент по ключу.
     * Если найден, возвращает указатель на значение (Value*), иначе nullptr.
     * После поиска найденный (или последний просмотренный) узел становится корнем (splay).
     * 
     * @param key Ключ для поиска.
     * @return Value* Указатель на значение или nullptr.
     */
    Value* search(const Key& key) {
        Node* node = findNode(key);
        return node ? &node->value : nullptr;
    }

    /**
     * @brief Константная версия поиска.
     * Не изменяет структуру дерева.
     * 
     * @param key Ключ для поиска.
     * @return const Value* Указатель на значение или nullptr.
     */
    const Value* search(const Key& key) const {
        Node* node = findNodeConst(key);
        return node ? &node->value : nullptr;
    }

    /**
     * @brief Проверяет, что дерево удовлетворяет свойству бинарного дерева поиска (BST).
     * 
     * @return true если дерево корректно, false иначе.
     */
    bool isValidBST() const {
        if (!_root) return true;
        return validateBST(_root, Key(), Key());
    }

    /**
     * @brief Возвращает количество элементов в дереве.
     * 
     * @return size_t Количество элементов.
     */
    size_t size() const { 
	return _size;
    }

    /**
     * @brief Проверяет, пусто ли дерево.
     * 
     * @return true если дерево пустое, false иначе.
     */
    bool empty() const {
	return _size==0;
    }

    private:
    struct Node {
	Key key;
	Value value;
	Node* parent;
	Node* left;
	Node* right;
	Node(const Key& key,const Value& value) : key(key),value(value),parent(nullptr),left(nullptr),right(nullptr) {}
    };
    Node* _root;
    size_t _size;
    void rotateRight(Node* node) {
        Node* leftChild = node->left;
        if (!leftChild) return;

        node->left = leftChild->right;
        if (leftChild->right) leftChild->right->parent = node;

        leftChild->parent = node->parent;
        if (!node->parent) _root = leftChild;
        else if (node == node->parent->left) node->parent->left = leftChild;
        else node->parent->right = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
    }

    void rotateLeft(Node* node) {
        Node* rightChild = node->right;
        if (!rightChild) return;

        node->right = rightChild->left;
        if (rightChild->left) rightChild->left->parent = node;

        rightChild->parent = node->parent;
        if (!node->parent) _root = rightChild;
        else if (node == node->parent->left) node->parent->left = rightChild;
        else node->parent->right = rightChild;

        rightChild->left = node;
        node->parent = rightChild;
    }

    void splay(Node* node) {
        while (node && node->parent) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            if (!grandparent) {
                if (node == parent->left) rotateRight(parent);
                else rotateLeft(parent);
            } else if (node == parent->left && parent == grandparent->left) {
                rotateRight(grandparent);
                rotateRight(parent);
            } else if (node == parent->right && parent == grandparent->right) {
                rotateLeft(grandparent);
                rotateLeft(parent);
            } else if (node == parent->left && parent == grandparent->right) {
                rotateRight(parent);
                rotateLeft(grandparent);
            } else {
                rotateLeft(parent);
                rotateRight(grandparent);
            }
        }
    }

    Node* findNode(const Key& key) {
        Node* current = _root;
        Node* last = nullptr;

        while (current) {
            last = current;
            if (key < current->key) current = current->left;
            else if (current->key < key) current = current->right;
            else {
                splay(current);
                return current;
            }
        }

        if (last) splay(last);
        return nullptr;
    }

    Node* findNodeConst(const Key& key) const {
        Node* current = _root;
        while (current) {
            if (key < current->key) current = current->left;
            else if (current->key < key) current = current->right;
            else return current;
        }
        return nullptr;
    }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    bool validateBST(Node* node, const Key& minKey, const Key& maxKey) const {
        if (!node) return true;
        if (node->key <= minKey || node->key >= maxKey) return false;
        return validateBST(node->left, minKey, node->key) && 
               validateBST(node->right, node->key, maxKey);
    }
};
