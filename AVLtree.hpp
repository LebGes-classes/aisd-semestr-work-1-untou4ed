#include <algorithm>
#include <iostream>
#include <functional>

template <typename T>
class AVLTree {
public:
    AVLTree() : root(nullptr) {} // конструктор
    
    ~AVLTree() { // деструктор
        clear(root);
    }

    // вставить ячейку со значением key
    void insert(T key) {
        root = insert(root, key);
    }

    // удаляем ячейку со значением key, если она есть
    void remove(T key) {
        root = remove(root, key);
    }

    // проверяем есть ли ячейка со значением key
    bool contains(T key) {
        return contains(root, key);
    }

    // выводим в порядке возрастания
    void printInOrder() {
        printInOrder(root);
        std::cout << std::endl;
    }

private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        int height;

        Node(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    // возвращаем высоту текущей ячейки 
    int height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    int balanceFactor(Node* node) {
        if (!node) {
            return 0;
        }
        // если меньше нуля - перевешивает левая ячейка, больше нуля - правая
        return height(node->right) - height(node->left);
    }

    void updateHeight(Node* node) {
        if (node) {
            // текущая высота - максимальная высота из поддеревьев + 1
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }


    // правое поддерево перевешивает, правый потомок поддерева больше или равен левому 
    Node* smallLeftRotate(Node* node) { // использова а, b, c,  по схемам из презентации
        Node* a = node;
        Node* b = node->right;
        Node* c = node->right->left;

        a->right = c;
        b->left = a;

            //    5                       
            // 3     9       ---->            9
            //     8   10                   5   10
            //    7       11              3   8     11
            //                               7         


        updateHeight(a);
        updateHeight(b);

        return b;
    }

    //левое поддерево тяжелее, его правый потомок меньше или равен левому
    Node* smallRightRotate(Node* node) {
        Node* a = node;
        Node* b = node->left;
        Node* c = node->left->right;

        b->right = a;
        a->left = c;

        //      a                        b
        //    b   r                   l     a
        //  l  c       ------------->     c   r
        updateHeight(b);
        updateHeight(a);

        return b;
    }
    
    Node* bigLeftRotate(Node* node) {
        Node* a = node;
        Node* b = node->right;
        b = smallRightRotate(b);
        //           a                                     c
        //        l      b         ------------->       a     b
        //            c     r                          l m   n  r
        //          m   n
        return smallLeftRotate(a);
    }

    Node* bigRightRotate(Node* z) {
        z->left = smallLeftRotate(z->left);
        //           a                                     c
        //        b     r         ------------->         b    a
        //       l  c                                   l m  n r
        //         m n
        return smallRightRotate(z);
    }

    
    // балансируем ячейку
    Node* balance(Node* node) {
        updateHeight(node);
        int bf = balanceFactor(node);
        
        // левое поддерево тяжелее
        if (bf < -1) {
            // правое потомок поддерва тяжеее левого
            if (height(node->left->right) < height(node->left->left)) {
                return bigRightRotate(node);
            }
            return smallRightRotate(node);
        }
        // правое поддерево тяжелее
        else if (bf > 1) {
            // правое потомок поддерва тяжеее левого
            if (height(node->right->right) < height(node->right->left) < 0) {
                return bigLeftRotate(node);
            }
            return smallLeftRotate(node);
        }
        return node;
    }

    // вставка нового элемента
    Node* insert(Node* node, T key) {
        if (!node) return new Node(key);

        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        else { // если ячейка с таким ключом уже есть, то вставки не происходит
            return node; 
        }

        return balance(node);
    }

    //найти минимальное значение (самое левое)
    Node* findMin(Node* node) {
        if (node->left) {
            return findMin(node->left);
        }
        return node;
    }


    Node* removeMin(Node* node) {
        // идем в лево до тех пор пока не найдем минимальную
        if (!node->left) {  // если левой больше нет, заменяем ячейку на правое поддерево
            return node->right;
        }
        node->left = removeMin(node->left);
        return balance(node);
    }

    Node* remove(Node* node, T key) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else { // ключ совпадает, мы в нужной ячейке
            Node* left = node->left;
            Node* right = node->right;
            delete node;

            // подставляем вместо удаленной левое поддерево, если нет правого
            if (!right) return left;
            
            // если есть правое, то минимальную ячейку из него
            Node* min = findMin(right); 
            min->right = removeMin(right);
            min->left = left;

            return balance(min);
        } // балансируем каждую ячейку в которой оказались
        return balance(node);
    }

    // содержится ли ячейка со значением key
    bool contains(Node* node, T key) {
        if (!node) return false;
        if (key < node->key) { // идем в лево
            return contains(node->left, key);
        } else if (key >node->key) { // идем в право 
            return contains(node->right, key);
        }
        return true;
    }

    void printInOrder(Node* node) {
        if (!node) return;

        printInOrder(node->left);

        std::cout << node->key << " "; // делаем вывод в виде (левые ячейки) (корень) (правые ячейки)

        printInOrder(node->right);
    }

    // очищаем ячейку
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};