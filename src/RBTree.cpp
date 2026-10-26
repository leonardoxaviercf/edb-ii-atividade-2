#include "RBTree.hpp"
#include <algorithm>
#include <climits>

RBTree::RBTree() {
    NIL = new Node(0, Color::BLACK);
    NIL->left = NIL->right = NIL->parent = NIL;
    root = NIL;
}

RBTree::~RBTree() {
    clear();
    delete NIL;
}

void RBTree::setColor(Node* node, Color newColor) {
    if (node != NIL && node->color != newColor) {
        node->color = newColor;
        metrics.colorFlips++;
    }
}

void RBTree::rotateLeft(Node* x) {
    metrics.singleRotationsLeft++;
    Node* y = x->right;
    x->right = y->left;

    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NIL) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void RBTree::rotateRight(Node* y) {
    metrics.singleRotationsRight++;
    Node* x = y->left;
    y->left = x->right;

    if (x->right != NIL) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == NIL) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

bool RBTree::insert(int key) {
    Node* y = NIL;
    Node* x = root;

    while (x != NIL) {
        y = x;
        metrics.comparisons++;
        if (key == x->key) {
            // Rejeita chaves duplicadas
            return false;
        } else if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    Node* z = new Node(key, Color::RED, NIL, NIL, y);
    if (y == NIL) {
        root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    insertFixup(z);
    return true;
}

void RBTree::insertFixup(Node* z) {
    while (z->parent->color == Color::RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == Color::RED) {
                // Caso 1: Tio é vermelho (apenas recoloração)
                setColor(z->parent, Color::BLACK);
                setColor(y, Color::BLACK);
                setColor(z->parent->parent, Color::RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Caso 2: Tio é preto e z é filho à direita (rotação simples prepara Caso 3)
                    z = z->parent;
                    rotateLeft(z);
                }
                // Caso 3: Tio é preto e z é filho à esquerda
                setColor(z->parent, Color::BLACK);
                setColor(z->parent->parent, Color::RED);
                rotateRight(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == Color::RED) {
                // Caso 1
                setColor(z->parent, Color::BLACK);
                setColor(y, Color::BLACK);
                setColor(z->parent->parent, Color::RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    // Caso 2
                    z = z->parent;
                    rotateRight(z);
                }
                // Caso 3
                setColor(z->parent, Color::BLACK);
                setColor(z->parent->parent, Color::RED);
                rotateLeft(z->parent->parent);
            }
        }
    }
    setColor(root, Color::BLACK);
}

void RBTree::rbTransplant(Node* u, Node* v) {
    if (u->parent == NIL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

RBTree::Node* RBTree::minimum(Node* node) const {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

RBTree::Node* RBTree::searchNode(int key) {
    Node* current = root;
    while (current != NIL) {
        metrics.comparisons++;
        if (key == current->key) return current;
        if (key < current->key) current = current->left;
        else current = current->right;
    }
    return NIL;
}

bool RBTree::remove(int key) {
    Node* z = searchNode(key);
    if (z == NIL) {
        return false;
    }

    Node* y = z;
    Color yOriginalColor = y->color;
    Node* x = NIL;

    if (z->left == NIL) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        setColor(y, z->color);
    }

    delete z;

    if (yOriginalColor == Color::BLACK) {
        deleteFixup(x);
    }
    NIL->parent = NIL;
    return true;
}

void RBTree::deleteFixup(Node* x) {
    while (x != root && x->color == Color::BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == Color::RED) {
                // Caso 1: Irmão de x é vermelho
                setColor(w, Color::BLACK);
                setColor(x->parent, Color::RED);
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                // Caso 2: Ambos os filhos de w são pretos
                setColor(w, Color::RED);
                x = x->parent;
            } else {
                if (w->right->color == Color::BLACK) {
                    // Caso 3: Filho direito de w é preto e esquerdo é vermelho
                    setColor(w->left, Color::BLACK);
                    setColor(w, Color::RED);
                    rotateRight(w);
                    w = x->parent->right;
                }
                // Caso 4: Filho direito de w é vermelho
                setColor(w, x->parent->color);
                setColor(x->parent, Color::BLACK);
                setColor(w->right, Color::BLACK);
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            // Simétrico: x é filho à direita
            Node* w = x->parent->left;
            if (w->color == Color::RED) {
                // Caso 1
                setColor(w, Color::BLACK);
                setColor(x->parent, Color::RED);
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                // Caso 2
                setColor(w, Color::RED);
                x = x->parent;
            } else {
                if (w->left->color == Color::BLACK) {
                    // Caso 3
                    setColor(w->right, Color::BLACK);
                    setColor(w, Color::RED);
                    rotateLeft(w);
                    w = x->parent->left;
                }
                // Caso 4
                setColor(w, x->parent->color);
                setColor(x->parent, Color::BLACK);
                setColor(w->left, Color::BLACK);
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    setColor(x, Color::BLACK);
}

bool RBTree::search(int key) {
    return searchNode(key) != NIL;
}

void RBTree::inOrder() const {
    if (root == NIL) {
        std::cout << "[Arvore vazia]\n";
        return;
    }
    inOrderRec(root);
    std::cout << "\n";
}

void RBTree::inOrderRec(Node* node) const {
    if (node == NIL) return;
    inOrderRec(node->left);
    std::cout << node->key << " ";
    inOrderRec(node->right);
}

void RBTree::printTree() const {
    if (root == NIL) {
        std::cout << "[Arvore Rubro-Negra vazia]\n";
        return;
    }
    printTreeRec(root, "", true);
}

void RBTree::printTreeRec(Node* node, std::string indent, bool last) const {
    if (node != NIL) {
        std::cout << indent;
        if (last) {
            std::cout << "└── ";
            indent += "    ";
        } else {
            std::cout << "├── ";
            indent += "│   ";
        }

        if (node->color == Color::RED) {
            std::cout << ANSI::RED << node->key << " [V]" << ANSI::RESET << "\n";
        } else {
            std::cout << ANSI::YELLOW << node->key << " [P]" << ANSI::RESET << "\n";
        }

        printTreeRec(node->left, indent, false);
        printTreeRec(node->right, indent, true);
    }
}

int RBTree::getHeight() const {
    return heightRec(root);
}

int RBTree::heightRec(Node* node) const {
    if (node == NIL) return 0;
    return 1 + std::max(heightRec(node->left), heightRec(node->right));
}

bool RBTree::isEmpty() const {
    return root == NIL;
}

bool RBTree::validateProperties() const {
    if (root == NIL) return true;

    // Propriedade 2: A raiz deve ser sempre preta
    if (root->color != Color::BLACK) return false;

    int blackHeight = 0;
    return validateRec(root, INT_MIN, INT_MAX, blackHeight);
}

bool RBTree::validateRec(Node* node, int minVal, int maxVal, int& blackHeight) const {
    if (node == NIL) {
        blackHeight = 1;
        return true;
    }

    // Verifica ordem de BST
    if (node->key <= minVal || node->key >= maxVal) return false;

    // Propriedade 4: Um nó vermelho não pode ter filhos vermelhos
    if (node->color == Color::RED) {
        if (node->left->color == Color::RED || node->right->color == Color::RED) {
            return false;
        }
    }

    int leftBH = 0, rightBH = 0;
    if (!validateRec(node->left, minVal, node->key, leftBH) ||
        !validateRec(node->right, node->key, maxVal, rightBH)) {
        return false;
    }

    // Propriedade 5: Todos os caminhos devem ter a mesma altura negra
    if (leftBH != rightBH) return false;

    blackHeight = leftBH + (node->color == Color::BLACK ? 1 : 0);
    return true;
}

void RBTree::clear() {
    clearRec(root);
    root = NIL;
    resetMetrics();
}

void RBTree::clearRec(Node* node) {
    if (node == NIL) return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}