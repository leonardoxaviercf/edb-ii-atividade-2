#include "AVLTree.hpp"
#include <algorithm>
#include <climits>
#include <cmath>

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    clear();
}

int AVLTree::height(Node* node) const {
    return node ? node->height : 0;
}

int AVLTree::getBalanceFactor(Node* node) const {
    return node ? height(node->left) - height(node->right) : 0;
}

void AVLTree::updateHeight(Node* node) {
    if (node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

AVLTree::Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Realiza a rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas (primeiro o nó que desceu, depois a nova sub-raiz)
    updateHeight(y);
    updateHeight(x);

    return x;
}

AVLTree::Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Realiza a rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    updateHeight(x);
    updateHeight(y);

    return y;
}

bool AVLTree::insert(int key) {
    bool inserted = false;
    root = insertRec(root, key, inserted);
    return inserted;
}

AVLTree::Node* AVLTree::insertRec(Node* node, int key, bool& inserted) {
    if (!node) {
        inserted = true;
        return new Node(key);
    }

    metrics.comparisons++;
    if (key < node->key) {
        node->left = insertRec(node->left, key, inserted);
    } else if (key > node->key) {
        node->right = insertRec(node->right, key, inserted);
    } else {
        // Elemento duplicado: ignora inserção e preserva árvore intacta
        inserted = false;
        return node;
    }

    if (!inserted) return node;

    updateHeight(node);
    int balance = getBalanceFactor(node);

    // Caso 1: Esquerda-Esquerda (Rotação Simples à Direita)
    if (balance > 1 && key < node->left->key) {
        metrics.singleRotationsRight++;
        return rotateRight(node);
    }

    // Caso 2: Direita-Direita (Rotação Simples à Esquerda)
    if (balance < -1 && key > node->right->key) {
        metrics.singleRotationsLeft++;
        return rotateLeft(node);
    }

    // Caso 3: Esquerda-Direita (Rotação Dupla Esquerda-Direita)
    if (balance > 1 && key > node->left->key) {
        metrics.doubleRotationsLR++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Caso 4: Direita-Esquerda (Rotação Dupla Direita-Esquerda)
    if (balance < -1 && key < node->right->key) {
        metrics.doubleRotationsRL++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

AVLTree::Node* AVLTree::getMinValueNode(Node* node) const {
    Node* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

bool AVLTree::remove(int key) {
    bool removed = false;
    root = removeRec(root, key, removed);
    return removed;
}

AVLTree::Node* AVLTree::removeRec(Node* node, int key, bool& removed) {
    if (!node) {
        removed = false;
        return nullptr;
    }

    metrics.comparisons++;
    if (key < node->key) {
        node->left = removeRec(node->left, key, removed);
    } else if (key > node->key) {
        node->right = removeRec(node->right, key, removed);
    } else {
        // Nó encontrado para remoção
        removed = true;

        // Caso 1 e 2: Nó folha (0 filhos) ou com apenas 1 filho
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }

        // Caso 3: Nó com 2 filhos (substitui pelo sucessor em-ordem)
        Node* temp = getMinValueNode(node->right);
        node->key = temp->key;
        bool dummy = false;
        node->right = removeRec(node->right, temp->key, dummy);
    }

    if (!node) return nullptr;

    updateHeight(node);
    int balance = getBalanceFactor(node);

    // Rebalanceamento pós-remoção
    // Esquerda-Esquerda
    if (balance > 1 && getBalanceFactor(node->left) >= 0) {
        metrics.singleRotationsRight++;
        return rotateRight(node);
    }

    // Esquerda-Direita
    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        metrics.doubleRotationsLR++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Direita-Direita
    if (balance < -1 && getBalanceFactor(node->right) <= 0) {
        metrics.singleRotationsLeft++;
        return rotateLeft(node);
    }

    // Direita-Esquerda
    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        metrics.doubleRotationsRL++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

bool AVLTree::search(int key) {
    return searchRec(root, key);
}

bool AVLTree::searchRec(Node* node, int key) {
    if (!node) return false;

    metrics.comparisons++;
    if (key == node->key) return true;
    if (key < node->key) return searchRec(node->left, key);
    return searchRec(node->right, key);
}

void AVLTree::inOrder() const {
    if (!root) {
        std::cout << "[Arvore vazia]\n";
        return;
    }
    inOrderRec(root);
    std::cout << "\n";
}

void AVLTree::inOrderRec(Node* node) const {
    if (!node) return;
    inOrderRec(node->left);
    std::cout << node->key << " ";
    inOrderRec(node->right);
}

void AVLTree::printTree() const {
    if (!root) {
        std::cout << "[Arvore AVL vazia]\n";
        return;
    }
    printTreeRec(root, "", true);
}

void AVLTree::printTreeRec(Node* node, std::string indent, bool last) const {
    if (node != nullptr) {
        std::cout << indent;
        if (last) {
            std::cout << "└── ";
            indent += "    ";
        } else {
            std::cout << "├── ";
            indent += "│   ";
        }

        std::cout << ANSI::CYAN << node->key << ANSI::RESET
                  << " (h:" << node->height << ", fb:" << getBalanceFactor(node) << ")\n";

        printTreeRec(node->left, indent, false);
        printTreeRec(node->right, indent, true);
    }
}

int AVLTree::getHeight() const {
    return height(root);
}

bool AVLTree::isEmpty() const {
    return root == nullptr;
}

bool AVLTree::validateProperties() const {
    return validateRec(root, INT_MIN, INT_MAX);
}

bool AVLTree::validateRec(Node* node, int minVal, int maxVal) const {
    if (!node) return true;

    // Verifica propriedade de BST
    if (node->key <= minVal || node->key >= maxVal) return false;

    // Verifica invariante de balanceamento da AVL (|FB| <= 1) e consistência da altura
    int fb = getBalanceFactor(node);
    int expectedHeight = 1 + std::max(height(node->left), height(node->right));
    if (std::abs(fb) > 1 || node->height != expectedHeight) return false;

    return validateRec(node->left, minVal, node->key) &&
           validateRec(node->right, node->key, maxVal);
}

void AVLTree::clear() {
    clearRec(root);
    root = nullptr;
    resetMetrics();
}

void AVLTree::clearRec(Node* node) {
    if (!node) return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}